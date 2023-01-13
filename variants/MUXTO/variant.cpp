#include "Arduino.h"
#include "pinmux.inc"

std::array<uint16_t, 3> getPinCfgs(const pinsize_t pin, PinCfgReq_t req) {

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
  { BSP_IO_PORT_01_PIN_09,    P109   }, /* (0) D0  -------------------------  DIGITAL  */
  { BSP_IO_PORT_00_PIN_00,    P000   }, /* (1) D1  */
  { BSP_IO_PORT_04_PIN_08,    P408   }, /* (1) D1  */
  { BSP_IO_PORT_01_PIN_10,    P110   }, /* (1) D1  */
};

extern "C" const size_t g_pin_cfg_size = sizeof(g_pin_cfg);

const ioport_pin_cfg_t bsp_pin_cfg_data[] = {
  { ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_USB_FS), BSP_IO_PORT_09_PIN_15 },
  { ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_USB_FS), BSP_IO_PORT_09_PIN_14 },
  { ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_USB_FS), BSP_IO_PORT_04_PIN_07 },
};

static const ioport_cfg_t bsp_pin_cfg = {
  .number_of_pins = sizeof(bsp_pin_cfg_data) / sizeof(ioport_pin_cfg_t),
  .p_pin_cfg_data = &bsp_pin_cfg_data[0],
};
static ioport_instance_ctrl_t ioport_ctrl;

void initVariant() {
    //R_IOPORT_Open(&ioport_ctrl, &bsp_pin_cfg);
    //R_IOPORT_PinCfg(NULL, BSP_IO_PORT_09_PIN_14, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_FS));
    //R_IOPORT_PinCfg(NULL, BSP_IO_PORT_09_PIN_15, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_FS));
    //R_IOPORT_PinCfg(NULL, BSP_IO_PORT_04_PIN_07, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_FS));
}
