#ifndef _ARDUINO_LW_IP_INTERFACE_H
#define _ARDUINO_LW_IP_INTERFACE_H
#include <stdint.h>
#include "lwipopts.h"
#include "lwip/include/lwip/ip_addr.h"
#include "lwip/include/lwip/dhcp.h"
#include "lwip/include/lwip/udp.h"
#include "lwip/include/lwip/tcp.h"
#include "lwip/include/lwip/opt.h"
#include "lwip/include/lwip/init.h"
#include "lwip/include/lwip/timeouts.h"
#include "lwip/include/lwip/dns.h"
#include "lwip/include/lwip/prot/dhcp.h"
#include "eth0.h"

/* Maximum number of client per server */
#define MAX_CLIENT  32

#define MAX_DHCP_TRIES  4
#define TIMEOUT_DNS_REQUEST 10000U

typedef enum {
  D_DHCP_STATE_IDLE,
  D_DHCP_STATE_START,
  D_DHCP_STATE_WAIT_FOR_ADDRESS,
  D_DHCP_STATE_GOT_ADDRESS
} DhcpState_t;

void add_eth0_interface(const uint8_t *mac, const uint8_t *ip, const uint8_t *gw, const uint8_t *netmask);
bool is_eth0_initialized();
bool is_eth0_link_up();
uint32_t get_eth0_ip_add();
uint32_t get_eth0_gw_add();
uint32_t get_eth0_nm_add();
uint32_t get_dns_add();
uint32_t get_dhcp_add();
void lwip_task();

ip_addr_t *u8_to_ip_addr(uint8_t *ipu8, ip_addr_t *ipaddr);
uint32_t ip_addr_to_u32(ip_addr_t *ipaddr);

#if LWIP_DHCP
void dhcp_start_cmd();
void dhcp_stop_cmd();
bool is_dhcp_started();
void set_dhcp_not_used();
bool dhcp_address_acquired(); 
uint8_t dhcp_get_lease_state();
#endif

#if LWIP_DNS
void set_up_dns(const uint8_t *dnsaddr);
int8_t get_ip_address_from_hostname(const char *hostname, uint32_t *ipaddr);
#endif



#endif