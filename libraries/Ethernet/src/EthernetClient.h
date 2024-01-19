#pragma once

#include "lwipClient.h"

class EthernetClient : public lwipClient {
public:
    EthernetClient() {
    }
    EthernetClient(struct tcp_pcb *pcb, lwipServer *server)
    : lwipClient(pcb, server) {
    }
    EthernetClient(const lwipClient &c)
    : lwipClient(c) {
        this->bindCNetIf(Ethernet);
    }

    int connect(IPAddress ip, uint16_t port) {
        auto res = lwipClient::connect(ip, port);

        this->bindCNetIf(Ethernet);

        return res;
    }
};
