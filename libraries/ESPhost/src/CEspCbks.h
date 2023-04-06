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

   bool setCallback(int id, EspCallback_f event_cb);
   bool resetCallback(int id);
   bool callCallback(int id, CCtrlMsgWrapper *resp);
   bool isCallbackSet(int id);
};


#endif