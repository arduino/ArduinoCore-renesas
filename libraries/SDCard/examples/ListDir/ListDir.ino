#include "SDCard.h"

uint8_t g_src_buffer[1024] BSP_ALIGN_VARIABLE(4);
uint8_t g_read_buffer[1024] BSP_ALIGN_VARIABLE(4);

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB) {}
  delay(100);
  SerialUSB.println("******SD Card List Directories sketch******");
  SerialUSB.println();
  
  if (!SDCard.mount()) {
    SerialUSB.print("SDCard.mount Error: ");
    SerialUSB.println(SDCard.getError());
    while(1) {}
  }

  if (!SDCard.open()) {
    SerialUSB.print("SDCard.open Error: ");
    SerialUSB.println(SDCard.getError());
    while(1) {}
  }

  listDirectories();
  
  if (!SDCard.unmount()) {
    SerialUSB.print("SDCard.unmount Error: ");
    SerialUSB.println(SDCard.getError());
    while(1) {}
  }
}

void loop() {
  delay(1000);
}

void listDirectories() {
  char entryName[FILE_NAME_MAX + 1];
  SerialUSB.println("List SDCARD content: ");
  while(SDCard.getEntryName(entryName)) {
    SerialUSB.println(entryName);
  }
  SerialUSB.println("Done :)");
}
