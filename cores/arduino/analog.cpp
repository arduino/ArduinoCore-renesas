#include "Arduino.h"
#include "pwm.h"
#include "analog.h"
#include "dac.h"
#include "bsp_api.h"
#include "hal_data.h"

#define MAX_ADC_CHANNELS   29

static int _privateGetHwAnalogResolution();
static int _analogRequestedReadResolution = 10;

static void ADC_irq_cbk(adc_callback_args_t * cb_data);
static uint16_t analog_values_by_channels[MAX_ADC_CHANNELS] = {0};

ADC_Container::ADC_Container(uint8_t unit, ADCCbk_f cbk, void *ctx /*= nullptr*/) {
  cfg_extend.add_average_count    = ADC_ADD_OFF;
  cfg_extend.clearing             = ADC_CLEAR_AFTER_READ_ON;
  cfg_extend.trigger_group_b      = ADC_TRIGGER_SYNC_ELC;
  cfg_extend.double_trigger_mode  = ADC_DOUBLE_TRIGGER_DISABLED;
  cfg_extend.adc_vref_control     = ADC_VREF_CONTROL_AVCC0_AVSS0;
  cfg_extend.enable_adbuf         = 0;
  cfg_extend.window_a_irq         = FSP_INVALID_VECTOR;
  cfg_extend.window_a_ipl         = (12);
  cfg_extend.window_b_irq         = FSP_INVALID_VECTOR;
  cfg_extend.window_b_ipl         = (12);

  cfg.unit                        = unit;
  cfg.mode                        = ADC_MODE_SINGLE_SCAN;
  /* 20221109 [maidnl] FIX Requested resolution: 
    Resolution of ADC is fixed to the highest possible and
    never changed, requested resolution is used to scale the values provided 
    to user */
  #if 12U == BSP_FEATURE_ADC_MAX_RESOLUTION_BITS
  cfg.resolution                  = ADC_RESOLUTION_12_BIT;
  #elif 14U == BSP_FEATURE_ADC_MAX_RESOLUTION_BITS
  cfg.resolution                  = ADC_RESOLUTION_14_BIT;
  #elif 16U == BSP_FEATURE_ADC_MAX_RESOLUTION_BITS
  cfg.resolution                  = ADC_RESOLUTION_16_BIT;
  #else
  #error BSP_FEATURE_ADC_MAX_RESOLUTION_BITS is set to an unandled version
  /* should never happen... but in any case 12 is the resolution supported by
     both ADC (R7FA4M1AB and R7FA6M5BH) */
  cfg.resolution                  = ADC_RESOLUTION_12_BIT;
  #endif
  cfg.alignment                   = (adc_alignment_t) ADC_ALIGNMENT_RIGHT;
  cfg.trigger                     = ADC_TRIGGER_SOFTWARE; 
  cfg.p_callback                  = cbk;
  cfg.p_context                   = ctx;
  cfg.p_extend                    = &cfg_extend;
  cfg.scan_end_irq                = FSP_INVALID_VECTOR;
  cfg.scan_end_ipl                = (12);
  cfg.scan_end_b_irq              = FSP_INVALID_VECTOR;
  cfg.scan_end_b_ipl              = (12);  

  channel_cfg.sample_hold_states  = 24;
  channel_cfg.scan_mask           = 0;          
  channel_cfg.scan_mask_group_b   = 0; 
  channel_cfg.add_mask            = 0;
  channel_cfg.p_window_cfg        = nullptr;
  channel_cfg.priority_group_a    = ADC_GROUP_A_PRIORITY_OFF;
  channel_cfg.sample_hold_mask    = 0;
};

ADC_Container::~ADC_Container() {
  if(channel_cfg.p_window_cfg != nullptr) {
    delete channel_cfg.p_window_cfg;
  }
}

static ADC_Container adc(0,ADC_irq_cbk);
static ADC_Container adc1(1,ADC_irq_cbk);

static ADCIrqCbk_f scan_complete_cbk = nullptr;
static ADCIrqCbk_f scan_complete_b_cbk = nullptr;
static ADCIrqCbk_f window_compare_a_cbk = nullptr;
static ADCIrqCbk_f window_compare_b_cbk = nullptr;


static void readAllGroupA(ADC_Container *_adc) {
  for(int i = 0; i < MAX_ADC_CHANNELS; i++) {
    if(_adc->channel_cfg.scan_mask & (1 << i)) {
      //is the channel active -> yes, read it
      R_ADC_Read(&(_adc->ctrl), (adc_channel_t)i, analog_values_by_channels + i);
    }
  }
}

