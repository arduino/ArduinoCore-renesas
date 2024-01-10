#pragma once
#include <Client.h>
#include <lwip/include/lwip/tcp.h>
#include <IPAddress.h>
#include <Print.h>
#include "CNetIf.h"
#include "lwipServer.h"
#include <memory>

// TODO improve documentation

enum _tcp_state_t: uint8_t {
    TCP_NONE = 0,
    TCP_ACCEPTED,
    TCP_CONNECTED,
    TCP_CLOSING
};

class lwipClient : public arduino::Client {
public:
    lwipClient();
    lwipClient(uint8_t sock);
    lwipClient(struct tcp_pcb* tcpClient, lwipServer *server); // FIXME this should be a private constructor, friend of Server

    // disable copy constructor
    lwipClient(const lwipClient&);
    lwipClient& operator=(const lwipClient&);

    // keep move constructor
    lwipClient(lwipClient&&);
    lwipClient& operator=(lwipClient&&);

    virtual ~lwipClient();

    virtual uint8_t status();
    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connect(const char* host, uint16_t port);

    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t* buf, size_t size);

    inline virtual int available() {
        return this->tcp_info->pbuf_head == nullptr ? 0 : this->tcp_info->pbuf_head->tot_len - this->tcp_info->pbuf_offset;
    }

    virtual int read();
    virtual int read(uint8_t* buf, size_t size);
    size_t read_until_token(
        const uint8_t* buffer, uint16_t buffer_size, char* token, bool &found);

    virtual int peek();
    virtual void flush();
    virtual void stop();
    virtual uint8_t connected();
    virtual operator bool();

    virtual bool operator==(const bool value) {
        return bool() == value;
    }
    virtual bool operator!=(const bool value) {
        return bool() != value;
    }
    virtual bool operator==(const lwipClient&); // TODO why do we need this comparison operators?
    virtual bool operator!=(const lwipClient& rhs) {
        return !this->operator==(rhs);
    };

    uint8_t getSocketNumber();
    virtual uint16_t localPort() {
        return (this->tcp_info->pcb->local_port);
    };
    virtual IPAddress remoteIP() {
        return (IPAddress(this->tcp_info->pcb->remote_ip.addr));
    };
    virtual uint16_t remotePort() {
        return (this->tcp_info->pcb->remote_port);
    };
    void setConnectionTimeout(uint16_t timeout) {
        _timeout = timeout;
    }

    void bindCNetIf(CNetIf &n) {
        tcp_bind_netif(this->tcp_info->pcb, n.getNi());
    }

    friend class lwipServer;

    using Print::write;

private:
    // TCP related info of the socket
    struct tcp_info_t {
        _tcp_state_t state;
        struct pbuf* pbuf_head;
        struct tcp_pcb* pcb;
        uint16_t pbuf_offset;
        // this pointer is used to correctly clean the lwipClient when created from a server class
        lwipServer* server;
    };

    std::shared_ptr<tcp_info_t> tcp_info;

    uint16_t _timeout = 10000;
    ip_addr_t _ip;

    err_t connected_callback(struct tcp_pcb* tpcb, err_t err);
    err_t recv_callback(struct tcp_pcb* tpcb, struct pbuf* p, err_t err);

    friend err_t _lwip_tcp_recv_callback(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err);
    friend err_t _lwip_tcp_connected_callback(void* arg, struct tcp_pcb* tpcb, err_t err);
};

inline const lwipClient CLIENT_NONE(nullptr, nullptr);