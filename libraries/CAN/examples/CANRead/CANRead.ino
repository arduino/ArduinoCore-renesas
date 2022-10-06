/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <CAN.h>

/**************************************************************************************
 * CONST
 **************************************************************************************/

static uint32_t const CAN_ID = 0x20;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(115200);
  while (!Serial) { }

  Serial.println("Start CAN Read test over CAN");

  Serial.print("Opening CAN...");
  if (!CAN.begin())
  {
    Serial.println(" failed :(");
    for (;;) { }
  }
  Serial.println(" OK :)");
}

void loop() {
  
  CanMsg msg;

  if (CAN.read(msg))
  {
    Serial.println("Message received :)");
    Serial.print("ID: 0x");
    Serial.println(msg.id, HEX);
    Serial.print("Length: ");
    Serial.println(msg.data_length);
    Serial.print("Data: ");
    
    for (int i = 0; i < msg.data_length; i++)
    {
      Serial.print("0x");
      Serial.print(msg.data[i], HEX);
      Serial.print(", ");
    } 
    Serial.println();
  }
}
