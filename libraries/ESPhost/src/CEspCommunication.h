/* ########################################################################## */
/* - File: CEspCommunication.h
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

/* SHORT DESCRIPTION:
 * This class manage the communication between the SPI driver and the application
 * layer using 2 queues, one that goes from application to the SPI driver and
 * the other that works in the opposite way
 * this class is implemented as static class */

#ifndef _ARDUINO_ESP_COMMUNICATION_CLASS_H
#define _ARDUINO_ESP_COMMUNICATION_CLASS_H

#include <queue>
#include "CMsg.h"

class CEspCom {
private:
   static std::queue<CMsg> to_ESP32_queue;
   static std::queue<CMsg> from_ESP32_queue;
   static std::queue<CMsg> rxStationQueue;
   static std::queue<CMsg> rxSoftApQueue;
public:
   /* application layer call this function when wants to send a message to esp*/
   static bool send_msg_to_esp(CMsg &msg);
   /* application layer call this function when wants to know if a message 
      from esp is available (function returns true in that case and false 
      otherwise) */
   static bool get_msg_from_esp(CMsg &msg);
   /* spi driver call this function when it wants to send a received message
      to the application layer, since the esp can send also 'dummy' or wrong
      message the function controls the payload header and put the 
      message in the queue only if correct (return true in that case false 
      otherwise)*/ 
   static bool send_msg_to_app(const uint8_t *buffer, uint16_t dim);
   /* spi driver calls this function when it wants to know if there are message
      to be sent to esp 
      the function returns true if there are messages to be sent and false
      otherwise
      it the latter case the buffer is memset to 0 (dummy message to be used
      if there is something to be received from esp and nothing to be sent to)*/
   static bool get_msg_from_app(uint8_t *buffer, uint16_t dim);

   static bool storeStationMsg(CMsg &msg);
   static bool storeSoftApMsg(CMsg &msg); 
   static bool getMsgForStation(CMsg &msg);
   static int  peekMsgSizeForStation();
   static bool getMsgForSoftAp(CMsg &msg);

   static void clearStationRx();
   static void clearSoftApRx();
   static void clearFromEspQueue();
   static void clearToEspQueue();

};


#endif