#include "udp_helpers.h"




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
void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                          const ip_addr_t *addr, u16_t port)
{
  struct udp_struct *udp_arg = (struct udp_struct *)arg;

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