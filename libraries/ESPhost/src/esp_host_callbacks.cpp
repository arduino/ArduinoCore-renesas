#include "esp_host_callbacks.h"

/*
 * ----- RESPONSE CALLBACKS TABLE (former ctrl_resp_cb_table)
 */
#define RESPONSE_TABLE_DIM                  (CTRL_RESP_MAX - CTRL_RESP_BASE) 

static ctrl_resp_cb_t esp_host_response_table [RESPONSE_TABLE_DIM] = { nullptr };

/* PAY ATTENTION: 
   there are different ID for REQUESTS (i.e. msg sent to ESP32) and RESPONSE 
   (i.e. msg received from ESP32 as answer to a request) 
   In the original code callbacks were manipulated both via request id AND 
   response id --> difficult to read and understand 
   the following function "calculate" the correct index both in case of 
   REQUEST ID or RESPONSE ID
   */
static int get_reponse_index(int r) {
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

/* 
 * SET RESPONSE callback  
 */
int esp_host_set_response_cb(int id, ctrl_resp_cb_t resp_cb) {
   int index = get_reponse_index(id); 
   if(index >= 0) {
      esp_host_response_table[index] = resp_cb;
      return CALLBACK_SET_SUCCESS;
   }
   else {
      return MSG_ID_OUT_OF_ORDER;
   }
}

/* 
 * IS RESPONSE callback SET ? 
 */
int esp_host_is_response_cb_set(int id) {
   int index = get_reponse_index(id); 
   if(index >= 0) {
      if(esp_host_response_table[index] != nullptr) {
         return CALLBACK_AVAILABLE;
      }
      else {
         return CALLBACK_NOT_REGISTERED;
      } 
   }
   else {
      return MSG_ID_OUT_OF_ORDER;
   }
}

/* 
 * GET RESPONSE callback  
 */
ctrl_resp_cb_t esp_host_get_response_cb(int id) {
   int index = get_reponse_index(id); 
   if(index >= 0) {
      return esp_host_response_table[index];
   }
   return nullptr;
}

/* 
 * CALL RESPONSE callback  
 */
int esp_host_call_response_cb(int id, ctrl_cmd_t *app_resp) {
   int index = get_reponse_index(id); 
   if(index >= 0) {
      if(esp_host_response_table[index] != nullptr) {
         return esp_host_response_table[index](app_resp);
      }
      else {
         return CALLBACK_NOT_REGISTERED;
      } 
   }
   else {
      return MSG_ID_OUT_OF_ORDER;
   }
}

/*
 * RESET response callback
 */
int esp_host_reset_response_cb(int id) {
   int index = get_reponse_index(id); 
   if(index >= 0) {
      esp_host_response_table[index] = nullptr;
      return CALLBACK_SET_SUCCESS;
   }
   else {
      return MSG_ID_OUT_OF_ORDER;
   }
}



/*
 * ----- EVENT CODE CALLBACKS TABLE (former ctrl_event_cb_table)
 */
#define EVENT_TABLE_DIM (CTRL_EVENT_MAX - CTRL_EVENT_BASE) 

static ctrl_event_cb_t esp_host_event_table[EVENT_TABLE_DIM] = { NULL };

static int get_event_index(int r) {
   if(r >= CTRL_EVENT_BASE && r < CTRL_EVENT_MAX) {
      return r - CTRL_EVENT_BASE;
   }
   else {
      return -1;
   }
}

/* 
 * SET RESPONSE callback  
 */
int esp_host_set_event_cb(int id, ctrl_event_cb_t event_cb) {
   int index = get_event_index(id); 
   if(index >= 0) {
      esp_host_event_table[index] = event_cb;
      return CALLBACK_SET_SUCCESS;
   }
   else {
      return MSG_ID_OUT_OF_ORDER;
   }
}

/* 
 * IS RESPONSE callback SET ? 
 */
int esp_host_is_event_cb_set(int id) {
   int index = get_event_index(id); 
   if(index >= 0) {
      if(esp_host_event_table[index] != nullptr) {
         return CALLBACK_AVAILABLE;
      }
      else {
         return CALLBACK_NOT_REGISTERED;
      } 
   }
   else {
      return MSG_ID_OUT_OF_ORDER;
   }
}

/* 
 * GET RESPONSE callback  
 */
ctrl_event_cb_t esp_host_get_event_cb(int id) {
   int index = get_event_index(id); 
   if(index >= 0) {
      return esp_host_event_table[index];
   }
   return nullptr;
}

/* 
 * CALL RESPONSE callback  
 */
int esp_host_call_event_cb(int id, ctrl_cmd_t *app_resp) {
   int index = get_event_index(id); 
   if(index >= 0) {
      if(esp_host_event_table[index] != nullptr) {
         return esp_host_event_table[index](app_resp);
      }
      else {
         return CALLBACK_NOT_REGISTERED;
      } 
   }
   else {
      return MSG_ID_OUT_OF_ORDER;
   }
}

/*
 * RESET event callback
 */
int esp_host_reset_event_cb(int id) {
   int index = get_event_index(id); 
   if(index >= 0) {
      esp_host_event_table[index] = nullptr;
      return CALLBACK_SET_SUCCESS;
   }
   else {
      return MSG_ID_OUT_OF_ORDER;
   }
}


















