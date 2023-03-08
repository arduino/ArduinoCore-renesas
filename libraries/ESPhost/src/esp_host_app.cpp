#include "esp_host_app.h"


int esp_host_app_init() {
   esp_host_spi_init();
   esp_host_set_cb_rx(esp_host_msg_received);
}


int esp_host_get_wifi_mac_address(int mode, char *mac_out, int mac_out_dim) {
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