#include "Arduino.h"

extern ioport_instance_ctrl_t g_ioport_ctrl;

void pinMode(pin_size_t pin, PinMode mode) {
	R_IOPORT_PinCfg(&g_ioport_ctrl, g_APinDescription[pin].name, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
}

void digitalWrite(pin_size_t pin, PinStatus val) {
	R_IOPORT_PinWrite(&g_ioport_ctrl, g_APinDescription[pin].name, val == LOW ? BSP_IO_LEVEL_LOW : BSP_IO_LEVEL_HIGH);
}

PinStatus digitalRead(pin_size_t pin) {
	bsp_io_level_t ret;
	R_IOPORT_PinRead(&g_ioport_ctrl, g_APinDescription[pin].name, &ret);
	return (ret == BSP_IO_LEVEL_LOW ? LOW : HIGH);
}
