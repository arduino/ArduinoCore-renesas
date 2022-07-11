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
  if (g_AAnalogOutPinDescription[pinNumber - DAC].ch == 0) {
    R_DAC_Open(&g_dac0_ctrl, &g_dac0_cfg);
    R_DAC_Start(&g_dac0_ctrl);
    R_DAC_Write(&g_dac0_ctrl, value);
  }
#if NUM_ANALOG_OUTPUTS > 1
   else if (g_AAnalogOutPinDescription[pinNumber - DAC].ch == 1) {
    R_DAC_Open(&g_dac1_ctrl, &g_dac1_cfg);
    R_DAC_Start(&g_dac1_ctrl);
    R_DAC_Write(&g_dac1_ctrl, value);
  }
#endif
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
