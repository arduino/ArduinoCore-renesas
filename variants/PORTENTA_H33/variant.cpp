#include "Arduino.h"

const AnalogPinDescription g_AAnalogPinDescription[] = {
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_0 },    // A0    ADC2_INP0
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_1 },    // A1    ADC2_INP1
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_2 },    // A2    ADC3_INP0
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_4 },    // A3    ADC3_INP1
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_5 },    // A4    ADC1_INP12
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_6 },    // A5    ADC2_INP13
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_14 },   // A6    ADC1_INP18
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_15 }    // A7    ADC1_INP7
};

const AnalogOutPinDescription g_AAnalogOutPinDescription[] = {
  {&g_dac0_ctrl, &g_dac0_cfg },               // A6    DAC_CH0
  {&g_dac1_ctrl, &g_dac1_cfg },                // A7    DAC_CH1
};

pwmTable_t pwmTable[] = {
  {&g_timer0_ctrl, &g_timer0_cfg, GPT_IO_PIN_GTIOCA, NULL},
  {nullptr,        nullptr,       GPT_IO_PIN_GTIOCB, NULL},
  {nullptr,        nullptr,       GPT_IO_PIN_GTIOCA, NULL},
  {nullptr,        nullptr,       GPT_IO_PIN_GTIOCB, NULL},
  {&g_timer2_ctrl, &g_timer2_cfg, GPT_IO_PIN_GTIOCA, NULL},
  {nullptr,        nullptr,       GPT_IO_PIN_GTIOCB, NULL},
  {nullptr,        nullptr,       GPT_IO_PIN_GTIOCA, NULL},
  {&g_timer3_ctrl, &g_timer3_cfg, GPT_IO_PIN_GTIOCB, NULL},
  {&g_timer4_ctrl, &g_timer4_cfg, GPT_IO_PIN_GTIOCA, NULL},
  {&g_timer4_ctrl, &g_timer4_cfg, GPT_IO_PIN_GTIOCB, NULL},
  {nullptr,        nullptr,       GPT_IO_PIN_GTIOCA, NULL},
  {&g_timer5_ctrl, &g_timer5_cfg, GPT_IO_PIN_GTIOCB, NULL},
  {&g_timer6_ctrl, &g_timer6_cfg, GPT_IO_PIN_GTIOCA, NULL},
  {&g_timer6_ctrl, &g_timer6_cfg, GPT_IO_PIN_GTIOCB, NULL},
};

const irqTable_t irqTable[] = {
    {&g_external_irq0_ctrl, &g_external_irq0_cfg},    // ext_int0
    {&g_external_irq1_ctrl, &g_external_irq1_cfg},    // ext_int1
    {&g_external_irq2_ctrl, &g_external_irq2_cfg},    // ext_int2
    {&g_external_irq3_ctrl, &g_external_irq3_cfg},    // ext_int3
    {&g_external_irq4_ctrl, &g_external_irq4_cfg},    // ext_int4
    {&g_external_irq5_ctrl, &g_external_irq5_cfg},    // ext_int5
    {&g_external_irq6_ctrl, &g_external_irq6_cfg},    // ext_int6
    {&g_external_irq7_ctrl, &g_external_irq7_cfg},    // ext_int7
};

uart_instance_t UartTable[] = {
  {&g_uart0_ctrl, &g_uart0_cfg, &g_uart_on_sci},
  {&g_uart1_ctrl, &g_uart1_cfg, &g_uart_on_sci},
  {&g_uart2_ctrl, &g_uart2_cfg, &g_uart_on_sci},
  {&g_uart3_ctrl, &g_uart3_cfg, &g_uart_on_sci},
  {&g_uart4_ctrl, &g_uart4_cfg, &g_uart_on_sci},
  {&g_uart5_ctrl, &g_uart5_cfg, &g_uart_on_sci},
  {&g_uart6_ctrl, &g_uart6_cfg, &g_uart_on_sci},
  {&g_uart7_ctrl, &g_uart7_cfg, &g_uart_on_sci},
  {&g_uart8_ctrl, &g_uart8_cfg, &g_uart_on_sci},
  {&g_uart9_ctrl, &g_uart9_cfg, &g_uart_on_sci},
};

void __attribute__((weak)) isr_i2c0 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c1 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c2 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c3 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c4 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c5 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c6 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c7 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c8 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c9 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c10 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c11 (i2c_master_callback_args_t * p_args) {}
void __attribute__((weak)) isr_i2c12 (i2c_master_callback_args_t * p_args) {}

