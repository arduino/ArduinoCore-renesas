#include "boot.h"


/* call this function when you want to force the board to go in bootloader */
void goBootloader() {
   R_SYSTEM->PRCR = (uint16_t) BSP_PRV_PRCR_PRC1_UNLOCK;
   BOOT_DOUBLE_TAP_DATA = DOUBLE_TAP_MAGIC;
   R_SYSTEM->PRCR = (uint16_t) BSP_PRV_PRCR_LOCK;
   ((R_USB_FS0_Type*)R_USB_FS0_BASE)->SYSCFG_b.DPRPU = 0;
   NVIC_SystemReset();
   while (1); // WDT will fire here
}
