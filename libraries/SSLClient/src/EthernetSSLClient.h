#ifndef ARDUINO_LWIP_ETHERNET_SSL_CLIENT_H
#define ARDUINO_LWIP_ETHERNET_SSL_CLIENT_H

#include "EthernetClient.h"
#include "SSLClient.h"

class EthernetSSLClient : public SSLClient {

  public:
    EthernetSSLClient();
    ~EthernetSSLClient();

  private:
    EthernetClient * _client;

};

#endif

