#include "eth0.h"
#include "../arch/ethernetDriver.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*                          ETHERNET 0 INTERFACE                              */
/* -------------------------------------------------------------------------- */
/* Defaul addresses for ip, netmask and gateway (can be customized in eth0.h) */
const uint8_t default_ip4_address[4] = {IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3};
const uint8_t default_ip4_netmask[4] = {NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3};
const uint8_t default_ip4_gateway[4] = {GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3};
/* -------------------------------------------------------------------------- */
/* Netif global configuration used by LwIP                                    */
struct netif eth0if;
static bool eth0if_initialized = false;
netif *eth0if_get_ptr() { return &eth0if; }
bool is_eth0if_initialized() { return eth0if_initialized; }
/* Real addresses passed to netif_add LwIP call                               */
static ip_addr_t ip_address;
static ip_addr_t nm_address;
static ip_addr_t gw_address;
/* -------------------------------------------------------------------------- */
/* transmission buffer                                                        */
static uint8_t eth0_tx_buffer[ETH_BUFF_DIM];
/* -------------------------------------------------------------------------- */

LinkChangeCbk_f eth0_link_changed = nullptr;
void eth0if_set_link_change_cbk(LinkChangeCbk_f fn) {
  eth0_link_changed = fn;
}


static void set_ip4_address(ip_addr_t *ipadd, const uint8_t *add,const uint8_t *def_add) {
  if (add != NULL) {
    IP_ADDR4(ipadd, add[0], add[1], add[2], add[3]);
  } 
  else {
    #if LWIP_DHCP
        ip_addr_set_zero_ip4(ipadd);
    #else
        IP_ADDR4(ipadd, def_add[0], def_add[1], def_add[2], def_add[3]);
    #endif /* LWIP_DHCP */
  }
}

void eth0if_set_ip4_address(const uint8_t *ad) { set_ip4_address(&ip_address, ad,default_ip4_address);}
void eth0if_set_ip4_netmask(const uint8_t *nm) { set_ip4_address(&nm_address, nm,default_ip4_netmask);}
void eth0if_set_ip4_gateway(const uint8_t *gw) { set_ip4_address(&gw_address, gw,default_ip4_gateway);}
void eth0if_set_mac_address(const uint8_t *mad){ 
  if(mad != nullptr)
    eth_set_mac_address(mad); 
}


static err_t eth0if_output(struct netif *netif, struct pbuf *p) {
  
  Serial.println("eth0if_output");

  err_t errval = ERR_OK;
  struct pbuf *q;
  
  /* copy frame from pbufs to driver buffers */
  for (q = p; q != NULL; q = q->next) {
    /* according to comments in lwip pbuf.h a single packet may be queued in more
       than 1 pbuf (pbuf chain)
       the packet is completed when ->tot_len is equal to ->len
       since ->tot_len is "total length of this buffer and all next buffers in chain
       belonging to the same packet." 
       */

    bool packed_completed = false;
    /* this is the first pbuf of the current packet -> take note of the total 
       lenght of the packet */
    uint16_t bytes_to_be_copied = q->tot_len;
    uint16_t bytes_actually_copied = 0;
    if(bytes_to_be_copied > ETH_BUFF_DIM) {
      /* TODO -> HANDLE ERROR */
      errval = ERR_MEM;
      break;
    }

    while(!packed_completed && q != NULL) {
      if(bytes_actually_copied + q->len < ETH_BUFF_DIM) {
        memcpy( (eth0_tx_buffer + bytes_actually_copied), q->payload, q->len);
        bytes_actually_copied += q->len;
        if(q->tot_len == q->len) {
          /* all the pbuf containing a packet have been copied -> the packet can be
           sent */
          packed_completed = true;
        }
        else {
          q = q->next;
        }
      }
      else {
        errval = ERR_BUF;
      }
    } 

    if(bytes_to_be_copied != bytes_actually_copied) {
      /* TODO -> HANDLE ERROR */
      errval = ERR_BUF;
      break;
    }
  
    if(!eth_output(eth0_tx_buffer, bytes_actually_copied)) {
      errval = ERR_IF;
      break;
    }
  }
  return errval;
}

void pippo() {
  Serial.println("???????????????????????????????????????????????????????");
}


