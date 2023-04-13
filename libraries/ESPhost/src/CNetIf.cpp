#include "CNetIf.h"

const uint8_t default_eth_ip[4] = {IP_ETH_0, IP_ETH_1, IP_ETH_2, IP_ETH_3};
const uint8_t default_eth_nm[4] = {NM_ETH_0, NM_ETH_1, NM_ETH_2, NM_ETH_3};
const uint8_t default_eth_gw[4] = {GW_ETH_0, GW_ETH_1, GW_ETH_2, GW_ETH_3};

const uint8_t default_wst_ip[4] = {IP_WST_0, IP_WST_1, IP_WST_2, IP_WST_3};
const uint8_t default_wst_nm[4] = {NM_WST_0, NM_WST_1, NM_WST_2, NM_WST_3};
const uint8_t default_wst_gw[4] = {GW_WST_0, GW_WST_1, GW_WST_2, GW_WST_3};


const uint8_t default_wsa_ip[4] = {IP_WSA_0, IP_WSA_1, IP_WSA_2, IP_WSA_3};
const uint8_t default_wsa_nm[4] = {NM_WSA_0, NM_WSA_1, NM_WSA_2, NM_WSA_3};
const uint8_t default_wsa_gw[4] = {GW_WSA_0, GW_WSA_1, GW_WSA_2, GW_WSA_3};

CNetIf * CLwipIf::net_ifs[] = {nullptr};
bool CLwipIf::wifi_hw_initialized = false; 
bool CLwipIf::connected_to_access_point = false; 
WifiStatus_t CLwipIf::wifi_status = WL_IDLE_STATUS;

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
   if(net_ifs[NI_ETHERNET]  != nullptr) {
      net_ifs[NI_ETHERNET]->task();
   }

   if(net_ifs[NI_WIFI_STATION]  != nullptr) {
      net_ifs[NI_WIFI_STATION]->task();
   }

   if(net_ifs[NI_WIFI_SOFTAP]  != nullptr) {
      net_ifs[NI_WIFI_SOFTAP]->task();
   }

   /* Handle LwIP timeouts */
   sys_check_timeouts();
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


/* This function returns a newly allocate Network interface if the interface 
   was not allocated before or nullptr if the Network Interface was already 
   allocated */
/* -------------------------------------------------------------------------- */
CNetIf *CLwipIf::_get(NetIfType_t t) {
/* -------------------------------------------------------------------------- */   
   CNetIf *rv = nullptr;
   if(t >= 0 && t < NETWORK_INTERFACES_MAX_NUM) {
      if(net_ifs[t] == nullptr) {
         switch(t) {
            case NI_WIFI_STATION:
               rv = new CWifiStation();
            break;
            case NI_WIFI_SOFTAP:
               rv = new CWifiSoftAp();
            break;

            case NI_ETHERNET:
               rv = new CEthernet();
            break;
            default:
            break;
         }
         net_ifs[t] = rv;
      }
   }
   return rv;
}





