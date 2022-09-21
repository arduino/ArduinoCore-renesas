#include "Arduino.h"
#include "pinmux.inc"

pwmTable_t pwmTable[] = {
  {&g_timer0_ctrl, &g_timer0_cfg, GPT_IO_PIN_GTIOCA, NULL}, // D9
  {&g_timer0_ctrl, &g_timer0_cfg, GPT_IO_PIN_GTIOCB, NULL}, // D10
  {&g_timer1_ctrl, &g_timer1_cfg, GPT_IO_PIN_GTIOCA, NULL}, // D11
  {&g_timer1_ctrl, &g_timer1_cfg, GPT_IO_PIN_GTIOCB, NULL}, // D3
  {&g_timer2_ctrl, &g_timer2_cfg, GPT_IO_PIN_GTIOCA, NULL}, // D5
  {nullptr,        nullptr,       GPT_IO_PIN_GTIOCB, NULL},
  {nullptr,        nullptr,       GPT_IO_PIN_GTIOCA, NULL},
  {&g_timer3_ctrl, &g_timer3_cfg, GPT_IO_PIN_GTIOCB, NULL}, // D6
};

const irqTable_t irqTable[] = {
    {&g_external_irq0_ctrl, &g_external_irq0_cfg},    // ext_int0
    {&g_external_irq1_ctrl, &g_external_irq1_cfg},    // ext_int1
};

void __attribute__((weak)) i2c_callback (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) sci_i2c_callback (i2c_master_callback_args_t * p_args) {}

void __attribute__((weak)) spi_callback(spi_callback_args_t *p_args) {}
void __attribute__((weak)) sci_spi_callback(spi_callback_args_t *p_args) {}

spi_instance_t SpiTable[] = {
  {nullptr            , nullptr           , nullptr             },    //SPI0
  {&g_spi0_ctrl       , &g_spi0_cfg       , &g_spi_on_spi       },    //SPI1
  {&g_spi1_ctrl       , &g_spi1_cfg       , &g_spi_on_sci       },    //SCI0
  {&g_spi2_ctrl       , &g_spi2_cfg       , &g_spi_on_sci       },    //SCI1
  {&g_spi3_ctrl       , &g_spi3_cfg       , &g_spi_on_sci       },    //SCI2
  {nullptr            , nullptr           , nullptr             },    //SCI3
  {nullptr            , nullptr           , nullptr             },    //SCI4
  {nullptr            , nullptr           , nullptr             },    //SCI5
  {nullptr            , nullptr           , nullptr             },    //SCI6
  {nullptr            , nullptr           , nullptr             },    //SCI7
  {nullptr            , nullptr           , nullptr             },    //SCI8
  {nullptr            , nullptr           , nullptr             },    //SCI9
};

sciTable_t SciTable[] {
/*
    +-----------------+------------------- +-------------------+
    |      UART       |        I2C         |        SPI        |
    +-----------------+------------------- +-------------------+
 */
  { nullptr           , nullptr            , &SpiTable[2]      },
  { nullptr           , nullptr            , &SpiTable[3]      },
  { nullptr           , nullptr            , &SpiTable[4]      },
  { nullptr           , nullptr            , nullptr           },
  { nullptr           , nullptr            , nullptr           },
  { nullptr           , nullptr            , nullptr           },
  { nullptr           , nullptr            , nullptr           },
  { nullptr           , nullptr            , nullptr           },
  { nullptr           , nullptr            , nullptr           },
  { nullptr           , nullptr            , nullptr           },
};

