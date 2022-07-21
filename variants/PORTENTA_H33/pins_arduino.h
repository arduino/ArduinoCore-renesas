#ifdef __cplusplus
extern "C" unsigned int PINCOUNT_fn();
extern "C" unsigned int I2C_COUNT_fn();
extern "C" unsigned int SPI_COUNT_fn();
extern "C" unsigned int UART_COUNT_fn();
#endif

#define PIN(X,Y) (X * 16 + Y)

// Pin count
// ----
#define PINS_COUNT           (PINCOUNT_fn())
#define NUM_DIGITAL_PINS     (22u)
#define NUM_ANALOG_INPUTS    (7u)
#define NUM_ANALOG_OUTPUTS   (2u)

#define I2C_COUNT           (I2C_COUNT_fn())
#define SPI_COUNT           (SPI_COUNT_fn())
#define UART_COUNT          (UART_COUNT_fn())

#define PIN_A0   (8u)
#define PIN_A1   (9u)
#define PIN_A2   (10u)
#define PIN_A3   (11u)
#define PIN_A4   (12u)
#define PIN_A5   (13u)
#define PIN_A6   (14u)
#define PIN_A7   (15u)

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
#define PIN_LED     (25u)
#define LED_BUILTIN PIN_LED
#define LEDR        (24u)
#define LEDG        (25u)
#define LEDB        (26u)


#define USE_4BIT_SDCARD   1

#define CAN_STDBY     (52u)

#define PIN_SPI_SS    (44u)

static const uint8_t SS   = PIN_SPI_SS;

#define SERIAL_HOWMANY		5

#define WIRE_HOWMANY      2

#define SPI_HOWMANY       2

#define CAN_HOWMANY       2

#define EXT_INTERRUPTS_HOWMANY 8
