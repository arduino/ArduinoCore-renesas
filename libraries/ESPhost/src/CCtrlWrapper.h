#ifndef _ARDUINO_ESP_CONTROL_REQUEST_CLASS_H
#define _ARDUINO_ESP_CONTROL_REQUEST_CLASS_H

#include "esp_hosted_config.pb-c.h"
#include "CMsg.h"

const uint8_t REQ_WIFI_MAC_ADDRESS = (uint8_t)CTRL_MSG_ID__Req_GetMACAddress;

#define PRIO_Q_SERIAL                             0
#define PRIO_Q_BT                                 1
#define PRIO_Q_OTHERS                             2
#define MAX_PRIORITY_QUEUES                       3

/* ESP Payload Header Flags */
#define MORE_FRAGMENT                             (1 << 0)

/* Serial interface */
#define SERIAL_IF_FILE                            "/dev/esps0"





typedef enum {
   ESP_OPEN_DATA_PATH,
   ESP_CLOSE_DATA_PATH,
   ESP_RESET,
   ESP_MAX_HOST_INTERRUPT,
} ESP_HOST_INTERRUPT;


typedef enum {
   ESP_WLAN_SDIO_SUPPORT = (1 << 0),
   ESP_BT_UART_SUPPORT = (1 << 1),
   ESP_BT_SDIO_SUPPORT = (1 << 2),
   ESP_BLE_ONLY_SUPPORT = (1 << 3),
   ESP_BR_EDR_ONLY_SUPPORT = (1 << 4),
   ESP_WLAN_SPI_SUPPORT = (1 << 5),
   ESP_BT_SPI_SUPPORT = (1 << 6),
   ESP_CHECKSUM_ENABLED = (1 << 7),
} ESP_CAPABILITIES;

typedef enum {
   ESP_TEST_RAW_TP = (1 << 0),
   ESP_TEST_RAW_TP__ESP_TO_HOST = (1 << 1)
} ESP_RAW_TP_MEASUREMENT;

typedef enum {
   ESP_PACKET_TYPE_EVENT,
} ESP_PRIV_PACKET_TYPE;

typedef enum {
   ESP_PRIV_EVENT_INIT,
} ESP_PRIV_EVENT_TYPE;

typedef enum {
   ESP_PRIV_CAPABILITY,
   ESP_PRIV_SPI_CLK_MHZ,
   ESP_PRIV_FIRMWARE_CHIP_ID,
   ESP_PRIV_TEST_RAW_TP
} ESP_PRIV_TAG_TYPE;

struct esp_priv_event {
   uint8_t     event_type;
   uint8_t     event_len;
   uint8_t     event_data[0];
}__attribute__((packed));


typedef enum {
   WIFI_MODE_NONE = CTRL__WIFI_MODE__NONE,
   WIFI_MODE_STA = CTRL__WIFI_MODE__STA,
   WIFI_MODE_AP = CTRL__WIFI_MODE__AP,
   WIFI_MODE_APSTA = CTRL__WIFI_MODE__APSTA,
   WIFI_MODE_MAX
} WifiMode_t;


class CCtrlTranslate {
public:
   /* this function extracts a CtrlMsg from a received CMsg 
      the CtrlMsg obtained from this function has to be deleted! */
   static CtrlMsg *CMsg2CtrlMsg(CMsg& msg) {
      static CMsg msg_accumulator;
      CtrlMsg *rv = nullptr;
      /* need to save the more message here because msg is deleted in add_msg function
         and only the payload is copied so that the flags are lost */
      bool more_msg = msg.get_flags() & MORE_FRAGMENT;
      msg_accumulator.add_msg(msg);
      if( !more_msg) {
         /* there a not more fragment to wait for -> process the message */
         if(msg_accumulator.verify_tlv_header()) {
            /* header is correct */
            rv = ctrl_msg__unpack(NULL, msg_accumulator.get_protobuf_dim(), msg_accumulator.get_protobuf_ptr());
         }
         msg_accumulator.clear();
      }
      return rv;
   }

};


template<typename T>
class CCtrlMsgWrapper {
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
   /* ----------------------------------------------------------------------- */
   CCtrlMsgWrapper(int request_id, void (*init)(T *)) {
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
   
   /* ----------------------------------------------------------------------- */

   CMsg get_wifi_mac_address_msg(WifiMode_t mode) {
      if(payload != nullptr && mode < WIFI_MODE_MAX && mode > WIFI_MODE_NONE) {
         request.req_get_mac_address = payload;
         payload->mode = (CtrlWifiMode)mode;
      }
      return get_msg();
   }

   ~CCtrlMsgWrapper() {
      if(payload != nullptr) {
         delete payload;
      }
   }

};



#endif