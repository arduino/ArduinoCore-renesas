#include "Arduino.h"

const AnalogPinDescription g_AAnalogPinDescription[] = {
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_0 },    // A0    ADC0_AN0
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_1 },    // A1    ADC0_AN1
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_2 },    // A2    ADC0_AN2
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_14},    // A3    ADC3_AN9
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_1 },    // A4    ADC1_AN21
  {&g_adc0_ctrl, &g_adc0_cfg, ADC_CHANNEL_0 }     // A5    ADC2_AN22
};

const AnalogOutPinDescription g_AAnalogOutPinDescription[] = {
  {&g_dac0_ctrl, &g_dac0_cfg }                // A3
};

pwmTable_t pwmTable[] = {
  {&g_timer0_ctrl, &g_timer0_cfg, GPT_IO_PIN_GTIOCA, NULL}, // D9
  {&g_timer0_ctrl, &g_timer0_cfg, GPT_IO_PIN_GTIOCB, NULL}, // D10
  {&g_timer1_ctrl, &g_timer1_cfg, GPT_IO_PIN_GTIOCA, NULL}, // D11
  {&g_timer1_ctrl, &g_timer1_cfg, GPT_IO_PIN_GTIOCB, NULL}, // D3
  {&g_timer2_ctrl, &g_timer2_cfg, GPT_IO_PIN_GTIOCA, NULL}, // D5
  {&g_timer2_ctrl, &g_timer2_cfg, GPT_IO_PIN_GTIOCB, NULL}, // D5
  {&g_timer3_ctrl, &g_timer3_cfg, GPT_IO_PIN_GTIOCA, NULL}, // D6
  {&g_timer3_ctrl, &g_timer3_cfg, GPT_IO_PIN_GTIOCB, NULL}, // D6
};

const irqTable_t irqTable[] = {
    {&g_external_irq0_ctrl, &g_external_irq0_cfg},    // ext_int0
    {&g_external_irq1_ctrl, &g_external_irq1_cfg},    // ext_int1
};

const PinDescription g_APinDescription[] = {
/*
    +------------------------+---------------+---------------------+--------------------------+
    |       PIN Name         |      PWM      |    EXT INTERRUPT    |          Notes           |
    |                        |               |                     |  Board Pin | Peripheral  |
    +------------------------+---------------+---------------------+--------------------------+
 */
    // Digital
    { BSP_IO_PORT_01_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D0     |   UART TX   */
    { BSP_IO_PORT_03_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D1     |   UART RX   */
    { BSP_IO_PORT_02_PIN_06,    NOT_ON_PWM,     EXTERNAL_INT_0   }, /*   D2     |             */
    { BSP_IO_PORT_01_PIN_04,    PWM_TIM1_CHB,   EXTERNAL_INT_1   }, /*   D3     |             */
    { BSP_IO_PORT_04_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D4     |             */
    { BSP_IO_PORT_01_PIN_03,    PWM_TIM2_CHA,   NOT_AN_INTERRUPT }, /*   D5     |             */
    { BSP_IO_PORT_01_PIN_12,    PWM_TIM3_CHB,   NOT_AN_INTERRUPT }, /*   D6     |             */
    { BSP_IO_PORT_04_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D7     |             */
    { BSP_IO_PORT_03_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D8     |             */
    { BSP_IO_PORT_03_PIN_00,    PWM_TIM0_CHA,   NOT_AN_INTERRUPT }, /*   D9     |             */
    { BSP_IO_PORT_01_PIN_08,    PWM_TIM0_CHB,   NOT_AN_INTERRUPT }, /*   D10    |     CS      */
    { BSP_IO_PORT_01_PIN_09,    PWM_TIM1_CHA,   NOT_AN_INTERRUPT }, /*   D11    |     MOSI    */
    { BSP_IO_PORT_01_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D12    |     MISO    */
    { BSP_IO_PORT_01_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D13    |     SCK     */
    { BSP_IO_PORT_01_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D14    |     SDA     */
    { BSP_IO_PORT_01_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D15    |     SCL     */

    // Analog
    { BSP_IO_PORT_00_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A0     |     AIN     */
    { BSP_IO_PORT_00_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A1     |     AIN     */
    { BSP_IO_PORT_00_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A2     |     AIN     */
    { BSP_IO_PORT_00_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A3     |   AIN/AOUT  */
    { BSP_IO_PORT_01_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A4     |     AIN     */
    { BSP_IO_PORT_01_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   A5     |     AIN     */

    // RGB LED
    { BSP_IO_PORT_05_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D22    |     LEDR    */
    { BSP_IO_PORT_00_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT }, /*   D23    |     LEDB    */

    { BSP_IO_PORT_01_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_01_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_01_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_01_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_02_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_02_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_03_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
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
    { BSP_IO_PORT_03_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_04_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_04_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_05_PIN_01,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_05_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_06_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_02,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_03,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_04,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_06,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_06_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
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
    { BSP_IO_PORT_08_PIN_05,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_07,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_08,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_09,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_10,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_11,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_12,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_13,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_14,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
    { BSP_IO_PORT_08_PIN_15,    NOT_ON_PWM,     NOT_AN_INTERRUPT },

    { BSP_IO_PORT_09_PIN_00,    NOT_ON_PWM,     NOT_AN_INTERRUPT },
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
};

extern "C" {
  unsigned int PINCOUNT_fn() {
    return (sizeof(g_APinDescription) / sizeof(g_APinDescription[0]));
  }
}

void initVariant() {

}
