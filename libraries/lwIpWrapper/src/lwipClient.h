#pragma once
#include <Client.h>
#include <lwip/include/lwip/tcp.h>
#include <IPAddress.h>
#include <Print.h>

class lwipClient : public Client {

public:
    lwipClient();
    lwipClient(uint8_t sock);
    lwipClient(struct tcp_struct* tcpClient); // FIXME this should be a private constructor, friend of Server

    // disable copy constructor
    LWIPTCPClient(const LWIPTCPClient&) = delete;
    void operator=(const LWIPTCPClient&) = delete;

    // keep move constructor
    LWIPTCPClient(LWIPTCPClient&&);
    void operator=(LWIPTCPClient&&);

    virtual ~LWIPTCPClient();

    virtual uint8_t status();
    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connect(const char* host, uint16_t port);

    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t* buf, size_t size);

    inline virtual int available() { return this->pbuf_head == nullptr ? 0 : this->pbuf_head->tot_len; }

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
        return (_tcp_client->pcb->local_port);
    };
    virtual IPAddress remoteIP() {
        return (IPAddress(_tcp_client->pcb->remote_ip.addr));
    };
    virtual uint16_t remotePort() {
        return (_tcp_client->pcb->remote_port);
    };
    void setConnectionTimeout(uint16_t timeout) {
        _timeout = timeout;
    }

    friend class lwipServer;

    using Print::write;

private:
    enum _tcp_state_t: uint8_t {
        TCP_NONE = 0,
        // TCP_ACCEPTED,
        TCP_CONNECTED,
        TCP_CLOSING
    };

    // TCP related info of the socket
    _tcp_state_t state =        TCP_NONE;
    struct pbuf* pbuf_head =    nullptr;
    struct tcp_pcb* pcb =              nullptr;
    uint16_t pbuf_offset =      0;

    uint16_t _timeout = 10000;
    ip_addr_t _ip;

    err_t connected_callback(struct tcp_pcb* tpcb, err_t err);
    void free_pbuf_chain(uint16_t copied);
    err_t recv_callback(struct tcp_pcb* tpcb, struct pbuf* p, err_t err);

    friend err_t _lwip_tcp_recv_callback(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err);
    friend err_t _lwip_tcp_connected_callback(void* arg, struct tcp_pcb* tpcb, err_t err);
};
