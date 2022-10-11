


#include "Arduino.h"
#include "IRQManager.h"
#include "pwm.h"
#include "analog.h"
#include "bsp_api.h"
#include "hal_data.h"



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
  cfg.resolution                  = ADC_RESOLUTION_14_BIT;
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
};

static ADCIrqCbk_f scan_complete_unit0_cbk = nullptr;
static ADCIrqCbk_f scan_complete_b_unit0_cbk = nullptr;
static ADCIrqCbk_f conversion_complete_unit0_cbk = nullptr;
static ADCIrqCbk_f window_compare_a_unit0_cbk = nullptr;
static ADCIrqCbk_f window_compare_b_unit0_cbk = nullptr;

static ADCIrqCbk_f scan_complete_unit1_cbk = nullptr;
static ADCIrqCbk_f scan_complete_b_unit1_cbk = nullptr;
static ADCIrqCbk_f conversion_complete_unit1_cbk = nullptr;
static ADCIrqCbk_f window_compare_a_unit1_cbk = nullptr;
static ADCIrqCbk_f window_compare_b_unit1_cbk = nullptr;


static void ADC_irq_cbk(adc_callback_args_t * cb_data) {
  uint16_t unit = cb_data->unit;
  adc_event_t event = cb_data->event;

  if(cb_data->unit == 0) {
    if(cb_data->event == ADC_EVENT_SCAN_COMPLETE) {
      if(scan_complete_unit0_cbk != nullptr) {
        scan_complete_unit0_cbk();
      }
    }
    else if(cb_data->event == ADC_EVENT_SCAN_COMPLETE_GROUP_B) {
      if(scan_complete_b_unit0_cbk != nullptr) {
        scan_complete_b_unit0_cbk();
      }

    }
    else if(cb_data->event == ADC_EVENT_CONVERSION_COMPLETE) {
      if(conversion_complete_unit0_cbk != nullptr) {
        conversion_complete_unit0_cbk();
      }
    }
    else if(cb_data->event == ADC_EVENT_WINDOW_COMPARE_A) {
      if(window_compare_a_unit0_cbk != nullptr) {
        window_compare_a_unit0_cbk();
      }
    }
    else if(cb_data->event == ADC_EVENT_WINDOW_COMPARE_B) {
      if(window_compare_b_unit0_cbk != nullptr) {
        window_compare_b_unit0_cbk();
      }
    }
  }
  else if(cb_data->unit == 1) {
    if(scan_complete_unit1_cbk != nullptr) {
      scan_complete_unit1_cbk();
    }
    else if(cb_data->event == ADC_EVENT_SCAN_COMPLETE_GROUP_B) {
      if(scan_complete_b_unit1_cbk != nullptr) {
        scan_complete_b_unit1_cbk();
      }

    }
    else if(cb_data->event == ADC_EVENT_CONVERSION_COMPLETE) {
      if(conversion_complete_unit1_cbk != nullptr) {
        conversion_complete_unit1_cbk();
      }
    }
    else if(cb_data->event == ADC_EVENT_WINDOW_COMPARE_A) {
      if(window_compare_a_unit1_cbk != nullptr) {
        window_compare_a_unit1_cbk();
      }
    }
    else if(cb_data->event == ADC_EVENT_WINDOW_COMPARE_B) {
      if(window_compare_b_unit1_cbk != nullptr) {
        window_compare_b_unit1_cbk();
      }
    }
  }
}


static ADC_Container adc(0,ADC_irq_cbk);
static ADC_Container adc1(1,ADC_irq_cbk);

/* -------------------------------------------------------------------------- */
static ADC_Container *get_ADC_container_ptr(pin_size_t pin, uint16_t &cfg) {
/* -------------------------------------------------------------------------- */  
  ADC_Container *rv = nullptr;
  uint16_t cfg_adc = 0;
  if(pin >= 0) {
    const uint16_t *cfg = g_pin_cfg[pin].list;
    cfg_adc = getPinCfg(cfg, PIN_CFG_REQ_ADC);
  }
  if(cfg_adc > 0 ) {
    if(IS_ADC1(cfg_adc)) {
      rv = &adc;
    }
    else {
      rv = &adc1;
    }
  }
  cfg = cfg_adc;
  return rv;

}

