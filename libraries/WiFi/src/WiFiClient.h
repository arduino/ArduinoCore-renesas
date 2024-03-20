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

    int connect(const char* host, uint16_t port) {
        auto res = lwipClient::connect(host, port);

        this->bindCNetIf(WiFiStation);

        return res;
    }

    int connect(IPAddress ip, uint16_t port) {
        auto res = lwipClient::connect(ip, port);

        this->bindCNetIf(WiFiStation);

        return res;
    }
};
