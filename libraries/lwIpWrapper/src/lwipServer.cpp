extern "C" {
#include "string.h"
}

#include "CNetIf.h"
#include "lwipClient.h"
#include "lwipServer.h"

lwipServer::lwipServer(uint16_t port)
{
    _port = port;
    _tcp_server = {};
}

void lwipServer::begin()
{
    if (_tcp_server.pcb != NULL) {
        return;
    }

    _tcp_server.pcb = tcp_new();

    if (_tcp_server.pcb == NULL) {
        return;
    }

    tcp_arg(_tcp_server.pcb, &_tcp_client);
    _tcp_server.state = TCP_NONE;

    if (ERR_OK != tcp_bind(_tcp_server.pcb, IP_ADDR_ANY, _port)) {
        memp_free(MEMP_TCP_PCB, _tcp_server.pcb);
        _tcp_server.pcb = NULL;
        return;
    }

    _tcp_server.pcb = tcp_listen(_tcp_server.pcb);
    tcp_accept(_tcp_server.pcb, tcp_accept_callback);
}

void lwipServer::begin(uint16_t port)
{
    _port = port;
    begin();
}

void lwipServer::accept()
{
    /* Free client if disconnected */
    for (int n = 0; n < MAX_CLIENT; n++) {
        if (_tcp_client[n] && !_tcp_client[n].connected()) {
            _tcp_client[n] = lwipClient();
        }
    }
}

lwipClient lwipServer::available()
{
    accept();

    for (int n = 0; n < MAX_CLIENT; n++) {
        if (_tcp_client[n].available()) {
            return _tcp_client[n];
        }
    }

    return lwipClient();
}

size_t lwipServer::write(uint8_t b)
{
    return write(&b, 1);
}

size_t lwipServer::write(const uint8_t* buffer, size_t size)
{
    size_t n = 0;

    accept();

    for (int n = 0; n < MAX_CLIENT; n++) {
        if (_tcp_client[n].status() == TCP_ACCEPTED) {
            n += _tcp_client[n].write(buffer, size);
        }
    }

    return n;
}
