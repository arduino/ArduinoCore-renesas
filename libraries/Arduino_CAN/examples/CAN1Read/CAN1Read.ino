/**************************************************************************************
 * COMPILE TIME CHECKS
 **************************************************************************************/

#ifndef ARDUINO_PORTENTA_C33
# error "CAN1 is only available on Portenta C33."
#endif /* ARDUINO_PORTENTA_C33 */

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

  /* You need to enable the CAN transceiver
 * by commenting in below code when using
 * a Portenta H33 on a Portenta Max Carrier.
 * Note: Only CAN1 is available on the Portenta
 * Max Carrier's RJ10 CAN connector.
 */
#if (PIN_CAN1_STBY >= 0)
  pinMode(PIN_CAN1_STBY, OUTPUT);
  digitalWrite(PIN_CAN1_STBY, LOW);
#endif

  if (!CAN1.begin(CanBitRate::BR_250k))
  {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }
}

void loop()
{
  if (CAN1.available())
  {
    CanMsg const msg = CAN1.read();
    Serial.println(msg);
  }
}
