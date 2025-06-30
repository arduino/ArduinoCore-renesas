#ifndef ARDUINO_LWIP_ETHERNET_SERVER_H
#define ARDUINO_LWIP_ETHERNET_SERVER_H



#include "lwipServer.h"
#include "EthernetClient.h"

class EthernetServer : public lwipServer {
   public:
   EthernetServer() {}
   EthernetServer(uint16_t port) : lwipServer(port) {}

   EthernetClient available() {
     lwipClient client = lwipServer::available();
     return EthernetClient(client);
   }
};

#endif