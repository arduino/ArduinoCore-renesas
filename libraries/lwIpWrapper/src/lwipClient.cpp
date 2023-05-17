extern "C" {
#include "string.h"
}

#include "Arduino.h"

#include "lwipClient.h"

/* -------------------------------------------------------------------------- */
lwipClient::lwipClient()
    : _tcp_client(NULL)
{
}
/* -------------------------------------------------------------------------- */

/* Deprecated constructor. Keeps compatibility with W5100 architecture
sketches but sock is ignored. */
/* -------------------------------------------------------------------------- */
lwipClient::lwipClient(uint8_t sock)
    : _tcp_client(NULL)
{
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
lwipClient::lwipClient(struct tcp_struct* tcpClient)
{
    _tcp_client = tcpClient;
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
int lwipClient::connect(const char* host, uint16_t port)
{
    /* -------------------------------------------------------------------------- */
    IPAddress remote_addr;

    int ret = CLwipIf::getInstance().getHostByName(host, remote_addr);
    if (ret == 1) {
        return connect(remote_addr, port);
    } else {
        return 0;
    }
}

/* -------------------------------------------------------------------------- */
int lwipClient::connect(IPAddress ip, uint16_t port)
{
    /* -------------------------------------------------------------------------- */
    if (_tcp_client == NULL) {
        /* Allocates memory for client */
        _tcp_client = (struct tcp_struct*)mem_malloc(sizeof(struct tcp_struct));

        if (_tcp_client == NULL) {
            return 0;
        }
    }

    /* Creates a new TCP protocol control block */
    _tcp_client->pcb = tcp_new();

    if (_tcp_client->pcb == NULL) {
        return 0;
    }

    _tcp_client->data.p = NULL;
    _tcp_client->data.available = 0;
    _tcp_client->state = TCP_NONE;

    uint32_t startTime = millis();
    ip_addr_t ipaddr;
    tcp_arg(_tcp_client->pcb, _tcp_client);
    if (ERR_OK != tcp_connect(_tcp_client->pcb, u8_to_ip_addr(rawIPAddress(ip), &ipaddr), port, &tcp_connected_callback)) {
        stop();
        return 0;
    }

    startTime = millis();
    while (_tcp_client->state == TCP_NONE) {
        CLwipIf::getInstance().lwip_task();
        if ((_tcp_client->state == TCP_CLOSING) || ((millis() - startTime) >= _timeout)) {
            stop();
            return 0;
        }
    }

    return 1;
}

/* -------------------------------------------------------------------------- */
size_t lwipClient::write(uint8_t b)
{
    /* -------------------------------------------------------------------------- */
    return write(&b, 1);
}

/* -------------------------------------------------------------------------- */
size_t lwipClient::write(const uint8_t* buf, size_t size)
{
    /* -------------------------------------------------------------------------- */
    if ((_tcp_client == NULL) || (_tcp_client->pcb == NULL) || (buf == NULL) || (size == 0)) {
        return 0;
    }

    /* If client not connected or accepted, it can't write because connection is
    not ready */
    if ((_tcp_client->state != TCP_ACCEPTED) && (_tcp_client->state != TCP_CONNECTED)) {
        return 0;
    }

    size_t max_send_size, bytes_to_send;
    size_t bytes_sent = 0;
    size_t bytes_left = size;
    err_t res;

    do {
        max_send_size = tcp_sndbuf(_tcp_client->pcb);
        bytes_to_send = bytes_left > max_send_size ? max_send_size : bytes_left;

        if (bytes_to_send > 0) {
            res = tcp_write(_tcp_client->pcb, &buf[bytes_sent], bytes_to_send, TCP_WRITE_FLAG_COPY);

            if (res == ERR_OK) {
                bytes_sent += bytes_to_send;
                bytes_left = size - bytes_sent;
            } else if (res != ERR_MEM) {
                // other error, cannot continue
                return 0;
            }
        }

        // Force to send data right now!
        if (ERR_OK != tcp_output(_tcp_client->pcb)) {
            return 0;
        }
        CLwipIf::getInstance().lwip_task();

    } while (bytes_sent != size);

    return size;
}

/* -------------------------------------------------------------------------- */
int lwipClient::available()
{
    /* -------------------------------------------------------------------------- */
    CLwipIf::getInstance().lwip_task();
    if (_tcp_client != NULL) {
        return _tcp_client->data.available;
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
int lwipClient::read()
{
    /* -------------------------------------------------------------------------- */
    uint8_t b;
    if ((_tcp_client != NULL) && (_tcp_client->data.p != NULL)) {
        __disable_irq();
        pbuffer_get_data(&(_tcp_client->data), &b, 1);
        __enable_irq();
        return b;
    }
    // No data available
    return -1;
}

/* -------------------------------------------------------------------------- */
int lwipClient::read(uint8_t* buf, size_t size)
{
    /* -------------------------------------------------------------------------- */
    if ((_tcp_client != NULL) && (_tcp_client->data.p != NULL)) {
        __disable_irq();
        int rv = pbuffer_get_data(&(_tcp_client->data), buf, size);
        __enable_irq();
        return rv;
    }
    return -1;
}

/* -------------------------------------------------------------------------- */
int lwipClient::peek()
{
    /* -------------------------------------------------------------------------- */
    uint8_t b;
    // Unlike recv, peek doesn't check to see if there's any data available, so we must
    if (!available()) {
        return -1;
    }
    __disable_irq();
    b = pbuf_get_at(_tcp_client->data.p, 0);
    __enable_irq();
    return b;
}

/* -------------------------------------------------------------------------- */
void lwipClient::flush()
{
    /* -------------------------------------------------------------------------- */
    if ((_tcp_client == NULL) || (_tcp_client->pcb == NULL)) {
        return;
    }
    tcp_output(_tcp_client->pcb);
    CLwipIf::getInstance().lwip_task();
}

/* -------------------------------------------------------------------------- */
void lwipClient::stop()
{
    /* -------------------------------------------------------------------------- */
    if (_tcp_client == NULL) {
        return;
    }

    // close tcp connection if not closed yet
    if (status() != TCP_CLOSING) {
        tcp_connection_close(_tcp_client->pcb, _tcp_client);
    }
}

/* -------------------------------------------------------------------------- */
uint8_t lwipClient::connected()
{
    /* -------------------------------------------------------------------------- */
    uint8_t s = status();
    return ((available() && (s == TCP_CLOSING)) || (s == TCP_CONNECTED) || (s == TCP_ACCEPTED));
}

/* -------------------------------------------------------------------------- */
uint8_t lwipClient::status()
{
    if (_tcp_client == NULL) {
        return TCP_NONE;
    }
    return _tcp_client->state;
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.

/* -------------------------------------------------------------------------- */
lwipClient::operator bool()
{
    /* -------------------------------------------------------------------------- */
    return (_tcp_client && (_tcp_client->state != TCP_CLOSING));
}

/* -------------------------------------------------------------------------- */
bool lwipClient::operator==(const lwipClient& rhs)
{
    /* -------------------------------------------------------------------------- */
    return _tcp_client == rhs._tcp_client && _tcp_client->pcb == rhs._tcp_client->pcb;
}

/* This function is not a function defined by Arduino. This is a function
specific to the W5100 architecture. To keep the compatibility we leave it and
returns always 0. */
/* -------------------------------------------------------------------------- */
uint8_t lwipClient::getSocketNumber()
{
    /* -------------------------------------------------------------------------- */
    return 0;
}
