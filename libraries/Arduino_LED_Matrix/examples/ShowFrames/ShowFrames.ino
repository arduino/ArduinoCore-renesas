#include "LED_Matrix.h"
#include "frames.h"

LED_Matrix matrix;

void setup() {
  Serial.begin(115200);
  // you can also load frames at runtime, without stopping the refresh
  matrix.load(frames);
  matrix.begin();
  // turn on autoscroll to avoid calling next() to show the next frame; the paramenter is in milliseconds
  matrix.autoscroll(300);
}

void loop() {
  // if autoscroll() was not called, user needs to call next() when needed
  // matrix.next();
  delay(1000);
  Serial.println(millis());
}
