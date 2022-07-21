#ifndef __ARDUINO__H__
#define __ARDUINO__H__

#ifdef AZURE_RTOS_THREADX
#include "usb/SerialUSB.h"
#include "tx_api.h"
#endif
#include "api/ArduinoAPI.h"
#include "bsp_api.h"
#include "r_ioport.h"
#include "pwm.h"
#include "Serial.h"

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

typedef struct rtc_alarm_time
{
    bool       sec_match;              ///< Enable the alarm based on a match of the seconds field
    bool       min_match;              ///< Enable the alarm based on a match of the minutes field
    bool       hour_match;             ///< Enable the alarm based on a match of the hours field
    bool       mday_match;             ///< Enable the alarm based on a match of the days field
    bool       mon_match;              ///< Enable the alarm based on a match of the months field
    bool       year_match;             ///< Enable the alarm based on a match of the years field
    bool       dayofweek_match;        ///< Enable the alarm based on a match of the dayofweek field
} RtcAlarmSettings;

void setRtcTime(rtc_time_t time);
rtc_time_t getRtcTime();
void onRtcInterrupt(void (*func)(rtc_callback_args_t *));
void setRtcPeriodicInterrupt(rtc_periodic_irq_select_t period);
void setRtcAlarm(rtc_time_t time, RtcAlarmSettings time_match);
bool isRtcRunning();


// Definitions for PWM channels
typedef enum _EPWMChannel
{
  NOT_ON_PWM=-1,
  PWM_TIM0_CHA=0,
  PWM_TIM0_CHB=1,
  PWM_TIM1_CHA=2,
  PWM_TIM1_CHB=3,
  PWM_TIM2_CHA=4,
  PWM_TIM2_CHB=5,
  PWM_TIM3_CHA=6,
  PWM_TIM3_CHB=7,
  PWM_TIM4_CHA=6,
  PWM_TIM4_CHB=7,
  PWM_TIM5_CHA=6,
  PWM_TIM5_CHB=7,
  PWM_TIM6_CHA=6,
  PWM_TIM6_CHB=7,
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

typedef struct _PinDescription
{
  bsp_io_port_pin_t name;
  EPWMChannel       PWMChannel;
  EExt_Interrupts   ExtInt;
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


typedef struct {
    gpt_instance_ctrl_t* gpt_ctrl;
    const timer_cfg_t* gpt_cfg;
    gpt_io_pin_t gpt_pin;
    PwmOut* pwm;
} pwmTable_t;

typedef struct {
    icu_instance_ctrl_t* icu_ctrl;
    const external_irq_cfg_t* irq_cfg;
} irqTable_t;

extern PinDescription g_APinDescription[];
extern const AnalogPinDescription g_AAnalogPinDescription[];
extern const AnalogOutPinDescription g_AAnalogOutPinDescription[];
extern pwmTable_t pwmTable[];
extern const irqTable_t irqTable[];

#define digitalPinToBspPin(P)       (g_APinDescription[P].name)
#define digitalPinToAnalogPin(P)    (P >= PINS_COUNT ? -1 : P < A0 ? P : (P-A0))
#define digitalPinToInterruptPin(P) (g_APinDescription[P].ExtInt)
#define digitalPinToPwmPin(P)       (g_APinDescription[P].PWMChannel)
#define digitalPinToPwmObj(P)       (pwmTable[digitalPinToPwmPin(P)].pwm)

void pinPeripheral(bsp_io_port_pin_t bspPin, uint32_t bspPeripheral);
void pinPeripheral(uint32_t pinNumber, uint32_t bspPeripheral);

#define Serial1 _UART1_
#define Serial2 _UART2_
#define Serial3 _UART3_
#define Serial4 _UART4_
#define Serial5 _UART5_

#include "pins_arduino.h"

#endif //__ARDUINO__H__