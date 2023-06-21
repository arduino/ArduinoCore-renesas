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
#include "ymodem.h"

BlockDevice* root = BlockDevice::get_default_instance();
MBRBlockDevice sys_bd(root, 1);
FATFileSystem sys_fs("sys");

char filename[256] = {'\0'};

long getFileLen(FILE *file) {
  fseek(file, 0, SEEK_END);
  long len = ftell(file);
  fseek(file, 0, SEEK_SET);
  //Decrement len by 1 to remove the CRC from the count
  return len;
}

long getFileSize(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    return size;
}

// void printProgress(uint32_t offset, uint32_t size, uint32_t threshold, bool reset) {
//   static int percent_done = 0;
//   if (reset == true) {
//     percent_done = 0;
//     Serial.println("Flashed " + String(percent_done) + "%");
//   } else {
//     uint32_t percent_done_new = offset * 100 / size;
//     if (percent_done_new >= percent_done + threshold) {
//       percent_done = percent_done_new;
//       Serial.println("Flashed " + String(percent_done) + "%");
//     }
//   }
// }

void setup() {

  Serial.begin(115200);
  // while (!Serial);

  int err =  sys_fs.mount(&sys_bd);
  if (err) {
    // Reformat if we can't mount the filesystem
    // this should only happen on the first boot
    // Serial.println("No filesystem containing the WiFi firmware was found.");
    // Serial.println("Usually that means that the WiFi firmware has not been installed yet"
    //               " or was overwritten with another firmware.\n");
    // Serial.println("Formatting the filsystem to install the firmware and certificates...\n");
    err = sys_fs.reformat(&sys_bd);
  }

  // DIR *dir;
  // struct dirent *ent;

  // if ((dir = opendir("/sys")) != NULL) {
  //   /* print all the files and directories within directory */
  //   while ((ent = readdir (dir)) != NULL) {
  //     Serial.println("Searching for WiFi firmware file " + String(ent->d_name) + " ...");
  //     String fullname = "/sys/" + String(ent->d_name);
  //     if (fullname == "/sys/cacert.pem") {
  //       Serial.println("A WiFi firmware is already installed. "
  //                      "Do you want to install the firmware anyway? Y/[n]");
  //       while (1) {
  //         if (Serial.available()) {
  //           int c = Serial.read();
  //           if (c == 'Y' || c == 'y') {
  //             sys_fs.reformat(&sys_bd);
  //             break;
  //           }
  //           if (c == 'N' || c == 'n') {
  //             Serial.println("It's now safe to reboot or disconnect your board.");
  //             return;
  //           }
  //         }
  //       }
  //     }
  //   }
  //   closedir (dir);
  // }

  

  int chunk_size = 128;
  int byte_count = 0;
  FILE* fp = fopen("/sys/cacert.pem", "wb");

  // Serial.println("Flashing certificates");
  // printProgress(byte_count, cacert_pem_len, 10, true);
  while (byte_count < cacert_pem_len) {
    if(byte_count + chunk_size > cacert_pem_len)
      chunk_size = cacert_pem_len - byte_count;
    int ret = fwrite(&cacert_pem[byte_count], chunk_size, 1 ,fp);
    if (ret != 1) {
      // Serial.println("Error writing certificates");
      break;
    }
    byte_count += chunk_size;
    // printProgress(byte_count, cacert_pem_len, 10, false);
  }
  fclose(fp);

  // fp = fopen("/sys/cacert.pem", "rb");
  // char buffer[128];
  // int ret = fread(buffer, 1, 128, fp);
  // Serial.write(buffer, ret);
  // while (ret == 128) {
  //   ret = fread(buffer, 1, 128, fp);
  //   Serial.write(buffer, ret);
  // }
  // fclose(fp);

  // Serial.println("\nFirmware and certificates updated!");
  // Serial.println("It's now safe to reboot or disconnect your board.");
}

void loop() {

  uint8_t command = 0xFF;

  if (Serial.available()) {
    command = Serial.read();
  }

  if (command == 'Y') {
    FILE* f = fopen("/sys/temp.bin", "wb");
    while (Serial.available()) {
      Serial.read();
    }
    Serial.print("Y");
    int ret = Ymodem_Receive(f, 1024 * 1024, filename);
    String name = String(filename);
    if (ret > 0 && name != "") {
      name = "/sys/" + name;
      fclose(f);
      ret = rename("/sys/temp.bin", name.c_str());
    }
  }
  if (command == 'R') {
    String filename = Serial.readStringUntil('\r');
    filename.trim();
    String filename_abs = String("/sys/") + filename;
    FILE* f = fopen(filename_abs.c_str(), "rb");
    while (Serial.available()) {
      Serial.read();
    }
    if (f != NULL) {
      Serial.print("R");
      int ret = Ymodem_Transmit((char*)filename.c_str(), getFileLen(f), f);
      fclose(f);
    }
  }
  if (command == 0xFF) {
    delay(10);
  }
}
