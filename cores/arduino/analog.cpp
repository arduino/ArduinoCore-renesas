#include "Arduino.h"

#include "bsp_api.h"
#include "hal_data.h"

static int _writeResolution = 8;

int analogRead(pin_size_t pinNumber)
{
  static bool begin = false;
  pin_size_t adc_idx = digitalPinToAnalogPin(pinNumber);

  pinPeripheral(digitalPinToBspPin(pinNumber), (uint32_t) IOPORT_CFG_ANALOG_ENABLE);

	if(begin == false)
	{
	    R_ADC_Open(g_AAnalogPinDescription[adc_idx].adc_ctrl, g_AAnalogPinDescription[adc_idx].adc_cfg);
	    begin = true;
	}

  //Clear Scan Mask
  adc_channel_cfg_t adc_channel_cfg = g_adc0_channel_cfg;
  adc_channel_cfg.scan_mask = 0;
  //Enable scan only for the current pin
  adc_channel_cfg.scan_mask |= (1 << g_AAnalogPinDescription[adc_idx].ch);
	R_ADC_ScanCfg(g_AAnalogPinDescription[adc_idx].adc_ctrl, &adc_channel_cfg);
  R_ADC_ScanStart(g_AAnalogPinDescription[adc_idx].adc_ctrl);
  adc_status_t status;
  status.state = ADC_STATE_SCAN_IN_PROGRESS;
  while (ADC_STATE_SCAN_IN_PROGRESS == status.state)
  {
      R_ADC_StatusGet(g_AAnalogPinDescription[adc_idx].adc_ctrl, &status);
  }

  uint16_t result;
  R_ADC_Read(g_AAnalogPinDescription[adc_idx].adc_ctrl, g_AAnalogPinDescription[adc_idx].ch, &result);
  return (int)result;

}

void analogWriteResolution(int bits) {
  _writeResolution = bits;
}

int getAnalogWriteResolution() {
  return _writeResolution;
}


void analogWrite(pin_size_t pinNumber, int value)
{
  uint32_t pwmPeriod = 0;
  uint32_t pulse_width = 0;
  if (g_APinDescription[pinNumber].PWMChannel != NOT_ON_PWM) {
    //PWM pin
    PwmOut* pwm = digitalPinToPwmObj(pinNumber);
    if (pwm == NULL) {
      pwm = new PwmOut(pinNumber);
      digitalPinToPwmObj(pinNumber) = pwm;
      pwmPeriod = pwmTable[digitalPinToPwmPin(pinNumber)].gpt_cfg->period_counts;
      pulse_width = pwmPeriod*value/pow(2,_writeResolution);
      pwm->begin(pwmPeriod, pulse_width, true);
    } else {
      pwmPeriod = pwmTable[digitalPinToPwmPin(pinNumber)].gpt_cfg->period_counts;
      pulse_width = pwmPeriod*value/pow(2,_writeResolution);
      pwm->period_raw(pwmPeriod);
      pwm->pulseWidth_raw(pulse_width);
    }
  } else {
#ifdef DAC
    //DAC pin
    //Check if it is a valid DAC pin
    if (((pinNumber - DAC) >= 0) && ((pinNumber - DAC) < NUM_ANALOG_OUTPUTS)) {
      //R_DAC_Stop(g_AAnalogOutPinDescription[pinNumber - DAC].dac_ctrl);
      R_DAC_Open(g_AAnalogOutPinDescription[pinNumber - DAC].dac_ctrl, g_AAnalogOutPinDescription[pinNumber - DAC].dac_cfg);
      if (value > 4096) {
        // Saturate to MAX ADC value
        value = 4096;
      }
      R_DAC_Write(g_AAnalogOutPinDescription[pinNumber - DAC].dac_ctrl, value);
      R_DAC_Start(g_AAnalogOutPinDescription[pinNumber - DAC].dac_ctrl);
    }
#endif //DAC
  }
}


int getAnalogReadResolution()
{
  int read_resolution = 0;
  switch (g_adc0_cfg.resolution)
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
