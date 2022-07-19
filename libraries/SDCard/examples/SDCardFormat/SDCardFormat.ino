#include "SDCard.h"

#define SD_VOLUME_NAME "SD_Volume"
#define SD_NUM_FATS 1
#define SD_NUM_HIDDEN_SECTORS 0
#define SD_TOT_SECTORS 15558144
#define SD_SECTOR_SIZE 512

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB) {}
  delay(100);
  SerialUSB.println("****** SD Card Format sketch ******");
  SerialUSB.println();

  SerialUSB.println("Formatting options:");
  SerialUSB.println("Volume name: " + String(SD_VOLUME_NAME));
  SerialUSB.println("Number of FATs: " + String(SD_NUM_FATS));
  SerialUSB.println("Total number of sectors: " + String(SD_TOT_SECTORS));
  SerialUSB.println("Number of bytes per sector: " + String(SD_SECTOR_SIZE));
  SerialUSB.println("Number of hidden sectors: " + String(SD_NUM_HIDDEN_SECTORS));

  SerialUSB.println("\nDo you want to format the SD Card? Y/[n]");

  if(waitResponse()) {
    if (!SDCard.mount()) { 
      SerialUSB.print("Error mounting the SD Card: ");
      SerialUSB.println(SDCard.getError());
      while(1) {}
    }
    SerialUSB.println("Start formatting the SD Card. This operation may take few minutes to complete...");
    if (!SDCard.format(SD_VOLUME_NAME, SD_NUM_FATS, SD_NUM_HIDDEN_SECTORS, SD_TOT_SECTORS, SD_SECTOR_SIZE)) {
      SerialUSB.print("Error formatting the SD Card: ");
      SerialUSB.println(SDCard.getError());
      while(1) {}
    }
    SerialUSB.println("SD Card successfully formatted! :)");
  } else {
    SerialUSB.println("You decided NOT to format the SD Card. Unmounting the SD card...");
  }

  SDCard.unmount();
  
}

void loop() {
  delay(1000);
}


bool waitResponse() {
  bool confirmation = false;
  while (confirmation == false) {
    if (SerialUSB.available()) {
      char choice = SerialUSB.read();
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
