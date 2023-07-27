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
#define NUM_DIGITAL_PINS     (13u)
#define NUM_ANALOG_INPUTS    (9u)
#define NUM_ANALOG_OUTPUTS   (2u)

#define I2C_COUNT           (I2C_COUNT_fn())
#define SPI_COUNT           (SPI_COUNT_fn())
#define UART_COUNT          (UART_COUNT_fn())
#define SCI_COUNT           (SCI_COUNT_fn())

#undef DAC_MAX_CONVERSION_PERIOD
#define DAC_MAX_CONVERSION_PERIOD   1

#define DAC8_HOWMANY  (1)
#define DAC12_HOWMANY (1)
#define DAC1         1
#define DAC0        0
#define IS_DAC(x)                                (x == DAC0 || x == DAC1)
#define DAC_ADDRESS_12_CH0                       R_DAC_BASE
#define DAC_ADDRESS_8_CH0                        R_DAC8_BASE
// TODO: check if true
#define DAC_ADDRESS_8_CH1                        (R_DAC8_BASE+1)

#define PIN_A0   (0u)
#define PIN_A1   (1u)
#define PIN_A2   (2u)
#define PIN_A3   (3u)
//#define PIN_A4   (13u)
//#define PIN_A5   (14u)
#define PIN_A6   (6u)
#define PIN_A7   (7u)
#define PIN_A8   (8u)
#define PIN_A9   (9u)
#define PIN_A10  (10u)



#undef A0
#undef A1
#undef A2
#undef A3
#undef A4
#undef A5
#undef A6
#undef A7
#undef A8
#undef A9
#undef A10

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
//static const uint8_t A4 = PIN_A4;
//static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;
static const uint8_t A8 = PIN_A8;
static const uint8_t A9 = PIN_A9;
static const uint8_t A10 = PIN_A10;

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


// LEDs
// ----
#define PIN_LED     (11u)
#define LED_BUILTIN PIN_LED


/****** UART CORE DEFINES ******/

#define SERIAL_HOWMANY	    2
#define UART1_TX_PIN        6
#define UART1_RX_PIN        7
#define UART2_TX_PIN        2
#define UART2_RX_PIN        1

#define USB_VID           (0x2341)
#define USB_PID           (0x006D)
#define USB_NAME          "Arduino Capri"

#define NO_1200_BPS_RESET   1

#define GPT_HOWMANY       6
#define GTP32_HOWMANY     1
#define AGT_HOWMANY       2

/****** WIRE CORE DEFINES ******/

#define WIRE_HOWMANY      2
#define WIRE_SDA_PIN      4
#define WIRE_SCL_PIN      5
#define WIRE1_SDA_PIN     6
#define WIRE1_SCL_PIN     7

/****** SPI CORE DEFINES ******/

#define SPI_HOWMANY       1

#define PIN_SPI_MOSI  (10)
#define PIN_SPI_MISO  (9)
#define PIN_SPI_SCK   (8)
#define PIN_SPI_CS    (-1)

#define FORCE_SPI_MODE  (MODE_SPI)

static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

/* EEPROM DEFINES */
/* this uses internal flash as eeprom
#define ARDUINO_FLASH_TYPE  LP_FLASH
#define FLASH_BASE_ADDRESS  0x40100000
#define FLASH_TOTAL_SIZE    0x2000
#define FLASH_BLOCK_SIZE    0x400
*/

// TODO: removeme
#ifdef __cplusplus
extern "C"
#endif
void agt_int_isr(void);
