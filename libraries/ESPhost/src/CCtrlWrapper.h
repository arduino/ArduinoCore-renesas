/* ########################################################################## */
/* - File: CCtrlWrapper.h
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

#ifndef _ARDUINO_ESP_CONTROL_REQUEST_CLASS_H
#define _ARDUINO_ESP_CONTROL_REQUEST_CLASS_H



/* This class wraps the original ESP-host code */


#include <vector>

#include "esp_hosted_config.pb-c.h"

#include "CMsg.h"

/* error return value */
#define ESP_CONTROL_ERROR_WHILE_PROCESS_CTRL_MSG      -11
#define ESP_CONTROL_UNABLE_TO_CONVERT_CMSG            -10
#define ESP_CONTROL_WRONG_REQUEST_INVALID_MSG          -9
#define ESP_CONTROL_MAX_WIFI_POWER_OUT_OF_RANGE        -8
#define ESP_CONTROL_ACCESS_POINT_NOT_FOUND             -7
#define ESP_CONTROL_INVALID_PASSWORD                   -6
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
#define ESP_CONTROL_ACCESS_POINT_NOT_CONNECTED          5
#define ESP_CONTROL_MSG_SENT_AND_CB_SET_UP              6


#define MAX_SSID_LENGTH              32
#define MIN_PWD_LENGTH               8
#define MAX_PWD_LENGTH               64
#define STATUS_LENGTH                14
#define TIMEOUT_PSERIAL_RESP         30
#define MIN_CHNL_NO                  1
#define MAX_CHNL_NO                  11
#define MIN_CONN_NO                  1
#define MAX_CONN_NO                  10

#define SSID_LENGTH                          32
#define MAX_MAC_STR_LEN                      18
#define BSSID_LENGTH                         MAX_MAC_STR_LEN
#define PASSWORD_LENGTH                      64
#define STATUS_LENGTH                        14
#define VENDOR_OUI_BUF                       3

#define SUCCESS_CRTLWRAPPER                              0
#define FAILURE_CRTLWRAPPER                              -1

enum {
   CTRL_ERR_NOT_CONNECTED = 1,
   CTRL_ERR_NO_AP_FOUND,
   CTRL_ERR_INVALID_PASSWORD,
   CTRL_ERR_INVALID_ARGUMENT,
   CTRL_ERR_OUT_OF_RANGE,
   CTRL_ERR_MEMORY_FAILURE,
   CTRL_ERR_UNSUPPORTED_MSG,
   CTRL_ERR_INCORRECT_ARG,
   CTRL_ERR_PROTOBUF_ENCODE,
   CTRL_ERR_PROTOBUF_DECODE,
   CTRL_ERR_SET_ASYNC_CB,
   CTRL_ERR_TRANSPORT_SEND,
   CTRL_ERR_REQUEST_TIMEOUT,
   CTRL_ERR_REQ_IN_PROG,
   OUT_OF_RANGE
};


