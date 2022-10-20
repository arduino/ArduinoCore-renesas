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

    if(!prefer_sci && IS_SCI(*(cfg + index))) {
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
    else if(PIN_CFG_REQ_SCK == req && IS_PIN_SCK(*(cfg + index))) {
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
    else if(PIN_CFG_REQ_CAN_RX == req && IS_PIN_CAN_RX(*(cfg + index))) {
      return *(cfg + index);
    }
    else if(PIN_CFG_REQ_CAN_TX == req && IS_PIN_CAN_TX(*(cfg + index))) {
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
  { BSP_IO_PORT_04_PIN_00,    P400   }, /* (4) D4  */
  { BSP_IO_PORT_01_PIN_03,    P103   }, /* (5) D5  */
  { BSP_IO_PORT_01_PIN_12,    P112   }, /* (6) D6  */
  { BSP_IO_PORT_04_PIN_09,    P409   }, /* (7) D7  */
  { BSP_IO_PORT_03_PIN_02,    P302   }, /* (8) D8  */
  { BSP_IO_PORT_03_PIN_00,    P300   }, /* (9) D9  */
  { BSP_IO_PORT_01_PIN_08,    P108   }, /* (10) D10 */
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
  { BSP_IO_PORT_00_PIN_11,    P011   }, /* (22) D22 --------------------------- RGB LED */
  { BSP_IO_PORT_00_PIN_13,    P013   }, /* (23) D23 */
  { BSP_IO_PORT_00_PIN_12,    P012   }, /* (24) D24 ------------------------- TX/RX LED */
};

extern "C" const size_t g_pin_cfg_size = sizeof(g_pin_cfg);

void initVariant() {

}