/* -------------------------------------------------------------------------- */
static ADC_Container *get_ADC_container_ptr(bsp_io_port_pin_t pin, uint16_t &cfg) {
/* -------------------------------------------------------------------------- */  
  ADC_Container *rv = nullptr;
  int32_t index = getPinIndex(pin);
  return get_ADC_container_ptr((pin_size_t)index,cfg);
  
}

/* -------------------------------------------------------------------------- */
static int adcConvert(ADC_Container *adc,uint16_t cfg_adc) {
/* -------------------------------------------------------------------------- */  
  if(adc != nullptr) {
    //Enable scan only for the current pin
    adc->channel_cfg.scan_mask = (1 << GET_CHANNEL(cfg_adc));
    
    R_ADC_Open(&(adc->ctrl), &(adc->cfg));
    R_ADC_ScanCfg(&(adc->ctrl), &(adc->channel_cfg));
    R_ADC_ScanStart(&(adc->ctrl));

    adc_status_t status;
    status.state = ADC_STATE_SCAN_IN_PROGRESS;
    while (ADC_STATE_SCAN_IN_PROGRESS == status.state)
    {
      R_ADC_StatusGet(&(adc->ctrl), &status);
    }

    uint16_t result;
    R_ADC_Read(&(adc->ctrl), (adc_channel_t)GET_CHANNEL(cfg_adc), &result);
    return (int)result;
  }
  else {
    return -1;
  }
}

static bool adcAttachIrq(ADC_Container *adc, ADCIrqCbk_f cbk, ADCIrqType_t type, uint8_t priority){
  if(adc != nullptr) {
    bool was_opened = (adc->ctrl.opened) ? true : false;
    if(was_opened) {
      R_ADC_Close(&(adc->ctrl));
    }

    if(type == ADC_IRQ_SCAN_END) {
      if(IRQManager::getInstance().addADCScanEnd(adc)) {
        adc->cfg.scan_end_ipl = priority;
        if(adc->cfg.unit == 0) {
          scan_complete_unit0_cbk = cbk;
        } 
        else if(adc->cfg.unit == 1) {
          scan_complete_unit1_cbk = cbk;
        }
      }
    }
    else if(type == ADC_IRQ_SCAN_END_B) {
      if(IRQManager::getInstance().addADCScanEndB(adc)) {
        adc->cfg.scan_end_b_ipl = priority;
        if(adc->cfg.unit == 0) {
          scan_complete_b_unit0_cbk = cbk;
        } 
        else if(adc->cfg.unit == 1) {
          scan_complete_b_unit1_cbk = cbk;
        }
      }

    }
    else if(type == ADC_IRQ_WIN_CMP_A) {
      if(IRQManager::getInstance().addADCWinCmpA(adc)) {
        ((adc_extended_cfg_t *)(adc->cfg.p_extend))->window_a_ipl = priority;
        if(adc->cfg.unit == 0) {
          window_compare_a_unit0_cbk = cbk;
        } 
        else if(adc->cfg.unit == 1) {
          window_compare_a_unit1_cbk = cbk;
        }
      }

    }
    else if(type == ADC_IRQ_WIN_CMP_B) {

      if(IRQManager::getInstance().addADCWinCmpB(adc)) {
        ((adc_extended_cfg_t *)(adc->cfg.p_extend))->window_b_ipl = priority;
        if(adc->cfg.unit == 0) {
          window_compare_b_unit0_cbk = cbk;
        } 
        else if(adc->cfg.unit == 1) {
          window_compare_b_unit1_cbk = cbk;
        }
      }
    }

    if(was_opened) {
      R_ADC_Open(&(adc->ctrl), &(adc->cfg));
    }
  }
  else {
    return false;
  }

}


