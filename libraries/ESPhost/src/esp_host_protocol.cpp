#include "esp_host_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif



static inline uint16_t compute_checksum(uint8_t *buf, uint16_t len) {
   uint16_t checksum = 0;
   uint16_t i = 0;

   while(i < len) {
      checksum += buf[i];
      i++;
   }
   return checksum;
}



/* this function takes a buffer containing a message to be sent to ESP32 in src
   it checks if is a valid message and thef format the dest buffer with the
   actual content to be sent 
   it is supposed that src and dest have a proper dimension */

bool to_esp32_check_and_copy_message(uint8_t *src, uint8_t *dest, uint32_t dest_size) {
   bool rv = false;

   interface_buffer_handle_t *buf_handle = (interface_buffer_handle_t *)src;
   int len = buf_handle->payload_len;
   int max_payload = dest_size - sizeof(struct esp_payload_header);
   if(len > 0) {
      /* there is something to send */
      rv = true;
      /* memset to 0 the tx buffer */
      memset(dest,0x00, dest_size);
      /* format the dest buffer to be sent */
      struct  esp_payload_header *payload_header;
      uint8_t *payload = NULL;
      
      /* Form Tx header */
      payload_header = (struct esp_payload_header *) dest;
      payload = dest + sizeof(struct esp_payload_header);
      
      payload_header->len     = htole16(len);
      payload_header->offset  = htole16(sizeof(struct esp_payload_header));
      payload_header->if_type = buf_handle->if_type;
      payload_header->if_num  = buf_handle->if_num;
      memcpy(payload, buf_handle->payload, (len > max_payload) ? max_payload : len);
      payload_header->checksum = htole16(compute_checksum(payload,sizeof(struct esp_payload_header)+len));
   }

   return rv;
}


bool from_esp32_check_and_copy_message(uint8_t *src, uint32_t src_size) {
   
   struct  esp_payload_header *payload_header = (struct esp_payload_header *) src;
   uint16_t len = le16toh(payload_header->len);
   uint16_t offset = le16toh(payload_header->offset);
   
   /* verify if sizes are correct */
   if(len == 0 || len > src_size || offset != sizeof(struct esp_payload_header)) {
      return false;
   }
   
   /* verify checksum */
   uint16_t rx_checksum = le16toh(payload_header->checksum);
   /* reset checksum because it was 0 when it was calculated */
   payload_header->checksum = 0;
   uint16_t checksum = compute_checksum(src, len+offset);  

   if(checksum != rx_checksum) {
      return false;
   } 

   interface_buffer_handle_t buf_handle = {0};

   buf_handle.priv_buffer_handle = src;
   buf_handle.free_buf_handle    = NULL;  //TBV
   buf_handle.payload_len        = len;
   buf_handle.if_type            = payload_header->if_type;
   buf_handle.if_num             = payload_header->if_num;
   buf_handle.payload            = src + offset;
   buf_handle.seq_num            = le16toh(payload_header->seq_num);
   buf_handle.flag               = payload_header->flags;

   uint8_t *payload = buf_handle.payload;
   
   if (buf_handle.if_type == ESP_SERIAL_IF) {

         #ifdef TOBE_IMPLEMENTED
         serial_rx_handler(&buf_handle);
         #endif

   } 
   else if((buf_handle.if_type == ESP_STA_IF) || (buf_handle.if_type == ESP_AP_IF)) {
         
         #ifdef TOBE_IMPLEMENTED
         priv = get_priv(buf_handle.if_type, buf_handle.if_num);

         if (priv) {
            buffer = (struct pbuf *)malloc(sizeof(struct pbuf));
            assert(buffer);

            buffer->len = buf_handle.payload_len;
            buffer->payload = malloc(buf_handle.payload_len);
            assert(buffer->payload);

            memcpy(buffer->payload, buf_handle.payload,
                  buf_handle.payload_len);

            netdev_rx(priv->netdev, buffer);
         }
         #endif

   } else if (buf_handle.if_type == ESP_PRIV_IF) {
         
         #ifdef TOBE_IMPLEMENTED
         buffer = (struct pbuf *)malloc(sizeof(struct pbuf));
         assert(buffer);

         buffer->len = buf_handle.payload_len;
         buffer->payload = malloc(buf_handle.payload_len);
         assert(buffer->payload);

         memcpy(buffer->payload, buf_handle.payload,
               buf_handle.payload_len);

         process_priv_communication(buffer);
         /* priv transaction received */
         printf("Received INIT event\n\r");

         event = (struct esp_priv_event *) (payload);
         if (event->event_type == ESP_PRIV_EVENT_INIT) {
            /* halt spi transactions for some time,
             * this is one time delay, to give breathing
             * time to slave before spi trans start */
            stop_spi_transactions_for_msec(50000);
            if (spi_drv_evt_handler_fp) {
               spi_drv_evt_handler_fp(TRANSPORT_ACTIVE);
            }
         } else {
            /* User can re-use this type of transaction */
         }
         #endif
   } else if (buf_handle.if_type == ESP_TEST_IF) {
         #if TEST_RAW_TP
         update_test_raw_tp_rx_len(buf_handle.payload_len);
         #endif
   } else {
         //printf("unknown type %d \n\r", buf_handle.if_type);
   }

      #ifdef TOBE_IMPLEMENTED
      /* Free buffer handle */
      /* When buffer offloaded to other module, that module is
       * responsible for freeing buffer. In case not offloaded or
       * failed to offload, buffer should be freed here.
       */
      if (buf_handle.free_buf_handle) {
         buf_handle.free_buf_handle(buf_handle.priv_buffer_handle);
      }
      #endif

}


#ifdef __cplusplus
}
#endif