#ifndef __ARDUINO__H__
#define __ARDUINO__H__

#include "api/ArduinoAPI.h"
#include "bsp_api.h"
//#include "bsp_feature.h"

#include "variant.h"
#include "r_ioport_api.h"
#include "common_data.h"

#if defined(__cplusplus)

using namespace arduino;

#include "usb/SerialUSB.h"

#include "Serial.h"
#endif

#include <api/itoa.h>
#include <api/deprecated-avr-comp/avr/dtostrf.h>
#include <strings.h>

#if defined(__cplusplus)

#undef F
// C++11 F replacement declaration
template <typename T1>
auto F(T1&& A)
  -> const arduino::__FlashStringHelper*
{
  return (const arduino::__FlashStringHelper*)A;
}

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif // abs

#define abs(x) ((x)>0?(x):-(x))

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

pin_size_t digitalPinToInterrupt(pin_size_t pin);

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

// analog extra functionalities

typedef enum _ar_aref {
  AR_INTERNAL,
  AR_INTERNAL_1_5V,
  AR_INTERNAL_2_0V,
  AR_INTERNAL_2_5V,
  AR_EXTERNAL,
  AR_DEFAULT,
} ar_aref;

#if defined(__cplusplus)
float analogReference();
#endif

#ifndef NO_USB
#define Serial  SerialUSB
#define Serial1 _UART1_
#define Serial2 _UART2_
#define Serial3 _UART3_
#define Serial4 _UART4_
#define Serial5 _UART5_
#else
#define Serial _UART1_
#define Serial1 _UART2_
#define Serial2 _UART3_
#define Serial3 _UART4_
#define Serial4 _UART5_
#endif

#define IOPORT_PRV_PERIPHERAL_FUNCTION    (1U << 16)
#define IOPORT_PRV_PORT_OFFSET            (8U)
#define BSP_IO_PRV_8BIT_MASK              (0xFF)

#include "pins_arduino.h"
#include "usb/USB.h"

#endif //__ARDUINO__H__
