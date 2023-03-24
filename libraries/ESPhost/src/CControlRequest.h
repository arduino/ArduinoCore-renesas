#ifndef _ARDUINO_ESP_CONTROL_REQUEST_CLASS_H
#define _ARDUINO_ESP_CONTROL_REQUEST_CLASS_H

#include "esp_hosted_config.pb-c.h"
#include "CMsg.h"

const uint8_t REQ_WIFI_MAC_ADDRESS = (uint8_t)CTRL_MSG_ID__Req_GetMACAddress;




typedef enum {
   WIFI_MODE_NONE = CTRL__WIFI_MODE__NONE,
   WIFI_MODE_STA = CTRL__WIFI_MODE__STA,
   WIFI_MODE_AP = CTRL__WIFI_MODE__AP,
   WIFI_MODE_APSTA = CTRL__WIFI_MODE__APSTA,
   WIFI_MODE_MAX
} WifiMode_t;


template<typename T>
class CControlRequest {
private:
   T *payload;
   CtrlMsg request;
   
   CMsg get_msg() {
      int protobuf_len = ctrl_msg__get_packed_size(&request);
      CMsg msg(protobuf_len);
      if(msg.is_valid()){
         /* pack request into the message */
         ctrl_msg__pack(&request, msg.get_protobuf_ptr());
         msg.set_tlv_header(CTRL_EP_NAME_RESP);
         msg.set_payload_header(ESP_SERIAL_IF, 0);
         return msg;
      }
      return CMsg(0);
   }

public:
   CControlRequest(int request_id, void (*init)(T *)) {
      /* init the protobuf request*/
      memset(&request,0x00,sizeof(request));
      ctrl_msg__init(&request);
      /* set the id of the message and the payload case */
      request.msg_id = (CtrlMsgId)request_id;
      request.payload_case = (CtrlMsg__PayloadCase)request_id;
      /* allocate the payload */
      payload = new T;
      /* call the init function for the payload */
      if(payload != nullptr) {
         if(init != nullptr) {
            init(payload);
         }
      }
   }
   CMsg get_wifi_mac_address_msg(WifiMode_t mode) {
      if(payload != nullptr && mode < WIFI_MODE_MAX && mode > WIFI_MODE_NONE) {
         request.req_get_mac_address = payload;
         payload->mode = (CtrlWifiMode)mode;
      }
      return get_msg();
   }

   ~CControlRequest() {
      if(payload != nullptr) {
         delete payload;
      }
   }

};



#endif