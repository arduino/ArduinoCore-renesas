#include "Arduino.h"
#include "pinmux.inc"

uint16_t getPinCfg(const uint16_t *cfg, PinCfgReq_t req, bool prefer_sci /*= false*/) {
  if(cfg == nullptr) {
    return 0;
  }
  bool thats_all = false;
  uint8_t index = 0;

  uint16_t config = 0;
  uint16_t config_sci = 0;

  while(!thats_all) {

    if(PIN_CFG_REQ_UART_TX == req && IS_PIN_UART_TX(*(cfg + index))) {
      config = *(cfg + index);
    }
    else if(PIN_CFG_REQ_UART_RX == req && IS_PIN_UART_RX(*(cfg + index))) {
      config = *(cfg + index);
    }
    else if(PIN_CFG_REQ_SCL == req && IS_PIN_SCL(*(cfg + index))) {
      if (IS_SCI(*(cfg + index))) {
        config_sci = *(cfg + index);
      } else {
        config = *(cfg + index);
      }
    }
    else if(PIN_CFG_REQ_SDA == req && IS_PIN_SDA(*(cfg + index))) {
      if (IS_SCI(*(cfg + index))) {
        config_sci = *(cfg + index);
      } else {
        config = *(cfg + index);
      }
    }
    else if(PIN_CFG_REQ_MISO == req && IS_PIN_MISO(*(cfg + index))) {
      if (IS_SCI(*(cfg + index))) {
        config_sci = *(cfg + index);
      } else {
        config = *(cfg + index);
      }
    }
    else if(PIN_CFG_REQ_MOSI == req && IS_PIN_MOSI(*(cfg + index))) {
      if (IS_SCI(*(cfg + index))) {
        config_sci = *(cfg + index);
      } else {
        config = *(cfg + index);
      }
    }
    else if(PIN_CFG_REQ_SCK == req && IS_PIN_SCK(*(cfg + index))) {
      if (IS_SCI(*(cfg + index))) {
        config_sci = *(cfg + index);
      } else {
        config = *(cfg + index);
      }
    }
    else if(PIN_CFG_REQ_PWM == req && IS_PIN_PWM(*(cfg + index))) {
      config = *(cfg + index);
    }
    else if(PIN_CFG_REQ_INTERRUPT == req && IS_PIN_INTERRUPT(*(cfg + index))) {
      config = *(cfg + index);
    }
    else if(PIN_CFG_REQ_ADC == req && IS_PIN_ANALOG(*(cfg + index))) {
      config = *(cfg + index);
    }

    if(IS_LAST_ITEM(*(cfg + index))) {
      thats_all = true;
    }
    else {
      index++;
    }
  }
  if (config_sci != 0 && (prefer_sci || config == 0)) {
    config = config_sci;
  }
  return config;
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
