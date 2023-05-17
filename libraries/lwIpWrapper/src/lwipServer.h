#ifndef _ARDUINO_LWIP_SERVER_H
#define _ARDUINO_LWIP_SERVER_H

#include "Server.h"
#include "lwipTcp.h"

class lwipClient;

class lwipServer : public Server {
protected:
    uint16_t _port;
    struct tcp_struct _tcp_server;
    struct tcp_struct* _tcp_client[MAX_CLIENT];

    void accept(void);

public:
    lwipServer(uint16_t port = 80);
    lwipClient available();
    virtual void begin();
    virtual void begin(uint16_t port);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t* buf, size_t size);
    using Print::write;
};

#endif