static void readAllGroupB(ADC_Container *_adc) {
  for(int i = 0; i < MAX_ADC_CHANNELS; i++) {
    if(_adc->channel_cfg.scan_mask_group_b & (1 << i)) {
      //is the channel active -> yes, read it
      R_ADC_Read(&(_adc->ctrl), (adc_channel_t)i, analog_values_by_channels + i);
    }
  }
}


static void ADC_irq_cbk(adc_callback_args_t * cb_data) {
  if(cb_data->event == ADC_EVENT_SCAN_COMPLETE) {
    if(scan_complete_cbk != nullptr) {
      if(cb_data->unit == 0) {
        readAllGroupA(&adc);
      }
      else if(cb_data->unit == 1) {
        readAllGroupA(&adc1);
      }
      scan_complete_cbk(cb_data->unit);
    }
  }
  else if(cb_data->event == ADC_EVENT_SCAN_COMPLETE_GROUP_B) {
    if(scan_complete_b_cbk != nullptr) {
      if(cb_data->unit == 0) {
        readAllGroupB(&adc);
      }
      else if(cb_data->unit == 1) {
        readAllGroupB(&adc1);
      }
      scan_complete_b_cbk(cb_data->unit);
    }
  }
  else if(cb_data->event == ADC_EVENT_WINDOW_COMPARE_A) {
    if(window_compare_a_cbk != nullptr) {
      window_compare_a_cbk(cb_data->unit);
    }
  }
  else if(cb_data->event == ADC_EVENT_WINDOW_COMPARE_B) {
    if(window_compare_b_cbk != nullptr) {
      window_compare_b_cbk(cb_data->unit);
    }
  }
}





/* -------------------------------------------------------------------------- */
static ADC_Container *get_ADC_container_ptr(int32_t pin, uint16_t &cfg) {
/* -------------------------------------------------------------------------- */  
  ADC_Container *rv = nullptr;
  auto cfg_adc = getPinCfgs(pin, PIN_CFG_REQ_ADC);
  if(cfg_adc[0] > 0 ) {
    if(IS_ADC1(cfg_adc[0])) {
      rv = &adc1;
    }
    else {
      rv = &adc;
    }
  }
  cfg = cfg_adc[0];
  return rv;

}




/* -------------------------------------------------------------------------- */
/* ADD PIN TO COMPARE                                                         */
/* -------------------------------------------------------------------------- */

/* That function works on COMPARE A - more than 1 PIN is used at once */
/* -------------------------------------------------------------------------- */
static bool addPinToCompare(ADC_Container *_adc, bool lower_or_outside_wnd, uint8_t ch, bsp_io_port_pin_t pin) {
/* -------------------------------------------------------------------------- */  
  bool rv = false;
  if(_adc != nullptr) {
    if(_adc->channel_cfg.p_window_cfg == nullptr) {
      _adc->channel_cfg.p_window_cfg  = new adc_window_cfg_t;
    }
    if(_adc->channel_cfg.p_window_cfg != nullptr) {
      pinPeripheral(pin, (uint32_t) IOPORT_CFG_ANALOG_ENABLE);
      _adc->channel_cfg.p_window_cfg->compare_mask |= (1 << ch);
      _adc->channel_cfg.p_window_cfg->compare_mode_mask |= (lower_or_outside_wnd << ch);
    }
  }
  return rv;
}

/* -------------------------------------------------------------------------- */
bool analogAddPinToCompare(bsp_io_port_pin_t pin, bool lower_or_outside_wnd) {
/* -------------------------------------------------------------------------- */
  int32_t index = getPinIndex(pin);
  uint16_t cfg_adc = 0;
  ADC_Container *_adc = get_ADC_container_ptr(index, cfg_adc);
  
  return addPinToCompare(_adc, lower_or_outside_wnd, GET_CHANNEL(cfg_adc),pin);
}

/* -------------------------------------------------------------------------- */
bool analogAddPinToCompare(pin_size_t pinNumber, bool lower_or_outside_wnd) {
/* -------------------------------------------------------------------------- */  
  int32_t index = digitalPinToAnalogPin(pinNumber);
  uint16_t cfg_adc = 0;
  ADC_Container *_adc = get_ADC_container_ptr(index, cfg_adc);
  return addPinToCompare(_adc, lower_or_outside_wnd, GET_CHANNEL(cfg_adc),digitalPinToBspPin(index));
}



