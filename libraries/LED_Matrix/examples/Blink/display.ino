/*
  ASCIIDraw
  Use the ArduinoGraphics library to draw ASCII art on the Serial Monitor.
  This is intended primarily to allow testing of the library.
  See the Arduino_MKRRGB library for a more useful demonstration of the ArduinoGraphics library.
  The circuit:
  - Arduino board
  This example code is in the public domain.
*/

//LEDsDrawClass LEDS;

void draw_leds() {

  LEDS.beginDraw();

  const char text[] = "UNO";
  LEDS.textFont(Font_4x6);
  const byte textWidth = strlen(text) * LEDS.textFontWidth();
  const byte textHeight = LEDS.textFontHeight();
  const byte textX = 0;//(canvasWidth - textWidth) / 2;
  const byte textY = 0;//(canvasHeight - textHeight) / 2;
  LEDS.beginText(textX, textY, 1, 1, 1);
  LEDS.print(text);
  LEDS.textScrollSpeed(1000);

  LEDS.endText();

  // print the drawing to the Serial Monitor
  LEDS.endDraw();
}
