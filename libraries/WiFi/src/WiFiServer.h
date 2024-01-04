#pragma once

#include "lwipServer.h"
#include "lwipClient.h"

class WiFiServer: public lwipServer {
public:
    void begin() {
        lwipServer::begin();
        this->bindCNetIf(WiFi);
    }

    WiFiClient available() {
        return WiFiClient(lwipServer::available());
    }
};
