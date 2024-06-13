#pragma once

#define PIN(X,Y) (X * 16 + Y)

#ifdef __cplusplus
extern "C" unsigned int PINCOUNT_fn();
#endif

// Pin count
// ----
#define PINS_COUNT           (PINCOUNT_fn())
#define NUM_DIGITAL_PINS     (8u)
#define NUM_ANALOG_INPUTS    (16u)
#define NUM_ANALOG_OUTPUTS   (0u)


#define DAC8_HOWMANY (0)
#define DAC12_HOWMANY (0)
#define DAC         A0
#define IS_DAC(x)   (x == DAC ? true : false)
#define DAC_ADDRESS_12_CH0          R_DAC_BASE

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
#undef A11
#undef A12
#undef A13
#undef A14
#undef A15


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

/* ++++++++++++++++++++++++++++ DIGITAL OUTPUTS ++++++++++++++++++++++++++++ */
#define D0                       (0u)
#define D1                       (1u)
#define D2                       (2u)
#define D3                       (3u)
#define D4                       (4u)
#define D5                       (5u)
#define D6                       (6u)
#define D7                       (7u)
/* ++++++++++++++++++++++++++++++++++ LEDS +++++++++++++++++++++++++++++++++ */
#define OPTA_LED_RED             (8u)
#define OPTA_LED_BLUE            (9u)
#define OPTA_LED_GREEN           (10u)

#define LED_RGB_ON                 LOW
#define LED_RGB_OFF                HIGH
/* ++++++++++++++++++++++++++++++++++ CAN ++++++++++++++++++++++++++++++++++ */
#define CAN_ENABLE                 (11u)
#define CAN_TX                     (12u)
#define CAN_RX                     (13u)
/* ++++++++++++++++++++++++++++ ANALOG INPUTS ++++++++++++++++++++++++++++++ */
#define AN0                        (14u)
#define AN1                        (15u)
#define AN2                        (16u)
#define AN3                        (17u)
#define AN4                        (18u)
#define AN5                        (19u)
#define AN6                        (20u)
#define AN7                        (21u)
#define AN8                        (22u)
#define AN9                        (23u)
#define AN10                       (24u)
#define AN11                       (25u)
#define AN12                       (26u)
#define AN13                       (27u)
#define AN14                       (28u)
#define AN15                       (29u)
/* +++++++++++++++++++++++++++++++ DETECTS +++++++++++++++++++++++++++++++++ */
#define DETECT_IN                  (30u)
#define DETECT_OUT                 (31u)
/* +++++++++++++++++++++++++++++++++ UART ++++++++++++++++++++++++++++++++++ */
#define UART_RX                    (32u)
#define UART_TX                    (33u)
/* +++++++++++++++++++++++++++++++++ I2C +++++++++++++++++++++++++++++++++++ */
#define I2C_SDA                    (34u)
#define I2C_SCL                    (35u)
/* +++++++++++++++++++++++++++++++++ RS485 +++++++++++++++++++++++++++++++++ */
#define RS485_DE                   (36u)
#define RS485_RE_                  (37u)
#define RS485_RX                   (38u)
#define RS485_TX                   (39u)


static const uint8_t A0 = AN0;
static const uint8_t A1 = AN1;
static const uint8_t A2 = AN2;
static const uint8_t A3 = AN3;
static const uint8_t A4 = AN4;
static const uint8_t A5 = AN5;
static const uint8_t A6 = AN6;
static const uint8_t A7 = AN7;
static const uint8_t A8 = AN8;
static const uint8_t A9 = AN9;
static const uint8_t A10 = AN10;
static const uint8_t A11 = AN11;
static const uint8_t A12 = AN12;
static const uint8_t A13 = AN13;
static const uint8_t A14 = AN14;
static const uint8_t A15 = AN15;


#undef PIN_LED
#undef LED_BUILTIN

/****** RTC CORE DEFINES *******/
#define RTC_HOWMANY     0

/****** UART CORE DEFINES ******/
#define SERIAL_HOWMANY		 1
#define UART1_TX_PIN        UART_RX
#define UART1_RX_PIN        UART_TX

/****** WIRE CORE DEFINES ******/
#define WIRE_HOWMANY      1
#define WIRE_SDA_PIN      I2C_SDA 
#define WIRE_SCL_PIN      I2C_SCL 

static const uint8_t SDA = WIRE_SDA_PIN;
static const uint8_t SCL = WIRE_SDA_PIN;

/****** SPI CORE DEFINES ******/

#define SPI_HOWMANY       0
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
#define PIN_CAN0_TX       CAN_TX
#define PIN_CAN0_RX       CAN_RX
#define PIN_CAN0_STBY     CAN_ENABLE

#define EXT_INTERRUPTS_HOWMANY 2



#define USB_VID           (0x2341)
#define USB_PID           (0x006E)
#define USB_NAME          "OPTA DIGITAL"

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
