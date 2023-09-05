/*
 * This sketch is a modified version of the SDUBoot example from the SDU
 * library in the Arduino SAMD core.
 *
 * SDUBoot.ino is converted with build.sh into HEX values in a .h file
 * in 'boot' folder of the SDU library. The SDU library with support of
 * the linker script inserts the binary representation of this sketch
 * before the actual sketch. This way this sketch acts as a second stage
 * bootloader always uploaded with the sketch's binary.
 *
 * To make this second stage bootloader as small as possible while it is
 * still an Arduino sketch, it can't be built with standard boards
 * configurations of UNO R4 Minima or Wifi.
 * Additionally the libfsp.a is built without code flash writing support
 * and this sketch needs it.
 *
 * I created a custom board definition to build this sketch to a binary
 * size less than 0x6000 bytes.
 * https://github.com/JAndrassy/my_boards/tree/master/renesas-uno
 * The custom unor4_sdu variant has peripherals support removed with many
 * 'HOWMANY' settings in pins_arduino.h set to 0 to make the SDUBoot sketch
 * bin even smaller.
 * The bin size reduction for libfsp.a and the sketch is described here
 * https://github.com/arduino/ArduinoCore-renesas/discussions/118
 */

#include <SD.h>
#include <r_flash_lp.h>

#define SDU_START    0x4000
#define SDU_SIZE    0x6000

#define SKETCH_START (uint32_t*)(SDU_START + SDU_SIZE)

#ifndef SDCARD_SS_PIN
#define SDCARD_SS_PIN 4
#endif

#define UPDATE_FILE "UPDATE.BIN"

flash_lp_instance_ctrl_t ctrl;
flash_cfg_t cfg;

void stopAgt(); //add to core/time.cpp as { main_timer.close();}

void setup() {

  delay(1);

  if (SD.begin(SDCARD_SS_PIN) && SD.exists(UPDATE_FILE)) {

    File updateFile = SD.open(UPDATE_FILE);
    uint32_t updateSize = updateFile.size();
    bool updateFlashed = false;

    if (updateSize > SDU_SIZE) {
      // skip the SDU section
      updateFile.seek(SDU_SIZE);
      updateSize -= SDU_SIZE;

      cfg.data_flash_bgo = false;
      cfg.p_callback = nullptr;
      cfg.p_context = nullptr;
      cfg.ipl = (BSP_IRQ_DISABLED);
      cfg.irq = FSP_INVALID_VECTOR;

      fsp_err_t rv = R_FLASH_LP_Open(&ctrl, &cfg);
      if (rv == FSP_SUCCESS) {

        uint32_t flashAddress = (uint32_t) SKETCH_START;

        // erase the pages
        __disable_irq();
        rv = R_FLASH_LP_Erase(&ctrl, flashAddress, (updateSize / BSP_FEATURE_FLASH_LP_CF_BLOCK_SIZE) + 1);
        __enable_irq();
        if (rv == FSP_SUCCESS) {

          uint8_t buffer[32 * BSP_FEATURE_FLASH_LP_CF_WRITE_SIZE];

          // write the pages
          for (uint32_t i = 0; i < updateSize; i += sizeof(buffer)) {
            updateFile.read(buffer, sizeof(buffer));
            __disable_irq();
            R_FLASH_LP_Write(&ctrl, (uint32_t) &buffer, flashAddress, sizeof(buffer));
            __enable_irq();
            flashAddress += sizeof(buffer);
          }

          updateFlashed = true;
        }

        R_FLASH_LP_Close(&ctrl);
      }

      updateFile.close();

      if (updateFlashed) {
        SD.remove(UPDATE_FILE);
      }
    }
  }

  stopAgt();

  SysTick->CTRL = 0;

  // Disable MSP monitoring.
  R_MPU_SPMON->SP[0].CTL = 0;

  // jump to the sketch
  __set_MSP(*SKETCH_START);

  //Reset vector table address
  SCB->VTOR = ((uint32_t) (SKETCH_START ) & SCB_VTOR_TBLOFF_Msk);

  // address of Reset_Handler is written by the linker at the beginning of the .text section (see linker script)
  uint32_t resetHandlerAddress = (uint32_t) *(SKETCH_START + 1);
  // jump to reset handler
  asm("bx %0"::"r"(resetHandlerAddress));
}

void loop() {
}