/* -------------------------------------------------------------------------- */
/* ADD PIN TO SCAN GROUP                                                      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
static bool addPinToGroup(ADC_Container *_adc, ScanGroup_t g, uint8_t ch, bsp_io_port_pin_t pin) {
/* -------------------------------------------------------------------------- */  
  bool rv = false;
  if(_adc != nullptr) {
    pinPeripheral(pin, (uint32_t) IOPORT_CFG_ANALOG_ENABLE);
    if(g == ADC_SCAN_GROUP_A) {
      _adc->channel_cfg.scan_mask |= (1 << ch);
      rv = true;
    }
    #ifdef USE_SCAN_GROUP_B
    else if(g == ADC_SCAN_GROUP_B) {
      _adc->channel_cfg.scan_mask_group_b |= (1 << ch);
      rv = true;
    }
    #endif
  }
  return rv;
}

/* -------------------------------------------------------------------------- */
bool analogAddPinToGroup(bsp_io_port_pin_t pin, ScanGroup_t g /*= ADC_SCAN_GROUP_A*/) {
/* -------------------------------------------------------------------------- */
  int32_t index = getPinIndex(pin);
  uint16_t cfg_adc = 0;
  ADC_Container *adc = get_ADC_container_ptr(index, cfg_adc);
  
  return addPinToGroup(adc, g, GET_CHANNEL(cfg_adc),pin);
}

/* -------------------------------------------------------------------------- */
bool analogAddPinToGroup(pin_size_t pinNumber, ScanGroup_t g /*= ADC_SCAN_GROUP_A*/) {
/* -------------------------------------------------------------------------- */  
  int32_t index = digitalPinToAnalogPin(pinNumber);
  uint16_t cfg_adc = 0;
  ADC_Container *adc = get_ADC_container_ptr(index, cfg_adc);
  return addPinToGroup(adc, g, GET_CHANNEL(cfg_adc),digitalPinToBspPin(index));
}


void analogStartScan() {
  if(adc.ctrl.opened)  {
    R_ADC_ScanStart(&(adc.ctrl));
  }
  if(adc1.ctrl.opened)  {
    R_ADC_ScanStart(&(adc1.ctrl));
  }
}


/* -------------------------------------------------------------------------- */
bool attachScanEndIrq(ADCIrqCbk_f cbk, adc_mode_t mode /*= ADC_MODE_SINGLE_SCAN*/, uint8_t priority /*= 12*/) {
/* -------------------------------------------------------------------------- */
  bool rv = true;
  
  if(adc.channel_cfg.scan_mask) {
    if(adc.ctrl.opened)
      R_ADC_Close(&(adc.ctrl));
    if(IRQManager::getInstance().addADCScanEnd(&adc)) {
        adc.cfg.scan_end_ipl = priority;
        scan_complete_cbk = cbk;
    }
    else {
      rv = false;
    }
    adc.cfg.mode = mode;
    
    R_ADC_Open(&(adc.ctrl), &(adc.cfg));
    R_ADC_ScanCfg(&(adc.ctrl), &(adc.channel_cfg));
    R_ADC_ScanStart(&(adc.ctrl));
  }
  
  if(adc1.channel_cfg.scan_mask) {
    if(adc1.ctrl.opened)
      R_ADC_Close(&(adc1.ctrl));
    if(IRQManager::getInstance().addADCScanEnd(&adc1)) {
        adc1.cfg.scan_end_ipl = priority;
        scan_complete_cbk = cbk;
    }
    else {
      rv = false;
    }
    adc1.cfg.mode = mode;
    R_ADC_Open(&(adc1.ctrl), &(adc1.cfg));
    R_ADC_ScanCfg(&(adc1.ctrl), &(adc1.channel_cfg));
    R_ADC_ScanStart(&(adc1.ctrl));
  }

  return rv;
}

#ifdef USE_SCAN_GROUP_B

