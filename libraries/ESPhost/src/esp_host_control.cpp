#include "esp_host_control.h"




#define MAX_INTERFACE_LEN            IFNAMSIZ
#define MAC_SIZE_BYTES               6
#define MIN_MAC_STR_LEN              17


#define SUCCESS                      0
#define FAILURE                      -1
#define MAX_SSID_LENGTH              32
#define MIN_PWD_LENGTH               8
#define MAX_PWD_LENGTH               64
#define STATUS_LENGTH                14
#define TIMEOUT_PSERIAL_RESP         30
#define MIN_CHNL_NO                  1
#define MAX_CHNL_NO                  11
#define MIN_CONN_NO                  1
#define MAX_CONN_NO                  10


#define CTRL_LIB_STATE_INACTIVE      0
#define CTRL_LIB_STATE_INIT          1
#define CTRL_LIB_STATE_READY         2


#define CLEANUP_APP_MSG(app_msg) do {                                         \
  if (app_msg) {                                                              \
    if (app_msg->free_buffer_handle) {                                        \
      if (app_msg->free_buffer_func) {                                        \
        app_msg->free_buffer_func(app_msg->free_buffer_handle);               \
        app_msg->free_buffer_handle = NULL;                                   \
      }                                                                       \
    }                                                                         \
    mem_free(app_msg);                                                        \
  }                                                                           \
} while(0);


#define CHECK_CTRL_MSG_NON_NULL_VAL(msGparaM, prinTmsG)                       \
    if (!msGparaM) {                                                          \
        command_log(prinTmsG"\n");                                            \
        goto fail_parse_ctrl_msg;                                             \
    }

#define CHECK_CTRL_MSG_NON_NULL(msGparaM)                                     \
    if (!ctrl_msg->msGparaM) {                                                \
        command_log("Failed to process rx data\n");                           \
        goto fail_parse_ctrl_msg;                                             \
    }

#define CHECK_CTRL_MSG_FAILED(msGparaM)                                       \
    if (ctrl_msg->msGparaM->resp) {                                           \
        command_log("Failure resp/event: possibly precondition not met\n");   \
        goto fail_parse_ctrl_msg;                                             \
    }

