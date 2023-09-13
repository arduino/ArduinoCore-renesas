/* ########################################################################## */
/* - File: CEspCommunication.cpp
   - Copyright (c): 2023 Arduino srl.
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

#include "CEspCommunication.h"

std::queue<CMsg> CEspCom::to_ESP32_queue;
std::queue<CMsg> CEspCom::from_ESP32_queue;
std::queue<CMsg> CEspCom::rxStationQueue;
std::queue<CMsg> CEspCom::rxSoftApQueue;
/* -------------------------------------------------------------------------- */
bool CEspCom::send_msg_to_esp(CMsg &msg) {
/* -------------------------------------------------------------------------- */  
   if(msg.is_valid()) {
      CEspCom::to_ESP32_queue.push(std::move(msg));
      return true;
   }
   return false;
}

/* -------------------------------------------------------------------------- */
bool CEspCom::get_msg_from_esp(CMsg &msg) {
/* -------------------------------------------------------------------------- */  
   if(from_ESP32_queue.size() > 0) {
      msg = std::move(from_ESP32_queue.front());
      CEspCom::from_ESP32_queue.pop();
      return true;
   }
   return false;
}

/* -------------------------------------------------------------------------- */
void CEspCom::clearFromEspQueue() {
/* -------------------------------------------------------------------------- */   
   CEspCom::from_ESP32_queue = {};
}

/* -------------------------------------------------------------------------- */
bool CEspCom::send_msg_to_app(const uint8_t *buffer, uint16_t dim) {
/* -------------------------------------------------------------------------- */  
   CMsg msg;
   bool rv = false;
   if(msg.store_rx_buffer(buffer, dim)) {
      if(msg.is_valid()) {
         CEspCom::from_ESP32_queue.push(std::move(msg));
         rv = true;
      }
      else {
         
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
bool CEspCom::get_msg_from_app(uint8_t *buffer, uint16_t dim) {
/* -------------------------------------------------------------------------- */  
   bool rv = false;
   if(CEspCom::to_ESP32_queue.size() > 0) {
      CMsg msg = std::move(CEspCom::to_ESP32_queue.front());
      msg.read(buffer,dim);
      CEspCom::to_ESP32_queue.pop();
      rv = true;
   }
   else {
      memset(buffer,0x00,dim);
      rv = false;
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
void CEspCom::clearToEspQueue() {
/* -------------------------------------------------------------------------- */
   CEspCom::to_ESP32_queue = {};
}


/* -------------------------------------------------------------------------- */
bool CEspCom::storeStationMsg(CMsg &msg) {
/* -------------------------------------------------------------------------- */   
   if(msg.is_valid()) {
      CEspCom::rxStationQueue.push(std::move(msg));
      return true;
   }
   return false;

}

/* -------------------------------------------------------------------------- */
bool CEspCom::storeSoftApMsg(CMsg &msg) {
/* -------------------------------------------------------------------------- */   
   if(msg.is_valid()) {
      CEspCom::rxSoftApQueue.push(std::move(msg));
      return true;
   }
   return false;

}    

/* -------------------------------------------------------------------------- */
void CEspCom::clearStationRx()  {
/* -------------------------------------------------------------------------- */   
   CEspCom::rxStationQueue = {};
}

/* -------------------------------------------------------------------------- */
void CEspCom::clearSoftApRx() {
/* -------------------------------------------------------------------------- */   
   CEspCom::rxSoftApQueue = {};
}

/* -------------------------------------------------------------------------- */
bool CEspCom::getMsgForStation(CMsg &msg) {
/* -------------------------------------------------------------------------- */
   if(CEspCom::rxStationQueue.size() > 0) {
      msg = std::move(CEspCom::rxStationQueue.front());
      CEspCom::rxStationQueue.pop();
      return true;
   }
   return false;
}

/* -------------------------------------------------------------------------- */
int CEspCom::peekMsgSizeForStation() {
/* -------------------------------------------------------------------------- */
   if(CEspCom::rxStationQueue.size() > 0) {
      return CEspCom::rxStationQueue.front().get_size();
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
bool CEspCom::getMsgForSoftAp(CMsg &msg) {
/* -------------------------------------------------------------------------- */
   if(CEspCom::rxSoftApQueue.size() > 0) {
      msg = std::move(CEspCom::rxSoftApQueue.front());
      CEspCom::rxSoftApQueue.pop();
      return true;
   }
   return false;
}
