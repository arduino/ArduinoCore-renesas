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


/* error return value */
#define ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE     -5
#define ESP_CONTROL_ERROR_MISSING_CTRL_RESPONSE        -4
#define ESP_CONTROL_ERROR_MSG_PREPARATION              -3
#define ESP_CONTROL_ERROR_SPI_COMMUNICATION            -2
#define ESP_CONTROL_CTRL_ERROR                         -1
/* ok return value */
#define ESP_CONTROL_OK                                  0
/* valid return values */ 
#define ESP_CONTROL_EMPTY_RX_QUEUE                      1
#define ESP_CONTROL_MSG_RX                              2
#define ESP_CONTROL_MSG_RX_BUT_HANDLED_BY_CB            3
#define ESP_CONTROL_EVENT_MESSAGE_RX                    4 

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

   int getAccessPointConfig();
   int disconnectAccessPoint();
   int getSoftAccessPointConfig();
   int getSoftConnectedStationList();
   int stopSoftAccessPoint();
   int getAccessPointScanList(int timeout);
   int connectAccessPoint(const char *ssid, const char *pwd, const char *bssid, bool wpa3_support, uint32_t interval);
   int setSoftAccessPointVndIe();
   int startSoftAccessPoint();
   
   int getPsMode();
   int setPsMode();
   
   int beginOTA();
   int endOTA();
   int OtaWrite();

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