/* -------------------------------------------------------------------------- */
bool attachScanEndBIrq(ADCIrqCbk_f cbk, adc_mode_t mode /*= ADC_MODE_GROUP_SCAN*/, uint8_t priority) {
/* -------------------------------------------------------------------------- */
  bool rv = true;
  
  if(adc.channel_cfg.scan_mask_group_b) {
    if(adc.ctrl.opened)
      R_ADC_Close(&(adc.ctrl));
    if(IRQManager::getInstance().addADCScanEndB(&adc)) {
        adc.cfg.scan_end_ipl = priority;
        scan_complete_b_cbk = cbk;
    }
    else {
      rv = false;
    }
    adc.cfg.mode = mode;
    R_ADC_Open(&(adc.ctrl), &(adc.cfg));
    R_ADC_ScanCfg(&(adc.ctrl), &(adc.channel_cfg));
    adc.ctrl.p_reg->ADCSR   = (uint16_t) 8768;
    adc.ctrl.p_reg->ADSTRGR = (uint16_t) 2314;
    adc.ctrl.p_reg->ADGSPCR = 0;
    //R_ADC_ScanStart(&(adc.ctrl));
    //R_ADC_ScanGroupStart (&(adc.ctrl), ADC_GROUP_MASK_ALL );

  }
  
  if(adc1.channel_cfg.scan_mask_group_b) {
    if(adc1.ctrl.opened)
      R_ADC_Close(&(adc1.ctrl));
    if(IRQManager::getInstance().addADCScanEndB(&adc1)) {
        adc1.cfg.scan_end_ipl = priority;
        scan_complete_b_cbk = cbk;
    }
    else {
      rv = false;
    }
    adc1.cfg.mode = mode;
    R_ADC_Open(&(adc1.ctrl), &(adc1.cfg));
    R_ADC_ScanCfg(&(adc1.ctrl), &(adc1.channel_cfg));
    R_ADC_ScanStart(&(adc1.ctrl));
    //R_ADC_ScanGroupStart (&(adc1.ctrl), ADC_GROUP_MASK_ALL );
  }

  return rv;
}

#endif

bool analogAttachIrqCompareA(uint16_t low_th, uint16_t high_th, bool enable_window, ADCIrqCbk_f cbk, adc_mode_t mode /*= ADC_MODE_GROUP_SCAN*/, uint8_t priority /* = 12 */){
  bool rv = true;
  if(adc.channel_cfg.p_window_cfg != nullptr) {
    if(adc.channel_cfg.p_window_cfg->compare_mask) {
      if(adc.ctrl.opened)
        R_ADC_Close(&(adc.ctrl));
      adc.channel_cfg.p_window_cfg->compare_ref_low = low_th;
      adc.channel_cfg.p_window_cfg->compare_ref_high = high_th;
      adc.channel_cfg.p_window_cfg->compare_cfg = static_cast<adc_compare_cfg_t>(adc.channel_cfg.p_window_cfg->compare_cfg |  ADC_COMPARE_CFG_A_ENABLE);
      if(enable_window) {
        adc.channel_cfg.p_window_cfg->compare_cfg = static_cast<adc_compare_cfg_t>(adc.channel_cfg.p_window_cfg->compare_cfg |  ADC_COMPARE_CFG_WINDOW_ENABLE);
      }
      if(IRQManager::getInstance().addADCWinCmpA(&adc)) {
        ((adc_extended_cfg_t *)(adc.cfg.p_extend))->window_a_ipl = priority;
        window_compare_a_cbk = cbk;
      }
      adc.cfg.mode = mode;
      R_ADC_Open(&(adc.ctrl), &(adc.cfg));
      R_ADC_ScanCfg(&(adc.ctrl), &(adc.channel_cfg));
      R_ADC_ScanStart(&(adc.ctrl));
    }
    else {
      rv = false;
    }
  }
  else {
    rv = false;
  }

  if(adc1.channel_cfg.p_window_cfg != nullptr) {
    if(adc1.channel_cfg.p_window_cfg->compare_mask) {
      if(adc1.ctrl.opened)
        R_ADC_Close(&(adc1.ctrl));
      adc1.channel_cfg.p_window_cfg->compare_ref_low = low_th;
      adc1.channel_cfg.p_window_cfg->compare_ref_high = high_th;
      adc1.channel_cfg.p_window_cfg->compare_cfg = static_cast<adc_compare_cfg_t>(adc1.channel_cfg.p_window_cfg->compare_cfg |  ADC_COMPARE_CFG_A_ENABLE);
      if(enable_window) {
        adc1.channel_cfg.p_window_cfg->compare_cfg = static_cast<adc_compare_cfg_t>(adc1.channel_cfg.p_window_cfg->compare_cfg |  ADC_COMPARE_CFG_WINDOW_ENABLE);
      }
      if(IRQManager::getInstance().addADCWinCmpA(&adc)) {
        ((adc_extended_cfg_t *)(adc1.cfg.p_extend))->window_a_ipl = priority;
        window_compare_a_cbk = cbk;
      }
      adc1.cfg.mode = mode;
      R_ADC_Open(&(adc1.ctrl), &(adc1.cfg));
      R_ADC_ScanCfg(&(adc1.ctrl), &(adc1.channel_cfg));
      R_ADC_ScanStart(&(adc1.ctrl));
    }
    else {
      rv = false;
    }
  }
  else {
    rv = false;
  }

  return rv;

}

