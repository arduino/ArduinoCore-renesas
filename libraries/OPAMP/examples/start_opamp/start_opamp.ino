#include <OPAMP.h>

void setup () {
  Serial.begin(9600);
  delay(2000);  // serial monitor delay
  // activate OPAMP
  if (!OPAMP.begin(OPAMP_SPEED_HIGHSPEED)) {
    Serial.println("Failed to start OPAMP!");
  }
  uint8_t status = OPAMP.getStatus();
  if (status & (1u << 0u)) {
    Serial.println("OPAMP running on channel 0!");
  } else {
    Serial.println("OPAMP channel 0 is not running!");
  }
}

void loop() {
  delay(1000);  // do nothing
}
