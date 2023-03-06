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



bool store_message_from_ESP32(uint8_t *buf, uint32_t size);
bool read_message_from_ESP32(uint8_t *buf, uint32_t size);

bool store_message_to_ESP32(uint8_t *buf, uint32_t size);
/* read a message from the queue and put it into the buffer, the message in
   the queue is automatically erased */
bool read_message_to_ESP32(uint8_t *buf, uint32_t size);




#endif