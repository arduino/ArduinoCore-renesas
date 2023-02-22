/*
  Portenta H33 - USB MASS STORAGE DEVICE

  The sketch shows how mount an H33 as Mass Storage Device on QSPI Flash AND SD Card

  The circuit:
   - Portenta H33 + Breakout board

  created February 21th, 2023
  by Daniele Aimo (d.aimo@arduino.cc)

  This example code is in the public domain.
*/

/* 
 * CONFIGURATION DEFINES 
 */
    
/* the name of the filesystem */
#define TEST_QSPI_FS_NAME "qspi"
#define TEST_SD_FS_NAME "sd"

#include "SDCardBlockDevice.h"
#include "QSPIFlashBlockDevice.h"
#include "UsbMsd.h"
#include "FATFileSystem.h"

QSPIFlashBlockDevice bd_qspi(PIN_QSPI_CLK, PIN_QSPI_SS, PIN_QSPI_D0, PIN_QSPI_D1, PIN_QSPI_D2, PIN_QSPI_D3); 
SDCardBlockDevice bd_sd(PIN_SDHI_CLK, PIN_SDHI_CMD, PIN_SDHI_D0, PIN_SDHI_D1, PIN_SDHI_D2, PIN_SDHI_D3, PIN_SDHI_CD, PIN_SDHI_WP);
USBMSD msd{&bd_qspi,&bd_sd};
FATFileSystem fs_qspi(TEST_QSPI_FS_NAME);
FATFileSystem fs_sd(TEST_SD_FS_NAME);

std::string root_qspi_folder = std::string("/") + std::string(TEST_QSPI_FS_NAME);
std::string root_sd_folder = std::string("/") + std::string(TEST_SD_FS_NAME);

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

int err_qspi = 0;
int err_sd = 0;

/* -------------------------------------------------------------------------- */
/*                                      SETUP                                 */
/* -------------------------------------------------------------------------- */
void setup() {
  
  /* SERIAL INITIALIZATION */
  Serial.begin(9600);
  while(!Serial) {
    
  }

  Serial.println("*** USB Mass Storage DEVICE on QSPI Flash AND SD Card ***");
  
  /* Mount the QSPI device */
  err_qspi =  fs_qspi.mount(&bd_qspi);
  if (err_qspi) {
    Serial.println("Unable to mount filesystem on QSPI Flash");
    Serial.println("Probably the QSPI has not been formatted properly");
    Serial.println("Use the FormatQSPIForMSD sketch to format the QSPI flash");
  }  
  
  /* Mount the SD Card (if present) */
  err_sd = fs_sd.mount(&bd_sd);
  if (err_sd) {
    Serial.println("Unable to mount filesystem on SD Card");
    
  }  
}

/* -------------------------------------------------------------------------- */
/*                                       LOOP                                 */
/* -------------------------------------------------------------------------- */
void loop() {
  int flag = 0;
  if(err_qspi == 0) {
    Serial.print("Content of the folder ");
    Serial.print(root_qspi_folder.c_str());
    Serial.println(":");
    printDirectoryContent(root_qspi_folder.c_str());
    flag++;
  }

  if(err_sd == 0) {
    Serial.print("Content of the folder ");
    Serial.print(root_sd_folder.c_str());
    Serial.println(":");
    printDirectoryContent(root_sd_folder.c_str());
    flag++;
  }

  if(flag == 0) {
    Serial.println("Unable to mount BOTH filesystems");
  }


  delay(2000);
}
