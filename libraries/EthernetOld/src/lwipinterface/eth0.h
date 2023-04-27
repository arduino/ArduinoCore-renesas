#ifndef __ETH0_ARDUINO_INTERFACE_H
#define __ETH0_ARDUINO_INTERFACE_H

#include <stdint.h>
#include "lwip/include/lwip/netif.h"
#include "lwip/include/lwip/ip_addr.h"
#include "lwip/include/lwip/etharp.h"
#include "lwip/include/netif/ethernet.h"

/* undefine to remove assert in fifo lenght */
//#define USE_ASSERT_DEBUG

#define IFNAME0 'e'
#define IFNAME1 't'


/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   (uint8_t) 192
#define IP_ADDR1   (uint8_t) 168
#define IP_ADDR2   (uint8_t) 0
#define IP_ADDR3   (uint8_t) 10

/*NETMASK*/
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0

/*Gateway Address*/
#define GW_ADDR0   (uint8_t) 192
#define GW_ADDR1   (uint8_t) 168
#define GW_ADDR2   (uint8_t) 0
#define GW_ADDR3   (uint8_t) 1

using LinkChangeCbk_f = void (*)(bool up);

#ifdef __cplusplus
extern "C" {
#endif



void eth0if_set_link_change_cbk(LinkChangeCbk_f fn);

void eth0if_set_ip4_address(const uint8_t *ad);
void eth0if_set_ip4_netmask(const uint8_t *nm);
void eth0if_set_ip4_gateway(const uint8_t *gw);
void eth0if_set_mac_address(const uint8_t *mad);

/* first set up the addresses with the previous function (use nullptr if DHCP 
   is going to be used) then  */
void eth0if_lwip_config(bool is_default);
bool is_eth0if_initialized();
netif *eth0if_get_ptr();
void eth0if_frame_received();

struct pbuf* get_rx_pbuf();

#ifdef __cplusplus
}
#endif

#endif