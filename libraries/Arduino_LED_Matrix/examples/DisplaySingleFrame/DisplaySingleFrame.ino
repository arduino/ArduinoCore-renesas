/*
  Single Frame
  
  Displays single frames using matrix.loadFrame
  
  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/led-matrix
*/

#include "Arduino_LED_Matrix.h"   // Include the LED_Matrix library
#include "frames.h"               // Include a header file containing some custom icons

ArduinoLEDMatrix matrix;          // Create an instance of the ArduinoLEDMatrix class

void setup() {
  Serial.begin(115200);           // Initialize serial communication at a baud rate of 115200
  matrix.begin();                 // Initialize the LED matrix
}

void loop() {
  // Load and display the "chip" frame on the LED matrix
  matrix.loadFrame(chip);
  delay(500);  // Pause for 500 milliseconds (half a second)

  // Load and display the "danger" frame on the LED matrix
  matrix.loadFrame(danger);
  delay(500);

  // Load and display the "happy" frame on the LED matrix
  matrix.loadFrame(happy);
  delay(500);

  // Load and display the "big heart" frame provided by the library
  matrix.loadFrame(LEDMATRIX_HEART_BIG);
  delay(500);

  // Print the current value of millis() to the serial monitor
  Serial.println(millis());
}
