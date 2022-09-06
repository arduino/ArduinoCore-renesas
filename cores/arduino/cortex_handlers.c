#include "Arduino.h"

#if 0

// TODO: check if gdb is able to unwind the call stack even without this

/* 
test code:

int illegal_instruction_execution(void) {
  int (*bad_instruction)(void) = (int (*)())0xE0000000;
  return bad_instruction();
}
*/

/* On hard fault, copy HARDFAULT_PSP to the sp reg so gdb can give a trace */
void **HARDFAULT_PSP;
register void *stack_pointer asm("sp");
void HardFault_Handler(void) {
    asm("mrs %0, psp" : "=r"(HARDFAULT_PSP) : :);
    stack_pointer = HARDFAULT_PSP;
    while(1);
}

#endif