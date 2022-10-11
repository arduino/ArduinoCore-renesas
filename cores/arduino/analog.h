#ifndef ARDUINO_ANALOG_H
#define ARDUINO_ANALOG_H

#include "Arduino.h"

#include "r_adc_api.h"
#include "r_adc.h"
#include "bsp_api.h"



using ADCCbk_f = void (*)(adc_callback_args_t * p_args);

using ADCIrqCbk_f = void (*)();

using ADCIrqType_t = enum {
  ADC_IRQ_SCAN_END,
  ADC_IRQ_SCAN_END_B,
  ADC_IRQ_WIN_CMP_A,
  ADC_IRQ_WIN_CMP_B
};

class ADC_Container {
public:
  ADC_Container(uint8_t unit, ADCCbk_f cbk, void *ctx = nullptr);
  adc_instance_ctrl_t ctrl;
  adc_extended_cfg_t cfg_extend;
  adc_cfg_t cfg;
  adc_channel_cfg_t channel_cfg;
};




bool attachAnalogInterrupt(pin_size_t pinNumber, ADCIrqCbk_f cbk, ADCIrqType_t type, uint8_t priority = 12);
bool attachAnalogInterrupt(bsp_io_port_pin_t g, ADCIrqCbk_f cbk, ADCIrqType_t type, uint8_t priority = 12);

int analogRead(bsp_io_port_pin_t pinNumber);


#endif