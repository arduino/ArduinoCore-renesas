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
      return true;
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