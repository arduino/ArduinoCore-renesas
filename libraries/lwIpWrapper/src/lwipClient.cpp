extern "C" {
#include "string.h"
}

#include "Arduino.h"

#include "lwipClient.h"
#include "lwippbuf.h"
#include "CNetIf.h"
#include "utils.h"
// FIXME understand hos to syncronize the interrupt thread and "userspace"
// TODO look into tcp_bind_netif for Ethernet and WiFiClient classes
// TODO generalize the functions for extracting and inserting data into pbufs, they may be reused in UDP
// TODO look into application polling:
//      When a connection is idle (i.e., no data is either transmitted or received), lwIP will repeatedly poll the application by calling a specified callback function. This can be used either as a watchdog timer for killing connections that have stayed idle for too long, or as a method of waiting for memory to become available. For instance, if a call to tcp_write() has failed because memory wasn't available, the application may use the polling functionality to call tcp_write() again when the connection has been idle for a while.


// Forward declarations
err_t _lwip_tcp_connected_callback(void* arg, struct tcp_pcb* tpcb, err_t err);
err_t _lwip_tcp_recv_callback(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err);
static err_t _lwip_tcp_sent_callback(void* arg, struct tcp_pcb* tpcb, u16_t len);
void _lwip_tcp_err_callback(void *arg, err_t err);

lwipClient::lwipClient()
: tcp_info(new tcp_info_t)
{
    // tcp_info = std::shared_ptr<tcp_info_t>(new tcp_info_t);
    this->tcp_info->state         = TCP_NONE;
    this->tcp_info->pcb           = nullptr;
    this->tcp_info->server        = nullptr;
    this->tcp_info->pbuf_offset   = 0;
    this->tcp_info->pbuf_head     = nullptr;
}

/* Deprecated constructor. Keeps compatibility with W5100 architecture
sketches but sock is ignored. */
lwipClient::lwipClient(uint8_t sock) {}

lwipClient::lwipClient(struct tcp_pcb* pcb, lwipServer *server)
: tcp_info(new tcp_info_t)
{
    // tcp_info = std::shared_ptr<tcp_info_t>(new tcp_info_t);
    this->tcp_info->state         = TCP_ACCEPTED;
    this->tcp_info->pcb           = pcb;
    this->tcp_info->server        = server;
    this->tcp_info->pbuf_offset   = 0;
    this->tcp_info->pbuf_head     = nullptr;

    tcp_arg(this->tcp_info->pcb, this);

    tcp_err(this->tcp_info->pcb, _lwip_tcp_err_callback); // FIXME make this a user callback?

    /* initialize LwIP tcp_recv callback function */
    tcp_recv(this->tcp_info->pcb, _lwip_tcp_recv_callback);

    /* initialize LwIP tcp_sent callback function */
    tcp_sent(this->tcp_info->pcb, _lwip_tcp_sent_callback); // FIXME do we actually need it?
}

lwipClient::lwipClient(const lwipClient& c)
: tcp_info(c.tcp_info), _timeout(c._timeout), _ip(c._ip) {
}

lwipClient& lwipClient::operator=(const lwipClient& rhs) {
    this->tcp_info =    rhs.tcp_info;
    this->_timeout =    rhs._timeout;
    this->_ip =         rhs._ip;
    return *this;
}

lwipClient::lwipClient(lwipClient&& c)
: tcp_info(std::move(c.tcp_info)), _timeout(std::move(c._timeout)), _ip(std::move(c._ip)) {
}

lwipClient& lwipClient::operator=(lwipClient&& rhs) {
    this->tcp_info =    std::move(rhs.tcp_info);
    this->_timeout =    std::move(rhs._timeout);
    this->_ip =         std::move(rhs._ip);
    return *this;
}

lwipClient::~lwipClient() {
    if(this->tcp_info->state != TCP_CLOSING) {
        this->stop();
    }
}

int lwipClient::connect(const char* host, uint16_t port) {
    IPAddress remote_addr;

    int ret = CLwipIf::getInstance().getHostByName(host, remote_addr); // TODO test this
    if (ret == 1) {
        return connect(remote_addr, port);
    } else {
        return 0;
    }
}

