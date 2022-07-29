#include "Arduino.h"
#include "pwm.h"
#include "bsp_api.h"

static uint32_t pclkd_freq_hz = 0;

PwmOut::PwmOut(pin_size_t pinNumber) : 
  _pin(pinNumber),
  _pwm_idx(0),
  _enabled(false)
{

}

PwmOut::~PwmOut() {
}

bool PwmOut:: begin(uint32_t period_width, uint32_t pulse_width, bool raw) {

  pinPeripheral(digitalPinToBspPin(_pin), (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_GPT1);

  if (digitalPinToPwmPin(_pin) == NOT_ON_PWM) {
    //Return error if pin is not a PWM
    return false;
  }
  _pwm_idx = digitalPinToPwmPin(_pin);
  int err = R_GPT_Open(pwmTable[_pwm_idx].gpt_ctrl, pwmTable[_pwm_idx].gpt_cfg);
  if ((err != FSP_ERR_ALREADY_OPEN) && (err != FSP_SUCCESS)) {
    return false;
  }

  if (raw) {
    if (!period_raw(period_width)) {
      return false;
    }

    if (!pulseWidth_raw(pulse_width)) {
      return false;
    }
  } else {
    if (!period(period_width)) {
      return false;
    }

    if (!pulseWidth(pulse_width)) {
      return false;
    }
  }

  if (R_GPT_Start(pwmTable[_pwm_idx].gpt_ctrl)) {
    return false;
  }

  _enabled = true;
  return true;
}

bool PwmOut::period(int ms) {
  pclkd_freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKD) >> pwmTable[_pwm_idx].gpt_cfg->source_div;
  uint32_t period_counts_ms = (uint32_t) ((uint64_t) pclkd_freq_hz/1000 * ms);
  if (R_GPT_PeriodSet(pwmTable[_pwm_idx].gpt_ctrl, period_counts_ms) != FSP_SUCCESS) {
    return false;
  }
  return true;
}

bool PwmOut::pulseWidth(int ms) {
  uint32_t pulse_counts_ms = (uint32_t) ((uint64_t) pclkd_freq_hz/1000 * ms);
  if (R_GPT_DutyCycleSet(pwmTable[_pwm_idx].gpt_ctrl, pulse_counts_ms, pwmTable[_pwm_idx].gpt_pin) != FSP_SUCCESS) {
    return false;
  }
  return true;
}

bool PwmOut::period_us(int us) {
  pclkd_freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKD) >> pwmTable[_pwm_idx].gpt_cfg->source_div;
  uint32_t period_counts_us = (uint32_t) ((uint64_t) pclkd_freq_hz/1000000 * us);
  if (R_GPT_PeriodSet(pwmTable[_pwm_idx].gpt_ctrl, period_counts_us) != FSP_SUCCESS) {
    return false;
  }
  return true;
}

bool PwmOut::pulseWidth_us(int us) {
  uint32_t pulse_counts_us = (uint32_t) ((uint64_t) pclkd_freq_hz/1000000 * us);
  if (R_GPT_DutyCycleSet(pwmTable[_pwm_idx].gpt_ctrl, pulse_counts_us, pwmTable[_pwm_idx].gpt_pin) != FSP_SUCCESS) {
    return false;
  }
  return true;
}

bool PwmOut::period_raw(int period) {
  if (R_GPT_PeriodSet(pwmTable[_pwm_idx].gpt_ctrl, period) != FSP_SUCCESS) {
    return false;
  }
  return true;
}

bool PwmOut::pulseWidth_raw(int pulse) {
  if (R_GPT_DutyCycleSet(pwmTable[_pwm_idx].gpt_ctrl, pulse, pwmTable[_pwm_idx].gpt_pin) != FSP_SUCCESS) {
    return false;
  }
  return true;
}

void PwmOut::suspend() {
  if (_enabled) {
    R_GPT_Stop(pwmTable[_pwm_idx].gpt_ctrl);
    _enabled = false;
  }
}

void PwmOut::resume() {
  if (!_enabled) {
    R_GPT_Start(pwmTable[_pwm_idx].gpt_ctrl);
    _enabled = true;
  }
}

void PwmOut::end() {
  R_GPT_Reset(pwmTable[_pwm_idx].gpt_ctrl);
  _enabled = false;
}