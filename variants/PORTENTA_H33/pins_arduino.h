#pragma once

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
#define NUM_ANALOG_INPUTS    (7u)
#define NUM_ANALOG_OUTPUTS   (2u)

#define I2C_COUNT           (I2C_COUNT_fn())
#define SPI_COUNT           (SPI_COUNT_fn())
#define UART_COUNT          (UART_COUNT_fn())
#define SCI_COUNT           (SCI_COUNT_fn())

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



/****** UART CORE DEFINES ******/

#define SERIAL_HOWMANY		5

#define UART1_CHANNEL     5
#define UART2_CHANNEL     6
#define UART3_CHANNEL     7
#define UART4_CHANNEL     8
#define UART5_CHANNEL     9

/****** WIRE CORE DEFINES ******/

#define WIRE_HOWMANY      2

#define WIRE_CHANNEL      0
#define IS_WIRE_SCI       0

#define WIRE1_CHANNEL     1
#define IS_WIRE1_SCI      0

#define WIRE2_CHANNEL     2
#define IS_WIRE2_SCI      0

/****** SPI CORE DEFINES ******/

#define SPI_HOWMANY       2

#define SPI_CHANNEL       1
#define IS_SPI_SCI        0

#define SPI1_CHANNEL      4
#define IS_SPI1_SCI       1

#define PIN_SPI_SS    (44u)

static const uint8_t SS   = PIN_SPI_SS;

/****** CAN CORE DEFINES ******/

#define CAN_HOWMANY       2

#define CAN_STDBY     (52u)

/****** GTP CORE DEFINES *******/

#define GTP32_HOWMANY     4
#define GTP16_HOWMANY     6
#define GPT_HOWMANY       10

/****** AGT CORE DEFINES *******/
#define AGT_HOWMANY       6

#define EXT_INTERRUPTS_HOWMANY 8

#define USE_4BIT_SDCARD   1

#define USB_VID           (0x2341)
#define USB_PID           (0x0068)
#define USB_NAME          "Portenta H33"
