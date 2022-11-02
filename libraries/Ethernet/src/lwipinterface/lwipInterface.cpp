#include "lwipInterface.h"

//void(*p_callback)(timer_callback_args_t *)
extern void attach_ethernet_task(void (*fnc)());

void sys_printf(const char *format, ...) {
  
}

void DHCP_task(struct netif *netif);
/* initialization of lwip 
   called automatically when add_eth0_interface is called */
void init_lwip() {
   static bool init_performed = false;

   if (!init_performed) {
      /* Initialize the LwIP stack */
      lwip_init();
      init_performed = true;
   }
}

/* should be the same as stm32_eth_init() */
/* it set address and then configure eth0 for the use with lwip */
void add_eth0_interface(const uint8_t *mac, const uint8_t *ip, const uint8_t *gw, const uint8_t *netmask) {
  init_lwip();
  eth0if_set_mac_address(mac);
  eth0if_set_ip4_address(ip);
  eth0if_set_ip4_netmask(netmask);
  eth0if_set_ip4_gateway(gw);
  eth0if_lwip_config(true);

  attach_ethernet_task(lwip_task);
  lwip_task();
}


extern struct netif eth0if;

/* main lwip task (should be called periodically) */
void lwip_task() {
  /* 
  #ifndef ETH_INPUT_USE_IT
  ethernetif_input(&gnetif);
  #endif
  */
  /*
  if ((HAL_GetTick() - gEhtLinkTickStart) >= TIME_CHECK_ETH_LINK_STATE) {
    ethernetif_set_link(&gnetif);
    gEhtLinkTickStart = HAL_GetTick();
  }
  */


  /* Handle LwIP timeouts */
  sys_check_timeouts();

  #if LWIP_DHCP
  static unsigned long dhcp_last_time_call = 0;
  if(dhcp_last_time_call == 0 || millis() - dhcp_last_time_call > DHCP_FINE_TIMER_MSECS) {
    DHCP_task(&eth0if);
  }
  #endif

}

/* -------------------------------------------------------------------------- */
/*                            GETTERS FUNCTIONS                               */
/* -------------------------------------------------------------------------- */
/* get adresses */
uint32_t get_eth0_ip_add() { return ip4_addr_get_u32(&(eth0if_get_ptr()->ip_addr));}
uint32_t get_eth0_gw_add() { return ip4_addr_get_u32(&(eth0if_get_ptr()->gw)); }
uint32_t get_eth0_nm_add() { return ip4_addr_get_u32(&(eth0if_get_ptr()->netmask));}

uint32_t get_dns_add() {
  const ip_addr_t *tmp = dns_getserver(0);
  return ip4_addr_get_u32(tmp);
}

