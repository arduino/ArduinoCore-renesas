#pragma once

#define PIN(X,Y) (X * 16 + Y)

// Pin count
// ----
#ifdef __cplusplus
extern "C" unsigned int PINCOUNT_fn();
#endif
#define PINS_COUNT           (PINCOUNT_fn())
#define NUM_DIGITAL_PINS     (45u)
#define NUM_ANALOG_INPUTS    (6u)
#define NUM_ANALOG_OUTPUTS   (2u)

#define PIN_A0   (7u)
#define PIN_A1   (8u)
#define PIN_A2   (9u)
#define PIN_A3   (10u)
#define PIN_A4   (11u)
#define PIN_A5   (12u)


#define DAC8_HOWMANY (0)
#define DAC12_HOWMANY (2)
#define DAC         7
#define DAC1        8
#define IS_DAC(x)   (x == DAC || x == DAC1)
#define DAC_ADDRESS_12_CH0          R_DAC_BASE
// TODO: check if true
#define DAC_ADDRESS_12_CH1          (R_DAC_BASE+2)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;

// Digital pins
// -----------
#define PIN_D0   (0u)
#define PIN_D1   (1u)
#define PIN_D2   (2u)
#define PIN_D3   (3u)
#define PIN_D4   (4u)
#define PIN_D5   (5u)
#define PIN_D6   (6u)

#undef D0
#undef D1
#undef D2
#undef D3
#undef D4
#undef D5
#undef D6
#undef D7
static const uint8_t D0 = PIN_D0;
static const uint8_t D1 = PIN_D1;
static const uint8_t D2 = PIN_D2;
static const uint8_t D3 = PIN_D3;
static const uint8_t D4 = PIN_D4;
static const uint8_t D5 = PIN_D5;
static const uint8_t D6 = PIN_D6;

// LEDs
// ----
#define PIN_LED     (14u)
#define LED_BUILTIN PIN_LED
#define LED_RGB     (13u)

/******  BUTTON DEFINES  *******/
#define USR_BTN     (31u)


/****** RTC CORE DEFINES *******/
#define RTC_HOWMANY       1

/****** UART CORE DEFINES ******/

#define SERIAL_HOWMANY	    2
#define UART1_TX_PIN        20
#define UART1_RX_PIN        21
#define UART2_TX_PIN        32
#define UART2_RX_PIN        33
#define UART2_RTS_PIN       34
#define UART2_CTS_PIN       35

/****** WIRE CORE DEFINES ******/

#define WIRE_HOWMANY      1
#define WIRE_SDA_PIN      15
#define WIRE_SCL_PIN      16

/****** SPI CORE DEFINES ******/

#define SPI_HOWMANY       1

#define PIN_SPI_MOSI      18
#define PIN_SPI_MISO      17
#define PIN_SPI_SCK       19
#define PIN_SPI_CS        0
#define FORCE_SPI_MODE    (MODE_SCI)

static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t CS  =  PIN_SPI_CS;
static const uint8_t SS  =  PIN_SPI_CS;

static const uint8_t POCI = PIN_SPI_MISO;
static const uint8_t PICO = PIN_SPI_MOSI;

// /****** CAN CORE DEFINES ******/

#define CANFD_HOWMANY     0

// /****** GTP CORE DEFINES *******/

#define GTP32_HOWMANY     4
#define GTP16_HOWMANY     6
#define GPT_HOWMANY       10

// /****** AGT CORE DEFINES *******/
#define AGT_HOWMANY       6

/****** ETHERNET CORE DEFINES *******/
#define ETHERNET_HOWMANY   0

/****** SDCARD CORE DEFINES *******/
#define SDCARD_HOWMANY     1

#define USE_4BIT_SDCARD   1

/****** USB CORE DEFINES ******/
#define USB_VID           (0x1b4f)
#define USB_PID           (0x0037)
#define USB_NAME          "SparkFun Thing Plus RA6M5"

/****** FLASH DEFINES ******/
#define ARDUINO_FLASH_TYPE  HP_FLASH
#define FLASH_BASE_ADDRESS  0x08000000
#define FLASH_TOTAL_SIZE    0x2000
#define FLASH_BLOCK_SIZE    0x40

/****** QSPI CORE DEFINES ******/

#define PIN_QSPI_CLK     (41)
#define PIN_QSPI_SS      (42)
#define PIN_QSPI_D0      (43)
#define PIN_QSPI_D1      (44)
#define PIN_QSPI_D2      (45)
#define PIN_QSPI_D3      (46)

#define HAS_QSPI

#define QSPI_TOTAL_SIZE       (0x1000000) 
#define QSPI_READ_BLOCK_SIZE  (0x1)
#define QSPI_ERASE_BLOCK_SIZE (0x1000)
#define QSPI_WRITE_BLOCK_SIZE (0x1)

/****** MISC GPIO CORE DEFINES ******/
#define EXT_INTERRUPTS_HOWMANY 15

#define SD_NEN            (36)
#define BLE_NRST          (37)
#define VREF_EN           (38)
#define PWR_STAT          (39)

#define HAS_SDHI

#define PIN_SDHI_CLK (22)
#define PIN_SDHI_CMD (23)
#define PIN_SDHI_D0  (24)
#define PIN_SDHI_D1  (25)
#define PIN_SDHI_D2  (26)
#define PIN_SDHI_D3  (27)
#define PIN_SDHI_CD  (28)  
#define PIN_SDHI_WP  (29)

#define RTC_CLOCK_SOURCE    RTC_CLOCK_SOURCE_SUBCLK
