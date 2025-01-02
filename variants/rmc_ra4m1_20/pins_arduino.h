#pragma once

#define PIN(X,Y) (X * 16 + Y)

// Pin count
// ----
#ifdef __cplusplus
extern "C" unsigned int PINCOUNT_fn();
#endif
#define PINS_COUNT           (PINCOUNT_fn())
#define NUM_DIGITAL_PINS     (82u)
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
#define PIN_D0   (0u)
#define PIN_D1   (1u)
#define PIN_D2   (2u)
#define PIN_D3   (3u)
#define PIN_D4   (4u)
#define PIN_D5   (5u)
#define PIN_D6   (6u)
#define PIN_D7   (7u)
#define PIN_D8   (8u)
#define PIN_D9   (9u)
#define PIN_D10   (10u)
#define PIN_D11   (11u)
#define PIN_D12   (12u)
#define PIN_D13   (13u)
#define PIN_D14   (14u)
#define PIN_D15   (15u)
#define PIN_D16   (16u)
#define PIN_D17   (17u)
#define PIN_D18   (18u)
#define PIN_D19   (19u)
#define PIN_D20   (20u)
#define PIN_D21   (21u)
#define PIN_D22   (22u)
#define PIN_D23   (23u)
#define PIN_D24   (24u)
#define PIN_D25   (25u)
#define PIN_D26   (26u)
#define PIN_D27   (27u)
#define PIN_D28   (28u)
#define PIN_D29   (29u)
#define PIN_D30   (30u)
#define PIN_D31   (31u)
#define PIN_D32   (32u)
#define PIN_D33   (33u)
#define PIN_D34   (34u)
#define PIN_D35   (35u)
#define PIN_D36   (36u)
#define PIN_D37   (37u)
#define PIN_D38   (38u)
#define PIN_D39   (39u)
#define PIN_D40   (40u)
#define PIN_D41   (41u)
#define PIN_D42   (42u)
#define PIN_D43   (43u)
#define PIN_D44   (44u)
#define PIN_D45   (45u)
#define PIN_D46   (46u)
#define PIN_D47   (47u)
#define PIN_D48   (48u)
#define PIN_D49   (49u)
#define PIN_D50   (50u)
#define PIN_D51   (51u)
#define PIN_D52   (52u)
#define PIN_D53   (53u)
#define PIN_D54   (54u)
#define PIN_D55   (55u)
#define PIN_D56   (56u)
#define PIN_D57   (57u)
#define PIN_D58   (58u)
#define PIN_D59   (59u)
#define PIN_D60   (60u)
#define PIN_D61   (61u)
#define PIN_D62   (62u)
#define PIN_D63   (63u)
#define PIN_D64   (64u)
#define PIN_D65   (65u)
#define PIN_D66   (66u)
#define PIN_D67   (67u)
#define PIN_D68   (68u)
#define PIN_D69   (69u)
#define PIN_D70   (70u)
#define PIN_D71   (71u)
#define PIN_D72   (72u)
#define PIN_D73   (73u)
#define PIN_D74   (74u)
#define PIN_D75   (75u)
#define PIN_D76   (76u)
#define PIN_D77   (77u)
#define PIN_D78   (78u)

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
#undef D28
#undef D29
#undef D30
#undef D31
#undef D32
#undef D33
#undef D34
#undef D35
#undef D36
#undef D37
#undef D38
#undef D39
#undef D40
#undef D41
#undef D42
#undef D43
#undef D44
#undef D45
#undef D46
#undef D47
#undef D48
#undef D49
#undef D50
#undef D51
#undef D52
#undef D53
#undef D54
#undef D55
#undef D56
#undef D57
#undef D58
#undef D59
#undef D60
#undef D61
#undef D62
#undef D63
#undef D64
#undef D65
#undef D66
#undef D67
#undef D68
#undef D69
#undef D70
#undef D71
#undef D72
#undef D73
#undef D74
#undef D75
#undef D76
#undef D77
#undef D78

