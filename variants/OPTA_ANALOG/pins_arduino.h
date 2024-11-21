#pragma once

#define PIN(X,Y) (X * 16 + Y)

#ifdef __cplusplus
extern "C" unsigned int PINCOUNT_fn();
#endif

// Pin count
// ----
#define PINS_COUNT           (PINCOUNT_fn())
#define NUM_DIGITAL_PINS     (14u)
#define NUM_ANALOG_INPUTS    (16u)
#define NUM_ANALOG_OUTPUTS   (0u)

/* ++++++++++++++++++++++++++++ DIGITAL OUTPUTS ++++++++++++++++++++++++++++ */
#define DIO_RESET_1                (0u)
#define DIO_RESET_2                (1u)
#define DIO_RTD_SWITCH_1           (2u)
#define DIO_RTD_SWITCH_2           (3u)
/* ++++++++++++++++++++++++++++++++++ PWMS +++++++++++++++++++++++++++++++++ */
#define PWM_0                      (4u)
#define PWM_1                      (7u)
#define PWM_2                      (6u)
#define PWM_3                      (5u)
#define PWM_FAULT_1                (8u)
#define PWM_FAULT_2                (9u)
/* ++++++++++++++++++++++++++++++++++ LEDS +++++++++++++++++++++++++++++++++ */
#define LED_1                      (10u)
#define LED_2                      (11u)
#define LED_3                      (12u)
#define LED_4                      (13u)
#define LED_5                      (14u)
#define LED_6                      (15u)
#define LED_7                      (16u)
#define LED_8                      (17u)

#define LED_ON                     HIGH
#define LED_OFF                    LOW

#define OPTA_LED_RED               (18u)
#define OPTA_LED_BLUE              (19u)
#define OPTA_LED_GREEN             (20u)

#define LED_RGB_ON                 LOW
#define LED_RGB_OFF                HIGH
/* +++++++++++++++++++++ IRQs FROM ANALOG BOARD ++++++++++++++++++++++++++++ */
#define ALERT_1_IRQ1               (21u)
#define ALERT_2_IRQ0               (22u)
#define ADC_READY_1_IRQ12          (23u)
#define ADC_READY_2_IRQ9           (24u)
/* +++++++++++++++++++++++++++++++ DETECTS +++++++++++++++++++++++++++++++++ */
#define DETECT_IN                  (25u)
#define DETECT_OUT                 (26u)
/* +++++++++++++++++++++++++++++++++ UART ++++++++++++++++++++++++++++++++++ */
#define UART_RX                    (27u)
#define UART_TX                    (28u)
/* +++++++++++++++++++++++++++++++++ I2C +++++++++++++++++++++++++++++++++++ */
#define I2C_SCL                    (29u)
#define I2C_SDA                    (30u)
/* +++++++++++++++++++++++++++++++++ SPI +++++++++++++++++++++++++++++++++++ */
#define SPI_MISO                   (31u)
#define SPI_MOSI                   (32u)
#define SPI_CK                     (33u)
#define SPI_CS_1                   (34u)
#define SPI_CS_2                   (35u)
/* +++++++++++++++++++++++++++++++++ SPI +++++++++++++++++++++++++++++++++++ */
#define LDAC1                      (36u)
#define LDAC2                      (37u)

/* ************************** UART CORE DEFINES **************************** */
#define SERIAL_HOWMANY		        1
#define UART1_TX_PIN               UART_TX
#define UART1_RX_PIN               UART_RX
/* ************************** WIRE CORE DEFINES **************************** */
#define WIRE_HOWMANY               1
#define WIRE_SDA_PIN               I2C_SDA 
#define WIRE_SCL_PIN               I2C_SCL 
static const uint8_t SDA = WIRE_SDA_PIN;
static const uint8_t SCL = WIRE_SDA_PIN;
/* *************************** SPI CORE DEFINES **************************** */
#define SPI_HOWMANY                1
#define PIN_SPI_MOSI               SPI_MOSI
#define PIN_SPI_MISO               SPI_MISO
#define PIN_SPI_SCK                SPI_CK
#define PIN_SPI_CS                 SPI_CS_1
#define FORCE_SPI_MODE             (MODE_SPI)
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t CS  =  PIN_SPI_CS;
static const uint8_t SS  =  PIN_SPI_CS;
/* *************************** GTP CORE DEFINES **************************** */
#define GTP32_HOWMANY              2
#define GTP16_HOWMANY              6
#define GPT_HOWMANY                8
/* *************************** AGT CORE DEFINES **************************** */
#define AGT_HOWMANY                2


/****** CAN CORE DEFINES ******/
#define CAN_HOWMANY       0
#define PIN_CAN0_TX       (4)
#define PIN_CAN0_RX       (5)
#define PIN_CAN0_STBY    (-1)

#define EXT_INTERRUPTS_HOWMANY 2



#define USB_VID           (0x2341)
#define USB_PID           (0x0071)
#define USB_NAME          "OPTA ANALOG"

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
