/*
  Software Secure Element serial number

  This sketch prints the Software Secure Element serial number:
  ESP32-S3-MINI-1 efuse mac address

  Circuit:
   - UNO R4 WiFi
*/

#include <SoftwareATSE.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!SATSE.begin()) {
    Serial.println("Failed to communicate with Software Secure Element!");
    Serial.println("Make sure your WiFi firmware version is greater than 0.3.0");
    while (1);
  }
}

void loop() {
  Serial.print("S/N = ");
  Serial.println(SATSE.serialNumber());

  delay(1000);
}
