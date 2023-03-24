/* ########################################################################## */
/* - File: CEspControl.cpp
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

#include "CEspControl.h"

#include "esp_host_data_structures.h"

extern void esp_host_send_msg_to_esp32(CMsg &msg);
extern bool esp_host_esp32_get_msg_from_app(uint8_t *buffer, uint16_t dim);
extern bool esp_host_esp32_send_to_app(const uint8_t *buffer, uint16_t dim);
extern bool esp_host_get_msg_from_esp32(CMsg &msg);
extern int esp_host_perform_spi_communication();

extern int esp_host_spi_init(void);
extern int esp_host_get_msgs_received(CtrlMsg **response);

/* -------------------------------------------------------------------------- */
/* GET INSTANCE SINGLETONE FUNCTION */
/* -------------------------------------------------------------------------- */
CEspControl& CEspControl::getInstance() {
/* -------------------------------------------------------------------------- */   
   static CEspControl    instance;
   return instance;
}

/* -------------------------------------------------------------------------- */
/* PRIVATE CONSTRUCTOR*/
/* -------------------------------------------------------------------------- */
CEspControl::CEspControl() {
/* -------------------------------------------------------------------------- */
   esp_host_spi_init();
}
/* -------------------------------------------------------------------------- */
/* DESTRUCTOR */
/* -------------------------------------------------------------------------- */
CEspControl::~CEspControl() {
/* -------------------------------------------------------------------------- */

}



static ctrl_cmd_t answer;

/* #############
 *  PARSE EVENT
 * ############# */

static int esp_host_parse_event(CtrlMsg *ctrl_msg) {
   int rv = FAILURE;

   if (!ctrl_msg) {
      return FAILURE;
   }   

   memset(&answer,0x00, sizeof(ctrl_cmd_t));

   answer.msg_type          = CTRL_EVENT;
   answer.msg_id            = ctrl_msg->msg_id;
   answer.resp_event_status = SUCCESS;

   switch (ctrl_msg->msg_id) {
      case CTRL_EVENT_ESP_INIT: 
         rv = SUCCESS;  
         break;
      case CTRL_EVENT_HEARTBEAT: 
         if(ctrl_msg->event_heartbeat) {
            answer.u.e_heartbeat.hb_num = ctrl_msg->event_heartbeat->hb_num;
            rv = SUCCESS;
         }
         break;
      case CTRL_EVENT_STATION_DISCONNECT_FROM_AP: 
         if(ctrl_msg->event_station_disconnect_from_ap) {
            answer.resp_event_status = ctrl_msg->event_station_disconnect_from_ap->resp;
            rv = SUCCESS;
         } 
         break;
      case CTRL_EVENT_STATION_DISCONNECT_FROM_ESP_SOFTAP: 
         if(ctrl_msg->event_station_disconnect_from_esp_softap) {
            answer.resp_event_status = ctrl_msg->event_station_disconnect_from_esp_softap->resp;
            if(answer.resp_event_status == SUCCESS) {
               if(ctrl_msg->event_station_disconnect_from_esp_softap->mac.data) {
                  strncpy(answer.u.e_sta_disconnected.mac,(char *)ctrl_msg->event_station_disconnect_from_esp_softap->mac.data, ctrl_msg->event_station_disconnect_from_esp_softap->mac.len);
                  rv = SUCCESS;
               }
            }
         }
         break;
      default: 
         break;
   }

   return rv;
}

int CEspControl::process_ctrl_response(CtrlMsg *ans) {
   int rv = ESP_CONTROL_OK;
   if(ans == nullptr) {
      return ESP_CONTROL_CTRL_ERROR;
   }

   /*
    * HANDLE EVENTs -> only handled by event callbacks
    */

   if(ans->msg_type == CTRL_MSG_TYPE__Event) {
      if(esp_host_is_event_cb_set(ans->msg_id) == CALLBACK_AVAILABLE) { 
         if(esp_host_parse_event(ans) == SUCCESS) {
            esp_host_call_event_cb(ans->msg_id, &answer);
            rv = ESP_CONTROL_EVENT_MESSAGE_RX;
            #ifdef ESP_HOST_DEBUG_ENABLED
            Serial.println("! EVENT RECEIVED");
            #endif

         } 
      }
   }
   else if(ans->msg_type == CTRL_MSG_TYPE__Resp) {
      
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.println("! MESSAGE RECEIVED");
         #endif
         if(esp_host_is_response_cb_set(ans->msg_id) == CALLBACK_AVAILABLE) {
            /* TODO: incorrect callback !!!!!!!!!!! */
            esp_host_call_response_cb(ans->msg_id, &answer);
            rv = ESP_CONTROL_MSG_RX_BUT_HANDLED_BY_CB;
         }
         else {
            rv = ESP_CONTROL_EVENT_MESSAGE_RX;
            #ifdef ESP_HOST_DEBUG_ENABLED
            Serial.println("! MESSAGE RECEIVED A");
            #endif
         }
      
   }
   return rv;

}


