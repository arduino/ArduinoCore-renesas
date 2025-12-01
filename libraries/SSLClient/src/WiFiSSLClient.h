#ifndef ARDUINO_LWIP_WIFI_SSL_CLIENT_H
#define ARDUINO_LWIP_WIFI_SSL_CLIENT_H

#include "WiFiClient.h"
#include "SSLClient.h"

class WiFiSSLClient : public SSLClient {

  public:
    WiFiSSLClient();
    ~WiFiSSLClient();

  private:
    WiFiClient * _client;

};

#endif