/* -------------------------------------------------------------------------- */
void eth0if_input(struct netif *netif) {
/* -------------------------------------------------------------------------- */  
  Serial.println("eth0if_input");

  err_t err;
  struct pbuf *q;
  struct pbuf *p = NULL;

   
  uint32_t rx_frame_dim = 0;
  uint8_t *rx_frame_buf = eth_input(&rx_frame_dim);
  /* zero copy mode is used */
  if(rx_frame_buf != nullptr) {
    if(rx_frame_dim > 0) {
      Serial.print("FRAME ");
      Serial.println(rx_frame_dim);
      Serial.write(rx_frame_buf, rx_frame_dim);
      p = pbuf_alloc(PBUF_RAW, (uint16_t)rx_frame_dim, PBUF_POOL);
    
      if(p != NULL && rx_frame_buf != nullptr) {
        q = p;
        uint16_t bytes_actually_copied = 0;
        bool copied_finished = false;

        while(!copied_finished && q != NULL)  {
          memcpy(q->payload, rx_frame_buf + bytes_actually_copied, q->len);
          bytes_actually_copied += q->len;
          if(bytes_actually_copied >= rx_frame_dim) {
            copied_finished = true;
          }
          q = q->next;
        }
      }
      else {
        /* no packet could be read, silently ignore this */
        return;
      }
      eth_release_rx_buffer();

      Serial.println("<<<<<<<--- INPUT ");
      err = netif->input(p, netif);
    }
  }

  /* entry point to the LwIP stack */
  

  if (err != ERR_OK) {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
    pbuf_free(p);
    p = NULL;
  }
}

/* -------------------------------------------------------------------------- */
void eth0if_frame_received() {
/* -------------------------------------------------------------------------- */  
  Serial.println("eth0if_frame_received");
  eth0if_input(&eth0if);
}

/* -------------------------------------------------------------------------- */
static void eth0if_link_up() {
/* -------------------------------------------------------------------------- */  
  Serial.println("----- LINK UP ");


  //eth_execute_link_process();

  netif_set_link_up(&eth0if);
  /* When the netif is fully configured this function must be called.*/
  netif_set_up(&eth0if);

  #if LWIP_IGMP
  if (!(eth0if.flags & NETIF_FLAG_IGMP)) {
    eth0if.flags |= NETIF_FLAG_IGMP;
    igmp_init();
    igmp_start(&eth0if);
  }
  #endif
  if(eth0_link_changed != nullptr) {
    eth0_link_changed(true);
  }
}

/* -------------------------------------------------------------------------- */
static void eth0if_link_down() {
/* -------------------------------------------------------------------------- */  
  Serial.println("----- LINK DOWN ");

  //eth_execute_link_process();

  netif_set_link_down(&eth0if);
  /*  When the netif link is down this function must be called.*/
  netif_set_down(&eth0if);
  if(eth0_link_changed != nullptr) {
    eth0_link_changed(false);
  }
}

/* this is the function to be passed to the netif_add as input function */
/* -------------------------------------------------------------------------- */
static err_t eth0if_init(struct netif *netif) {
/* -------------------------------------------------------------------------- */  
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = eth0if_output;

  /* set MAC hardware address */
  netif->hwaddr_len = eth_get_mac_address(netif->hwaddr);

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

  /* set the callback function that is called when an ethernet frame is physically
     received, it is important that the callbacks are set before the initializiation */
  eth_set_rx_frame_cbk(eth0if_frame_received);
  eth_set_link_on_cbk(eth0if_link_up);
  eth_set_link_off_cbk(eth0if_link_down);


  /* initialize the hardware */
  eth_init();
  

  eth0if_initialized = true;

  return ERR_OK;
}

/* this function is called by lwip when the drivers notify lwip that eth0 is
   "gone" up or down 
   the use of this function in our case is quite silly 
   the function will be called by lwip when netif_set_link_up or netif_set_link_down
   are called but those function are called in eth0if_link_up end eth0if_link_down
   so it makes no sense tu use it */
#if LWIP_NETIF_LINK_CALLBACK
void eht0if_link_toggle_cbk(struct netif *netif) {
  
}
#endif



/* sort of constructor of eth0if */
/* -------------------------------------------------------------------------- */
void eth0if_lwip_config(bool is_default) {
/* -------------------------------------------------------------------------- */  
  netif_remove(&eth0if);
  /* Add the network interface */
  netif_add(&eth0if, &ip_address, &nm_address, &gw_address, NULL, eth0if_init, ethernet_input);

  /* Registers the default network interface */
  if(is_default) {
    netif_set_default(&eth0if);
  }

  /* QUESTA ROBA NON SERVE A UN TUBO*/
  if (netif_is_link_up(&eth0if)) {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&eth0if);
  } else {
    /* When the netif link is down this function must be called */
    netif_set_down(&eth0if);
  }

  #if LWIP_NETIF_LINK_CALLBACK
    /* Set the link callback function, this function is called on change of link status */
     netif_set_link_callback(&eth0if, eht0if_link_toggle_cbk);
  #endif /* LWIP_NETIF_LINK_CALLBACK */


}

#ifdef __cplusplus
}
#endif