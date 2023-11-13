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

extern "C" const PinMuxCfg_t g_pin_cfg[] = { 
  { BSP_IO_PORT_03_PIN_01,    P301   }, /* (0) D0  -------------------------  DIGITAL  */
  { BSP_IO_PORT_03_PIN_02,    P302   }, /* (1) D1  */
  { BSP_IO_PORT_01_PIN_05,    P105   }, /* (2) D2  */
  { BSP_IO_PORT_01_PIN_04,    P104   }, /* (3) D3~ */
  { BSP_IO_PORT_01_PIN_03,    P103   }, /* (4) D4  */
  { BSP_IO_PORT_01_PIN_02,    P102   }, /* (5) D5~ */
  { BSP_IO_PORT_01_PIN_06,    P106   }, /* (6) D6~ */
  { BSP_IO_PORT_01_PIN_07,    P107   }, /* (7) D7  */
  { BSP_IO_PORT_03_PIN_04,    P304   }, /* (8) D8  */
  { BSP_IO_PORT_03_PIN_03,    P303   }, /* (9) D9~  */
  { BSP_IO_PORT_01_PIN_12,    P112   }, /* (10) D10~ */
  { BSP_IO_PORT_01_PIN_09,    P109   }, /* (11) D11~ */
  { BSP_IO_PORT_01_PIN_10,    P110   }, /* (12) D12 */
  { BSP_IO_PORT_01_PIN_11,    P111   }, /* (13) D13 */
  { BSP_IO_PORT_00_PIN_14,    P014   }, /* (14) A0  --------------------------  ANALOG  */
  { BSP_IO_PORT_00_PIN_00,    P000   }, /* (15) A1  */
  { BSP_IO_PORT_00_PIN_01,    P001   }, /* (16) A2  */
  { BSP_IO_PORT_00_PIN_02,    P002   }, /* (17) A3  */
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (18) A4/SDA  */
  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (19) A5/SCL  */

  { BSP_IO_PORT_05_PIN_00,    P500   }, /* (20) Analog voltage measure pin  */
  { BSP_IO_PORT_00_PIN_12,    P012   }, /* (21) TX LED  */
  { BSP_IO_PORT_00_PIN_13,    P013   }, /* (22) RX LED  */

  { BSP_IO_PORT_05_PIN_01,    P501   }, /* (23) TX on SWD connector  */
  { BSP_IO_PORT_05_PIN_02,    P502   }, /* (24) RX on SWD connector  */
  { BSP_IO_PORT_01_PIN_08,    P108   }, /* (25) SWDIO  */
  { BSP_IO_PORT_03_PIN_00,    P300   }, /* (26) SWCLK  */
};

extern "C" {
    unsigned int PINCOUNT_fn() {
        return (sizeof(g_pin_cfg) / sizeof(g_pin_cfg[0]));
    }
}

int32_t getPinIndex(bsp_io_port_pin_t p) {
  int max_index = PINS_COUNT;
  int rv = -1;
  for(int i = 0; i < max_index; i++) {
    if(g_pin_cfg[i].pin == p) {
      rv = i;
      break;
    }
  }
  return rv;
}

#include "FspTimer.h"

void usb_post_initialization() {
  ((R_USB_FS0_Type*)R_USB_FS0_BASE)->USBMC_b.VDCEN = 1;
}

void enableSubclockInputPins() {
  R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_CGC);
  R_SYSTEM->SOSCCR_b.SOSTP = 1;
  R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_CGC);
}

void initVariant() {
  // bootloader configures LED_BUILTIN as PWM output, deconfigure it to avoid spurious signals
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(3, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(5, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(6, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(9, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(10, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(11, PIN_CFG_REQ_PWM)[0]));
}