bool attachAnalogInterrupt(pin_size_t pinNumber, ADCIrqCbk_f cbk, ADCIrqType_t type, uint8_t priority /*= 12*/) {
  pin_size_t adc_idx = digitalPinToAnalogPin(pinNumber);
  uint16_t cfg_adc = 0;
  ADC_Container *adc = get_ADC_container_ptr(adc_idx, cfg_adc);
  return adcAttachIrq(adc, cbk, type, priority);
  

}

bool attachAnalogInterrupt(bsp_io_port_pin_t pinNumber, ADCIrqCbk_f cbk, ADCIrqType_t type, uint8_t priority /*= 12*/) {
  uint16_t cfg_adc = 0;
  ADC_Container *adc = get_ADC_container_ptr(pinNumber, cfg_adc);

  return adcAttachIrq(adc, cbk, type, priority);
}


/* -------------------------------------------------------------------------- */
/* FSP PIN NUMBER */
int analogRead(bsp_io_port_pin_t pinNumber) {
/* -------------------------------------------------------------------------- */
  uint16_t cfg_adc = 0;
  ADC_Container *adc = get_ADC_container_ptr(pinNumber, cfg_adc);

  pinPeripheral(pinNumber, (uint32_t) IOPORT_CFG_ANALOG_ENABLE);
  
  return adcConvert(adc,cfg_adc);
}


/* -------------------------------------------------------------------------- */
/* LEGACY PIN NUMBER */
int analogRead(pin_size_t pinNumber) {
/* -------------------------------------------------------------------------- */  
  pin_size_t adc_idx = digitalPinToAnalogPin(pinNumber);
  uint16_t cfg_adc = 0;
  ADC_Container *adc = get_ADC_container_ptr(adc_idx, cfg_adc);

  pinPeripheral(digitalPinToBspPin(adc_idx), (uint32_t) IOPORT_CFG_ANALOG_ENABLE);

  return adcConvert(adc,cfg_adc);
}

void analogReference(uint8_t mode) {
  // TODO: in case VREFH is selected, please configure the pin accordingly
  // something like
  // pinPeripheral(digitalPinToBspPin(VREFH), (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | VREFH)
  R_ADC_Close(&adc.ctrl);
  adc.cfg_extend.adc_vref_control = (adc_vref_control_t)mode;
  R_ADC_Open(&adc.ctrl, &adc.cfg);
}

void analogReadResolution(int bits) {
  R_ADC_Close(&adc.ctrl);

  adc_resolution_t read_resolution;

  switch (bits) {
    case 10:
      read_resolution = ADC_RESOLUTION_10_BIT;
      break;
    case 8:
      read_resolution = ADC_RESOLUTION_8_BIT;
      break;
    case 14:
      read_resolution = ADC_RESOLUTION_14_BIT;
      break;
    case 16:
      read_resolution = ADC_RESOLUTION_16_BIT;
      break;
    case 24:
      read_resolution = ADC_RESOLUTION_24_BIT;
      break;
    case 12:
    default:
      read_resolution = ADC_RESOLUTION_12_BIT;
      break;
  }

  auto res = R_ADC_Open(&adc.ctrl, &adc.cfg);
  if (res == FSP_SUCCESS) {
    adc.cfg.resolution = read_resolution;
  }
  // oops, we selected an impossible resolution; don't change it in the structure to avoid misbehaviours
}

