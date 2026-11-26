#ifndef ARDUINO_LWIP_WIFI_SERVER_H
#define ARDUINO_LWIP_WIFI_SERVER_H


#include "lwipServer.h"
#include "WiFiClient.h"

class WiFiServer : public lwipServer {
   public:
   WiFiServer() {}
   WiFiServer(uint16_t port) : lwipServer(port) {}

   WiFiClient available() {
     lwipClient client = lwipServer::available();
     return WiFiClient(client);
   }
};

#endif