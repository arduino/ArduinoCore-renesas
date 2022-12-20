#ifndef _ARDUINO_RENESAS_UDP_HELPERS_H
#define _ARDUINO_RENESAS_UDP_HELPERS_H


#include "lwip/include/lwip/ip_addr.h"
#include "lwip/include/lwip/dhcp.h"
#include "lwip/include/lwip/udp.h"
#include "lwip/include/lwip/tcp.h"
#include "lwip/include/lwip/opt.h"
#include "lwip/include/lwip/pbuf.h"
#include "udp_tcp_data_types.h"

void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                          const ip_addr_t *addr, u16_t port);

#endif