typedef enum {

   CTRL_MSGID_INVALID = CTRL_MSG_ID__MsgId_Invalid,
   /*
    ** Request Msgs *
    */
   CTRL_REQ_BASE                      = CTRL_MSG_ID__Req_Base,
   CTRL_REQ_GET_MAC_ADDR              = CTRL_MSG_ID__Req_GetMACAddress, //0x65
   CTRL_REQ_SET_MAC_ADDR              = CTRL_MSG_ID__Req_SetMacAddress, //0x66
   CTRL_REQ_GET_WIFI_MODE             = CTRL_MSG_ID__Req_GetWifiMode,   //0x67
   CTRL_REQ_SET_WIFI_MODE             = CTRL_MSG_ID__Req_SetWifiMode,   //0x68

   CTRL_REQ_GET_AP_SCAN_LIST          = CTRL_MSG_ID__Req_GetAPScanList, //0x69
   CTRL_REQ_GET_AP_CONFIG             = CTRL_MSG_ID__Req_GetAPConfig,   //0x6a
   CTRL_REQ_CONNECT_AP                = CTRL_MSG_ID__Req_ConnectAP,     //0x6b
   CTRL_REQ_DISCONNECT_AP             = CTRL_MSG_ID__Req_DisconnectAP,  //0x6c

   CTRL_REQ_GET_SOFTAP_CONFIG         = CTRL_MSG_ID__Req_GetSoftAPConfig,            //0x6d
   CTRL_REQ_SET_SOFTAP_VND_IE         = CTRL_MSG_ID__Req_SetSoftAPVendorSpecificIE,  //0x6e
   CTRL_REQ_START_SOFTAP              = CTRL_MSG_ID__Req_StartSoftAP,                //0x6f
   CTRL_REQ_GET_SOFTAP_CONN_STA_LIST  = CTRL_MSG_ID__Req_GetSoftAPConnectedSTAList,  //0x70
   CTRL_REQ_STOP_SOFTAP               = CTRL_MSG_ID__Req_StopSoftAP,                 //0x71

   CTRL_REQ_SET_PS_MODE               = CTRL_MSG_ID__Req_SetPowerSaveMode,   //0x72
   CTRL_REQ_GET_PS_MODE               = CTRL_MSG_ID__Req_GetPowerSaveMode,   //0x73

   CTRL_REQ_OTA_BEGIN                 = CTRL_MSG_ID__Req_OTABegin,           //0x74
   CTRL_REQ_OTA_WRITE                 = CTRL_MSG_ID__Req_OTAWrite,           //0x75
   CTRL_REQ_OTA_END                   = CTRL_MSG_ID__Req_OTAEnd,             //0x76

   CTRL_REQ_SET_WIFI_MAX_TX_POWER     = CTRL_MSG_ID__Req_SetWifiMaxTxPower,  //0x77
   CTRL_REQ_GET_WIFI_CURR_TX_POWER    = CTRL_MSG_ID__Req_GetWifiCurrTxPower, //0x78

   CTRL_REQ_CONFIG_HEARTBEAT          = CTRL_MSG_ID__Req_ConfigHeartbeat,    //0x79
   /*
    * Add new control path command response before Req_Max
    * and update Req_Max
    */
   CTRL_REQ_MAX = CTRL_MSG_ID__Req_Max,
   /*
    ** Response Msgs *
    */
   CTRL_RESP_BASE                     = CTRL_MSG_ID__Resp_Base,
   CTRL_RESP_GET_MAC_ADDR             = CTRL_MSG_ID__Resp_GetMACAddress,  //0x65 -> 0xc9
   CTRL_RESP_SET_MAC_ADDRESS          = CTRL_MSG_ID__Resp_SetMacAddress,  //0x66 -> 0xca
   CTRL_RESP_GET_WIFI_MODE            = CTRL_MSG_ID__Resp_GetWifiMode,    //0x67 -> 0xcb
   CTRL_RESP_SET_WIFI_MODE            = CTRL_MSG_ID__Resp_SetWifiMode,    //0x68 -> 0xcc

   CTRL_RESP_GET_AP_SCAN_LIST         = CTRL_MSG_ID__Resp_GetAPScanList,  //0x69 -> 0xcd
   CTRL_RESP_GET_AP_CONFIG            = CTRL_MSG_ID__Resp_GetAPConfig,    //0x6a -> 0xce
   CTRL_RESP_CONNECT_AP               = CTRL_MSG_ID__Resp_ConnectAP,      //0x6b -> 0xdf
   CTRL_RESP_DISCONNECT_AP            = CTRL_MSG_ID__Resp_DisconnectAP,   //0x6c -> 0xd0

   CTRL_RESP_GET_SOFTAP_CONFIG        = CTRL_MSG_ID__Resp_GetSoftAPConfig,           //0x6d -> 0xd1
   CTRL_RESP_SET_SOFTAP_VND_IE        = CTRL_MSG_ID__Resp_SetSoftAPVendorSpecificIE, //0x6e -> 0xd2
   CTRL_RESP_START_SOFTAP             = CTRL_MSG_ID__Resp_StartSoftAP,               //0x6f -> 0xd3
   CTRL_RESP_GET_SOFTAP_CONN_STA_LIST = CTRL_MSG_ID__Resp_GetSoftAPConnectedSTAList, //0x70 -> 0xd4
   CTRL_RESP_STOP_SOFTAP              = CTRL_MSG_ID__Resp_StopSoftAP,                //0x71 -> 0xd5

   CTRL_RESP_SET_PS_MODE              = CTRL_MSG_ID__Resp_SetPowerSaveMode, //0x72 -> 0xd6
   CTRL_RESP_GET_PS_MODE              = CTRL_MSG_ID__Resp_GetPowerSaveMode, //0x73 -> 0xd7

   CTRL_RESP_OTA_BEGIN                = CTRL_MSG_ID__Resp_OTABegin,         //0x74 -> 0xd8
   CTRL_RESP_OTA_WRITE                = CTRL_MSG_ID__Resp_OTAWrite,         //0x75 -> 0xd9
   CTRL_RESP_OTA_END                  = CTRL_MSG_ID__Resp_OTAEnd,           //0x76 -> 0xda

   CTRL_RESP_SET_WIFI_MAX_TX_POWER     = CTRL_MSG_ID__Resp_SetWifiMaxTxPower,  //0x77 -> 0xdb
   CTRL_RESP_GET_WIFI_CURR_TX_POWER    = CTRL_MSG_ID__Resp_GetWifiCurrTxPower, //0x78 -> 0xdc

   CTRL_RESP_CONFIG_HEARTBEAT          = CTRL_MSG_ID__Resp_ConfigHeartbeat,    //0x79 -> 0xdd
   /*
    * Add new control path comm       and response before Resp_Max
    * and update Resp_Max
    */
   CTRL_RESP_MAX = CTRL_MSG_ID__Resp_Max,
   /*
    ** Events
    */
   CTRL_EVENT_BASE            = CTRL_MSG_ID__Event_Base,
   CTRL_EVENT_ESP_INIT        = CTRL_MSG_ID__Event_ESPInit,
   CTRL_EVENT_HEARTBEAT       = CTRL_MSG_ID__Event_Heartbeat,
   CTRL_EVENT_STATION_DISCONNECT_FROM_AP =
      CTRL_MSG_ID__Event_StationDisconnectFromAP,
   CTRL_EVENT_STATION_DISCONNECT_FROM_ESP_SOFTAP =
      CTRL_MSG_ID__Event_StationDisconnectFromESPSoftAP,
   /*
    * Add new control path command notification before Event_Max
    * and update Event_Max
    */
   CTRL_EVENT_MAX = CTRL_MSG_ID__Event_Max,
} AppMsgId_e;











typedef struct {
   int32_t reason;
   char mac[MAX_MAC_STR_LEN];
} event_station_disconn_t;











/* ESP Payload Header Flags */
#define MORE_FRAGMENT                             (1 << 0)






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


/* -------------------------------------------------------------------------- */
typedef enum {
   WIFI_PS_MIN_MODEM = CTRL__WIFI_POWER_SAVE__MIN_MODEM,
   WIFI_PS_MAX_MODEM = CTRL__WIFI_POWER_SAVE__MAX_MODEM,
   WIFI_PS_INVALID,
} PowerSave_t;



/* -------------------------------------------------------------------------- */
typedef struct {
   /* event */
   uint32_t hb_num;
   /* Req */
   uint8_t enable;
   uint32_t duration;
} HeartBeat_t;

/* -------------------------------------------------------------------------- */
typedef struct {
   /* Should be set to WIFI_VENDOR_IE_ELEMENT_ID (0xDD) */
   uint8_t element_id;
   /* Len of all bytes in the element data
    * following this field. Minimum 4 */
   uint8_t length;
   /* Vendor identifier (OUI) */
   uint8_t vendor_oui[VENDOR_OUI_BUF];
   /* Vendor-specific OUI type */
   uint8_t vendor_oui_type;
   /*length of payload field*/
   uint16_t payload_len;
   /* Payload. Length is equal to value in 'length' field, minus 4 */
   uint8_t* payload;
} VendorIeData_t;

