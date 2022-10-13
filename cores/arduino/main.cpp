#include "Arduino.h"
#include "usb/USB.h"

/* TODO: make me configurable by the variant */
#define APPLICATION_VECTOR_TABLE_ADDRESS_RAM    0x20007F00

void startAgt();

void _init() {
   R_BSP_PinAccessEnable();
}

volatile uint32_t *irq_vector_table;

void startAgt(void);

extern uint32_t __ROM_Start;
extern const fsp_vector_t __VECTOR_TABLE[];
extern const fsp_vector_t g_vector_table[];

void arduino_main(void)
{
   // Disable stack pointer protection
   // TODO: the best thing would be keeping SPMON active but changing
   // R_MPU_SPMON->SP[0].EA = __stack_top; on every call to malloc()
   // When stack and heap would collide, we could signal the NMI with mbed style leds patterns
#if _RA_CORE != CM33
   // TODO: find out why H33 build can't find the symbol
   R_MPU_SPMON->SP[0].CTL = 0;
#endif

   __disable_irq();
   irq_vector_table = (volatile uint32_t *)APPLICATION_VECTOR_TABLE_ADDRESS_RAM;
   int _i;
   for (_i=0; _i<BSP_CORTEX_VECTOR_TABLE_ENTRIES; _i++) {
      *(irq_vector_table + _i) = (uint32_t)__VECTOR_TABLE[_i];
   }
   for (_i=0; _i<BSP_ICU_VECTOR_MAX_ENTRIES; _i++) {
      *(irq_vector_table + _i +BSP_CORTEX_VECTOR_TABLE_ENTRIES) = (uint32_t)g_vector_table[_i];
   }
   
   SCB->VTOR = (uint32_t)irq_vector_table;
   
   __DSB();
   __enable_irq();

   _init();
   initVariant();
   __USBStart();
   Serial.begin(115200);
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