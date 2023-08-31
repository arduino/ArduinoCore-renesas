#include "Arduino.h"
extern "C" {
   #include "cm_backtrace/cm_backtrace.h"
}

/* TODO: make me configurable by the variant */
extern const uint32_t __StackTop;
const uint32_t APPLICATION_VECTOR_TABLE_ADDRESS_RAM = (uint32_t)&__StackTop;

void _init() {
   R_BSP_PinAccessEnable();
}

volatile uint32_t *irq_vector_table;

void startAgt(void);

extern uint32_t __ROM_Start;
extern const fsp_vector_t __VECTOR_TABLE[];
extern const fsp_vector_t g_vector_table[];


#if FSP_PRIV_TZ_USE_SECURE_REGS

#define BSP_PRV_BITS_PER_WORD    (32)

void unsecure_registers() {

    /* Unprotect security registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_SAR);

 #if !BSP_TZ_SECURE_BUILD
    /* Set the DMAC channels to secure access. */
    R_CPSCU->ICUSARC = ~R_CPSCU_ICUSARC_SADMACn_Msk;
 #endif

    /* Place all vectors in non-secure state unless they are used in the secure project. */
    uint32_t interrupt_security_state[BSP_ICU_VECTOR_MAX_ENTRIES / BSP_PRV_BITS_PER_WORD];
    memset(&interrupt_security_state, UINT8_MAX, sizeof(interrupt_security_state));

    for (size_t i = 0U; i < BSP_ICU_VECTOR_MAX_ENTRIES; i++)
    {
         /* This is a secure vector. Clear the associated bit. */
         uint32_t index = i / BSP_PRV_BITS_PER_WORD;
         uint32_t bit   = i % BSP_PRV_BITS_PER_WORD;
         interrupt_security_state[index] &= ~(1U << bit);
    }

    /* The Secure Attribute managed within the ARM CPU NVIC must match the security attribution of IELSEn
     * (Reference section 13.2.9 in the RA6M4 manual R01UH0890EJ0050). */
    uint32_t volatile * p_icusarg = &R_CPSCU->ICUSARG;
    for (size_t i = 0U; i < BSP_ICU_VECTOR_MAX_ENTRIES / BSP_PRV_BITS_PER_WORD; i++)
    {
        p_icusarg[i]  = interrupt_security_state[i];
        NVIC->ITNS[i] = interrupt_security_state[i];
    }

    /* Protect security registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_SAR);
}
#endif

#define stringify(s) str(s)
#define str(s) #s

extern "C" void Stacktrace_Handler(void);

void arduino_main(void)
{
   // Disable stack pointer protection
   // TODO: the best thing would be keeping SPMON active but changing
   // R_MPU_SPMON->SP[0].EA = __stack_top; on every call to malloc()
   // When stack and heap would collide, we could signal the NMI with mbed style leds patterns
#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__)
   __set_MSPLIM(0);
#if FSP_PRIV_TZ_USE_SECURE_REGS
   unsecure_registers();
#endif
#else
   R_MPU_SPMON->SP[0].CTL = 0;
#endif

   __disable_irq();
   irq_vector_table = (volatile uint32_t *)APPLICATION_VECTOR_TABLE_ADDRESS_RAM;
   size_t _i;
   for (_i=0; _i<BSP_CORTEX_VECTOR_TABLE_ENTRIES; _i++) {
      *(irq_vector_table + _i) = (uint32_t)__VECTOR_TABLE[_i];
   }
   for (_i=0; _i<BSP_ICU_VECTOR_MAX_ENTRIES; _i++) {
      *(irq_vector_table + _i +BSP_CORTEX_VECTOR_TABLE_ENTRIES) = (uint32_t)g_vector_table[_i];
   }

   

   SCB->VTOR = (uint32_t)irq_vector_table;

   __DSB();
   __enable_irq();

#ifdef BACKTRACE_SUPPORT
   // "install" stacktrace print over Hardfault handler
   *(irq_vector_table + 3) = (uint32_t)Stacktrace_Handler;
   cm_backtrace_init(stringify(PROJECT_NAME), "RA", __DATE__ "\0");
#endif

   _init();
   initVariant();
   analogReference();
#ifndef NO_USB
   __USBStart();
   Serial.begin(115200);
#endif
   startAgt();
   setup();
   while (1)
   {
      loop();
   }
}

#ifdef BACKTRACE_SUPPORT
/* for printf compatibility */
extern "C" __attribute__((weak)) int _write (int fhdl, const void *buf, size_t count) {
  (void)fhdl;
  return Serial.write_raw((uint8_t*)buf, count);
}
#endif

extern "C" {
   void hal_entry(void) {
      arduino_main();
   }
}

//Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (*func)()) {
   (void)func;
   return 0;
}

namespace __gnu_cxx {
   void __verbose_terminate_handler() { }
}
extern "C" __attribute__((weak)) void __cxa_pure_virtual(void);
extern "C" __attribute__((weak)) void __cxa_pure_virtual(void)
{
   exit(1);
}

// Used by malloc() to request more heap memory. This function
// must be defined if we want malloc() to return a null pointer
// if more space is requested than is available. Notice that
// there's no support for decrement as the very first action,
// because the result of that is unspecified by the standard,
// so we can do whatever we want in that case.
extern "C" void * _sbrk(ptrdiff_t change)
{
  extern char __HeapBase;   // From the linker script
  extern char __HeapLimit;  // From the linker script
  static char *programBreak = nullptr;

  // If malloc() hasn't been called before, then set the program break
  // to __HeapBase from the linker script
  if (nullptr == programBreak)
  {
    programBreak = &__HeapBase;
  }
  // We must return the old program break if everything is ok, so save it
  char * const oldProgramBreak = programBreak;
  // Check that the new program break doesn't pass __HeapLimit from the
  // linker script
  if ((programBreak + change) > &__HeapLimit)
  {
    return (void *) -1;
  }
  // Update the program break according to the requested amount of heap
  // memory
  programBreak += change;
  return oldProgramBreak;
}
