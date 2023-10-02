/* ########################################################################## */
/* - File: CEspControl.h
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
 * This class manage all the 'Control' requests that can be send to ESP32 fw  
 * It is conceived as a singletone object */

#ifndef _ARDUINO_ESP_CONTROL_CLASS_H
#define _ARDUINO_ESP_CONTROL_CLASS_H

/*
 * configuration defines
 */ 


#include <string>

#include "CCtrlWrapper.h"
#include "CEspCommunication.h"
#include "EspSpiDriver.h"
#include "CNetIf.h"
#include "CEspCbks.h"
#include <string>

#define WIFI_MAC_ADDRESS_DIM 6

class CNetUtilities {
public:
   
   /* 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
      X X : X X : X X : X  X  :  X  X  :  X  X */
   static bool macStr2macArray(uint8_t *mac_out, const char *mac_in) {
      if(mac_in[2] != ':' || 
         mac_in[5] != ':' ||
         mac_in[8] != ':' ||
         mac_in[11] != ':' ||
         mac_in[14] != ':') {
         return false;
      }

      for(int i = 0; i < 6; i++) {
         std::string str_num(mac_in+(i*3),2);
         *(mac_out+i) = std::stoul(str_num,nullptr,16);
      }

      return true;
   }

   static void macArray2macStr(char *mac_out, const uint8_t *mac_in) {
      std::string MAC = "";
      for(int i = 0; i < WIFI_MAC_ADDRESS_DIM; i++) {
         MAC += std::to_string(*(mac_in + i));
         if(i < WIFI_MAC_ADDRESS_DIM - 1) {
            MAC += ":";
         }
      }
      for(unsigned int i = 0; i < MAC.size(); i++ ) {
         *(mac_out + i) = MAC[i];
      }  
   }
};



class CEspControl {
public:
   static CEspControl& getInstance();
   CEspControl(CEspControl const&)               = delete;
   void operator=(CEspControl const&)            = delete;
   ~CEspControl();

   /* The following functions allow the user to control the ESP32 behaviour 
      Each of this function has the last one parameter which is function callback
      If that parameter is different from nullptr then a callback is automatically
      set up and will be called when the answer to the request will arrive
      If that parameter is a nullptr then the function will wait for the answer 
      to arrive and the behaviour will be synchronous.
      
      ! IMPORTANT: It turns out that when callback are used the "get" parameters of 
      the function are unavailable when the function returns. 

      ! IMPORTANT: when callbacks are used it is user responsability to call
                 communicateWithEsp() in order make the communication happen and 
                 get the callback to eventually be called! */

   int getWifiMacAddress(WifiMac_t& mac, EspCallback_f cb = nullptr);
   int setWifiMacAddress(WifiMac_t& mac, EspCallback_f cb = nullptr);
   int getWifiMode(WifiMode_t &mode, EspCallback_f cb = nullptr);
   int setWifiMode(WifiMode_t mode, EspCallback_f cb = nullptr);

   int getAccessPointScanList(std::vector<AccessPoint_t>& l, EspCallback_f cb = nullptr);

   int connectAccessPoint(WifiApCfg_t &ap_cfg, EspCallback_f cb = nullptr);
   int getAccessPointConfig(WifiApCfg_t &ap, EspCallback_f cb = nullptr);
   int disconnectAccessPoint(EspCallback_f cb = nullptr);

   int getSoftAccessPointConfig(SoftApCfg_t &sap_cfg, EspCallback_f cb = nullptr);
   int getSoftConnectedStationList(std::vector<WifiConnectedSta_t>& l, EspCallback_f cb = nullptr);
   int setSoftAccessPointVndIe(WifiVendorSoftApIe_t &vendor_ie, EspCallback_f cb = nullptr);
   int startSoftAccessPoint(SoftApCfg_t &cfg, EspCallback_f cb = nullptr);
   int stopSoftAccessPoint(EspCallback_f cb = nullptr);
   
   /* heartbeat "answer" is always an event, a callback must be provide in any case */
   int configureHeartbeat(HeartBeat_t &hb, EspCallback_f cb);
   
   void listenForInitEvent(EspCallback_f cb);
   void listenForStationDisconnectEvent(EspCallback_f cb);
   void listenForDisconnectionFromSoftApEvent(EspCallback_f cb);

   int getPowerSaveMode(int &power_save_mode, EspCallback_f cb = nullptr);
   int setPowerSaveMode(int power_save_mode, EspCallback_f cb = nullptr);

   int beginOTA(EspCallback_f cb = nullptr);
   int endOTA(EspCallback_f cb = nullptr);
   int otaWrite(OtaWrite_t &ow, EspCallback_f cb = nullptr);

   int getWifiCurrentTxPower(uint32_t &max_power, EspCallback_f cb = nullptr);
   int setWifiMaxTxPower(uint32_t max_power, EspCallback_f cb = nullptr);

   int sendBuffer(ESP_INTERFACE_TYPE type, uint8_t num, uint8_t *buf, uint16_t dim);
   
   uint8_t *getStationRx(uint8_t &if_num, uint16_t &dim);
   uint16_t peekStationRxMsgSize();
   uint8_t *getSoftApRx(uint8_t &if_num, uint16_t &dim);

   
   
   

   int initSpiDriver() { return esp_host_spi_init(); }


   

   void communicateWithEsp();
   
   
   
   /* timeout for synchronous answer */
   void setTimeout_ms(uint32_t tout) {timeout_ms = tout; }
   uint32_t getTimeout_ms() { return timeout_ms; }
private:
   uint32_t timeout_ms;
   CEspControl();

   void prepare_and_send_request(AppMsgId_e id, int& res, void *arg, EspCallback_f cb, CCtrlMsgWrapper& rv);

   /* dispatches the received message to one of the other handler function below*/
   int process_msgs_received(CCtrlMsgWrapper *ans);
   /* process ctrl messages */
   int process_ctrl_messages(CMsg& msg, CCtrlMsgWrapper *ans);
   
   /* process priv messages */
   int process_priv_messages(CCtrlMsgWrapper *ans);
   
   /* process test messages */
   int process_test_messages(CCtrlMsgWrapper* ans);
   int wait_for_answer(CCtrlMsgWrapper* ans);
   int send_net_packet(CMsg& msg);


};



#endif