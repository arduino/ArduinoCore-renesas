#ifndef ESP_HOST_PROTOCOL_H
#define ESP_HOST_PROTOCOL_H

//#define DEBUG_ESP_HOST_PROTOCOL



#include <stdbool.h>
#include <stdint.h>

#include "CMsg.h"




/** Exported Structures **/
/* interface header */
typedef struct {
   union {
      void *priv_buffer_handle;
   };
   uint8_t if_type;
   uint8_t if_num;
   uint8_t *payload;
   uint8_t flag;
   uint16_t payload_len;
   uint16_t seq_num;

   void (*free_buf_handle)(void *buf_handle);
} interface_buffer_handle_t;









#endif