#include "Serial.h"

#if SERIAL_HOWMANY > 0
UART _UART1_(UART1_TX_PIN, UART1_RX_PIN);
#endif