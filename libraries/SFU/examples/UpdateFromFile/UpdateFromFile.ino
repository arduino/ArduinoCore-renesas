/*
 * Local/embedded OTA update: stores firmware in sketch and applies it locally.
 * No network required; useful for testing and pre-loaded devices.
 * See ../README.md for complete documentation and packaging instructions.
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
