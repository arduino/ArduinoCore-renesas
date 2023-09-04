
/* Key code for writing PRCR register. */
#define BSP_PRV_PRCR_KEY                              (0xA500U)
#define BSP_PRV_PRCR_PRC1_UNLOCK                      ((BSP_PRV_PRCR_KEY) | 0x2U)
#define BSP_PRV_PRCR_LOCK                             ((BSP_PRV_PRCR_KEY) | 0x0U)

#define BSP_PRV_BITS_PER_WORD                         (32)


#define POST_APPLICATION_ADDR                         (0x10000)
#define SKETCH_FLASH_OFFSET                           (64 * 1024)

void setup() {
  Serial.begin(9600);
  //while(!Serial){}
  Serial.println("2nd stage bootloader");
  
  int app_valid = (((*(uint32_t *) SKETCH_FLASH_OFFSET + POST_APPLICATION_ADDR) & 0xFF000000) == 0x20000000);
  if (app_valid) {
    Serial.print("Booting application @");
    Serial.print(SKETCH_FLASH_OFFSET + POST_APPLICATION_ADDR, HEX);
    boot5(SKETCH_FLASH_OFFSET + POST_APPLICATION_ADDR);
  } else {
    Serial.print("Application not found");
  }
}

void loop() {

}

void boot5(uint32_t address) {

  R_SYSTEM->PRCR = (uint16_t) BSP_PRV_PRCR_PRC1_UNLOCK;
  R_BSP_MODULE_STOP(FSP_IP_USBFS, 0);
  R_BSP_MODULE_STOP(FSP_IP_USBHS, 0);
  R_SYSTEM->PRCR = (uint16_t) BSP_PRV_PRCR_LOCK;

    /* Disable MSP monitoring. */
#if BSP_FEATURE_TZ_HAS_TRUSTZONE
    __set_MSPLIM(0);
#else
    R_MPU_SPMON->SP[0].CTL = 0;
#endif

  __disable_irq(); // Note: remember to enable IRQ in application
  __DSB();
  __ISB();

  // Disable SysTick
  SysTick->CTRL = 0;

  SCB->VTOR  = address;

  // Cleanup NVIC
  for (size_t i = 0U; i < BSP_ICU_VECTOR_MAX_ENTRIES / BSP_PRV_BITS_PER_WORD; i++)
  {
    NVIC->ICER[i] = 0xFF;
  }

  uint32_t mainStackPointer = *(volatile uint32_t *)(address);
  __set_MSP(mainStackPointer);
  uint32_t programResetHandlerAddress = *(volatile uint32_t *) (address + 4);
  void (* programResetHandler)(void) = (void (*)(void)) programResetHandlerAddress;
  programResetHandler();
}
