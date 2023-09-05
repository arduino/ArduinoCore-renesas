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

static uint32_t msg_cnt = 0;

void loop()
{
  /* Assemble a CAN message with the format of
   * 0xCA 0xFE 0x00 0x00 [4 byte message counter]
   */
  uint8_t const msg_data[] = {0xCA,0xFE,0,0,0,0,0,0};
  memcpy((void *)(msg_data + 4), &msg_cnt, sizeof(msg_cnt));
  CanMsg const msg(CanStandardId(CAN_ID), sizeof(msg_data), msg_data);

  /* Transmit the CAN message, capture and display an
   * error core in case of failure.
   */
  if (int const rc = CAN1.write(msg); rc < 0)
  {
    Serial.print  ("CAN.write(...) failed with error code ");
    Serial.println(rc);
    for (;;) { }
  }

  /* Increase the message counter. */
  msg_cnt++;

  /* Only send one message per second. */
  delay(1000);
}
