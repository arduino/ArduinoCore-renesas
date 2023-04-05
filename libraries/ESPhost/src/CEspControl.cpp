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

uint8_t CEspControl::mac_address[WIFI_MAC_ADDRESS_DIM];
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
   Serial.println("Process control response");
   Serial.print("Message type: ");
   Serial.println(ans->msg_type);
   Serial.print("Message ID: ");
   Serial.println(ans->msg_id);



   if(ans->msg_type == CTRL_MSG_TYPE__Event) {
      #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
      Serial.println(" -> EVENT RECEIVED");
      #endif
      
      if(esp_host_is_event_cb_set(ans->msg_id) == CALLBACK_AVAILABLE) { 
         if(esp_host_parse_event(ans) == SUCCESS) {
            esp_host_call_event_cb(ans->msg_id, &answer);
            rv = ESP_CONTROL_EVENT_MESSAGE_RX;
            

         } 
      }
   }
   else if(ans->msg_type == CTRL_MSG_TYPE__Resp) {
         if(esp_host_is_response_cb_set(ans->msg_id) == CALLBACK_AVAILABLE) {
            /* TODO: incorrect callback !!!!!!!!!!! */
            esp_host_call_response_cb(ans->msg_id, &answer);
            rv = ESP_CONTROL_MSG_RX_BUT_HANDLED_BY_CB;
            #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
            Serial.println(" -> CTRL MESSAGE HANDLED BY CALLBACK");
            #endif
         }
         else {
            rv = ESP_CONTROL_EVENT_MESSAGE_RX;
            #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
            Serial.println(" -> CTRL MESSAGE HANDLED BY APPLICATION");
            #endif
         }
      
   }
   return rv;

}


/* -------------------------------------------------------------------------- */
/* This function process ONE message from the queue of the received messages 
   from ESP. 
   

   */
