#include "CAN.h"

#define CAN_ID              (0x20)
#define CAN_FRAME_LEN       8

void setup() {
  SerialUSB.begin(115200);
  delay(1000);
  SerialUSB.println("Start CAN Read test over CAN1");

  SerialUSB.print("Opening CAN1...");
  if (!CAN1.begin()) {
    SerialUSB.println(" failed :(");
    while(1) {}
  }
  SerialUSB.println(" OK :)");
}

void loop() {
  
  CanMessage msg;
  if (CAN1.read(msg)) {
    SerialUSB.println("Message received :)");
    SerialUSB.print("ID: 0x");
    SerialUSB.println(msg.id, HEX);
    SerialUSB.print("Length: ");
    SerialUSB.println(msg.data_length);
    SerialUSB.print("Data: ");
    
    for (int i=0; i<CAN_FRAME_LEN; i++) {
      SerialUSB.print("0x");
      SerialUSB.print(msg.data[i], HEX);
      SerialUSB.print(", ");
    } 
    SerialUSB.println();

  }
  delay(1000);
  
}
