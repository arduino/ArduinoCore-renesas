#include "Arduino.h" 
#include "IRQManager.h"

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

static agt_instance_ctrl_t agt_ctrl;
static timer_cfg_t agt_cfg;
static agt_extended_cfg_t agt_extend;


void startAgt() {
	agt_extend.count_source = AGT_CLOCK_PCLKB;
	agt_extend.agto = AGT_PIN_CFG_DISABLED;
	agt_extend.agtoab_settings_b.agtoa = AGT_PIN_CFG_DISABLED;
	agt_extend.agtoab_settings_b.agtob = AGT_PIN_CFG_DISABLED;
	agt_extend.measurement_mode = AGT_MEASURE_DISABLED;
	agt_extend.agtio_filter = AGT_AGTIO_FILTER_NONE;
	agt_extend.enable_pin = AGT_ENABLE_PIN_NOT_USED;
	agt_extend.trigger_edge = AGT_TRIGGER_EDGE_RISING;

	agt_cfg.mode = TIMER_MODE_PERIODIC;
	agt_cfg.period_counts = (uint32_t) 0x7530;/* Actual period: 0.01 seconds. Actual duty: 50%. */
	agt_cfg.duty_cycle_counts = 0x3a98;
	agt_cfg.source_div = (timer_source_div_t) 3;
	agt_cfg.channel = 0;
	agt_cfg.p_callback = timer10_callback;
	agt_cfg.p_context = NULL;
	agt_cfg.p_extend = &agt_extend;
	agt_cfg.cycle_end_ipl = (12);
	IRQManager::getInstance().addPeripheral(IRQ_AGT,(void*)&agt_cfg);
	R_AGT_Open(&agt_ctrl, &agt_cfg);
	R_AGT_Start(&agt_ctrl);
  pclkb_freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB) >> agt_cfg.source_div;
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
	R_AGT_StatusGet(&agt_ctrl, &status);
	// Convert time to ms
	uint32_t time_ms = (status.counter*1000)/pclkb_freq_hz + agt_time_us/1000;
	return time_ms;
#endif
}

unsigned long micros() {
	timer_status_t status;
	R_AGT_StatusGet(&agt_ctrl, &status);
	// Convert time to us
	uint32_t time_us = (status.counter*1000000)/pclkb_freq_hz + agt_time_us;
	return time_us;
}
