#ifndef ARDUINO_LWIP_WIFI_CLIENT_H
#define ARDUINO_LWIP_WIFI_CLIENT_H


#include "lwipClient.h"

class WiFiClient : public lwipClient {
   public:
   WiFiClient() {}
   WiFiClient(struct tcp_struct *tcpClient) : lwipClient(tcpClient) {}
};

#endif