/* -------------------------------------------------------------------------- */
typedef enum {
   WIFI_VND_IE_TYPE_BEACON      = CTRL__VENDOR_IETYPE__Beacon,
   WIFI_VND_IE_TYPE_PROBE_REQ   = CTRL__VENDOR_IETYPE__Probe_req,
   WIFI_VND_IE_TYPE_PROBE_RESP  = CTRL__VENDOR_IETYPE__Probe_resp,
   WIFI_VND_IE_TYPE_ASSOC_REQ   = CTRL__VENDOR_IETYPE__Assoc_req,
   WIFI_VND_IE_TYPE_ASSOC_RESP  = CTRL__VENDOR_IETYPE__Assoc_resp,
} VendorIeType_t;

/* -------------------------------------------------------------------------- */
typedef enum {
   WIFI_VND_IE_ID_0 = CTRL__VENDOR_IEID__ID_0,
   WIFI_VND_IE_ID_1 = CTRL__VENDOR_IEID__ID_1,
} VendorIeId_t;

/* -------------------------------------------------------------------------- */
typedef struct {
   bool enable;
   VendorIeType_t type;
   VendorIeId_t idx;
   VendorIeData_t vnd_ie;
} WifiVendorSoftApIe_t;

/* -------------------------------------------------------------------------- */
typedef struct {
   uint8_t bssid[BSSID_LENGTH];
   int rssi;
} WifiConnectedSta_t;

/* -------------------------------------------------------------------------- */
typedef enum {
   WIFI_BW_HT20 = CTRL__WIFI_BW__HT20,
   WIFI_BW_HT40 = CTRL__WIFI_BW__HT40,
} WifiBandwidth_t;

/* -------------------------------------------------------------------------- */
typedef enum {
   WIFI_AUTH_OPEN = CTRL__WIFI_SEC_PROT__Open,
   WIFI_AUTH_WEP = CTRL__WIFI_SEC_PROT__WEP,
   WIFI_AUTH_WPA_PSK = CTRL__WIFI_SEC_PROT__WPA_PSK,
   WIFI_AUTH_WPA2_PSK = CTRL__WIFI_SEC_PROT__WPA2_PSK,
   WIFI_AUTH_WPA_WPA2_PSK = CTRL__WIFI_SEC_PROT__WPA_WPA2_PSK,
   WIFI_AUTH_WPA2_ENTERPRISE = CTRL__WIFI_SEC_PROT__WPA2_ENTERPRISE,
   WIFI_AUTH_WPA3_PSK = CTRL__WIFI_SEC_PROT__WPA3_PSK,
   WIFI_AUTH_WPA2_WPA3_PSK = CTRL__WIFI_SEC_PROT__WPA2_WPA3_PSK,
   WIFI_AUTH_MAX,
} WifiEnryption_t;


/* -------------------------------------------------------------------------- */
typedef struct {
   uint8_t ssid[SSID_LENGTH];
   uint8_t pwd[PASSWORD_LENGTH];
   int channel;
   WifiEnryption_t encryption_mode;
   int max_connections;
   bool ssid_hidden;
   WifiBandwidth_t bandwidth;
   char out_mac[MAX_MAC_STR_LEN];
} SoftApCfg_t;

/* -------------------------------------------------------------------------- */
typedef struct {
   uint8_t *ota_data;
   uint32_t ota_data_len;
} OtaWrite_t;

/* -------------------------------------------------------------------------- */
typedef struct {
   uint8_t ssid[SSID_LENGTH];
   uint8_t bssid[BSSID_LENGTH];
   int rssi;
   int channel;
   int encryption_mode;
} AccessPoint_t;

/* -------------------------------------------------------------------------- */
typedef struct {
   uint8_t ssid[SSID_LENGTH];
   uint8_t pwd[PASSWORD_LENGTH];
   uint8_t bssid[BSSID_LENGTH];
   bool is_wpa3_supported;
   int rssi;
   int channel;
   int encryption_mode;
   uint16_t listen_interval;
   char status[STATUS_LENGTH];
   char out_mac[MAX_MAC_STR_LEN];
} WifiApCfg_t;

/* -------------------------------------------------------------------------- */
typedef enum {
   WIFI_MODE_NONE = CTRL__WIFI_MODE__NONE,
   WIFI_MODE_STA = CTRL__WIFI_MODE__STA,
   WIFI_MODE_AP = CTRL__WIFI_MODE__AP,
   WIFI_MODE_APSTA = CTRL__WIFI_MODE__APSTA,
   WIFI_MODE_MAX
} WifiMode_t;

/* -------------------------------------------------------------------------- */
typedef struct {
   WifiMode_t mode;
   char mac[MAX_MAC_STR_LEN];
} WifiMac_t;

/* -------------------------------------------------------------------------- */
template<typename T>
static int checkResponsePayload(CtrlMsg *ans, int req, T *payload, bool check_resp = false) {
/* -------------------------------------------------------------------------- */   
   
   if(ans == nullptr) {
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }
   if(ans->msg_id != (CtrlMsgId)req) {
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }
   if(payload == nullptr) {
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }
   if(check_resp) {
      if(payload->resp != 0) {
         return ESP_CONTROL_OK;
      }
      else {
         return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
      }
   }
   return ESP_CONTROL_OK;
}


/* -------------------------------------------------------------------------- */
template<typename T>
static int checkEventPayload(CtrlMsg *ans, int req, T *payload) {
/* -------------------------------------------------------------------------- */   
   
   if(ans == nullptr) {
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }
   if(ans->msg_id != (CtrlMsgId)req) {
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }
   if(payload == nullptr) {
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }
   return ESP_CONTROL_OK;
}


/* -------------------------------------------------------------------------- */
class CCtrlMsgWrapper {
/* -------------------------------------------------------------------------- */   
private:
   
   CtrlMsg request;
   CtrlMsg *answer;
   bool payload_set;
   
   void init_ctrl_msg(int request_id) {
      /* init the protobuf request*/
      memset(&request,0x00,sizeof(request));
      ctrl_msg__init(&request);
      /* set the id of the message and the payload case */
      request.msg_id = (CtrlMsgId)request_id;
      request.payload_case = (CtrlMsg__PayloadCase)request_id;

      payload_set = true;
   }

