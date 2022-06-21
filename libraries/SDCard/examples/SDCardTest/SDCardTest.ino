#include "SDCard.h"

#define FILE_NAME "TEST_FILE.txt"

uint8_t g_src_buffer[1024] BSP_ALIGN_VARIABLE(4);
uint8_t g_read_buffer[1024] BSP_ALIGN_VARIABLE(4);

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB) {}
  delay(100);
  SerialUSB.println("Start");
  SerialUSB.println();
  delay(100);

  int status = 0;
  
  status = SDCard.mount();
  if (status != FSP_SUCCESS) {
    SerialUSB.print("SDCard.mount Error: ");
    SerialUSB.println(status, HEX);
    while(1) {}
  } else {
    SerialUSB.println("SDCard.mount OK :)");
  }

  delay(100);

  status = SDCard.open();
  if (status != FX_SUCCESS) {
    SerialUSB.print("SDCard.open Error: ");
    SerialUSB.println(status, HEX);
    while(1) {}
  } else {
    SerialUSB.print("SDCard.open OK :)");
  }

  delay(100);

  status = SDCard.deleteFile(FILE_NAME);
  if (status != FX_SUCCESS) {
    SerialUSB.print("SDCard.deleteFile Error: ");
    SerialUSB.println(status, HEX);
    //while(1) {}
  } else {
    SerialUSB.print("SDCard.deleteFile OK :)");
  }

  delay(100);

  status = SDCard.createFile(FILE_NAME);
  if (status != FX_SUCCESS && status != FX_ALREADY_CREATED) {
    SerialUSB.print("SDCard.createFile Error: ");
    SerialUSB.println(status, HEX);
    while(1) {}
  } else {
    SerialUSB.print("SDCard.createFile OK :)");
  }

  delay(100);
  
  for (int i=0; i<512; i++) {
    g_src_buffer[i] = (uint8_t) ('A' + (i % 26));
  }

  FX_FILE src_file;
  status = SDCard.writeFile(&src_file, FILE_NAME, g_src_buffer, sizeof(g_src_buffer));
  if (status != FX_SUCCESS) {
    SerialUSB.print("SDCard.writeFile Error: ");
    SerialUSB.println(status, HEX);
    while(1) {}
  } else {
    SerialUSB.print("SDCard.writeFile OK :)");
  }

  delay(100);

  uint32_t size;
  status = SDCard.readFile(&src_file, FILE_NAME, g_read_buffer, sizeof(g_src_buffer), &size);
  SerialUSB.print("SDCard.readFile returned ");
  SerialUSB.println(status, HEX);
  SerialUSB.print("Size read: ");
  SerialUSB.println(size);

  if (memcmp(g_src_buffer, g_read_buffer, 1024) != 0) {
    SerialUSB.println("Buffers are different!");
  } else {
    SerialUSB.println("Write-Read OK :)");
  }
  
  status = SDCard.unmount();
  if (status != FX_SUCCESS) {
    SerialUSB.print("SDCard.unmount Error: ");
    SerialUSB.println(status, HEX);
  } else {
    SerialUSB.print("SDCard.unmount OK :)");
  }
}

void loop() {

}
