#ifndef ARDUINO_LWIP_CLIENT_H
#define ARDUINO_LWIP_CLIENT_H

#include "Arduino.h"
#include "CNetIf.h"
#include "Client.h"
#include "IPAddress.h"
#include "Print.h"
#include "lwipMem.h"
#include "lwipTcp.h"
#include "lwipTypes.h"

class lwipClient : public Client {

public:
    lwipClient();
    lwipClient(uint8_t sock);
    lwipClient(struct tcp_struct* tcpClient);

    uint8_t status();
    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connect(const char* host, uint16_t port);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t* buf, size_t size);
    virtual int available();
    virtual int read();
    virtual int read(uint8_t* buf, size_t size);
    virtual int peek();
    virtual void flush();
    virtual void stop();
    virtual uint8_t connected();
    virtual operator bool();
    virtual bool operator==(const bool value)
    {
        return bool() == value;
    }
    virtual bool operator!=(const bool value)
    {
        return bool() != value;
    }
    virtual bool operator==(const lwipClient&);
    virtual bool operator!=(const lwipClient& rhs)
    {
        return !this->operator==(rhs);
    };
    uint8_t getSocketNumber();
    virtual uint16_t localPort()
    {
        return (_tcp_client->pcb->local_port);
    };
    virtual IPAddress remoteIP()
    {
        return (IPAddress(_tcp_client->pcb->remote_ip.addr));
    };
    virtual uint16_t remotePort()
    {
        return (_tcp_client->pcb->remote_port);
    };
    void setConnectionTimeout(uint16_t timeout)
    {
        _timeout = timeout;
    }

    friend class lwipServer;

    using Print::write;

private:
    struct tcp_struct* _tcp_client;
    uint16_t _timeout = 10000;
};

#endif