   void copyData(uint8_t *dst, int dst_len, uint8_t *src, int scr_len ) {
      if(src != nullptr && dst != nullptr && dst_len > 0 && scr_len > 0) {
         memset(dst,0x00,dst_len);
         uint32_t len = (dst_len < scr_len) ? dst_len : scr_len;
         memcpy(dst,src,len);
      }
   }

public:
   /* ----------------------------------------------------------------------- */
   void setRequestId(int request_id) {
   /* ----------------------------------------------------------------------- */   
      init_ctrl_msg(request_id);
   }
   /* ----------------------------------------------------------------------- */
   CtrlMsgType getType() {
   /* ----------------------------------------------------------------------- */  
      if(answer != nullptr) {
         return answer->msg_type;
      }
      return CTRL_MSG_TYPE__MsgType_Invalid;
   }

   /* ----------------------------------------------------------------------- */
   CtrlMsgId getId() {
   /* ----------------------------------------------------------------------- */   
      if(answer != nullptr) {
         return answer->msg_id;
      }
      return CTRL_MSG_ID__MsgId_Invalid;
   }

   /* get a msg to be sent by a network interface, i.e. the network send buf */
   /* ----------------------------------------------------------------------- */
   CMsg getNetIfMsg(ESP_INTERFACE_TYPE type, uint8_t num,uint8_t *buf, uint16_t dim) {
   /* ----------------------------------------------------------------------- */
      /* create a message without the TLV header, besides this message will not
         manage a CtrlMsg, neither pack it into a protobuf */
      CMsg msg(dim, false);
      if(msg.is_valid()) {
         /* copy the buffer into the "protobuf" place as data */
         memcpy(msg.data(), buf, dim);
         msg.set_payload_header(type, num);
         return msg;
      }

      return CMsg(0,false);
   }

   /* ----------------------------------------------------------------------- */
   bool extractFromMsg(CMsg& msg) {
   /* ----------------------------------------------------------------------- */   
      static CMsg msg_accumulator;
      bool rv = false;
      /* need to save the more message here because msg is deleted in add_msg function
         and only the payload is copied so that the flags are lost */
      bool more_msg = msg.get_flags() & MORE_FRAGMENT;
      msg_accumulator.add_msg(msg);
      if( !more_msg) {
         /* there a not more fragment to wait for -> process the message */
         if(msg_accumulator.verify_tlv_header()) {
            /* header is correct */
            answer = ctrl_msg__unpack(NULL, msg_accumulator.get_protobuf_dim(), msg_accumulator.get_protobuf_ptr());
            rv = true;
         }
         msg_accumulator.clear();
      }
      return rv;
   }
   /* ----------------------------------------------------------------------- */
   CCtrlMsgWrapper()  :  answer(nullptr) { }
   /* ----------------------------------------------------------------------- */
   CCtrlMsgWrapper(int request_id) : answer(nullptr) {
   /* ----------------------------------------------------------------------- */   
      init_ctrl_msg(request_id);
   }
   /* ----------------------------------------------------------------------- */
   ~CCtrlMsgWrapper() {
   /* ----------------------------------------------------------------------- */
      if(answer != nullptr) {
         //Serial.println("THE answer has been FREE_UNPACKED!");
         ctrl_msg__free_unpacked(answer, NULL);
      }
   }

   /* ======================================================================= 
    * The following functions are ment to form a CMsg that will be used to 
    * send the request to be transmitted to the ESP
    * ======================================================================= */

   /* ----------------------------------------------------------------------- */
   CMsg getMsg() {
   /* ----------------------------------------------------------------------- */   
      int protobuf_len = ctrl_msg__get_packed_size(&request);
      CMsg msg(protobuf_len);
      if(msg.is_valid() && payload_set){
         /* pack request into the message */
         ctrl_msg__pack(&request, msg.get_protobuf_ptr());
         msg.set_tlv_header(CTRL_EP_NAME_RESP);
         msg.set_payload_header(ESP_SERIAL_IF, 0);
         return msg;
      }
      return CMsg(0);
   }

   /* ----------------------------------------------------------------------- */
   CMsg getWifiMacAddressMsg(WifiMode_t mode) {
   /* ----------------------------------------------------------------------- */
      payload_set = false;
      CtrlMsgReqGetMacAddress payload;
      ctrl_msg__req__get_mac_address__init(&payload);
      if(mode < WIFI_MODE_MAX && mode > WIFI_MODE_NONE) {
         request.req_get_mac_address = &payload;
         payload.mode = (CtrlWifiMode)mode;
         payload_set = true;
      }
      return getMsg();
   }
   
   /* ----------------------------------------------------------------------- */
   CMsg setWifiMacAddressMsg(WifiMode_t mode, const char *mac, uint8_t mac_len) {
   /* ----------------------------------------------------------------------- */
      payload_set = false;
      CtrlMsgReqSetMacAddress payload;
      ctrl_msg__req__set_mac_address__init(&payload);
      if(mode < WIFI_MODE_MAX && mode > WIFI_MODE_NONE 
         && mac != nullptr && mac_len < MAX_MAC_STR_LEN) {
         request.req_set_mac_address = &payload;
         payload.mode = (CtrlWifiMode)mode;
         payload.mac.len = (mac_len < MAX_MAC_STR_LEN) ? mac_len : MAX_MAC_STR_LEN;
         payload.mac.data = (uint8_t *)mac;
         payload_set = true;
      }
      return getMsg();
   }

   /* ----------------------------------------------------------------------- */
   CMsg setWifiModeMsg(WifiMode_t mode) {
   /* ----------------------------------------------------------------------- */
      payload_set = false;
      CtrlMsgReqSetMode payload;
      ctrl_msg__req__set_mode__init(&payload);
      if(mode < WIFI_MODE_MAX && mode > WIFI_MODE_NONE) {
         request.req_set_wifi_mode = &payload;
         payload.mode = (CtrlWifiMode)mode;
         payload_set = true;
      }
      return getMsg();
   }
   
   /* ----------------------------------------------------------------------- */
   CMsg setPowerSaveModeMsg(PowerSave_t power_save_mode) {
   /* ----------------------------------------------------------------------- */
      payload_set = false;
      CtrlMsgReqSetMode payload;
      ctrl_msg__req__set_mode__init(&payload);
      if(power_save_mode < WIFI_PS_INVALID && power_save_mode >= WIFI_PS_MIN_MODEM) {
         request.req_set_power_save_mode = &payload;
         payload.mode = (CtrlWifiPowerSave)power_save_mode;
         payload_set = true;
      }
      return getMsg();
   }
   
