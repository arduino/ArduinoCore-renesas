#ifndef _ARDUINO_RENESAS_ETHERNET_DRIVER_
#define _ARDUINO_RENESAS_ETHERNET_DRIVER_

#include "r_ether_phy_api.h"
#include "r_ether_phy.h"
#include "r_ether_api.h"
#include "r_ether.h"
#include <functional>

using EtherCallback_f        = std::function<void(void)>;

#define ETHERNET_IRQ_PRIORITY   10


#define MAC_ADDRESS_DIM     6
#define ETH_BUFF_DIM        1536 


/* set the MAC ADDRESS, use the function before the initialization */
void eth_set_mac_address(const uint8_t *mad);
int  eth_get_mac_address(uint8_t *mad);

bool eth_init();
void eth_execute_link_process();
uint8_t *eth_input(volatile uint32_t *dim);
bool eth_output(uint8_t *buf, uint16_t dim);
bool eth_output_can_transimit();
void eth_release_rx_buffer();
uint8_t *eth_get_tx_buffer(uint16_t *size);
void eth_set_rx_frame_cbk(EtherCallback_f fn);
void eth_set_tx_frame_cbk(EtherCallback_f fn);
void eth_set_link_on_cbk(EtherCallback_f fn);
void eth_set_link_off_cbk(EtherCallback_f fn);
void eth_set_lan_wake_up_cbk(EtherCallback_f fn);
void eth_set_magic_packet_cbk(EtherCallback_f fn);


#endif
