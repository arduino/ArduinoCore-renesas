/*
 * This example demonstrates how to use to update the firmware of the Arduino Portenta C33 using
 * a firmware image stored on the QSPI.
 *
 * Steps:
 *   1) Create a sketch for the Portenta C33 and verify
 *      that it both compiles and works on a board.
 *   2) In the IDE select: Sketch -> Export compiled Binary.
 *   3) Create an OTA update file utilising the tools 'lzss.py' and 'bin2ota.py' stored in
 *      https://github.com/arduino-libraries/ArduinoIoTCloud/tree/master/extras/tools .
 *      A) ./lzss.py --encode SKETCH.bin SKETCH.lzss
 *      B) ./bin2ota.py PORTENTA_C33 SKETCH.lzss SKETCH.ota
 *   4) Upload the OTA file to a network reachable location, e.g. OTAUsage.ino.PORTENTA_C33.ota
 *      has been uploaded to: http://downloads.arduino.cc/ota/OTAUsage.ino.PORTENTA_C33.ota
 *   5) Perform an OTA update via steps outlined below.
 */

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <SFU.h>
#include <BlockDevice.h>
#include <MBRBlockDevice.h>
#include <FATFileSystem.h>
#include <WiFiC3.h>
#include <Arduino_DebugUtils.h>
#include "arduino_secrets.h" 

/******************************************************************************
 * CONSTANT
 ******************************************************************************/

/* Please enter your sensitive data in the Secret tab/arduino_secrets.h */
static char const SSID[] = SECRET_SSID;  /* your network SSID (name) */
static char const PASS[] = SECRET_PASS;  /* your network password (use for WPA, or use as key for WEP) */

#if defined(ARDUINO_PORTENTA_C33)
static char const OTA_FILE_LOCATION[] = "http://downloads.arduino.cc/ota/OTAUsage.ino.PORTENTA_C33.ota";
#else
#error "Board not supported"
#endif

BlockDevice* block_device = BlockDevice::get_default_instance();
MBRBlockDevice mbr(block_device, 1);
FATFileSystem fs("ota");

WiFiClient client;

/******************************************************************************
 * SETUP/LOOP
 ******************************************************************************/

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}

  Debug.setDebugLevel(DBG_VERBOSE);

  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("Communication with WiFi module failed!");
    return;
  }

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    Serial.print  ("Attempting to connect to '");
    Serial.print  (SSID);
    Serial.println("'");
    status = WiFi.begin(SSID, PASS);
    delay(10000);
  }
  Serial.print  ("You're connected to '");
  Serial.print  (WiFi.SSID());
  Serial.println("'");

  int err = -1;
  /* Mount the filesystem. */
  if (err = fs.mount(&mbr) != 0)
  {
     DEBUG_ERROR("%s: fs.mount() failed with %d", __FUNCTION__, err);
     return;
  }

  SFU::begin();

  SFU::download(client, "/ota/UPDATE.BIN.OTA", OTA_FILE_LOCATION);

  /* Unmount the filesystem. */
  if ((err = fs.unmount()) != 0)
  {
     DEBUG_ERROR("%s: fs.unmount() failed with %d", __FUNCTION__, err);
     return;
  }

  SFU::apply();
}

void loop()
{

}
