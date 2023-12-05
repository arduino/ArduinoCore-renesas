#include "Arduino.h"
#include "FspTimer.h"
#include "gallery.h"

#define NUM_LEDS    96

#if __has_include("ArduinoGraphics.h")
#include <ArduinoGraphics.h>
#define MATRIX_WITH_ARDUINOGRAPHICS
#endif

static const int pin_zero_index = 28;

static const uint8_t pins[][2] = {
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

#define LED_MATRIX_PORT0_MASK       ((1 << 3) | (1 << 4) | (1 << 11) | (1 << 12) | (1 << 13) | (1 << 15))
#define LED_MATRIX_PORT2_MASK       ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 12) | (1 << 13))

static void turnLed(int idx, bool on) {
  R_PORT0->PCNTR1 &= ~((uint32_t) LED_MATRIX_PORT0_MASK);
  R_PORT2->PCNTR1 &= ~((uint32_t) LED_MATRIX_PORT2_MASK);

  if (on) {
    bsp_io_port_pin_t pin_a = g_pin_cfg[pins[idx][0] + pin_zero_index].pin;
    R_PFS->PORT[pin_a >> 8].PIN[pin_a & 0xFF].PmnPFS =
      IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_HIGH;

    bsp_io_port_pin_t pin_c = g_pin_cfg[pins[idx][1] + pin_zero_index].pin;
    R_PFS->PORT[pin_c >> 8].PIN[pin_c & 0xFF].PmnPFS =
      IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_LOW;
  }
}

static uint32_t reverse(uint32_t x)
{
    x = ((x >> 1) & 0x55555555u) | ((x & 0x55555555u) << 1);
    x = ((x >> 2) & 0x33333333u) | ((x & 0x33333333u) << 2);
    x = ((x >> 4) & 0x0f0f0f0fu) | ((x & 0x0f0f0f0fu) << 4);
    x = ((x >> 8) & 0x00ff00ffu) | ((x & 0x00ff00ffu) << 8);
    x = ((x >> 16) & 0xffffu) | ((x & 0xffffu) << 16);
    return x;
}

// TODO: this is dangerous, use with care
#define loadSequence(frames)                loadWrapper(frames, sizeof(frames))
#define renderBitmap(bitmap, rows, columns) loadPixels(&bitmap[0][0], rows*columns)

static uint8_t __attribute__((aligned)) framebuffer[NUM_LEDS / 8];

class ArduinoLEDMatrix
#ifdef MATRIX_WITH_ARDUINOGRAPHICS
    : public ArduinoGraphics
#endif
     {

public:
    ArduinoLEDMatrix()
    #ifdef MATRIX_WITH_ARDUINOGRAPHICS
        : ArduinoGraphics(canvasWidth, canvasHeight)
    #endif
    {}
    // TODO: find a better name
    // autoscroll will be slower than calling next() at precise times
    void autoscroll(uint32_t interval_ms) {
        _interval = interval_ms;
    }
    void on(size_t pin) {
        turnLed(pin, true);
    }
    void off(size_t pin) {
        turnLed(pin, false);
    }
    int begin() {
        uint8_t type;
        uint8_t ch = FspTimer::get_available_timer(type);
        // TODO: avoid passing "this" argument to remove autoscroll
        _ledTimer.begin(TIMER_MODE_PERIODIC, type, ch, 10000.0, 50.0, turnOnLedISR, this);
        _ledTimer.setup_overflow_irq();
        _ledTimer.open();
        _ledTimer.start();
    }
    void next() {
        uint32_t frame[3];
        frame[0] = reverse(*(_frames+(_currentFrame*4)+0));
        frame[1] = reverse(*(_frames+(_currentFrame*4)+1));
        frame[2] = reverse(*(_frames+(_currentFrame*4)+2));
        _interval = *(_frames+(_currentFrame*4)+3);
        _currentFrame = (_currentFrame + 1) % _framesCount;
        if(_currentFrame == 0){
            if(!_loop){
                _interval = 0;
            }
            if(_callBack != nullptr){
                _callBack();
            }
            _sequenceDone = true;
        }
        memcpy(framebuffer, (uint32_t*)frame, sizeof(frame));
    }
    void loadFrame(const uint32_t buffer[3]){
        uint32_t tempBuffer[][4] = {{
            buffer[0], buffer[1], buffer[2], 0
        }};
        loadSequence(tempBuffer);
        next();
        _interval = 0;
    }
    void renderFrame(uint8_t frameNumber){
        _currentFrame = frameNumber % _framesCount;
        next();
        _interval = 0;
    }
    void play(bool loop = false){
        _loop = loop;
        _sequenceDone = false;
        next();
    }
    bool sequenceDone(){
        if(_sequenceDone){
            _sequenceDone = false;
            return true;
        }
        return false;
    }

    void loadPixels(uint8_t *arr, size_t size){
        uint32_t partialBuffer = 0;
        uint8_t pixelIndex = 0;
        uint8_t *frameP = arr;
        uint32_t *frameHolderP = _frameHolder;
        while (pixelIndex < size) {
            partialBuffer |= *frameP++;
            if ((pixelIndex + 1) % 32 == 0) {
                *(frameHolderP++) = partialBuffer;
            }
            partialBuffer = partialBuffer << 1;
            pixelIndex++;
        }
        loadFrame(_frameHolder);
    };

    void loadWrapper(const uint32_t frames[][4], uint32_t howMany) {
        _currentFrame = 0;
        _frames = (uint32_t*)frames;
        _framesCount = (howMany / 4) / sizeof(uint32_t);
    }
    // WARNING: callbacks are fired from ISR. The execution time will be limited.
    void setCallback(voidFuncPtr callBack){
        _callBack = callBack;
    }

#ifdef MATRIX_WITH_ARDUINOGRAPHICS
    virtual void set(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
      if (y >= canvasHeight || x >= canvasWidth) {
        return;
      }
      // the r parameter is (mis)used to set the character to draw with
      _canvasBuffer[y][x] = (r | g | b) > 0 ? 1 : 0;
    }

    void endText(int scrollDirection = NO_SCROLL) {
      ArduinoGraphics::endText(scrollDirection);
      renderBitmap(_canvasBuffer, canvasHeight, canvasWidth);
    }

    // display the drawing
    void endDraw() {
      ArduinoGraphics::endDraw();
      // clear first line (no idea why it gets filled with random bits, probably some math not working fine for super small displays)
      for (int i = 0; i < canvasWidth; i++) {
        _canvasBuffer[0][i] = 0;
      }
      renderBitmap(_canvasBuffer, canvasHeight, canvasWidth);
    }

  private:
    static const byte canvasWidth = 12;
    static const byte canvasHeight = 8;
    uint8_t _canvasBuffer[canvasHeight][canvasWidth] = {{0}};
#endif

private:
    int _currentFrame = 0;
    uint32_t _frameHolder[3];
    uint32_t* _frames;
    uint32_t _framesCount;
    uint32_t _interval = 0;
    uint32_t _lastInterval = 0;
    bool _loop = false;
    FspTimer _ledTimer;
    bool _sequenceDone = false;
    voidFuncPtr _callBack;

    static void turnOnLedISR(timer_callback_args_t *arg) {
        static volatile int i_isr = 0;
        turnLed(i_isr, ((framebuffer[i_isr >> 3] & (1 << (i_isr % 8))) != 0));
        i_isr = (i_isr + 1) % NUM_LEDS;
        if (arg != nullptr && arg->p_context != nullptr) {
            ArduinoLEDMatrix* _m = (ArduinoLEDMatrix*)arg->p_context;
            if (_m->_interval != 0 && millis() - _m->_lastInterval > _m->_interval) {
                _m->next();
                _m->_lastInterval = millis();
            }
        }
    }
};