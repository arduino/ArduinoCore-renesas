#include "Arduino.h" 

volatile unsigned long agt_time_us = 0;
uint32_t pclkb_freq_hz = 0;

void delay(uint32_t ms) {
#ifdef AZURE_RTOS_THREADX
	tx_thread_sleep(ms);
#else
	R_BSP_SoftwareDelay(ms, BSP_DELAY_UNITS_MILLISECONDS);
#endif
}

void delayMicroseconds(unsigned int us) {
	R_BSP_SoftwareDelay(us, BSP_DELAY_UNITS_MICROSECONDS);
}

void yield() {
#ifdef AZURE_RTOS_THREADX
	tx_thread_relinquish();
#endif
}

void startAgt() {
	R_AGT_Open(&g_timer10_ctrl, &g_timer10_cfg);
	R_AGT_Start(&g_timer10_ctrl);
  pclkb_freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB) >> g_timer10_cfg.source_div;
}

void timer10_callback(timer_callback_args_t *p_args) {
	agt_time_us += 10000; //10ms
}

unsigned long millis()
{
#ifdef AZURE_RTOS_THREADX
	return tx_time_get();
#else
	timer_status_t status;
	R_AGT_StatusGet(&g_timer10_ctrl, &status);
	// Convert time to ms
	uint32_t time_ms = (status.counter*1000)/pclkb_freq_hz + agt_time_us/1000;
	return time_ms;
#endif
}

unsigned long micros() {
	timer_status_t status;
	R_AGT_StatusGet(&g_timer10_ctrl, &status);
	// Convert time to us
	uint32_t time_us = (status.counter*1000000)/pclkb_freq_hz + agt_time_us;
	return time_us;
}

void setRtcTime(rtc_time_t time) {
	time.tm_year -= 1900; // Start date 1900
	R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
	R_RTC_CalendarTimeSet(&g_rtc0_ctrl, &time);
}

rtc_time_t getRtcTime() {
	rtc_time_t time_read;
	R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
	R_RTC_CalendarTimeGet(&g_rtc0_ctrl, &time_read);
	time_read.tm_year += 1900; // Start date 1900
	return time_read;
}

void setRtcPeriodicInterrupt(rtc_periodic_irq_select_t period, void (* func)(rtc_callback_args_t *)) {
	R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
	g_rtc0_ctrl.p_callback = func;
	R_RTC_PeriodicIrqRateSet(&g_rtc0_ctrl, period);
}

void setRtcAlarm(rtc_time_t time, RtcAlarmSettings time_match, void (* func)(rtc_callback_args_t *)) {
	R_RTC_Open(&g_rtc0_ctrl, &g_rtc0_cfg);
	g_rtc0_ctrl.p_callback = func;
	rtc_alarm_time_t alarm_time;
	alarm_time.time = time;
	memcpy(&alarm_time.sec_match, &time_match, sizeof(RtcAlarmSettings));
	R_RTC_CalendarAlarmSet(&g_rtc0_ctrl, &alarm_time);
}

void __attribute__((weak)) rtc_callback(rtc_callback_args_t *p_args) {}
