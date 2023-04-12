#include "Arduino.h"
#include "FspTimer.h"

#define NUM_LEDS    96

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
#define load(x)     load_wrapper(x, sizeof(x))

static uint8_t __attribute__((aligned)) framebuffer[NUM_LEDS / 8];

class LED_Matrix {

public:
    LED_Matrix() {}
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
    void begin() {
        uint8_t type;
        uint8_t ch = FspTimer::get_available_timer(type);
        // TODO: avoid passing "this" argument to remove autoscroll
        led_timer.begin(TIMER_MODE_PERIODIC, type, ch, 10000.0, 50.0, turnOnLedISR, this);
        led_timer.setup_overflow_irq();
        led_timer.open();
        led_timer.start();
    }
    void next() {
        uint32_t frame[3];
        static int j = 0;
        frame[0] = reverse(*(_frames+(j*4)+0));
        frame[1] = reverse(*(_frames+(j*4)+1));
        frame[2] = reverse(*(_frames+(j*4)+2));
        _interval = *(_frames+(j*4)+3);
        j = (j + 1) % _lines;
        memcpy(framebuffer, (uint32_t*)frame, sizeof(frame));
    }
    void load_wrapper(const uint32_t frames[][4], uint32_t howMany) {
        _frames = (uint32_t*)frames;
        _lines = (howMany / 4) / sizeof(uint32_t);
    }
private:
    uint32_t* _frames;
    uint32_t _lines;
    uint32_t _interval = 0;
    uint32_t _last_interval = 0;
    FspTimer led_timer;

    static void turnOnLedISR(timer_callback_args_t *arg) {
        static volatile int i_isr = 0;
        turnLed(i_isr, ((framebuffer[i_isr >> 3] & (1 << (i_isr % 8))) != 0));
        i_isr = (i_isr + 1) % NUM_LEDS;
        if (arg != nullptr && arg->p_context != nullptr) {
            LED_Matrix* _m = (LED_Matrix*)arg->p_context;
            if (_m->_interval != 0 && millis() - _m->_last_interval > _m->_interval) {
                _m->next();
                _m->_last_interval = millis();
            }
        }
    }
};