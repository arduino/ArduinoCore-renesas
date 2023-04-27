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

std::array<uint16_t, 3> getPinCfgs(const pin_size_t pin, PinCfgReq_t req) {

  std::array<uint16_t, 3> ret = {0 , 0, 0};
  if (pin > g_pin_cfg_size) {
    return ret;
  }

  uint8_t cfg_idx = 0;
  const uint16_t* cfg = g_pin_cfg[pin].list;

  bool thats_all = false;
  uint8_t index = 0;

  while(!thats_all) {

    if(PIN_CFG_REQ_UART_TX == req && IS_PIN_UART_TX(*(cfg + index))) {
      ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_UART_RX == req && IS_PIN_UART_RX(*(cfg + index))) {
      ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_SCL == req && IS_PIN_SCL(*(cfg + index))) {
        ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_SDA == req && IS_PIN_SDA(*(cfg + index))) {
        ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_MISO == req && IS_PIN_MISO(*(cfg + index))) {
        ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_MOSI == req && IS_PIN_MOSI(*(cfg + index))) {
        ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_SCK == req && IS_PIN_SCK(*(cfg + index))) {
        ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_PWM == req && IS_PIN_PWM(*(cfg + index))) {
      ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_INTERRUPT == req && IS_PIN_INTERRUPT(*(cfg + index))) {
      ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_ADC == req && IS_PIN_ANALOG(*(cfg + index))) {
      ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_CAN_RX == req && IS_PIN_CAN_RX(*(cfg + index))) {
      ret[cfg_idx++] = *(cfg + index);
    }
    else if(PIN_CFG_REQ_CAN_TX == req && IS_PIN_CAN_TX(*(cfg + index))) {
      ret[cfg_idx++] = *(cfg + index);
    }

    if(IS_LAST_ITEM(*(cfg + index))) {
      thats_all = true;
    }
    else {
      index++;
    }
  }
  return ret;
}

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
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (14) D14 */
  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (15) D15 */
  { BSP_IO_PORT_00_PIN_14,    P014   }, /* (16) A0  --------------------------  ANALOG  */
  { BSP_IO_PORT_00_PIN_00,    P000   }, /* (17) A1  */
  { BSP_IO_PORT_00_PIN_01,    P001   }, /* (18) A2  */
  { BSP_IO_PORT_00_PIN_02,    P002   }, /* (19) A3  */
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (20) A4  */
  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (21) A5  */

  { BSP_IO_PORT_02_PIN_05,    P205   }, /* (22) D22 ------------------------  RGB LED RED */
  { BSP_IO_PORT_02_PIN_04,    P204   }, /* (23) D23 ------------------------  RGB LED BLUE */
  { BSP_IO_PORT_00_PIN_12,    P012   }, /* (24) D24 ------------------------- TX LED */
  { BSP_IO_PORT_00_PIN_13,    P013   }, /* (25) D25 ------------------------- RX LED */

  { BSP_IO_PORT_05_PIN_01,    P501   }, /* (26) D26  ESP_RX */
  { BSP_IO_PORT_05_PIN_02,    P502   }, /* (27) D27  ESP_TX */
  { BSP_IO_PORT_00_PIN_04,    P004   }, /* (28) D28  ESP_IO9 */
  { BSP_IO_PORT_04_PIN_02,    P402   }, /* (29) D29  ESP_EN */
  { BSP_IO_PORT_04_PIN_00,    P400   }, /* (30) D30  ESP_SPI_CK */
  { BSP_IO_PORT_04_PIN_01,    P401   }, /* (31) D31  ESP_SPI_CS */
  { BSP_IO_PORT_04_PIN_10,    P410   }, /* (32) D32  ESP_SPI_MISO */
  { BSP_IO_PORT_04_PIN_11,    P411   }, /* (33) D33  ESP_SPI_MOSI */

  { BSP_IO_PORT_04_PIN_08,    P408   }, /* (34) D34  EXT_SCL */
  { BSP_IO_PORT_04_PIN_09,    P409   }, /* (35) D35  EXT_SDA */

  { BSP_IO_PORT_05_PIN_00,    P500   }, /* (36) Analog voltage measure pin  */
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

#include "FspTimer.h"

void initVariant() {
  // bootloader configures LED_BUILTIN as PWM output, deconfigure it to avoid spurious signals
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDR, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDR, LOW);
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(3, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(5, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(6, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(9, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(10, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(11, PIN_CFG_REQ_PWM)[0]));
}