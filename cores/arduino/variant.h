#ifndef ARDUINO_VARIANT_H
#define ARDUINO_VARIANT_H

typedef struct
{
  bsp_io_port_pin_t        pin;
  const uint16_t          *list;
} PinMuxCfg_t ;


typedef enum {
    PIN_CFG_REQ_UART_TX,
    PIN_CFG_REQ_UART_RX,
    PIN_CFG_REQ_SCL,
    PIN_CFG_REQ_SDA,
    PIN_CFG_REQ_MISO,
    PIN_CFG_REQ_MOSI,
    PIN_CFG_REQ_SCK,
    PIN_CFG_REQ_PWM,
    PIN_CFG_REQ_INTERRUPT,
    PIN_CFG_REQ_CTS_RTS,
    PIN_CFG_REQ_ADC,
    PIN_CFG_REQ_CAN_RX,
    PIN_CFG_REQ_CAN_TX,
    PIN_CFG_REQ_DAC,
    /* to be completed */
} PinCfgReq_t;

int32_t getPinIndex(bsp_io_port_pin_t p);
#ifdef __cplusplus
#include <array>
std::array<uint16_t, 3> getPinCfgs(const pin_size_t pin, PinCfgReq_t req);
#endif

#define SCI_CHANNEL                 0x1
#define NOT_SCI_CHANNEL             0x0
#define IS_SCI(x)                   (x & SCI_CHANNEL)

#define PIN_USE_POS                 (1)
#define PIN_USE_MASK                (0x3E)

#define PIN_UNAVAILABLE             (0 << PIN_USE_POS)
#define PIN_INTERRUPT               (1 << PIN_USE_POS)
#define PIN_SCL                     (2 << PIN_USE_POS)
#define PIN_SDA                     (3 << PIN_USE_POS)
#define PIN_RX_MISO_SCL             (4 << PIN_USE_POS)
#define PIN_TX_MOSI_SDA             (5 << PIN_USE_POS)
#define PIN_MISO                    (6 << PIN_USE_POS)
#define PIN_MOSI                    (7 << PIN_USE_POS)
#define PIN_SCK                     (8 << PIN_USE_POS)
#define PIN_PWM                     (9 << PIN_USE_POS)
#define PIN_ANALOG                  (10 << PIN_USE_POS)
#define PIN_CTS_RTS_SS              (11 << PIN_USE_POS)
#define PIN_PWM_GPT                 (9 << PIN_USE_POS)
#define PIN_PWM_AGT                 (12 << PIN_USE_POS)
#define PIN_CAN_TX                  (13 << PIN_USE_POS)
#define PIN_CAN_RX                  (14 << PIN_USE_POS)
#define PIN_CTSRTS                  PIN_CTS_RTS_SS
#define PIN_DAC                     (15 << PIN_USE_POS)

/* ..... MAX TO 63 */

#define IS_PIN_UNAVAILABLE(x)       ((x & PIN_USE_MASK) ==  PIN_UNAVAILABLE)
#define IS_PIN_INTERRUPT(x)         ((x & PIN_USE_MASK) ==  PIN_INTERRUPT)

#define IS_PIN_UART_RX(x)           ((x & PIN_USE_MASK) ==  PIN_RX_MISO_SCL)
#define IS_PIN_UART_TX(x)           ((x & PIN_USE_MASK) ==  PIN_TX_MOSI_SDA)

#define IS_PIN_SCL(x)               (((x & PIN_USE_MASK) ==  PIN_SCL) || ((x & PIN_USE_MASK) ==  PIN_RX_MISO_SCL))
#define IS_PIN_SDA(x)               (((x & PIN_USE_MASK) ==  PIN_SDA) || ((x & PIN_USE_MASK) ==  PIN_TX_MOSI_SDA))

#define IS_PIN_MISO(x)              (((x & PIN_USE_MASK) ==  PIN_MISO) || ((x & PIN_USE_MASK) ==  PIN_RX_MISO_SCL))
#define IS_PIN_MOSI(x)              (((x & PIN_USE_MASK) ==  PIN_MOSI) || ((x & PIN_USE_MASK) ==  PIN_TX_MOSI_SDA))

#define IS_PIN_SCK(x)               ((x & PIN_USE_MASK) ==  PIN_SCK)
#define IS_PIN_PWM(x)               (((x & PIN_USE_MASK) ==  PIN_PWM_GPT) || ((x & PIN_USE_MASK) ==  PIN_PWM_AGT))
#define IS_PIN_AGT_PWM(x)           ((x & PIN_USE_MASK) ==  PIN_PWM_AGT)
#define IS_PIN_GPT_PWM(x)           ((x & PIN_USE_MASK) ==  PIN_PWM_GPT)

#define IS_PIN_ANALOG(x)            ((x & PIN_USE_MASK) ==  PIN_ANALOG)

