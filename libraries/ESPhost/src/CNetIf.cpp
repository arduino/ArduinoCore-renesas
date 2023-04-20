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

FspTimer CLwipIf::timer;

ip_addr_t *u8_to_ip_addr(uint8_t *ipu8, ip_addr_t *ipaddr) {
  IP_ADDR4(ipaddr, ipu8[0], ipu8[1], ipu8[2], ipu8[3]);
  return ipaddr;
}


uint32_t ip_addr_to_u32(ip_addr_t *ipaddr) {
  return ip4_addr_get_u32(ipaddr);
}

/* -------------------------------------------------------------------------- */
CLwipIf::CLwipIf() : dns_num(-1) , willing_to_start_sync_req(false) , async_requests_ongoing(true) {
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
   if(CLwipIf::wifi_hw_initialized)
      CEspControl::getInstance().communicateWithEsp();


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

   if(willing_to_start_sync_req) {
      timer.disable_overflow_irq();
      willing_to_start_sync_req = false;
      async_requests_ongoing = false;
   }
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
CNetIf *CLwipIf::setUpEthernet(const uint8_t* _ip, 
                               const uint8_t* _gw, 
                               const uint8_t* _nm) {
/* -------------------------------------------------------------------------- */   
  /* get return nullptr if ni was already allocated */
  
  CNetIf *rv = nullptr;
  if(rv != nullptr) {
      

      
      
  }

  return rv;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::disconnectEventcb(CCtrlMsgWrapper *resp) {
   if(CLwipIf::connected_to_access_point) {
      wifi_status = WL_DISCONNECTED;
      if(net_ifs[NI_WIFI_STATION] != nullptr) {
         net_ifs[NI_WIFI_STATION]->setLinkDown();
      }
   }
}


/* -------------------------------------------------------------------------- */
int CLwipIf::initEventCb(CCtrlMsgWrapper *resp) {
   Serial.println("wifi_hw_initialized!");
   CLwipIf::wifi_hw_initialized = true;
}


/* -------------------------------------------------------------------------- */
int CLwipIf::setWifiMode(WifiMode_t mode) {
/* -------------------------------------------------------------------------- */   
      CLwipIf::getInstance().startSyncRequest();
      int rv = CEspControl::getInstance().setWifiMode(WIFI_MODE_STA);
      CLwipIf::getInstance().restartAsyncRequest();
      return rv;
}


/* -------------------------------------------------------------------------- */
bool CLwipIf::initWifiHw(bool asStation) {
/* -------------------------------------------------------------------------- */   
   bool rv = true;
   
   if(!CLwipIf::wifi_hw_initialized) {
      
      CEspControl::getInstance().listenForStationDisconnectEvent(CLwipIf::disconnectEventcb);
      CEspControl::getInstance().listenForInitEvent(CLwipIf::initEventCb);
      if(CEspControl::getInstance().initSpiDriver() == 0) {
         wifi_status = WL_NO_SSID_AVAIL;
      }

      if(wifi_status == WL_NO_SSID_AVAIL) {
         Serial.println("start wait");
         
         int time_num = 0;
         while(time_num < WIFI_INIT_TIMEOUT_MS && !CLwipIf::wifi_hw_initialized) {
            CEspControl::getInstance().communicateWithEsp();
            R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
            time_num++;
         }
         
         Serial.println("exit wait");
         if(asStation) {
            Serial.println("Call wifi mode set");
            
            int res = CLwipIf::getInstance().setWifiMode(WIFI_MODE_STA);

            if( res == ESP_CONTROL_OK) {
               Serial.println("Scan for access points");
               CLwipIf::getInstance().scanForAp();
            }
         }
         else {
            CEspControl::getInstance().setWifiMode(WIFI_MODE_AP);
         }
      }
   }

   if(wifi_status != WL_SCAN_COMPLETED) {
      rv = false;
   }
   
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
   static int id = 0;
   CNetIf *rv = nullptr;
   bool isStation = true;
   bool isEth = false;

   if(type >= 0 && type < NETWORK_INTERFACES_MAX_NUM) {
      if(net_ifs[type] == nullptr) {
         switch(type) {
            case NI_WIFI_STATION:
               Serial.println("CLwipIf::get (FIRST TIME) NI_WIFI_STATION");
               net_ifs[type] = new CWifiStation();
               isStation = true;
            break;
            
            case NI_WIFI_SOFTAP:
               Serial.println("CLwipIf::get (FIRST TIME) NI_WIFI_SOFTAP");
               net_ifs[type] = new CWifiSoftAp();
               isStation = false;
            break;

            case NI_ETHERNET:
               Serial.println("CLwipIf::get (FIRST TIME) NI_ETHERNET");
               net_ifs[type] = new CEth();
               isEth = true;
            break;
            default:
            break;
         }
         
         if(net_ifs[type] != nullptr) {
            Serial.println("CLwipIf::get (FIRST TIME) INITIALIZATION");
            if(!isEth) {
               CLwipIf::initWifiHw(isStation);
               net_ifs[type]->begin(_ip,_gw,_nm);
               net_ifs[type]->setId(0);
            }
            else {

            }

         }
      }
      Serial.println("CLwipIf::get");
      rv = net_ifs[type];
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

         #ifdef DEBUG_OUTPUT_DISABLED
         Serial.println("Bytes LWIP wants to send: ");
      
         for(int i = 0; i < bytes_actually_copied; i++) {
            Serial.print(buf[i], HEX);
            Serial.print(" ");
         }
         Serial.println();
         #endif


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
      /* ETHERNET */

   }
   return true;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::getMacAddress(NetIfType_t type, uint8_t* mac) {
/* -------------------------------------------------------------------------- */   
   int rv = 0;
   WifiMac_t MAC;
   
   CLwipIf::getInstance().startSyncRequest();
   
   if(type == NI_WIFI_STATION) {
      MAC.mode = WIFI_MODE_STA;
      if(CEspControl::getInstance().getWifiMacAddress(MAC) == ESP_CONTROL_OK) {
         //Serial.print("++++++++++++++++++ SETTING MAC ADDRESS TO:  ");
         //Serial.println(MAC.mac);

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

   CLwipIf::getInstance().startSyncRequest();
   return rv;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::scanForAp() {
/* -------------------------------------------------------------------------- */   
   access_points.clear();
   CLwipIf::getInstance().startSyncRequest();
   int res = CEspControl::getInstance().getAccessPointScanList(access_points);
   CLwipIf::getInstance().restartAsyncRequest();
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
   int rv =  ESP_CONTROL_CTRL_ERROR;
   bool found = false;
   uint8_t index = 0;
   for(uint8_t i = 0; i < access_points.size(); i++) {
      if(strcmp(ssid, (const char *)access_points[i].ssid) == 0) {
         found = true;
         index = i;
      }
   }

   if(found) {
      Serial.println("SSID found!");

      memset(ap.ssid,0x00,SSID_LENGTH);
      memcpy(ap.ssid,access_points[index].ssid,SSID_LENGTH);
      memset(ap.pwd,0x00,PASSWORD_LENGTH);
      if(pwd != nullptr) {
         memcpy(ap.pwd,pwd, (strlen(pwd) < PASSWORD_LENGTH) ? strlen(pwd) : PASSWORD_LENGTH) ;
      }
      memset(ap.bssid,0x00,BSSID_LENGTH);
      memcpy(ap.bssid,access_points[index].bssid,BSSID_LENGTH);

      CLwipIf::getInstance().startSyncRequest();
      if(CEspControl::getInstance().connectAccessPoint(ap) == ESP_CONTROL_OK) {
         CLwipIf::connected_to_access_point = true;
         Serial.println("############### CONNECTED TO ACCESS POINT");
         
         
         wifi_status = WL_CONNECTED;
         CEspControl::getInstance().getAccessPointConfig(access_point_cfg);
         rv = ESP_CONTROL_OK;

         /* when we get the connection to access point we are sure we are STATION 
            and we are connected */
         if(net_ifs[NI_WIFI_STATION] != nullptr) {
            net_ifs[NI_WIFI_STATION]->setLinkUp();
         }

      }
      else {
         wifi_status = WL_CONNECT_FAILED;
         CLwipIf::connected_to_access_point = false;
      }

      CLwipIf::getInstance().restartAsyncRequest();
   }
   else {
      Serial.println("SSID not found in the list of available AP");
   }
   return rv;
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
   CLwipIf::getInstance().startSyncRequest();
   int rv = CEspControl::getInstance().disconnectAccessPoint();
   CLwipIf::getInstance().restartAsyncRequest();
   wifi_status = WL_DISCONNECTED;
   if(net_ifs[NI_WIFI_STATION] != nullptr) {
      net_ifs[NI_WIFI_STATION]->setLinkDown();
   }
   return rv;
}


void toggle_led_debug() {
   static int i = 0;
   static bool st = false;
   i++;
   if(i > 5) {
      if(st) {
         digitalWrite(LED_BUILTIN, HIGH);
      }
      else {
         digitalWrite(LED_BUILTIN, LOW);
      }
      st = !st;
      i = 0;
   }
}


#ifdef LWIP_USE_TIMER
/* -------------------------------------------------------------------------- */
void CLwipIf::timer_cb(timer_callback_args_t *arg) {
/*  -------------------------------------------------------------------------- */   
  
  toggle_led_debug();
  CLwipIf::getInstance().lwip_task();
} 
#endif

/* ***************************************************************************
 *                               DNS related functions
 * ****************************************************************************/

/* -------------------------------------------------------------------------- */
void CLwipIf::dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
/* -------------------------------------------------------------------------- */   
  if (ipaddr != NULL) {
    *((uint32_t *)callback_arg) = ip4_addr_get_u32(ipaddr);
  } else {
    *((uint32_t *)callback_arg) = 0;
  }
}

/* -------------------------------------------------------------------------- */
int8_t CLwipIf::get_ip_address_from_hostname(const char *hostname, uint32_t *ipaddr) {
/* -------------------------------------------------------------------------- */   
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
        lwip_task(); 
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

/* -------------------------------------------------------------------------- */
int CLwipIf::getHostByName(const char *aHostname, IPAddress &aResult) {
/* -------------------------------------------------------------------------- */   
   int ret = 0;
   uint32_t ipResult = 0;

   // See if it's a numeric IP address
   if (inet2aton(aHostname, aResult)) {
     // It is, our work here is done
     return 1;
   }

   if(getDns(0) == IPAddress(0,0,0,0)) {
      Serial.println("INVALID DNS SERVER !!!!!!!");
     return INVALID_SERVER;
   }

   
   #if LWIP_DNS
   ret = get_ip_address_from_hostname(aHostname, &ipResult);
   aResult = IPAddress(ipResult);
   #endif
   return ret;
}


/* -------------------------------------------------------------------------- */
int CLwipIf::inet2aton(const char *address, IPAddress &result) {
/* -------------------------------------------------------------------------- */   
  uint16_t acc = 0; // Accumulator
  uint8_t dots = 0;

  if (address == NULL) {
    return 0;
  }

  while (*address) {
    char c = *address++;
    if (c >= '0' && c <= '9') {
      acc = acc * 10 + (c - '0');
      if (acc > 255) {
        // Value out of [0..255] range
        return 0;
      }
    } else if (c == '.') {
      if (dots == 3) {
        // Too much dots (there must be 3 dots)
        return 0;
      }
      result[dots++] = acc;
      acc = 0;
    } else {
      // Invalid char
      return 0;
    }
  }

  if (dots != 3) {
    // Too few dots (there must be 3 dots)
    return 0;
  }
  result[3] = acc;
  return 1;
}



/* -------------------------------------------------------------------------- */
void CLwipIf::beginDns(IPAddress aDNSServer) {
/* -------------------------------------------------------------------------- */   
   addDns(aDNSServer);
}

/* -------------------------------------------------------------------------- */
void CLwipIf::addDns(IPAddress aDNSServer) {
/* -------------------------------------------------------------------------- */   
   #if LWIP_DNS
   ip_addr_t ip;
   dns_num++;
   uint8_t *dnsaddr = aDNSServer.raw_address();
   /* DNS initialized by DHCP when call dhcp_start() */
   bool dhcp_started = false;

   for(int i = 0; i < NETWORK_INTERFACES_MAX_NUM; i++) {
      if(net_ifs[i] != nullptr) {
         if(net_ifs[i]->DhcpIsStarted()) {
            dhcp_started = true;
            break;
         }
      }
   }

   if (!dhcp_started) {
     dns_init();
     IP_ADDR4(&ip, dnsaddr[0], dnsaddr[1], dnsaddr[2], dnsaddr[3]);
     dns_setserver(dns_num, &ip);
     
   }
   #endif
}

//bool CLwipIf::isDnsAvailable() {

//}


/* -------------------------------------------------------------------------- */
IPAddress CLwipIf::getDns(int _num) {
/* -------------------------------------------------------------------------- */
   #if LWIP_DNS
   const ip_addr_t *tmp = dns_getserver(_num);
   return IPAddress(ip4_addr_get_u32(tmp));
   #else
   IPAddress(0,0,0,0);
   #endif   

}

/* ########################################################################## */
/*                      BASE NETWORK INTERFACE CLASS                          */
/* ########################################################################## */

/* -------------------------------------------------------------------------- */
CNetIf::CNetIf() :  dhcp_started(false), dhcp_acquired(false), id(0), dhcp_st(DHCP_IDLE_STATUS), _dhcp_lease_state(DHCP_CHECK_NONE) {
/* -------------------------------------------------------------------------- */   
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


/* ***************************************************************************
 *                               DHCP related functions
 * ****************************************************************************/

/* -------------------------------------------------------------------------- */
void CNetIf::DhcpNotUsed() {
/* -------------------------------------------------------------------------- */   
   DhcpStop();
   dhcp_inform(getNi());
}

/* -------------------------------------------------------------------------- */
int CNetIf::checkLease() {
/* -------------------------------------------------------------------------- */   
   int rc = DHCP_CHECK_NONE;

   task();
   rc = dhcp_get_lease_state();

   if (rc != _dhcp_lease_state) {
     switch (_dhcp_lease_state) {
       case DHCP_CHECK_NONE:
         _dhcp_lease_state = rc;
         rc = DHCP_CHECK_NONE;
         break;

      case DHCP_CHECK_RENEW_OK:
         _dhcp_lease_state = rc;
         if (rc == DHCP_CHECK_NONE) {
           rc = DHCP_CHECK_RENEW_OK;
         } else {
           rc = DHCP_CHECK_RENEW_FAIL;
         }
         break;

      case DHCP_CHECK_REBIND_OK:
         _dhcp_lease_state = rc;
         if (rc == DHCP_CHECK_NONE) {
           rc = DHCP_CHECK_REBIND_OK;
         } else {
           rc = DHCP_CHECK_REBIND_FAIL;
         }
         break;

      default:
         _dhcp_lease_state = DHCP_CHECK_NONE;
         break;
      }
   }

  return rc;
}


/* -------------------------------------------------------------------------- */
uint8_t CNetIf::dhcp_get_lease_state() {
/* -------------------------------------------------------------------------- */   
   uint8_t res = 0;
   struct dhcp *dhcp = (struct dhcp *)netif_get_client_data(getNi(), LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

   if (dhcp->state == DHCP_STATE_RENEWING) {
      res = 2;
   } else if (dhcp->state == DHCP_STATE_REBINDING) {
      res = 4;
   }
   return res;
}

/* -------------------------------------------------------------------------- */
bool CNetIf::dhcp_request() {
/* -------------------------------------------------------------------------- */
   /* make a DHCP request: it runs till an address is acquired or a timeout 
      expires */
   unsigned long startTime = millis();
   bool acquired = false;

   do{
      task();
      acquired = isDhcpAcquired();
      if(!acquired && ((millis() - startTime) > dhcp_timeout)) {
         break;
      }

   } while(!acquired);

   return acquired;
}

/* -------------------------------------------------------------------------- */
void CNetIf::dhcp_reset() {
/* -------------------------------------------------------------------------- */
   /* it resets the DHCP status to IDLE */
   while(dhcp_st != DHCP_IDLE_STATUS) {
      task();
   }
}

/* -------------------------------------------------------------------------- */
void CNetIf::DhcpSetTimeout(unsigned long t) {
/* -------------------------------------------------------------------------- */   
   dhcp_timeout = t;
}

/* -------------------------------------------------------------------------- */
bool CNetIf::isDhcpAcquired() { 
   return dhcp_acquired; 
}
/* -------------------------------------------------------------------------- */
bool CNetIf::DhcpStart() {
   /* first stop / reset */
   DhcpStop();
   /* then actually start */
   dhcp_started = true;
   dhcp_st = DHCP_START_STATUS; 
   return dhcp_request();

}
/* -------------------------------------------------------------------------- */
void CNetIf::DhcpStop()  {
/* -------------------------------------------------------------------------- */   
   dhcp_started = false;
   if(dhcp_st == DHCP_IDLE_STATUS) {
      return;
   }
   if(dhcp_st == DHCP_GOT_STATUS && netif_is_link_up(getNi())) {
      dhcp_st = DHCP_RELEASE_STATUS; 
   }
   else {
      dhcp_st = DHCP_STOP_STATUS;
   }
   dhcp_reset();
   
}
/* -------------------------------------------------------------------------- */
void CNetIf::dhcp_task() {
/* -------------------------------------------------------------------------- */
   
   struct dhcp *lwip_dhcp;
   
   switch(dhcp_st){
      case DHCP_IDLE_STATUS:
         /* nothing to do... wait for DhcpStart() to start the process */
      break;
      case DHCP_START_STATUS:
         if(netif_is_link_up(getNi())) {
            Serial.println("----------------------------------- DHCP start");
            ip_addr_set_zero_ip4(&(getNi()->ip_addr));
            ip_addr_set_zero_ip4(&(getNi()->netmask));
            ip_addr_set_zero_ip4(&(getNi()->gw));
            /* start lwIP dhcp */
            dhcp_start(getNi());
            dhcp_st = DHCP_WAIT_STATUS;
         }
      break;
      case DHCP_WAIT_STATUS:
         if(netif_is_link_up(getNi())) {
            if (dhcp_supplied_address(getNi())) {
               Serial.println("----------------------------------- DHCP got!");
               dhcp_st = DHCP_GOT_STATUS;
            } 
            else {
               /* TIMEOUT */
               lwip_dhcp = (struct dhcp *)netif_get_client_data(getNi(), LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
               if (lwip_dhcp->tries > MAX_DHCP_TRIES) {
                 dhcp_st = DHCP_STOP_STATUS;
               }
            }
         }
         else {
            dhcp_st = DHCP_START_STATUS;
         }
      break;
      case DHCP_GOT_STATUS:
         if (!netif_is_link_up(getNi())) {
            
            dhcp_st = DHCP_STOP_STATUS;
         } 

      break;
      case DHCP_RELEASE_STATUS:
         dhcp_release(getNi());
         dhcp_st = DHCP_STOP_STATUS;
      break;
      case DHCP_STOP_STATUS:

         dhcp_stop(getNi());
         if(dhcp_started) {
            dhcp_st = DHCP_START_STATUS;
         }
         else {
            dhcp_st = DHCP_IDLE_STATUS;
         }
      break;

   }
}

/* -------------------------------------------------------------------------- */
void CNetIf::setLinkUp() {
/* -------------------------------------------------------------------------- */   
   netif_set_link_up(&ni);
   /* When the netif is fully configured this function must be called.*/
   netif_set_up(&ni);
}

/* -------------------------------------------------------------------------- */
void CNetIf::setLinkDown() {
/* -------------------------------------------------------------------------- */   
   netif_set_link_down(&ni);
   /* When the netif is fully configured this function must be called.*/
   netif_set_down(&ni);

}


/* ########################################################################## */
/*                      ETHERNET NETWORK INTERFACE CLASS                      */
/* ########################################################################## */
CEth::CEth() {}
CEth::~CEth() {}



void CEth::setIp(const uint8_t *_ip) {
   setAddr(&ip,_ip, (const uint8_t*)&default_eth_ip);
}

void CEth::setNm(const uint8_t *_nm) {
   setAddr(&ip,_nm, (const uint8_t*)&default_eth_ip);
}

void CEth::setGw(const uint8_t *_gw) {
   setAddr(&ip,_gw, (const uint8_t*)&default_eth_ip);
}

/* -------------------------------------------------------------------------- */
void CEth::begin(const uint8_t* _ip, 
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
void CEth::task() {
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

   if(buf != nullptr) {
      //Serial.println("Wifi Station - msg rx");

      struct pbuf* p = pbuf_alloc(PBUF_RAW, dim, PBUF_RAM);
      if(p != NULL) {
         /* Copy ethernet frame into pbuf */
         pbuf_take((struct pbuf* )p, (uint8_t *) buf, (uint32_t)dim);
         delete []buf;

         if(ni.input(p, &ni) != ERR_OK) {
            pbuf_free(p);
         }
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
  
   if(buf != nullptr) {

   struct pbuf* p = pbuf_alloc(PBUF_RAW, dim, PBUF_RAM);


      if(p != NULL) {
         /* Copy ethernet frame into pbuf */
         pbuf_take((struct pbuf* )p, (uint8_t *) buf, (uint32_t)dim);
         delete []buf;

         if(ni.input(p, &ni) != ERR_OK) {
            pbuf_free(p);
         }
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
