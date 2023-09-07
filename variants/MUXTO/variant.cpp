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
const uint16_t P104[] = { PIN_ANALOG | CHANNEL_20 | LAST_ITEM_GUARD };
const uint16_t P409[] = { PIN_SCL    | CHANNEL_0  | LAST_ITEM_GUARD };
const uint16_t P105[] = { PIN_ANALOG | CHANNEL_21 | LAST_ITEM_GUARD };
const uint16_t P206[] = { LAST_ITEM_GUARD };

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

  { BSP_IO_PORT_03_PIN_01,    P301   }, /* (0) D0  -------------------------  DIGITAL  */
  { BSP_IO_PORT_03_PIN_02,    P302   }, /* (1) D1  */
  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (2) D2  */
  { BSP_IO_PORT_00_PIN_00,    P000   }, /* (3) D3~ */
  { BSP_IO_PORT_00_PIN_14,    P014   }, /* (4) D4  */
  { BSP_IO_PORT_00_PIN_15,    P015   }, /* (5) D5~ */
  { BSP_IO_PORT_04_PIN_00,    P400   }, /* (6) D6~ */
  { BSP_IO_PORT_05_PIN_01,    P501   }, /* (7) D8  */
  { BSP_IO_PORT_04_PIN_01,    P401   }, /* (8) D7  */
  { BSP_IO_PORT_05_PIN_02,    P502   }, /* (9) D9~  */
  { BSP_IO_PORT_02_PIN_14,    P214   }, /* (10) D11~ */
  { BSP_IO_PORT_02_PIN_15,    P215   }, /* (11) D10~ */
  { BSP_IO_PORT_02_PIN_13,    P213   }, /* (12) D12 */
  { BSP_IO_PORT_02_PIN_12,    P212   }, /* (13) D13 */
  { BSP_IO_PORT_01_PIN_08,    P108   }, /* (14) D14 */
  { BSP_IO_PORT_03_PIN_00,    P300   }, /* (15) D15 */
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (16) A0  D16 --------------------------  ANALOG  */
  { BSP_IO_PORT_01_PIN_02,    P102   }, /* (17) A1  D17  */
  { BSP_IO_PORT_01_PIN_03,    P103   }, /* (18) A2  D18  */
  { BSP_IO_PORT_01_PIN_04,    P104   }, /* (19) A3  D19  */
  { BSP_IO_PORT_04_PIN_08,    P408   }, /* (20) A4  D20  SDA - PAY ATTENTION: this pin has NOT analog input capability */
  { BSP_IO_PORT_04_PIN_09,    P409   }, /* (21) A5  D21  SCL - PAY ATTENTION: this pin has NOT analog input capability */
  { BSP_IO_PORT_01_PIN_05,    P105   }, /* (22) A6  D22  */

  { BSP_IO_PORT_01_PIN_09,    P109   }, /* (23) D23 ------------------------- TX   */
  { BSP_IO_PORT_01_PIN_10,    P110   }, /* (24) D24 ------------------------- RX   */
  { BSP_IO_PORT_00_PIN_13,    P013   }, /* (25) D25 ------------------------- DAC8 */
  { BSP_IO_PORT_05_PIN_00,    P500   }, /* (26) D26 ------------------------- DAC12 */
  { BSP_IO_PORT_02_PIN_06,    P206   }, /* (27) D27  */
};

extern "C" {
    unsigned int PINCOUNT_fn() {
        return (sizeof(g_pin_cfg) / sizeof(g_pin_cfg[0]));
    }
}

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

void usb_post_initialization() {
  ((R_USB_FS0_Type*)R_USB_FS0_BASE)->USBMC_b.VDCEN = 1;
  ((R_USB_FS0_Type*)R_USB_FS0_BASE)->SYSCFG_b.DPRPU = 1;
}

void initVariant() {
    //R_IOPORT_Open(&ioport_ctrl, &bsp_pin_cfg);
    //R_IOPORT_PinCfg(NULL, BSP_IO_PORT_09_PIN_14, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_FS));
    //R_IOPORT_PinCfg(NULL, BSP_IO_PORT_09_PIN_15, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_FS));
    //R_IOPORT_PinCfg(NULL, BSP_IO_PORT_04_PIN_07, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_USB_FS));
}
