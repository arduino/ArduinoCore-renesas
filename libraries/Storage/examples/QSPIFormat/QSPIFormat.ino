/*
  Portenta C33 - QSPI Flash format

  The sketch formats the board QSPI flash as follow:

  * Partition 1  5MB: used for network certificates and OTA
  * Partition 2 11MB: general purpose

  This example code is in the public domain.
*/

#include "BlockDevice.h"
#include "MBRBlockDevice.h"
#include "LittleFileSystem.h"
#include "FATFileSystem.h"

BlockDevice* root = BlockDevice::get_default_instance();
MBRBlockDevice sys_bd(root, 1);
MBRBlockDevice user_bd(root, 2);
FATFileSystem sys_fs("sys");
FileSystem * user_data_fs;

bool waitResponse() {
  bool confirmation = false;
  while (confirmation == false) {
    if (Serial.available()) {
      char choice = Serial.read();
      switch (choice) {
        case 'y':
        case 'Y':
          confirmation = true;
          return true;
          break;
        case 'n':
        case 'N':
          confirmation = true;
          return false;
          break;
        default:
          continue;
      }
    }
  }
}

void setup() {

  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nWARNING! Running the sketch all the content of the QSPI flash will be erased.");
  Serial.println("Do you want to proceed? Y/[n]");

  if (true == waitResponse()) {
    MBRBlockDevice::partition(root, 1, 0x0B, 0, 5 * 1024 * 1024);
    MBRBlockDevice::partition(root, 2, 0x0B, 5 * 1024 * 1024, 16 * 1024 * 1024);

    int err = sys_fs.reformat(&sys_bd);
    if (err) {
      Serial.println("Error formatting sys partition");
    }

    Serial.println("\nDo you want to use LittleFS to format user data partition? Y/[n]");
    Serial.println("If No, FatFS will be used to format user partition.");

    if (true == waitResponse()) {
      Serial.println("Formatting user partition with LittleFS.");
      user_data_fs = new LittleFileSystem("user");
    } else {
      Serial.println("Formatting user partition with FatFS.");
      user_data_fs = new FATFileSystem("user");
    }

    err = user_data_fs->reformat(&user_bd);
    if (err) {
      Serial.println("Error formatting user partition");
    }

    Serial.println("\nQSPI Flash formatted!");
  }

  Serial.println("It's now safe to reboot or disconnect your board.");
}

void loop() {

}