uint32_t get_dhcp_add() {
  struct dhcp *dhcp = (struct dhcp *)netif_get_client_data(eth0if_get_ptr(), LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
  return ip4_addr_get_u32(&(dhcp->server_ip_addr));
}

/* get initialization status */
bool is_eth0_initialized() { return is_eth0if_initialized(); }
/* get link status */
bool is_eth0_link_up() { return (bool)netif_is_link_up(eth0if_get_ptr());}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                          CONVERSION FUNCTIONS                              */
/* -------------------------------------------------------------------------- */
ip_addr_t *u8_to_ip_addr(uint8_t *ipu8, ip_addr_t *ipaddr) {
  IP_ADDR4(ipaddr, ipu8[0], ipu8[1], ipu8[2], ipu8[3]);
  return ipaddr;
}


uint32_t ip_addr_to_u32(ip_addr_t *ipaddr) {
  return ip4_addr_get_u32(ipaddr);
}


/* -------------------------------------------------------------------------- */
/*                             DHCP FUNCTIONS                                 */
/* -------------------------------------------------------------------------- */
static bool dhcp_address_obtained = false;
static bool dhcp_cmd_start = false;
bool is_dhcp_started() {return dhcp_cmd_start; }
#if LWIP_DHCP
/* -------------------------------------------------------------------------- */
bool dhcp_address_acquired() {return dhcp_address_obtained; }
void dhcp_start_cmd() { dhcp_cmd_start = true; }
void dhcp_stop_cmd()  { dhcp_cmd_start = false; }
void set_dhcp_not_used() {
  dhcp_cmd_start = false;
  dhcp_inform(eth0if_get_ptr());
}
uint8_t dhcp_get_lease_state(void) {
  uint8_t res = 0;
  struct dhcp *dhcp = (struct dhcp *)netif_get_client_data(eth0if_get_ptr(), LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

  if (dhcp->state == DHCP_STATE_RENEWING) {
    res = 2;
  } else if (dhcp->state == DHCP_STATE_REBINDING) {
    res = 4;
  }

  return res;
}


/* -------------------------------------------------------------------------- */
void DHCP_task(struct netif *netif) {
/* -------------------------------------------------------------------------- */
  
  struct dhcp *dhcp;

  static DhcpState_t dhcp_state = D_DHCP_STATE_IDLE;

  switch(dhcp_state) {
    /* IDLE __________________________________________________________________*/
    case D_DHCP_STATE_IDLE:
      if(dhcp_cmd_start) {
        dhcp_state = D_DHCP_STATE_START;
      }
    break;
    /* START _________________________________________________________________*/
    case D_DHCP_STATE_START:
      if(!dhcp_cmd_start) {
        dhcp_state = D_DHCP_STATE_IDLE;
      }
      else if (netif_is_link_up(netif)) {

        ip_addr_set_zero_ip4(&netif->ip_addr);
        ip_addr_set_zero_ip4(&netif->netmask);
        ip_addr_set_zero_ip4(&netif->gw);
        
        dhcp_start(netif);
        
        dhcp_state = D_DHCP_STATE_WAIT_FOR_ADDRESS;
      }
      else 
    break;
    /* WAIT ADDRESS __________________________________________________________*/
    case D_DHCP_STATE_WAIT_FOR_ADDRESS:
      if (netif_is_link_up(netif)) {
        if (dhcp_supplied_address(netif)) {
          dhcp_state = D_DHCP_STATE_GOT_ADDRESS;
        } 
        else {
          /* TIMEOUT */
          dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
          if (dhcp->tries > MAX_DHCP_TRIES || dhcp_cmd_start == false) {
            /* go back to idle state and reset the command */
            dhcp_cmd_start = false;
            dhcp_state = D_DHCP_STATE_IDLE;
            dhcp_stop(netif);
          }
        }
      }
      else {
        dhcp_state = D_DHCP_STATE_START;
      }
    break;
    /* ADDRESS OBTAINED ______________________________________________________*/
    case D_DHCP_STATE_GOT_ADDRESS:
      if (netif_is_link_up(netif)) {
        if(dhcp_cmd_start == false) {
          /* Force release */
          dhcp_release(netif);
          dhcp_stop(netif);
          dhcp_state = D_DHCP_STATE_IDLE;
          dhcp_address_obtained = false;
        }
        else {
          dhcp_address_obtained = true;
        }
      
      }
      else {
        dhcp_stop(netif);
        dhcp_state = D_DHCP_STATE_START;
        dhcp_address_obtained = false;
      }
    break;
    default:
    break;
  }
}

#endif


/* -------------------------------------------------------------------------- */
/*                             DHCP FUNCTIONS                                 */
/* -------------------------------------------------------------------------- */
#if LWIP_DNS

void set_up_dns(const uint8_t *dnsaddr) {
  ip_addr_t ip;

  /* DNS initialized by DHCP when call dhcp_start() */
  if (!is_dhcp_started()) {
    dns_init();
    IP_ADDR4(&ip, dnsaddr[0], dnsaddr[1], dnsaddr[2], dnsaddr[3]);
    dns_setserver(0, &ip);
  }
}

void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
  if (ipaddr != NULL) {
    *((uint32_t *)callback_arg) = ip4_addr_get_u32(ipaddr);
  } else {
    *((uint32_t *)callback_arg) = 0;
  }
}


int8_t get_ip_address_from_hostname(const char *hostname, uint32_t *ipaddr)
{
  ip_addr_t iphost;
  err_t err;
  unsigned long dns_request_sent = 0;
  int8_t ret = 0;

  *ipaddr = 0;
  err = dns_gethostbyname(hostname, &iphost, &dns_callback, ipaddr);

  switch (err) {
    case ERR_OK:
      *ipaddr = ip4_addr_get_u32(&iphost);
      ret = 1;
      break;

    case ERR_INPROGRESS:
      dns_request_sent = millis();
      while (*ipaddr == 0) {
        lwip_task(); /* TODO */
        if ((millis() - dns_request_sent) >= TIMEOUT_DNS_REQUEST) {
          ret = -1;
          break;
        }
      }

      if (ret == 0) {
        if (*ipaddr == 0) {
          ret = -2;
        } else {
          ret = 1;
        }
      }
      break;

    case ERR_ARG:
      ret = -4;
      break;

    default:
      ret = -4;
      break;
  }

  return ret;
}
#endif
