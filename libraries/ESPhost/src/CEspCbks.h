/* ########################################################################## */
/* - File: CEspCbks.h
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

/* This class manage the callbacks that can be set up for the different requests
   the host can make to ESP32, plus the events (that are sort of special 
   messages the ESP32 spontaneously send to the HOST) 
   While the use of callbacks is optional in case of synchronous request (i.e. 
   the host makes a request and wait for the answer), the callback are compulsory
   for EVENTS */


#ifndef ARDUINO_HOST_ESP_CALLBACKS
#define ARDUINO_HOST_ESP_CALLBACKS



#include "CCtrlWrapper.h"

#define RESPONSE_TABLE_DIM                  (CTRL_RESP_MAX - CTRL_RESP_BASE) 
#define EVENT_TABLE_DIM                     (CTRL_EVENT_MAX - CTRL_EVENT_BASE) 
#define TOTAL_TABLE_DIM                     RESPONSE_TABLE_DIM + EVENT_TABLE_DIM


using EspCallback_f    = int (*)(CCtrlMsgWrapper *resp);




class CEspCbk {
private:
   CEspCbk();
   EspCallback_f callbacks[TOTAL_TABLE_DIM];
   int get_response_index(int r);
   int get_event_index(int r);
   int get_index(int r);

public:
   static CEspCbk& getInstance();
   CEspCbk(CEspCbk const&)               = delete;
   void operator=(CEspCbk const&)        = delete;
   ~CEspCbk() {}

   /* setCallback returns true if the callback is set and is different from nullptr
      but if it is nullptr will in any case reset the reset the cb */
   bool setCallback(int id, EspCallback_f event_cb);
   bool resetCallback(int id);
   bool callCallback(int id, CCtrlMsgWrapper *resp);
   bool isCallbackSet(int id);
};


#endif