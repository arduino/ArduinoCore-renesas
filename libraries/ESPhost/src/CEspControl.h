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

#include "CCtrlWrapper.h"
#include "CEspCommunication.h"
#include "esp_host_callbacks.h"




class CEspControl {
public:
   static CEspControl& getInstance();
   CEspControl(CEspControl const&)               = delete;
   void operator=(CEspControl const&)            = delete;
   ~CEspControl();

   int getWifiMacAddress(WifiMode_t mode, char* mac, uint8_t mac_buf_size);
   int setWifiMacAddress(WifiMode_t mode, const char* mac, uint8_t mac_buf_size);
   int getWifiMode(WifiMode_t &mode);
   int setWifiMode(WifiMode_t mode);

   int getAccessPointScanList(vector<wifi_scanlist_t>& l);
   int getAccessPointConfig(wifi_ap_config_t &ap);
   int connectAccessPoint(const char *ssid, const char *pwd, const char *bssid, bool wpa3_support, uint32_t interval, wifi_ap_config_t &ap_out);
   int disconnectAccessPoint();
   
   int getPowerSaveMode(int &power_save_mode);
   int setPowerSaveMode(int power_save_mode);

   int beginOTA();
   int endOTA();
   int otaWrite(ota_write_t &ow);


   int getSoftAccessPointConfig();
   int getSoftConnectedStationList();
   int stopSoftAccessPoint();
   int setSoftAccessPointVndIe();
   int startSoftAccessPoint();
   
   int getWifiCurrentTxPower();
   int setWifiMaxTxPower();

   int configureHeartbeat();

private:
   CEspControl();

   int process_msgs_received(CtrlMsg **response);
   int process_ctrl_response(CtrlMsg *ans);
   int perform_esp_communication(CMsg& msg,  CtrlMsg **response);


};



#endif