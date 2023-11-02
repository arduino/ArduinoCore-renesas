/*
  CANRead

  Receive and read CAN Bus messages
  
  
  Mailbox Groups 0-3 (Mailbox  0-15) -> TRANSMIT
  Mailbox Groups 4-5 (Mailbox 16-23) -> RECEIVE EXTENDED
  Mailbox Groups 6-7 (Mailbox 24-31) -> RECEIVE STANDARD

  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/can
*/

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino_CAN.h>

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(115200);
  while (!Serial) { }

  CAN.setMailboxMask(4, 0x1FFFFFFF);
  CAN.setMailboxMask(6, 0x1FFFFFFF);

  for (int c=16; c <= 31; c++) {
    CAN.setMailboxID(c, 0x10);
  }

  if (!CAN.begin(CanBitRate::BR_250k))
  {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }
}

void loop()
{
  if (CAN.available())
  {
    CanMsg const msg = CAN.read();
    Serial.println(msg);
  }
}
