/*
 * Simple test sketch for OTA updates: cycles RGB LED colors.
 * Used as the target firmware for OTA deployment.
 * See ../README.md for complete documentation and usage examples.
 */
#include "SFU.h"

void setLed(int blue, int gree, int red) {
  if (blue == 1) {
    digitalWrite(LEDB, LOW);
  }
  else {
    digitalWrite(LEDB, HIGH);
  }

  if (gree == 1) {
    digitalWrite(LEDG, LOW);
  }
  else {
    digitalWrite(LEDG, HIGH);
  }

  if (red == 1) {
    digitalWrite(LEDR, LOW);
  }
  else {
    digitalWrite(LEDR, HIGH);
  }
}


void setup()
{
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDR, OUTPUT);
}

void loop()
{ // Blue LED on
  setLed(1, 0, 0);
  delay(1000);
  // Green LED on
  setLed(0, 1, 0);
  delay(1000);
  // Red LED on
  setLed(0, 0, 1);
  delay(1000);
}
