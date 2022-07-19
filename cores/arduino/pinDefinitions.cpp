#include "Arduino.h"
#include "bsp_api.h"
#include "hal_data.h"

#define IOPORT_PRV_PERIPHERAL_FUNCTION    (1U << 16)
#define IOPORT_PRV_PORT_OFFSET            (8U)
#define BSP_IO_PRV_8BIT_MASK              (0xFF)

void pinPeripheral(bsp_io_port_pin_t bspPin, uint32_t bspPeripheral) {

    bsp_io_port_pin_t pin;
    ioport_cfg_t * p_pin_data = (ioport_cfg_t *) g_ioport.p_cfg;

    R_BSP_PinAccessEnable();           // Protect PWPR from re-entrancy

    g_APinDescription[bspPin].PeripheralConfig = bspPeripheral;
    pin = g_APinDescription[bspPin].name;

    /* PMR bits should be cleared before specifying PSEL. Reference section "20.7 Notes on the PmnPFS Register Setting"
     * in the RA6M3 manual R01UH0886EJ0100. */
    if ((bspPeripheral & IOPORT_PRV_PERIPHERAL_FUNCTION) > 0)
    {
        /* Clear PMR */
        R_PFS->PORT[pin >> IOPORT_PRV_PORT_OFFSET].PIN[pin & BSP_IO_PRV_8BIT_MASK].PmnPFS_b.PMR = 0;

        /* New config with PMR = 0 */
        R_PFS->PORT[pin >> IOPORT_PRV_PORT_OFFSET].PIN[pin &
                                                       BSP_IO_PRV_8BIT_MASK].PmnPFS =
            (bspPeripheral & ~((uint32_t) IOPORT_PRV_PERIPHERAL_FUNCTION));
    }

    /* Write configuration */
    R_PFS->PORT[pin >> IOPORT_PRV_PORT_OFFSET].PIN[pin & BSP_IO_PRV_8BIT_MASK].PmnPFS = bspPeripheral;

    R_BSP_PinAccessDisable();
}


void pinPeripheral(uint32_t pinNumber, uint32_t bspPeripheral) {
  pinPeripheral(digitalPinToBspPin(pinNumber), bspPeripheral);
}