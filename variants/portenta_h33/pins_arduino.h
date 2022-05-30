#ifdef __cplusplus
extern "C" unsigned int PINCOUNT_fn();
#endif

#define PIN(X,Y) (X * 16 + Y)

// Pin count
// ----
#define PINS_COUNT           (PINCOUNT_fn())
#define NUM_DIGITAL_PINS     (22u)
#define NUM_ANALOG_INPUTS    (7u)
#define NUM_ANALOG_OUTPUTS   (1u)

#define PIN_A0   (0u)
#define PIN_A1   (1u)
#define PIN_A2   (2u)
#define PIN_A3   (3u)
#define PIN_A4   (4u)
#define PIN_A5   (5u)
#define PIN_A6   (6u)
#define PIN_A7   (7u)

#define DAC      (A6)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;

// Digital pins
// -----------
#define D0  (0u)
#define D1  (1u)
#define D2  (2u)
#define D3  (3u)
#define D4  (4u)
#define D5  (5u)
#define D6  (6u)
#define D7  (7u)

// LEDs
// ----
#define PIN_LED     (24u)
#define LED_BUILTIN PIN_LED
#define LEDR        (23u)
#define LEDG        (24u)
#define LEDB        (25u)

#define SERIAL_HOWMANY		5

#define WIRE_HOWMANY      3
