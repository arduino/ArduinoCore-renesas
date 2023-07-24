#include "Arduino.h" 
#include "IRQManager.h"
#include "FspTimer.h"

// this file implements the following public funcions: delay, delayMicroseconds, yield, millis, micros
// The millis and micros implementation uses timer AGT0 (24 HMz, 16-bits, count-down mode, 1 ms period)

volatile unsigned long agt_time_ms = 0;

__attribute__((weak)) void delay(uint32_t ms) {
	R_BSP_SoftwareDelay(ms, BSP_DELAY_UNITS_MILLISECONDS);
}

void delayMicroseconds(unsigned int us) {
	R_BSP_SoftwareDelay(us, BSP_DELAY_UNITS_MICROSECONDS);
}

__attribute__((weak)) void yield() {
}

static FspTimer main_timer;
// specifying these details as constants makes micros() faster !
#define _timer_type                AGT_TIMER
#define _timer_index               0
#define _timer_get_underflow_bit() R_AGT0->AGTCR_b.TUNDF
#define _timer_clock_divider       TIMER_SOURCE_DIV_8 // dividers 1, 2 and 8 work because _timer_period is < 16-bit. the divider 4 seems not supported: acts as 1
#define _timer_clock_freq          24000000UL
#define _timer_counts_per_us       (_timer_clock_freq / ((1 << _timer_clock_divider) * 1000000UL))
#define _timer_period              (_timer_counts_per_us * 1000UL)
#define TIMER_PRIORITY             8

static void timer_micros_callback(timer_callback_args_t __attribute((unused))* p_args) {
	agt_time_ms += 1;
}

void startAgt() {
	main_timer.begin(TIMER_MODE_PERIODIC, _timer_type, _timer_index, _timer_period, 1, _timer_clock_divider, timer_micros_callback);;
	main_timer.setup_overflow_irq(TIMER_PRIORITY);
	main_timer.open();
	main_timer.start(); // bug in R4 1.0.2: calling start() is not necessary: open() starts the counter already !?
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
	NVIC_DisableIRQ(main_timer.get_cfg()->cycle_end_irq);
	uint32_t ms = agt_time_ms;
	uint32_t const down_counts = main_timer.get_counter();
	if (_timer_get_underflow_bit() && (down_counts > (_timer_period / 2)))
	{
		// the counter wrapped arround just before it was read
		++ms;
	}
	NVIC_EnableIRQ(main_timer.get_cfg()->cycle_end_irq);
	uint32_t const up_counts = (_timer_period - 1) - down_counts;
	return  (ms * 1000) + (up_counts / _timer_counts_per_us);
}