/* -------------------------------------------------------------------------- */
int CEspControl::process_msgs_received(CtrlMsg **response) {
/* -------------------------------------------------------------------------- */   
   int rv = ESP_CONTROL_EMPTY_RX_QUEUE;  
   CMsg msg;
   /* get the message */
   if(CEspCom::get_msg_from_esp(msg)) {
      
      #ifdef ESP_HOST_DEBUG_ENABLED
      Serial.print("[RX PROCESS] Receiving message from ESP rx queue -> ");
      #endif

      if(msg.get_if_type() == ESP_SERIAL_IF) {
         
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.println(" CONTROL MESSAGE");
         #endif
         
         /* response MUST BE deleted */
         *response = CCtrlMsgWrapper<int>::CMsg2CtrlMsg(msg);
         if(*response != nullptr) {
            int res = process_ctrl_response(*response);
            
            if(res == ESP_CONTROL_EVENT_MESSAGE_RX) {
               rv = ESP_CONTROL_MSG_RX;
               //break;
            }
         }
         else {
            
         }
         
      }
      else if(msg.get_if_type() == ESP_STA_IF || msg.get_if_type() == ESP_AP_IF) {
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.println(" NETWORK MESSAGE");
         Serial.print(" Network interface: ");
         Serial.println(msg.get_if_num());
         #endif
      
         /* net if message received */
      }
      else if(msg.get_if_type() == ESP_PRIV_IF) {
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.println(" PRIV MESSAGE");
         #endif

      }
      else if(msg.get_if_type() == ESP_TEST_IF) {
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.println(" TEST MESSAGE");
         #endif
         
      }
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* This function:
   1. verify that msg is valid
   2. if it is insert it into the queue for SPI communication
   3. performs spi communication untill a control message has been received
   NOTE THAT:
   - the function performs spi communication in any case (also if the msg is not
     put into the queue). This is deliberate because we want in any case "to push"
     the communication, but in this case it is likely we don't get any control
     message back (because ww did not sent any request actually) 
   - since it is important to know if the msg request has actually been sent
     that returned value ESP_CONTROL_WRONG_REQUEST_INVALID_MSG is "preserved" and
     it is not overwritten by the result of the communication process 
   Returned values: 
   - ESP_CONTROL_WRONG_REQUEST_INVALID_MSG message has not been sent 
   - ESP_CONTROL_ERROR_MISSING_CTRL_RESPONSE message has been sent to the SPI
       but not control message answer has been received 
   - ESP_CONTROL_MSG_RX request has been sent and a control message answer has been
       received */
/* -------------------------------------------------------------------------- */
int CEspControl::perform_esp_communication(CMsg& msg,  CtrlMsg **response) {
   int rv = ESP_CONTROL_OK; 

   if(msg.is_valid()) {
      /* if the message is valid send it to the spi driver in oder to be 
         sent to ESP32 */
      CEspCom::send_msg_to_esp(msg);
      Serial.println("E");
   }
   else {
      rv = ESP_CONTROL_WRONG_REQUEST_INVALID_MSG;
   }

   int time_num = 0;
   
   /* VERIFY IF ESP32 is ready to accept a SPI transaction */
   bool esp_ready = false;
   bsp_io_level_t handshake;
   do {
      /* send messages untill a valid message is received into the rx queu */
      esp_host_perform_spi_communication(true);
   
      /* process all the received messages, untill a control message is found */
      if(ESP_CONTROL_MSG_RX != process_msgs_received(response)) {
         if(rv != ESP_CONTROL_WRONG_REQUEST_INVALID_MSG) {
            rv = ESP_CONTROL_ERROR_MISSING_CTRL_RESPONSE;
         }  
         else {
            break;
         }     
      }
      else {
         if(rv != ESP_CONTROL_WRONG_REQUEST_INVALID_MSG) {
            rv = ESP_CONTROL_OK;
         } 
         break;
      }
      
      R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MICROSECONDS);
      time_num++;
   } while(time_num < 50000);

   return rv;
}

/* -------------------------------------------------------------------------- */
int CEspControl::send_net_packet(CMsg& msg) {
/* -------------------------------------------------------------------------- */
   int rv = ESP_CONTROL_OK; 

   if(msg.is_valid()) {
      /* if the message is valid send it to the spi driver in oder to be 
         sent to ESP32 */
      CEspCom::send_msg_to_esp(msg);
   }
   else {
      rv = ESP_CONTROL_WRONG_REQUEST_INVALID_MSG;
   }
   
   /* send a message without waiting for an answer */
   esp_host_perform_spi_communication(false);

   return rv;
}

/* -------------------------------------------------------------------------- */
void CEspControl::communicateWithEsp() {
/* -------------------------------------------------------------------------- */   
   CtrlMsg *ans = nullptr;
   esp_host_perform_spi_communication(false);
   process_msgs_received(&ans);
   if(ans != nullptr) {
      Serial.println("-------DELETION NEEDED --------");
   }
}


/* --------------------------------------------------
   GENERAL NOTE ABOUT THE REQUEST function structure:
   --------------------------------------------------
   All the request made to the ESP works in the same way.
   1. a CCtrlMsgWrapper object is created with the proper "protobuf" related type 
      for that request, this ensure that (whatever the is the CtrlMsg is) the
      protobuf stuff are set and intialized correctly 
   
   Note: There are requests that do not "pack" anything in the protobuf, in that case
         the template argument of the CCtrlMsgWrapper template is set to 'int' but 
         it is not actually used
   2. a specific function of the CCtrlMsgWrapper object is called to pack the 
      'inner' CtrlMsg along with other information into a CMsg (that is the message
      that is actually inserted into a queue and sent to SPI driver to be transmitted)
   3. the function perform_esp_communication is called. That function puts the
      CMsg into the queue for the ESP driver and then asks the driver to perform
      the SPI transaction (it may actually request more than one transaction till 
      a control answer is received)
      That function takes as the first argument the message to be sent and as
      second argument a "place" to put the CtrlMsg response.
      The CtrlMsg response is allocate when the received protobuf is unpacked and
      need to be 'free' afterwards.
      It is for this reason that the second argument is req.getAnswerPtrAddress().
      The pointer to the response CtrlMsg is placed into the CCtrlMsgWrapper object
      request itself. 
      This ensure that when it goes out of scope (at the end of the function call)
      the CtrlMsg response is properly de-allocated (if necessary)
      This has another advantage, it creates a link between the request and the response
      which are in fact the same 'thing' so that when...
   4. ... a specific CCtrlMsgWrapper object function is called to properly extract
      information from the the received CtrlMsg, that function already has access
      to the CtrlMsg pointer that holds the answer because is inside the same class. 

   It is not possible to pass to the perform_esp_communication a reference to the 
   CCtrlMsgWrapper object because the class is a template (this will require 
   different signatures depending on the template argument)
   */





/* -------------------------------------------------------------------------- */
/* GET WIFI MAC ADDRESS: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getWifiMacAddress(WifiMode_t mode, char* mac, uint8_t mac_buf_size) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqGetMacAddress> req(CTRL_REQ_GET_MAC_ADDR, ctrl_msg__req__get_mac_address__init);
   CMsg msg = req.getWifiMacAddressMsg((WifiMode_t)mode);
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.extractMacAddress(mac, mac_buf_size)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
      else {
         if(mac_buf_size >= 17) {
            CNetUtilities::macStr2macArray(mac_address, mac);
         }
      }
   }
   
   return rv;
}

/* -------------------------------------------------------------------------- */
/* SET WIFI MAC ADDRESS: see GENERAL NOTE ABOUT THE REQUEST function structure above  */
/* -------------------------------------------------------------------------- */
int CEspControl::setWifiMacAddress(WifiMode_t mode,const char* mac, uint8_t mac_buf_size) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqSetMacAddress> req(CTRL_REQ_SET_MAC_ADDR, ctrl_msg__req__set_mac_address__init);
   CMsg msg = req.setWifiMacAddressMsg((WifiMode_t)mode, mac, mac_buf_size);
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.checkMacAddressSet()) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
      if(mac_buf_size >= 17) {
         CNetUtilities::macStr2macArray(mac_address, mac);
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET WIFI MODE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getWifiMode(WifiMode_t &mode) {
   
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_WIFI_MODE);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.extractWifiMode( mode)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* SET WIFI MODE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::setWifiMode(WifiMode_t mode) {
   ;
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqSetMode> req(CTRL_REQ_SET_WIFI_MODE, ctrl_msg__req__set_mode__init);
   CMsg msg = req.setWifiModeMsg(mode);
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.isSetWifiModeResponse()) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return rv;

}


