#include "Arduino_LED_Matrix.h"
#include "frames.h"

ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(115200);
  matrix.begin();
}

void loop() {
  matrix.loadFrame(chip);
  delay(500);
  matrix.loadFrame(danger);
  delay(500);
  matrix.loadFrame(happy);
  delay(500);
  matrix.loadFrame(heart);
  delay(500);
  Serial.println(millis());
}