int lwipClient::connect(IPAddress ip, uint16_t port) {
    err_t err = ERR_OK;

    // the connect method is only connected when trying to connect a client to a server
    // and not when a client is created out of a listening socket
    this->tcp_info->pcb = tcp_new();

    if(this->tcp_info->pcb == nullptr) {
        // return ; // TODO find the proper error code
        return err;
    }

    tcp_err(this->tcp_info->pcb, _lwip_tcp_err_callback); // FIXME make this a user callback?
    if(err != ERR_OK) {
        return err;
    }

    this->tcp_info->state = TCP_NONE;

    tcp_arg(this->tcp_info->pcb, this);

    this->_ip = fromArduinoIP(ip);

    // FIXME this doesn't include timeout of connection, does lwip have it by default?
    err = tcp_connect(
        this->tcp_info->pcb, &this->_ip, port, // FIXME check if _ip gets copied
        _lwip_tcp_connected_callback // FIXME we need to define a static private function
    );
    return err;
}

err_t _lwip_tcp_connected_callback(void* arg, struct tcp_pcb* tpcb, err_t err) {
    if(arg == NULL) {
        // Setup was not performed correctly and the arg was not setup properly
        // _lwip_tcp_connection_close(tpcb, tcp_arg);
        // this->stop();// FIXME this doesn't exist

        return ERR_ARG;
    }

    lwipClient* client = (lwipClient*)arg;

    client->connected_callback(tpcb, err);
}

err_t lwipClient::connected_callback(struct tcp_pcb* tpcb, err_t err) {
    if(err != ERR_OK) {
        // lwip_tcp_connection_close(tpcb, tcp_arg);
        this->stop();

        return err;
    }

    if(tcp_arg == NULL) {
        // Setup was not performed correctly and the arg was not setup properly
        // lwip_tcp_connection_close(tpcb, tcp_arg);
        this->stop();

        return ERR_ARG;
    }

    this->tcp_info->state = TCP_CONNECTED;

    /* initialize LwIP tcp_recv callback function */
    tcp_recv(tpcb, _lwip_tcp_recv_callback);

    /* initialize LwIP tcp_sent callback function */
    tcp_sent(tpcb, _lwip_tcp_sent_callback); // FIXME do we actually need it?

    /* initialize LwIP tcp_err callback function */
    // tcp_err(tpcb, lwip_tcp_err_callback); // initialized before, because we may get error during connection

    // TODO understand if this could be helpful
    // tcp_poll(tpcb, NULL, 0);

    return err;
}

static err_t _lwip_tcp_sent_callback(void* arg, struct tcp_pcb* tpcb, u16_t len) {
    if(arg == NULL) {
        // Setup was not performed correctly and the arg was not setup properly
        // _lwip_tcp_connection_close(tpcb, tcp_arg);
        // this->stop(); // FIXME this doesn't exist

        return ERR_ARG;
    }

    lwipClient* client = (lwipClient*)arg;
}

// callback function that should be called when data has successfully been received (i.e., acknowledged)
// by the remote host. The len argument passed to the callback function gives the amount bytes that
// was acknowledged by the last acknowledgment.
void _lwip_tcp_err_callback(void *arg, err_t err) {
    if(arg == NULL) {
        // Setup was not performed correctly and the arg was not setup properly
        // _lwip_tcp_connection_close(tpcb, tcp_arg);
        // this->stop(); // FIXME this doesn't exist

        // return ERR_ARG;
        return;
    }

    lwipClient* client = (lwipClient*)arg;
    // TODO add a callback for tcp errors in lwipClient
}

err_t _lwip_tcp_recv_callback(void* arg, struct tcp_pcb* tpcb, struct pbuf* p, err_t err) {
    if(arg == NULL) {
        // Setup was not performed correctly and the arg was not setup properly
        // _lwip_tcp_connection_close(tpcb, tcp_arg);
        // this->stop(); // FIXME this doesn't exist

        return ERR_ARG;
    }

    lwipClient* client = (lwipClient*)arg;

    client->recv_callback(tpcb, p, err);
}

