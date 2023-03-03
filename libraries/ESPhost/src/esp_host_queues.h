/* ########################################################################## */
/* - File: esp_host_queues.h
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


#ifndef ESP_HOST_QUEUES_H
#define ESP_HOST_QUEUES_H

#include "FifoBuffer.h"

/* The class CMsg is a mean to transfert "message" (i.e. buffers of uint8_t) from
   to the ESP32 
   The buffer is automatically deleted after a read 
   So that the "correct" use of this class is:
   1. use "store" to store a message in the class and put it into the queue 
   2. read and put the result in a different buffer  */

class CMsg {
private:
   uint8_t *buf;
   uint32_t dim;
   
   uint32_t allocate(uint32_t d) {
      buf = new uint8_t[d];
      if(buf != nullptr) {
         dim = d;
         return dim;
      }
      return 0;

   }
public:
   CMsg() : buf{nullptr}, dim{0} {}
   ~CMsg() { clear(); }
   CMsg(const CMsg& m) {
        buf = m.buf;
        dim = m.dim;
   }
   void operator=(const CMsg& m) {
        buf = m.buf;
        dim = m.dim;
    }
   void clear() {
      if(buf != nullptr) {
         delete []buf;
         buf = nullptr;
         dim = 0;
      }
   }
   /* copy the buffer into a new allocated buf */
   bool store(uint8_t *buffer, uint32_t d) {
      clear();
      if(allocate(d) == d) {
         memcpy(buf,buffer,d);
         return true;
      }
      return false;
   }
   uint8_t *get_buffer() {return buf;}
   uint32_t get_size() {return dim;}
   bool read(uint8_t *dest, uint32_t d) {
      if(d >= dim) {
         memcpy(dest, buf, dim);
         clear();
         return true;
      }
      return false;
   }

};

bool store_message_from_ESP32(uint8_t *buf, uint32_t size);
bool read_message_from_ESP32(uint8_t *buf, uint32_t size);

bool store_message_to_ESP32(uint8_t *buf, uint32_t size);
/* read a message from the queue and put it into the buffer, the message in
   the queue is automatically erased */
bool read_message_to_ESP32(uint8_t *buf, uint32_t size);
/* get a message, but it does not delete it, available tells if the message
   is actually present it the queue  */
CMsg get_raw_message_to_EPS32(bool *available);



#endif