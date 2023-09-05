/*
  Portenta C33 - 2nd stage bootloader

  The sketch checks for available update files on QSPI system partion
  or SD card and try to update the Sketch

  Build (release) with arduino-cli:
    # arduino-cli compile --export-binaries --clean --verbose -b arduino-git:renesas:portenta_c33 --build-property "build.extra_flags=-DNO_USB -DPORTENTA_C33_SFU_BUILD"
  Build (debug) with arduino-cli:
    # arduino-cli compile --export-binaries --clean --verbose -b arduino-git:renesas:portenta_c33 --build-property "build.extra_flags=-DNO_USB -DPORTENTA_C33_SFU_DEBUG_OTA -DPORTENTA_C33_SFU_BUILD"
  Export loader binary:
    # xxd -i build/arduino-git.renesas.portenta_c33/loader.ino.bin | grep -E '0x' > ../../src/c33.h

  This example code is in the public domain.
*/
#include <BlockDevice.h>
#include <MBRBlockDevice.h>
#include <CodeFlashBlockDevice.h>
#include <FATFileSystem.h>
#include "ota.h"

#ifdef PORTENTA_C33_SFU_SDCARD_OTA
#include <SDCardBlockDevice.h>
#endif

#ifdef PORTENTA_C33_SFU_DEBUG_OTA
#include <Arduino_DebugUtils.h>
#endif

/* Key code for writing PRCR register. */
#define BSP_PRV_PRCR_KEY                              (0xA500U)
#define BSP_PRV_PRCR_PRC1_UNLOCK                      ((BSP_PRV_PRCR_KEY) | 0x2U)
#define BSP_PRV_PRCR_LOCK                             ((BSP_PRV_PRCR_KEY) | 0x0U)

#define BSP_PRV_BITS_PER_WORD                         (32)

#define SKETCH_FLASH_OFFSET                           (64 * 1024)

#define FULL_UPDATE_FILE_PATH                         "/ota/UPDATE.BIN"
#define FULL_UPDATE_FILE_PATH_OTA                     FULL_UPDATE_FILE_PATH ".OTA"

#define VERSION                                       1

#ifdef PORTENTA_C33_SFU_SDCARD_OTA
SDCardBlockDevice sd(PIN_SDHI_CLK, PIN_SDHI_CMD, PIN_SDHI_D0, PIN_SDHI_D1, PIN_SDHI_D2, PIN_SDHI_D3, PIN_SDHI_CD, PIN_SDHI_WP);
#endif

BlockDevice* qspi = BlockDevice::get_default_instance();
CodeFlashBlockDevice& cf = CodeFlashBlockDevice::getInstance();
MBRBlockDevice sys_bd(qspi, 1);
FATFileSystem sys_fs("ota");

void setup() {

#ifdef PORTENTA_C33_SFU_DEBUG_OTA
  Serial.begin(115200);
  while(!Serial);
  Debug.setDebugOutputStream(&Serial);
  Debug.setDebugLevel(DBG_VERBOSE);
  DEBUG_INFO("SFU version: %d", VERSION);
#endif

  qspi->init();
  int err =  sys_fs.mount(&sys_bd);
  if (!err) {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_INFO("Try QSPI OTA");
#endif
    try_ota();
  } else {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_ERROR("Cannot mount QSPI filesystem. Error: %d", err);
#endif
  }

#ifdef PORTENTA_C33_SFU_SDCARD_OTA
  sd.init();
  err = sys_fs.mount(&sd);
  if (!err) {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_INFO("Try SD OTA");
#endif
    try_ota();
  } else {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_ERROR("Cannot mount SDCARD filesystem. Error: %d", err);
#endif
  }
#endif
  
  int app_valid = (((*(uint32_t *) SKETCH_FLASH_OFFSET + POST_APPLICATION_ADDR) & 0xFF000000) == 0x20000000);
  if (app_valid) {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_INFO("Booting application @ 0x%x", SKETCH_FLASH_OFFSET + POST_APPLICATION_ADDR);
#endif
    boot5(SKETCH_FLASH_OFFSET + POST_APPLICATION_ADDR);
  } else {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_INFO("Application not found");
#endif
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

int try_ota(void) {
  FILE *update_file;
  FILE *target_file;

  update_file = fopen(FULL_UPDATE_FILE_PATH_OTA, "rb");
  if (update_file) {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_INFO("Compressed update file found");
#endif
    target_file = fopen(FULL_UPDATE_FILE_PATH, "wb");
    if (!target_file) {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
      DEBUG_ERROR("Error creating file for decompression");
#endif
      return -1;
    }
    int err = verify_decompress(update_file, target_file);
    fclose(update_file);
    fclose(target_file);
    remove(FULL_UPDATE_FILE_PATH_OTA);
  } else {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_INFO("Compressed update file not found");
#endif
  }

  update_file = fopen(FULL_UPDATE_FILE_PATH, "rb");
  if (update_file) {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_INFO("Update file found");
#endif
    int err = verify_flash(update_file);
    fclose(update_file);
    remove(FULL_UPDATE_FILE_PATH);
  } else {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_INFO("Update file not found");
#endif
  }
}

int verify_decompress(FILE* update_file, FILE* target_file) {
  int err = verify_header(update_file);
  if (err) {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_ERROR("Error during header verification: %d", err);
#endif
    return err;
  }
  err = decompress(update_file, target_file, nullptr);
  return err;
}

int verify_flash(FILE* file) {
  int err = verify_sketch(file);
  if (err != 0) {
#ifdef PORTENTA_C33_SFU_DEBUG_OTA
    DEBUG_ERROR("Error during sketch verification: %d", err);
#endif
    return err;
  }
  err = flash(file, &cf, SKETCH_FLASH_OFFSET + POST_APPLICATION_ADDR);
  return err;
}
