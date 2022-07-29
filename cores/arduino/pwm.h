#include "Arduino.h"

#ifndef __ARDUINO_PWM_H__
#define __ARDUINO_PWM_H__

class PwmOut {
  public:
    PwmOut(pin_size_t pinNumber);
    ~PwmOut();

    bool begin(uint32_t period_width, uint32_t pulse_width, bool raw = false);
    void end();
    bool period(int ms);
    bool pulseWidth(int ms);
    bool period_us(int us);
    bool pulseWidth_us(int us);
    bool period_raw(int period);
    bool pulseWidth_raw(int pulse);
    void suspend();
    void resume();

  private:
    bool _enabled;
    pin_size_t _pin;
    pin_size_t _pwm_idx;

    timer_cfg_t timer_cfg;
};

#endif //__ARDUINO_PWM_H__