/* -------------------------------------------------------------------------- */
CNetIf *CLwipIf::setUpEthernet(const uint8_t* _ip, 
                               const uint8_t* _gw, 
                               const uint8_t* _nm) {
/* -------------------------------------------------------------------------- */   
  /* get return nullptr if ni was already allocated */
  CNetIf *rv = _get(NI_ETHERNET);

  if(rv != nullptr) {
      

      
      
  }

  return rv;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::disconnectEventcb(CCtrlMsgWrapper *resp) {
   if(CLwipIf::connected_to_access_point) {
      wifi_status = WL_DISCONNECTED;
   }
}


/* -------------------------------------------------------------------------- */
int CLwipIf::initEventCb(CCtrlMsgWrapper *resp) {
   wifi_hw_initialized = true;
}


/* -------------------------------------------------------------------------- */
void CLwipIf::initWifiHw(bool asStation) {
/* -------------------------------------------------------------------------- */   
   if(!wifi_hw_initialized) {
      
      CEspControl::getInstance().listenForStationDisconnectEvent(disconnectEventcb);
      CEspControl::getInstance().listenForInitEvent(initEventCb);
      if(CEspControl::getInstance().initSpiDriver() == 0) {
         wifi_status = WL_NO_SSID_AVAIL;
      }

      int time_num = 0;
      while(time_num < WIFI_INIT_TIMEOUT_MS && !wifi_hw_initialized) {
         R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
         time_num++;
      }

      if(asStation) {
         CEspControl::getInstance().setWifiMode(WIFI_MODE_STA);
      }
      else {
         CEspControl::getInstance().setWifiMode(WIFI_MODE_AP);
      }
   }
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
         rv = _get(NI_WIFI_STATION);
         if(rv != nullptr) {
            initWifiHw(true);
            rv->begin(_ip,_gw,_nm);
            /* id is set up based on the presence of the 'other' wifi interface */
            if(net_ifs[NI_WIFI_SOFTAP] != nullptr) {
               rv->setId(1);
            }
            else {
               rv->setId(0);
            }
         }
         break;
      case NI_WIFI_SOFTAP:
         rv = _get(NI_WIFI_SOFTAP);
         if(rv != nullptr) {
            initWifiHw(false);
            rv->begin(_ip,_gw,_nm);
            /* id is set up based on the presence of the 'other' wifi interface */
            if(net_ifs[NI_WIFI_STATION] != nullptr) {
               rv->setId(1);
            }
            else {
               rv->setId(0);
            }
         }

         break;

      case NI_ETHERNET:
         rv = setUpEthernet(_ip, _gw, _nm);
         break;

      default:
         rv = nullptr;
         break;
   }
   return rv;
}



/* -------------------------------------------------------------------------- */
err_t CLwipIf::initEth(struct netif* _ni) {
/* -------------------------------------------------------------------------- */   
   #if LWIP_NETIF_HOSTNAME
   /* Initialize interface hostname */
   _ni->hostname = "C33-onEth";
   #endif /* LWIP_NETIF_HOSTNAME */

   _ni->name[0] = ETH_IFNAME0;
   _ni->name[1] = ETH_IFNAME1;
   /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
   _ni->output = etharp_output;
   //_ni->linkoutput = eth0if_output;

   /* set MAC hardware address */
   //_ni->hwaddr_len = eth_get_mac_address(netif->hwaddr);

   /* maximum transfer unit */
   _ni->mtu = 1500;

   /* device capabilities */
   /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
   _ni->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

   /* set the callback function that is called when an ethernet frame is physically
      received, it is important that the callbacks are set before the initializiation */
   //eth_set_rx_frame_cbk(eth0if_frame_received);
   //eth_set_link_on_cbk(eth0if_link_up);
   //eth_set_link_off_cbk(eth0if_link_down);


   /* initialize the hardware */
   //eth_init();
  

   return ERR_OK;

}



/* -------------------------------------------------------------------------- */
err_t CLwipIf::ouputEth(struct netif* _ni, struct pbuf *p) {
/* -------------------------------------------------------------------------- */   
  err_t errval = ERR_OK;
  //assert (p->tot_len <= ETH_BUFF_DIM);
  //uint16_t bytes_actually_copied = pbuf_copy_partial(p, eth0_tx_buffer, p->tot_len, 0);
  //if(bytes_actually_copied > 0) {
    //if(!eth_output(eth0_tx_buffer, bytes_actually_copied)) {
      //errval = ERR_IF;
    //}
  //}
  return errval;
   
}

/* -------------------------------------------------------------------------- */
err_t CLwipIf::outputWifiStation(struct netif* _ni, struct pbuf *p) {
/* -------------------------------------------------------------------------- */   
   err_t errval = ERR_IF;
  
   uint8_t *buf = new uint8_t[p->tot_len];

   if(buf != nullptr) {
      uint16_t bytes_actually_copied = pbuf_copy_partial(p, buf, p->tot_len, 0);
      if(bytes_actually_copied > 0) {
         int ifn = 0;
         if(net_ifs[NI_WIFI_STATION] != nullptr) {
            ifn = net_ifs[NI_WIFI_STATION]->getId();
         }
         if(CEspControl::getInstance().sendBuffer(ESP_STA_IF, ifn, buf, bytes_actually_copied) == ESP_CONTROL_OK) {
            errval = ERR_OK;
         }
      }
      delete []buf;
   }

   return errval;
}

