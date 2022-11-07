/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <CAN.hpp>

/**************************************************************************************
 * CONSTANTS
 **************************************************************************************/

static uint32_t const CAN_ID = 0x20;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  //Serial.begin(115200);
  //while (!Serial) { }

  //Serial.println("Start CAN Write test over CAN");

  //Serial.print("Opening CAN...");
  if (!CAN.begin())
  {
    //Serial.println(" failed :(");
    for (;;) {}
  }
/*
  if (CAN.enableInternalLoopback() < 0)
  {
    for(;;) { }
  }
*/

  //Serial.println(" OK :)");
}

static uint8_t msg_cnt = 0;

void loop()
{
  uint8_t const msg_data[] = {1,2,3,4,5,6,7,msg_cnt};
  CanMsg msg(CAN_ID, sizeof(msg_data), msg_data);

  if (CAN.write(msg) < 0)
  {
    //Serial.println("Message write failed :(");
    for (;;) {}
  }

  msg_cnt++;

  delay(1000);
}
