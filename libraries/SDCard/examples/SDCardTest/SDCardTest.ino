#include "SDCard.h"

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB) {}
  delay(1000);
  SerialUSB.println("Start");
  SerialUSB.println();
  
  SDCard.init();
  SDCard.open();
  SDCard.createFile("testFile.txt");
  SDCard.writeFile("testFile.txt", "CIAO A TUTTI", 12);
  
  uint8_t *buf;
  uint32_t size;
  SDCard.readFile("testFile.txt", buf, 12, &size);
  if (size != 12 || memcmp("CIAO A TUTTI", buf, 12) != 0)) {
    SerialUSB.print("Size read: ");
    SerialUSB.println(size);
    SerialUSB.print("buf: ");
    for (int i = 0; i < 12; i++) {
      SerialUSB.print(buf[i], HEX);
      SerialUSB.print(", ");
    }
    SerialUSB.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
