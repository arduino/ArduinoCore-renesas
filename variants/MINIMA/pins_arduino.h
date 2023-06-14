#pragma once

#define PIN(X,Y) (X * 16 + Y)

// Pin count
// ----
#define PINS_COUNT           (PINCOUNT_fn())
#define NUM_DIGITAL_PINS     (22u)
#define NUM_ANALOG_INPUTS    (6u)
#define NUM_ANALOG_OUTPUTS   (1u)

#define PIN_A0   (14u)
#define PIN_A1   (15u)
#define PIN_A2   (16u)
#define PIN_A3   (17u)
#define PIN_A4   (18u)
#define PIN_A5   (19u)

#define DAC8_HOWMANY (0)
#define DAC12_HOWMANY (1)
#define DAC         A0
#define IS_DAC(x)   (x == DAC ? true : false)
#define DAC_ADDRESS_12_CH0          R_DAC_BASE

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

/****** RTC CORE DEFINES *******/
#define RTC_HOWMANY     1


/****** UART CORE DEFINES ******/

#define SERIAL_HOWMANY		1
#define UART1_TX_PIN        1
#define UART1_RX_PIN        0

/****** WIRE CORE DEFINES ******/

#define WIRE_HOWMANY      1
#define WIRE_SDA_PIN      18 /* A4 */
#define WIRE_SCL_PIN      19 /* A5 */

static const uint8_t SDA = WIRE_SDA_PIN;
static const uint8_t SCL = WIRE_SDA_PIN;

/****** SPI CORE DEFINES ******/

#define SPI_HOWMANY       1

#define PIN_SPI_MOSI  (11)
#define PIN_SPI_MISO  (12)
#define PIN_SPI_SCK   (13)
#define PIN_SPI_CS    (10)
#define FORCE_SPI_MODE  (MODE_SPI)

static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t CS  =  PIN_SPI_CS;
static const uint8_t SS  =  PIN_SPI_CS;

/****** GTP CORE DEFINES *******/

#define GTP32_HOWMANY     2
#define GTP16_HOWMANY     6
#define GPT_HOWMANY       8

/****** AGT CORE DEFINES *******/
#define AGT_HOWMANY       2


/****** CAN CORE DEFINES ******/

#define CAN_HOWMANY       1

#define PIN_CAN0_TX       (4)
#define PIN_CAN0_RX       (5)
#define PIN_CAN0_STBY    (-1)


#define EXT_INTERRUPTS_HOWMANY 2

#define AVCC_MEASURE_PIN        20
#define AVCC_MULTIPLY_FACTOR    8.33

#define USB_VID           (0x2341)
#define USB_PID           (0x0069)
#define USB_NAME          "UNO R4 Minima"

/* EEPROM DEFINES */

#define ARDUINO_FLASH_TYPE  LP_FLASH
#define FLASH_BASE_ADDRESS  0x40100000
#define FLASH_TOTAL_SIZE    0x2000
#define FLASH_BLOCK_SIZE    0x400

// TODO: removeme
#ifdef __cplusplus
extern "C" {
#endif
void iic_slave_tei_isr(void);
void iic_slave_eri_isr(void);
#ifdef __cplusplus
}
#endif
