#include "CAN.h"

#define CAN_ID              (0x20)
#define CAN_FRAME_LEN       8

void setup() {
  SerialUSB.begin(115200);
  delay(1000);
  SerialUSB.println("Start CAN Write test over CAN1");

  SerialUSB.print("Opening CAN1...");
  if (!CAN1.begin()) {
    SerialUSB.println(" failed :(");
    while(1) {}
  }
  SerialUSB.println(" OK :)");
}

void loop() {
  uint8_t msg_data[CAN_FRAME_LEN] = {1,2,3,4,5,6,7,8};
  CanMessage msg = {CAN_ID, CAN_FRAME_LEN, msg_data};
  if (!CAN1.write(msg)) {
    SerialUSB.println("Message write failed :(");
    while(1) {}
  }
  delay(3000);
}
