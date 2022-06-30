#include "Arduino.h"
#include "pwm.h"
#include "bsp_api.h"

static uint32_t pclkd_freq_hz = 0;

PwmOut::PwmOut(pin_size_t pinNumber) : 
  _pin(pinNumber),
  _period(0),
  _pulse_width(0),
  _enabled(false)
{

}

PwmOut::~PwmOut() {
}

bool PwmOut:: begin(uint32_t period_width, uint32_t pulse_width) {
  if (R_GPT_Open(pwmTable[_pin].gpt_ctrl, pwmTable[_pin].gpt_cfg)) {
    return false;
  }

  if (!period(period_width)) {
    return false;
  }

  if (!pulseWidth(pulse_width)) {
    return false;
  }

  if (R_GPT_Start(pwmTable[_pin].gpt_ctrl)) {
    return false;
  }

  _enabled = true;
  return true;
}

bool PwmOut::period(int ms) {
  pclkd_freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKD) >> pwmTable[_pin].gpt_cfg->source_div;
  uint32_t period_counts = (uint32_t) ((uint64_t) pclkd_freq_hz/1000 * ms);
  if (R_GPT_PeriodSet(pwmTable[_pin].gpt_ctrl, period_counts) != FSP_SUCCESS) {
    return false;
  }
  _period = ms;
  return true;
}

bool PwmOut::pulseWidth(int ms) {
  uint32_t pulse_counts = (uint32_t) ((uint64_t) pclkd_freq_hz/1000 * ms);
  if (R_GPT_DutyCycleSet(pwmTable[_pin].gpt_ctrl, pulse_counts, pwmTable[_pin].gpt_pin) != FSP_SUCCESS) {
    return false;
  }
  _pulse_width = ms;
  return true;
}

uint32_t PwmOut::getPeriod() {
  timer_info_t info;
  (void) R_GPT_InfoGet(pwmTable[_pin].gpt_ctrl, &info);
  uint32_t time_ms = info.period_counts/(pclkd_freq_hz/1000);
  return time_ms;
}

uint32_t PwmOut::getPulseWidth() {
  return _pulse_width;
}

void PwmOut::suspend() {
  if (_enabled) {
    R_GPT_Stop(pwmTable[_pin].gpt_ctrl);
    _enabled = false;
  }
}

void PwmOut::resume() {
  if (!_enabled) {
    R_GPT_Start(pwmTable[_pin].gpt_ctrl);
    _enabled = true;
  }
}

void PwmOut::end() {
  R_GPT_Reset(pwmTable[_pin].gpt_ctrl);
  _enabled = false;
}