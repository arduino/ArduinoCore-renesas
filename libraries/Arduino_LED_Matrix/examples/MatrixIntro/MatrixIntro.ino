/*
  Heart Animation Sketch
  
  This is the default sketch that comes
  shipped with every UNO R4 WiFi board.

  After the animation (a heart) is complete,
  the built-in LED blinks infinitely.

  No additional circuit required.

  created 26 Jun 2023
  by Martino Facchin

  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/led-matrix
*/


// include the LED Matrix library from the Uno R4 core:
#include "Arduino_LED_Matrix.h"
// make an instance of the library:
ArduinoLEDMatrix matrix;
//include the "animation.h" header file that stores the frames for the animation 
#include "animation.h"

void setup() {
  Serial.begin(115200);
  //load frames from the animation.h file
  matrix.loadSequence(frames);
  // start the matrix
  matrix.begin();
  
  // turn on autoscroll to avoid calling next() to show the next frame; the paramenter is in milliseconds
  // matrix.autoscroll(300);

  //play the animation on the matrix
  matrix.play(true);

  //define LED_BUILTIN as an output
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  //blinks the built-in LED every second
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
