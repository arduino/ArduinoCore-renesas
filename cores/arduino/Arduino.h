#ifndef __ARDUINO__H__
#define __ARDUINO__H__

#ifdef AZURE_RTOS_THREADX
#include "tx_api.h"
#endif



#include "api/ArduinoAPI.h"
#include "bsp_api.h"
#include "hal_data.h"


#include "variant.h"

#if defined(__cplusplus)

using namespace arduino;

#include "usb/SerialUSB.h"

#include "Serial.h"
#endif

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
#if defined(__cplusplus)
// In c++ mode, we also provide analogReadResolution and analogWriteResolution getters
int analogReadResolution();
int analogWriteResolution();
#endif
int getAnalogWriteResolution();
ioport_peripheral_t getPinConfig(bsp_io_port_pin_t pin);

#if defined(__cplusplus)
extern "C" {
#endif
extern const PinMuxCfg_t g_pin_cfg[];
extern const size_t g_pin_cfg_size;
#if defined(__cplusplus)
}
#endif

#define digitalPinToBspPin(P)       (g_pin_cfg[P].pin)
#define digitalPinToAnalogPin(P)    (P < A0 ? A0 + P : P)

#define IOPORT_PRV_PORT_ADDRESS(port_number)    ((uint32_t) (R_PORT1 - R_PORT0) * (port_number) + R_PORT0)

#define digitalPinToPort(P)		      (digitalPinToBspPin(P) >> 8)
#define digitalPinToBitMask(P)      (1 << (digitalPinToBspPin(P) & 0xFF))
#define portOutputRegister(port)    &(((R_PORT0_Type *)IOPORT_PRV_PORT_ADDRESS(port))->PODR)
#define portInputRegister(port)     &(((R_PORT0_Type *)IOPORT_PRV_PORT_ADDRESS(port))->PIDR)
#define portModeRegister(port)      &(((R_PORT0_Type *)IOPORT_PRV_PORT_ADDRESS(port))->PDR)

void pinPeripheral(bsp_io_port_pin_t bspPin, uint32_t bspPeripheral);
#if defined(__cplusplus)
void pinPeripheral(uint32_t pinNumber, uint32_t bspPeripheral);
#endif

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define Serial  SerialUSB
#define Serial1 _UART1_
#define Serial2 _UART2_
#define Serial3 _UART3_
#define Serial4 _UART4_
#define Serial5 _UART5_

#include "pins_arduino.h"

#endif //__ARDUINO__H__
