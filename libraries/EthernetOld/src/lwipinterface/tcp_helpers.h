#ifndef _ARDUINO_RENESAS_TCP_HELPERS_H
#define _ARDUINO_RENESAS_TCP_HELPERS_H


#include "lwipInterface.h"
/*
#include "../lwipopts.h"
#include "lwip/include/lwip/ip_addr.h"
#include "lwip/include/lwip/dhcp.h"
#include "lwip/include/lwip/udp.h"
#include "lwip/include/lwip/tcp.h"
#include "lwip/include/lwip/opt.h"
#include "lwip/include/lwip/pbuf.h"
*/
#include "udp_tcp_data_types.h"


#if LWIP_TCP
  err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
  err_t tcp_accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err);
  void tcp_connection_close(struct tcp_pcb *tpcb, struct tcp_struct *tcp);

#else
  #error "LWIP_TCP must be enabled in lwipopts.h"
#endif

#endif