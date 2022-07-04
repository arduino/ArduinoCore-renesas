#include "CAN.h"

#define CAN_ID              (0x20)
#define CAN_FRAME_LEN       8

void setup() {
  Serial5.begin(115200);
  delay(1000);
  Serial5.println("Start CAN Write test over CAN1");

  Serial5.print("Opening CAN1...");
  if (!CAN1.begin()) {
    Serial5.println(" failed :(");
    while(1) {}
  }
  Serial5.println(" OK :)");
}

void loop() {
  uint8_t msg_data[CAN_FRAME_LEN] = {1,2,3,4,5,6,7,8};
  CanMessage msg = {CAN_ID, CAN_FRAME_LEN, msg_data};
  if (!CAN1.write(msg)) {
    Serial5.println("Message write failed :(");
    while(1) {}
  }
  delay(3000);
}
