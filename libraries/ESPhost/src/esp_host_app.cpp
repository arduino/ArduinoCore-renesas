#include "esp_host_app.h"


int esp_host_app_init() {
   esp_host_spi_init();
   //esp_host_set_cb_rx(esp_host_msg_received);
}


int esp_host_get_wifi_mac_address_old(int mode, char *mac_out, int mac_out_dim) {
   ctrl_cmd_t req;

   req.msg_type        = CTRL_REQ;
   req.ctrl_resp_cb    = nullptr;
   req.cmd_timeout_sec = DEFAULT_CTRL_RESP_TIMEOUT;

   req.u.wifi_mac.mode = mode;

   req.msg_id          = CTRL_REQ_GET_MAC_ADDR;                                               
   
   if(SUCCESS != esp_host_ctrl_send_req(&req)) {                                            
        return FAILURE;                                                  
   }

   ctrl_cmd_t *ans;

   if(esp_host_wait_for_answer(ans)) {
      if(!ans->u.wifi_mac.mac) {
         //printf("NULL MAC returned\n\r");
         return FAILURE;
      }
      strncpy(mac_out, ans->u.wifi_mac.mac, MAX_MAC_STR_LEN);
      mac_out[MAX_MAC_STR_LEN-1] = '\0';
   }

}





CMsg esp_host_prepare_msg(int protobuf_len, CtrlMsg *ptr) {
   

}

extern int esp_host_spi_transaction(void) ;


CMsg get_wifi_mac_address_request_msg(int mode) {
   /* actual CtrlMsg esp-host structure - to be packed in a protobuf */
   CtrlMsg   req = {0};
   
   /* protobuf initialization */
   ctrl_msg__init(&req);

   /* request for MAC address */
   req.msg_id = (CtrlMsgId)CTRL_MSG_ID__Req_GetMACAddress;
   req.payload_case = (CtrlMsg__PayloadCase) CTRL_MSG_ID__Req_GetMACAddress;

   /* allocation of the */
   CtrlMsgReqGetMacAddress *payload = new CtrlMsgReqGetMacAddress;

   if(payload != nullptr) {
      req.req_get_mac_address = payload;

      ctrl_msg__req__get_mac_address__init(payload);
      payload->mode = CTRL__WIFI_MODE__STA;    // TOBEVERIFIED!

      int protobuf_len = ctrl_msg__get_packed_size(&req);
      CMsg msg(protobuf_len);
      
      if(msg.is_valid()){
         /* pack request into the message */
         ctrl_msg__pack(&req, msg.get_protobuf_ptr());
         msg.set_tlv_header(CTRL_EP_NAME_RESP);
         msg.set_payload_header(ESP_SERIAL_IF, 0);
      }
   
      delete payload;
      return msg;
   }
   /* if something went wrong return an empty message */
   return CMsg(0);
}

int esp_host_get_wifi_mac_address(int mode, char *mac_out, int mac_out_dim) {
   int rv = ESP_HOSTED_OK;
   CMsg msg = get_wifi_mac_address_request_msg(mode);
   if(msg.is_valid()) {
      application_send_msg_to_esp32(msg);
      if(esp_host_perform_spi_communication() == ESP_HOSTED_SPI_DRIVER_OK) {

      }
      else {
         rv = ESP_HOSTED_ERROR_SPI_COMMUNICATION;
      }
      
   }
   else {
      rv = ESP_HOSTED_ERROR_MSG_PREPARATION;
   }
   return rv;
}


int esp_host_get_wifi_mac_address_v0(int mode, char *mac_out, int mac_out_dim) {

   CtrlMsg   req = {0};

   ctrl_msg__init(&req);
   req.msg_id = (CtrlMsgId)CTRL_MSG_ID__Req_GetMACAddress;
   req.payload_case = (CtrlMsg__PayloadCase) CTRL_MSG_ID__Req_GetMACAddress;

   CtrlMsgReqGetMacAddress *field = new CtrlMsgReqGetMacAddress; 

   if(field != nullptr) {
      req.req_get_mac_address = field;
      ctrl_msg__req__get_mac_address__init(field);
      field->mode = CTRL__WIFI_MODE__STA;

      int protobuf_len = ctrl_msg__get_packed_size(&req);
      CMsg msg(protobuf_len);
      ctrl_msg__pack(&req, msg.get_protobuf_ptr());
      msg.set_tlv_header(CTRL_EP_NAME_RESP);
      msg.set_payload_header(ESP_SERIAL_IF, 0);
      application_send_msg_to_esp32(msg);
      delete field;
   }

   
   ctrl_cmd_t *ans;

   esp_host_spi_transaction();
   
   while(esp_host_msg_received(&ans) == 0) {
      esp_host_spi_transaction();
   }

   if(!ans->u.wifi_mac.mac) {
         //printf("NULL MAC returned\n\r");
         return FAILURE;
      }
      strncpy(mac_out, ans->u.wifi_mac.mac, MAX_MAC_STR_LEN);
      mac_out[MAX_MAC_STR_LEN-1] = '\0';

   esp_host_spi_transaction();




}