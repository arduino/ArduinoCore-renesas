/*
  This sketch allows live editing of the matrix pixels using WebSerial
  To test, head to https://ledmatrix-editor.arduino.cc

  The LED Matrix editor is part of Arduino Labs (https://labs.arduino.cc/), and is therefore considered experimental software.

  Don't forget to close any serial monitor already opened.

  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/led-matrix  
*/

#include "Arduino_LED_Matrix.h" // Include the LED_Matrix library

ArduinoLEDMatrix matrix;        // Create an instance of the ArduinoLEDMatrix class

void setup() {
  Serial.begin(115200);         // Initialize serial communication at a baud rate of 115200
  matrix.begin();               // Initialize the LED matrix
}

// Define an array to hold pixel data for a single frame (4 pixels)
uint32_t frame[] = {
  0, 0, 0, 0xFFFF
};

void loop() {
  // Check if there are at least 12 bytes available in the serial buffer
  if(Serial.available() >= 12){
    // Read 4 bytes from the serial buffer and compose them into a 32-bit value for each element in the frame
    frame[0] = Serial.read() | Serial.read() << 8 | Serial.read() << 16 | Serial.read() << 24;
    frame[1] = Serial.read() | Serial.read() << 8 | Serial.read() << 16 | Serial.read() << 24;
    frame[2] = Serial.read() | Serial.read() << 8 | Serial.read() << 16 | Serial.read() << 24;
    
     // Load and display the received frame data on the LED matrix
    matrix.loadFrame(frame);
  }
}