static bool attachIrqCompareB(ADC_Container *_adc, uint8_t ch, bool lower_or_outside_wnd, uint16_t low_th, uint16_t high_th, bool enable_window, ADCIrqCbk_f cbk, adc_mode_t mode, uint8_t priority) {
  bool rv = true;
  if(_adc->channel_cfg.p_window_cfg == nullptr) {
      _adc->channel_cfg.p_window_cfg  = new adc_window_cfg_t;
  }

  if(_adc->channel_cfg.p_window_cfg != nullptr) {
    if(_adc->ctrl.opened)
      R_ADC_Close(&(_adc->ctrl));
    
    _adc->channel_cfg.p_window_cfg->compare_b_ref_low = low_th;
    _adc->channel_cfg.p_window_cfg->compare_b_ref_high = high_th;
    _adc->channel_cfg.p_window_cfg->compare_b_channel = (adc_window_b_channel_t)ch;
    if(lower_or_outside_wnd) {
      _adc->channel_cfg.p_window_cfg->compare_b_mode = ADC_WINDOW_B_MODE_LESS_THAN_OR_OUTSIDE;
    }
    else {
      _adc->channel_cfg.p_window_cfg->compare_b_mode = ADC_WINDOW_B_MODE_GREATER_THAN_OR_INSIDE;
    }
    _adc->channel_cfg.p_window_cfg->compare_cfg = static_cast<adc_compare_cfg_t>(_adc->channel_cfg.p_window_cfg->compare_cfg |  ADC_COMPARE_CFG_B_ENABLE);
    if(enable_window) {
      _adc->channel_cfg.p_window_cfg->compare_cfg = static_cast<adc_compare_cfg_t>(_adc->channel_cfg.p_window_cfg->compare_cfg |  ADC_COMPARE_CFG_WINDOW_ENABLE);
    }
    if(IRQManager::getInstance().addADCWinCmpB(_adc)) {
        ((adc_extended_cfg_t *)(_adc->cfg.p_extend))->window_b_ipl = priority;
        window_compare_b_cbk = cbk;
    }
    else {
      rv = false;
    }
    _adc->cfg.mode = mode;
    R_ADC_Open(&(_adc->ctrl), &(_adc->cfg));
    R_ADC_ScanCfg(&(_adc->ctrl), &(_adc->channel_cfg));
    R_ADC_ScanStart(&(_adc->ctrl));
  }
  else {
    rv = false;
  }
  return rv;
}


bool analogAttachIrqCompareB(pin_size_t pinNumber, bool lower_or_outside_wnd, uint16_t low_th, uint16_t high_th, bool enable_window, ADCIrqCbk_f cbk,adc_mode_t mode , uint8_t priority /* = 12 */) {
  int32_t index = digitalPinToAnalogPin(pinNumber);
  uint16_t cfg_adc = 0;
  ADC_Container *_adc = get_ADC_container_ptr(index, cfg_adc);
  pinPeripheral(digitalPinToBspPin(index), (uint32_t) IOPORT_CFG_ANALOG_ENABLE);
  return attachIrqCompareB(_adc,GET_CHANNEL(cfg_adc),lower_or_outside_wnd,low_th, high_th,enable_window, cbk, mode, priority);
}

bool analogAttachIrqCompareB(bsp_io_port_pin_t pin, bool lower_or_outside_wnd, uint16_t low_th, uint16_t high_th, bool enable_window, ADCIrqCbk_f cbk, adc_mode_t mode ,uint8_t priority /* = 12 */) {
  int32_t index = getPinIndex(pin);
  uint16_t cfg_adc = 0;
  ADC_Container *_adc = get_ADC_container_ptr(index, cfg_adc);
  pinPeripheral(pin, (uint32_t) IOPORT_CFG_ANALOG_ENABLE);
  return attachIrqCompareB(_adc,GET_CHANNEL(cfg_adc),lower_or_outside_wnd,low_th, high_th,enable_window, cbk, mode, priority );
}

