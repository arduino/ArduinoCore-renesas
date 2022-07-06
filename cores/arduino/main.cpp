#include "Arduino.h"

void _init() {
   R_BSP_PinAccessEnable();
}

#ifdef AZURE_RTOS_THREADX
USBSerial SerialUSB;
extern "C" void pcdc_acm_thread_entry();
#endif

void arduino_main(void)
{
   _init();
   initVariant();
#ifdef AZURE_RTOS_THREADX
   pcdc_acm_thread_entry();
#endif
   startAgt();
   setup();
   while (1)
   {
      loop();
   }
}

#ifdef AZURE_RTOS_THREADX
extern "C" {
   void main_thd_entry(void) {
      arduino_main();
   }
}
#endif