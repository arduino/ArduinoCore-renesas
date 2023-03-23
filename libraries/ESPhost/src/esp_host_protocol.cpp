#include "esp_host_protocol.h"
#include <queue>

using namespace std;

queue<CMsg> to_ESP32_queue;
queue<CMsg> from_ESP32_queue;


void esp_host_send_msg_to_esp32(CMsg &msg) {
   to_ESP32_queue.push(std::move(msg));
}

bool esp_host_get_msg_from_esp32(CMsg &msg) {
   if(from_ESP32_queue.size() > 0) {
      msg = std::move(from_ESP32_queue.front());
      from_ESP32_queue.pop();
      return true;
   }
   return false;
}

bool esp_host_esp32_send_to_app(const uint8_t *buffer, uint16_t dim) {
   CMsg msg;
   bool rv = false;
   if(msg.store_rx_buffer(buffer, dim)) {
      from_ESP32_queue.push(std::move(msg));
      rv = true;
   }
   return rv;
}

bool esp_host_esp32_get_msg_from_app(uint8_t *buffer, uint16_t dim) {
   bool rv = false;
   if(to_ESP32_queue.size() > 0) {
      to_ESP32_queue.front().read(buffer,dim);
      to_ESP32_queue.pop();
      rv = true;
   }
   else {
      memset(buffer,0x00,dim);
      rv = false;
   }
   return rv;
}












