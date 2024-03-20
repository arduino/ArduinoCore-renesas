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
        lwipClient* res = available_ptr();
        return res != nullptr ? WiFiClient(*res) : WiFiClient(CLIENT_NONE);
    }
};
