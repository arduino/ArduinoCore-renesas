#include "esp_host_protocol.h"



#define QUEUE_DIM 10

/* queue that contains "messages" to be sent to ESP32 via SPI */
static arduino::FifoBuffer<CMsg, QUEUE_DIM> to_ESP32_queue;
/* queue that contains "messages" received from ESP32 via SPI */
static arduino::FifoBuffer<CMsg, QUEUE_DIM> from_ESP32_queue;


/* 
 * This function has to be called just after the PROTOBUF part of the message
 * has been copied into the msg
 * this function will set TLV structure, esp payload header structure and send
 * the message in to the queue for the ESP32 (SPI)
 */

bool application_send_msg_to_esp32(CMsg &msg, const char *ep_name, uint8_t if_type, uint8_t if_num) {
   msg.set_tlv_header(ep_name);
   msg.set_payload_header(if_type, if_num);
   return to_ESP32_queue.store(msg);
}

/* this function has to be called by SPI driver to get a buffer to be sent via SPI  
   buffer is the destination buffer */

bool esp32_receive_msg_to_be_sent_on_SPI(uint8_t *buffer, uint16_t dim) {
   bool read_ok = false;
   CMsg msg = to_ESP32_queue.read(&read_ok);
   if(read_ok) {
      /* read copy into buffer and delete the buffer in the msg !*/
      memset(buffer,0x00,dim);
      return msg.read(buffer,dim);
   }
   return false;
}


bool esp32_send_msg_to_application(uint8_t *buffer, uint16_t dim) {
   CMsg msg;
   if(msg.store_rx_buffer(buffer, dim)) {
      return from_ESP32_queue.store(msg);
   }
   return false;
}

bool application_receive_msg_from_esp32(CMsg &msg) {
   bool read_ok = false;
   msg = from_ESP32_queue.read(&read_ok);
   
   return read_ok;

}





