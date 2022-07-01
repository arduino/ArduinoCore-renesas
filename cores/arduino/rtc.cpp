#include "Arduino.h"

volatile bool rtc_open = false;

void setRtcTime(rtc_time_t time) {
  time.tm_year -= 1900; // Start date 1900
  if (!rtc_open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    rtc_open = true;
  }
  R_RTC_CalendarTimeSet(&g_rtc0_ctrl, &time);
}

rtc_time_t getRtcTime() {
  rtc_time_t time_read;
  if (!rtc_open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    rtc_open = true;
  }
  R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &time_read);
  time_read.tm_year += 1900; // Start date 1900
  return time_read;
}

void onRtcInterrupt(void (*func)(rtc_callback_args_t *)) {
  if (!rtc_open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    rtc_open = true;
  }
  g_rtc0_ctrl.p_callback = func;
}

void setRtcPeriodicInterrupt(rtc_periodic_irq_select_t period) {
  if (!rtc_open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    rtc_open = true;
  }
  R_RTC_PeriodicIrqRateSet(&g_rtc0_ctrl, period);
}

void setRtcAlarm(rtc_time_t time, RtcAlarmSettings time_match) {
  if (!rtc_open) {
    R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
    rtc_open = true;
  }
  rtc_alarm_time_t alarm_time;
  alarm_time.time = time;
  memcpy(&alarm_time.sec_match, &time_match, sizeof(RtcAlarmSettings));
  R_RTC_CalendarAlarmSet(&g_rtc0_ctrl, &alarm_time);
}

bool isRtcRunning() {
  rtc_info_t rtc_info;
  R_RTC_InfoGet(&g_rtc0_ctrl, &rtc_info);
  if (rtc_info.status == RTC_STATUS_RUNNING) {
    return true;
  }
  return false;
}

void __attribute__((weak)) rtc_callback(rtc_callback_args_t *p_args) {}