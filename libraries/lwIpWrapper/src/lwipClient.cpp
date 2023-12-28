extern "C" {
#include "string.h"
}

#include "Arduino.h"

#include "lwipClient.h"

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
    : pcb(NULL) {
}

/* Deprecated constructor. Keeps compatibility with W5100 architecture
sketches but sock is ignored. */
lwipClient::lwipClient(uint8_t sock)
    : pcb(NULL) {
}

lwipClient::lwipClient(struct tcp_pcb* pcb)
: pcb(pcb) {
}

lwipClient::~lwipClient() {
    this->stop();
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
    this->pcb = tcp_new();

    if(this->pcb == nullptr) {
        // return ; // TODO find the proper error code
        return err;
    }

    tcp_err(this->pcb, _lwip_tcp_err_callback); // FIXME make this a user callback?
    if(err != ERR_OK) {
        return err;
    }

    this->state = TCP_NONE;

    tcp_arg(this->pcb, this);

    this->_ip = fromArduinoIP(ip);

    // FIXME this doesn't include timeout of connection, does lwip have it by default?
    err = tcp_connect(
        this->pcb, &this->_ip, port, // FIXME check if _ip gets copied
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

    this->state = TCP_CONNECTED;

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
    if(this->state == TCP_CONNECTED) {
        if (this->pbuf_head == nullptr) {
            // no need to increment the references of the pbuf,
            // since it is already 1 and lwip shifts the control to this code
            this->pbuf_head = p;
        } else {
            // no need to increment the references of p, since it is already 1 and the only reference is this->pbuf_head->next
            pbuf_cat(this->pbuf_head, p);
        }

        ret_err = ERR_OK;
    }
    arduino::unlock();

    return ret_err;
}

size_t lwipClient::write(uint8_t b) {
    return write(&b, 1);
}

size_t lwipClient::write(const uint8_t* buf, size_t size) {
    arduino::lock();

    uint8_t* buffer_cursor = (uint8_t*)buffer;
    uint8_t bytes_to_send = 0;

    do {
        bytes_to_send = min(size - (buffer - buffer_cursor), tcp_sndbuf(this->pcb));

        /*
         * TODO: Look into the following flags, especially for write of 1 byte
         * TCP_WRITE_FLAG_COPY (0x01) data will be copied into memory belonging to the stack
         * TCP_WRITE_FLAG_MORE (0x02) for TCP connection, PSH flag will not be set on last segment sent
         */
        err_t res = tcp_write(this->pcb, buffer_cursor, bytes_to_send, TCP_WRITE_FLAG_COPY);

        if(res == ERR_OK) {
            buffer_cursor += bytes_to_send;
        } else if(res == ERR_MEM) {
            // FIXME handle this: we get into this case only if the sent data cannot be put in the send queue
        }

        // TODO understand if the tcp_write will send data if the buffer is not full
        // force send only if we filled the send buffer
        // if (ERR_OK != tcp_output(this->pcb)) {
        //     // return 0;
        //     break;
        // }
    } while(buffer_cursor < buffer + size);
    arduino::unlock();

    return buffer - buffer_cursor;
}

int lwipClient::read() {
    uint8_t c = 0;

    int res = read(&c, 1);
    return res == 1 ? c : res;
}

int lwipClient::read(uint8_t* buf, size_t size) {
    if(buffer_size==0 || buffer==nullptr || this->pbuf_head==nullptr) {
        return 0; // TODO extend checks
    }
    // copy data from the lwip buffer to the app provided buffer
    // TODO look into pbuf_get_contiguous(this->pbuf_head, buffer_cursor, len);
    // pbuf_get_contiguous: returns the pointer to the payload if buffer_size <= pbuf.len
    //      otherwise copies data in the user provided buffer. This can be used in a callback paradigm,
    //      in order to avoid memcpy data

    /*
     * a chain of pbuf is not granted to have a size multiple of buffer_size length
     * meaning that across different calls of this function a pbuf could be partially copied
     * we need to account that
     */
    arduino::lock();
    uint16_t copied = pbuf_copy_partial(this->pbuf_head, buffer, buffer_size, this->pbuf_offset);

    this->free_pbuf_chain(copied);
    // __enable_irq();
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
    b = pbuf_get_at(this->pbuf_head, 0); // TODO test this
    arduino::unlock();

    return b;
}

void lwipClient::flush() {
    if ((this->pcb == NULL)) {
        return;
    }
    tcp_output(this->pcb);
}

void lwipClient::stop() {
    tcp_recv(this->pcb, nullptr);
    tcp_sent(this->pcb, nullptr);
    tcp_poll(this->pcb, nullptr, 0);
    tcp_err(this->pcb, nullptr);
    tcp_accept(this->pcb, nullptr);

    if(this->pcb != nullptr) {
        err_t err = tcp_close(this->pcb);
        this->state = TCP_CLOSING;

        this->pcb = nullptr;

        // FIXME if err != ERR_OK retry, there may be memory issues, retry?
    }

    // reset all the other variables in this class

    // if(tcp->p != nullptr) {
    //     pbuf_free(tcp->p); // FIXME it happens that a pbuf, with ref == 0 is added for some reason
    // }
}

uint8_t lwipClient::connected() {
    return this->state != TCP_NONE; //TODO
}

uint8_t lwipClient::status() {
    if (this == NULL) {
        return TCP_NONE;
    }
    return this->state;
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.

lwipClient::operator bool() {
    return (this->pcb != nullptr);
}

bool lwipClient::operator==(const lwipClient& rhs) {
    // return pcb == rhs.this && this->pcb == rhs.this->pcb;
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
    if(buffer_size==0 || buffer==nullptr || this->pbuf_head==nullptr) {
        return 0; // TODO extend checks
    }
    arduino::lock();
    // TODO check that the buffer size is less than the token len

    uint16_t offset=this->pbuf_offset;
    /* iterate over pbufs until:
    * - the first occurrence of token
    * - the provided buffer is full
    * - the available pbufs have been consumed
    */
    size_t tkn_len = strlen(token);

    // FIXME if we have already found the token we hare wasting time to check the entire buffer again
    uint16_t position = pbuf_memfind(this->pbuf_head, token, tkn_len, this->pbuf_offset); // TODO check efficiency of this function
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

    uint16_t copied = pbuf_copy_partial(this->pbuf_head, (uint8_t*)buffer, buf_copy_len, this->pbuf_offset);

    this->free_pbuf_chain(copied);
    arduino::unlock();

    return copied;
}

void lwipClient::free_pbuf_chain(uint16_t copied) {
    arduino::lock();
    /*
     * free pbufs that have been copied, if copied == 0 we have an error
     * free the buffer chain starting from the head up to the last entire pbuf ingested
     * taking into account the previously not entirely consumed pbuf
     */
    uint32_t tobefreed = 0;
    copied += this->pbuf_offset;

    // in order to clean up the chain we need to find the pbuf in the last pbuf in the chain
    // that got completely consumed by the application, dechain it from it successor and delete the chain before it

    struct pbuf *head = this->pbuf_head, *last=head, *prev=nullptr; // FIXME little optimization prev can be substituted by last->next

    while(last!=nullptr && last->len + tobefreed <= copied) {
        tobefreed += last->len;
        prev = last;
        last = last->next;
    }

    // dechain if we are not at the end of the chain (last == nullptr)
    // and if we haven't copied entirely the first pbuf (prev == nullptr) (head == last)
    // if we reached the end of the chain set the this pbuf pointer to nullptr
    if(prev != nullptr && last != nullptr) {
        prev->next = nullptr;
        this->pbuf_head = last;
    } if(last == nullptr) {
        this->pbuf_head = nullptr;
    }

    // the chain that is referenced by head is detached by the one referenced by this->pbuf_head
    // free the chain if we haven't copied entirely the first pbuf (prev == nullptr)
    if(this->pbuf_head != head) {
        uint8_t refs = pbuf_free(head);
    }

    this->pbuf_offset = copied - tobefreed; // This offset should be referenced to the first pbuf in queue

    // acknowledge the received data
    tcp_recved(this->pcb, copied);
    arduino::unlock();
}
