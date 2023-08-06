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
const uint8_t _timer_type = AGT_TIMER;
const uint8_t _timer_index = 0;
inline uint8_t _timer_get_underflow_bit() { return R_AGT0->AGTCR_b.TUNDF; }
inline uint16_t _timer_get_counter() { return R_AGT0->AGT; }
// clock divider 8 works for the Uno R4 and Portenta C33 both because _timer_period is < 16-bit. 
// on the Uno R4 the AGT clock is 24 MHz / 8 -> 3000 ticks per ms
// on the Portenta C33 the AGT clock is 50 Mhz / 8 -> 6250 ticks per ms
const timer_source_div_t _timer_clock_divider = TIMER_SOURCE_DIV_8;
uint32_t _timer_period;
const uint8_t TIMER_PRIORITY = 8;

static void timer_micros_callback(timer_callback_args_t __attribute((unused))* p_args) {
	agt_time_ms += 1;
}

void startAgt() {
	const uint32_t _timer_clock_freq = R_FSP_SystemClockHzGet(_timer_type == AGT_TIMER ? FSP_PRIV_CLOCK_PCLKB : FSP_PRIV_CLOCK_PCLKD);
	_timer_period = _timer_clock_freq / ((1 << _timer_clock_divider) * 1000UL);
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
	uint32_t const down_counts = _timer_get_counter();
	if (_timer_get_underflow_bit() && (down_counts > (_timer_period / 2)))
	{
		// the counter wrapped around just before it was read
		++ms;
	}
	NVIC_EnableIRQ(main_timer.get_cfg()->cycle_end_irq);
	uint32_t const up_counts = (_timer_period - 1) - down_counts;
	return  (ms * 1000) + ((up_counts * 1000) / _timer_period);
}
