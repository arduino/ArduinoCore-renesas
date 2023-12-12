#include "Arduino.h" 
#include "IRQManager.h"
#include "FspTimer.h"

// this file implements the following public funcions: delay, delayMicroseconds, yield, millis, micros

__attribute__((weak)) void delay(uint32_t ms)
{
  auto const start = millis();
  auto const stop = start + ms;
  while(millis() < stop) yield();
}

void delayMicroseconds(unsigned int us) {
	R_BSP_SoftwareDelay(us, BSP_DELAY_UNITS_MICROSECONDS);
}

__attribute__((weak)) void yield() {
}

static FspTimer   agt_timer;
volatile uint32_t agt_time_ms = 0;

static void timer_micros_callback(timer_callback_args_t __attribute((unused))* p_args) {
	agt_time_ms += 1;
}

void startAgt() {
	// configure AGT timer 0 to generate an underflow interrupt every 1 ms
	// a clock divider 8 works for both the Uno R4 and Portenta C33 because number of clock ticks 
	// in 1 ms (period) is an integer number and below the 16-bit counter limit
	// on the Uno R4 the AGT clock is 24 MHz / 8 -> 3000 ticks per ms
	// on the Portenta C33 the AGT clock is 50 Mhz / 8 -> 6250 ticks per ms
	const uint32_t clock_freq_Hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB);
	const uint32_t period = clock_freq_Hz / ((1 << TIMER_SOURCE_DIV_8) * 1000UL);
	agt_timer.begin(/* mode */ TIMER_MODE_PERIODIC,
                  /* type */ AGT_TIMER,
                  /* channel */ 0,
                  period,
                  /* pulse */ 1,
                  TIMER_SOURCE_DIV_8,
                  timer_micros_callback);;
	agt_timer.setup_overflow_irq(8);
	agt_timer.open();
	agt_timer.start(); // bug in R4 1.0.2: calling start() is not necessary: open() starts the counter already !?
}

unsigned long millis()
{
#ifdef AZURE_RTOS_THREADX
	return tx_time_get();
#else
	return agt_time_ms;
#endif
}

unsigned long micros() {
	// Return time in us
	const timer_cfg_t* cfg = agt_timer.get_cfg();
	NVIC_DisableIRQ(cfg->cycle_end_irq);
	uint32_t ms = agt_time_ms;
	// read from the R_AGT0 registers directly for performance reasons
	uint32_t const down_counts = R_AGT0->AGT; // get the counter value
	if (R_AGT0->AGTCR_b.TUNDF && (down_counts > (cfg->period_counts / 2))) {
		// if the TUNDF (underflow) bit is set, the counter wrapped around 
		// just before down_counts was read and agt_time_ms was not yet updated
		++ms;
	}
	NVIC_EnableIRQ(cfg->cycle_end_irq);
	uint32_t const up_counts = (cfg->period_counts - 1) - down_counts;
	return  (ms * 1000) + ((up_counts * 1000) / cfg->period_counts);
}
