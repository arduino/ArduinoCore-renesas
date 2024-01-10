#include "Arduino.h"

#ifndef BOOTLOADER_INCLUDE_H
#define BOOTLOADER_INCLUDE_H

/* Key code for writing PRCR register. */
#define BSP_PRV_PRCR_KEY                 (0xA500U)
#define BSP_PRV_PRCR_PRC1_UNLOCK          ((BSP_PRV_PRCR_KEY) | 0x2U)
#define BSP_PRV_PRCR_LOCK                ((BSP_PRV_PRCR_KEY) | 0x0U)

#ifdef NO_BACKUP_REGISTERS
#define BOOT_DOUBLE_TAP_DATA              (*((volatile uint32_t *)0x20007FF0))
#else
#define BOOT_DOUBLE_TAP_DATA              (*((volatile uint32_t *) &R_SYSTEM->VBTBKR[0]))
#endif
#define DOUBLE_TAP_MAGIC                  0x07738135


/* call this function when you want to force the board to go in bootloader */
void goBootloader();
#endif