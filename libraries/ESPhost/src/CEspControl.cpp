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

#define ESP_HOST_DEBUG_ENABLED

extern int esp_host_perform_spi_communication();
extern int esp_host_spi_init(void);


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
            Serial.println(" -> EVENT RECEIVED");
            #endif

         } 
      }
   }
   else if(ans->msg_type == CTRL_MSG_TYPE__Resp) {
         if(esp_host_is_response_cb_set(ans->msg_id) == CALLBACK_AVAILABLE) {
            /* TODO: incorrect callback !!!!!!!!!!! */
            esp_host_call_response_cb(ans->msg_id, &answer);
            rv = ESP_CONTROL_MSG_RX_BUT_HANDLED_BY_CB;
            #ifdef ESP_HOST_DEBUG_ENABLED
            Serial.println(" -> CTRL MESSAGE HANDLED BY CALLBACK");
            #endif
         }
         else {
            rv = ESP_CONTROL_EVENT_MESSAGE_RX;
            #ifdef ESP_HOST_DEBUG_ENABLED
            Serial.println(" -> CTRL MESSAGE HANDLED BY APPLICATION");
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
            int res = process_ctrl_response(*response);
            
            if(res == ESP_CONTROL_EVENT_MESSAGE_RX) {
               rv = ESP_CONTROL_MSG_RX;
               break;
            }
         }
         else {
            
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


int CEspControl::perform_esp_communication(CMsg& msg,  CtrlMsg **response) {
   int rv = ESP_CONTROL_MSG_RX; 

   if(msg.is_valid()) {
      /* if the message is valid send it to the spi driver in oder to be 
         sent to ESP32 */
      CEspCom::send_msg_to_esp(msg);
   }
   
   
   int time_num = 0;
   
   /* VERIFY IF ESP32 is ready to accept a SPI transaction */
   bool esp_ready = false;
   bsp_io_level_t handshake;
   do {
      /* send to and receive from all the messages for ESP */
      esp_host_perform_spi_communication();
   
      /* process all the received messages, untill a control message is found */
      if(ESP_CONTROL_MSG_RX != process_msgs_received(response)) {
         rv = ESP_CONTROL_ERROR_MISSING_CTRL_RESPONSE;      
      }
      else {
         rv = ESP_CONTROL_MSG_RX;
         break;
      }
      
      R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MICROSECONDS);
      time_num++;
   } while(time_num < 5000);

   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET WIFI MAC ADDRESS */
/* -------------------------------------------------------------------------- */
int CEspControl::getWifiMacAddress(WifiMode_t mode, char* mac, uint8_t mac_buf_size) {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqGetMacAddress> req(CTRL_REQ_GET_MAC_ADDR, ctrl_msg__req__get_mac_address__init);
   CMsg msg = req.getWifiMacAddressMsg((WifiMode_t)mode);
   
   
   if(ESP_CONTROL_MSG_RX == perform_esp_communication(msg, &ans)) {
      #ifdef ESP_HOST_DEBUG_ENABLED
      Serial.println("EXTRACT MAC ADDRESS ");
      #endif
      if(!CCtrlTranslate::extractMacAddress(ans, mac, mac_buf_size)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
      ctrl_msg__free_unpacked(ans, NULL);

   }
   
   return rv;
}

/* -------------------------------------------------------------------------- */
/* SET WIFI MAC ADDRESS */
/* -------------------------------------------------------------------------- */
int CEspControl::setWifiMacAddress(WifiMode_t mode,const char* mac, uint8_t mac_buf_size) {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqSetMacAddress> req(CTRL_REQ_SET_MAC_ADDR, ctrl_msg__req__set_mac_address__init);
   CMsg msg = req.setWifiMacAddressMsg((WifiMode_t)mode, mac, mac_buf_size);
   
   
   if(ESP_CONTROL_MSG_RX == perform_esp_communication(msg, &ans)) {
      #ifdef ESP_HOST_DEBUG_ENABLED
      Serial.println("CHECK MAC ADDRESS IS SET");
      #endif
      if(!CCtrlTranslate::checkMacAddressSet(ans)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
      ctrl_msg__free_unpacked(ans, NULL);

   }
   
   return rv;

}

/* -------------------------------------------------------------------------- */
/* GET WIFI MODE */
/* -------------------------------------------------------------------------- */
int CEspControl::getWifiMode(WifiMode_t &mode) {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_WIFI_MODE);
   CMsg msg = req.getMsg();

   if(ESP_CONTROL_MSG_RX == perform_esp_communication(msg, &ans)) {
      if(!CCtrlTranslate::extractWifiMode(ans, mode)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
      ctrl_msg__free_unpacked(ans, NULL); 
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* SET WIFI MODE */
/* -------------------------------------------------------------------------- */
int CEspControl::setWifiMode(WifiMode_t mode) {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqSetMode> req(CTRL_REQ_SET_WIFI_MODE, ctrl_msg__req__set_mode__init);
   CMsg msg = req.setWifiModeMsg(mode);
   if(ESP_CONTROL_MSG_RX == perform_esp_communication(msg, &ans)) {
      if(!CCtrlTranslate::isSetWifiModeResponse(ans)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
      ctrl_msg__free_unpacked(ans, NULL); 
   }
   return rv;

}


/* -------------------------------------------------------------------------- */
/* GET ACCESS POINT SCAN LIST */
/* -------------------------------------------------------------------------- */
int CEspControl::getAccessPointScanList(vector<wifi_scanlist_t>& l) {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_AP_SCAN_LIST);
   CMsg msg = req.getMsg();
   if(ESP_CONTROL_MSG_RX == perform_esp_communication(msg, &ans)) {
      if(!CCtrlTranslate::extractAccessPointList(ans,l)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
      ctrl_msg__free_unpacked(ans, NULL); 
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* DISCONNECT ACCESS POINT */
/* -------------------------------------------------------------------------- */
int CEspControl::disconnectAccessPoint() {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_DISCONNECT_AP);
   CMsg msg = req.getMsg();
   if(ESP_CONTROL_MSG_RX == perform_esp_communication(msg, &ans)) {
      if(!CCtrlTranslate::isAccessPointDisconnected(ans)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
      ctrl_msg__free_unpacked(ans, NULL); 
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET ACCESS POINT CONFIG */
/* -------------------------------------------------------------------------- */
int CEspControl::getAccessPointConfig(wifi_ap_config_t &ap) {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_CTRL_ERROR;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqConnectAP> req(CTRL_REQ_GET_AP_CONFIG, ctrl_msg__req__connect_ap__init);
   CMsg msg = req.getMsg();

   if(ESP_CONTROL_MSG_RX == perform_esp_communication(msg, &ans)) {
      rv = CCtrlTranslate::extractAccessPointConfig(ans, ap);
      ctrl_msg__free_unpacked(ans, NULL); 
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* CONNECT ACCESS POINT */
/* -------------------------------------------------------------------------- */
int CEspControl::connectAccessPoint(const char *ssid, const char *pwd, const char *bssid, bool wpa3_support, uint32_t interval, wifi_ap_config_t &ap_out) {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqConnectAP> req(CTRL_REQ_CONNECT_AP);
   CMsg msg = req.getConnectAccessPointMsg(ssid, pwd, bssid, wpa3_support,interval);
   
   if(ESP_CONTROL_MSG_RX == perform_esp_communication(msg, &ans)) {
      rv = CCtrlTranslate::isAccessPointConnected(ans, ap_out);
      ctrl_msg__free_unpacked(ans, NULL); 
   }
   

   return rv;
}





/* -------------------------------------------------------------------------- */
/* GET PS MODE */
/* -------------------------------------------------------------------------- */
int CEspControl::getPowerSaveMode(int &power_save_mode) {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_PS_MODE);
   CMsg msg = req.getMsg();

   if(ESP_CONTROL_MSG_RX == perform_esp_communication(msg, &ans)) {
      if(!CCtrlTranslate::getPowerSaveModeSet(ans, power_save_mode)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
      ctrl_msg__free_unpacked(ans, NULL); 
   }
  

   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET PS MODE */
/* -------------------------------------------------------------------------- */
int CEspControl::setPowerSaveMode(int power_save_mode) {
   
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   
   CCtrlMsgWrapper<CtrlMsgReqSetMode> req(CTRL_REQ_SET_PS_MODE, ctrl_msg__req__set_mode__init);
   CMsg msg = req.setPowerSaveModeMsg(power_save_mode);

   if(ESP_CONTROL_MSG_RX == perform_esp_communication(msg, &ans)) {
    if(!CCtrlTranslate::isPowerSaveModeSet(ans)) {
       rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
    }
    ctrl_msg__free_unpacked(ans, NULL); 
   }

   return rv;
}


/* ??????????????????????????????????????????????????????????????????????????? */


/* -------------------------------------------------------------------------- */
/* GET SOFT ACCESS POINT CONFIG */
/* -------------------------------------------------------------------------- */
int CEspControl::getSoftAccessPointConfig() {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_SOFTAP_CONFIG);
   CMsg msg = req.getMsg();


   return rv;
}




/* -------------------------------------------------------------------------- */
/* GET SOFT CONNECTED STATION LIST */
/* -------------------------------------------------------------------------- */
int CEspControl::getSoftConnectedStationList() {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_SOFTAP_CONN_STA_LIST);
   CMsg msg = req.getMsg();


   return rv;
}

/* -------------------------------------------------------------------------- */
/* STOP SOFT ACCESS POINT */
/* -------------------------------------------------------------------------- */
int CEspControl::stopSoftAccessPoint() {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_STOP_SOFTAP);
   CMsg msg = req.getMsg();
  

   return rv;
}



/* -------------------------------------------------------------------------- */
/* BEGIN OTA */
/* -------------------------------------------------------------------------- */
int CEspControl::beginOTA() {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_OTA_BEGIN);
   CMsg msg = req.getMsg();
  

   return rv;


}

/* -------------------------------------------------------------------------- */
/* END OTA */
/* -------------------------------------------------------------------------- */
int CEspControl::endOTA() {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_OTA_END);
   CMsg msg = req.getMsg();
  

   return rv;


}

/* -------------------------------------------------------------------------- */
/* END OTA */
/* -------------------------------------------------------------------------- */
int CEspControl::getWifiCurrentTxPower() {

   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_WIFI_CURR_TX_POWER);
   CMsg msg = req.getMsg();
  

   return rv;

}






int CEspControl::setSoftAccessPointVndIe() {

   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_SET_SOFTAP_VND_IE);
   


   CMsg msg /* TODO !!!*/;
  

   return rv;

}

int CEspControl::startSoftAccessPoint() {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_START_SOFTAP);
   


   CMsg msg /* TODO !!!*/;
  

   return rv;

}




int CEspControl::setWifiMaxTxPower() {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_SET_WIFI_MAX_TX_POWER);
   


   CMsg msg /* TODO !!!*/;
  

   return rv;

}

 int CEspControl::configureHeartbeat() {
   CtrlMsg *ans;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_CONFIG_HEARTBEAT);
   


   CMsg msg /* TODO !!!*/;
  

   return rv;
 }