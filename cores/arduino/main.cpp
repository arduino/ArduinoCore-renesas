#include "Arduino.h"
#include "usb/USB.h"

void startAgt();

void _init() {
   R_BSP_PinAccessEnable();
}

static uint32_t* vectors[256];

void startAgt(void);

extern uint32_t __ROM_Start;

void arduino_main(void)
{
   __disable_irq();
   memcpy(vectors, (void*)__ROM_Start, sizeof(vectors)*sizeof(vectors[0]));
   SCB->VTOR = (uint32_t)vectors;
   __enable_irq();

   _init();
   initVariant();
   __USBStart();
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
#else
extern "C" {
   void hal_entry(void) {
      arduino_main();
   }
}
#endif