#ifndef __DHCPS_H__
#define __DHCPS_H__

#define USE_LWIP_AS_LIBRARY

#ifdef USE_LWIP_AS_LIBRARY
#include "lwip/include/lwip/dhcp.h"
#include "lwip/include/lwip/dns.h"
#include "lwip/include/lwip/init.h"
#include "lwip/include/lwip/ip_addr.h"
#include "lwip/include/lwip/opt.h"
#include "lwip/include/lwip/prot/dhcp.h"
#include "lwip/include/lwip/tcp.h"
#include "lwip/include/lwip/timeouts.h"
#include "lwip/include/lwip/udp.h"
#include "lwip/include/netif/ethernet.h"
#else
#include "lwIP_Arduino.h"
#endif
#include <stdbool.h>

typedef struct dhcps_state {
    s16_t state;
} dhcps_state;

typedef struct dhcps_msg {
    u8_t op, htype, hlen, hops;
    u8_t xid[4];
    u16_t secs, flags;
    u8_t ciaddr[4];
    u8_t yiaddr[4];
    u8_t siaddr[4];
    u8_t giaddr[4];
    u8_t chaddr[16];
    u8_t sname[64];
    u8_t file[128];
    u8_t options[312];
} dhcps_msg;

/*   Defined in esp_misc.h */
typedef struct {
    bool enable;
    ip4_addr_t start_ip;
    ip4_addr_t end_ip;
} dhcps_lease_t;

enum dhcps_offer_option {
    OFFER_START = 0x00,
    OFFER_ROUTER = 0x01,
    OFFER_DNS = 0x02,
    OFFER_END
};

#define DHCPS_COARSE_TIMER_SECS 1
#define DHCPS_MAX_LEASE 0x64
#define DHCPS_LEASE_TIME_DEF (120)
#define DHCPS_LEASE_UNIT 60

typedef u32_t dhcps_time_t;
typedef u8_t dhcps_offer_t;

typedef struct {
    dhcps_offer_t dhcps_offer;
    dhcps_offer_t dhcps_dns;
    dhcps_time_t dhcps_time;
    dhcps_lease_t dhcps_poll;
} dhcps_options_t;

typedef void (*dhcps_cb_t)(u8_t client_ip[4]);

static inline bool dhcps_router_enabled(dhcps_offer_t offer)
{
    return (offer & OFFER_ROUTER) != 0;
}

static inline bool dhcps_dns_enabled(dhcps_offer_t offer)
{
    return (offer & OFFER_DNS) != 0;
}

void dhcps_start(struct netif* netif);
void dhcps_stop(struct netif* netif);
void* dhcps_option_info(u8_t op_id, u32_t opt_len);
void dhcps_set_option_info(u8_t op_id, void* opt_info, u32_t opt_len);
bool dhcp_search_ip_on_mac(u8_t* mac, ip4_addr_t* ip);
void dhcps_dns_setserver(const ip_addr_t* dnsserver);
ip4_addr_t dhcps_dns_getserver(void);
void dhcps_set_new_lease_cb(dhcps_cb_t cb);

#endif
