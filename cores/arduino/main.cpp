#include "Arduino.h"
#include "usb/USB.h"

void startAgt();

void _init() {
   R_BSP_PinAccessEnable();
}

static uint32_t vectors[256] __attribute__((aligned (0x1000U)));

void startAgt(void);

extern uint32_t __ROM_Start;
extern const fsp_vector_t __VECTOR_TABLE[];
extern const fsp_vector_t g_vector_table[];

void arduino_main(void)
{
   int _i;
   for (_i=0; _i<BSP_CORTEX_VECTOR_TABLE_ENTRIES; _i++) {
      vectors[_i] = (uint32_t)__VECTOR_TABLE[_i];
   }
   for (_i=0; _i<BSP_ICU_VECTOR_MAX_ENTRIES; _i++) {
      vectors[_i+BSP_CORTEX_VECTOR_TABLE_ENTRIES] = (uint32_t)g_vector_table[_i];
   }
   __disable_irq();
   SCB->VTOR = (uint32_t)vectors;
   __DSB();
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