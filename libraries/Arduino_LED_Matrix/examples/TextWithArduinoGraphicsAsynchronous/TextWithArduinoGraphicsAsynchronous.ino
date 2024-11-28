// TextAnimation works only when ArduinoGraphics is installed and used.
// ArduinoGraphics is an external library and needs to be installed using
// Library Manager.
// To use ArduinoGraphics APIs, please include BEFORE Arduino_LED_Matrix and TextAnimation
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "TextAnimation.h"

ArduinoLEDMatrix matrix;

// 100 frames maximum. Compute as maximum length of text you want to print (eg. 20 chars) 
// multiplied by font width (eg. 5 for Font_5x7), so 20 chars * 5 px = 100. If you enter lower
// value (or your text get unexpectedly long), animation will be cut and end soon.
TEXT_ANIMATION_DEFINE(anim, 100)

int counter;
bool requestNext = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  matrix.begin();
  matrix.beginDraw();

  matrix.stroke(0xFFFFFFFF);
  matrix.textFont(Font_5x7);
  matrix.textScrollSpeed(60);
  matrix.setCallback(matrixCallback);
  
  const char text[] = "   UNO r4 ";
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endTextAnimation(SCROLL_LEFT, anim);
  
  matrix.loadTextAnimationSequence(anim);
  matrix.play();

  // now animation play asynchronously. Will call matrixCallback once completed.
}

void matrixCallback() {
  // callback is executed in IRQ and should run as fast as possible
  requestNext = true;
}

void loop() {
  if (requestNext) {
    requestNext = false;

    matrix.beginText(0, 1, 0xFFFFFF);
    matrix.print("   ");
    matrix.println(counter);
    matrix.endTextAnimation(SCROLL_RIGHT, anim);
  
    if (counter++ >= 20) {
      counter = 0;
    }

    matrix.loadTextAnimationSequence(anim);
    matrix.play();
  }

  delay(500);
  digitalWrite(LED_BUILTIN, 0);
  delay(500);
  digitalWrite(LED_BUILTIN, 1);
}
