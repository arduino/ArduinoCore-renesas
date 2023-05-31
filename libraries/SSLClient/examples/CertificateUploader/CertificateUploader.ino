/*
  Portenta C33 - Certificate uploader

  The sketch uploads and saves network certificates on the system
  partition of the QSPI flash

  This example code is in the public domain.
*/

#include "BlockDevice.h"
#include "MBRBlockDevice.h"
#include "FATFileSystem.h"
#include "certificates.h"

BlockDevice* root = BlockDevice::get_default_instance();
MBRBlockDevice sys_bd(root, 1);
FATFileSystem sys_fs("sys");

long getFileSize(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    return size;
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

  int err =  sys_fs.mount(&sys_bd);
  if (err) {
    // Reformat if we can't mount the filesystem
    // this should only happen on the first boot
    Serial.println("No filesystem containing the WiFi firmware was found.");
    Serial.println("Usually that means that the WiFi firmware has not been installed yet"
                  " or was overwritten with another firmware.\n");
    Serial.println("Formatting the filsystem to install the firmware and certificates...\n");
    err = sys_fs.reformat(&sys_bd);
  }

  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir("/sys")) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      Serial.println("Searching for WiFi firmware file " + String(ent->d_name) + " ...");
      String fullname = "/sys/" + String(ent->d_name);
      if (fullname == "/sys/cacert.pem") {
        Serial.println("A WiFi firmware is already installed. "
                       "Do you want to install the firmware anyway? Y/[n]");
        while (1) {
          if (Serial.available()) {
            int c = Serial.read();
            if (c == 'Y' || c == 'y') {
              sys_fs.reformat(&sys_bd);
              break;
            }
            if (c == 'N' || c == 'n') {
              Serial.println("It's now safe to reboot or disconnect your board.");
              return;
            }
          }
        }
      }
    }
    closedir (dir);
  }

  

  int chunck_size = 128;
  int byte_count = 0;
  FILE* fp = fopen("/sys/cacert.pem", "wb");

  Serial.println("Flashing certificates");
  printProgress(byte_count, cacert_pem_len, 10, true);
  while (byte_count < cacert_pem_len) {
    if(byte_count + chunck_size > cacert_pem_len)
      chunck_size = cacert_pem_len - byte_count;
    int ret = fwrite(&cacert_pem[byte_count], chunck_size, 1 ,fp);
    if (ret != 1) {
      Serial.println("Error writing certificates");
      break;
    }
    byte_count += chunck_size;
    printProgress(byte_count, cacert_pem_len, 10, false);
  }
  fclose(fp);

  fp = fopen("/sys/cacert.pem", "rb");
  char buffer[128];
  int ret = fread(buffer, 1, 128, fp);
  Serial.write(buffer, ret);
  while (ret == 128) {
    ret = fread(buffer, 1, 128, fp);
    Serial.write(buffer, ret);
  }
  fclose(fp);

  Serial.println("\nFirmware and certificates updated!");
  Serial.println("It's now safe to reboot or disconnect your board.");
}

void loop() {

}
