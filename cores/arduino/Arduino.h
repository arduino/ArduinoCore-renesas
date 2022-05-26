#ifndef __ARDUINO__H__
#define __ARDUINO__H__

#include "usb/SerialUSB.h"
#include "api/ArduinoAPI.h"
#include "bsp_api.h"
#include "r_ioport.h"
#include "tx_api.h"
#include "pwm.h"

#if defined(__cplusplus)

#undef F
// C++11 F replacement declaration
template <typename T1>
auto F(T1&& A)
  -> const arduino::__FlashStringHelper*
{
  return (const arduino::__FlashStringHelper*)A;
}

#endif //__cplusplus

#define interrupts()        __enable_irq()
#define noInterrupts()      __disable_irq()

// We provide analogReadResolution and analogWriteResolution APIs
int getAnalogReadResolution();
void analogReadResolution(int bits);
void analogWriteResolution(int bits);

typedef struct _PinDescription
{
  bsp_io_port_pin_t name;
} PinDescription ;


typedef struct _AnalogPinDescription
{
  bsp_io_port_pin_t name;
  adc_channel_t ch;
} AnalogPinDescription;

typedef struct _AnalogOutPinDescription
{
  bsp_io_port_pin_t name;
  int ch;
} AnalogOutPinDescription;


typedef struct {
    gpt_instance_ctrl_t* gpt_ctrl;
    const timer_cfg_t* gpt_cfg;
    gpt_io_pin_t gpt_pin;
} pwmTable_t;

extern const PinDescription g_APinDescription[];
extern const AnalogPinDescription g_AAnalogPinDescription[];
extern const AnalogOutPinDescription g_AAnalogOutPinDescription[];
extern const pwmTable_t pwmTable[];

#include "pins_arduino.h"

#endif //__ARDUINO__H__