/*
  Portenta C33 - QSPI Flash format

  The sketch formats the board QSPI flash as follow:

  * Partition 1  1MB: used for network certificates
  * Partition 2  5MB: OTA
  * Partition 3  1MB: Provisioning KVStore
  * Partition 4  7MB: User data

  This example code is in the public domain.
*/

#include "BlockDevice.h"
#include "MBRBlockDevice.h"
#include "LittleFileSystem.h"
#include "FATFileSystem.h"
#include "certificates.h"

BlockDevice* root = BlockDevice::get_default_instance();
MBRBlockDevice wifi_data(root, 1);
MBRBlockDevice ota_data(root, 2);
MBRBlockDevice kvstore_data(root, 3);
MBRBlockDevice user_data(root, 4);
FATFileSystem wifi_data_fs("wlan");
FATFileSystem ota_data_fs("fs");
FileSystem * user_data_fs;

bool waitResponse() {
  bool confirmation = false;
  bool proceed = false;
  while (confirmation == false) {
    if (Serial.available()) {
      char choice = Serial.read();
      switch (choice) {
        case 'y':
        case 'Y':
          confirmation = true;
          proceed = true;
          break;
        case 'n':
        case 'N':
          confirmation = true;
          proceed = false;
          break;
        default:
          continue;
      }
    }
  }
  return proceed;
}

void printProgress(uint32_t offset, uint32_t size, uint32_t threshold, bool reset) {
  static int percent_done = 0;
  if (reset == true) {
    percent_done = 0;
    Serial.println("Flashed " + String(percent_done) + "%");
  } else {
    uint32_t percent_done_new = offset * 100 / size;
    if (percent_done_new >= percent_done + threshold) {
      percent_done = percent_done_new;
      Serial.println("Flashed " + String(percent_done) + "%");
    }
  }
}

void setup() {

  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nWARNING! Running the sketch all the content of the QSPI flash will be erased.");
  Serial.println("The following partitions will be created:");
  Serial.println("Partition 1: Network certificates 1MB");
  Serial.println("Partition 2: OTA 5MB");
  Serial.println("Partition 3: Provisioning KVStore 1MB");
  Serial.println("Partition 4: User data 7MB"),
  Serial.println("Do you want to proceed? Y/[n]");

  if (true == waitResponse()) {
    if (root->init() != BD_ERROR_OK) {
      Serial.println(F("Error: QSPI init failure."));
      return;
    }

    Serial.println("Do you want to perform a full erase of the QSPI flash before proceeding? Y/[n]");
    Serial.println("Note: Full flash erase can take up to one minute.");
    bool fullErase = waitResponse();
    if (fullErase == true) {
      Serial.println("Full erase started, please wait...");
      root->erase(0x0, root->size());
      Serial.println("Full erase completed.");
    } else {
      // Erase only the first sector containing the MBR
      root->erase(0x0, root->get_erase_size());
    }

    MBRBlockDevice::partition(root, 1, 0x0B, 0, 1 * 1024 * 1024);
    MBRBlockDevice::partition(root, 2, 0x0B, 1 * 1024 * 1024,  6 * 1024 * 1024);
    MBRBlockDevice::partition(root, 3, 0x0B, 6 * 1024 * 1024,  7 * 1024 * 1024);
    MBRBlockDevice::partition(root, 4, 0x0B, 7 * 1024 * 1024, 14 * 1024 * 1024);
    // free space from 15.5MB to 16 MB

    bool reformat = true;
    if (!wifi_data_fs.mount(&wifi_data)) {
      Serial.println("\nPartition 1 already contains a filesystem, do you want to reformat it? Y/[n]");
      wifi_data_fs.unmount();

      reformat = waitResponse();
    }

    if (reformat && wifi_data_fs.reformat(&wifi_data)) {
      Serial.println("Error formatting WiFi partition");
      return;
    }

    bool restore = true;
    if (reformat || fullErase) {
      Serial.println("\nDo you want to restore the WiFi firmware and certificates? Y/[n]");
      restore = waitResponse();
    }

    if (reformat && restore) {
      flashCertificates();
    }

    reformat = true;
    if (!ota_data_fs.mount(&ota_data)) {
      Serial.println("\nPartition 2 already contains a filesystem, do you want to reformat it? Y/[n]");
      ota_data_fs.unmount();

      reformat = waitResponse();
    }

    if (reformat && ota_data_fs.reformat(&ota_data)) {
      Serial.println("Error formatting OTA partition");
      return;
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

    reformat = true;
    if (!user_data_fs->mount(&user_data)) {
      Serial.println("\nPartition 4 already contains a filesystem, do you want to reformat it? Y/[n]");
      user_data_fs->unmount();

      reformat = waitResponse();
    }

    if (reformat && user_data_fs->reformat(&user_data)) {
      Serial.println("Error formatting user partition");
      return;
    }

    Serial.println("\nQSPI Flash formatted!");
  }

  Serial.println("It's now safe to reboot or disconnect your board.");
}

const uint32_t file_size = 421098;
extern const unsigned char wifi_firmware_image_data[];

void flashCertificates() {
  FILE* fp = fopen("/wlan/cacert.pem", "wb");

  Serial.println("Flashing certificates");
  uint32_t chunk_size = 128;
  uint32_t byte_count = 0;
  printProgress(byte_count, cacert_pem_len, 10, true);
  while (byte_count < cacert_pem_len) {
    if(byte_count + chunk_size > cacert_pem_len)
      chunk_size = cacert_pem_len - byte_count;
    int ret = fwrite(&cacert_pem[byte_count], chunk_size, 1 ,fp);
    if (ret != 1) {
      Serial.println("Error writing certificates");
      break;
    }
    byte_count += chunk_size;
    printProgress(byte_count, cacert_pem_len, 10, false);
  }
  fclose(fp);
}

void loop() {

}
