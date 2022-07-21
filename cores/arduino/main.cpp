#include "Arduino.h"
#include "usb/USB.h"

void _init() {
   R_BSP_PinAccessEnable();
}

void startAgt(void);

void arduino_main(void)
{
   _init();
   initVariant();
   __USBStart();
   startAgt();
   setup();
   while (1)
   {
      //tud_task();
      loop();
   }
}

#ifdef AZURE_RTOS_THREADX
extern "C" {
   void main_thd_entry(void) {
      arduino_main();
   }
}
#else
extern "C" {
   void hal_entry(void) {
      arduino_main();
   }
}
#endif