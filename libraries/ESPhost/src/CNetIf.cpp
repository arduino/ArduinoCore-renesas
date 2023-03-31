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
   for (auto& [name, ni] : net_ifs) {
      if(ni != nullptr) {
         delete ni;
      }  
   }
}

/* Add an interface: the interface can be retrieved with its name             */
/* -------------------------------------------------------------------------- */
CNetIf *CLwipIf::add(string name, 
                     const uint8_t* _ip, 
                     const uint8_t* _gw, 
                     const uint8_t* _nm, 
                     LwipInit_f init, 
                     LwipInput_f input) {
/* -------------------------------------------------------------------------- */
   /* 1. check if the name has already been used */
   if(net_ifs.find(name) != net_ifs.end()) {
      /* the name of the interface is already present ! */
      return nullptr;
   }
   /* 2. allocate a new Network interface */
   CNetIf *ni = new CNetIf();
   
   if(ni != nullptr) {
      /* not really sure if save the addresses is necessary or not... */
      
      /* 3. set addresses */
      ni->setIp(_ip);
      ni->setGw(_gw);
      ni->setNm(_nm);
      
      /* 4. add network interface to lwIP */
      if(nullptr == netif_add(ni->getNi(), 
                              ni->getIp(),
                              ni->getGw(),
                              ni->getNm(),
                              NULL,
                              init,
                              input)) {
         /* 4.1 something went wrong, free memory */
         delete ni;
         ni = nullptr;
      }
      else {
         /* 5. add ni to map */
         net_ifs.insert({name,ni});
      }
   }
   return ni;
}

/* -------------------------------------------------------------------------- */
CNetIf *CLwipIf::add(string name, 
                     LwipInit_f init, 
                     LwipInput_f input) {
/* -------------------------------------------------------------------------- */   
   CNetIf *ni = add(name,nullptr,nullptr,nullptr, init, input);
   if(ni != nullptr) {
      ni->DhcpStart();
   }
}

#ifdef LWIP_USE_TIMER
/* -------------------------------------------------------------------------- */
void CLwipIf::timer_cb(timer_callback_args_t *arg) {
/*  -------------------------------------------------------------------------- */   
  CLwipIf::getInstance().lwip_task();
} 
#endif


/* ########################################################################## */
/*                         NETWORK INTERFACE CLASS                            */
/* ########################################################################## */


/*  ----------------------
 *  DHCP related functions
 *  ---------------------- */ 



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


/* -------------------------------------------------------------------------- */
void CNetIf::setAddr(ip_addr_t *dst, const uint8_t* src) {
/* -------------------------------------------------------------------------- */   
   ip_addr_set_zero_ip4(dst);
   if(src != nullptr) {
      IP_ADDR4(dst, src[0], src[1], src[2], src[3]);
   }
}






/* -------------------------------------------------------------------------- */
CNetIf::CNetIf() {
/* -------------------------------------------------------------------------- */   
   dhcp_acquired = false;
   dhcp_st = DHCP_IDLE;
}