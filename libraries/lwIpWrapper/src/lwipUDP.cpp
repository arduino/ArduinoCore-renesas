
#include "Udp.h"

#include "lwipUDP.h"

#include "lwip/include/lwip/igmp.h"
#include "lwip/include/lwip/ip_addr.h"

#if LWIP_UDP
/**
 * @brief This function is called when an UDP datagram has been received on
 * the port UDP_PORT.
 * @param arg user supplied argument
 * @param pcb the udp_pcb which received data
 * @param p the packet buffer that was received
 * @param addr the remote IP address from which the packet was received
 * @param port the remote port from which the packet was received
 * @retval None
 */
void udp_receive_callback(void* arg, struct udp_pcb* pcb, struct pbuf* p,
    const ip_addr_t* addr, u16_t port)
{
    struct udp_struct* udp_arg = (struct udp_struct*)arg;

    /* Send data to the application layer */
    if ((udp_arg != NULL) && (udp_arg->pcb == pcb)) {
        // Free the old p buffer if not read
        if (udp_arg->data.p != NULL) {
            pbuf_free(udp_arg->data.p);
        }

        udp_arg->data.p = p;
        udp_arg->data.available = p->len;

        ip_addr_copy(udp_arg->ip, *addr);
        udp_arg->port = port;

        if (udp_arg->onDataArrival != NULL) {
            udp_arg->onDataArrival();
        }
    } else {
        pbuf_free(p);
    }
}

#endif /* LWIP_UDP */

/* Constructor */
lwipUDP::lwipUDP() { }

/* Start lwipUDP socket, listening at local IP ip and port PORT */
uint8_t lwipUDP::begin(IPAddress ip, uint16_t port, bool multicast)
{
    // Can create a single udp connection per socket
    if (_udp.pcb != NULL) {
        return 0;
    }

    _udp.pcb = udp_new();

    if (_udp.pcb == NULL) {
        return 0;
    }

    ip_addr_t ipaddr;
    err_t err;
    u8_to_ip_addr(rawIPAddress(ip), &ipaddr);
    if (multicast) {
        err = udp_bind(_udp.pcb, IP_ADDR_ANY, port);
    } else {
        err = udp_bind(_udp.pcb, &ipaddr, port);
    }

    if (ERR_OK != err) {
        stop();
        return 0;
    }

#if LWIP_IGMP
    if ((multicast) && (ERR_OK != igmp_joingroup(IP_ADDR_ANY, &ipaddr))) {
        return 0;
    }
#endif
    udp_recv(_udp.pcb, &udp_receive_callback, &_udp);

    _port = port;
    _remaining = 0;

    CLwipIf::getInstance().lwip_task();

    return 1;
}

/* return number of bytes available in the current packet,
   will return zero if parsePacket hasn't been called yet */
int lwipUDP::available()
{
    return _remaining;
}

/* Release any resources being used by this lwipUDP instance */
void lwipUDP::stop()
{
    if (_udp.pcb != NULL) {
        udp_disconnect(_udp.pcb);
        udp_remove(_udp.pcb);
        _udp.pcb = NULL;
    }

    CLwipIf::getInstance().lwip_task();
}

int lwipUDP::beginPacket(const char* host, uint16_t port)
{
    // Look up the host first
    int ret = 0;
    // DNSClient dns;
    IPAddress remote_addr;
    ret = CLwipIf::getInstance().getHostByName(host, remote_addr);

    if (ret == 1) {
        return beginPacket(remote_addr, port);
    } else {
        return ret;
    }
}

int lwipUDP::beginPacket(IPAddress ip, uint16_t port)
{
    if (_udp.pcb == NULL) {
        return 0;
    }

    _sendtoIP = ip;
    _sendtoPort = port;

    udp_recv(_udp.pcb, &udp_receive_callback, &_udp);
    CLwipIf::getInstance().lwip_task();

    return 1;
}

int lwipUDP::endPacket()
{
    if ((_udp.pcb == NULL) || (_data == NULL)) {
        return 0;
    }

    ip_addr_t ipaddr;
    if (ERR_OK != udp_sendto(_udp.pcb, _data, u8_to_ip_addr(rawIPAddress(_sendtoIP), &ipaddr), _sendtoPort)) {
        __disable_irq();
        _data = pbuffer_free_data(_data);
        __enable_irq();
        return 0;
    }

    _data = NULL;

    CLwipIf::getInstance().lwip_task();

    return 1;
}

size_t lwipUDP::write(uint8_t byte)
{
    return write(&byte, 1);
}

size_t lwipUDP::write(const uint8_t* buffer, size_t size)
{
    __disable_irq();
    _data = pbuffer_put_data(_data, buffer, size);
    __enable_irq();
    if (_data == NULL) {
        return 0;
    }

    return size;
}

int lwipUDP::parsePacket()
{
    // discard any remaining bytes in the last packet
    // while (_remaining) {
    //   // could this fail (loop endlessly) if _remaining > 0 and recv in read fails?
    //   // should only occur if recv fails after telling us the data is there, lets
    //   // hope the w5100 always behaves :)
    //   read();
    // }

    CLwipIf::getInstance().lwip_task();

    if (_udp.data.available > 0) {
        _remoteIP = IPAddress(ip_addr_to_u32(&(_udp.ip)));
        _remotePort = _udp.port;
        _remaining = _udp.data.available;

        return _remaining;
    }
    // There aren't any packets available
    return 0;
}

int lwipUDP::read()
{
    uint8_t byte;

    if (_udp.data.p == NULL) {
        return -1;
    }

    if (_remaining > 0) {
        if (read(&byte, 1) > 0) {
            return byte;
        }
    }

    // If we get here, there's no data available
    return -1;
}

int lwipUDP::read(unsigned char* buffer, size_t len)
{
    if (_udp.data.p == NULL) {
        return -1;
    }

    if (_remaining > 0) {
        int got;

        if (_remaining <= len) {
            // data should fit in the buffer
            __disable_irq();
            got = (int)pbuffer_get_data(&(_udp.data), (uint8_t*)buffer, _remaining);
            __enable_irq();
        } else {
            // too much data for the buffer,
            // grab as much as will fit
            __disable_irq();
            got = (int)pbuffer_get_data(&(_udp.data), (uint8_t*)buffer, len);
            __enable_irq();
        }

        if (got > 0) {
            _remaining -= got;
            return got;
        }
    }

    // If we get here, there's no data available or recv failed
    return -1;
}

int lwipUDP::peek()
{
    uint8_t b;
    // Unlike recv, peek doesn't check to see if there's any data available, so we must.
    // If the user hasn't called parsePacket yet then return nothing otherwise they
    // may get the UDP header
    if (!_remaining) {
        return -1;
    }
    b = pbuf_get_at(_udp.data.p, 0);
    return b;
}

void lwipUDP::flush()
{
    // TODO: we should wait for TX buffer to be emptied
}

/* Start lwipUDP socket, listening at local port PORT */
uint8_t lwipUDP::beginMulticast(IPAddress ip, uint16_t port)
{
    return begin(ip, port, true);
}

#if LWIP_UDP
void lwipUDP::onDataArrival(std::function<void()> onDataArrival_fn)
{
    _udp.onDataArrival = onDataArrival_fn;
}
#endif
