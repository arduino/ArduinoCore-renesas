/*
  CANRead

  Receive and read CAN Bus messages
  Read ID 0x100 EXTENDED and ID 0x10 STANDARD
  
  
  Mailbox Groups 0-3 (Mailbox  0-15) -> TRANSMIT
  Mailbox Groups 4-5 (Mailbox 16-23) -> RECEIVE EXTENDED
  Mailbox Groups 6-7 (Mailbox 24-31) -> RECEIVE STANDARD

  Mailbox Mask 5,7 are preset with 0x1FFFFFFF
  Mailbox Mask 4,6 are preset with 0x00


  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/can
*/

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino_CAN.h>

#define Mask11Bit 0x1FFC0000 
#define Mask29Bit 0x1FFFFFFF 

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(115200);
  while (!Serial) { }

  CAN.setFilterMask_Extended(Mask29Bit);
  CAN.setFilterMask_Standard(Mask11Bit);

  for (int c=16; c <= 23; c++) {
    CAN.setMailboxID(c, 0x0100);
  }

  for (int c=24; c <= 31; c++) {
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
