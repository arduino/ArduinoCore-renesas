#include "SDCard.h"

#define FILE_NAME "TEST_FILE.txt"

uint8_t g_src_buffer[1024] BSP_ALIGN_VARIABLE(4);
uint8_t g_read_buffer[1024] BSP_ALIGN_VARIABLE(4);

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB) {}
  delay(100);
  SerialUSB.println("*******Start SD Card test sketch*******");
  SerialUSB.println();

  SerialUSB.print("Mounting SD card... ");
  if (!SDCard.mount()) {
    printError();
  }

  delay(100);

  SerialUSB.print("\nOpening SD card... ");
  if (!SDCard.open()) {
    printError();
  }

  delay(100);

  SerialUSB.print("\nCreating file " + String(FILE_NAME) +"... ");
  if (!SDCard.createFile(FILE_NAME) && SDCard.getError() != SD_FILE_ALREADY_CREATED) {
    printError();
  }

  delay(100);
  
  for (int i=0; i<512; i++) {
    g_src_buffer[i] = (uint8_t) ('A' + (i % 26));
  }

  FX_FILE src_file;
  SerialUSB.print("\nWriting content inside " + String(FILE_NAME) +"... ");
  if (!SDCard.writeFile(&src_file, FILE_NAME, g_src_buffer, sizeof(g_src_buffer))) {
    printError();
  }

  delay(100);

  uint32_t size;
  SerialUSB.print("\nReading back " + String(FILE_NAME) +" content... ");
  if (!SDCard.readFile(&src_file, FILE_NAME, g_read_buffer, sizeof(g_src_buffer), &size)) {
    printError();
  }

  SerialUSB.print("\nComparing buffers...");
  if (memcmp(g_src_buffer, g_read_buffer, 1024) != 0) {
    SerialUSB.print("Buffers are different!");
    printResult(false);
  }

  SerialUSB.print("\nUnmounting SD card... ");
  if (!SDCard.unmount()) {
    printError();
  }

  printResult(true);
  
}

void loop() {
  delay(1000);
}


void printResult(bool sd_pass) {
  if (sd_pass) {
    SerialUSB.println();
    SerialUSB.println("\n*******SD Card test PASSED :) *******");
    SerialUSB.println();
  } else {
    SerialUSB.println();
    SerialUSB.println("\n*******SD Card test FAILED :( *******");
    SerialUSB.println();
  }
}


void printError() {
  SerialUSB.print(" Error: ");
  SerialUSB.println(SDCard.getError());
  printResult(false);
  while(1) {}
}
