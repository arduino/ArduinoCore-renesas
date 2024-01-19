extern "C" {
#include "string.h"
}

#include "CNetIf.h"
#include "lwipClient.h"
#include "lwipServer.h"
#include "utils.h"

err_t tcp_accept_callback(void* arg, struct tcp_pcb* newpcb, err_t err);

lwipServer::lwipServer(const IPAddress &listen_ip, uint16_t port)
: _port(port), listen_address(listen_ip), server_pcb(nullptr) {
}

lwipServer::lwipServer(uint16_t port)
: _port(port), listen_address(INADDR_NONE), server_pcb(nullptr) {
}

void lwipServer::begin()
{
    if (server_pcb != NULL) {
        return;
    }

    server_pcb = tcp_new();

    if (server_pcb == NULL) {
        return;
    }

    tcp_arg(server_pcb, this);

    if (ERR_OK != tcp_bind(server_pcb, IP_ADDR_ANY, _port)) { // TODO Put the listen address here
        memp_free(MEMP_TCP_PCB, server_pcb);
        server_pcb = NULL;
        return;
    }

    server_pcb = tcp_listen(server_pcb);
    tcp_accept(server_pcb, tcp_accept_callback);
}

void lwipServer::begin(uint16_t port)
{
    _port = port;
    begin();
}

// void lwipServer::clean() {
//     // this index is a placeholder to the first empty position that needs to be filled
//     int8_t moveto = -1;

//     new_size = size;
//     // remove all the closed clients
//     for (int i=0; i < size; i++) {
//         if (client.status() == TCP_CLOSING) {
//             delete clients[n];
//             clients[n] = nullptr;
//             new_size--;

//             if(moveto == -1) {
//                 moveto = n;
//             }
//         }

//         if(moveto >= 0 && clients[n] != nullptr) {
//             clients[moveto] = clients[n];
//             clients[n] = nullptr;
//             moveto++;
//         }
//     }

//     size = new_size
// }

void lwipServer::remove(lwipClient* client) {
    arduino::lock();

    bool found = false;
    for (int i=0; i < size; i++) {
        if(found) {
            // we move the client to delete to the end of the array, then we remove it
            clients[i-1] = clients[i];
        } else if(*client == *clients[i]) {
            found = true;
        }
    }

    delete clients[--size];
    clients[size] = nullptr;

    arduino::unlock();
}

bool lwipServer::accept(struct tcp_pcb* new_client) {
    bool res = false;
    // this->clean();
    arduino::lock();
    if(size < MAX_CLIENT-1) {
        clients[size] = new lwipClient(new_client, this);
        size++;
        clients_available++;
        res = true;
    }
    arduino::unlock();

    return res;
}

lwipClient lwipServer::available()
{
    lwipClient* res = available_ptr();
    return res != nullptr ? *res : CLIENT_NONE;
}

lwipClient* lwipServer::available_ptr()
{
    lwipClient* res=nullptr;
    arduino::lock();
    if(size > 0 && clients_available>0) {
        res = clients[size-clients_available--]; // TODO verify index
    }
    arduino::unlock();

    return res;
}

size_t lwipServer::write(uint8_t b)
{
    return write(&b, 1);
}

size_t lwipServer::write(const uint8_t* buffer, size_t size) {
    arduino::lock();
    size_t written=0;
    // this->clean();

    for (int i = 0; i < MAX_CLIENT; i++) {
        written += clients[i]->write(buffer, size);
    }
    arduino::unlock();

    return written;
}

err_t tcp_accept_callback(void* arg, struct tcp_pcb* newpcb, err_t err) {
    arduino::lock();
    lwipServer* server = (lwipServer*) arg;
    err_t ret_err = ERR_OK;

    /* set priority for the newly accepted tcp connection newpcb */
    tcp_setprio(newpcb, TCP_PRIO_MIN);

    if ((arg == NULL) || (ERR_OK != err) || !server->accept(newpcb)) {
        tcp_close(newpcb);
        ret_err = ERR_ARG;
    }
    arduino::unlock();
    return ret_err;
}