#ifndef ARDUINO_WIFI_UDP_CLIENT_H
#define ARDUINO_WIFI_UDP_CLIENT_H


#include "lwipUDP.h"

class WiFiUDP : public lwipUDP {
   public:
   WiFiUDP() {}
   virtual uint8_t begin(uint16_t port) override {
      CNetIf *ni = CLwipIf::getInstance().get(NI_WIFI_STATION);
      if(ni != nullptr) {
         return lwipUDP::begin(IPAddress(ni->getIpAdd()), port);
      }
      return 0;
   }
};

#endif