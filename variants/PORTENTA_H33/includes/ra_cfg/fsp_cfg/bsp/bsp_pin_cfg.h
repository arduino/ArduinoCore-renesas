/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#include "r_ioport.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define OSPI_SIO6 (BSP_IO_PORT_01_PIN_03)
#define OSPI_SIO0 (BSP_IO_PORT_01_PIN_06)
#define OSPI_SIO3 (BSP_IO_PORT_01_PIN_07)
#define ETH_MDC (BSP_IO_PORT_04_PIN_01)
#define ETH_MDIO (BSP_IO_PORT_04_PIN_02)
#define ETH_TXEN (BSP_IO_PORT_04_PIN_05)
#define ETH_TXD1 (BSP_IO_PORT_04_PIN_06)
#define CAN_TXD (BSP_IO_PORT_06_PIN_09)
#define CAN_RDX (BSP_IO_PORT_06_PIN_10)
#define ETH_TXD0 (BSP_IO_PORT_07_PIN_00)
#define ETH_50REF (BSP_IO_PORT_07_PIN_01)
#define ETH_RXD0 (BSP_IO_PORT_07_PIN_02)
#define ETH_RXD1 (BSP_IO_PORT_07_PIN_03)
#define ETH_RXERR (BSP_IO_PORT_07_PIN_04)
#define ETH_CRSDV (BSP_IO_PORT_07_PIN_05)
extern const ioport_cfg_t g_bsp_pin_cfg; /* PortentaH33 */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif /* BSP_PIN_CFG_H_ */
