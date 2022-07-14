#include "Arduino.h"

#ifndef __ARDUINO_PWM_H__
#define __ARDUINO_PWM_H__

class PwmOut {
  public:
    PwmOut(pin_size_t pinNumber);
    ~PwmOut();

    bool begin(uint32_t period_width, uint32_t pulse_width);
    void end();
    bool period(int ms);
    bool pulseWidth(int ms);
    bool period_us(int us);
    bool pulseWidth_us(int us);
    uint32_t getPeriod();
    uint32_t getPulseWidth();
    void suspend();
    void resume();

  private:
    bool _enabled;
    pin_size_t _pin;
    pin_size_t _pwm_idx;
    uint32_t _period;
    uint32_t _period_us;
    uint32_t _pulse_width;
    uint32_t _pulse_width_us;
};

#endif //__ARDUINO_PWM_H__
