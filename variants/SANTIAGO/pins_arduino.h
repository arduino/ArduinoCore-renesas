#ifdef __cplusplus
extern "C" unsigned int PINCOUNT_fn();
extern "C" unsigned int I2C_COUNT_fn();
extern "C" unsigned int SPI_COUNT_fn();
extern "C" unsigned int UART_COUNT_fn();
extern "C" unsigned int SCI_COUNT_fn();
#endif

#define PIN(X,Y) (X * 16 + Y)

// Pin count
// ----
#define PINS_COUNT           (PINCOUNT_fn())
#define NUM_DIGITAL_PINS     (22u)
#define NUM_ANALOG_INPUTS    (6u)
#define NUM_ANALOG_OUTPUTS   (1u)

#define I2C_COUNT           (I2C_COUNT_fn())
#define SPI_COUNT           (SPI_COUNT_fn())
#define UART_COUNT          (UART_COUNT_fn())
#define SCI_COUNT           (SCI_COUNT_fn())

#define PIN_A0   (16u)
#define PIN_A1   (17u)
#define PIN_A2   (18u)
#define PIN_A3   (19u)
#define PIN_A4   (20u)
#define PIN_A5   (21u)

#define DAC      (A0)

#undef A0
#undef A1
#undef A2
#undef A3
#undef A4
#undef A5
static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;

// Digital pins
// -----------
#undef D0
#undef D1
#undef D2
#undef D3
#undef D4
#undef D5
#undef D6
#undef D7
#undef D8
#undef D9
#undef D10
#undef D11
#undef D12
#undef D13
#undef D14
#undef D15
#define D0  (0u)
#define D1  (1u)
#define D2  (2u)
#define D3  (3u)
#define D4  (4u)
#define D5  (5u)
#define D6  (6u)
#define D7  (7u)
#define D8  (8u)
#define D9  (9u)
#define D10 (10u)
#define D11 (11u)
#define D12 (12u)
#define D13 (13u)
#define D14 (14u)
#define D15 (15u)

// LEDs
// ----
#define PIN_LED     (13u)
#define LED_BUILTIN PIN_LED
#define LEDR        (22u)
#define LEDG        (13u)
#define LEDB        (23u)


/****** UART CORE DEFINES ******/

#define SERIAL_HOWMANY		1

#define UART1_CHANNEL     2

/****** WIRE CORE DEFINES ******/

#define WIRE_HOWMANY      1

#define WIRE_CHANNEL      1
#define IS_WIRE_SCI       0

/****** SPI CORE DEFINES ******/

#define SPI_HOWMANY       1

#define SPI_CHANNEL       1
#define IS_SPI_SCI        0

#define PIN_SPI_SS    (10)

static const uint8_t SS   = PIN_SPI_SS;


#define EXT_INTERRUPTS_HOWMANY 2

#define BOOT_DOUBLE_TAP_ADDRESS           (0x20007f48ul)

#define USB_VID           (0x2341)
#define USB_PID           (0x0069)
#define USB_NAME          "Santiago"
