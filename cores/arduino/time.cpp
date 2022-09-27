#include "Arduino.h" 
#include "IRQManager.h"
#include "FspTimer.h"

volatile unsigned long agt_time_ms = 0;
uint32_t _freq_hz = 0;

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



FspTimer main_timer;

static uint32_t _top_counter;

static void timer_micros_callback(timer_callback_args_t *p_args) {
	agt_time_ms += 10; //10ms
}

void startAgt() {
	main_timer.begin(TIMER_MODE_PERIODIC, AGT_TIMER, 0, (uint32_t) 0x7530, 0x3a98, (timer_source_div_t) 3, timer_micros_callback);
	IRQManager::getInstance().addPeripheral(IRQ_AGT,(void*)main_timer.get_cfg());
	main_timer.open();
	_top_counter = main_timer.get_counter();
	main_timer.start();
	_freq_hz = main_timer.get_freq_hz();
}

unsigned long millis()
{
#ifdef AZURE_RTOS_THREADX
	return tx_time_get();
#else
	
	// Convert time to ms
	NVIC_DisableIRQ(main_timer.get_cfg()->cycle_end_irq);
	uint32_t time_ms = ((main_timer.get_period_raw() - main_timer.get_counter()) / main_timer.get_cfg()->source_div)/1000 + agt_time_ms;
	NVIC_EnableIRQ(main_timer.get_cfg()->cycle_end_irq);
	return time_ms;
#endif
}

unsigned long micros() {
	
	// Convert time to us
	NVIC_DisableIRQ(main_timer.get_cfg()->cycle_end_irq);
	uint32_t time_us = ((main_timer.get_period_raw() - main_timer.get_counter()) / main_timer.get_cfg()->source_div) +  agt_time_ms * 1000;
	NVIC_EnableIRQ(main_timer.get_cfg()->cycle_end_irq);
	return time_us;
}
