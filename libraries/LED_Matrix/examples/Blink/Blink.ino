#include "FspTimer.h"
#include "ArduinoGraphics.h"

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
}

const int led_zero_index = 28;
const int leds_howmany = 11;

size_t pins[][2] = {
  { 7, 3 }, // 0
  { 3, 7 },
  { 7, 4 },
  { 4, 7 },
  { 3, 4 },
  { 4, 3 },
  { 7, 8 },
  { 8, 7 },
  { 3, 8 },
  { 8, 3 },
  { 4, 8 }, // 10
  { 8, 4 },
  { 7, 0 },
  { 0, 7 },
  { 3, 0 },
  { 0, 3 },
  { 4, 0 },
  { 0, 4 },
  { 8, 0 },
  { 0, 8 },
  { 7, 6 }, // 20
  { 6, 7 },
  { 3, 6 },
  { 6, 3 },
  { 4, 6 },
  { 6, 4 },
  { 8, 6 },
  { 6, 8 },
  { 0, 6 },
  { 6, 0 },
  { 7, 5 }, // 30
  { 5, 7 },
  { 3, 5 },
  { 5, 3 },
  { 4, 5 },
  { 5, 4 },
  { 8, 5 },
  { 5, 8 },
  { 0, 5 },
  { 5, 0 },
  { 6, 5 }, // 40
  { 5, 6 },
  { 7, 1 },
  { 1, 7 },
  { 3, 1 },
  { 1, 3 },
  { 4, 1 },
  { 1, 4 },
  { 8, 1 },
  { 1, 8 },
  { 0, 1 }, // 50
  { 1, 0 },
  { 6, 1 },
  { 1, 6 },
  { 5, 1 },
  { 1, 5 },
  { 7, 2 },
  { 2, 7 },
  { 3, 2 },
  { 2, 3 },
  { 4, 2 },
  { 2, 4 },
  { 8, 2 },
  { 2, 8 },
  { 0, 2 },
  { 2, 0 },
  { 6, 2 },
  { 2, 6 },
  { 5, 2 },
  { 2, 5 },
  { 1, 2 },
  { 2, 1 },
  { 7, 10 },
  { 10, 7 },
  { 3, 10 },
  { 10, 3 },
  { 4, 10 },
  { 10, 4 },
  { 8, 10 },
  { 10, 8 },
  { 0, 10 },
  { 10, 0 },
  { 6, 10 },
  { 10, 6 },
  { 5, 10 },
  { 10, 5 },
  { 1, 10 },
  { 10, 1 },
  { 2, 10 },
  { 10, 2 },
  { 7, 9 },
  { 9, 7 },
  { 3, 9 },
  { 9, 3 },
  { 4, 9 },
  { 9, 4 },
};

#include "charmatrix.h"
void display_char(int letter) {

  for (int i = 0; i < 5; i++) {
    if (font[letter].d[i] & 1) {
      turnLed(3 + (12 * i), true);
    }
    if (font[letter].d[i] & 2) {
      turnLed(2 + (12 * i), true);
    }
    if (font[letter].d[i] & 4) {
      turnLed(1 + (12 * i), true);
    }
    if (font[letter].d[i] & 8) {
      turnLed(0 + (12 * i), true);
    }
  }
}

void turnLed(int idx, bool on) {
  size_t pin_anode = pins[idx][0] + led_zero_index;
  size_t pin_catode = pins[idx][1] + led_zero_index;

  for (int i = led_zero_index; i < led_zero_index + leds_howmany; i++) {
    //if (i != pin_anode && i != pin_catode) {
    pinMode(i, INPUT);
    //}
  }
  if (on) {
    pinMode(pin_anode, OUTPUT);
    pinMode(pin_catode, OUTPUT);
    digitalWrite(pin_anode, HIGH);
    digitalWrite(pin_catode, LOW);
  }
}

#include <ArduinoGraphics.h>

const byte canvasWidth = 12;
const byte canvasHeight = 8;

class LEDsDrawClass : public ArduinoGraphics {
  public:
    // can be used with an object of any class that inherits from the Print class
    LEDsDrawClass(Print &printObject = (Print &)Serial) :
      ArduinoGraphics(canvasWidth, canvasHeight),
      _printObject(&printObject) {
      memset((uint8_t*)_canvasBuffer, 0, canvasWidth * canvasHeight) ;
    }

    // this function is called by the ArduinoGraphics library's functions
    virtual void set(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
      // the r parameter is (mis)used to set the character to draw with
      _canvasBuffer[y][x] = r;
      // cast unused parameters to void to fix "unused parameter" warning
      (void)g;
      (void)b;
    }

    // display the drawing
    void endDraw() {
      ArduinoGraphics::endDraw();
      for (int i = 0; i < canvasWidth * canvasHeight; i++) {
        Serial.println(((uint8_t*)_canvasBuffer)[i]);
      }
    }

    uint8_t* getCanvas() {
      return (uint8_t*)_canvasBuffer;
    }

  private:
    Print *_printObject;
    char _canvasBuffer[canvasHeight][canvasWidth] = {{0}};
};

LEDsDrawClass LEDS;

volatile int i_isr = 0;
void __attribute__((naked)) turnOnLedISR(timer_callback_args_t *arg) {
  asm("push {LR}");
  turnLed(i_isr, (LEDS.getCanvas()[i_isr] > 0));
  i_isr = (i_isr + 1) % 96;
  asm("pop {r0}"); asm("bx r0");
}

int i = 0;
// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
  Serial.println(i++);
  Serial2.println("AT");
  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }
  Serial2.println("AT+WIFIBEGIN=BCMI,ArduinoccRulez");
  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }

  int j = 0;

#if 1
  while (1) {
    turnLed(j++ % 96, true);
    delay(33);
    if (j == 96) {
      break;
    }
  }

  FspTimer led_timer;

  uint8_t type;
  uint8_t ch = FspTimer::get_available_timer(type);
  led_timer.begin(TIMER_MODE_PERIODIC, type, ch, 10000.0, 50.0, turnOnLedISR);
  led_timer.setup_overflow_irq();
  led_timer.open();
  led_timer.start();

  draw_leds();

  while (1) {
    if (Serial.available()) {
      Serial.write(Serial.read());
    }
  }

  while (1) {
    for (int i = 0; i < 96; i++) {
      delayMicroseconds(100);
      turnLed(i, true);
    }
  }
#endif

  while (1) {
    while (Serial1.available()) {
      Serial.write(Serial1.read());
    }
    if (millis() % 500 == 0) {
      delay(1);
      j++;
    }
    display_char(j % (sizeof(font) / sizeof(font[0])));
  }
}
