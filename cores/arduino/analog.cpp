#include "Arduino.h"

#include "bsp_api.h"
#include "hal_data.h"

int analogRead(pin_size_t pinNumber)
{
	static bool begin = false;

	if(begin == false)
	{
	    R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
	    R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
	    begin = true;
	}

  (void) R_ADC_ScanStart(&g_adc0_ctrl);
  adc_status_t status;
  status.state = ADC_STATE_SCAN_IN_PROGRESS;
  while (ADC_STATE_SCAN_IN_PROGRESS == status.state)
  {
      (void) R_ADC_StatusGet(&g_adc0_ctrl, &status);
  }

  uint16_t result;
  R_ADC_Read(&g_adc0_ctrl, g_AAnalogPinDescription[pinNumber - A0].ch, &result);
  return (int)result;

}


void analogWrite(pin_size_t pinNumber, int value)
{
  if (g_APinDescription[pinNumber].PWMChannel != NOT_ON_PWM) {
    //PWM pin
    uint32_t pulse_width = (value*100)/65536;
    PwmOut* pwm = digitalPinToPwmObj(pinNumber);
    if (pwm == NULL) {
      pwm = new PwmOut(pinNumber);
      digitalPinToPwmObj(pinNumber) = pwm;
      //pwmTable[digitalPinToPwmPin(pinNumber)].pwm = pwm;
      //Start a PWM with 100ms period
      pwm->begin(100, pulse_width);
    } else {
      pwm->suspend();
      pwm->pulseWidth_us(pulse_width);
      pwm->period_us(100);
      pwm->resume();
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