/* -------------------------------------------------------------------------- */
err_t CLwipIf::initWifiStation(struct netif* _ni) {
/* -------------------------------------------------------------------------- */
   #if LWIP_NETIF_HOSTNAME
   /* Initialize interface hostname */
   _ni->hostname = "C33-WifiSta";
   #endif /* LWIP_NETIF_HOSTNAME */

   _ni->name[0] = WST_IFNAME0;
   _ni->name[1] = WST_IFNAME1;
   /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
   _ni->output = etharp_output;
   _ni->linkoutput = outputWifiStation;

   /* maximum transfer unit */
   _ni->mtu = 1500;

   /* device capabilities */
   /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
   _ni->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

   /* set MAC hardware address */
   _ni->hwaddr_len = CLwipIf::getInstance().getMacAddress(NI_WIFI_STATION, _ni->hwaddr);
  

   return ERR_OK;
}



/* -------------------------------------------------------------------------- */
err_t CLwipIf::outputWifiSoftAp(struct netif* _ni, struct pbuf *p) {
/* -------------------------------------------------------------------------- */   
   err_t errval = ERR_IF;
  
   uint8_t *buf = new uint8_t[p->tot_len];

   if(buf != nullptr) {
      uint16_t bytes_actually_copied = pbuf_copy_partial(p, buf, p->tot_len, 0);
      if(bytes_actually_copied > 0) {
         int ifn = 0;
         if(net_ifs[NI_WIFI_SOFTAP] != nullptr) {
            ifn = net_ifs[NI_WIFI_SOFTAP]->getId();
         }

         if(CEspControl::getInstance().sendBuffer(ESP_AP_IF, ifn, buf, bytes_actually_copied) == ESP_CONTROL_OK) {
            errval = ERR_OK;
         }
      }
      delete []buf;
   }

   return errval;
}





/* -------------------------------------------------------------------------- */
err_t CLwipIf::initWifiSoftAp(struct netif* _ni) {
/* -------------------------------------------------------------------------- */
   #if LWIP_NETIF_HOSTNAME
   /* Initialize interface hostname */
   _ni->hostname = "C33-WifiSta";
   #endif /* LWIP_NETIF_HOSTNAME */

   _ni->name[0] = WSA_IFNAME0;
   _ni->name[1] = WSA_IFNAME1;
   /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
   _ni->output = etharp_output;
   _ni->linkoutput = outputWifiSoftAp;

   /* maximum transfer unit */
   _ni->mtu = 1500;

   /* device capabilities */
   /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
   _ni->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

   

   /* set MAC hardware address */
   _ni->hwaddr_len = CLwipIf::getInstance().getMacAddress(NI_WIFI_SOFTAP, _ni->hwaddr);
}



/* -------------------------------------------------------------------------- */
bool CLwipIf::setMacAddress(NetIfType_t type, uint8_t* mac) {
/* -------------------------------------------------------------------------- */   


   if(type == NI_WIFI_STATION) {

   }
   else if(type == NI_WIFI_SOFTAP) {

   }
   else {

   }
}

