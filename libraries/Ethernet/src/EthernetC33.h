#pragma once

#ifndef ARDUINO_PORTENTA_C33
// force discovering wth shield library
// this doesn't work ATM
// a check and a bailout should be added to all the other files too
#include "utility/w5100.h"
#endif

#include <inttypes.h>
#include "IPAddress.h"

#include "EthernetClient.h"
#include "EthernetServer.h"
#include "EthernetDriver.h"

#include "CNetIf.h"

#ifdef ARDUINO_PORTENTA_C33

// TODO Instantiate the drivers for ethernet with default configuration parameters
inline EthernetC33Driver EthernetDriver(2, 2, mem_malloc, 1536);

// FIXME Instantiate a global variable from CEth, calling it Ethernet
inline CEth Ethernet(&EthernetDriver);

#endif // ARDUINO_PORTENTA_C33