/* --------------------------------------------------------------------------
                      SINGLE CHANNEL CONVERSION
   -------------------------------------------------------------------------- */




/* -------------------------------------------------------------------------- */
static int adcConvert(ADC_Container *_adc,uint16_t cfg_adc) {
/* -------------------------------------------------------------------------- */  
  if(_adc != nullptr) {
    _adc->cfg.mode = ADC_MODE_SINGLE_SCAN;
    //Enable scan only for the current pin
    // USE | (OR) to keep previous configuration (like for IRQ conversion)
    _adc->channel_cfg.scan_mask |= (1 << GET_CHANNEL(cfg_adc));
    
    R_ADC_Open(&(_adc->ctrl), &(_adc->cfg));
    R_ADC_ScanCfg(&(_adc->ctrl), &(_adc->channel_cfg));
    R_ADC_ScanStart(&(_adc->ctrl));

    adc_status_t status;
    status.state = ADC_STATE_SCAN_IN_PROGRESS;
    while (ADC_STATE_SCAN_IN_PROGRESS == status.state)
    {
      R_ADC_StatusGet(&(_adc->ctrl), &status);
    }

    uint16_t result;
    R_ADC_Read(&(_adc->ctrl), (adc_channel_t)GET_CHANNEL(cfg_adc), &result);
    
    result = map(result, 0, (1 << _privateGetHwAnalogResolution()), 0, (1 << _analogRequestedReadResolution));
    return (int)result;
  }
  else {
    return -1;
  }
}



/* -------------------------------------------------------------------------- */
/* FSP PIN NUMBER */
int analogRead(bsp_io_port_pin_t pin) {
/* -------------------------------------------------------------------------- */
  int32_t index = getPinIndex(pin);
  uint16_t cfg_adc = 0;
  ADC_Container *_adc = get_ADC_container_ptr(index, cfg_adc);
  pinPeripheral(pin, (uint32_t) IOPORT_CFG_ANALOG_ENABLE);
  
  return adcConvert(_adc,cfg_adc);
}

/* -------------------------------------------------------------------------- */
/* LEGACY PIN NUMBER */
int analogRead(pin_size_t pinNumber) {
/* -------------------------------------------------------------------------- */  
  int32_t adc_idx = digitalPinToAnalogPin(pinNumber);
  uint16_t cfg_adc = 0;
  ADC_Container *_adc = get_ADC_container_ptr(adc_idx, cfg_adc);
  pinPeripheral(digitalPinToBspPin(adc_idx), (uint32_t) IOPORT_CFG_ANALOG_ENABLE);

  return adcConvert(_adc,cfg_adc);
}

void analogReference(uint8_t mode) {

 adc_vref_control_t ctrl;

  switch (mode) {
      case AR_INTERNAL:
        ctrl = ADC_VREF_CONTROL_IVREF_AVSS0; break;
      case AR_INTERNAL_1_5V:
        ctrl = ADC_VREF_CONTROL_1_5V_OUTPUT; break;
      case AR_INTERNAL_2_0V:
        ctrl = ADC_VREF_CONTROL_2_0V_OUTPUT; break;
      case AR_INTERNAL_2_5V:
        ctrl = ADC_VREF_CONTROL_2_5V_OUTPUT; break;
      case AR_EXTERNAL:
        ctrl = ADC_VREF_CONTROL_VREFH0_AVSS0; break;
      default:
        ctrl = ADC_VREF_CONTROL_AVCC0_AVSS0; break;
  }

  if(adc.ctrl.opened) {
    R_ADC_Close(&adc.ctrl);
  }
  adc.cfg_extend.adc_vref_control = ctrl;
  R_ADC_Open(&adc.ctrl, &adc.cfg);

  if(adc1.ctrl.opened) {
    R_ADC_Close(&adc1.ctrl);
  }
  adc1.cfg_extend.adc_vref_control = ctrl;
  R_ADC_Open(&adc1.ctrl, &adc1.cfg);
}

#if defined(AVCC_MEASURE_PIN)
static float aref = 0;
#endif

