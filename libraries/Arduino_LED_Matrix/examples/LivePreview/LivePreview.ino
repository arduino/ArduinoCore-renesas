/*
  This sketch allows live editing of the matrix pixels using WebSerial
  To test, head to https://ledmatrix-editor.arduino.cc
  press 'p' and select 'UNO R4 WiFi' board from the list.

  Don't forget to close any serial monitor already opened.
*/

#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(115200);
  matrix.begin();
}

uint32_t frame[] = {
  0, 0, 0, 0xFFFF
};

void loop() {
  if(Serial.available() >= 12){
    frame[0] = Serial.read() | Serial.read() << 8 | Serial.read() << 16 | Serial.read() << 24;
    frame[1] = Serial.read() | Serial.read() << 8 | Serial.read() << 16 | Serial.read() << 24;
    frame[2] = Serial.read() | Serial.read() << 8 | Serial.read() << 16 | Serial.read() << 24;
    matrix.loadFrame(frame);
  }
}