uint16_t getPinCfg(const uint16_t *cfg, PinCfgReq_t req, bool prefer_sci /*= false*/) {
  if(cfg == nullptr) {
    return 0;
  }
  bool thats_all = false;
  uint8_t index = 0;
  while(!thats_all) {

    /* usually not SCI peripheral have higher priority (they came
       first in the table) but it is possible to prefer SCI peripheral */
    if(prefer_sci && !IS_SCI(*(cfg + index))) {
      if(IS_LAST_ITEM(*(cfg + index))) {
        thats_all = true;
      }
      else {
        index++;
      }
      continue;
    }


    if(PIN_CFG_REQ_UART_TX == req && IS_PIN_UART_TX(*(cfg + index))) {
      return *(cfg + index);
    }
    else if(PIN_CFG_REQ_UART_RX == req && IS_PIN_UART_RX(*(cfg + index))) {
      return *(cfg + index);
    }
    else if(PIN_CFG_REQ_SCL == req && IS_PIN_SCL(*(cfg + index))) {
      return *(cfg + index);
    }
    else if(PIN_CFG_REQ_SDA == req && IS_PIN_SDA(*(cfg + index))) {
      return *(cfg + index);
    }
    else if(PIN_CFG_REQ_MISO == req && IS_PIN_MISO(*(cfg + index))) {
      return *(cfg + index);
    }
    else if(PIN_CFG_REQ_MOSI == req && IS_PIN_MOSI(*(cfg + index))) {
      return *(cfg + index);
    }
    else if(PIN_CFG_REQ_PWM == req && IS_PIN_PWM(*(cfg + index))) {
      return *(cfg + index);
    }
    else if(PIN_CFG_REQ_INTERRUPT == req && IS_PIN_INTERRUPT(*(cfg + index))) {
      return *(cfg + index);
    }
    else if(PIN_CFG_REQ_ADC == req && IS_PIN_ANALOG(*(cfg + index))) {
      return *(cfg + index);
    }

    if(IS_LAST_ITEM(*(cfg + index))) {
      thats_all = true;
    }
    else {
      index++;
    }
  }
  return 0;
} 

extern "C" const PinMuxCfg_t g_pin_cfg[] = { 
  { BSP_IO_PORT_03_PIN_01,    P301   }, /* (0) D0  -------------------------  DIGITAL  */
  { BSP_IO_PORT_01_PIN_02,    P102   }, /* (1) D1  */
  { BSP_IO_PORT_02_PIN_06,    P206   }, /* (2) D2  */
  { BSP_IO_PORT_01_PIN_04,    P104   }, /* (3) D3  */
  { BSP_IO_PORT_04_PIN_00,    P105   }, /* (4) D4  */
  { BSP_IO_PORT_01_PIN_03,    P113   }, /* (5) D5  */
  { BSP_IO_PORT_01_PIN_12,    P112   }, /* (6) D6  */
  { BSP_IO_PORT_04_PIN_09,    P409   }, /* (7) D7  */
  { BSP_IO_PORT_03_PIN_02,    P302   }, /* (8) D8  */
  { BSP_IO_PORT_03_PIN_00,    P106   }, /* (9) D9  */
  { BSP_IO_PORT_01_PIN_08,    P205   }, /* (10) D10 */
  { BSP_IO_PORT_01_PIN_09,    P109   }, /* (11) D11 */
  { BSP_IO_PORT_01_PIN_10,    P110   }, /* (12) D12 */
  { BSP_IO_PORT_01_PIN_11,    P111   }, /* (13) D13 */
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (14) D14 */
  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (15) D15 */
  { BSP_IO_PORT_00_PIN_14,    P014   }, /* (16) A0  --------------------------  ANALOG  */
  { BSP_IO_PORT_00_PIN_00,    P000   }, /* (17) A1  */
  { BSP_IO_PORT_00_PIN_01,    P001   }, /* (18) A2  */
  { BSP_IO_PORT_00_PIN_02,    P002   }, /* (19) A3  */
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (20) A4  */
  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (21) A5  */
  { BSP_IO_PORT_03_PIN_03,    P303   }, /* (22) D22 --------------------------- RGB LED */
  { BSP_IO_PORT_03_PIN_04,    P304   }, /* (23) D23 */
  { BSP_IO_PORT_00_PIN_12,    P012   }, /* (24) D24 ------------------------- TX/RX LED */
  { BSP_IO_PORT_04_PIN_08,    P408   }, /* (25) D25 */
};

extern "C" const size_t g_pin_cfg_size = sizeof(g_pin_cfg);

