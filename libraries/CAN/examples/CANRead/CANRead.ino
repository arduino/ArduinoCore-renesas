#include "CAN.h"

#define CAN_ID              (0x20)
#define CAN_FRAME_LEN       8

void setup() {
  Serial5.begin(115200);
  delay(1000);
  Serial5.println("Start CAN Read test over CAN1");

  Serial5.print("Opening CAN1...");
  if (!CAN1.begin()) {
    Serial5.println(" failed :(");
    while(1) {}
  }
  Serial5.println(" OK :)");
}

void loop() {
  CanMessage msg;
  if (!CAN1.read(msg)) {
    Serial5.println("Message read failed :(");
    while(1) {}
  }
  Serial5.println(msg.data[0]);
  delay(3000);
}
