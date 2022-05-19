#include "Arduino.h"

void _init() {
   R_BSP_PinAccessEnable();
}

extern "C" void pcdc_acm_thread_entry();

void arduino_main(void)
{
   _init();
   initVariant();
   pcdc_acm_thread_entry();
   setup();
   while (1)
   {
      loop();
   }
}