const PinDescription g_APinDescription[] = {
/*
    +------------------------+---------------+---------------------+----------------+--------------------------+
    |       PIN Name         |      PWM      |    EXT INTERRUPT    |                           Notes           |
    |                        |               |                     |                   Board Pin | Peripheral  |
    +------------------------+---------------+---------------------+----------------+--------------------------+
 */
    // Digital
    { BSP_IO_PORT_03_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     SERIAL_BUS,    IOPORT_PERIPHERAL_SCI0_2_4_6_8}, /*   D0     |   UART RX   */
    { BSP_IO_PORT_01_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     SERIAL_BUS,    IOPORT_PERIPHERAL_SCI1_3_5_7_9}, /*   D1     |   UART TX   */
    { BSP_IO_PORT_02_PIN_06,    NOT_ON_PWM,     EXTERNAL_INT_0,       NOT_A_BUS     }, /*   D2     |             */
    { BSP_IO_PORT_01_PIN_04,    PWM_TIM1_CHB,   EXTERNAL_INT_1,       NOT_A_BUS     }, /*   D3     |             */
    { BSP_IO_PORT_04_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D4     |             */
    { BSP_IO_PORT_01_PIN_03,    PWM_TIM2_CHA,   NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D5     |             */
    { BSP_IO_PORT_01_PIN_12,    PWM_TIM3_CHB,   NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D6     |             */
    { BSP_IO_PORT_04_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D7     |             */
    { BSP_IO_PORT_03_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D8     |             */
    { BSP_IO_PORT_03_PIN_00,    PWM_TIM0_CHA,   NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D9     |             */
    { BSP_IO_PORT_01_PIN_08,    PWM_TIM0_CHB,   NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D10    |     CS      */
    { BSP_IO_PORT_01_PIN_09,    PWM_TIM1_CHA,   NOT_AN_INTERRUPT,     SPI_BUS       }, /*   D11    |     MOSI    */
    { BSP_IO_PORT_01_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     SPI_BUS       }, /*   D12    |     MISO    */
    { BSP_IO_PORT_01_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     SPI_BUS       }, /*   D13    |     SCK     */
    { BSP_IO_PORT_01_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     WIRE_BUS      }, /*   D14    |     SDA     */
    { BSP_IO_PORT_01_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     WIRE_BUS      }, /*   D15    |     SCL     */

    // Analog
    { BSP_IO_PORT_00_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   A0     |   AIN/AOUT  */
    { BSP_IO_PORT_00_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   A1     |     AIN     */
    { BSP_IO_PORT_00_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   A2     |     AIN     */
    { BSP_IO_PORT_00_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   A3     |     AIN     */
    { BSP_IO_PORT_01_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   A4     |     AIN     */
    { BSP_IO_PORT_01_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   A5     |     AIN     */

    // RGB LED
    { BSP_IO_PORT_00_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D22    |     LEDR    */
    { BSP_IO_PORT_00_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D23    |     LEDB    */
    // TX and RX LEDS
    { BSP_IO_PORT_00_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D24    |     TX LED  */
    { BSP_IO_PORT_02_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     }, /*   D25    |     RX LED  */


    { BSP_IO_PORT_01_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_01_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_01_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_01_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },

    { BSP_IO_PORT_02_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    
    { BSP_IO_PORT_02_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_02_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },

    { BSP_IO_PORT_03_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_03_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },

    { BSP_IO_PORT_04_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_04_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },

    { BSP_IO_PORT_05_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_05_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },

    { BSP_IO_PORT_06_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_06_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },

    { BSP_IO_PORT_07_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_07_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },

    { BSP_IO_PORT_08_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_08_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },

    { BSP_IO_PORT_09_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
    { BSP_IO_PORT_09_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT,     NOT_A_BUS     },
};

extern "C" {
  unsigned int PINCOUNT_fn() {
    return (sizeof(g_APinDescription) / sizeof(g_APinDescription[0]));
  }
  unsigned int SPI_COUNT_fn() {
    return (sizeof(SpiTable) / sizeof(SpiTable[1]));
  }
  unsigned int SCI_COUNT_fn() {
    return (sizeof(SciTable) / sizeof(SciTable[0]));
  }
}

void initVariant() {

}
