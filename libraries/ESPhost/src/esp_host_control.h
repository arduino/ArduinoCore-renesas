#ifndef ESP_HOST_CONTROL_H
#define ESP_HOST_CONTROL_H



#include <stdbool.h>

#include "esp_host_protocol.h"
#include "esp_hosted_platform.h"
#include "esp_host_callbacks.h"
#include "esp_host_spi_driver.h"




int esp_host_msg_received(ctrl_cmd_t **ans);


int esp_host_wait_for_answer(ctrl_cmd_t *req);


/* Initialize hosted control library
 *
 * This is first step for application while using control path
 * This will allocate and instantiate hosted control library
 *
 * Returns:
 * > SUCCESS - 0
 * > FAILURE - -1
 **/
int init_hosted_control_lib(void);

/* De-initialize hosted control library
 *
 * This is last step for application while using control path
 * This will deallocate and cleanup hosted control library
 *
 * Returns:
 * > SUCCESS - 0
 * > FAILURE - -1
 **/
int deinit_hosted_control_lib(void);

/* Get the MAC address of station or softAP interface of ESP32 */
ctrl_cmd_t * wifi_get_mac(ctrl_cmd_t req);

/* Set MAC address of ESP32 interface for given wifi mode */
ctrl_cmd_t * wifi_set_mac(ctrl_cmd_t req);

/* Get Wi-Fi mode of ESP32 */
ctrl_cmd_t * wifi_get_mode(ctrl_cmd_t req);

/* Set the Wi-Fi mode of ESP32 */
ctrl_cmd_t * wifi_set_mode(ctrl_cmd_t req);

/* Set Wi-Fi power save mode of ESP32 */
ctrl_cmd_t * wifi_set_power_save_mode(ctrl_cmd_t req);

/* Get the Wi-Fi power save mode of ESP32 */
ctrl_cmd_t * wifi_get_power_save_mode(ctrl_cmd_t req);

/* Get list of available neighboring APs of ESP32 */
ctrl_cmd_t * wifi_ap_scan_list(ctrl_cmd_t req);

/* Get the AP config to which ESP32 station is connected */
ctrl_cmd_t * wifi_get_ap_config(ctrl_cmd_t req);

/* Set the AP config to which ESP32 station should connect to */
ctrl_cmd_t * wifi_connect_ap(ctrl_cmd_t req);

/* Disconnect ESP32 station from AP */
ctrl_cmd_t * wifi_disconnect_ap(ctrl_cmd_t req);

/* Set configuration of ESP32 softAP and start broadcasting */
ctrl_cmd_t * wifi_start_softap(ctrl_cmd_t req);

/* Get configuration of ESP32 softAP */
ctrl_cmd_t * wifi_get_softap_config(ctrl_cmd_t req);

/* Stop ESP32 softAP */
ctrl_cmd_t * wifi_stop_softap(ctrl_cmd_t req);

/* Get list of connected stations to ESP32 softAP */
ctrl_cmd_t * wifi_get_softap_connected_station_list(ctrl_cmd_t req);

/* Function set 802.11 Vendor-Specific Information Element.
 * It needs to get called before starting of ESP32 softAP */
ctrl_cmd_t * wifi_set_vendor_specific_ie(ctrl_cmd_t req);

/* Sets maximum WiFi transmitting power at ESP32 */
ctrl_cmd_t * wifi_set_max_tx_power(ctrl_cmd_t req);

/* Gets current WiFi transmiting power at ESP32 */
ctrl_cmd_t * wifi_get_curr_tx_power(ctrl_cmd_t req);

/* Configure heartbeat event. Be default heartbeat is not enabled.
 * To enable heartbeats, user need to use this API in addition
 * to setting event callback for heartbeat event */
ctrl_cmd_t * config_heartbeat(ctrl_cmd_t req);

/* Performs an OTA begin operation for ESP32 which erases and
 * prepares existing flash partition for new flash writing */
ctrl_cmd_t * ota_begin(ctrl_cmd_t req);

/* Performs an OTA write operation for ESP32, It writes bytes from `ota_data`
 * buffer with `ota_data_len` number of bytes to OTA partition in flash. Number
 * of bytes can be small than size of complete binary to be flashed. In that
 * case, this caller is expected to repeatedly call this function till
 * total size written equals size of complete binary */
ctrl_cmd_t * ota_write(ctrl_cmd_t req);

/* Performs an OTA end operation for ESP32, It validates written OTA image,
 * sets newly written OTA partition as boot partition for next boot,
 * Creates timer which reset ESP32 after 5 sec */
ctrl_cmd_t * ota_end(ctrl_cmd_t req);

/* Get the interface up for interface `iface` */
int interface_up(int sockfd, char* iface);

/* Get the interface down for interface `iface` */
int interface_down(int sockfd, char* iface);

/* Set ethernet interface MAC address `mac` to interface `iface` */
int set_hw_addr(int sockfd, char* iface, char* mac);

/* Create an endpoint for communication */
int create_socket(int domain, int type, int protocol, int *sock);

/* Close an endpoint of the communication */
int close_socket(int sock);

/* -------------------------------------------------------------------------- */


/* i.e. original ctrl_app_send_req*/
int esp_host_ctrl_send_req(ctrl_cmd_t *app_req);




#endif