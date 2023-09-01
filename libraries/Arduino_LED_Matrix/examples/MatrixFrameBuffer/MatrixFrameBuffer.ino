/*
  Matrix Frame Buffer
  
  This Arduino sketch demonstrates the creation and manipulation of 
  a frame buffer for the LED matrix. The frame buffer is used to control 
  the lighting of individual LEDs on the matrix, turning them randomly on and off.
  
  See the full documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/led-matrix
*/

// Include the LED_Matrix library
#include "Arduino_LED_Matrix.h"  

// Create an instance of the ArduinoLEDMatrix class
ArduinoLEDMatrix matrix;        

// Define the frame array for the LED matrix with pixel values
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

// Set up time intervals and dimensions for the matrix
unsigned long lastTickTime, lastGameTickTime;
#define UPDATE_INTERVAL 100
#define GAME_UPDATE_INTERVAL 66

#define ROWS 8
#define COLUMNS 12

// Variables to track the current positions
uint8_t pointX = 0, pointY = 0;

void setup() {
  // Initialize serial communication and delaying for setup
  Serial.begin(115200);
  delay(1500);

  // Initialize the LED matrix
  matrix.begin();

  // Initialize time tracking variables
  lastGameTickTime = lastTickTime = millis();
}

void loop() {
  // Track the current time
  unsigned long msNow = millis();

  // Update the game logic with a fixed interval
  if (msNow - lastGameTickTime > GAME_UPDATE_INTERVAL) {
    // Increment pointX and handling wraparound
    pointX++;
    if (pointX >= COLUMNS) {
      pointX = 0;
      pointY++;
      if (pointY >= ROWS) {
        pointY = 0;
      }
    }

    // Generate random positions and pixel value
    pointX = random(COLUMNS);
    pointY = random(ROWS);
    uint8_t pixelValue = random(2);

    // Update the frame with the new pixel value
    frame[pointY][pointX] = pixelValue;

    // Update the last game tick time
    lastGameTickTime = msNow;
  }

  // Render the LED matrix with the current frame at a fixed interval
  if (msNow - lastTickTime > UPDATE_INTERVAL) {
    matrix.renderBitmap(frame, 8, 12);

    // Update the last rendering tick time
    lastTickTime = msNow;
  }
}