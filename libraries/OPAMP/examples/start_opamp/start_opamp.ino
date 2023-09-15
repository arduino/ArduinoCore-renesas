#include <OPAMP.h>

void setup () {
  Serial.begin(9600);
  delay(2000); // serial monitor delay
  // activate OPAMP, default channel 0
  // Plus: Analog A1
  // Minus: Analog A2 
  // Output: Analog A3
  if (!OPAMP.begin(OPAMP_SPEED_HIGHSPEED)) {
    Serial.println("Failed to start OPAMP!");
  }
  bool const isRunning = OPAMP.isRunning(0);
  if (isRunning) {
    Serial.println("OPAMP running on channel 0!");
  } else {
    Serial.println("OPAMP channel 0 is not running!");
  }
}

void loop() {
  delay(1000); // do nothing
}
