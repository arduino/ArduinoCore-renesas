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

#include "CEspCbks.h"

/* -------------------------------------------------------------------------- */
CEspCbk::CEspCbk() {
/* -------------------------------------------------------------------------- */   
   for(int i = 0; i < TOTAL_TABLE_DIM; i++) {
      callbacks[i] = nullptr;
   }
}

/* -------------------------------------------------------------------------- */
/* GET INSTANCE SINGLETONE FUNCTION */
/* -------------------------------------------------------------------------- */
CEspCbk& CEspCbk::getInstance() {
/* -------------------------------------------------------------------------- */   
   static CEspCbk    instance;
   return instance;
}


/* -------------------------------------------------------------------------- */
int CEspCbk::get_response_index(int r) {
/* -------------------------------------------------------------------------- */   
   if(r >= CTRL_REQ_BASE && r < CTRL_REQ_MAX) {
      return r - CTRL_REQ_BASE;
   }
   else if(r >= CTRL_RESP_BASE && r < CTRL_RESP_MAX) {
      return r - CTRL_RESP_BASE;
   }
   else {
      return -1;
   }
}

/* -------------------------------------------------------------------------- */
int CEspCbk::get_event_index(int r) {
/* -------------------------------------------------------------------------- */   
   if(r >= CTRL_EVENT_BASE && r < CTRL_EVENT_MAX) {
      return (r - CTRL_EVENT_BASE) + RESPONSE_TABLE_DIM;
   }
   else {
      return -1;
   }
}

/* -------------------------------------------------------------------------- */
int CEspCbk::get_index(int r) {
/* -------------------------------------------------------------------------- */   
   int rv = get_event_index(r);

   if(rv == -1) {
      rv = get_response_index(r);
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
bool CEspCbk::setCallback(int id, EspCallback_f event_cb) {
/* -------------------------------------------------------------------------- */
   int i = get_index(id);
   if(i >= 0) {
      callbacks[i] = event_cb;
      if(event_cb != nullptr) {
         return true;
      }
   }
   return false;   
}

/* -------------------------------------------------------------------------- */
bool CEspCbk::resetCallback(int id) {
/* -------------------------------------------------------------------------- */   
   int i = get_index(id);
   if(i >= 0) {
      callbacks[i] = nullptr;
      return true;
   }
   return false; 
}

/* -------------------------------------------------------------------------- */
bool CEspCbk::callCallback(int id, CCtrlMsgWrapper *resp){
/* -------------------------------------------------------------------------- */   
   int i = get_index(id);
   if(i >= 0) {
      if(callbacks[i] != nullptr) {
         callbacks[i](resp);
         return true;
      }
   }
   return false; 
}

/* -------------------------------------------------------------------------- */
bool CEspCbk::isCallbackSet(int id) {
/* -------------------------------------------------------------------------- */   
   int i = get_index(id);
   if(i >= 0) {
      if(callbacks[i] != nullptr) {
         return true;
      }
   }
   return false; 
}