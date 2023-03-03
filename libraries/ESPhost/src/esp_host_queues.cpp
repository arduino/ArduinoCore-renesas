/* ########################################################################## */
/* - File: esp_host_queues.cpp
   - Copyright (c): 2022 Arduino srl.
   - Author: Daniele Aimo (d.aimo@arduino.cc)

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc.,51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA */
/* ########################################################################## */

#include "esp_host_queues.h"


/* SHORT DESCRITION:
   This file contains the queues and the function used for the communication
   between the spi driver and the higher sw layer */

#define QUEUE_DIM 10



/* queue that contains "messages" to be sent to ESP32 via SPI */
static arduino::FifoBuffer<CMsg, QUEUE_DIM> to_ESP32_queue;
/* queue that contains "messages" received from ESP32 via SPI */
static arduino::FifoBuffer<CMsg, QUEUE_DIM> from_ESP32_queue;

bool store_message_from_ESP32(uint8_t *buf, uint32_t size) {
   CMsg m;
   if(m.store(buf,size)) {
      if(from_ESP32_queue.store(m)) {
         return true;
      }
   }
   return false;
}

bool read_message_from_ESP32(uint8_t *buf, uint32_t size) {
   bool read_ok = false;
   CMsg m = from_ESP32_queue.read(&read_ok);
   if(read_ok) {
      return m.read(buf,size);
   }
   return false;
}

bool store_message_to_ESP32(uint8_t *buf, uint32_t size) {
   CMsg m;
   if(m.store(buf,size)) {
      if(to_ESP32_queue.store(m)) {
         return true;
      }
   }
   return false;
}

bool read_message_to_ESP32(uint8_t *buf, uint32_t size) {
   bool read_ok = false;
   CMsg m = to_ESP32_queue.read(&read_ok);
   if(read_ok) {
      return m.read(buf,size);
   }
   return false;
}

CMsg get_raw_message_to_EPS32(bool *available) {
   return to_ESP32_queue.read(available);
}