float analogReference() {
    switch (adc.cfg_extend.adc_vref_control) {
      case ADC_VREF_CONTROL_1_5V_OUTPUT:
        return 1.5;
      case ADC_VREF_CONTROL_2_0V_OUTPUT:
        return 2.0;
      case ADC_VREF_CONTROL_2_5V_OUTPUT:
        return 2.5;
      case ADC_VREF_CONTROL_VREFH0_AVSS0:
        // the user must know the voltage he applies from outside
        return NAN;
#ifdef AR_INTERNAL_VOLTAGE
      case ADC_VREF_CONTROL_IVREF_AVSS0:
        return AR_INTERNAL_VOLTAGE;
#endif
      default:
        #if defined(AVCC_MEASURE_PIN)
        if (aref == 0) {
          analogReference(AR_INTERNAL);
          delayMicroseconds(5);
          for (int i = 0; i < 10; i++) {
            aref += analogRead(AVCC_MEASURE_PIN) * AR_INTERNAL_VOLTAGE * AVCC_MULTIPLY_FACTOR / (1 << _analogRequestedReadResolution);
          }
          aref = aref / 10;
          analogReference(AR_DEFAULT);
        }
        return aref;
        #else
        return NAN;
        #endif
    }
}

void analogReadResolution(int bits) {
  /* 20221109 [maidnl] FIX Requested resolution: 
     Requested read resolution never changes the actual ADC resolution which is
     fixed by default to the higest possible */
  #define ANALOG_READ_HARDWARE_RESOLUTION_FIXED
  #ifdef ANALOG_READ_HARDWARE_RESOLUTION_FIXED
  if(bits == 8 || bits == 10 || bits == 12 || bits == 14 || bits == 16) {
    _analogRequestedReadResolution = bits; 
  }
  else {
    /* use a "strange value" to signal something went wrong */
    _analogRequestedReadResolution = 0; 
  }
  #else
  /* Keep this code that presume R_ADC_Open returns an error when the requested
     resolution is not set to the supported value (requested changes made to Renesas) */

  adc_resolution_t old_read_resolution = adc.cfg.resolution;
  adc_resolution_t old1_read_resolution = adc1.cfg.resolution;

  switch (bits) {
    case 10:
      _analogRequestedReadResolution = 10; 
      adc.cfg.resolution = ADC_RESOLUTION_10_BIT;
      adc1.cfg.resolution = ADC_RESOLUTION_10_BIT;
      break;
    case 8:
      _analogRequestedReadResolution = 8; 
      adc.cfg.resolution = ADC_RESOLUTION_8_BIT;
      adc1.cfg.resolution = ADC_RESOLUTION_8_BIT;
      break;
    case 14:
      _analogRequestedReadResolution = 14; 
      adc.cfg.resolution = ADC_RESOLUTION_14_BIT;
      adc1.cfg.resolution = ADC_RESOLUTION_14_BIT;
      break;
    case 16:
      _analogRequestedReadResolution = 16; 
      adc.cfg.resolution = ADC_RESOLUTION_16_BIT;
      adc1.cfg.resolution = ADC_RESOLUTION_16_BIT;
      break;
    case 24:
      _analogRequestedReadResolution = 24; 
      adc.cfg.resolution = ADC_RESOLUTION_24_BIT;
      adc1.cfg.resolution = ADC_RESOLUTION_24_BIT;
      break;
    case 12:
    default:
      _analogRequestedReadResolution = 12; 
      adc.cfg.resolution = ADC_RESOLUTION_12_BIT;
      adc1.cfg.resolution = ADC_RESOLUTION_12_BIT;
      break;
  }

  if(adc.ctrl.opened) {
    R_ADC_Close(&adc.ctrl);
  }
  auto res = R_ADC_Open(&adc.ctrl, &adc.cfg);
  if (res != FSP_SUCCESS) {
    adc.cfg.resolution = old_read_resolution;
  }  

  if(adc1.ctrl.opened) {
    R_ADC_Close(&adc1.ctrl);
  }
  res = R_ADC_Open(&adc1.ctrl, &adc1.cfg);
  if (res != FSP_SUCCESS) {
    adc1.cfg.resolution = old1_read_resolution;
  }
  #endif
}


int _privateGetHwAnalogResolution() {
  int read_resolution = 0;
  switch (adc.cfg.resolution)
  {
  case ADC_RESOLUTION_12_BIT:
    read_resolution = 12;
    break;
  case ADC_RESOLUTION_10_BIT:
    read_resolution = 10;
    break;
  case ADC_RESOLUTION_8_BIT:
    read_resolution = 8;
    break;
  case ADC_RESOLUTION_14_BIT:
    read_resolution = 14;
    break;
  case ADC_RESOLUTION_16_BIT:
    read_resolution = 16;
    break;
  case ADC_RESOLUTION_24_BIT:
    read_resolution = 24;
    break;
  default:
    read_resolution = 0;
    break;
  }
  return read_resolution;
}


