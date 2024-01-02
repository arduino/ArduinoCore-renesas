#ifndef ARDUINO_LWIP_ETHERNET_CLIENT_H
#define ARDUINO_LWIP_ETHERNET_CLIENT_H


#include "lwipClient.h"

class EthernetClient : public lwipClient {
public:
   EthernetClient() {
      this->bindCNetIf(Ethernet);
   }
   EthernetClient(struct tcp_pcb *pcb) : lwipClient(pcb) {
      this->bindCNetIf(Ethernet);
   }
};

#endif