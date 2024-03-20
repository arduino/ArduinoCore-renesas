#ifndef ARDUINO_LWIP_ETHERNET_UDP_SERVER_H
#define ARDUINO_LWIP_ETHERNET_UDP_SERVER_H




#include "lwipUDP.h"

class EthernetUDP : public lwipUDP {
   public:
   EthernetUDP() {}
   virtual uint8_t begin(uint16_t port) override {
      CNetIf *ni = &Ethernet;
      if(ni != nullptr) {
         return lwipUDP::begin(IPAddress(ni->getIpAdd()), port);
      }
      return 0;
   }
};


#endif

