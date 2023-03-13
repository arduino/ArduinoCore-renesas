#include "Arduino.h"
#include "pinmux.inc"
const uint16_t P014[] = { LAST_ITEM_GUARD };
const uint16_t P302[] = { LAST_ITEM_GUARD };
const uint16_t P015[] = { LAST_ITEM_GUARD };
const uint16_t P401[] = { LAST_ITEM_GUARD };
const uint16_t P215[] = { LAST_ITEM_GUARD };
const uint16_t P214[] = { LAST_ITEM_GUARD };
const uint16_t P108[] = { LAST_ITEM_GUARD };
const uint16_t P300[] = { LAST_ITEM_GUARD };
const uint16_t P104[] = { LAST_ITEM_GUARD };
const uint16_t P409[] = { LAST_ITEM_GUARD };
const uint16_t P105[] = { LAST_ITEM_GUARD };
const uint16_t P206[] = { LAST_ITEM_GUARD };

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
	  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (2) D2  */
	  { BSP_IO_PORT_00_PIN_00,    P000   }, /* (3) D3~ */
	  { BSP_IO_PORT_00_PIN_14,    P014   }, /* (4) D4  */
	  { BSP_IO_PORT_00_PIN_15,    P015   }, /* (5) D5~ */
	  { BSP_IO_PORT_04_PIN_00,    P400   }, /* (6) D6~ */
	  { BSP_IO_PORT_05_PIN_01,    P501   }, /* (8) D8  */
	  { BSP_IO_PORT_04_PIN_01,    P401   }, /* (7) D7  */
	  { BSP_IO_PORT_05_PIN_02,    P502   }, /* (9) D9~  */
	  { BSP_IO_PORT_02_PIN_14,    P214   }, /* (11) D11~ */
	  { BSP_IO_PORT_02_PIN_15,    P215   }, /* (10) D10~ */
	  { BSP_IO_PORT_02_PIN_13,    P213   }, /* (12) D12 */
	  { BSP_IO_PORT_02_PIN_12,    P212   }, /* (13) D13 */
	  { BSP_IO_PORT_01_PIN_08,    P108   }, /* (14) D14 */
	  { BSP_IO_PORT_03_PIN_00,    P300   }, /* (15) D15 */
	  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (16) A0  --------------------------  ANALOG  */
	  { BSP_IO_PORT_01_PIN_02,    P102   }, /* (17) A1  */
	  { BSP_IO_PORT_01_PIN_03,    P103   }, /* (18) A2  */
	  { BSP_IO_PORT_01_PIN_04,    P104   }, /* (19) A3  */
	  { BSP_IO_PORT_04_PIN_08,    P408   }, /* (20) A4  */
	  { BSP_IO_PORT_04_PIN_09,    P409   }, /* (21) A5  */
	  { BSP_IO_PORT_01_PIN_05,    P105   }, /* (22) A6  */
		  		  
	  { BSP_IO_PORT_01_PIN_09,    P109   }, /* (23) D16 ------------------------- TX   */
	  { BSP_IO_PORT_01_PIN_10,    P110   }, /* (24) D17 ------------------------- RX   */
	  { BSP_IO_PORT_00_PIN_13,    P013   }, /* (25) D18 ------------------------- DAC1 */
      { BSP_IO_PORT_05_PIN_00,    P500   }, /* (26) D19 ------------------------- DAC2 */
	  { BSP_IO_PORT_02_PIN_06,    P206   }, /* (27) D20  */
		  
	
	  
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
