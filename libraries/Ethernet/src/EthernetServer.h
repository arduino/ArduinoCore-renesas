#pragma once

#include "lwipServer.h"
#include "EthernetClient.h"

class EthernetServer: public lwipServer {
public:
    EthernetServer(uint16_t port) : lwipServer(port) {}
    void begin() {
        lwipServer::begin();
        this->bindCNetIf(Ethernet);
    }

    EthernetClient available() {
        lwipClient* res = available_ptr();
        return res != nullptr ? EthernetClient(*res) : EthernetClient(CLIENT_NONE);
    }
};
