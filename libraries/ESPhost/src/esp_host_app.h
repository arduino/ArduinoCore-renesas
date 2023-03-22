#ifndef ESP_HOST_APP_LAYER_H
#define ESP_HOST_APP_LAYER_H

#include "esp_host_control.h"
#include "esp_host_spi_driver.h"


#define ESP_HOSTED_OK    0
 
#define ESP_HOSTED_ERROR_MSG_PREPARATION 1 
#define ESP_HOSTED_ERROR_SPI_COMMUNICATION 2
int esp_host_app_init();

int esp_host_get_wifi_mac_address(int mode, char *mac_out, int mac_out_dim);
int esp_host_get_wifi_mac_address_v0(int mode, char *mac_out, int mac_out_dim);
int esp_host_get_wifi_mac_address_old(int mode, char *mac_out, int mac_out_dim);



#endif