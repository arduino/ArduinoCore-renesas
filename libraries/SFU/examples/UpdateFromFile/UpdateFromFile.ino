/*
 * This example demonstrates how to use the SFU to update the firmware of the
 * Arduino Portenta C33 using a compressed firmware image stored in the QSPI flash.
 * In real applications, you will get the file from the internet and not include it
 * in a header file like we do here for simplicity.
 *
 * Steps:
 *   1) Create a sketch for the Portenta C33 and verify
 *      that it both compiles and works on a board.
 *   2) In the IDE select: Sketch -> Export compiled Binary.
 *   3) Create an OTA update file utilising the tools 'lzss.py' and 'bin2ota.py' stored in
 *      https://github.com/arduino-libraries/ArduinoIoTCloud/tree/master/extras/tools .
 *      A) ./lzss.py --encode SKETCH.bin SKETCH.lzss
 *      B) ./bin2ota.py PORTENTA_C33 SKETCH.lzss SKETCH.ota
 *   4) Create the header file to be included in this Sketch
 *      xxd -i SKETCH.ota > update.h
 *   5) Upload this Sketch to perform the update
 */

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <SFU.h>
#include <BlockDevice.h>
#include <MBRBlockDevice.h>
#include <FATFileSystem.h>
#include <Arduino_DebugUtils.h>
#include "update.h"

BlockDevice* block_device = BlockDevice::get_default_instance();
MBRBlockDevice mbr(block_device, 1);
FATFileSystem fs("ota");


/******************************************************************************
 * SETUP/LOOP
 ******************************************************************************/

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}

  Debug.setDebugLevel(DBG_VERBOSE);

  int err = -1;
  if ((err = fs.reformat(&mbr)) != 0)
  {
     DEBUG_ERROR("%s: fs.reformat() failed with %d", __FUNCTION__, err);
     return;
  }

  FILE * file = fopen("/ota/UPDATE.BIN.OTA", "wb");
  if (!file)
  {
    DEBUG_ERROR("%s: fopen() failed", __FUNCTION__);
    fclose(file);
    return;
  }

  DEBUG_INFO("Start copy update file on QSPI flash.");
  for(int i = 0; i < OTAUsage_ino_PORTENTA_C33_ota_len; i++)
  {
    char const c = OTAUsage_ino_PORTENTA_C33_ota[i];

    if (fwrite(&c, 1, sizeof(c), file) != sizeof(c))
    {
      DEBUG_ERROR("%s: Writing of firmware image to flash failed", __FUNCTION__);
      fclose(file);
      return;
    }
  }

  fclose(file);

  /* Unmount the filesystem. */
  if ((err = fs.unmount()) != 0)
  {
     DEBUG_ERROR("%s: fs.unmount() failed with %d", __FUNCTION__, err);
     return;
  }

  delay(1000);

  DEBUG_INFO("Board reset to trigger the update.");
  DEBUG_INFO("Do not reset or disconnect the board.");
  DEBUG_INFO("Wait until the RGB LED lights up with different colours.");
  NVIC_SystemReset();
}

void loop()
{

}