i2c_master_instance_t I2CMasterTable[] = {
  {&g_i2c_master0_ctrl, &g_i2c_master0_cfg, &g_i2c_master_on_iic},
  {&g_i2c_master1_ctrl, &g_i2c_master1_cfg, &g_i2c_master_on_iic},
  {&g_i2c_master2_ctrl, &g_i2c_master2_cfg, &g_i2c_master_on_iic},
  {&g_i2c3_ctrl,        &g_i2c3_cfg,        &g_i2c_master_on_sci},
  {&g_i2c4_ctrl,        &g_i2c4_cfg,        &g_i2c_master_on_sci},
  {&g_i2c5_ctrl,        &g_i2c5_cfg,        &g_i2c_master_on_sci},
  {&g_i2c6_ctrl,        &g_i2c6_cfg,        &g_i2c_master_on_sci},
  {&g_i2c7_ctrl,        &g_i2c7_cfg,        &g_i2c_master_on_sci},
  {&g_i2c8_ctrl,        &g_i2c8_cfg,        &g_i2c_master_on_sci},
  {&g_i2c9_ctrl,        &g_i2c9_cfg,        &g_i2c_master_on_sci},
  {&g_i2c10_ctrl,       &g_i2c10_cfg,       &g_i2c_master_on_sci},
  {&g_i2c11_ctrl,       &g_i2c11_cfg,       &g_i2c_master_on_sci},
  {&g_i2c12_ctrl,       &g_i2c12_cfg,       &g_i2c_master_on_sci},
};

spi_instance_t SpiTable[] = {
  {&g_spi0_ctrl, &g_spi0_cfg, &g_spi_on_spi},
  {&g_spi1_ctrl, &g_spi1_cfg, &g_spi_on_spi},
  {&g_spi2_ctrl, &g_spi2_cfg, &g_spi_on_sci},
  {&g_spi3_ctrl, &g_spi3_cfg, &g_spi_on_sci},
};

sciTable_t SciTable[] {
/*
    +-----------------+------------------- +-------------------+
    |      UART       |        I2C         |        SPI        |
    +-----------------+------------------- +-------------------+
 */
  { &UartTable[5]     , &I2CMasterTable[6] , &SpiTable[3]      },
  { &UartTable[6]     , &I2CMasterTable[3] , nullptr           },
  { &UartTable[7]     , &I2CMasterTable[4] , nullptr           },
  { &UartTable[8]     , &I2CMasterTable[5] , nullptr           },
  { &UartTable[9]     , &I2CMasterTable[7] , &SpiTable[2]      },
  { &UartTable[0]     , &I2CMasterTable[8] , nullptr           },
  { &UartTable[1]     , &I2CMasterTable[9] , nullptr           },
  { &UartTable[2]     , &I2CMasterTable[10], nullptr           },
  { &UartTable[3]     , &I2CMasterTable[11], nullptr           },
  { &UartTable[4]     , &I2CMasterTable[12], nullptr           },
};

