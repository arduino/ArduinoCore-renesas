#ifndef ARDUINO_LWIP_WIFI_CLIENT_H
#define ARDUINO_LWIP_WIFI_CLIENT_H


#include "lwipClient.h"

class WiFiClient : public lwipClient {
   public:
   WiFiClient() {
      this->bindCNetIf(WiFi);
   }
   WiFiClient(struct tcp_pcb *pcb) : lwipClient(pcb) {
      this->bindCNetIf(WiFi);
   }
};

#endif

