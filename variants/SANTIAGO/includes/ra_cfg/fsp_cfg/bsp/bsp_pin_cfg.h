/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#include "r_ioport.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define A0 (BSP_IO_PORT_00_PIN_00)
#define A1 (BSP_IO_PORT_00_PIN_01)
#define A2 (BSP_IO_PORT_00_PIN_02)
#define LED_RED (BSP_IO_PORT_00_PIN_11)
#define TX_LED (BSP_IO_PORT_00_PIN_12)
#define LED_BLUE (BSP_IO_PORT_00_PIN_13)
#define A3 (BSP_IO_PORT_00_PIN_14)
#define FUSB_INT (BSP_IO_PORT_00_PIN_15)
#define A5_SCL (BSP_IO_PORT_01_PIN_00)
#define A4_SDA (BSP_IO_PORT_01_PIN_01)
#define D1_TX (BSP_IO_PORT_01_PIN_02)
#define D5 (BSP_IO_PORT_01_PIN_03)
#define D3 (BSP_IO_PORT_01_PIN_04)
#define D10 (BSP_IO_PORT_01_PIN_08)
#define D11_MOSI (BSP_IO_PORT_01_PIN_09)
#define D12_MISO (BSP_IO_PORT_01_PIN_10)
#define D13_SCK (BSP_IO_PORT_01_PIN_11)
#define D6 (BSP_IO_PORT_01_PIN_12)
#define RX_LED (BSP_IO_PORT_02_PIN_01)
#define D2 (BSP_IO_PORT_02_PIN_06)
#define D9 (BSP_IO_PORT_03_PIN_00)
#define D0_RX (BSP_IO_PORT_03_PIN_01)
#define D8 (BSP_IO_PORT_03_PIN_02)
#define D4 (BSP_IO_PORT_04_PIN_00)
#define USB_VBUS (BSP_IO_PORT_04_PIN_07)
#define USB_ID (BSP_IO_PORT_04_PIN_08)
#define D7 (BSP_IO_PORT_04_PIN_09)
#define VBUS_HOST_EN (BSP_IO_PORT_05_PIN_00)
extern const ioport_cfg_t g_bsp_pin_cfg; /* R7FA4M1AB3CNE.pincfg */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif /* BSP_PIN_CFG_H_ */