#define IS_PIN_CAN_RX(x)            ((x & PIN_USE_MASK) ==  PIN_CAN_RX)
#define IS_PIN_CAN_TX(x)            ((x & PIN_USE_MASK) ==  PIN_CAN_TX)

#define IS_PIN_DAC(x)               ((x & PIN_USE_MASK) ==  PIN_DAC)


#define CHANNEL_POS                 (6)
#define CHANNEL_MASK                (0x7C0)
#define CHANNEL_0                   (0 << CHANNEL_POS)
#define CHANNEL_1                   (1 << CHANNEL_POS)
#define CHANNEL_2                   (2 << CHANNEL_POS)
#define CHANNEL_3                   (3 << CHANNEL_POS)
#define CHANNEL_4                   (4 << CHANNEL_POS)
#define CHANNEL_5                   (5 << CHANNEL_POS)
#define CHANNEL_6                   (6 << CHANNEL_POS)
#define CHANNEL_7                   (7 << CHANNEL_POS)
#define CHANNEL_8                   (8 << CHANNEL_POS)
#define CHANNEL_9                   (9 << CHANNEL_POS)
#define CHANNEL_10                  (10 << CHANNEL_POS)
#define CHANNEL_11                  (11 << CHANNEL_POS)
#define CHANNEL_12                  (12 << CHANNEL_POS)
#define CHANNEL_13                  (13 << CHANNEL_POS)
#define CHANNEL_14                  (14 << CHANNEL_POS)
#define CHANNEL_15                  (15 << CHANNEL_POS)
#define CHANNEL_16                  (16 << CHANNEL_POS)
#define CHANNEL_17                  (17 << CHANNEL_POS)
#define CHANNEL_18                  (18 << CHANNEL_POS)
#define CHANNEL_19                  (19 << CHANNEL_POS)
#define CHANNEL_20                  (20 << CHANNEL_POS)
#define CHANNEL_21                  (21 << CHANNEL_POS)
#define CHANNEL_22                  (22 << CHANNEL_POS)
#define CHANNEL_23                  (23 << CHANNEL_POS)
#define CHANNEL_24                  (24 << CHANNEL_POS)
#define CHANNEL_25                  (25 << CHANNEL_POS)
#define CHANNEL_26                  (26 << CHANNEL_POS)
#define CHANNEL_27                  (27 << CHANNEL_POS)
#define CHANNEL_28                  (28 << CHANNEL_POS)

#define GET_CHANNEL(x)              ((x & CHANNEL_MASK) >> CHANNEL_POS)

/* no more */

#define ADD_CONFIG_POS              (11)
#define ADD_CONFIG_MASK             (0x7800)
/* they depend upon the pin use ... max 15 cfg possible */

#define PWM_CHANNEL_A               (0 << ADD_CONFIG_POS)
#define IS_PWM_ON_A(x)              ((x & ADD_CONFIG_MASK) == PWM_CHANNEL_A)
#define PWM_CHANNEL_B               (1 << ADD_CONFIG_POS)
#define IS_PWM_ON_B(x)              ((x & ADD_CONFIG_MASK) == PWM_CHANNEL_B)

#define SCI_EVEN_CFG                (0 << ADD_CONFIG_POS)
#define USE_SCI_EVEN_CFG(x)         ((x & ADD_CONFIG_MASK) == SCI_EVEN_CFG)
#define SCI_ODD_CFG                 (1 << ADD_CONFIG_POS)
#define USE_SCI_ODD_CFG(x)          ((x & ADD_CONFIG_MASK) == SCI_ODD_CFG)

#define GPT_EVEN_CFG                0
#define GPT_ODD_CFG                 0

#define ADD_CFG_DONT_CARE           (15 << ADD_CONFIG_POS)
#define IS_ADD_CFG_DONT_CARE(x)     ((x & ADD_CONFIG_MASK) == ADD_CFG_DONT_CARE)

#define DAC_8BIT                    (1 << ADD_CONFIG_POS)
#define IS_DAC_8BIT(x)              ((x & ADD_CONFIG_MASK) == DAC_8BIT)

#define LAST_ITEM_MASK              (0x8000)
#define NOT_LAST_ITEM               (0 << 15)
#define LAST_ITEM_GUARD             (1 << 15)
#define IS_LAST_ITEM(x)             ((x & LAST_ITEM_MASK) ==  LAST_ITEM_GUARD)

#define ADC_0                       (0 << ADD_CONFIG_POS)
#define IS_ADC0(x)                  ((x & ADD_CONFIG_MASK) == ADC_0)
#define ADC_1                       (1 << ADD_CONFIG_POS)
#define IS_ADC1(x)                  ((x & ADD_CONFIG_MASK) == ADC_1)

#define LP_FLASH                    1
#define HP_FLASH                    2

#endif