   /* ----------------------------------------------------------------------- */
   CMsg otaWriteMsg(OtaWrite_t &ow) {
   /* ----------------------------------------------------------------------- */
      payload_set = false;
      CtrlMsgReqOTAWrite payload;
      ctrl_msg__req__otawrite__init(&payload);   
      request.req_ota_write = &payload;
      payload.ota_data.data = ow.ota_data;
      payload.ota_data.len = ow.ota_data_len;
      payload_set = true;
      return getMsg();
   }


   /* ----------------------------------------------------------------------- */
   CMsg getConnectAccessPointMsg(const char *ssid, const char *pwd, const char *bssid, bool wpa3_support, uint32_t interval) {
   /* ----------------------------------------------------------------------- */
      payload_set = false;
      CtrlMsgReqConnectAP payload;
      ctrl_msg__req__connect_ap__init(&payload);
      request.req_connect_ap = &payload;
      if(ssid != nullptr && strlen(ssid) <=  MAX_SSID_LENGTH &&
         pwd !=nullptr && strlen(pwd) <= MAX_PWD_LENGTH &&
         bssid != nullptr && strlen(bssid) <= MAX_MAC_STR_LEN) {
         payload.ssid  = (char *)ssid;
         payload.pwd   = (char *)pwd;
         payload.bssid = (char *)bssid;
         payload.is_wpa3_supported = wpa3_support;
         payload.listen_interval = interval;
         payload_set = true;
      }
      return getMsg();
   }

   /* ----------------------------------------------------------------------- */
   CMsg setWifiMaxTxPowerMsg(uint32_t max_power) {
   /* ----------------------------------------------------------------------- */
      CtrlMsgReqSetWifiMaxTxPower payload;
      ctrl_msg__req__set_wifi_max_tx_power__init(&payload);
      request.req_set_wifi_max_tx_power = &payload;
      payload.wifi_max_tx_power = max_power;
      payload_set = true;
      
      return getMsg();
   }
   
   /* ----------------------------------------------------------------------- */
   CMsg setSoftAccessPointVndIeMsg(WifiVendorSoftApIe_t &vendor_ie) {
   /* ----------------------------------------------------------------------- */
      payload_set = false;
      CtrlMsgReqSetSoftAPVendorSpecificIE payload;
      ctrl_msg__req__set_soft_apvendor_specific_ie__init(&payload);
      
      if(vendor_ie.type <= WIFI_VND_IE_TYPE_ASSOC_RESP && 
         /*vendor_ie.type >= WIFI_VND_IE_TYPE_BEACON && */ 
         vendor_ie.idx <= WIFI_VND_IE_ID_1 && 
         /* vendor_ie.idx >= WIFI_VND_IE_ID_0 && */
         vendor_ie.vnd_ie.payload != nullptr) {
         
         request.req_set_softap_vendor_specific_ie = &payload;

         payload.enable = vendor_ie.enable;
         payload.type = (CtrlVendorIEType)vendor_ie.type;
         payload.idx = (CtrlVendorIEID)vendor_ie.idx;
         CtrlMsgReqVendorIEData vendor_data;
         ctrl_msg__req__vendor_iedata__init(&vendor_data);
         payload.vendor_ie_data = &vendor_data;
         payload.vendor_ie_data->element_id = vendor_ie.vnd_ie.element_id;
         payload.vendor_ie_data->length = vendor_ie.vnd_ie.length;
         payload.vendor_ie_data->vendor_oui.data = vendor_ie.vnd_ie.vendor_oui;
         payload.vendor_ie_data->vendor_oui.len = VENDOR_OUI_BUF;
         payload.vendor_ie_data->payload.data = vendor_ie.vnd_ie.payload;
         payload.vendor_ie_data->payload.len = vendor_ie.vnd_ie.payload_len;
         payload_set = true;  
      }
      return getMsg();
      
   }

   /* ----------------------------------------------------------------------- */
   CMsg startSoftAccessPointMsg(SoftApCfg_t &cfg) {
   /* ----------------------------------------------------------------------- */
      payload_set = false;
      CtrlMsgReqStartSoftAP payload;
      ctrl_msg__req__start_soft_ap__init(&payload);
      

      bool cfg_ok = true;

      if(strlen((char *)&cfg.ssid) > MAX_SSID_LENGTH  ||
         strlen((char *)&cfg.ssid) == 0 ) {
         Serial.println("[ERROR]: Invalid SSID");
         /* INVALID SSID */
         cfg_ok = false;
      }

      if((cfg.encryption_mode != WIFI_AUTH_OPEN) &&
             ((strlen((char *)&cfg.pwd) > MAX_PWD_LENGTH) ||
              (strlen((char *)&cfg.pwd) < MIN_PWD_LENGTH)) ) {
         /* INVALID BASS*/
         Serial.println("[ERROR]: Invalid password");
         cfg_ok = false;
      }
      if((cfg.channel < MIN_CHNL_NO) ||
              (cfg.channel > MAX_CHNL_NO)) {
         /* INVALID CHANNEL */
         Serial.println("[ERROR]: Invalid channel ");
         cfg_ok = false;
      }
      if(/*( cfg.encryption_mode < WIFI_AUTH_OPEN) || */
             (cfg.encryption_mode == WIFI_AUTH_WEP) ||
             (cfg.encryption_mode > WIFI_AUTH_WPA_WPA2_PSK)) {
         Serial.println("[ERROR]: Invalid encryption");

         /* ENCRYPTION NOT SUPPORTED */
         cfg_ok = false;
      }
      if((cfg.max_connections < MIN_CONN_NO) ||
             (cfg.max_connections > MAX_CONN_NO)) {
         /* INVALID CONNECTION NUMBER */
         Serial.println("[ERROR]: Invalid connection number");
         cfg_ok = false;
      }
      if((cfg.bandwidth < WIFI_BW_HT20) ||
              (cfg.bandwidth > WIFI_BW_HT40)) {
         Serial.println("[ERROR]: Invalid bandwith");
         /* INVALID BANDWIDTH */
         cfg_ok = false;
      }
      
         /* ... all seems to be correct ... */
      if(cfg_ok) {
         request.req_start_softap = &payload;

         payload.ssid = (char *)&cfg.ssid;
         payload.pwd = (char *)(char *)&cfg.pwd;
         payload.chnl = cfg.channel;
         payload.sec_prot = (CtrlWifiSecProt)cfg.encryption_mode;
         payload.max_conn = cfg.max_connections;
         payload.ssid_hidden = cfg.ssid_hidden;
         payload.bw = cfg.bandwidth;  

         payload_set = true; 
      }

      return getMsg();
   }
   
