/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <CAN.h>

/**************************************************************************************
 * CONSTANTS
 **************************************************************************************/

static uint32_t const CAN_ID = 0x20;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(115200);
  while (!Serial) { }

  Serial.println("Start CAN Write test over CAN");

  Serial.print("Opening CAN...");
  if (!CAN.begin())
  {
    Serial.println(" failed :(");
    for (;;) {}
  }
  Serial.println(" OK :)");
}

void loop()
{
  uint8_t const msg_data[] = {1,2,3,4,5,6,7,8};
  CanMsg msg = {CAN_ID, sizeof(msg_data), msg_data};

  if (!CAN.write(msg))
  {
    Serial.println("Message write failed :(");
    for (;;) {}
  }

  delay(1000);
}
