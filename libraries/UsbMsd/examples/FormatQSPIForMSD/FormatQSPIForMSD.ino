/*
  Portenta H33 - USB MASS STORAGE DEVICE

  The sketch shows how to format and mount an H33 as Mass Storage Device on QSPI Flash

  The circuit:
   - Portenta H33

  created February 7th, 2023
  by Daniele Aimo (d.aimo@arduino.cc)

  This example code is in the public domain.
*/

/* 
 * CONFIGURATION DEFINES 
 */
 
/* the name of the filesystem */
#define TEST_FS_NAME "qspi"

#include "QSPIFlashBlockDevice.h"
#include "MBRBlockDevice.h"
#include "UsbMsd.h"
#include "FATFileSystem.h"

QSPIFlashBlockDevice block_device(PIN_QSPI_CLK, PIN_QSPI_SS, PIN_QSPI_D0, PIN_QSPI_D1, PIN_QSPI_D2, PIN_QSPI_D3); 
MBRBlockDevice mbr(&block_device, 1);
USBMSD msd(&block_device);
FATFileSystem fs(TEST_FS_NAME);

std::string root_folder = std::string("/") + std::string(TEST_FS_NAME);

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
  
  #ifdef ALLOW_BLINKING_ALIVE
  pinMode(LED_BUILTIN, OUTPUT);
  #endif
 
  Serial.println("*** USB Mass Storage DEVICE on QSPI: format the device ***");
  
  /* CREATE A PARTITION big as the whole device */
  MBRBlockDevice::partition(&block_device, 1, 0x0e, 0, 1024* 1024 * 16);
  /* Mount the partition */
  int err =  fs.mount(&block_device);
  if (err) {
    Serial.println("No Filesystem found, formatting...");
    err = fs.reformat(&block_device);
  }

  if (err) {
    Serial.println("[ERROR]: Something went wrong while formatting the new partition");  
  }
  
}

/* -------------------------------------------------------------------------- */
/*                                       LOOP                                 */
/* -------------------------------------------------------------------------- */
void loop() {
  printDirectoryContent(root_folder.c_str());
  delay(2000);
}
