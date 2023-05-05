#ifndef ethernetsslclient_h
#define ethernetsslclient_h

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

