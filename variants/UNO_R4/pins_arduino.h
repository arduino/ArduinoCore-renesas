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

#define PIN_IRQ0   (23u)
#define PIN_IRQ1   (24u)
#define PIN_IRQ2   (25u)
#define PIN_IRQ3   (26u)
#define PIN_IRQ4   (27u)
#define PIN_IRQ5   (28u)
#define PIN_IRQ6   (29u)
#define PIN_IRQ7   (30u)
#define PIN_IRQ8   (31u)
#define PIN_IRQ9   (32u)

static const uint8_t IRQ0 = PIN_IRQ0;
static const uint8_t IRQ1 = PIN_IRQ1;
static const uint8_t IRQ2 = PIN_IRQ2;
static const uint8_t IRQ3 = PIN_IRQ3;
static const uint8_t IRQ4 = PIN_IRQ4;
static const uint8_t IRQ5 = PIN_IRQ5;
static const uint8_t IRQ6 = PIN_IRQ6;
static const uint8_t IRQ7 = PIN_IRQ7;
static const uint8_t IRQ8 = PIN_IRQ8;
static const uint8_t IRQ9 = PIN_IRQ9;

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

#define CAN_STDBY   (33u)

#define PIN_SPI_SS    (19)

static const uint8_t SS   = PIN_SPI_SS;

#define SERIAL_HOWMANY		5

#define WIRE_HOWMANY      3

#define SPI_HOWMANY       2

#define CAN_HOWMANY       2

#define EXT_INTERRUPTS_HOWMANY 10
