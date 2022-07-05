#include "Arduino.h"

const AnalogPinDescription g_AAnalogPinDescription[] = {
  { BSP_IO_PORT_00_PIN_00,   ADC_CHANNEL_0 },    // A0    ADC2_INP0
  { BSP_IO_PORT_00_PIN_01,   ADC_CHANNEL_1 },    // A1    ADC2_INP1
  { BSP_IO_PORT_00_PIN_02,   ADC_CHANNEL_2 },    // A2    ADC3_INP0
  { BSP_IO_PORT_00_PIN_04,   ADC_CHANNEL_4 },    // A3    ADC3_INP1
  { BSP_IO_PORT_00_PIN_05,   ADC_CHANNEL_5 },    // A4    ADC1_INP12
  { BSP_IO_PORT_00_PIN_06,   ADC_CHANNEL_6 },    // A5    ADC2_INP13
  { BSP_IO_PORT_00_PIN_14,   ADC_CHANNEL_14 },   // A6    ADC1_INP18
  { BSP_IO_PORT_00_PIN_15,   ADC_CHANNEL_15 }    // A7    ADC1_INP7
};

const AnalogOutPinDescription g_AAnalogOutPinDescription[] = {
  { BSP_IO_PORT_00_PIN_14,   0 },                // A6    DAC_CH0
  { BSP_IO_PORT_00_PIN_15,   1 }                 // A7    DAC_CH1
};

const pwmTable_t pwmTable[] = {
  {&g_timer0_ctrl, &g_timer0_cfg, GPT_IO_PIN_GTIOCA}, // D0
  {&g_timer2_ctrl, &g_timer2_cfg, GPT_IO_PIN_GTIOCA}, // D1
  {&g_timer3_ctrl, &g_timer3_cfg, GPT_IO_PIN_GTIOCB}, // D2
  {&g_timer4_ctrl, &g_timer4_cfg, GPT_IO_PIN_GTIOCA}, // D3
  {&g_timer4_ctrl, &g_timer4_cfg, GPT_IO_PIN_GTIOCB}, // D4
  {&g_timer5_ctrl, &g_timer5_cfg, GPT_IO_PIN_GTIOCB}, // D5
  {&g_timer6_ctrl, &g_timer6_cfg, GPT_IO_PIN_GTIOCA}, // D6
  {&g_timer6_ctrl, &g_timer6_cfg, GPT_IO_PIN_GTIOCB}, // D7
};