int analogReadResolution()
{
  /* 20221109 [maidnl] FIX Requested resolution: 
     always return requested resolution - for user*/
  
  #ifdef ANALOG_READ_HARDWARE_RESOLUTION_FIXED
  return _analogRequestedReadResolution;
  #else
  return _privateGetHwAnalogResolution();
  #endif
}

/* 20221109 [maidnl] - default resolution must be fixed at 8 
   (backward compatibility with UNO rev.3) */
static int _writeResolution = 8;

void analogWriteResolution(int bits) {
  _writeResolution = bits;
}

int analogWriteResolution() {
  return _writeResolution;
}

#define DEFAULT_PERIOD_MSEC    (2)
#define DEFAULT_RAW_PERIOD_32  (0x17ea7) /* 490 Hz ~ 0.0020408125 seconds */
#define DEFAULT_RAW_PERIOD_16  (0x5fa9)  /* 490 Hz ~ 0.00204075 seconds */
#define DEFAULT_SOURCE_DIV_32  (0)
#define DEFAULT_SOURCE_DIV_16  (2)

class PwmArray {
  public:
  PwmArray() {}
  PwmOut *get(int pinNumber) {
    size_t i = 0;
    while (i < last_index) {
      if (chans[i] == pinNumber) {
        return pwm_outs[i];
      }
      i++;
    }
    return nullptr;
  }
  PwmOut *get_from_channel(int channel) {
    size_t i = 0;
    while (i < last_index) {
      if (pwm_outs[i]->get_timer()->get_cfg()->channel == channel) {
        return pwm_outs[i];
      }
      i++;
    }
    return nullptr;
  }
  bool set(int pinNumber, PwmOut *p) {
    if (last_index < sizeof(chans)/sizeof(chans[0]) && p != nullptr) {
      chans[last_index] = pinNumber;
      pwm_outs[last_index] = p;
      last_index++;
      return true;
    }
    return false;
  }
  private:
  size_t last_index = 0;
  pin_size_t chans[(GPT_HOWMANY+AGT_HOWMANY)*2] = {0};
  PwmOut* pwm_outs[(GPT_HOWMANY+AGT_HOWMANY)*2] = {nullptr};
};



static PwmArray pwms{};

void analogWrite(pin_size_t pinNumber, int value)
{
#if (DAC12_HOWMANY > 0) || (DAC8_HOWMANY > 0)
  if (IS_DAC(pinNumber)) {
    auto cfg_dac = getPinCfgs(pinNumber, PIN_CFG_REQ_DAC);
    if(IS_DAC_8BIT(cfg_dac[0])) {
      #if DAC8_HOWMANY > 0
      if(GET_CHANNEL(cfg_dac[0]) < DAC8_HOWMANY) {
        _dac8[GET_CHANNEL(cfg_dac[0])].analogWrite(value);
      }
      #endif
    }
    else {
      if(GET_CHANNEL(cfg_dac[0]) < DAC12_HOWMANY) {
        _dac12[GET_CHANNEL(cfg_dac[0])].analogWrite(value);
      }
    }
    return;
  }
#endif

  PwmOut *ptr = pwms.get(pinNumber);

  /* verify if PwmOut has already been instantiated */
  if (ptr == nullptr) {
    /* if not instatiate it */
    ptr = new PwmOut(pinNumber);
    bool added = pwms.set(pinNumber, ptr);
    if(!added || !ptr->begin()) {
      delete ptr;
      ptr = nullptr;
    }
  }

  if(ptr != nullptr) {
    //check the pinmux in case it's been modified by a call to pinMode()
    bool has_peripheral_mux = R_PFS->PORT[g_pin_cfg[pinNumber].pin >> IOPORT_PRV_PORT_OFFSET].PIN[g_pin_cfg[pinNumber].pin & BSP_IO_PRV_8BIT_MASK].PmnPFS & IOPORT_CFG_PERIPHERAL_PIN;
    if (!has_peripheral_mux) {
      ptr->end();
      ptr->begin();
    }
  }

  if(ptr != nullptr) {
    ptr->pulse_perc((float)value * 100.0 / (1 << _writeResolution));
  }
}

FspTimer* __get_timer_for_channel(int channel) {
  return  pwms.get_from_channel(channel)->get_timer();
}
