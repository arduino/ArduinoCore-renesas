#include "Arduino.h"
#include "pinmux.inc"

const uint16_t P400_b[] = {
PIN_PWM|CHANNEL_6|PWM_CHANNEL_A|GPT_ODD_CFG,
PIN_SCL|CHANNEL_0,
PIN_INTERRUPT|CHANNEL_0,
SCI_CHANNEL|PIN_SCK|CHANNEL_0|SCI_EVEN_CFG|LAST_ITEM_GUARD
};
#define P400 P400_b

const uint16_t P408_b[] = {
PIN_PWM|CHANNEL_5|PWM_CHANNEL_B|GPT_ODD_CFG,
PIN_INTERRUPT|CHANNEL_7,
SCI_CHANNEL|PIN_CTS_RTS_SS|CHANNEL_1|SCI_EVEN_CFG,
SCI_CHANNEL|PIN_RX_MISO_SCL|CHANNEL_9|SCI_ODD_CFG|LAST_ITEM_GUARD
};
#define P408 P408_b

const uint16_t P014_b[] = {
PIN_DAC|CHANNEL_0,
PIN_ANALOG|CHANNEL_9|LAST_ITEM_GUARD
};
#define P014 P014_b

const uint16_t P402_b[] = {
PIN_CAN_RX|CHANNEL_0,
PIN_INTERRUPT|CHANNEL_4,
SCI_CHANNEL|PIN_RX_MISO_SCL|CHANNEL_1|SCI_ODD_CFG|LAST_ITEM_GUARD
};
#define P402 P402_b

extern "C" const PinMuxCfg_t g_pin_cfg[] = { 
  /* DIGITAL OUTPUT */
  { BSP_IO_PORT_03_PIN_03,    P303   }, /* (0) D0 HW rev 0.3 GPIO4            */
  { BSP_IO_PORT_02_PIN_04,    P204   }, /* (1) D1 HW rev 0.3 GPIO5            */
  { BSP_IO_PORT_02_PIN_05,    P205   }, /* (2) D2 HW rev 0.3 GPIO6            */
  { BSP_IO_PORT_02_PIN_06,    P206   }, /* (3) D3 HW rev 0.3 GPIO7            */
  { BSP_IO_PORT_01_PIN_05,    P105   }, /* (4) D4 HW rev 0.3 GPIO8            */
  { BSP_IO_PORT_01_PIN_12,    P112   }, /* (5) D5 HW rev 0.3 GPIO9            */
  { BSP_IO_PORT_01_PIN_11,    P111   }, /* (6) D6 HW rev 0.3 GPIO10           */
  { BSP_IO_PORT_00_PIN_11,    P011   }, /* (7) D7 HW rev 0.3 GPIO11           */
  /* LEDs */
  { BSP_IO_PORT_01_PIN_13,    P113   }, /* (8) D8 HW rev 0.3 LED RED          */
  { BSP_IO_PORT_04_PIN_10,    P410   }, /* (9) D9 HW rev 0.3 LED BLUE         */
  { BSP_IO_PORT_04_PIN_11,    P411   }, /* (10) D10 HW rev 0.3 LED GREEN      */
   /* CAN */
  { BSP_IO_PORT_01_PIN_10,    P110   }, /* (11) D11 HW rev 0.3 CAN ENABLE     */
  { BSP_IO_PORT_01_PIN_09,    P109   }, /* (12) D12 HW rev 0.3 CAN TX         */
  { BSP_IO_PORT_04_PIN_02,    P402   }, /* (13) D13 HW rev 0.3 CAN RX         */
  /* ANALOG INPUT */
  { BSP_IO_PORT_00_PIN_00,    P000   }, /* (14) A0 HW rev 0.3                 */
  { BSP_IO_PORT_00_PIN_01,    P001   }, /* (15) A1 HW rev 0.3                 */ 
  { BSP_IO_PORT_00_PIN_02,    P002   }, /* (16) A2 HW rev 0.3                 */
  { BSP_IO_PORT_00_PIN_03,    P003   }, /* (17) A3 HW rev 0.3                 */
  { BSP_IO_PORT_00_PIN_04,    P004   }, /* (18) A4 HW rev 0.3                 */
  { BSP_IO_PORT_00_PIN_12,    P012   }, /* (19) A5 HW rev 0.3                 */
  { BSP_IO_PORT_00_PIN_13,    P013   }, /* (20) A6 HW rev 0.3                 */
  { BSP_IO_PORT_00_PIN_14,    P014   }, /* (21) A7 HW rev 0.3                 */
  { BSP_IO_PORT_00_PIN_15,    P015   }, /* (22) A8 HW rev 0.3                 */
  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (23) A9 HW rev 0.3                 */
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (24) A13 HW rev 0.3                */
  { BSP_IO_PORT_01_PIN_02,    P102   }, /* (25) A14 HW rev 0.3                */
  { BSP_IO_PORT_01_PIN_03,    P103   }, /* (26) A15 HW rev 0.3                */
  { BSP_IO_PORT_05_PIN_02,    P502   }, /* (27) A10 HW rev 0.3                */
  { BSP_IO_PORT_05_PIN_01,    P501   }, /* (28) A11 HW rev 0.3                */
  { BSP_IO_PORT_05_PIN_00,    P500   }, /* (29) A12 HW rev 0.3                */
  /* DIGITAL CONTROLS */
  { BSP_IO_PORT_03_PIN_04,    P304   }, /* (30) DETECT_IN HW rev 0.3          */
  { BSP_IO_PORT_01_PIN_04,    P104   }, /* (31) DETECT_OUT HW rev 0.3         */
  /* UART */
  { BSP_IO_PORT_03_PIN_02,    P302   }, /* (32) UART_TX HW rev 0.3            */
  { BSP_IO_PORT_03_PIN_01,    P301   }, /* (33) UART_RX HW rev 0.3            */
  /* I2C */
  { BSP_IO_PORT_04_PIN_01,    P401   }, /* (34) SDA HW rev 0.3                */
  { BSP_IO_PORT_04_PIN_00,    P400   }, /* (35) SCL HW rev 0.3                */
  /* RS485 */
  { BSP_IO_PORT_01_PIN_06,    P106   }, /* (36) HW rev 0.3 RS485 DE           */
  { BSP_IO_PORT_01_PIN_07,    P107   }, /* (37) HW rev 0.3 RS485 ~RE          */
  { BSP_IO_PORT_04_PIN_08,    P408   }, /* (38) HW rev 0.3 RS485 RX           */
  { BSP_IO_PORT_04_PIN_09,    P409   }, /* (39) HW rev 0.3 GPIO7 TX           */
};

extern "C" const size_t g_pin_cfg_size = sizeof(g_pin_cfg);

int32_t getPinIndex(bsp_io_port_pin_t p) {
  int max_index = g_pin_cfg_size / sizeof(g_pin_cfg[0]);
  int rv = -1;
  for(int i = 0; i < max_index; i++) {
    if(g_pin_cfg[i].pin == p) {
      rv = i;
      break;
    }
  }
  return rv;
}

extern "C" {
    unsigned int PINCOUNT_fn() {
        return (sizeof(g_pin_cfg) / sizeof(g_pin_cfg[0]));
    }
}

#include "FspTimer.h"

void initVariant() {
  // bootloader configures LED_BUILTIN as PWM output, deconfigure it to avoid spurious signals
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, LOW);
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(3, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(5, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(6, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(9, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(10, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(11, PIN_CFG_REQ_PWM)[0]));
}
