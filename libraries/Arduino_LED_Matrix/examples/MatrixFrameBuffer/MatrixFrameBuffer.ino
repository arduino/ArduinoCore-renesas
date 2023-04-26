#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

uint8_t frame[8][12] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }
};

unsigned long lastTickTime, lastGameTickTime;
#define UPDATE_INTERVAL 100
#define GAME_UPDATE_INTERVAL 66

#define ROWS 8
#define COLUMNS 12

uint8_t pointX = 0, pointY = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1500);
  matrix.begin();
  lastGameTickTime = lastTickTime = millis();
}

void loop() {
  unsigned long msNow = millis();
  if (msNow - lastGameTickTime > GAME_UPDATE_INTERVAL) {
    pointX++;
    if (pointX >= COLUMNS) {
      pointX = 0;
      pointY++;
      if (pointY >= ROWS) {
        pointY = 0;
      }
    }
    pointX = random(COLUMNS);
    pointY = random(ROWS);
    uint8_t pixelValue = random(2);
    frame[pointY][pointX] = pixelValue;
    lastGameTickTime = msNow;
  }
  if (msNow - lastTickTime > UPDATE_INTERVAL) {
    matrix.renderBitmap(frame, 8, 12);
    lastTickTime = msNow;
  }
}