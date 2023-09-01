#pragma once

#define PIN(X,Y) (X * 16 + Y)

// Pin count
// ----
#ifdef __cplusplus
extern "C" unsigned int PINCOUNT_fn();
#endif
#define PINS_COUNT           (PINCOUNT_fn())
#define NUM_DIGITAL_PINS     (22u)
#define NUM_ANALOG_INPUTS    (6u)
#define NUM_ANALOG_OUTPUTS   (2u)

#define I2C_COUNT           (I2C_COUNT_fn())
#define SPI_COUNT           (SPI_COUNT_fn())
#define UART_COUNT          (UART_COUNT_fn())
#define SCI_COUNT           (SCI_COUNT_fn())

#undef DAC_MAX_CONVERSION_PERIOD
#define DAC_MAX_CONVERSION_PERIOD   1

#define DAC8_HOWMANY  (1)
#define DAC12_HOWMANY (1)
#define DAC         26
#define DAC2        25
#define IS_DAC(x)                                (x == DAC || x == DAC2)
#define DAC_ADDRESS_12_CH0                       R_DAC_BASE
#define DAC_ADDRESS_8_CH0                        R_DAC8_BASE
// TODO: check if true
#define DAC_ADDRESS_8_CH1                        (R_DAC8_BASE+1)

#define PIN_A0   (16u)
#define PIN_A1   (17u)
#define PIN_A2   (18u)
#define PIN_A3   (19u)
#define PIN_A4   (20u)
#define PIN_A5   (21u)
#define PIN_A6   (22u)



#undef A0
#undef A1
#undef A2
#undef A3
#undef A4
#undef A5
#undef A6

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;

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
#undef D16
#undef D17
#undef D18
#undef D19
#undef D20
#undef D21
#undef D22
#undef D23
#undef D24
#undef D25
#undef D26
#undef D27


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
#define D16 (16u)
#define D17 (17u)
#define D18 (18u)
#define D19 (19u)
#define D20 (20u)
#define D21 (21u)
#define D22 (22u)
#define D23 (23u)
#define D24 (24u)
#define D25 (25u)
#define D26 (26u)
#define D27 (27u)

// LEDs
// ----
#define PIN_LED     (4u)
#define LED_BUILTIN PIN_LED
//#define LEDR        (22u)
//#define LEDG        (13u)
//#define LEDB        (23u)

/****** UART CORE DEFINES ******/

#define SERIAL_HOWMANY	    2
#define UART1_TX_PIN        2
#define UART1_RX_PIN        3
#define UART2_TX_PIN        0
#define UART2_RX_PIN        1

#define USB_VID           (0x2341)
#define USB_PID           (0x006C)
#define USB_NAME          "SCIENCE R3"

#define NO_1200_BPS_RESET   1

#define GPT_HOWMANY       6
#define GTP32_HOWMANY     1
#define AGT_HOWMANY       2

/****** WIRE CORE DEFINES ******/

#define WIRE_HOWMANY      1
#define WIRE_SDA_PIN      20
#define WIRE_SCL_PIN      21

// TODO: removeme
#ifdef __cplusplus
extern "C"
#endif
void agt_int_isr(void);
