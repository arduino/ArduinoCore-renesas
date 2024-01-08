#pragma once

#include "lwipServer.h"
#include "lwipClient.h"

class WiFiServer: public lwipServer {
public:
    WiFiServer(uint16_t port) : lwipServer(port) {}
    void begin() {
        lwipServer::begin();
        this->bindCNetIf(WiFiStation);
    }

    WiFiClient available() {
        return WiFiClient(lwipServer::available());
    }
};
