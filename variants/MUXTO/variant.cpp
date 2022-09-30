#include "Arduino.h"
#include "pinmux.inc"

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
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (0) D0  -------------------------  DIGITAL  */
  { BSP_IO_PORT_01_PIN_02,    P102   }, /* (1) D1  */
  { BSP_IO_PORT_01_PIN_03,    P103   }, /* (1) D1  */
  { BSP_IO_PORT_01_PIN_09,    P109   }, /* (1) D1  */
};

extern "C" const size_t g_pin_cfg_size = sizeof(g_pin_cfg);

void initVariant() {

}
