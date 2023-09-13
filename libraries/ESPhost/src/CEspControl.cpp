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
   timeout_ms = 50000;
}
/* -------------------------------------------------------------------------- */
/* DESTRUCTOR */
/* -------------------------------------------------------------------------- */
CEspControl::~CEspControl() {
/* -------------------------------------------------------------------------- */

}




   

/* process priv messages */
/* -------------------------------------------------------------------------- */
int CEspControl::process_priv_messages(CCtrlMsgWrapper* response) {
/* -------------------------------------------------------------------------- */   
   (void)(response);
   return 0;
}

/* process test messages */
/* -------------------------------------------------------------------------- */
int CEspControl::process_test_messages(CCtrlMsgWrapper* response) {
   (void)(response);
   return 0;
}

/* -------------------------------------------------------------------------- */
uint8_t *CEspControl::getStationRx(uint8_t &if_num, uint16_t &dim) {
/* -------------------------------------------------------------------------- */   
   uint8_t *rv = nullptr;
   CMsg msg;
   __disable_irq();
   bool res = CEspCom::getMsgForStation(msg);
   if(!res) {
      CEspCom::clearStationRx();
   }
   __enable_irq();
   if(res) {
      if_num = msg.get_if_num();
      dim = msg.get_protobuf_dim();
      rv = new uint8_t[dim];
      if(rv != nullptr) {
         memcpy(rv,msg.data(),dim);
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
uint16_t CEspControl::peekStationRxMsgSize() {
/* -------------------------------------------------------------------------- */
   uint16_t res;
   __disable_irq();
   res = CEspCom::peekMsgSizeForStation();
   __enable_irq();
   return res;
}

/* -------------------------------------------------------------------------- */
uint8_t *CEspControl::getSoftApRx(uint8_t &if_num, uint16_t &dim) {
/* -------------------------------------------------------------------------- */   
   uint8_t *rv = nullptr;
   CMsg msg; 
   __disable_irq();
   bool res = CEspCom::getMsgForSoftAp(msg);
   if(!res) {
      CEspCom::clearSoftApRx();
   }
   __enable_irq();
   if(res) {
      if_num = msg.get_if_num();
      dim = msg.get_protobuf_dim();
      rv = new uint8_t[dim];
      if(rv != nullptr) {
         memcpy(rv,msg.data(),dim);
      }
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* PROCESS CONTROL MESSAGES */
/* -------------------------------------------------------------------------- */
int CEspControl::process_ctrl_messages(CMsg& msg, CCtrlMsgWrapper* response) {
/* -------------------------------------------------------------------------- */   
   int rv = ESP_CONTROL_OK;
   /* This function should be called only by process_msgs_received function:
      in this case response is assured to be different from nullptr...
      ...however check in any case... */

   if(response == nullptr) {
      return ESP_CONTROL_ERROR_WHILE_PROCESS_CTRL_MSG;
   }

   /* A message can be separated in different fragments, extractFromMsg returns 
      true if there are no more fragments and the full CtrlMsg can be used */

   if(response->extractFromMsg(msg)) {
      #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
      Serial.print("   [RX PROCESS] Process control response");
      Serial.print(" MSG TYPE: ");
      Serial.print(response->getType());
      Serial.print(" MSG ID: ");
      Serial.println(response->getId());
      #endif
      /* EVENTS______________________________________________________________ */
      if(response->getType() == CTRL_MSG_TYPE__Event) {
         #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
         Serial.println("   [RX PROCESS] -> EVENT RECEIVED");
         #endif
         /* callback will be called if set up */
         CEspCbk::getInstance().callCallback(response->getId(), response);
         rv = ESP_CONTROL_EVENT_MESSAGE_RX;
      }
      /* CONTROL_ANSWERS_____________________________________________________ */
      else if(response->getType() == CTRL_MSG_TYPE__Resp) {
         /* callback will be called if set up and the function return true in
            this case */
         if(CEspCbk::getInstance().callCallback(response->getId(), response)) {
            rv = ESP_CONTROL_MSG_RX_BUT_HANDLED_BY_CB;
            #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
            Serial.println("   [RX PROCESS] -> CTRL MESSAGE HANDLED BY CALLBACK");
            #endif
         }
         else {
            rv = ESP_CONTROL_MSG_RX;
            #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
            Serial.println("   [RX PROCESS] -> CTRL MESSAGE HANDLED BY APPLICATION");
            #endif
         }
      }
   }
   else {
      /* it is OK if we cannot use the message because we need more fragments */
   }

   return rv;
}


/* -------------------------------------------------------------------------- */
/* This function process ONE message from the queue of the received messages 
   from ESP at time.
   The messages are simply dispatched to the correct function that handle that 
   kind of messages  */
/* -------------------------------------------------------------------------- */
int CEspControl::process_msgs_received(CCtrlMsgWrapper* response) {
/* -------------------------------------------------------------------------- */   
   int rv = ESP_CONTROL_EMPTY_RX_QUEUE;  
   CMsg msg;
   bool res = false;
   do {
   /* get the message */
   __disable_irq();
   res = CEspCom::get_msg_from_esp(msg);
   if(!res) {
      CEspCom::clearFromEspQueue();
   }
   __enable_irq();
   if(res) {
      
      #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
      Serial.print("   [RX PROCESS] Receiving message from ESP rx queue -> ");
      #endif
      /* CONTROL_MESSAGES____________________________________________________ */
      if(msg.get_if_type() == ESP_SERIAL_IF) {
         #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
         Serial.println(" CONTROL MESSAGE");
         #endif
         /* At this point we are sure we are dealing with a control message 
            there can be 2 possibilities:
            1. this function was called with 'response' different from nullptr
               this means that we are waiting for a synchonous answer (and that 
               answer will be put into the response pointer
            2. this function was called with nullptr as argument. We are not
               waiting for a response (it can be an event or we have registered
               a callback). In this case we create here a "dummy" response 
               that will be used to decode the message arrived   

            This ensure that when the resp message goes out of scope all is properly
            cleaned up
         */
         
         if(response == nullptr) {
            CCtrlMsgWrapper resp(CTRL_RESP_BASE); // Dummy ID not used for any valid message
            rv = process_ctrl_messages(msg, &resp);
         }
         else {
            rv = process_ctrl_messages(msg, response); 
         }
         if(rv == ESP_CONTROL_MSG_RX) {
            break;
         }
      }
      /* NETWORK_MESSAGES____________________________________________________ */
      else if(msg.get_if_type() == ESP_STA_IF) {
         #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
         Serial.print(" NETWORK MESSAGE");
         Serial.print(" Station ");
         Serial.println(msg.get_if_num());
         #endif
         __disable_irq();
         CEspCom::storeStationMsg(msg); 
         __enable_irq();
      }
      else if(msg.get_if_type() == ESP_AP_IF) {
         #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
         Serial.print(" NETWORK MESSAGE");
         Serial.print(" Soft Ap ");
         Serial.println(msg.get_if_num());
         #endif
         __disable_irq();
         CEspCom::storeSoftApMsg(msg); 
         __enable_irq();
      }
      /* PRIV_MESSAGES_______________________________________________________ */
      else if(msg.get_if_type() == ESP_PRIV_IF) {
         #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
         Serial.println(" PRIV MESSAGE");
         #endif

      }
      /* TEST_MESSAGES_______________________________________________________ */
      else if(msg.get_if_type() == ESP_TEST_IF) {
         #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
         Serial.println(" TEST MESSAGE");
         #endif
         
      }
   }
   } while(res);
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
int CEspControl::wait_for_answer(CCtrlMsgWrapper* response) {
   int rv = ESP_CONTROL_ERROR_MISSING_CTRL_RESPONSE; 
   unsigned int time_num = 0;
   
   do {
      /* send messages untill a valid message is received into the rx queu */
      esp_host_perform_spi_communication(true);
   
      if(process_msgs_received(response) == ESP_CONTROL_MSG_RX) {
         #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
         Serial.println(" WAIT FOR ANSWER OK!!!!!");
         #endif
         rv = ESP_CONTROL_OK;
         break;
      }

      R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MICROSECONDS);
      time_num++;
   } while(time_num < timeout_ms);

   return rv;
}

/* -------------------------------------------------------------------------- */
int CEspControl::send_net_packet(CMsg& msg) {
/* -------------------------------------------------------------------------- */
   int rv = ESP_CONTROL_OK; 

   if(msg.is_valid()) {
      /* if the message is valid send it to the spi driver in oder to be 
         sent to ESP32 */
      __disable_irq();
      CEspCom::send_msg_to_esp(msg);
       __enable_irq();
   }
   else {
      rv = ESP_CONTROL_WRONG_REQUEST_INVALID_MSG;
   }
   
   return rv;
}

/* -------------------------------------------------------------------------- */
int CEspControl::sendBuffer(ESP_INTERFACE_TYPE type, uint8_t num, uint8_t *buf, uint16_t dim) {
/* -------------------------------------------------------------------------- */
   CCtrlMsgWrapper req; 
   CMsg msg = req.getNetIfMsg(type, num, buf, dim);
   return send_net_packet(msg);
}



/* -------------------------------------------------------------------------- */
void CEspControl::communicateWithEsp() {
/* -------------------------------------------------------------------------- */   
   if(isEspSpiInitialized() && !isSpiTransactionInProgress()) {
      esp_host_perform_spi_communication(false);
      process_msgs_received(nullptr);
   }
}


void CEspControl::prepare_and_send_request(AppMsgId_e id, 
                                                       int& res, 
                                                       void *arg,
                                                      EspCallback_f cb,
                                                      CCtrlMsgWrapper& rv) {
   bool go_on = true;
   rv.setRequestId(id);
   CMsg msg;
   
   switch(id){
      case CTRL_REQ_GET_MAC_ADDR:
      msg = rv.getWifiMacAddressMsg((WifiMode_t)(((WifiMac_t *)arg)->mode));
      break;
      case CTRL_REQ_SET_MAC_ADDR:{
         WifiMac_t *ptr = (WifiMac_t *)arg;
         msg = rv.setWifiMacAddressMsg(ptr->mode, ptr->mac, MAX_MAC_STR_LEN);
      }
      break;
      case CTRL_REQ_GET_WIFI_MODE:
         msg = rv.getMsg(); 
      break;
      case CTRL_REQ_SET_WIFI_MODE:
         msg = rv.setWifiModeMsg((WifiMode_t)(((WifiMac_t *)arg)->mode)); 
      break;
      case CTRL_REQ_GET_AP_SCAN_LIST:
         msg = rv.getMsg();
      break;
      case CTRL_REQ_DISCONNECT_AP:
         msg = rv.getMsg();
      break;
      case CTRL_REQ_GET_AP_CONFIG:
         msg = rv.getMsg();
      break;
      case CTRL_REQ_CONNECT_AP: {
         WifiApCfg_t *ptr = (WifiApCfg_t *)arg;
         msg = rv.getConnectAccessPointMsg((const char*)ptr->ssid, (const char*)ptr->pwd, (const char*)ptr->bssid, ptr->is_wpa3_supported,ptr->listen_interval);
      }
      break;
      case CTRL_REQ_GET_PS_MODE:
         msg = rv.getMsg();
      break;
      case CTRL_REQ_SET_PS_MODE:
         msg = rv.setPowerSaveModeMsg(*((PowerSave_t *)arg));
      break;
      case CTRL_REQ_OTA_WRITE: {
         OtaWrite_t *ptr = (OtaWrite_t *)arg;
         msg = rv.otaWriteMsg(*ptr);
      }
      break;
      case CTRL_REQ_OTA_BEGIN: 
         msg = rv.getMsg();
      break;
      case CTRL_REQ_OTA_END: 
         msg = rv.getMsg();
      break;
      case CTRL_REQ_STOP_SOFTAP: 
         msg = rv.getMsg();
      break;
      case CTRL_REQ_SET_WIFI_MAX_TX_POWER:
         msg = rv.setWifiMaxTxPowerMsg(*((uint32_t *)arg));
      break;
      case CTRL_REQ_GET_WIFI_CURR_TX_POWER:
         msg = rv.getMsg();
      break;
      case CTRL_REQ_GET_SOFTAP_CONFIG:
         msg = rv.getMsg();
      break;
      case CTRL_REQ_GET_SOFTAP_CONN_STA_LIST:
         msg = rv.getMsg();
      break;
      case CTRL_REQ_SET_SOFTAP_VND_IE:
         msg = rv.setSoftAccessPointVndIeMsg(*((WifiVendorSoftApIe_t *)arg));
      break;
      case CTRL_REQ_START_SOFTAP:
         msg = rv.startSoftAccessPointMsg(*((SoftApCfg_t *)arg));
      break;
      case CTRL_REQ_CONFIG_HEARTBEAT:{
         HeartBeat_t *ptr = (HeartBeat_t *)arg;
         msg = rv.configureHeartbeatMsg(ptr->enable, ptr->duration);
      }
      break;

      default: 
         /* UNKNOWN MESSAGE */
         go_on = false;
         res = ESP_CONTROL_WRONG_REQUEST_INVALID_MSG;
         //Serial.println("A ESP_CONTROL_WRONG_REQUEST_INVALID_MSG");
      break;
   }
   
   if(go_on) {
      __disable_irq();
      bool res = CEspCom::send_msg_to_esp(msg);
       __enable_irq();
      if(res) {
         /* setCallback returns true if a 'true' callback is set up, but it will in any
         case reset the cb if nullptr is passed */
         if(id == CTRL_REQ_CONFIG_HEARTBEAT) {
            CEspCbk::getInstance().setCallback(CTRL_EVENT_HEARTBEAT, cb);
            res = wait_for_answer(&rv);
         }
         else if(!CEspCbk::getInstance().setCallback(id, cb)) {
            res = wait_for_answer(&rv);
         }
         else {
            res = ESP_CONTROL_MSG_SENT_AND_CB_SET_UP;
            //Serial.println("B ESP_CONTROL_MSG_SENT_AND_CB_SET_UP");
         }
      }
      else {
         res = ESP_CONTROL_WRONG_REQUEST_INVALID_MSG;
         //Serial.println("C ESP_CONTROL_WRONG_REQUEST_INVALID_MSG");
      }
   }
   
   return;  
}

/* -------------------------------------------------------------------------- */
void CEspControl::listenForInitEvent(EspCallback_f cb) {
/* -------------------------------------------------------------------------- */   
   CEspCbk::getInstance().setCallback(CTRL_EVENT_ESP_INIT, cb);
}

/* -------------------------------------------------------------------------- */
void CEspControl::listenForStationDisconnectEvent(EspCallback_f cb) {
/* -------------------------------------------------------------------------- */   
   CEspCbk::getInstance().setCallback(CTRL_EVENT_STATION_DISCONNECT_FROM_AP, cb);
}   
/* -------------------------------------------------------------------------- */
void CEspControl::listenForDisconnectionFromSoftApEvent(EspCallback_f cb) {
/* -------------------------------------------------------------------------- */   
   CEspCbk::getInstance().setCallback(CTRL_EVENT_STATION_DISCONNECT_FROM_ESP_SOFTAP, cb);
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
   */
/* -------------------------------------------------------------------------- */
/* GET WIFI MAC ADDRESS: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getWifiMacAddress(WifiMac_t& CAM, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::getWifiMacAddress");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req;
   prepare_and_send_request(CTRL_REQ_GET_MAC_ADDR, rv, &CAM, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.extractMacAddress(CAM.mac, MAX_MAC_STR_LEN);
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* SET WIFI MAC ADDRESS: see GENERAL NOTE ABOUT THE REQUEST function structure above  */
/* -------------------------------------------------------------------------- */
int CEspControl::setWifiMacAddress(WifiMac_t& CAM, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::setWifiMacAddress");
   #endif

   int rv = ESP_CONTROL_OK;
   /* message request preparation */
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_SET_MAC_ADDR, rv, &CAM, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.checkMacAddressSet();
   }
   
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET WIFI MODE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getWifiMode(WifiMode_t &mode, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::getWifiMode");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_GET_WIFI_MODE, rv, nullptr, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.extractWifiMode(mode);
   }
   
   return rv;
}

/* -------------------------------------------------------------------------- */
/* SET WIFI MODE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::setWifiMode(WifiMode_t mode, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::setWifiMode");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req;
   prepare_and_send_request(CTRL_REQ_SET_WIFI_MODE, rv, &mode, cb, req);   
   if(rv == ESP_CONTROL_OK) {
      rv = req.isSetWifiModeResponse();
   }

   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.print("[RESPONSE] CEspControl::setWifiMode ");
   Serial.println(rv);
   #endif


   return rv;
}


/* -------------------------------------------------------------------------- */
/* GET ACCESS POINT SCAN LIST: see GENERAL NOTE ABOUT THE REQUEST function structure above*/
/* -------------------------------------------------------------------------- */
int CEspControl::getAccessPointScanList(std::vector<AccessPoint_t>& l, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::getAccessPointScanList");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req;
   prepare_and_send_request(CTRL_REQ_GET_AP_SCAN_LIST, rv, nullptr, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.extractAccessPointList(l);
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* DISCONNECT ACCESS POINT: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::disconnectAccessPoint(EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::disconnectAccessPoint");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req;
   prepare_and_send_request(CTRL_REQ_DISCONNECT_AP, rv, nullptr, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.isAccessPointDisconnected();
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET ACCESS POINT CONFIG: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getAccessPointConfig(WifiApCfg_t &ap, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::getAccessPointConfig");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_GET_AP_CONFIG, rv, nullptr, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.extractAccessPointConfig(ap);
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* CONNECT ACCESS POINT: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::connectAccessPoint(WifiApCfg_t &ap_cfg,  
                                    EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::connectAccessPoint");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_CONNECT_AP, rv, &ap_cfg, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.isAccessPointConnected(ap_cfg);
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET PS MODE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getPowerSaveMode(int &power_save_mode, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::getPowerSaveMode");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_GET_PS_MODE, rv, nullptr, cb, req);   
   if(rv == ESP_CONTROL_OK) {
      rv = req.getPowerSaveModeSet(power_save_mode);
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET PS MODE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::setPowerSaveMode(int power_save_mode, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::setPowerSaveMode");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req;
   prepare_and_send_request(CTRL_REQ_SET_PS_MODE, rv, &power_save_mode, cb, req); 
   if(rv == ESP_CONTROL_OK) {
      rv = req.isPowerSaveModeSet();
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* OTA WRITE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::otaWrite(OtaWrite_t &ow, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::otaWrite");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req;
   prepare_and_send_request(CTRL_REQ_OTA_WRITE, rv, &ow, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.getOtaWriteResult();
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* BEGIN OTA: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::beginOTA(EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::beginOTA");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_OTA_BEGIN, rv, nullptr, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.isOtaBegun();
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* END OTA: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::endOTA(EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::endOTA");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_OTA_END, rv, nullptr, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.isOtaEnded();
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* STOP SOFT ACCESS POINT: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::stopSoftAccessPoint(EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::stopSoftAccessPoint");
   #endif


   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_STOP_SOFTAP, rv, nullptr, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.isSoftAccessPointStopped();
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* SET WIFI MAX TX POWER: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::setWifiMaxTxPower(uint32_t max_power, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::setWifiMaxTxPower");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req;
   prepare_and_send_request(CTRL_REQ_SET_WIFI_MAX_TX_POWER, rv, &max_power, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.isWifiMaxPowerSet();
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* GET WIFI CURRENT TX POWER: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getWifiCurrentTxPower(uint32_t &max_power, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::getWifiCurrentTxPower");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_GET_WIFI_CURR_TX_POWER, rv, nullptr, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.extractCurrentWifiTxPower(max_power);
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* GET SOFT ACCESS POINT CONFIG: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getSoftAccessPointConfig(SoftApCfg_t &sap_cfg, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::getSoftAccessPointConfig");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_GET_SOFTAP_CONFIG, rv, nullptr, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.extractSoftAccessPointConfig(sap_cfg);
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* GET SOFT CONNECTED STATION LIST: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::getSoftConnectedStationList(std::vector<WifiConnectedSta_t>& l, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::getSoftConnectedStationList");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req; 
   prepare_and_send_request(CTRL_REQ_GET_SOFTAP_CONN_STA_LIST, rv, nullptr, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.extractSoftConnectedStationList(l);
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/* SET SOFT ACCESS POINT VENDOR IE: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::setSoftAccessPointVndIe(WifiVendorSoftApIe_t &vendor_ie, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::setSoftAccessPointVndIe");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req;
   prepare_and_send_request(CTRL_REQ_SET_SOFTAP_VND_IE, rv, &vendor_ie, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.isSoftAccessPointVndIeSet();
   }
   
   return rv;
}

/* -------------------------------------------------------------------------- */
/* START SOFT ACCESS POINT: see GENERAL NOTE ABOUT THE REQUEST function structure above */
/* -------------------------------------------------------------------------- */
int CEspControl::startSoftAccessPoint(SoftApCfg_t &cfg, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::startSoftAccessPoint");
   #endif

   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req;
   prepare_and_send_request(CTRL_REQ_START_SOFTAP, rv, &cfg, cb, req);
   if(rv == ESP_CONTROL_OK) {
      rv = req.isSoftAccessPointStarted(cfg);
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/* CONFIGURE HEARTBEAT: see GENERAL NOTE ABOUT THE REQUEST function structure above  */
/* -------------------------------------------------------------------------- */
int CEspControl::configureHeartbeat(HeartBeat_t &hb, EspCallback_f cb) {
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.println("[REQUEST] CEspControl::configureHeartbeat");
   #endif


   int rv = ESP_CONTROL_OK;
   CCtrlMsgWrapper req;
   prepare_and_send_request(CTRL_REQ_CONFIG_HEARTBEAT, rv, &hb, cb, req);
   /* since heartbeat is an event we always wait for confirmation when we set, req
      up a cb */
   if(rv == ESP_CONTROL_OK) {
      rv = req.isHeartbeatConfigured();
   }
   return rv;
 }