   /* ----------------------------------------------------------------------- */
   CMsg  configureHeartbeatMsg(bool enable, int32_t duration) {
   /* ----------------------------------------------------------------------- */   
      CtrlMsgReqConfigHeartbeat payload;
      ctrl_msg__req__config_heartbeat__init(&payload);
      request.req_config_heartbeat = &payload;
      payload.enable = enable;
      payload.duration = duration;
      payload_set = true; 
      return getMsg();
   }


   /* ======================================================================= 
    * The following functions are ment to extract information from CtrlMsg
    * received from ESP
    * ======================================================================= */

   /* ----------------------------------------------------------------------- */
   int extractMacAddress(char *mac_out, int mac_out_dim) {
   /* ----------------------------------------------------------------------- */   
      int rv = checkResponsePayload<CtrlMsgRespGetMacAddress>(answer, 
                                                        (int)CTRL_RESP_GET_MAC_ADDR, 
                                                        answer->resp_get_mac_address);
      if(rv == ESP_CONTROL_OK) {
         
         if(answer->resp_get_mac_address->mac.data != nullptr) {
            copyData((uint8_t *)mac_out, 
                     mac_out_dim, 
                     answer->resp_get_mac_address->mac.data, 
                     answer->resp_get_mac_address->mac.len);
         }
         else {
            rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
         }
      }
      return rv;
   }
   /* ----------------------------------------------------------------------- */
   int checkMacAddressSet() {
   /* ----------------------------------------------------------------------- */   
      
      return checkResponsePayload<CtrlMsgRespSetMacAddress>(answer, 
                                                           (int)CTRL_RESP_SET_MAC_ADDRESS, 
                                                            answer->resp_set_mac_address);
   }

   /* ----------------------------------------------------------------------- */
   int extractWifiMode(WifiMode_t &mode) {
   /* ----------------------------------------------------------------------- */   
      
      if(checkResponsePayload<CtrlMsgRespGetMode>(answer, 
                                                 (int)CTRL_RESP_GET_WIFI_MODE, 
                                                 answer->resp_get_wifi_mode) == ESP_CONTROL_OK) {
         mode = (WifiMode_t)answer->resp_get_wifi_mode->mode;
         return ESP_CONTROL_OK;
      }
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }
   
   /* ----------------------------------------------------------------------- */
   int isSoftAccessPointStarted(SoftApCfg_t &sap_cfg) {     
   /* ----------------------------------------------------------------------- */ 
      
      if(checkResponsePayload<CtrlMsgRespStartSoftAP>(answer, 
                                                     (int)CTRL_RESP_START_SOFTAP, 
                                                     answer->resp_start_softap)  == ESP_CONTROL_OK ) {
         
         copyData((uint8_t*)sap_cfg.out_mac, 
                  MAX_MAC_STR_LEN, 
                  answer->resp_start_softap->mac.data,  
                  answer->resp_start_softap->mac.len );
         return ESP_CONTROL_OK;
      }
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }
   
   /* ----------------------------------------------------------------------- */
   int isSoftAccessPointVndIeSet() {
   /* ----------------------------------------------------------------------- */   
      
      return checkResponsePayload<CtrlMsgRespSetSoftAPVendorSpecificIE>(answer, 
                                                                       (int)CTRL_RESP_SET_SOFTAP_VND_IE, 
                                                                       answer->resp_set_softap_vendor_specific_ie);
   }
   
   /* ----------------------------------------------------------------------- */
   int isSetWifiModeResponse() {
   /* ----------------------------------------------------------------------- */   
      
      return checkResponsePayload<CtrlMsgRespSetMode>(answer, 
                                                     (int)CTRL_RESP_SET_WIFI_MODE, 
                                                     answer->resp_set_wifi_mode);
   }

   /* ----------------------------------------------------------------------- */
   int isAccessPointDisconnected() {
   /* ----------------------------------------------------------------------- */   
      
      return checkResponsePayload<CtrlMsgRespGetStatus>(answer, 
                                                       (int)CTRL_RESP_DISCONNECT_AP, 
                                                       answer->resp_disconnect_ap);  
   }
   
   /* ----------------------------------------------------------------------- */
   int isPowerSaveModeSet() {
   /* ----------------------------------------------------------------------- */   
      
      return checkResponsePayload<CtrlMsgRespSetMode>(answer, 
                                                     (int)CTRL_RESP_SET_PS_MODE, 
                                                     answer->resp_set_power_save_mode);  
   }

   /* ----------------------------------------------------------------------- */
   int getOtaWriteResult() {
   /* ----------------------------------------------------------------------- */   
      
      return checkResponsePayload<CtrlMsgRespOTAWrite>(answer, 
                                                      (int)CTRL_RESP_OTA_WRITE, 
                                                      answer->resp_ota_write);  
   }
   
   /* ----------------------------------------------------------------------- */
   int isOtaBegun() {
   /* ----------------------------------------------------------------------- */   
      
      return checkResponsePayload<CtrlMsgRespOTABegin>(answer, 
                                                      (int)CTRL_RESP_OTA_BEGIN, 
                                                      answer->resp_ota_begin);
   }
   
   /* ----------------------------------------------------------------------- */
   int isOtaEnded() {
   /* ----------------------------------------------------------------------- */   
     
      return checkResponsePayload<CtrlMsgRespOTAEnd>(answer, 
                                                    (int)CTRL_RESP_OTA_END, 
                                                    answer->resp_ota_end);
   }
   
