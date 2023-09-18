/*
  Portenta H33 - USB MASS STORAGE DEVICE

  The sketch shows how mount an H33 as Mass Storage Device on QSPI Flash
  Before running this sketch, to properly format the QSPI flash run the
  Storage/QSPIFormat.ino sketch

  The circuit:
   - Portenta H33

  created February 21th, 2023
  by Daniele Aimo (d.aimo@arduino.cc)

  This example code is in the public domain.
*/

#include "BlockDevice.h"
#include "MBRBlockDevice.h"
#include "UsbMsd.h"
#include "FATFileSystem.h"

BlockDevice* root = BlockDevice::get_default_instance();
MBRBlockDevice sys_bd(root, 1);
MBRBlockDevice user_bd(root, 2);
FATFileSystem sys_fs("sys");
FATFileSystem user_fs("user");

int err = 0;

/* -------------------------------------------------------------------------- */
void printDirectoryContent(const char* name) {
/* -------------------------------------------------------------------------- */  
  DIR *dir;
  struct dirent *ent;
  int num = 0;

  if ((dir = opendir(name)) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      if(ent->d_type == DT_REG) {
        Serial.print(" - [File]: ");
      }
      else if(ent->d_type == DT_DIR) {
        Serial.print(" - [Fold]: ");
      }
      Serial.println(ent->d_name);
      num++;
    }
    closedir (dir);
    if(num == 0) {
      Serial.println(" ** EMPTY ** ");
    }
  }
  else {
    Serial.print("Failed to open folder ");
    Serial.println(name); 
  }
}

/* -------------------------------------------------------------------------- */
/*                                      SETUP                                 */
/* -------------------------------------------------------------------------- */
void setup() {

  /* SERIAL INITIALIZATION */
  Serial.begin(9600);
    while(!Serial) {
  }

  Serial.println("*** USB Mass Storage DEVICE on QSPI Flash ***");

  /* Mount the partition */
  err =  sys_fs.mount(&sys_bd);
  if (err) {
    Serial.println("Unable to mount system filesystem");
    while(1) {
    }
  }

  /* Mount the partition */
  err =  user_fs.mount(&user_bd);
  if (err) {
    Serial.println("Unable to mount user filesystem. Only FatFS is supported");
    /* Probably the user is using LittleFs. Go on and show only system fs */
  }
}

/* -------------------------------------------------------------------------- */
/*                                       LOOP                                 */
/* -------------------------------------------------------------------------- */
void loop() {
  Serial.println("Content of the system partition:");
  printDirectoryContent("/sys");
  if(!err) {
    Serial.println("Content of the user partition:");
    printDirectoryContent("/user");
  }
  delay(2000);
}
