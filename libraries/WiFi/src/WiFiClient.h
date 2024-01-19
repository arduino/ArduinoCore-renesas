#pragma once

#include "lwipClient.h"

class WiFiClient: public lwipClient {
public:
    WiFiClient() {
    }
    WiFiClient(struct tcp_pcb *pcb, lwipServer *server)
    : lwipClient(pcb, server) {
    }
    WiFiClient(const lwipClient &c)
    : lwipClient(c) {
        this->bindCNetIf(WiFiStation);
    }

    int connect(IPAddress ip, uint16_t port) {
        auto res = lwipClient::connect(ip, port);

        this->bindCNetIf(WiFiStation);

        return res;
    }
};
