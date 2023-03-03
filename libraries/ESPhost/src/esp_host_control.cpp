#include "esp_host_control.h"


#ifdef __cplusplus
extern "C" {
#endif


/* i.e. original ctrl_app_send_req*/
int esp_host_ctrl_send_req(ctrl_cmd_t *app_req) {
   int       ret = SUCCESS;
   CtrlMsg   req = {0};
   uint32_t  tx_len = 0;
   uint8_t  *tx_data = NULL;
   uint8_t  *buff_to_free1 = NULL;
   void     *buff_to_free2 = NULL;
   uint8_t   failure_status = 0;



   if (!app_req) {
      failure_status = CTRL_ERR_INCORRECT_ARG;
      //goto fail_req;
   }

   app_req->msg_type = CTRL_REQ;
   
   

   /* 2. Protobuf msg init */
   ctrl_msg__init(&req);
   
   #ifdef TOBE_IMPLEMENTED

   req.msg_id = app_req->msg_id;
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
      case CTRL_REQ_GET_WIFI_CURR_TX_POWER: {
         /* Intentional fallthrough & empty */
         break;
      } case CTRL_REQ_GET_AP_SCAN_LIST: {
         if (app_req->cmd_timeout_sec < DEFAULT_CTRL_RESP_AP_SCAN_TIMEOUT)
            app_req->cmd_timeout_sec = DEFAULT_CTRL_RESP_AP_SCAN_TIMEOUT;
         break;
      } case CTRL_REQ_GET_MAC_ADDR: {
         CTRL_ALLOC_ASSIGN(CtrlMsgReqGetMacAddress, req_get_mac_address);
         

         if ((app_req->u.wifi_mac.mode <= WIFI_MODE_NONE) ||
             (app_req->u.wifi_mac.mode >= WIFI_MODE_APSTA)) {
            command_log("Invalid parameter\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            goto fail_req;
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
            goto fail_req;
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
            goto fail_req;
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
            goto fail_req;
         }

         if (strlen((char *)p->pwd) > MAX_PWD_LENGTH) {
            command_log("Invalid password length\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            goto fail_req;
         }

         if (strlen((char *)p->bssid) > MAX_MAC_STR_LEN) {
            command_log("Invalid BSSID length\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            goto fail_req;
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
            goto fail_req;
         }

         if ((p->idx > WIFI_VND_IE_ID_1) || (p->idx < WIFI_VND_IE_ID_0)) {
            command_log("Invalid vendor ie ID index \n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            goto fail_req;
         }

         if (!p->vnd_ie.payload) {
            command_log("Invalid vendor IE buffer \n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            goto fail_req;
         }
         ctrl_msg__req__set_soft_apvendor_specific_ie__init(req_payload);

         req_payload->enable = p->enable;
         req_payload->type = (CtrlVendorIEType) p->type;
         req_payload->idx = (CtrlVendorIEID) p->idx;

         req_payload->vendor_ie_data = (CtrlMsgReqVendorIEData *)hosted_malloc(sizeof(CtrlMsgReqVendorIEData));

         if (!req_payload->vendor_ie_data) {
            command_log("Mem alloc fail\n");
            goto fail_req;
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
            goto fail_req;
         }

         if ((strlen((char *)&p->pwd) > MAX_PWD_LENGTH) ||
             ((p->encryption_mode != WIFI_AUTH_OPEN) &&
              (strlen((char *)&p->pwd) < MIN_PWD_LENGTH))) {
            command_log("Invalid password length\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            goto fail_req;
         }

         if ((p->channel < MIN_CHNL_NO) ||
             (p->channel > MAX_CHNL_NO)) {
            command_log("Invalid softap channel\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            goto fail_req;
         }

         if ((p->encryption_mode < WIFI_AUTH_OPEN) ||
             (p->encryption_mode == WIFI_AUTH_WEP) ||
             (p->encryption_mode > WIFI_AUTH_WPA_WPA2_PSK)) {

            command_log("Asked Encryption mode not supported\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            goto fail_req;
         }

         if ((p->max_connections < MIN_CONN_NO) ||
             (p->max_connections > MAX_CONN_NO)) {
            command_log("Invalid maximum connection number\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            goto fail_req;
         }

         if ((p->bandwidth < WIFI_BW_HT20) ||
             (p->bandwidth > WIFI_BW_HT40)) {
            command_log("Invalid bandwidth\n");
            failure_status = CTRL_ERR_INCORRECT_ARG;
            goto fail_req;
         }
         ctrl_msg__req__start_soft_ap__init(req_payload);

         req_payload->ssid = (char *)&p->ssid;
         req_payload->pwd = (char *)&p->pwd;
         req_payload->chnl = p->channel;
         req_payload->sec_prot = p->encryption_mode;
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
            goto fail_req;
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
            goto fail_req;
         }

         ctrl_msg__req__otawrite__init(req_payload);
         req_payload->ota_data.data = p->ota_data;
         req_payload->ota_data.len = p->ota_data_len;
         break;
      } case CTRL_REQ_SET_WIFI_MAX_TX_POWER: {
         CTRL_ALLOC_ASSIGN(CtrlMsgReqSetWifiMaxTxPower,
               req_set_wifi_max_tx_power);
         ctrl_msg__req__set_wifi_max_tx_power__init(req_payload);
         req_payload->wifi_max_tx_power = app_req->u.wifi_tx_power.power;
         break;
      } case CTRL_REQ_CONFIG_HEARTBEAT: {
         CTRL_ALLOC_ASSIGN(CtrlMsgReqConfigHeartbeat, req_config_heartbeat);
         ctrl_msg__req__config_heartbeat__init(req_payload);
         req_payload->enable = app_req->u.e_heartbeat.enable;
         req_payload->duration = app_req->u.e_heartbeat.duration;
         if (req_payload->enable) {
            printf("Enable heartbeat with duration %ld\n", (long int)req_payload->duration);
            if (CALLBACK_AVAILABLE != is_event_callback_registered(CTRL_EVENT_HEARTBEAT))
               printf("Note: ** Subscribe heartbeat event to get notification **\n");
         } else {
            printf("Disable Heartbeat\n");
         }
         break;
      } default: {
         failure_status = CTRL_ERR_UNSUPPORTED_MSG;
         printf("Unsupported Control Req[%u]",req.msg_id);
         goto fail_req;
         break;
      }
   }

   /* 4. Protobuf msg size */
   tx_len = ctrl_msg__get_packed_size(&req);
   if (!tx_len) {
      command_log("Invalid tx length\n");
      failure_status = CTRL_ERR_PROTOBUF_ENCODE;
      goto fail_req;
   }

   /* 5. Allocate protobuf msg */
   tx_data = (uint8_t *)hosted_calloc(1, tx_len);
   if (!tx_data) {
      command_log("Failed to allocate memory for tx_data\n");
      failure_status = CTRL_ERR_MEMORY_FAILURE;
      goto fail_req;
   }

   /* 6. Assign response callback
    * a. If the response callback is not set, this will reset the
    *    callback to NULL.
    * b. If the non NULL response is assigned, this will set the
    *    callback to user defined callback function */
   ret = set_async_resp_callback(app_req->msg_id, app_req->ctrl_resp_cb);
   if (ret < 0) {
      printf("could not set callback for req[%u]\n",req.msg_id);
      failure_status = CTRL_ERR_SET_ASYNC_CB;
      goto fail_req;
   }

   /* 7. Start timeout for response for async only
    * For sync procedures, hosted_get_semaphore takes care to
    * handle timeout situations */
   if (app_req->ctrl_resp_cb) {
      async_timer_handle = hosted_timer_start(app_req->cmd_timeout_sec, CTRL__TIMER_ONESHOT,
            ctrl_async_timeout_handler, app_req->ctrl_resp_cb);
      if (!async_timer_handle) {
         printf("Failed to start async resp timer\n");
         goto fail_req;
      }
   }

   /* 8. Pack in protobuf and send the request */
   ctrl_msg__pack(&req, tx_data);
   if (transport_pserial_send(tx_data, tx_len)) {
      command_log("Send control req[%u] failed\n",req.msg_id);
      failure_status = CTRL_ERR_TRANSPORT_SEND;
      goto fail_req;
   }



   /* 9. Free hook for application */
   if (app_req->free_buffer_handle) {
      if (app_req->free_buffer_func) {
         app_req->free_buffer_func(app_req->free_buffer_handle);
      }
   }

   /* 10. Cleanup */
   mem_free(tx_data);
   mem_free(buff_to_free2);
   mem_free(buff_to_free1);
   return SUCCESS;

fail_req:


   if (app_req->ctrl_resp_cb) {
      /* 11. In case of async procedure,
       * Let application know of failure using callback itself
       **/
      ctrl_cmd_t *app_resp = NULL;
      app_resp = (ctrl_cmd_t *)hosted_malloc(sizeof(ctrl_cmd_t));
      if (!app_resp) {
         printf("Failed to allocate app_resp\n");
         goto fail_req2;
      }
      memset(app_resp, 0, sizeof(ctrl_cmd_t));
      app_resp->msg_type = CTRL_RESP;
      app_resp->msg_id = (app_req->msg_id - CTRL_REQ_BASE + CTRL_RESP_BASE);
      app_resp->resp_event_status = failure_status;

      /* 12. In async procedure, it is important to get
       * some kind of acknowledgement to user */
      app_req->ctrl_resp_cb(app_resp);
   }

fail_req2:
   /* 13. Cleanup */
   if (app_req->free_buffer_handle) {
      if (app_req->free_buffer_func) {
         app_req->free_buffer_func(app_req->free_buffer_handle);
      }
   }

   mem_free(tx_data);
   mem_free(buff_to_free2);
   mem_free(buff_to_free1);
   #endif
   return FAILURE;
}


#ifdef __cplusplus
}
#endif