   /* ----------------------------------------------------------------------- */
   int isSoftAccessPointStopped() {
   /* ----------------------------------------------------------------------- */   
      
      return checkResponsePayload<CtrlMsgRespGetStatus>(answer, 
                                                       (int)CTRL_RESP_STOP_SOFTAP, 
                                                       answer->resp_stop_softap);
   }
   
   /* ----------------------------------------------------------------------- */
   bool isHeartbeatConfigured() {
   /* ----------------------------------------------------------------------- */   
      
      return checkResponsePayload<CtrlMsgRespConfigHeartbeat>(answer, 
                                                             (int)CTRL_RESP_CONFIG_HEARTBEAT, 
                                                             answer->resp_config_heartbeat);
   }

   /* ----------------------------------------------------------------------- */
   int extractCurrentWifiTxPower( uint32_t &max_power) {
   /* ----------------------------------------------------------------------- */   
     
      if(checkResponsePayload<CtrlMsgRespGetWifiCurrTxPower>(answer, 
                                                            (int)CTRL_RESP_GET_WIFI_CURR_TX_POWER, 
                                                            answer->resp_get_wifi_curr_tx_power)  == ESP_CONTROL_OK ) {
         max_power = answer->resp_get_wifi_curr_tx_power->wifi_curr_tx_power;
         return ESP_CONTROL_OK;
      }
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }

   /* ----------------------------------------------------------------------- */
   int isWifiMaxPowerSet() {
   /* ----------------------------------------------------------------------- */   
      
      int rv = ESP_CONTROL_CTRL_ERROR;
      if(checkResponsePayload<CtrlMsgRespSetWifiMaxTxPower>(answer, 
                                                           (int)CTRL_RESP_SET_WIFI_MAX_TX_POWER, 
                                                            answer->resp_set_wifi_max_tx_power) == ESP_CONTROL_OK ) {
         if(answer->resp_set_wifi_max_tx_power->resp == FAILURE_CRTLWRAPPER) {
            rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
         }
         else if(answer->resp_set_wifi_max_tx_power->resp == CTRL_ERR_OUT_OF_RANGE) {
            rv = ESP_CONTROL_MAX_WIFI_POWER_OUT_OF_RANGE;
         }
         else {
            rv = ESP_CONTROL_OK;
         }
      }
      return rv;
   }
   
   /* ----------------------------------------------------------------------- */
   int getPowerSaveModeSet(int &power_save_mode) {
   /* ----------------------------------------------------------------------- */   
      
      if(checkResponsePayload<CtrlMsgRespGetMode>(answer, 
                                                 (int)CTRL_RESP_GET_PS_MODE, 
                                                 answer->resp_get_power_save_mode) == ESP_CONTROL_OK ) {
         power_save_mode = answer->resp_get_power_save_mode->mode;
         return ESP_CONTROL_OK;
      }
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }
   
   /* ----------------------------------------------------------------------- */
   int extractSoftConnectedStationList(std::vector<WifiConnectedSta_t>& l) {
   /* ----------------------------------------------------------------------- */   
      
      if(checkResponsePayload<CtrlMsgRespSoftAPConnectedSTA>(answer, 
                                                            (int)CTRL_RESP_GET_SOFTAP_CONN_STA_LIST, 
                                                            answer->resp_softap_connected_stas_list)== ESP_CONTROL_OK ) {

         CtrlMsgRespSoftAPConnectedSTA *rp = answer->resp_softap_connected_stas_list;

         for(unsigned int i = 0; i < rp->num; i++) {
            WifiConnectedSta_t cs;
            memset((void *)&cs,0x00,sizeof(cs));
            copyData((uint8_t *)&(cs.bssid), BSSID_LENGTH, rp->stations[i]->mac.data, rp->stations[i]->mac.len );
            cs.rssi = rp->stations[i]->rssi;  
            l.push_back(cs);
         }
         return ESP_CONTROL_OK;
      }
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }

   /* ----------------------------------------------------------------------- */
   int extractSoftAccessPointConfig(SoftApCfg_t &sap_cfg) {
   /* ----------------------------------------------------------------------- */   
      
      if(checkResponsePayload<CtrlMsgRespGetSoftAPConfig>(answer, 
                                                         (int)CTRL_RESP_GET_SOFTAP_CONFIG, 
                                                         answer->resp_get_softap_config) == ESP_CONTROL_OK ) {
         copyData((uint8_t *)&(sap_cfg.ssid), 
                  SSID_LENGTH, 
                  answer->resp_get_softap_config->ssid.data, 
                  answer->resp_get_softap_config->ssid.len );
         copyData((uint8_t *)&(sap_cfg.pwd), 
                  PASSWORD_LENGTH, 
                  answer->resp_get_softap_config->pwd.data, 
                  answer->resp_get_softap_config->pwd.len );
         sap_cfg.channel = answer->resp_get_softap_config->chnl;
         sap_cfg.encryption_mode = (WifiEnryption_t)answer->resp_get_softap_config->sec_prot;
         sap_cfg.max_connections = answer->resp_get_softap_config->max_conn;
         sap_cfg.ssid_hidden = answer->resp_get_softap_config->ssid_hidden;
         sap_cfg.bandwidth = (WifiBandwidth_t)answer->resp_get_softap_config->bw;
         return ESP_CONTROL_OK;
      }
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;

   }
   
   /* ----------------------------------------------------------------------- */
   int extractAccessPointConfig(WifiApCfg_t &ap) {
   /* ----------------------------------------------------------------------- */   
      
      int rv = ESP_CONTROL_CTRL_ERROR;
      if(checkResponsePayload<CtrlMsgRespGetAPConfig>(answer, 
                                                     (int)CTRL_RESP_GET_AP_CONFIG, 
                                                      answer->resp_get_ap_config, false) == ESP_CONTROL_OK  ){
         if(answer->resp_get_ap_config->resp == CTRL_ERR_NOT_CONNECTED) {
            rv = ESP_CONTROL_ACCESS_POINT_NOT_CONNECTED;
         }
         else if(answer->resp_get_ap_config->resp == 0) {
            copyData((uint8_t *)&(ap.ssid), 
                     SSID_LENGTH, 
                     answer->resp_get_ap_config->ssid.data, 
                     SSID_LENGTH );
            copyData((uint8_t *)&(ap.bssid), 
                     BSSID_LENGTH, 
                     answer->resp_get_ap_config->bssid.data, 
                     BSSID_LENGTH );
            copyData((uint8_t *)&(ap.status), 
                     STATUS_LENGTH, 
                     (uint8_t *)"SUCCESS", 
                     strlen("SUCCESS") );
            ap.channel         = answer->resp_get_ap_config->chnl;
            ap.rssi            = answer->resp_get_ap_config->rssi;
            ap.encryption_mode = answer->resp_get_ap_config->sec_prot;
            rv = ESP_CONTROL_OK;
         }
         else {
            copyData((uint8_t *)&(ap.status), 
                     STATUS_LENGTH, 
                     (uint8_t *)"FAILURE", 
                     strlen("FAILURE") );
            rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
         }
      }
      return rv;
   }

