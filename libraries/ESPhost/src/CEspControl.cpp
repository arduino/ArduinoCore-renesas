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
   CControlRequest<CtrlMsgReqGetMacAddress> req(REQ_WIFI_MAC_ADDRESS, ctrl_msg__req__get_mac_address__init);
   CMsg msg = req.get_wifi_mac_address_msg((WifiMode_t)mode);
   
   if(msg.is_valid()) {
      /* send the message */
      CEspCom::send_msg_to_esp(msg);
     
      if(!esp_host_perform_spi_communication()) {
         CtrlMsg *ans;
         if(2 == esp_host_get_msgs_received(&ans)) {
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