const PinDescription g_APinDescription[] = {
    { BSP_IO_PORT_01_PIN_05 },    //D0
    { BSP_IO_PORT_01_PIN_11 },    //D1
    { BSP_IO_PORT_06_PIN_08 },    //D2
    { BSP_IO_PORT_06_PIN_01 },    //D3
    { BSP_IO_PORT_04_PIN_01 },    //D4
    { BSP_IO_PORT_03_PIN_03 },    //D5
    { BSP_IO_PORT_06_PIN_05 },    //D6
    { BSP_IO_PORT_01_PIN_06 },    //D7

    { BSP_IO_PORT_00_PIN_00 },    //A0
    { BSP_IO_PORT_00_PIN_01 },    //A1
    { BSP_IO_PORT_00_PIN_02 },    //A2
    { BSP_IO_PORT_00_PIN_04 },    //A3
    { BSP_IO_PORT_00_PIN_05 },    //A4
    { BSP_IO_PORT_00_PIN_06 },    //A5
    { BSP_IO_PORT_00_PIN_14 },    //A6
    { BSP_IO_PORT_00_PIN_15 },    //A7

    { BSP_IO_PORT_00_PIN_03 },
    { BSP_IO_PORT_00_PIN_07 },
    { BSP_IO_PORT_00_PIN_08 },
    { BSP_IO_PORT_00_PIN_10 },
    { BSP_IO_PORT_00_PIN_11 },
    { BSP_IO_PORT_00_PIN_12 },
    { BSP_IO_PORT_00_PIN_13 },

    // EXT_Interrupts
    { BSP_IO_PORT_08_PIN_06 },
    { BSP_IO_PORT_01_PIN_04 },
    { BSP_IO_PORT_08_PIN_03 },
    { BSP_IO_PORT_08_PIN_02 },
    { BSP_IO_PORT_10_PIN_10 },
    { BSP_IO_PORT_04_PIN_09 },
    { BSP_IO_PORT_07_PIN_07 },
    { BSP_IO_PORT_07_PIN_08 },
    { BSP_IO_PORT_00_PIN_09 },
    { BSP_IO_PORT_05_PIN_05 },

    { BSP_IO_PORT_01_PIN_00 },
    { BSP_IO_PORT_01_PIN_03 },
    { BSP_IO_PORT_01_PIN_07 },
    { BSP_IO_PORT_01_PIN_08 },
    { BSP_IO_PORT_01_PIN_09 },
    { BSP_IO_PORT_01_PIN_10 },
    { BSP_IO_PORT_01_PIN_12 },
    { BSP_IO_PORT_01_PIN_13 },
    { BSP_IO_PORT_01_PIN_14 },
    { BSP_IO_PORT_01_PIN_15 },

    { BSP_IO_PORT_02_PIN_00 },
    { BSP_IO_PORT_02_PIN_01 },
    { BSP_IO_PORT_02_PIN_02 },
    { BSP_IO_PORT_02_PIN_03 },
    { BSP_IO_PORT_02_PIN_04 },
    { BSP_IO_PORT_02_PIN_05 },
    { BSP_IO_PORT_02_PIN_06 },
    { BSP_IO_PORT_02_PIN_07 },
    { BSP_IO_PORT_02_PIN_08 },
    { BSP_IO_PORT_02_PIN_09 },
    { BSP_IO_PORT_02_PIN_10 },
    { BSP_IO_PORT_02_PIN_11 },
    { BSP_IO_PORT_02_PIN_12 },
    { BSP_IO_PORT_02_PIN_13 },
    { BSP_IO_PORT_02_PIN_14 },
    { BSP_IO_PORT_02_PIN_15 },

    { BSP_IO_PORT_03_PIN_00 },
    { BSP_IO_PORT_03_PIN_01 },
    { BSP_IO_PORT_03_PIN_02 },
    { BSP_IO_PORT_03_PIN_04 },
    { BSP_IO_PORT_03_PIN_05 },
    { BSP_IO_PORT_03_PIN_06 },
    { BSP_IO_PORT_03_PIN_07 },
    { BSP_IO_PORT_03_PIN_08 },
    { BSP_IO_PORT_03_PIN_09 },
    { BSP_IO_PORT_03_PIN_10 },
    { BSP_IO_PORT_03_PIN_11 },
    { BSP_IO_PORT_03_PIN_12 },
    { BSP_IO_PORT_03_PIN_13 },
    { BSP_IO_PORT_03_PIN_14 },
    { BSP_IO_PORT_03_PIN_15 },

    { BSP_IO_PORT_04_PIN_00 },
    { BSP_IO_PORT_04_PIN_01 },
    { BSP_IO_PORT_04_PIN_02 },
    { BSP_IO_PORT_04_PIN_03 },
    { BSP_IO_PORT_04_PIN_04 },
    { BSP_IO_PORT_04_PIN_05 },
    { BSP_IO_PORT_04_PIN_06 },
    { BSP_IO_PORT_04_PIN_07 },
    { BSP_IO_PORT_04_PIN_08 },
    { BSP_IO_PORT_04_PIN_10 },
    { BSP_IO_PORT_04_PIN_11 },
    { BSP_IO_PORT_04_PIN_12 },
    { BSP_IO_PORT_04_PIN_13 },
    { BSP_IO_PORT_04_PIN_14 },
    { BSP_IO_PORT_04_PIN_15 },

    { BSP_IO_PORT_05_PIN_00 },
    { BSP_IO_PORT_05_PIN_01 },
    { BSP_IO_PORT_05_PIN_02 },
    { BSP_IO_PORT_05_PIN_03 },
    { BSP_IO_PORT_05_PIN_04 },
    { BSP_IO_PORT_05_PIN_06 },
    { BSP_IO_PORT_05_PIN_07 },
    { BSP_IO_PORT_05_PIN_08 },
    { BSP_IO_PORT_05_PIN_09 },
    { BSP_IO_PORT_05_PIN_10 },
    { BSP_IO_PORT_05_PIN_11 },
    { BSP_IO_PORT_05_PIN_12 },
    { BSP_IO_PORT_05_PIN_13 },
    { BSP_IO_PORT_05_PIN_14 },
    { BSP_IO_PORT_05_PIN_15 },

    { BSP_IO_PORT_06_PIN_00 },
    { BSP_IO_PORT_06_PIN_02 },
    { BSP_IO_PORT_06_PIN_03 },
    { BSP_IO_PORT_06_PIN_04 },
    { BSP_IO_PORT_06_PIN_06 },
    { BSP_IO_PORT_06_PIN_07 },
    { BSP_IO_PORT_06_PIN_09 },
    { BSP_IO_PORT_06_PIN_10 },
    { BSP_IO_PORT_06_PIN_11 },
    { BSP_IO_PORT_06_PIN_12 },
    { BSP_IO_PORT_06_PIN_13 },
    { BSP_IO_PORT_06_PIN_14 },
    { BSP_IO_PORT_06_PIN_15 },

    { BSP_IO_PORT_07_PIN_00 },
    { BSP_IO_PORT_07_PIN_01 },
    { BSP_IO_PORT_07_PIN_02 },
    { BSP_IO_PORT_07_PIN_03 },
    { BSP_IO_PORT_07_PIN_04 },
    { BSP_IO_PORT_07_PIN_05 },
    { BSP_IO_PORT_07_PIN_06 },
    { BSP_IO_PORT_07_PIN_09 },
    { BSP_IO_PORT_07_PIN_10 },
    { BSP_IO_PORT_07_PIN_11 },
    { BSP_IO_PORT_07_PIN_12 },
    { BSP_IO_PORT_07_PIN_13 },
    { BSP_IO_PORT_07_PIN_14 },
    { BSP_IO_PORT_07_PIN_15 },

    { BSP_IO_PORT_08_PIN_00 },
    { BSP_IO_PORT_08_PIN_01 },
    { BSP_IO_PORT_08_PIN_04 },
    { BSP_IO_PORT_08_PIN_05 },
    { BSP_IO_PORT_08_PIN_07 },
    { BSP_IO_PORT_08_PIN_08 },
    { BSP_IO_PORT_08_PIN_09 },
    { BSP_IO_PORT_08_PIN_10 },
    { BSP_IO_PORT_08_PIN_11 },
    { BSP_IO_PORT_08_PIN_12 },
    { BSP_IO_PORT_08_PIN_13 },
    { BSP_IO_PORT_08_PIN_14 },
    { BSP_IO_PORT_08_PIN_15 },

    { BSP_IO_PORT_09_PIN_00 },
    { BSP_IO_PORT_09_PIN_01 },
    { BSP_IO_PORT_09_PIN_02 },
    { BSP_IO_PORT_09_PIN_03 },
    { BSP_IO_PORT_09_PIN_04 },
    { BSP_IO_PORT_09_PIN_05 },
    { BSP_IO_PORT_09_PIN_06 },
    { BSP_IO_PORT_09_PIN_07 },
    { BSP_IO_PORT_09_PIN_08 },
    { BSP_IO_PORT_09_PIN_09 },
    { BSP_IO_PORT_09_PIN_10 },
    { BSP_IO_PORT_09_PIN_11 },
    { BSP_IO_PORT_09_PIN_12 },
    { BSP_IO_PORT_09_PIN_13 },
    { BSP_IO_PORT_09_PIN_14 },
    { BSP_IO_PORT_09_PIN_15 },

    { BSP_IO_PORT_10_PIN_00 },
    { BSP_IO_PORT_10_PIN_01 },
    { BSP_IO_PORT_10_PIN_02 },
    { BSP_IO_PORT_10_PIN_03 },
    { BSP_IO_PORT_10_PIN_04 },
    { BSP_IO_PORT_10_PIN_05 },
    { BSP_IO_PORT_10_PIN_06 },
    { BSP_IO_PORT_10_PIN_07 },
    { BSP_IO_PORT_10_PIN_08 },
    { BSP_IO_PORT_10_PIN_09 },
    { BSP_IO_PORT_10_PIN_11 },
    { BSP_IO_PORT_10_PIN_12 },
    { BSP_IO_PORT_10_PIN_13 },
    { BSP_IO_PORT_10_PIN_14 },
    { BSP_IO_PORT_10_PIN_15 },

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
};

extern "C" {
  unsigned int PINCOUNT_fn() {
    return (sizeof(g_APinDescription) / sizeof(g_APinDescription[0]));
  }
}

void initVariant() {

}
