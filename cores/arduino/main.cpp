#include "Arduino.h"
#include "usb/USB.h"

void startAgt();

void _init() {
   R_BSP_PinAccessEnable();
}

static uint32_t vectors[BSP_CORTEX_VECTOR_TABLE_ENTRIES+BSP_ICU_VECTOR_MAX_ENTRIES] __attribute__((aligned (0x1000U)));

void startAgt(void);

extern uint32_t __ROM_Start;
extern const fsp_vector_t __VECTOR_TABLE[];
extern const fsp_vector_t g_vector_table[];

void arduino_main(void)
{
   __disable_irq();
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

extern "C" void __cxa_pure_virtual() { while(1); }
extern "C" void __cxa_deleted_virtual() { while(1); }
