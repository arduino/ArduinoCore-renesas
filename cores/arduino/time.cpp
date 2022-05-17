#include "Arduino.h" 

void delay(uint32_t ms) {
	tx_thread_sleep(ms);
}

void delayMicroseconds(unsigned int us) {
	R_BSP_SoftwareDelay(us, BSP_DELAY_UNITS_MICROSECONDS);
}

void yield() {
	tx_thread_relinquish();
}

unsigned long millis()
{
	return tx_time_get();
}

unsigned long micros() {
	// TODO: use a high resolution timer, understand if it is feasible to stop it if unused
	return 0;
}
