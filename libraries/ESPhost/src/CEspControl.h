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

#define LWIP_WIFI_HOSTNAME "c33-wifi"
#define IFNAME0 'w'
#define IFNAME1 'f'
#define MAX_TRANSFERT_UNIT 1500
#define IS_WIFI_DEFAULT_INTERFACE 1



#include "CCtrlWrapper.h"
#include "CEspCommunication.h"
#include "esp_host_spi_driver.h"
#include "CNetIf.h"
#include "CEspCbks.h"

#include "lwip/include/lwip/netif.h"

#define WIFI_MAC_ADDRESS_DIM 6

class CNetUtilities {
public:
   static int char2int(char ch) {
      if(ch < 48 || ch > 57) {
         return -1;
      }
      return ch - 48;
   }
   static bool twoChar2uint8(char h, char l, uint8_t *out) {
      int H = char2int(h);
      int L = char2int(l);
      if(H == -1 || L == -1) {
         return false;
      }
      *out = H * 10 + L;
      return true;
   }
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

      if(!twoChar2uint8(mac_in[0],  mac_in[1],  mac_out) ||
         !twoChar2uint8(mac_in[3],  mac_in[4],  mac_out + 1) ||
         !twoChar2uint8(mac_in[6],  mac_in[6],  mac_out + 2) ||
         !twoChar2uint8(mac_in[9],  mac_in[10], mac_out + 3) ||
         !twoChar2uint8(mac_in[12], mac_in[13], mac_out + 4) ||
         !twoChar2uint8(mac_in[15], mac_in[16], mac_out + 5) ) {
         return false;
      }

      return true;
   }
};



class CEspControl {
public:
   static CEspControl& getInstance();
   CEspControl(CEspControl const&)               = delete;
   void operator=(CEspControl const&)            = delete;
   ~CEspControl();

   static uint8_t mac_address[WIFI_MAC_ADDRESS_DIM];

   int getWifiMacAddress(WifiMode_t mode, char* mac, uint8_t mac_buf_size);
   int setWifiMacAddress(WifiMode_t mode, const char* mac, uint8_t mac_buf_size);
   int getWifiMode(WifiMode_t &mode);
   int setWifiMode(WifiMode_t mode);

   int getAccessPointScanList(vector<wifi_scanlist_t>& l);
   int connectAccessPoint(const char *ssid, const char *pwd, const char *bssid, bool wpa3_support, uint32_t interval, wifi_ap_config_t &ap_out);
   int getAccessPointConfig(wifi_ap_config_t &ap);
   int disconnectAccessPoint();

   int getSoftAccessPointConfig(softap_config_t &sap_cfg);
   int getSoftConnectedStationList(vector<wifi_connected_stations_list_t>& l);
   int setSoftAccessPointVndIe(wifi_softap_vendor_ie_t &vendor_ie);
   int startSoftAccessPoint(softap_config_t &cfg);
   int stopSoftAccessPoint();

   int configureHeartbeat(bool enable, int32_t duration);
   
   int getPowerSaveMode(int &power_save_mode);
   int setPowerSaveMode(int power_save_mode);

   int beginOTA();
   int endOTA();
   int otaWrite(ota_write_t &ow);

   

   int getWifiCurrentTxPower(uint32_t &max_power);
   int setWifiMaxTxPower(uint32_t max_power);


   int addNetworkInterface(string name, NetIfRxCb_f _rx_cb);
   int sendOnNetworkInterface(string name, uint8_t *buffer, uint32_t dim);
   int receiveFromNetworkInterface(string name, uint8_t *buffer, uint32_t dim);

   void communicateWithEsp();
   
   /* callback to be passed add_netif lwIP function with wifi initialization */
   static err_t lwip_init_wifi(struct netif *netif);
   /* callback used to set netif_linkoutput_fn used by lwIP to send packet on
      the physical interface */
   static err_t lwip_output_wifi(struct netif *netif, struct pbuf *p);

private:
   CEspControl();
   /* dispatches the received message to one of the other handler function below*/
   int process_msgs_received(CCtrlMsgWrapper *ans);
   /* process ctrl messages */
   int process_ctrl_messages(CMsg& msg, CCtrlMsgWrapper *ans);
   /* process event messages */
   int process_event_messages(CCtrlMsgWrapper *ans);
   /* process priv messages */
   int process_priv_messages(CCtrlMsgWrapper *ans);
   /* process net messages */
   int process_net_messages(CCtrlMsgWrapper *ans);
   /* process test messages */
   int process_test_messages(CCtrlMsgWrapper* ans);
   int perform_esp_communication(CMsg& msg,  CCtrlMsgWrapper* ans);
   int send_net_packet(CMsg& msg);


};



#endif