/* -------------------------------------------------------------------------- */
/* GET ACCESS POINT SCAN LIST: see GENERAL NOTE ABOUT THE REQUEST function structure above*/
/* -------------------------------------------------------------------------- */
int CEspControl::getAccessPointScanList(vector<wifi_scanlist_t>& l) {
   
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_AP_SCAN_LIST);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.extractAccessPointList(l)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* DISCONNECT ACCESS POINT: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::disconnectAccessPoint() {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_DISCONNECT_AP);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.isAccessPointDisconnected()) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET ACCESS POINT CONFIG: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getAccessPointConfig(wifi_ap_config_t &ap) {
   int rv = ESP_CONTROL_CTRL_ERROR;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_AP_CONFIG);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      rv = req.extractAccessPointConfig(ap);
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* CONNECT ACCESS POINT: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::connectAccessPoint(const char *ssid, const char *pwd, const char *bssid, bool wpa3_support, uint32_t interval, wifi_ap_config_t &ap_out) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqConnectAP> req(CTRL_REQ_CONNECT_AP, ctrl_msg__req__connect_ap__init);
   CMsg msg = req.getConnectAccessPointMsg(ssid, pwd, bssid, wpa3_support,interval);
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      rv = req.isAccessPointConnected(ap_out);
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET PS MODE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getPowerSaveMode(int &power_save_mode) {
   
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_PS_MODE);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.getPowerSaveModeSet(power_save_mode)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }  
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET PS MODE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::setPowerSaveMode(int power_save_mode) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqSetMode> req(CTRL_REQ_SET_PS_MODE, ctrl_msg__req__set_mode__init);
   CMsg msg = req.setPowerSaveModeMsg(power_save_mode);
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.isPowerSaveModeSet()) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* OTA WRITE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::otaWrite(ota_write_t &ow) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqOTAWrite> req(CTRL_REQ_OTA_WRITE, ctrl_msg__req__otawrite__init);
   CMsg msg = req.otaWriteMsg(ow);
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.getOtaWriteResult()) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* BEGIN OTA: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::beginOTA() {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_OTA_BEGIN);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.isOtaBegun()) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* END OTA: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::endOTA() {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_OTA_END);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.isOtaEnded()) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      } 
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* STOP SOFT ACCESS POINT: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::stopSoftAccessPoint() {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_STOP_SOFTAP);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.isSoftAccessPointStopped()) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* SET WIFI MAX TX POWER: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::setWifiMaxTxPower(uint32_t max_power) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqSetWifiMaxTxPower> req(CTRL_REQ_SET_WIFI_MAX_TX_POWER, ctrl_msg__req__set_wifi_max_tx_power__init);
   CMsg msg = req.setWifiMaxTxPowerMsg(max_power);
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      rv = req.isWifiMaxPowerSet();
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET WIFI CURRENT TX POWER: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getWifiCurrentTxPower(uint32_t &max_power) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_WIFI_CURR_TX_POWER);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.extractCurrentWifiTxPower(max_power)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      } 
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* GET SOFT ACCESS POINT CONFIG: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getSoftAccessPointConfig(softap_config_t &sap_cfg) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_SOFTAP_CONFIG);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.extractSoftAccessPointConfig(sap_cfg)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* GET SOFT CONNECTED STATION LIST: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getSoftConnectedStationList(vector<wifi_connected_stations_list_t>& l) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<int> req(CTRL_REQ_GET_SOFTAP_CONN_STA_LIST);
   CMsg msg = req.getMsg();
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.extractSoftConnectedStationList(l)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }

   return rv;
}

