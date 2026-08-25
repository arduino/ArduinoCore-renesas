#include "Arduino.h"
#include "pwm.h"
#include "bsp_api.h"

PwmOut::PwmOut(int pinNumber) :
  _pin(pinNumber),
  _enabled(false)
{

}

PwmOut::~PwmOut() {
}

bool PwmOut::cfg_pin(int max_index) {
  /* verify index are good */
  if(_pin < 0 || _pin >= max_index) {
    return false;
  }
  /* getting configuration from table */
  auto pin_cgf = getPinCfgs(_pin, PIN_CFG_REQ_PWM);

  /* verify configuration are good */
  if(pin_cgf[0] == 0) {
    return false;
  }

  timer_channel = GET_CHANNEL(pin_cgf[0]);

  _is_agt = IS_PIN_AGT_PWM(pin_cgf[0]);

  _pwm_channel = IS_PWM_ON_A(pin_cgf[0]) ? CHANNEL_A : CHANNEL_B;

  /* actually configuring PIN function */
  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[_pin].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | (_is_agt ? IOPORT_PERIPHERAL_AGT : IOPORT_PERIPHERAL_GPT1)));
  return true;

}

/* default begin function, starts the timers with default pwm configuration (490Hz and 50%) */
bool PwmOut::begin() {
  bool rv = true;
  int max_index = PINS_COUNT;
  rv &= cfg_pin(max_index);

  if(rv) {
    /* extended PWM CFG*/
    if(_is_agt) {
      rv &= timer.begin_pwm(AGT_TIMER, timer_channel, _pwm_channel);
    }
    else {
      rv &= timer.begin_pwm(GPT_TIMER, timer_channel, _pwm_channel);
    }
  }
  _enabled = rv;
  return rv;

}



/* begin "standard", period and pulse when raw is false are supposed to be expressed in usec */
/* -------------------------------------------------------------------------- */
bool PwmOut::begin(uint32_t period_width, uint32_t pulse_width, bool raw /*= false */, timer_source_div_t sd /*= TIMER_SOURCE_DIV_1*/) {
/* -------------------------------------------------------------------------- */
  _enabled = true;
  int max_index = PINS_COUNT;
  _enabled &= cfg_pin(max_index);
  
  if(_enabled) {
    if(raw) {
      _enabled &= timer.begin(TIMER_MODE_PWM, (_is_agt) ? AGT_TIMER : GPT_TIMER, timer_channel,  period_width, pulse_width, sd);
    }
    else {
      /* NOTE: I suppose period and pulse are expressed in us */
      float freq_hz = (1000000.0 / (float)period_width);
      float duty_perc = ((float)pulse_width * 100.0 / (float)period_width);
      _enabled &= timer.begin(TIMER_MODE_PWM, (_is_agt) ? AGT_TIMER : GPT_TIMER, timer_channel, freq_hz, duty_perc);
    }

    timer.add_pwm_extended_cfg();
    timer.enable_pwm_channel(_pwm_channel); 
    
  }
  if(_enabled) {
    _enabled &= timer.open();
    _enabled &= timer.start();
  }
  
  return _enabled;
}

/* -------------------------------------------------------------------------- */
bool PwmOut::begin(float freq_hz, float duty_perc) {
/* -------------------------------------------------------------------------- */
  _enabled = true;
  int max_index = PINS_COUNT;
  _enabled &= cfg_pin(max_index);

  if(_enabled) {
    _enabled &= timer.begin(TIMER_MODE_PWM, (_is_agt) ? AGT_TIMER : GPT_TIMER, timer_channel, freq_hz, duty_perc);
  }

  if(_enabled) {
    timer.add_pwm_extended_cfg();
    timer.enable_pwm_channel(_pwm_channel);

    _enabled &= timer.open();
    _enabled &= timer.start();
  }

  return _enabled;
}

bool PwmOut::period(int ms) {
  return timer.set_period_ms((double)ms);
}

bool PwmOut::pulseWidth(int ms) {
  return timer.set_pulse_ms((double)ms,_pwm_channel);  
}

bool PwmOut::period_us(int us) {
  return timer.set_period_us((double)us);
}

bool PwmOut::pulseWidth_us(int us) {
  return timer.set_pulse_us((double)us,_pwm_channel);
}

bool PwmOut::period_raw(int period) {
  return timer.set_period(period);
}

bool PwmOut::pulseWidth_raw(int pulse) {
  return timer.set_duty_cycle(pulse, _pwm_channel); 
}

bool PwmOut::pulse_perc(float duty) {
  float period = (float)timer.get_period_raw();
  float pulse = period * duty / 100.0;
  return pulseWidth_raw((int)pulse);
}


void PwmOut::suspend() {
  if (_enabled) {
    timer.stop();
    _enabled = false;
  }
}

void PwmOut::resume() {
  if (!_enabled) {
    timer.start();
    _enabled = true;
  }
}

void PwmOut::end() {
  timer.end();
  _enabled = false;
}