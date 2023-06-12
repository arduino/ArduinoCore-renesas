#include "Serial.h"

#if SERIAL_HOWMANY > 1
UART _UART2_(UART2_TX_PIN, UART2_RX_PIN);
#endif