/* -------------------------------------------------------------------------- */
/* PRIVATE: this function process all received messages from esp untill a control
 * answer is received */
/* -------------------------------------------------------------------------- */
int CEspControl::process_msgs_received(CtrlMsg **response) {
/* -------------------------------------------------------------------------- */   
   int rv = ESP_CONTROL_EMPTY_RX_QUEUE;  
   CMsg msg;
   /* get the message */
   while(CEspCom::get_msg_from_esp(msg)) {
      
      #ifdef ESP_HOST_DEBUG_ENABLED
      Serial.print("[RX msg]: ");
      #endif

      if(msg.get_if_type() == ESP_SERIAL_IF) {
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.print("Serial CONTROL MESSAGE");
         #endif
         
         /* response MUST BE deleted */
         *response = CCtrlTranslate::CMsg2CtrlMsg(msg);
         if(*response != nullptr) {
            #ifdef ESP_HOST_DEBUG_ENABLED
            Serial.println("    correctly received");
            #endif
            if(process_ctrl_response(*response) == ESP_CONTROL_MSG_RX) {
               rv = ESP_CONTROL_MSG_RX;
               break;
            }
         }
         
      }
      else if(msg.get_if_type() == ESP_STA_IF || msg.get_if_type() == ESP_AP_IF) {
         /* net if message received */
      }
      else if(msg.get_if_type() == ESP_PRIV_IF) {

      }
      else if(msg.get_if_type() == ESP_TEST_IF) {
         
      }
   }
   return rv;
}


int get_wifi_mac_address_from_response(CtrlMsg *ans, char *mac_out, int mac_out_dim) {
   if(ans != nullptr) {
   
   

   if(ans->msg_id == CTRL_RESP_GET_MAC_ADDR) {
      if(ans->resp_get_mac_address != nullptr) {
         if(ans->resp_get_mac_address->mac.data != nullptr) {
            if(ans->resp_get_mac_address->resp == 0) {
               memset(mac_out,0x00,mac_out_dim);
               uint8_t len_l = (ans->resp_get_mac_address->mac.len < mac_out_dim-1) ? ans->resp_get_mac_address->mac.len : mac_out_dim-1;
               strncpy(mac_out,(char *)ans->resp_get_mac_address->mac.data, len_l);

               /* CANCELLARE IL CONTROL MESSAGE !!!!!!!!*/

            }

         }
      
      }


   }

   }

   
   return 0;

}

/* -------------------------------------------------------------------------- */
/* GET WIFI MAC ADDRESS */
/* -------------------------------------------------------------------------- */
int CEspControl::getWifiMacAddress(WifiMode_t mode, char* mac, uint8_t mac_buf_size) {
   int rv = ESP_CONTROL_OK;
   
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqGetMacAddress> req(REQ_WIFI_MAC_ADDRESS, ctrl_msg__req__get_mac_address__init);
   CMsg msg = req.get_wifi_mac_address_msg((WifiMode_t)mode);
   
   if(msg.is_valid()) {
      /* send the message */
      CEspCom::send_msg_to_esp(msg);
     
      if(!esp_host_perform_spi_communication()) {
         CtrlMsg *ans;
         if(ESP_CONTROL_MSG_RX == process_msgs_received(&ans)) {
            get_wifi_mac_address_from_response(ans, mac, mac_buf_size);
         }
         else {
            rv = 55;
         }
      }
      else {
         rv = ESP_CONTROL_ERROR_SPI_COMMUNICATION;
      }
   }
   else {
      rv = ESP_CONTROL_ERROR_MSG_PREPARATION;
   }
   return rv;

}