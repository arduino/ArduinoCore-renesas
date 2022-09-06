#ifndef __ARDUINO__H__
#define __ARDUINO__H__

#ifdef AZURE_RTOS_THREADX
#include "tx_api.h"
#endif

#include "api/ArduinoAPI.h"
#include "bsp_api.h"
#include "hal_data.h"


#if defined(__cplusplus)

using namespace arduino;

#include "usb/SerialUSB.h"
#include "pwm.h"
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
int getAnalogWriteResolution();
ioport_peripheral_t getPinConfig(bsp_io_port_pin_t pin);

#if defined(__cplusplus)
using rtc_simple_cbk_t = void (*)();
bool openRtc();
bool setRtcTime(rtc_time_t time);
bool getRtcTime(struct tm &t);
void onRtcInterrupt();
bool setRtcPeriodicInterrupt(rtc_periodic_irq_select_t period);
bool setRtcAlarm(rtc_alarm_time_t alarm_time);
bool isRtcRunning();
void setRtcPeriodicClbk(rtc_simple_cbk_t f);
void setRtcAlarmClbk(rtc_simple_cbk_t f);
#endif

// Definitions for PWM channels
typedef enum _EPWMChannel
{
  NOT_ON_PWM=-1,
  PWM_TIM0_CHA=0,
  PWM_TIM0_CHB=1,
  PWM_TIM1_CHA,
  PWM_TIM1_CHB,
  PWM_TIM2_CHA,
  PWM_TIM2_CHB,
  PWM_TIM3_CHA,
  PWM_TIM3_CHB,
  PWM_TIM4_CHA,
  PWM_TIM4_CHB,
  PWM_TIM5_CHA,
  PWM_TIM5_CHB,
  PWM_TIM6_CHA,
  PWM_TIM6_CHB,
} EPWMChannel ;

typedef enum
{
  NOT_AN_INTERRUPT = -1,
  EXTERNAL_INT_0 = 0,
  EXTERNAL_INT_1,
  EXTERNAL_INT_2,
  EXTERNAL_INT_3,
  EXTERNAL_INT_4,
  EXTERNAL_INT_5,
  EXTERNAL_INT_6,
  EXTERNAL_INT_7,
  EXTERNAL_INT_8,
  EXTERNAL_INT_NONE = NOT_AN_INTERRUPT,
} EExt_Interrupts ;

typedef enum
{
  NOT_A_BUS = -1,
  WIRE_BUS = 0,
  WIRE1_BUS,
  WIRE2_BUS,
  SERIAL_BUS,
  SERIAL1_BUS,
  SERIAL2_BUS,
  SERIAL3_BUS,
  SERIAL4_BUS,
  SERIAL5_BUS,
  SPI_BUS,
  SPI1_BUS,
} EPeripheralBus ;

typedef struct _PinDescription
{
  bsp_io_port_pin_t name;
  EPWMChannel       PWMChannel;
  EExt_Interrupts   ExtInt;
  EPeripheralBus    PeripheralConfig;
  ioport_peripheral_t pinCfg;
} PinDescription ;


typedef struct _AnalogPinDescription
{
  adc_ctrl_t* adc_ctrl;
  const adc_cfg_t* adc_cfg;
  adc_channel_t ch;
} AnalogPinDescription;

typedef struct _AnalogOutPinDescription
{
  dac_ctrl_t* dac_ctrl;
  const dac_cfg_t* dac_cfg;
} AnalogOutPinDescription;

#if defined(__cplusplus)
typedef struct {
    gpt_instance_ctrl_t* gpt_ctrl;
    const timer_cfg_t* gpt_cfg;
    gpt_io_pin_t gpt_pin;
    PwmOut* pwm;
} pwmTable_t;

extern pwmTable_t pwmTable[];
#endif

typedef struct {
    icu_instance_ctrl_t* icu_ctrl;
    const external_irq_cfg_t* irq_cfg;
} irqTable_t;

typedef struct {
    uart_instance_t *uart_instance;
    i2c_master_instance_t *i2c_instance;
    spi_instance_t *spi_instance;
} sciTable_t;

extern const PinDescription g_APinDescription[];
extern const AnalogPinDescription g_AAnalogPinDescription[];
extern const AnalogOutPinDescription g_AAnalogOutPinDescription[];
extern const irqTable_t irqTable[];

extern uart_instance_t UartTable[];
extern i2c_master_instance_t I2CMasterTable[];
extern spi_instance_t SpiTable[];
extern sciTable_t SciTable[];

#define digitalPinToBspPin(P)       (g_APinDescription[P].name)
#define digitalPinToAnalogPin(P)    (P >= PINS_COUNT ? -1 : P < A0 ? P : (P-A0))
#define digitalPinToInterruptPin(P) (g_APinDescription[P].ExtInt)
#define digitalPinToPwmPin(P)       (g_APinDescription[P].PWMChannel)
#define digitalPinToPwmObj(P)       (pwmTable[digitalPinToPwmPin(P)].pwm)

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

#define Serial1 _UART1_
#define Serial2 _UART2_
#define Serial3 _UART3_
#define Serial4 _UART4_
#define Serial5 _UART5_

#include "pins_arduino.h"

#endif //__ARDUINO__H__