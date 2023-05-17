#ifndef ARDUINO_LWIP_ETHERNET_CLIENT_H
#define ARDUINO_LWIP_ETHERNET_CLIENT_H


#include "lwipClient.h"

class EthernetClient : public lwipClient {
   public:
   EthernetClient() {}
   EthernetClient(struct tcp_struct *tcpClient) : lwipClient(tcpClient) {}
};

#endif