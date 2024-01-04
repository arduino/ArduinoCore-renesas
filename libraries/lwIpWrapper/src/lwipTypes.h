#ifndef ARDUINO_LWIP_IF_TYPES_H
#define ARDUINO_LWIP_IF_TYPES_H

#include "lwip/include/lwip/ip_addr.h"
#include "lwip/include/lwip/pbuf.h"
#include <functional>

/* Exported types ------------------------------------------------------------*/
/* Struct to store received data */
struct pbuf_data {
    struct pbuf* p; // the packet buffer that was received
    uint16_t available; // number of data
};

/* UDP structure */
struct udp_struct {
    struct udp_pcb* pcb; /* pointer on the current udp_pcb */
    struct pbuf_data data;
    ip_addr_t ip; // the remote IP address from which the packet was received
    u16_t port; // the remote port from which the packet was received
    std::function<void()> onDataArrival;
};

#endif
