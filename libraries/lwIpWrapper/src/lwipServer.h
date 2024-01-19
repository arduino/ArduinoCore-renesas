#pragma once

#include <Server.h>
#include <IPAddress.h>
#include <CNetIf.h>
// #include "lwipClient.h"

class lwipClient;

class lwipServer: public Server {
public:
    lwipServer(const IPAddress &listen_ip = INADDR_NONE, uint16_t port = 80);
    lwipServer(uint16_t port = 80);
    lwipClient available();

    virtual void begin();
    virtual void begin(uint16_t port);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t* buf, size_t size);
    using Print::write;

    void bindCNetIf(CNetIf &n) {
        tcp_bind_netif(this->server_pcb, n.getNi());
    }
protected:
    /*
     * these methods are used to insert and remove lwipClients in lwipServer class
     * the idea is the following:
     * - a client is created by lwip, we wrap it around a lwipClient class and and returned by available() call
     * - the client is inserted in the last empty position of the clients array
     * - when a client connection is closed (by calling stop on it or delete on the client)
     *   the server is notified and the remove() method is called thus the client is removed from the server list.
     */
    bool accept(struct tcp_pcb* new_client);
    // void clean();
    void remove(lwipClient* client);

    lwipClient* available_ptr();

    uint16_t _port;
    const IPAddress &listen_address;
    tcp_pcb* server_pcb;
    uint16_t port;

    // this array in managed as a fixed size list with all the null values in the back.
    // size var indicates how full is the array
    // the total number of pcb should be MEMP_NUM_TCP_PCB, -1 that is the server PCB
private:
    uint8_t size=0, clients_available=0;
    lwipClient* clients[MAX_CLIENT-1];

    friend err_t tcp_accept_callback(void* arg, struct tcp_pcb* newpcb, err_t err);
    friend lwipClient;
};
