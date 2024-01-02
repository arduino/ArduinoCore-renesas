#pragma once

#include "CNetIf.h"

#include "WiFiClient.h"
#include "WiFiServer.h"
#include "WiFiUdp.h"

#define WIFI_FIRMWARE_LATEST_VERSION "1.5.0"

#ifdef ARDUINO_PORTENTA_C33
// TODO Instantiate the drivers for wifi with default configuration parameters
// ESPHostFGDriver WifiDriver;

// Instantiate a global variable from CWifiStation calling it WiFi

inline CWifiStation WiFi;
#endif // ARDUINO_PORTENTA_C33
