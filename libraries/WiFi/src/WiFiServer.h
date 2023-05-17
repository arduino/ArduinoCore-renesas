#ifndef ARDUINO_LWIP_WIFI_SERVER_H
#define ARDUINO_LWIP_WIFI_SERVER_H


#include "lwipServer.h"
#include "WiFiClient.h"

class WiFiServer : public lwipServer {
   public:
   WiFiServer() {}
   WiFiServer(uint16_t port) : lwipServer(port) {}

   WiFiClient available() {
      accept();

      for (int n = 0; n < MAX_CLIENT; n++) {
         if (_tcp_client[n] != NULL) {
            if (_tcp_client[n]->pcb != NULL) {
               WiFiClient client(_tcp_client[n]);
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
     return WiFiClient(default_client);
   }
};

#endif