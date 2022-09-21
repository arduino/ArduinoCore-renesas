#include "Arduino.h"

#include "bsp_api.h"
#include "hal_data.h"

class ADC_Container {
public:
  ADC_Container() {
    cfg_extend.clearing = ADC_CLEAR_AFTER_READ_ON;
    cfg_extend.trigger_group_b = ADC_TRIGGER_SYNC_ELC;
    cfg_extend.adc_vref_control = ADC_VREF_CONTROL_AVCC0_AVSS0;
    cfg.mode = ADC_MODE_SINGLE_SCAN;
    cfg.resolution = ADC_RESOLUTION_14_BIT;
    cfg.p_extend = &cfg_extend;
    channel_cfg.sample_hold_states = 24;
  };
  adc_instance_ctrl_t ctrl;
  adc_extended_cfg_t cfg_extend;
  adc_cfg_t cfg;
  adc_channel_cfg_t channel_cfg;
};

static ADC_Container adc;

int analogRead(pin_size_t pinNumber)
{
  static bool begin = false;
  pin_size_t adc_idx = digitalPinToAnalogPin(pinNumber);

  pinPeripheral(digitalPinToBspPin(adc_idx), (uint32_t) IOPORT_CFG_ANALOG_ENABLE);

	if(begin == false)
	{
	    R_ADC_Open(&adc.ctrl, &adc.cfg);
	    begin = true;
	}

  const uint16_t *cfg = g_pin_cfg[adc_idx].list;
  uint16_t cfg_adc = getPinCfg(cfg, PIN_CFG_REQ_ADC);

  //Enable scan only for the current pin
  adc.channel_cfg.scan_mask = (1 << GET_CHANNEL(cfg_adc));
	R_ADC_ScanCfg(&adc.ctrl, &adc.channel_cfg);
  R_ADC_ScanStart(&adc.ctrl);

  adc_status_t status;
  status.state = ADC_STATE_SCAN_IN_PROGRESS;
  while (ADC_STATE_SCAN_IN_PROGRESS == status.state)
  {
    R_ADC_StatusGet(&adc.ctrl, &status);
  }

  uint16_t result;
  R_ADC_Read(&adc.ctrl, (adc_channel_t)GET_CHANNEL(cfg_adc), &result);
  return (int)result;
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

  adc.cfg.resolution = read_resolution;
  R_ADC_Open(&adc.ctrl, &adc.cfg);
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
