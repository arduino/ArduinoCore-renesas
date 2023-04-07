#include "CNetIf.h"


/* -------------------------------------------------------------------------- */
CLwipIf::CLwipIf() {
/* -------------------------------------------------------------------------- */   
   /* Initialize lwIP stack, singletone implementation guarantees that lwip is
      initialized just once  */
   lwip_init();

   /* START THE TIMER FOR LWIP tasks - #CORE_DEPENDENT_STUFF */
   #ifdef LWIP_USE_TIMER
   uint8_t type = 8;
   uint8_t ch = FspTimer::get_available_timer(type);
   
   if(ch < 0) {
     ch = FspTimer::get_available_timer(type,true);
   } 

   timer.begin(TIMER_MODE_PERIODIC, type, ch, 10.0, 50.0, timer_cb);
   timer.setup_overflow_irq();
   timer.open();
   timer.start();
   #endif
}

/* -------------------------------------------------------------------------- */
void CLwipIf::lwip_task() {
/* -------------------------------------------------------------------------- */   

}

/* -------------------------------------------------------------------------- */
/* GET INSTANCE SINGLETONE FUNCTION */
/* -------------------------------------------------------------------------- */
CLwipIf& CLwipIf::getInstance() {
/* -------------------------------------------------------------------------- */   
   static CLwipIf    instance;
   return instance;
}

/* -------------------------------------------------------------------------- */
CLwipIf::~CLwipIf() {
/* -------------------------------------------------------------------------- */   
   for(int i = 0; i < NETWORK_INTERFACES_MAX_NUM; i++) {
      if(net_ifs[i] != nullptr) {
         delete net_ifs[i];
         net_ifs[i] = nullptr;
      }
   }
}


/* -------------------------------------------------------------------------- */
CNetIf *CLwipIf::setUpWifiStation(const uint8_t* _ip, 
                                  const uint8_t* _gw, 
                                  const uint8_t* _nm) {
/* -------------------------------------------------------------------------- */
  CNetIf *rv = nullptr;

  return rv;
}

/* -------------------------------------------------------------------------- */
CNetIf *CLwipIf::setUpWifiSoftAp(const uint8_t* _ip, 
                                 const uint8_t* _gw, 
                                 const uint8_t* _nm) {
/* -------------------------------------------------------------------------- */
  CNetIf *rv = nullptr;

  return rv;
}

/* -------------------------------------------------------------------------- */
CNetIf *CLwipIf::setUpEthernet(const uint8_t* _ip, 
                               const uint8_t* _gw, 
                               const uint8_t* _nm) {
/* -------------------------------------------------------------------------- */   
  CNetIf *rv = nullptr;

  return rv;
}


/* -------------------------------------------------------------------------- */
/* Sort of factory method, dependig on the requested type it setUp a different 
   Network interface and returns it to the caller */
/* -------------------------------------------------------------------------- */
CNetIf *CLwipIf::get(NetIfType_t type, 
                     const uint8_t* _ip, 
                     const uint8_t* _gw, 
                     const uint8_t* _nm) {
/* -------------------------------------------------------------------------- */
   CNetIf *rv = nullptr;
   switch(type) {
      case NI_WIFI_STATION:
         rv = setUpWifiStation(_ip, _gw, _nm);
         break;
      case NI_WIFI_SOFTAP:
         rv = setUpWifiSoftAp(_ip, _gw, _nm);
         break;

      case NI_ETHERNET:
         setUpEthernet(_ip, _gw, _nm);
         break;

      default:
         rv = nullptr;
         break;
   }
   return rv;
}




#ifdef LWIP_USE_TIMER
/* -------------------------------------------------------------------------- */
void CLwipIf::timer_cb(timer_callback_args_t *arg) {
/*  -------------------------------------------------------------------------- */   
  CLwipIf::getInstance().lwip_task();
} 
#endif


/* ########################################################################## */
/*                      BASE NETWORK INTERFACE CLASS                          */
/* ########################################################################## */

/* -------------------------------------------------------------------------- */
CNetIf::CNetIf() {
/* -------------------------------------------------------------------------- */   
   dhcp_acquired = false;
   dhcp_st = DHCP_IDLE;
   id = 0;
   #ifdef CNETWORK_INTERFACE_DEBUG
   Serial.println("[CNET]: CNetIf constructor");
   #endif
}

/* -------------------------------------------------------------------------- */
CNetIf::~CNetIf() {
/* -------------------------------------------------------------------------- */   
   #ifdef CNETWORK_INTERFACE_DEBUG
   Serial.println("[CNET]: CNetIf destructor");
   #endif

}


/* -------------------------------------------------------------------------- */
void CNetIf::setAddr(ip_addr_t *dst, const uint8_t* src) {
/* -------------------------------------------------------------------------- */   
   ip_addr_set_zero_ip4(dst);
   if(src != nullptr) {
      IP_ADDR4(dst, src[0], src[1], src[2], src[3]);
   }
}






/* -------------------------------------------------------------------------- */
bool CNetIf::isDhcpAcquired() { return dhcp_acquired; }
/* -------------------------------------------------------------------------- */
void CNetIf::DhcpStart() { dhcp_st = DHCP_START; }
/* -------------------------------------------------------------------------- */
void CNetIf::DhcpStop()  { 
   if(dhcp_st == DHCP_GOT && netif_is_link_up(getNi())) {
      dhcp_st = DHCP_RELEASE; 
   }
   else {
      dhcp_st = DHCP_STOP;
   }
}
/* -------------------------------------------------------------------------- */
void CNetIf::dhcp_task() {
/* -------------------------------------------------------------------------- */
   
   struct dhcp *lwip_dhcp;
   
   switch(dhcp_st){
      case DHCP_IDLE:
         /* nothing to do... wait for DhcpStart() to start the process */
      break;
      case DHCP_START:
         if(netif_is_link_up(getNi())) {
            ip_addr_set_zero_ip4(&(getNi()->ip_addr));
            ip_addr_set_zero_ip4(&(getNi()->netmask));
            ip_addr_set_zero_ip4(&(getNi()->gw));
            /* start lwIP dhcp */
            dhcp_start(getNi());
            dhcp_st = DHCP_WAIT;
         }
      break;
      case DHCP_WAIT:
         if(netif_is_link_up(getNi())) {
            if (dhcp_supplied_address(getNi())) {
               dhcp_st = DHCP_GOT;
            } 
            else {
               /* TIMEOUT */
               lwip_dhcp = (struct dhcp *)netif_get_client_data(getNi(), LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
               if (lwip_dhcp->tries > MAX_DHCP_TRIES) {
                 dhcp_st = DHCP_STOP;
               }
            }
         }
         else {
            dhcp_st = DHCP_START;
         }
      break;
      case DHCP_GOT:
         if (!dhcp_supplied_address(getNi())) {
            dhcp_st = DHCP_GOT;
         } 

      break;
      case DHCP_RELEASE:
         dhcp_release(getNi());
         dhcp_st = DHCP_STOP;
      break;
      case DHCP_STOP:
         dhcp_stop(getNi());
         dhcp_st = DHCP_IDLE;
      break;

   }
}