#define CTRL_ALLOC_ASSIGN(TyPe,MsG_StRuCt)                                    \
    TyPe *req_payload = (TyPe *)                                              \
        hosted_calloc(1, sizeof(TyPe));                                       \
    if (!req_payload) {                                                       \
      command_log("Failed to allocate memory for req.%s\n",#MsG_StRuCt);      \
      failure_status = CTRL_ERR_MEMORY_FAILURE;                               \
        break;                                                                \
    }                                                                         \
    req.MsG_StRuCt = req_payload;                                             \
    buff_to_free1 = (uint8_t*)req_payload;


/* Control response callbacks
 * These will be updated per control request received
 * 1. If application wants to use synchrounous, i.e. Wait till the response received
 *    after current control request is sent or timeout occurs,
 *    application will pass this callback in request as NULL.
 * 2. If application wants to use `asynchrounous`, i.e. Just send the request and
 *    unblock for next processing, application will assign function pointer in
 *    control request, which will be registered here.
 *    When the response comes, the this registered callback function will be called
 *    with input as response
 */
static ctrl_resp_cb_t ctrl_resp_cb_table [CTRL_RESP_MAX - CTRL_RESP_BASE] = { NULL };

/* Control event callbacks
 * These will be updated when user registers event callback
 * using `set_event_callback` API
 * 1. If application does not register event callback,
 *    Events received from ESP32 will be dropped
 * 2. If application registers event callback,
 *    and when registered event is received from ESP32,
 *    event callback will be called asynchronously
 */
static ctrl_event_cb_t ctrl_event_cb_table[CTRL_EVENT_MAX - CTRL_EVENT_BASE] = { NULL };

/* Returns CALLBACK_AVAILABLE if a non NULL control event
 * callback is available. It will return failure -
 *     MSG_ID_OUT_OF_ORDER - if request msg id is unsupported
 *     CALLBACK_NOT_REGISTERED - if aync callback is not available
 **/
static int is_event_callback_registered(int event)
{
   int event_cb_tbl_idx = event - CTRL_EVENT_BASE;

   if ((event<=CTRL_EVENT_BASE) || (event>=CTRL_EVENT_MAX)) {
      //printf("Could not identify event[%u]\n", event);
      return MSG_ID_OUT_OF_ORDER;
   }

   if (ctrl_event_cb_table[event_cb_tbl_idx]) {
      return CALLBACK_AVAILABLE;
   }

   return CALLBACK_NOT_REGISTERED;
}


/* Check and call control response asynchronous callback if available
 * else flag error
 *     MSG_ID_OUT_OF_ORDER - if response id is not understandable
 *     CALLBACK_NOT_REGISTERED - callback is not registered
 **/
static int call_async_resp_callback(ctrl_cmd_t *app_resp)
{
   if ((app_resp->msg_id <= CTRL_RESP_BASE) ||
       (app_resp->msg_id >= CTRL_RESP_MAX)) {
      return MSG_ID_OUT_OF_ORDER;
   }

   if (ctrl_resp_cb_table[app_resp->msg_id-CTRL_RESP_BASE]) {
      return ctrl_resp_cb_table[app_resp->msg_id-CTRL_RESP_BASE](app_resp);
   }

   return CALLBACK_NOT_REGISTERED;
}

/* Check and call control event asynchronous callback if available
 * else flag error
 *     MSG_ID_OUT_OF_ORDER - if event id is not understandable
 *     CALLBACK_NOT_REGISTERED - callback is not registered
 **/
static int call_event_callback(ctrl_cmd_t *app_event)
{
   if ((app_event->msg_id <= CTRL_EVENT_BASE) ||
       (app_event->msg_id >= CTRL_EVENT_MAX)) {
      return MSG_ID_OUT_OF_ORDER;
   }

   if (ctrl_event_cb_table[app_event->msg_id-CTRL_EVENT_BASE]) {
      return ctrl_event_cb_table[app_event->msg_id-CTRL_EVENT_BASE](app_event);
   }

   return CALLBACK_NOT_REGISTERED;
}



/* Set asynchronous control response callback from control **request**
 * In case of synchronous request, `resp_cb` will be NULL and table
 * `ctrl_resp_cb_table` will be updated with NULL
 * In case of asynchronous request, valid callback will be cached
 **/
static int set_async_resp_callback(int req_msg_id, ctrl_resp_cb_t resp_cb)
{
   /* Assign(Replace) response callback passed */
   int exp_resp_msg_id = (req_msg_id - CTRL_REQ_BASE + CTRL_RESP_BASE);
   if (exp_resp_msg_id >= CTRL_RESP_MAX) {
      //printf("Not able to map new request to resp id\n");
      return MSG_ID_OUT_OF_ORDER;
   } else {
      ctrl_resp_cb_table[exp_resp_msg_id-CTRL_RESP_BASE] = resp_cb;
      return CALLBACK_SET_SUCCESS;
   }
}

/* Set asynchronous control response callback from control **response**
 * In case of synchronous request, `resp_cb` will be NULL and table
 * `ctrl_resp_cb_table` will be updated with NULL
 * In case of asynchronous request, valid callback will be cached
 **/
static int is_async_resp_callback_registered_by_resp_msg_id(int resp_msg_id)
{
   if ((resp_msg_id <= CTRL_RESP_BASE) || (resp_msg_id >= CTRL_RESP_MAX)) {
      //printf("resp id[%u] out of range\n", resp_msg_id);
      return MSG_ID_OUT_OF_ORDER;
   }

   if (ctrl_resp_cb_table[resp_msg_id-CTRL_RESP_BASE]) {
      return CALLBACK_AVAILABLE;
   }

   return CALLBACK_NOT_REGISTERED;
}





/* i.e. original "ctrl_app_send_req" */
int esp_host_ctrl_send_req(ctrl_cmd_t *app_req) {
   
   int       ret = SUCCESS;
   CtrlMsg   req = {0};
   uint32_t  tx_protobuf_len = 0;
   uint8_t   failure_status = 0;
   uint8_t  *buff_to_free1 = NULL;
   void     *buff_to_free2 = NULL;

   if (!app_req) {
      failure_status = CTRL_ERR_INCORRECT_ARG;
      return FAILURE;
   }

   app_req->msg_type = CTRL_REQ;
   
   /* 2. Protobuf msg init */
   ctrl_msg__init(&req);
   
   req.msg_id = (CtrlMsgId)app_req->msg_id;
   /* payload case is exact match to msg id in esp_hosted_config.pb-c.h */
   req.payload_case = (CtrlMsg__PayloadCase) app_req->msg_id;

   /* 3. identify request and compose CtrlMsg */
   switch(req.msg_id) {
      case CTRL_REQ_GET_WIFI_MODE:
      case CTRL_REQ_GET_AP_CONFIG:
      case CTRL_REQ_DISCONNECT_AP:
      case CTRL_REQ_GET_SOFTAP_CONFIG:
      case CTRL_REQ_GET_SOFTAP_CONN_STA_LIST:
      case CTRL_REQ_STOP_SOFTAP:
      case CTRL_REQ_GET_PS_MODE:
      case CTRL_REQ_OTA_BEGIN:
      case CTRL_REQ_OTA_END:
      case CTRL_REQ_GET_WIFI_CURR_TX_POWER: 
         /* Intentional fallthrough & empty */
         break;
       
      case CTRL_REQ_GET_AP_SCAN_LIST: 
         if (app_req->cmd_timeout_sec < DEFAULT_CTRL_RESP_AP_SCAN_TIMEOUT)
            app_req->cmd_timeout_sec = DEFAULT_CTRL_RESP_AP_SCAN_TIMEOUT;
         break;
      case CTRL_REQ_GET_MAC_ADDR: {
         CTRL_ALLOC_ASSIGN(CtrlMsgReqGetMacAddress, req_get_mac_address);

         if ((app_req->u.wifi_mac.mode <= WIFI_MODE_NONE) ||
             (app_req->u.wifi_mac.mode >= WIFI_MODE_APSTA)) {
            command_log("Invalid parameter\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }
         ctrl_msg__req__get_mac_address__init(req_payload);
         req_payload->mode = app_req->u.wifi_mac.mode;

         break;
      } case CTRL_REQ_SET_MAC_ADDR: {
         wifi_mac_t * p = &app_req->u.wifi_mac;
         CTRL_ALLOC_ASSIGN(CtrlMsgReqSetMacAddress, req_set_mac_address);

         if ((p->mode <= WIFI_MODE_NONE) ||
             (p->mode >= WIFI_MODE_APSTA)||
             (!strlen(p->mac)) ||
             (strlen(p->mac) > MAX_MAC_STR_LEN)) {
            command_log("Invalid parameter\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }
         ctrl_msg__req__set_mac_address__init(req_payload);

         req_payload->mode = p->mode;
         req_payload->mac.len = min(strlen(p->mac), MAX_MAC_STR_LEN);
         req_payload->mac.data = (uint8_t *)p->mac;

         break;
      } case CTRL_REQ_SET_WIFI_MODE: {
         wifi_mode_t * p = &app_req->u.wifi_mode;
         CTRL_ALLOC_ASSIGN(CtrlMsgReqSetMode, req_set_wifi_mode);

         if ((p->mode < WIFI_MODE_NONE) || (p->mode >= WIFI_MODE_MAX)) {
            command_log("Invalid wifi mode\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }
         ctrl_msg__req__set_mode__init(req_payload);
         req_payload->mode = p->mode;
         break;
      } case CTRL_REQ_CONNECT_AP: {
         wifi_ap_config_t * p = &app_req->u.wifi_ap_config;
         CTRL_ALLOC_ASSIGN(CtrlMsgReqConnectAP,req_connect_ap);

         if ((strlen((char *)p->ssid) > MAX_SSID_LENGTH) ||
               (!strlen((char *)p->ssid))) {
            command_log("Invalid SSID length\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }

         if (strlen((char *)p->pwd) > MAX_PWD_LENGTH) {
            command_log("Invalid password length\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }

         if (strlen((char *)p->bssid) > MAX_MAC_STR_LEN) {
            command_log("Invalid BSSID length\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }
         ctrl_msg__req__connect_ap__init(req_payload);

         req_payload->ssid  = (char *)&p->ssid;
         req_payload->pwd   = (char *)&p->pwd;
         req_payload->bssid = (char *)&p->bssid;
         req_payload->is_wpa3_supported = p->is_wpa3_supported;
         req_payload->listen_interval = p->listen_interval;
         break;
      } case CTRL_REQ_SET_SOFTAP_VND_IE: {
         wifi_softap_vendor_ie_t *p = &app_req->u.wifi_softap_vendor_ie;
         CTRL_ALLOC_ASSIGN(CtrlMsgReqSetSoftAPVendorSpecificIE,
               req_set_softap_vendor_specific_ie);

         if ((p->type > WIFI_VND_IE_TYPE_ASSOC_RESP) ||
             (p->type < WIFI_VND_IE_TYPE_BEACON)) {
            command_log("Invalid vendor ie type \n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }

         if ((p->idx > WIFI_VND_IE_ID_1) || (p->idx < WIFI_VND_IE_ID_0)) {
            command_log("Invalid vendor ie ID index \n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }

         if (!p->vnd_ie.payload) {
            command_log("Invalid vendor IE buffer \n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }
         ctrl_msg__req__set_soft_apvendor_specific_ie__init(req_payload);

         req_payload->enable = p->enable;
         req_payload->type = (CtrlVendorIEType) p->type;
         req_payload->idx = (CtrlVendorIEID) p->idx;

         req_payload->vendor_ie_data = (CtrlMsgReqVendorIEData *)hosted_malloc(sizeof(CtrlMsgReqVendorIEData));

         if (!req_payload->vendor_ie_data) {
            command_log("Mem alloc fail\n");
            break;
         }
         buff_to_free2 = req_payload->vendor_ie_data;

         ctrl_msg__req__vendor_iedata__init(req_payload->vendor_ie_data);

         req_payload->vendor_ie_data->element_id = p->vnd_ie.element_id;
         req_payload->vendor_ie_data->length = p->vnd_ie.length;
         req_payload->vendor_ie_data->vendor_oui.data =p->vnd_ie.vendor_oui;
         req_payload->vendor_ie_data->vendor_oui.len = VENDOR_OUI_BUF;

         req_payload->vendor_ie_data->payload.data = p->vnd_ie.payload;
         req_payload->vendor_ie_data->payload.len = p->vnd_ie.payload_len;
         break;
      } case CTRL_REQ_START_SOFTAP: {
         softap_config_t *p = &app_req->u.wifi_softap_config;
         CTRL_ALLOC_ASSIGN(CtrlMsgReqStartSoftAP, req_start_softap);

         if ((strlen((char *)&p->ssid) > MAX_SSID_LENGTH) ||
             (!strlen((char *)&p->ssid))) {
            command_log("Invalid SSID length\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }

         if ((strlen((char *)&p->pwd) > MAX_PWD_LENGTH) ||
             ((p->encryption_mode != WIFI_AUTH_OPEN) &&
              (strlen((char *)&p->pwd) < MIN_PWD_LENGTH))) {
            command_log("Invalid password length\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }

         if ((p->channel < MIN_CHNL_NO) ||
             (p->channel > MAX_CHNL_NO)) {
            command_log("Invalid softap channel\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }

         if ((p->encryption_mode < WIFI_AUTH_OPEN) ||
             (p->encryption_mode == WIFI_AUTH_WEP) ||
             (p->encryption_mode > WIFI_AUTH_WPA_WPA2_PSK)) {

            command_log("Asked Encryption mode not supported\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }

         if ((p->max_connections < MIN_CONN_NO) ||
             (p->max_connections > MAX_CONN_NO)) {
            command_log("Invalid maximum connection number\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }

         if ((p->bandwidth < WIFI_BW_HT20) ||
             (p->bandwidth > WIFI_BW_HT40)) {
            command_log("Invalid bandwidth\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }
         ctrl_msg__req__start_soft_ap__init(req_payload);

         req_payload->ssid = (char *)&p->ssid;
         req_payload->pwd = (char *)&p->pwd;
         req_payload->chnl = p->channel;
         req_payload->sec_prot = (CtrlWifiSecProt)p->encryption_mode;
         req_payload->max_conn = p->max_connections;
         req_payload->ssid_hidden = p->ssid_hidden;
         req_payload->bw = p->bandwidth;
         break;
      } case CTRL_REQ_SET_PS_MODE: {
         wifi_power_save_t * p = &app_req->u.wifi_ps;
         CTRL_ALLOC_ASSIGN(CtrlMsgReqSetMode, req_set_power_save_mode);

         if ((p->ps_mode < WIFI_PS_MIN_MODEM) ||
             (p->ps_mode >= WIFI_PS_INVALID)) {
            command_log("Invalid power save mode\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }
         ctrl_msg__req__set_mode__init(req_payload);

         req_payload->mode = p->ps_mode;
         break;
      } case CTRL_REQ_OTA_WRITE: {
         ota_write_t *p = & app_req->u.ota_write;
         CTRL_ALLOC_ASSIGN(CtrlMsgReqOTAWrite, req_ota_write);

         if (!p->ota_data || (p->ota_data_len == 0)) {
            command_log("Invalid parameter\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            break;
         }

         ctrl_msg__req__otawrite__init(req_payload);
         req_payload->ota_data.data = p->ota_data;
         req_payload->ota_data.len = p->ota_data_len;
         break;
      } case CTRL_REQ_SET_WIFI_MAX_TX_POWER: {
         CTRL_ALLOC_ASSIGN(CtrlMsgReqSetWifiMaxTxPower,req_set_wifi_max_tx_power);
         ctrl_msg__req__set_wifi_max_tx_power__init(req_payload);
         req_payload->wifi_max_tx_power = app_req->u.wifi_tx_power.power;
         break;
      } case CTRL_REQ_CONFIG_HEARTBEAT: {
         CTRL_ALLOC_ASSIGN(CtrlMsgReqConfigHeartbeat, req_config_heartbeat);
         ctrl_msg__req__config_heartbeat__init(req_payload);
         req_payload->enable = app_req->u.e_heartbeat.enable;
         req_payload->duration = app_req->u.e_heartbeat.duration;
         if (req_payload->enable) {
            //printf("Enable heartbeat with duration %ld\n", (long int)req_payload->duration);
            //if (CALLBACK_AVAILABLE != is_event_callback_registered(CTRL_EVENT_HEARTBEAT))
               //printf("Note: ** Subscribe heartbeat event to get notification **\n");
         } else {
            //printf("Disable Heartbeat\n");
         }
         break;
      } default: {
         failure_status = CTRL_ERR_UNSUPPORTED_MSG;
         //printf("Unsupported Control Req[%u]",req.msg_id);
         
         break;
      }
   }

   if(failure_status == 0) {
      /* 4. Protobuf msg size */
      tx_protobuf_len = ctrl_msg__get_packed_size(&req);
      if (!tx_protobuf_len) {
         command_log("Invalid tx length\n");
         failure_status = CTRL_ERR_PROTOBUF_ENCODE;
      }
   }

   CMsg msg(tx_protobuf_len);

   if(msg.is_valid() && failure_status == 0) {
      /* 8. Pack in protobuf and send the request */
      ctrl_msg__pack(&req, msg.get_protobuf_ptr());
   }
   else {
      msg.clear();
      ret = FAILURE;
   }
      
   if(buff_to_free1 != NULL) {
      delete buff_to_free1;
   }
   if(buff_to_free2 != NULL) {
      delete buff_to_free2;
   }
   
   return ret;
}


