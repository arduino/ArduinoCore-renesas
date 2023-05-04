#ifndef ARDUINO_LWIP_ETHERNET_UDP_SERVER_H
#define ARDUINO_LWIP_ETHERNET_UDP_SERVER_H




#include "lwipUDP.h"

class EthernetUDP : public lwipUDP {
   public:
   EthernetUDP() {}
   CNetIf *ni = CLwipIf::getInstance().get(NI_ETHERNET);
      if(ni != nullptr) {
         return lwipUDP::begin(IPAddress(ni->getIpAdd()), port);
      }
      return 0;
};


#endif

