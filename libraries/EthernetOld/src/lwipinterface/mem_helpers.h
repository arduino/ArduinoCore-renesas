#ifndef _ARDUINO_RENESAS_MEM_HELPERS_H
#define _ARDUINO_RENESAS_MEM_HELPERS_H


#include "udp_tcp_data_types.h"


struct pbuf *pbuffer_put_data(struct pbuf *p, const uint8_t *buffer, size_t size);
struct pbuf *pbuffer_free_data(struct pbuf *p);
uint16_t pbuffer_get_data(struct pbuf_data *data, uint8_t *buffer, size_t size);


#endif