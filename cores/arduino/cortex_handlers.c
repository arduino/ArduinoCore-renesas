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
#endif

void Stacktrace_Handler(void) {
    asm("MOV     r0, lr");                  /* get lr */
    asm("MOV     r1, sp");                  /* get stack pointer (current is MSP) */
    asm("BL      cm_backtrace_fault");
    while(1);
}