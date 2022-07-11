#include "Arduino.h"

const AnalogPinDescription g_AAnalogPinDescription[] = {
  { BSP_IO_PORT_00_PIN_00,   ADC_CHANNEL_0 },    // A0    ADC0_AN0
  { BSP_IO_PORT_00_PIN_00,   ADC_CHANNEL_1 },    // A1    ADC0_AN1
  { BSP_IO_PORT_00_PIN_00,   ADC_CHANNEL_2 },    // A2    ADC0_AN2
  { BSP_IO_PORT_00_PIN_00,   ADC_CHANNEL_14},    // A3    ADC3_AN9
  { BSP_IO_PORT_00_PIN_01,   ADC_CHANNEL_1 },    // A4    ADC1_AN21
  { BSP_IO_PORT_00_PIN_01,   ADC_CHANNEL_0 }     // A5    ADC2_AN22
};

const AnalogOutPinDescription g_AAnalogOutPinDescription[] = {
  { BSP_IO_PORT_00_PIN_14,   0 }                 // A3
};

const pwmTable_t pwmTable[] = {
  {&g_timer1_ctrl, &g_timer1_cfg, GPT_IO_PIN_GTIOCB}, // D3
  {&g_timer2_ctrl, &g_timer2_cfg, GPT_IO_PIN_GTIOCA}, // D5
  {&g_timer3_ctrl, &g_timer3_cfg, GPT_IO_PIN_GTIOCB}, // D6
  {&g_timer0_ctrl, &g_timer0_cfg, GPT_IO_PIN_GTIOCA}, // D9
  {&g_timer0_ctrl, &g_timer0_cfg, GPT_IO_PIN_GTIOCB}, // D10
  {&g_timer1_ctrl, &g_timer1_cfg, GPT_IO_PIN_GTIOCA}, // D11
};

const PinDescription g_APinDescription[] = {
    // Digital
    { BSP_IO_PORT_01_PIN_02 },    //D0      //UART TX
    { BSP_IO_PORT_03_PIN_01 },    //D1      //UART RX
    { BSP_IO_PORT_02_PIN_06 },    //D2      //IRQ0
    { BSP_IO_PORT_01_PIN_04 },    //D3      //IRQ1  --  PWM0
    { BSP_IO_PORT_04_PIN_00 },    //D4
    { BSP_IO_PORT_01_PIN_03 },    //D5      //PWM1
    { BSP_IO_PORT_01_PIN_12 },    //D6      //PWM2
    { BSP_IO_PORT_04_PIN_09 },    //D7
    { BSP_IO_PORT_03_PIN_02 },    //D8
    { BSP_IO_PORT_03_PIN_00 },    //D9      //PWM3
    { BSP_IO_PORT_01_PIN_08 },    //D10     //CS    --  PWM4
    { BSP_IO_PORT_01_PIN_09 },    //D11     //MOSI  --  PWM4
    { BSP_IO_PORT_01_PIN_00 },    //D12     //MISO
    { BSP_IO_PORT_01_PIN_11 },    //D13     //SCK   --  LEDG
    { BSP_IO_PORT_01_PIN_01 },    //D14     //SDA
    { BSP_IO_PORT_01_PIN_00 },    //D15     //SCL

    // Analog
    { BSP_IO_PORT_00_PIN_00 },    //A0
    { BSP_IO_PORT_00_PIN_01 },    //A1
    { BSP_IO_PORT_00_PIN_02 },    //A2
    { BSP_IO_PORT_00_PIN_14 },    //A3
    { BSP_IO_PORT_01_PIN_01 },    //A4
    { BSP_IO_PORT_01_PIN_00 },    //A5

    // RGB LED
    { BSP_IO_PORT_05_PIN_00 },    //LEDR
    { BSP_IO_PORT_00_PIN_11 },    //LEDB

    { BSP_IO_PORT_06_PIN_11 },
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