PinDescription g_APinDescription[] = {
/*
    +------------------------+---------------+---------------------+--------------------------+
    |       PIN Name         |      PWM      |    EXT INTERRUPT    |          Notes           |
    |                        |               |                     |  Board Pin | Peripheral  |
    +------------------------+---------------+---------------------+--------------------------+
 */
    // Digital -- PWMs
    { BSP_IO_PORT_01_PIN_05,    PWM_TIM0_CHA,   NOT_AN_INTERRUPT }, /*   D0     |             */
    { BSP_IO_PORT_01_PIN_11,    PWM_TIM2_CHA,   NOT_AN_INTERRUPT }, /*   D1     |             */
    { BSP_IO_PORT_06_PIN_08,    PWM_TIM3_CHB,   NOT_AN_INTERRUPT }, /*   D2     |             */
    { BSP_IO_PORT_06_PIN_01,    PWM_TIM4_CHA,   NOT_AN_INTERRUPT }, /*   D3     |             */
    { BSP_IO_PORT_04_PIN_01,    PWM_TIM4_CHB,   NOT_AN_INTERRUPT }, /*   D4     |             */
    { BSP_IO_PORT_03_PIN_03,    PWM_TIM5_CHB,   NOT_AN_INTERRUPT }, /*   D5     |             */
    { BSP_IO_PORT_06_PIN_05,    PWM_TIM6_CHA,   NOT_AN_INTERRUPT }, /*   D6     |             */
    { BSP_IO_PORT_01_PIN_06,    PWM_TIM6_CHB,   NOT_AN_INTERRUPT }, /*   D7     |             */

    // Analog
    { BSP_IO_PORT_00_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A0     |     AIN     */
    { BSP_IO_PORT_00_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A1     |     AIN     */
    { BSP_IO_PORT_00_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A2     |     AIN     */
    { BSP_IO_PORT_00_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A3     |     AIN     */
    { BSP_IO_PORT_00_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A4     |     AIN     */
    { BSP_IO_PORT_00_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A5     |     AIN     */
    { BSP_IO_PORT_00_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A6     |   AIN/AOUT  */
    { BSP_IO_PORT_00_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A7     |   AIN/AOUT  */

    // EXT_Interrupts
    { BSP_IO_PORT_08_PIN_03,    NOT_ON_PWM,     EXTERNAL_INT_0   }, /*   D16    |             */
    { BSP_IO_PORT_08_PIN_02,    NOT_ON_PWM,     EXTERNAL_INT_1   }, /*   D17    |             */
    { BSP_IO_PORT_10_PIN_10,    NOT_ON_PWM,     EXTERNAL_INT_2   }, /*   D18    |             */
    { BSP_IO_PORT_04_PIN_09,    NOT_ON_PWM,     EXTERNAL_INT_3   }, /*   D19    |             */
    { BSP_IO_PORT_07_PIN_07,    NOT_ON_PWM,     EXTERNAL_INT_4   }, /*   D20    |             */
    { BSP_IO_PORT_07_PIN_08,    NOT_ON_PWM,     EXTERNAL_INT_5   }, /*   D21    |             */
    { BSP_IO_PORT_00_PIN_09,    NOT_ON_PWM,     EXTERNAL_INT_6   }, /*   D22    |             */
    { BSP_IO_PORT_05_PIN_05,    NOT_ON_PWM,     EXTERNAL_INT_7   }, /*   D23    |             */

    // RGB LED
    { BSP_IO_PORT_00_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D24    |     LEDR    */
    { BSP_IO_PORT_00_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D25    |     LEDG    */
    { BSP_IO_PORT_00_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D26    |     LEDB    */

    // I2C pins
    { BSP_IO_PORT_04_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D27    |     SDA     */
    { BSP_IO_PORT_04_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D28    |     SCL     */
    { BSP_IO_PORT_05_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D29    |     SDA1    */
    { BSP_IO_PORT_05_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D30    |     SCL1    */

    // UART pins
    { BSP_IO_PORT_08_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D31    |   UART TX   */
    { BSP_IO_PORT_05_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D32    |   UART RX   */
    { BSP_IO_PORT_05_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D33    |   UART1 TX  */
    { BSP_IO_PORT_03_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D34    |   UART1 RX  */
    { BSP_IO_PORT_06_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D35    |   UART2 TX  */
    { BSP_IO_PORT_06_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D36    |   UART2 RX  */
    { BSP_IO_PORT_10_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D37    |   UART3 TX  */
    { BSP_IO_PORT_06_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D38    |   UART3 RX  */
    { BSP_IO_PORT_06_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D39    |   UART4 TX  */
    { BSP_IO_PORT_01_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D40    |   UART4 RX  */

    // SPI pins
    { BSP_IO_PORT_01_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D41    |     MISO    */
    { BSP_IO_PORT_01_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D42    |     MOSI    */
    { BSP_IO_PORT_01_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D43    |     SCLK    */
    { BSP_IO_PORT_01_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D44    |    SPI_SS   */
    { BSP_IO_PORT_03_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D45    |     MISO1   */
    { BSP_IO_PORT_09_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D46    |     MOSI1   */
    { BSP_IO_PORT_02_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D47    |     SCLK1   */

    // CAN pins
    { BSP_IO_PORT_02_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D48    |   CAN RX    */
    { BSP_IO_PORT_02_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D49    |   CAN TX    */
    { BSP_IO_PORT_06_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D50    |   CAN1 TX   */
    { BSP_IO_PORT_06_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D51    |   CAN1 RX   */
    { BSP_IO_PORT_06_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D52    |  CAN_STDBY  */

    { BSP_IO_PORT_00_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_00_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_00_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_00_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_00_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_00_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_01_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_01_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_01_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_01_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_01_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_01_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_01_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_02_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_03_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_03_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_04_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_05_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_06_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_07_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_07_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_08_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_09_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_09_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_10_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_10_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
/*
    { BSP_IO_PORT_11_PIN_00 },
    { BSP_IO_PORT_11_PIN_01 },
    { BSP_IO_PORT_11_PIN_02 },
    { BSP_IO_PORT_11_PIN_03 },
    { BSP_IO_PORT_11_PIN_04 },
    { BSP_IO_PORT_11_PIN_05 },
    { BSP_IO_PORT_11_PIN_06 },
    { BSP_IO_PORT_11_PIN_07 },
    { BSP_IO_PORT_11_PIN_08 },
    { BSP_IO_PORT_11_PIN_09 },
    { BSP_IO_PORT_11_PIN_10 },
    { BSP_IO_PORT_11_PIN_11 },
    { BSP_IO_PORT_11_PIN_12 },
    { BSP_IO_PORT_11_PIN_13 },
    { BSP_IO_PORT_11_PIN_14 },
    { BSP_IO_PORT_11_PIN_15 },

    { BSP_IO_PORT_12_PIN_00 },
    { BSP_IO_PORT_12_PIN_01 },
    { BSP_IO_PORT_12_PIN_02 },
    { BSP_IO_PORT_12_PIN_03 },
    { BSP_IO_PORT_12_PIN_04 },
    { BSP_IO_PORT_12_PIN_05 },
    { BSP_IO_PORT_12_PIN_06 },
    { BSP_IO_PORT_12_PIN_07 },
    { BSP_IO_PORT_12_PIN_08 },
    { BSP_IO_PORT_12_PIN_09 },
    { BSP_IO_PORT_12_PIN_10 },
    { BSP_IO_PORT_12_PIN_11 },
    { BSP_IO_PORT_12_PIN_12 },
    { BSP_IO_PORT_12_PIN_13 },
    { BSP_IO_PORT_12_PIN_14 },
    { BSP_IO_PORT_12_PIN_15 },

    { BSP_IO_PORT_13_PIN_00 },
    { BSP_IO_PORT_13_PIN_01 },
    { BSP_IO_PORT_13_PIN_02 },
    { BSP_IO_PORT_13_PIN_03 },
    { BSP_IO_PORT_13_PIN_04 },
    { BSP_IO_PORT_13_PIN_05 },
    { BSP_IO_PORT_13_PIN_06 },
    { BSP_IO_PORT_13_PIN_07 },
    { BSP_IO_PORT_13_PIN_08 },
    { BSP_IO_PORT_13_PIN_09 },
    { BSP_IO_PORT_13_PIN_10 },
    { BSP_IO_PORT_13_PIN_11 },
    { BSP_IO_PORT_13_PIN_12 },
    { BSP_IO_PORT_13_PIN_13 },
    { BSP_IO_PORT_13_PIN_14 },
    { BSP_IO_PORT_13_PIN_15 },

    { BSP_IO_PORT_14_PIN_00 },
    { BSP_IO_PORT_14_PIN_01 },
    { BSP_IO_PORT_14_PIN_02 },
    { BSP_IO_PORT_14_PIN_03 },
    { BSP_IO_PORT_14_PIN_04 },
    { BSP_IO_PORT_14_PIN_05 },
    { BSP_IO_PORT_14_PIN_06 },
    { BSP_IO_PORT_14_PIN_07 },
    { BSP_IO_PORT_14_PIN_08 },
    { BSP_IO_PORT_14_PIN_09 },
    { BSP_IO_PORT_14_PIN_10 },
    { BSP_IO_PORT_14_PIN_11 },
    { BSP_IO_PORT_14_PIN_12 },
    { BSP_IO_PORT_14_PIN_13 },
    { BSP_IO_PORT_14_PIN_14 },
    { BSP_IO_PORT_14_PIN_15 },
    */
};

extern "C" {
  unsigned int PINCOUNT_fn() {
    return (sizeof(g_APinDescription) / sizeof(g_APinDescription[0]));
  }
  unsigned int I2C_COUNT_fn() {
    return (sizeof(I2CMasterTable) / sizeof(I2CMasterTable[0]));
  }
  unsigned int SPI_COUNT_fn() {
    return (sizeof(SpiTable) / sizeof(SpiTable[0]));
  }
  unsigned int UART_COUNT_fn() {
    return (sizeof(UartTable) / sizeof(UartTable[0]));
  }
}

void initVariant() {

}