/* -------------------------------------------------------------------------- */
int CLwipIf::getMacAddress(NetIfType_t type, uint8_t* mac) {
/* -------------------------------------------------------------------------- */   
   int rv = 0;
   WifiMac_t MAC;
   int res = CEspControl::getInstance().getWifiMacAddress(MAC);
   if(type == NI_WIFI_STATION) {
      MAC.mode = WIFI_MODE_STA;
      if(CEspControl::getInstance().getWifiMacAddress(MAC) == ESP_CONTROL_OK) {
         CNetUtilities::macStr2macArray(mac, MAC.mac);
         rv = MAC_ADDRESS_DIM;
      }
   }
   else if(type == NI_WIFI_SOFTAP) {
      MAC.mode = WIFI_MODE_AP;
      if(CEspControl::getInstance().getWifiMacAddress(MAC) == ESP_CONTROL_OK) {
         CNetUtilities::macStr2macArray(mac, MAC.mac);
         rv = MAC_ADDRESS_DIM;
      }
   }
   else {
      // TODO
      rv = MAC_ADDRESS_DIM;
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::scanForAp() {
/* -------------------------------------------------------------------------- */   
   access_points.clear();
   
   int res = CEspControl::getInstance().getAccessPointScanList(access_points);
   if(res == ESP_CONTROL_OK) {
      wifi_status = WL_SCAN_COMPLETED;
   }
   else {
      wifi_status = WL_NO_SSID_AVAIL;
   }
}

/* -------------------------------------------------------------------------- */
int CLwipIf::getApNum() { return access_points.size(); }
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
const char *CLwipIf::getSSID(uint8_t i){
/* -------------------------------------------------------------------------- */   
   if(access_points.size() > 0 && i < access_points.size()) {
      return (const char *)access_points[i].ssid;
   }
   return nullptr;
}

/* -------------------------------------------------------------------------- */
int32_t CLwipIf::getRSSI(uint8_t i) {
/* -------------------------------------------------------------------------- */   
   if(access_points.size() > 0 && i < access_points.size()) {
      return (int32_t)access_points[i].rssi;
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
uint8_t CLwipIf::getEncrType(uint8_t i){
/* -------------------------------------------------------------------------- */   
   if(access_points.size() > 0 && i < access_points.size()) {
      return (uint8_t)access_points[i].encryption_mode;
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
uint8_t *CLwipIf::getBSSID(uint8_t i, uint8_t *bssid) {
/* -------------------------------------------------------------------------- */   
   if(access_points.size() > 0 && i < access_points.size()) {
      CNetUtilities::macStr2macArray(bssid, (const char*)access_points[i].bssid);
      return bssid;
   }
   return nullptr;
}

/* -------------------------------------------------------------------------- */
uint8_t CLwipIf::getChannel(uint8_t i) {
/* -------------------------------------------------------------------------- */   
   if(access_points.size() > 0 && i < access_points.size()) {
      return (uint8_t)access_points[i].channel;
   }
   return 0;
}
/* -------------------------------------------------------------------------- */
int CLwipIf::connectToAp(const char *ssid, const char *pwd) {
/* -------------------------------------------------------------------------- */ 
   WifiApCfg_t ap;
    
   bool found = false;
   uint8_t index = 0;
   for(uint8_t i = 0; i < access_points.size(); i++) {
      if(strcmp(ssid, (const char *)access_points[i].ssid) == 0) {
         found = true;
         index = i;
      }
   }

   if(found) {
      memset(ap.ssid,0x00,SSID_LENGTH);
      memcpy(ap.ssid,access_points[index].ssid,SSID_LENGTH);
      memset(ap.pwd,0x00,PASSWORD_LENGTH);
      if(pwd != nullptr) {
         memcpy(ap.pwd,pwd, (strlen(pwd) < PASSWORD_LENGTH) ? strlen(pwd) : PASSWORD_LENGTH) ;
      }
      memset(ap.bssid,0x00,BSSID_LENGTH);
      memcpy(ap.bssid,access_points[index].bssid,BSSID_LENGTH);
   }

   if(CEspControl::getInstance().connectAccessPoint(ap) == ESP_CONTROL_OK) {
      CLwipIf::connected_to_access_point = true;
      wifi_status = WL_CONNECTED;
      CEspControl::getInstance().getAccessPointConfig(access_point_cfg);
      return ESP_CONTROL_OK;
   }
   else {
      wifi_status = WL_CONNECT_FAILED;
      CLwipIf::connected_to_access_point = false;
   }
   return ESP_CONTROL_CTRL_ERROR;
}

/* -------------------------------------------------------------------------- */
const char* CLwipIf::getSSID() {
/* -------------------------------------------------------------------------- */   
   return (const char *)access_point_cfg.ssid;
}

/* -------------------------------------------------------------------------- */
uint8_t *CLwipIf::getBSSID(uint8_t *bssid) {
/* -------------------------------------------------------------------------- */   
   CNetUtilities::macStr2macArray(bssid, (const char*)access_point_cfg.bssid);
   return bssid;
}

/* -------------------------------------------------------------------------- */
uint32_t CLwipIf::getRSSI() {
/* -------------------------------------------------------------------------- */   
   return (uint32_t)access_point_cfg.rssi;
}

/* -------------------------------------------------------------------------- */
uint8_t CLwipIf::getEncrType() {
/* -------------------------------------------------------------------------- */   
   return (uint8_t)access_point_cfg.encryption_mode;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::disconnectFromAp() {
/* -------------------------------------------------------------------------- */   
   wifi_status = WL_DISCONNECTED;
   return CEspControl::getInstance().disconnectAccessPoint();
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
   memset(hostname,0x00,MAX_HOSTNAME_DIM);
   hostname[0] = 'C';
   hostname[1] = '3';
   hostname[2] = '3';
   #if LWIP_NETIF_HOSTNAME
   ni.hostname = (const char *)&hostname;
   #endif
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
void CNetIf::setAddr(ip_addr_t *dst, const uint8_t* src, const uint8_t* def) {
/* -------------------------------------------------------------------------- */   
   ip_addr_set_zero_ip4(dst);
   if(src != nullptr) {
      IP_ADDR4(dst, src[0], src[1], src[2], src[3]);
   }
   else {
      IP_ADDR4(dst, def[0], def[1], def[2], def[3]);
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


/* ########################################################################## */
/*                      ETHERNET NETWORK INTERFACE CLASS                      */
/* ########################################################################## */
CEthernet::CEthernet() {}
CEthernet::~CEthernet() {}



void CEthernet::setIp(const uint8_t *_ip) {
   setAddr(&ip,_ip, (const uint8_t*)&default_eth_ip);
}

void CEthernet::setNm(const uint8_t *_nm) {
   setAddr(&ip,_nm, (const uint8_t*)&default_eth_ip);
}

void CEthernet::setGw(const uint8_t *_gw) {
   setAddr(&ip,_gw, (const uint8_t*)&default_eth_ip);
}

/* -------------------------------------------------------------------------- */
void CEthernet::begin(const uint8_t* _ip, 
                      const uint8_t* _gw, 
                      const uint8_t* _nm) {
/* -------------------------------------------------------------------------- */   
   setIp(_ip);
   setGw(_gw);
   setNm(_nm);

   netif_add(&ni, getIp(), getNm(), getGw(), NULL, CLwipIf::initEth, ethernet_input);

  /* Registers the default network interface */
  //if(is_default) {
    //netif_set_default(&ni);
  //}

  if (netif_is_link_up(&ni)) {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&ni);
  } else {
    /* When the netif link is down this function must be called */
    netif_set_down(&ni);
  }

  #if LWIP_NETIF_LINK_CALLBACK
    /* Set the link callback function, this function is called on change of link status */
     //netif_set_link_callback(&eth0if, eht0if_link_toggle_cbk);
  #endif /* LWIP_NETIF_LINK_CALLBACK */
}

/* -------------------------------------------------------------------------- */
void CEthernet::task() {
/* -------------------------------------------------------------------------- */   


}

/* ########################################################################## */
/*                    CWifiStation NETWORK INTERFACE CLASS                    */
/* ########################################################################## */

CWifiStation::CWifiStation() {}
CWifiStation::~CWifiStation() {}

void CWifiStation::setIp(const uint8_t *_ip) {
   setAddr(&ip,_ip, (const uint8_t*)&default_wst_ip);
}

void CWifiStation::setNm(const uint8_t *_nm) {
   setAddr(&ip,_nm, (const uint8_t*)&default_wst_nm);
}

void CWifiStation::setGw(const uint8_t *_gw) {
   setAddr(&ip,_gw, (const uint8_t*)&default_wst_gw);
}

void CWifiStation::begin(const uint8_t* _ip, 
                      const uint8_t* _gw, 
                      const uint8_t* _nm) {
   setIp(_ip);
   setGw(_gw);
   setNm(_nm);

   netif_add(&ni,  getIp(), getNm(), getGw(), NULL, CLwipIf::initWifiStation, ethernet_input);

  /* Registers the default network interface */
  //if(is_default) {
    //netif_set_default(&ni);
  //}

  if (netif_is_link_up(&ni)) {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&ni);
  } else {
    /* When the netif link is down this function must be called */
    netif_set_down(&ni);
  }

  #if LWIP_NETIF_LINK_CALLBACK
    /* Set the link callback function, this function is called on change of link status */
     //netif_set_link_callback(&eth0if, eht0if_link_toggle_cbk);
  #endif /* LWIP_NETIF_LINK_CALLBACK */
   
}

/* -------------------------------------------------------------------------- */
void CWifiStation::task() {
/* -------------------------------------------------------------------------- */   
   /* get messages and process it  */
   uint8_t if_num;
   uint16_t dim;

   /* shall we verify something about if_num??? */

   uint8_t *buf = CEspControl::getInstance().getStationRx(if_num, dim);

   struct pbuf* p = pbuf_alloc(PBUF_RAW, dim, PBUF_RAM);
   if(p != NULL) {
      /* Copy ethernet frame into pbuf */
      pbuf_take((struct pbuf* )p, (uint8_t *) buf, (uint32_t)dim);
      delete []buf;

      if(ni.input(p, &ni) != ERR_OK) {
         pbuf_free(p);
      }
   }

   #if LWIP_DHCP
   static unsigned long dhcp_last_time_call = 0;
   if(dhcp_last_time_call == 0 || millis() - dhcp_last_time_call > DHCP_FINE_TIMER_MSECS) {
     dhcp_task();
   }
   #endif

}

/* -------------------------------------------------------------------------- */
int CWifiStation::getMacAddress(uint8_t *mac) {
/* -------------------------------------------------------------------------- */   
   WifiMac_t MAC;
   MAC.mode = WIFI_MODE_STA;
   int rv = CEspControl::getInstance().getWifiMacAddress(MAC);
   CNetUtilities::macStr2macArray(mac, (const char*)MAC.mac);
   return rv;
}


/* ########################################################################## */
/*                      CWifiSoftAp NETWORK INTERFACE CLASS                   */
/* ########################################################################## */

CWifiSoftAp::CWifiSoftAp() {}
CWifiSoftAp::~CWifiSoftAp() {}

void CWifiSoftAp::setIp(const uint8_t *_ip) {
   setAddr(&ip,_ip,(const uint8_t*)&default_wsa_ip);
}

void CWifiSoftAp::setNm(const uint8_t *_nm) {
   setAddr(&nm,_nm, (const uint8_t*)&default_wsa_nm);
}

void CWifiSoftAp::setGw(const uint8_t *_gw) {
   setAddr(&gw,_gw, (const uint8_t*)&default_wsa_gw);
}

void CWifiSoftAp::begin(const uint8_t* _ip, 
                      const uint8_t* _gw, 
                      const uint8_t* _nm) {
   setIp(_ip);
   setGw(_gw);
   setNm(_nm);

   netif_add(&ni,  getIp(), getNm(), getGw(), NULL, CLwipIf::initWifiSoftAp, ethernet_input);

  /* Registers the default network interface */
  //if(is_default) {
    //netif_set_default(&ni);
  //}

  if (netif_is_link_up(&ni)) {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&ni);
  } else {
    /* When the netif link is down this function must be called */
    netif_set_down(&ni);
  }

  #if LWIP_NETIF_LINK_CALLBACK
    /* Set the link callback function, this function is called on change of link status */
     //netif_set_link_callback(&eth0if, eht0if_link_toggle_cbk);
  #endif /* LWIP_NETIF_LINK_CALLBACK */
}


/* -------------------------------------------------------------------------- */
void CWifiSoftAp::task() {
/* -------------------------------------------------------------------------- */   
   /* get messages and process it  */
   uint8_t if_num;
   uint16_t dim;
   uint8_t *buf = CEspControl::getInstance().getSoftApRx(if_num, dim);
  
   struct pbuf* p = pbuf_alloc(PBUF_RAW, dim, PBUF_RAM);
   if(p != NULL) {
      /* Copy ethernet frame into pbuf */
      pbuf_take((struct pbuf* )p, (uint8_t *) buf, (uint32_t)dim);
      delete []buf;

      if(ni.input(p, &ni) != ERR_OK) {
         pbuf_free(p);
      }
   }

   #if LWIP_DHCP
   static unsigned long dhcp_last_time_call = 0;
   if(dhcp_last_time_call == 0 || millis() - dhcp_last_time_call > DHCP_FINE_TIMER_MSECS) {
     dhcp_task();
   }
   #endif
}

/* -------------------------------------------------------------------------- */
int CWifiSoftAp::getMacAddress(uint8_t *mac) {
/* -------------------------------------------------------------------------- */   
   WifiMac_t MAC;
   MAC.mode = WIFI_MODE_AP;
   int rv = CEspControl::getInstance().getWifiMacAddress(MAC);
   CNetUtilities::macStr2macArray(mac, (const char*)MAC.mac);
   return rv;
}
