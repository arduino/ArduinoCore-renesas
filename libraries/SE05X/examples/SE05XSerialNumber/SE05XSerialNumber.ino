/*
  SE05X serial number

  This sketch prints the SE050 serial number

  Circuit:
   - Portenta C33
*/

#include <SE05X.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!SE05X.begin()) {
    Serial.println("Failed to communicate with SE05X!");
    while (1);
  }
}

void loop() {
  Serial.print("S/N = ");
  Serial.println(SE05X.serialNumber());

  delay(1000);
}