err_t lwipClient::recv_callback(struct tcp_pcb* tpcb, struct pbuf* p, err_t err) {
    err_t ret_err = ERR_OK;

    // FIXME this checks should be done on every callback
    if(err != ERR_OK) {
        this->stop();
        return err;
    }

    if (p == NULL) {
        // Remote host has closed the connection -> close from our side
        this->stop();

        return ERR_OK;
    }
    arduino::lock();
    if(this->tcp_info->state == TCP_CONNECTED || this->tcp_info->state == TCP_ACCEPTED) {
        if (this->tcp_info->pbuf_head == nullptr) {
            // no need to increment the references of the pbuf,
            // since it is already 1 and lwip shifts the control to this code
            this->tcp_info->pbuf_head = p;
        } else {
            // no need to increment the references of p, since it is already 1 and the only reference is this->tcp_info->pbuf_head->next
            pbuf_cat(this->tcp_info->pbuf_head, p);
        }

        ret_err = ERR_OK;
    }
    arduino::unlock();

    return ret_err;
}

size_t lwipClient::write(uint8_t b) {
    return write(&b, 1);
}

size_t lwipClient::write(const uint8_t* buffer, size_t size) {
    arduino::lock();

    uint8_t* buffer_cursor = (uint8_t*)buffer;
    uint8_t bytes_to_send = 0;

    do {
        bytes_to_send = min(size - (buffer - buffer_cursor), tcp_sndbuf(this->tcp_info->pcb));

        /*
         * TODO: Look into the following flags, especially for write of 1 byte
         * TCP_WRITE_FLAG_COPY (0x01) data will be copied into memory belonging to the stack
         * TCP_WRITE_FLAG_MORE (0x02) for TCP connection, PSH flag will not be set on last segment sent
         */
        err_t res = tcp_write(this->tcp_info->pcb, buffer_cursor, bytes_to_send, TCP_WRITE_FLAG_COPY);

        if(res == ERR_OK) {
            buffer_cursor += bytes_to_send;
        } else if(res == ERR_MEM) {
            // FIXME handle this: we get into this case only if the sent data cannot be put in the send queue
        }
    } while(buffer_cursor < buffer + size);

    tcp_output(this->tcp_info->pcb);

    arduino::unlock();
    return buffer - buffer_cursor;
}

int lwipClient::read() {
    uint8_t c = 0;

    int res = read(&c, 1);
    return res == 1 ? c : res;
}

int lwipClient::read(uint8_t* buffer, size_t size) {
    if(size==0 || buffer==nullptr || this->tcp_info->pbuf_head==nullptr) {
        return 0; // TODO extend checks
    }
    // copy data from the lwip buffer to the app provided buffer
    // TODO look into pbuf_get_contiguous(this->tcp_info->pbuf_head, buffer_cursor, len);
    // pbuf_get_contiguous: returns the pointer to the payload if size <= pbuf.len
    //      otherwise copies data in the user provided buffer. This can be used in a callback paradigm,
    //      in order to avoid memcpy data

    /*
     * a chain of pbuf is not granted to have a size multiple of size length
     * meaning that across different calls of this function a pbuf could be partially copied
     * we need to account that
     */
    arduino::lock();
    uint16_t copied = pbuf_copy_partial(this->tcp_info->pbuf_head, buffer, size, this->tcp_info->pbuf_offset);

    this->tcp_info->pbuf_head = free_pbuf_chain(this->tcp_info->pbuf_head, copied, &this->tcp_info->pbuf_offset);

    // acknowledge the received data
    tcp_recved(this->tcp_info->pcb, copied);
    arduino::unlock();

    return copied;
}

int lwipClient::peek() {
    uint8_t b;
    // Unlike recv, peek doesn't check to see if there's any data available, so we must
    if (!available()) {
        return -1;
    }

    arduino::lock();
    b = pbuf_get_at(this->tcp_info->pbuf_head, 0); // TODO test this
    arduino::unlock();

    return b;
}

void lwipClient::flush() {
    if ((this->tcp_info->pcb == NULL)) {
        return;
    }
    tcp_output(this->tcp_info->pcb);
}

