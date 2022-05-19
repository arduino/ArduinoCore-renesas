#include "Arduino.h"

extern ioport_instance_ctrl_t g_ioport_ctrl;

void pinMode(pin_size_t pin, PinMode mode) {
	switch (mode) {
		case INPUT:
			R_IOPORT_PinCfg(&g_ioport_ctrl, g_APinDescription[pin].name, IOPORT_CFG_PORT_DIRECTION_INPUT);
			break;
		case INPUT_PULLUP:
			R_IOPORT_PinCfg(&g_ioport_ctrl, g_APinDescription[pin].name, IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE);
			break;
		case OUTPUT:
			R_IOPORT_PinCfg(&g_ioport_ctrl, g_APinDescription[pin].name, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
			break;
	}
}

void digitalWrite(pin_size_t pin, PinStatus val) {
	R_IOPORT_PinWrite(&g_ioport_ctrl, g_APinDescription[pin].name, val == LOW ? BSP_IO_LEVEL_LOW : BSP_IO_LEVEL_HIGH);
}

PinStatus digitalRead(pin_size_t pin) {
	bsp_io_level_t ret;
	R_IOPORT_PinRead(&g_ioport_ctrl, g_APinDescription[pin].name, &ret);
	return (ret == BSP_IO_LEVEL_LOW ? LOW : HIGH);
}