int analogReadResolution()
{
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

static int _writeResolution = 12;

void analogWriteResolution(int bits) {
  _writeResolution = bits;
}

int analogWriteResolution() {
  return _writeResolution;
}

#ifdef DAC
class DAC_Container {
public:
  DAC_Container(int ch) {
    ext_cfg.enable_charge_pump = 0;
    ext_cfg.data_format = DAC_DATA_FORMAT_FLUSH_RIGHT;
    ext_cfg.output_amplifier_enabled = 0;
    ext_cfg.internal_output_enabled = false;
    cfg.p_extend = &ext_cfg;
    cfg.channel = ch;
  }
  dac_instance_ctrl_t ctrl;
  dac_extended_cfg_t ext_cfg;
  dac_cfg_t cfg;
};

static DAC_Container dac[] = {
  DAC_Container(0),
  #if NUM_ANALOG_OUTPUTS > 1
  DAC_Container(1),
  #endif
  #if NUM_ANALOG_OUTPUTS > 2
  DAC_Container(2),
  #endif
  #if NUM_ANALOG_OUTPUTS > 3
  DAC_Container(3),
  #endif
};

void analogWriteDAC(pin_size_t pinNumber, int value) {
  R_DAC_Open(&dac[pinNumber - DAC].ctrl, &dac[pinNumber - DAC].cfg);

  // DAC is 12 bit, so remap _writeResolution to (1 << 12)
  value = map(value, 0, (1 << _writeResolution), 0, (1 << 12));

  R_DAC_Write(&dac[pinNumber - DAC].ctrl, value);
  R_DAC_Start(&dac[pinNumber - DAC].ctrl);
}
#endif

#define MAX_PWM_ARDUINO_PIN    25
#define DEFAULT_PERIOD_MSEC    (2)
#define DEFAULT_RAW_PERIOD_32  (0x17ea7) /* 490 Hz ~ 0.0020408125 seconds */
#define DEFAULT_RAW_PERIOD_16  (0x5fa9)  /* 490 Hz ~ 0.00204075 seconds */
#define DEFAULT_SOURCE_DIV_32  (0)
#define DEFAULT_SOURCE_DIV_16  (2)

class PwmArray {
  public:
  PwmArray() {}
  ~PwmArray() {
    for(int i = 0; i < MAX_PWM_ARDUINO_PIN; i++) {
      if(pwm_outs[i] != nullptr){
        delete pwm_outs[i];
        pwm_outs[i] = nullptr;
      }
    }
  }
  PwmOut *get(int index) { 
    if(index < MAX_PWM_ARDUINO_PIN) {
      return pwm_outs[index]; 
    }
    else {
      return nullptr;
    }
  }
  bool set(int index, PwmOut *p) { 
    if(index < MAX_PWM_ARDUINO_PIN) {
      pwm_outs[index] = p;
      return true;
    }
    else {
      return false;
    }
  }
  private:
  PwmOut* pwm_outs[MAX_PWM_ARDUINO_PIN] = {nullptr};

};

static PwmArray pwms{};

void analogWrite(pin_size_t pinNumber, int value)
{
  uint32_t pwmPeriod = 0;

#ifdef DAC
  //Check if it is a valid DAC pin
  if (((pinNumber - DAC) >= 0) && ((pinNumber - DAC) < NUM_ANALOG_OUTPUTS)) {
    analogWriteDAC(pinNumber, value);
    return;
  }
#endif //DAC

  uint32_t pulse_width = pwmPeriod*value/pow(2,_writeResolution);

  if(pinNumber < MAX_PWM_ARDUINO_PIN) {
    PwmOut *ptr = nullptr;
    /* varify if PwmOut has already been instantiated */
    if(pwms.get(pinNumber) == nullptr) {
      /* if not instatiate it */
      ptr = new PwmOut(pinNumber);
      if(!pwms.set(pinNumber,ptr)) {
        delete ptr;
        ptr = nullptr;
      }
      else {
        if(!ptr->begin()) {
          delete ptr;
          ptr = nullptr;
          pwms.set(pinNumber,nullptr);
        }
      }
    }
    else {
      ptr = pwms.get(pinNumber);
    }

    if(ptr != nullptr) {
      ptr->pulse_perc( (float)value * 100.0 / (float)pow(2,_writeResolution)); 
    }
  }
}