/* -------------------------------------------------------------------------- */
/* SET SOFT ACCESS POINT VENDOR IE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::setSoftAccessPointVndIe(wifi_softap_vendor_ie_t &vendor_ie) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqSetSoftAPVendorSpecificIE> req(CTRL_REQ_SET_SOFTAP_VND_IE, ctrl_msg__req__set_soft_apvendor_specific_ie__init);
   CMsg msg = req.setSoftAccessPointVndIeMsg(vendor_ie);
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.isSoftAccessPointVndIeSet()) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   CtrlMsgReqSetSoftAPVendorSpecificIE *payload = req.getPayload();
   if(payload->vendor_ie_data != nullptr) {
      delete payload->vendor_ie_data;
   }

   return rv;
}

/* -------------------------------------------------------------------------- */
/* START SOFT ACCESS POINT: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::startSoftAccessPoint(softap_config_t &cfg) {
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqStartSoftAP> req(CTRL_REQ_START_SOFTAP, ctrl_msg__req__start_soft_ap__init);
   CMsg msg = req.startSoftAccessPointMsg(cfg);
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.isSoftAccessPointStarted(cfg)) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
 
   return rv;
}


/* -------------------------------------------------------------------------- */
/* CONFIGURE HEARTBEAT: see GENERAL NOTE ABOUT THE REQUEST function structure above  */
/* -------------------------------------------------------------------------- */
int CEspControl::configureHeartbeat(bool enable, int32_t duration) {
   
   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper<CtrlMsgReqConfigHeartbeat> req(CTRL_REQ_CONFIG_HEARTBEAT, ctrl_msg__req__config_heartbeat__init);
   CMsg msg = req.configureHeartbeatMsg(enable, duration);
   rv = perform_esp_communication(msg, req.getAnswerPtrAddress());
   if(rv == ESP_CONTROL_OK) {
      if(!req.isHeartbeatConfigured()) {
         rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return rv;
 }

/* -------------------------------------------------------------------------- */
 err_t CEspControl::lwip_init_wifi(struct netif *netif) {
/* -------------------------------------------------------------------------- */   
   char mac[18];

   #if LWIP_NETIF_HOSTNAME
   /* Initialize interface hostname */
   netif->hostname = LWIP_WIFI_HOSTNAME;
   #endif /* LWIP_NETIF_HOSTNAME */

   netif->name[0] = IFNAME0;
   netif->name[1] = IFNAME1;

   netif->output = etharp_output; /* ??????? */
   netif->linkoutput = lwip_output_wifi;
   
   /* getWifiMacAddress automatically makes a copy of mac_address into the 
      member variable mac_address */
   if(CEspControl::getInstance().getWifiMacAddress(WIFI_MODE_NONE, mac, (uint8_t) 18) == ESP_CONTROL_OK) {
     netif->hwaddr_len = WIFI_MAC_ADDRESS_DIM;
     netif->hwaddr[0] = mac_address[0];
     netif->hwaddr[1] = mac_address[2];
     netif->hwaddr[2] = mac_address[3];
     netif->hwaddr[3] = mac_address[4];
     netif->hwaddr[4] = mac_address[5];
     netif->hwaddr[5] = mac_address[6];
   }

   /* maximum transfer unit */
   netif->mtu = MAX_TRANSFERT_UNIT;

   /* device capabilities */
   /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
   netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;   /* ???????? */


   if(ESP_HOSTED_SPI_DRIVER_OK == esp_host_spi_init()) {
      return ERR_OK;
   }

   return ERR_IF;
 }  

/* -------------------------------------------------------------------------- */
err_t CEspControl::lwip_output_wifi(struct netif *netif, struct pbuf *p) {
/* -------------------------------------------------------------------------- */
   err_t errval = ERR_OK;
   
   CMsg msg = CCtrlMsgWrapper<int>::getNetIfMsg(ESP_STA_IF, netif->num, p->tot_len);
   if(msg.is_valid()) {
      uint16_t bytes_actually_copied = pbuf_copy_partial(p, msg.get_protobuf_ptr(), p->tot_len, 0);
      if(bytes_actually_copied > 0) {
         int res = CEspControl::getInstance().send_net_packet(msg);
         if(res == ESP_HOSTED_SPI_DRIVER_OK || res == ESP_HOSTED_SPI_MESSAGE_RECEIVED) {
            errval = ERR_IF;
         } 
      }
   }
   else {
      errval = ERR_IF;
   }

   
  return errval;
   
}