   /* ----------------------------------------------------------------------- */
   int isAccessPointConnected(WifiApCfg_t &ap) {
   /* ----------------------------------------------------------------------- */
      
      int rv = ESP_CONTROL_CTRL_ERROR;
      if(checkResponsePayload<CtrlMsgRespConnectAP>(answer, 
                                                   (int)CTRL_RESP_CONNECT_AP, 
                                                   answer->resp_connect_ap, 
                                                   false) == ESP_CONTROL_OK) {

         if(answer->resp_connect_ap->resp == CTRL_ERR_INVALID_PASSWORD) {
            rv = ESP_CONTROL_INVALID_PASSWORD;
         }
         else if(answer->resp_connect_ap->resp == CTRL_ERR_NO_AP_FOUND) {
            rv = ESP_CONTROL_ACCESS_POINT_NOT_FOUND;
         }
         else if(answer->resp_connect_ap->resp != 0) {
            rv = ESP_CONTROL_CTRL_ERROR;
         }
         else {
            if(answer->resp_connect_ap->mac.data != nullptr) {
               copyData((uint8_t *)&(ap.out_mac), 
                        MAX_MAC_STR_LEN, 
                        (uint8_t *)answer->resp_connect_ap->mac.data, 
                        answer->resp_connect_ap->mac.len );
               rv = ESP_CONTROL_OK;
            }
            else {
               rv = ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
            }
         }
      }
      return rv;
   }


   /* ----------------------------------------------------------------------- */
   int extractAccessPointList(std::vector<AccessPoint_t>& l) {
   /* ----------------------------------------------------------------------- */
      
      if(checkResponsePayload<CtrlMsgRespScanResult>(answer, 
                                                    (int)CTRL_RESP_GET_AP_SCAN_LIST, 
                                                    answer->resp_scan_ap_list) == ESP_CONTROL_OK ) {
         CtrlMsgRespScanResult *rp = answer->resp_scan_ap_list;
         for(unsigned int i = 0; i < rp->count; i++) {
            AccessPoint_t sc;
            memset((void *)&sc,0x00,sizeof(sc));

            if(rp->entries[i]->ssid.len) {
               memcpy(sc.ssid,
                     (char *)rp->entries[i]->ssid.data, 
                     (rp->entries[i]->ssid.len < SSID_LENGTH) ? rp->entries[i]->ssid.len : SSID_LENGTH);
            }

            if(rp->entries[i]->bssid.len) {
               memcpy(sc.bssid,(char *)rp->entries[i]->bssid.data, (rp->entries[i]->bssid.len < BSSID_LENGTH) ? rp->entries[i]->bssid.len : BSSID_LENGTH);
            }

            sc.channel = rp->entries[i]->chnl;
            sc.rssi = rp->entries[i]->rssi;
            sc.encryption_mode = rp->entries[i]->sec_prot;

            l.push_back(sc);
         }
         return ESP_CONTROL_OK;
      }
      return ESP_CONTROL_ERROR_UNABLE_TO_PARSE_RESPONSE;
   }
   
   /* ----------------------------------------------------------------------- */
   int isInitEventReceived() {
   /* ----------------------------------------------------------------------- */   
      return checkEventPayload<CtrlMsgEventESPInit>(answer, 
                                                       (int)CTRL_EVENT_ESP_INIT, 
                                                       answer->event_esp_init);
   }
   
   /* ----------------------------------------------------------------------- */
   int isHeartBeatEventReceived(int &hb) {
   /* ----------------------------------------------------------------------- */   
      int rv = checkEventPayload<CtrlMsgEventHeartbeat>(answer, 
                                                        (int)CTRL_EVENT_HEARTBEAT, 
                                                         answer->event_heartbeat);
      if(rv == ESP_CONTROL_OK) {
         hb = answer->event_heartbeat->hb_num;
      }
      return rv;
   }

   /* ----------------------------------------------------------------------- */
   int isStationDisconnectionEventReceived(int &reason) {
   /* ----------------------------------------------------------------------- */   
      int rv = checkEventPayload<CtrlMsgEventStationDisconnectFromAP>(answer, 
                                                          (int)CTRL_EVENT_STATION_DISCONNECT_FROM_AP, 
                                                           answer->event_station_disconnect_from_ap);
      if(rv == ESP_CONTROL_OK) {
         reason = answer->event_station_disconnect_from_ap->resp;
      }
      return rv;
   }
   
   /* ----------------------------------------------------------------------- */
   int isDisconnectionFromSoftApReceived(char *mac_out, int mac_out_dim) {
   /* ----------------------------------------------------------------------- */   
      int rv = checkEventPayload<CtrlMsgEventStationDisconnectFromESPSoftAP>(answer, 
                                    (int)CTRL_EVENT_STATION_DISCONNECT_FROM_ESP_SOFTAP, 
                                    answer->event_station_disconnect_from_esp_softap);

      if(rv == ESP_CONTROL_OK) {
         if(answer->event_station_disconnect_from_esp_softap->resp == SUCCESS_CRTLWRAPPER) {     
            copyData((uint8_t *)mac_out, 
                     mac_out_dim, 
                     answer->event_station_disconnect_from_esp_softap->mac.data, 
                     answer->event_station_disconnect_from_esp_softap->mac.len);
         }
      }
      return rv;
   }

};



#endif