static const uint8_t D0 = PIN_D0;
static const uint8_t D1 = PIN_D1;
static const uint8_t D2 = PIN_D2;
static const uint8_t D3 = PIN_D3;
static const uint8_t D4 = PIN_D4;
static const uint8_t D5 = PIN_D5;
static const uint8_t D6 = PIN_D6;
static const uint8_t D7 = PIN_D7;
static const uint8_t D8 = PIN_D8;
static const uint8_t D9 = PIN_D9;
static const uint8_t D10 = PIN_D10;
static const uint8_t D11 = PIN_D11;
static const uint8_t D12 = PIN_D12;
static const uint8_t D13 = PIN_D13;
static const uint8_t D14 = PIN_D14;
static const uint8_t D15 = PIN_D15;
static const uint8_t D16 = PIN_D16;
static const uint8_t D17 = PIN_D17;
static const uint8_t D18 = PIN_D18;
static const uint8_t D19 = PIN_D19;
static const uint8_t D20 = PIN_D20;
static const uint8_t D21 = PIN_D21;
static const uint8_t D22 = PIN_D22;
static const uint8_t D23 = PIN_D23;
static const uint8_t D24 = PIN_D24;
static const uint8_t D25 = PIN_D25;
static const uint8_t D26 = PIN_D26;
static const uint8_t D27 = PIN_D27;
static const uint8_t D28 = PIN_D28;
static const uint8_t D29 = PIN_D29;
static const uint8_t D30 = PIN_D30;
static const uint8_t D31 = PIN_D31;
static const uint8_t D32 = PIN_D32;
static const uint8_t D33 = PIN_D33;
static const uint8_t D34 = PIN_D34;
static const uint8_t D35 = PIN_D35;
static const uint8_t D36 = PIN_D36;
static const uint8_t D37 = PIN_D37;
static const uint8_t D38 = PIN_D38;
static const uint8_t D39 = PIN_D39;
static const uint8_t D40 = PIN_D40;
static const uint8_t D41 = PIN_D41;
static const uint8_t D42 = PIN_D42;
static const uint8_t D43 = PIN_D43;
static const uint8_t D44 = PIN_D44;
static const uint8_t D45 = PIN_D45;
static const uint8_t D46 = PIN_D46;
static const uint8_t D47 = PIN_D47;
static const uint8_t D48 = PIN_D48;
static const uint8_t D49 = PIN_D49;
static const uint8_t D50 = PIN_D50;
static const uint8_t D51 = PIN_D51;
static const uint8_t D52 = PIN_D52;
static const uint8_t D53 = PIN_D53;
static const uint8_t D54 = PIN_D54;
static const uint8_t D55 = PIN_D55;
static const uint8_t D56 = PIN_D56;
static const uint8_t D57 = PIN_D57;
static const uint8_t D58 = PIN_D58;
static const uint8_t D59 = PIN_D59;
static const uint8_t D60 = PIN_D60;
static const uint8_t D61 = PIN_D61;
static const uint8_t D62 = PIN_D62;
static const uint8_t D63 = PIN_D63;
static const uint8_t D64 = PIN_D64;
static const uint8_t D65 = PIN_D65;
static const uint8_t D66 = PIN_D66;
static const uint8_t D67 = PIN_D67;
static const uint8_t D68 = PIN_D68;
static const uint8_t D69 = PIN_D69;
static const uint8_t D70 = PIN_D70;
static const uint8_t D71 = PIN_D71;
static const uint8_t D72 = PIN_D72;
static const uint8_t D73 = PIN_D73;
static const uint8_t D74 = PIN_D74;
static const uint8_t D75 = PIN_D75;
static const uint8_t D76 = PIN_D76;
static const uint8_t D77 = PIN_D77;
static const uint8_t D78 = PIN_D78;

// LEDs
// ----
#define PIN_LED           (13u)
#define LED_BUILTIN       PIN_LED

/****** RTC CORE DEFINES *******/
#define RTC_HOWMANY       1


/****** UART CORE DEFINES ******/

#define SERIAL_HOWMANY    3
#define UART1_TX_PIN      (1)
#define UART1_RX_PIN      (0)
#define UART2_TX_PIN      (11)
#define UART2_RX_PIN      (12)
#define UART3_TX_PIN      (18)
#define UART3_RX_PIN      (19)

/****** WIRE CORE DEFINES ******/

#define WIRE_HOWMANY      2
#define WIRE_SDA_PIN      (18) /* (A4)  */
#define WIRE_SCL_PIN      (19) /* (A5)  */
#define WIRE1_SDA_PIN     (54) /* (P407) */
#define WIRE1_SCL_PIN     (53) /* (P408) */

static const uint8_t SDA = WIRE_SDA_PIN;
static const uint8_t SCL = WIRE_SCL_PIN;

/****** SPI CORE DEFINES ******/

#define SPI_HOWMANY       2

#define PIN_SPI_MOSI      (11)
#define PIN_SPI_MISO      (12)
#define PIN_SPI_SCK       (13)
#define PIN_SPI_CS        (10)
#define FORCE_SPI_MODE    (MODE_SPI)

#define PIN_SPI1_MOSI     (79)
#define PIN_SPI1_MISO     (80)
#define PIN_SPI1_SCK      (81)
#define PIN_SPI1_CS       (23)
#define FORCE_SPI1_MODE   (MODE_SPI)

static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;
static const uint8_t CS  =  PIN_SPI_CS;
static const uint8_t SS  =  PIN_SPI_CS;

static const uint8_t MOSI1 = PIN_SPI1_MOSI;
static const uint8_t MISO1 = PIN_SPI1_MISO;
static const uint8_t SCK1  = PIN_SPI1_SCK;
static const uint8_t CS1  =  PIN_SPI1_CS;
static const uint8_t SS1  =  PIN_SPI1_CS;

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

#define VUSB_LDO_ENABLE     1

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
