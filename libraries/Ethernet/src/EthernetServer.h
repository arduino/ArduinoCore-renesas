#pragma once

#include "lwipServer.h"
#include "EthernetClient.h"

class EthernetServer: public lwipServer {
public:
    void begin() {
        lwipServer::begin();
        this->bindCNetIf(WiFi);
    }

    EthernetClient available() {
        return EthernetClient(lwipServer::available());
    }
};
