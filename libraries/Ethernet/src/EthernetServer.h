#ifndef ARDUINO_LWIP_ETHERNET_SERVER_H
#define ARDUINO_LWIP_ETHERNET_SERVER_H



#include "lwipServer.h"
#include "EthernetClient.h"

class EthernetServer : public lwipServer {
   public:
   EthernetServer() {}
   EthernetServer(uint16_t port) : lwipServer(port) {}

   EthernetClient available() {
      accept();

      for (int n = 0; n < MAX_CLIENT; n++) {
         if (_tcp_client[n] != NULL) {
            if (_tcp_client[n]->pcb != NULL) {
               EthernetClient client(_tcp_client[n]);
               uint8_t s = client.status();
               if (s == TCP_ACCEPTED) {
                  if (client.available()) {
                     return client;
                  }
               }
            }
         }
     }

     struct tcp_struct *default_client = NULL;
     return EthernetClient(default_client);
   }
};

#endif