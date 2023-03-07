#include "esp_host_app.h"


#define MORE_FRAGMENT (1 << 0)

/* The message can be fragmented --> if the flags have the bit MORE_FRAGMENTED active */

static CMsg cumulative_msg;

void esp_host_msg_received() {
   CMsg msg;
   if(application_receive_msg_from_esp32(msg)) {
      if(msg.get_if_type() == ESP_SERIAL_IF) {
         /* control message received, please note that the msg is automatically
            cleared by the add_msg function (its pointers are stealed and all
            is nullified) */
         cumulative_msg.add_msg(msg);
         if(msg.get_flags() & MORE_FRAGMENT) {
            /* if FRAGMENT is active, wait for other fragments to complete the
               message*/
         }
         else {
            /* if no more FRAGMENT is active 
               first verify the tlv header */
            if(cumulative_msg.verify_tlv_header()) {
               /* here the */

            }
            else {
               /* something is wrong in the tlv header... delete it */
               cumulative_msg.clear();
            }
         }
      }
      else if(msg.get_if_type() == ESP_STA_IF || msg.get_if_type() == ESP_AP_IF) {
         /* net if message received */
      }
      else if(msg.get_if_type() == ESP_PRIV_IF) {

      }
      else if(msg.get_if_type() == ESP_TEST_IF) {
         
      }
   }


#ifdef TO_BE_REMOVED
   /* process received buffer for all possible interface types */
      if (buf_handle.if_type == ESP_SERIAL_IF) {

         /* serial interface path */
         serial_rx_handler(&buf_handle);

      } else if((buf_handle.if_type == ESP_STA_IF) ||
            (buf_handle.if_type == ESP_AP_IF)) {
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

      } else if (buf_handle.if_type == ESP_PRIV_IF) {
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
      } else if (buf_handle.if_type == ESP_TEST_IF) {
#if TEST_RAW_TP
         update_test_raw_tp_rx_len(buf_handle.payload_len);
#endif
      } else {
         printf("unknown type %d \n\r", buf_handle.if_type);
      }

      /* Free buffer handle */
      /* When buffer offloaded to other module, that module is
       * responsible for freeing buffer. In case not offloaded or
       * failed to offload, buffer should be freed here.
       */
      if (buf_handle.free_buf_handle) {
         buf_handle.free_buf_handle(buf_handle.priv_buffer_handle);
      }
   }
   #endif

}





int esp_host_app_init() {
   esp_host_spi_init();
   esp_host_set_cb_rx(esp_host_msg_received);
}