void lwipClient::stop() {
    if(this->tcp_info->pcb != nullptr) {
        tcp_recv(this->tcp_info->pcb, nullptr);
        tcp_sent(this->tcp_info->pcb, nullptr);
        tcp_poll(this->tcp_info->pcb, nullptr, 0);
        tcp_err(this->tcp_info->pcb, nullptr);
        tcp_accept(this->tcp_info->pcb, nullptr);

        err_t err = tcp_close(this->tcp_info->pcb);
        this->tcp_info->state = TCP_CLOSING;

        this->tcp_info->pcb = nullptr;

        // FIXME if err != ERR_OK retry, there may be memory issues, retry?
    }

    // reset all the other variables in this class

    // if(tcp->p != nullptr) {
    //     pbuf_free(tcp->p); // FIXME it happens that a pbuf, with ref == 0 is added for some reason
    // }
    if(this->tcp_info->server != nullptr) {
        // need to first make the server point to nullptr, then remove the client, can cause infinite recursion
        auto server = this->tcp_info->server;
        this->tcp_info->server = nullptr;
        server->remove(this);
    }
}

uint8_t lwipClient::connected() {
    return this->tcp_info->state == TCP_CONNECTED || this->tcp_info->state == TCP_ACCEPTED;
}

uint8_t lwipClient::status() {
    if (this == nullptr) {
        return TCP_NONE;
    }
    return this->tcp_info->state;
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.

lwipClient::operator bool() {
    return (this->tcp_info->pcb != nullptr);
}

bool lwipClient::operator==(const lwipClient& rhs) {
    // return pcb == rhs.this && this->tcp_info->pcb == rhs.this->tcp_info->pcb;
    return this->tcp_info == rhs.tcp_info;
}

/* This function is not a function defined by Arduino. This is a function
specific to the W5100 architecture. To keep the compatibility we leave it and
returns always 0. */
uint8_t lwipClient::getSocketNumber() {
    return 0;
}

// This function is useful for protocol that provide sequence delimiter, like http,
// this allows the user to avoid using temporary buffers
size_t lwipClient::read_until_token(
    const uint8_t* buffer, uint16_t buffer_size, char* token, bool &found) {
    if(buffer_size==0 || buffer==nullptr || this->tcp_info->pbuf_head==nullptr) {
        return 0; // TODO extend checks
    }
    arduino::lock();
    // TODO check that the buffer size is less than the token len

    uint16_t offset=this->tcp_info->pbuf_offset;
    /* iterate over pbufs until:
    * - the first occurrence of token
    * - the provided buffer is full
    * - the available pbufs have been consumed
    */
    size_t tkn_len = strlen(token);

    // FIXME if we have already found the token we hare wasting time to check the entire buffer again
    uint16_t position = pbuf_memfind(this->tcp_info->pbuf_head, token, tkn_len, this->tcp_info->pbuf_offset); // TODO check efficiency of this function
    uint16_t buf_copy_len = buffer_size;

    // TODO triple check the indices of these conditions
    if(position != 0xffff && position + tkn_len <= buffer_size) { // TODO consider how to handle the case that the chain is long 0xffff
        // We found the token and it fits the user provided buffer
        buf_copy_len = position + tkn_len;
        found = true;
    } else if(position != 0xffff && position < buffer_size && position + tkn_len > buffer_size) {
        // if the token is found and fits partially with the user provided buffer
        buf_copy_len = position - 1; // copy without consuming the token
        found = false;
    } else {
        /*
         * we cover 2 cases here:
         * - we didn't find the token
         * - we found the token, but it doesn't fit the user provided buffer
         */
        found = false;
    }

    uint16_t copied = pbuf_copy_partial(this->tcp_info->pbuf_head, (uint8_t*)buffer, buf_copy_len, this->tcp_info->pbuf_offset);

    this->tcp_info->pbuf_head = free_pbuf_chain(this->tcp_info->pbuf_head, copied, &this->tcp_info->pbuf_offset);

    // acknowledge the received data
    tcp_recved(this->tcp_info->pcb, copied);
    arduino::unlock();

    return copied;
}
