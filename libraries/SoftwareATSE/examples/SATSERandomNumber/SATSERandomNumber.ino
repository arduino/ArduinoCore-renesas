/*
  Software Secure Element Random Number

  This sketch uses the Software Secure Element to generate a random number
  every second and print it to the Serial monitor

  Circuit:
   - UNO R4 WiFi
*/

#include <SoftwareATSE.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!SATSE.begin()) {
    Serial.println("Failed to communicate with Software Secure Element!");
    while (1);
  }
}

void loop() {
  Serial.print("Random number = ");
  Serial.println(SATSE.random(65535));

  delay(1000);
}
