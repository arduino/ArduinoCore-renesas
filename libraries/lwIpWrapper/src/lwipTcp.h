#ifndef _ARDUINO_LWIP_IF_TCP_HELPERS_H
#define _ARDUINO_LWIP_IF_TCP_HELPERS_H

#include "CNetIf.h"
#include "lwipTypes.h"

#if LWIP_TCP
err_t tcp_connected_callback(void* arg, struct tcp_pcb* tpcb, err_t err);
err_t tcp_accept_callback(void* arg, struct tcp_pcb* newpcb, err_t err);
void tcp_connection_close(struct tcp_pcb* tpcb, struct tcp_struct* tcp);

#else
#error "LWIP_TCP must be enabled in lwipopts.h"
#endif

#endif