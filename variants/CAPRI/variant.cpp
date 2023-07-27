#include "Arduino.h"
#include "pinmux.inc"
/*
const uint16_t P014[] = { LAST_ITEM_GUARD };
const uint16_t P302[] = { LAST_ITEM_GUARD };
const uint16_t P015[] = { LAST_ITEM_GUARD };
const uint16_t P401[] = { LAST_ITEM_GUARD };
const uint16_t P215[] = { LAST_ITEM_GUARD };
const uint16_t P214[] = { LAST_ITEM_GUARD };
const uint16_t P108[] = { LAST_ITEM_GUARD };
const uint16_t P300[] = { LAST_ITEM_GUARD };
const uint16_t P104[] = { PIN_ANALOG | CHANNEL_20 | LAST_ITEM_GUARD };
const uint16_t P409[] = { PIN_SCL    | CHANNEL_0  | LAST_ITEM_GUARD };
const uint16_t P105[] = { PIN_ANALOG | CHANNEL_21 | LAST_ITEM_GUARD };
const uint16_t P206[] = { LAST_ITEM_GUARD };
*/
const uint16_t P013_b[] = {
PIN_DAC|DAC_8BIT|CHANNEL_0,
PIN_ANALOG|CHANNEL_5,
PIN_PWM|CHANNEL_6|PWM_CHANNEL_B|GPT_ODD_CFG|LAST_ITEM_GUARD
};
#define P013 P013_b

const uint16_t P500_b[] = {
PIN_DAC|CHANNEL_0,
PIN_ANALOG|CHANNEL_0,
PIN_PWM|CHANNEL_5|PWM_CHANNEL_A|GPT_ODD_CFG,
PIN_INTERRUPT|CHANNEL_3,
SCI_CHANNEL|PIN_RX_MISO_SCL|CHANNEL_0|SCI_EVEN_CFG|LAST_ITEM_GUARD
};
#define P500 P500_b

extern "C" const PinMuxCfg_t g_pin_cfg[] = { 

  { BSP_IO_PORT_00_PIN_13,    P013   }, /* (0)  A0  D0~  DAC8-------------------------  ANALOG & DIGITAL     */
  { BSP_IO_PORT_05_PIN_00,    P500   }, /* (1)  A1  D1~  DAC12   RX1                                         */
  { BSP_IO_PORT_05_PIN_01,    P501   }, /* (2)  A2  D2~          TX1                                         */
  { BSP_IO_PORT_05_PIN_02,    P502   }, /* (3)  A3  D3                                                       */
  { BSP_IO_PORT_04_PIN_08,    P408   }, /* (4)      D4~                SDA - PAY ATTENTION: this pin has NOT analog input capability */
  { BSP_IO_PORT_04_PIN_09,    P409   }, /* (5)      D5~                SCL - PAY ATTENTION: this pin has NOT analog input capability */
  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (6)  A6  D6~          TX    SDA9   only master                                    */
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (7)  A7  D7~          RX    SCL9   only master                    */
  { BSP_IO_PORT_01_PIN_03,    P103   }, /* (8)  A8  D8~                        SCK                           */
  { BSP_IO_PORT_01_PIN_04,    P104   }, /* (9)  A9  D9                         MISO                          */
  { BSP_IO_PORT_01_PIN_05,    P105   }, /* (10) A10 D10                        MOSI                          */

  { BSP_IO_PORT_04_PIN_00,    P400   }, /* (11)     D11                        LED BUILTIN                   */
  { BSP_IO_PORT_04_PIN_00,    P301   }, /* (12)     D12                        HEART PAD                     */


 
 // { BSP_IO_PORT_00_PIN_15,    P015   }, /* (13)  A4       not connected use 0ohm                       */
 // { BSP_IO_PORT_00_PIN_14,    P014   }, /* (14)  A5       not connected use 0ohm                      */

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
