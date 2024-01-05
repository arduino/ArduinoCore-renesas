#include "WiFiC3.h"


extern "C" void dhcps_start(struct netif *netif);



/* -------------------------------------------------------------------------- */
CWifi::CWifi() : _timeout(50000), ni(nullptr) {
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
const char* CWifi::firmwareVersion() {
/* -------------------------------------------------------------------------- */   
   /* silly "dummy" implementation to keep compatibility, at the present
      the WiFi fw does not return any version number */
   return WIFI_FIRMWARE_LATEST_VERSION;
}



/* -------------------------------------------------------------------------- */
int CWifi::begin(const char* ssid) {
/* -------------------------------------------------------------------------- */   
   ni = CLwipIf::getInstance().get(NI_WIFI_STATION);
   CLwipIf::getInstance().connectToAp(ssid, nullptr);
   if(ni != nullptr && !_useStaticIp) {
      ni->DhcpStart();
   }
   
   return CLwipIf::getInstance().getWifiStatus();
}


/* -------------------------------------------------------------------------- */
int CWifi::begin(const char* ssid, const char *passphrase) {
/* -------------------------------------------------------------------------- */   
   
   ni = CLwipIf::getInstance().get(NI_WIFI_STATION);
   CLwipIf::getInstance().connectToAp(ssid, passphrase); 
   if(ni != nullptr && !_useStaticIp) {
      ni->DhcpStart();
   }
   
   return CLwipIf::getInstance().getWifiStatus();
}

/* passphrase is needed so a default one will be set */
/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid) {
/* -------------------------------------------------------------------------- */   
   return beginAP(ssid,1);
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid, uint8_t channel) {
/* -------------------------------------------------------------------------- */   
   return beginAP(ssid,nullptr,channel);
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid, const char* passphrase) {
/* -------------------------------------------------------------------------- */   
   return beginAP(ssid,passphrase,1);
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid, const char* passphrase, uint8_t channel) {
/* -------------------------------------------------------------------------- */   
   
   ni = CLwipIf::getInstance().get(NI_WIFI_SOFTAP);
   CLwipIf::getInstance().startSoftAp(ssid,passphrase,channel); 
   if(ni != nullptr) {
      
      dhcps_start(ni->getNi());
   }
   
   return CLwipIf::getInstance().getWifiStatus();   
}



/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip) {
/* -------------------------------------------------------------------------- */   
   IPAddress _nm(255, 255, 255, 0);
   IPAddress _gw = local_ip;
   _gw[3] = 1;

   _config(local_ip, _gw, _nm);
}

extern uint8_t *IpAddress2uint8(IPAddress a);

/* -------------------------------------------------------------------------- */
void CWifi::_config(IPAddress local_ip, IPAddress gateway, IPAddress subnet) {
/* -------------------------------------------------------------------------- */    
   _useStaticIp = local_ip != INADDR_NONE;
   if(ni != nullptr) {
      ni->DhcpStop();
      ni->DhcpNotUsed();
      IP_ADDR4(&ni->ip, local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
      IP_ADDR4(&ni->gw, gateway[0], gateway[1], gateway[2], gateway[3]);
      IP_ADDR4(&ni->nm, subnet[0], subnet[1], subnet[2], subnet[3]);
   }
   else {
      CNetIf::default_ip = local_ip;
      CNetIf::default_nm = subnet; 
      CNetIf::default_gw = gateway;
      CNetIf::default_dhcp_server_ip = local_ip;
   }
}

/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip, IPAddress dns_server) {
/* -------------------------------------------------------------------------- */   
   config(local_ip);
   CLwipIf::getInstance().addDns(dns_server);
}

/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway) {
/* -------------------------------------------------------------------------- */   
   IPAddress _nm(255, 255, 255, 0);
   _config(local_ip, gateway, _nm);
   CLwipIf::getInstance().addDns(dns_server);
}

/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet) {
/* -------------------------------------------------------------------------- */
   _config(local_ip, gateway, subnet);
   CLwipIf::getInstance().addDns(dns_server);
}

/* -------------------------------------------------------------------------- */
void CWifi::setDNS(IPAddress dns_server1) {
/* -------------------------------------------------------------------------- */   
   CLwipIf::getInstance().addDns(dns_server1);
}

/* -------------------------------------------------------------------------- */
void CWifi::setDNS(IPAddress dns_server1, IPAddress dns_server2) {
/* -------------------------------------------------------------------------- */   
   CLwipIf::getInstance().addDns(dns_server1);
   CLwipIf::getInstance().addDns(dns_server2);
   
}

/* -------------------------------------------------------------------------- */
void CWifi::setHostname(const char* name) {
/* -------------------------------------------------------------------------- */   
   if(ni != nullptr) {
      ni->setHostname(name);
   }
}

/* -------------------------------------------------------------------------- */
int CWifi::disconnect() {
/* -------------------------------------------------------------------------- */   
   CLwipIf::getInstance().disconnectFromAp();
}

/* -------------------------------------------------------------------------- */
void CWifi::end(void) {
/* -------------------------------------------------------------------------- */   

}

/* -------------------------------------------------------------------------- */
uint8_t* CWifi::macAddress(uint8_t* mac) {
/* -------------------------------------------------------------------------- */   
   if(ni != nullptr) {
      if(ni->getMacAddress(mac) == WL_MAC_ADDR_LENGTH) {
         return mac;
      }
   }
   memset(mac,0x00,6);
   return mac;
}

/* -------------------------------------------------------------------------- */
int8_t CWifi::scanNetworks() {
/* -------------------------------------------------------------------------- */   
   ni = CLwipIf::getInstance().get(NI_WIFI_STATION);   
   if(CLwipIf::getInstance().scanForAp() == ESP_CONTROL_OK) {
      return CLwipIf::getInstance().getApNum();
   }
   return 0;
}
 
/* -------------------------------------------------------------------------- */   
IPAddress CWifi::localIP() {
/* -------------------------------------------------------------------------- */   
   if(ni != nullptr) {
      return IPAddress(ni->getIpAdd());   
   }
   return IPAddress((uint32_t)0);
}

/* -------------------------------------------------------------------------- */
IPAddress CWifi::subnetMask() {
/* -------------------------------------------------------------------------- */
   if(ni != nullptr) {
      return IPAddress(ni->getNmAdd());   
   }
   return IPAddress((uint32_t)0);
}

/* -------------------------------------------------------------------------- */
IPAddress CWifi::gatewayIP() {
/* -------------------------------------------------------------------------- */   
   if(ni != nullptr) {
      return IPAddress(ni->getGwAdd());   
   }
   return IPAddress((uint32_t)0);
}

/* -------------------------------------------------------------------------- */
IPAddress CWifi::dnsIP(int n) {
   return CLwipIf::getInstance().getDns(n);
}

/* -------------------------------------------------------------------------- */
const char* CWifi::SSID(uint8_t networkItem) {  
   return CLwipIf::getInstance().getSSID(networkItem);
}
/* -------------------------------------------------------------------------- */ 

/* -------------------------------------------------------------------------- */
int32_t CWifi::RSSI(uint8_t networkItem) {
   return CLwipIf::getInstance().getRSSI(networkItem);
}
/* -------------------------------------------------------------------------- */ 

/* -------------------------------------------------------------------------- */
uint8_t CWifi::encryptionType(uint8_t networkItem) {
   return CLwipIf::getInstance().getEncrType(networkItem);
}
/* -------------------------------------------------------------------------- */ 

/* -------------------------------------------------------------------------- */
uint8_t* CWifi::BSSID(uint8_t networkItem, uint8_t* bssid) {
   return CLwipIf::getInstance().getBSSID(networkItem,bssid);
}
/* -------------------------------------------------------------------------- */ 

/* -------------------------------------------------------------------------- */
uint8_t CWifi::channel(uint8_t networkItem) { 
   return CLwipIf::getInstance().getChannel(networkItem);
}
/* -------------------------------------------------------------------------- */ 

/* -------------------------------------------------------------------------- */ 
const char* CWifi::SSID() {
/* -------------------------------------------------------------------------- */    
   if(ni != nullptr) {
      return ni->getSSID();
   }
   return ""; 
}

/* -------------------------------------------------------------------------- */ 
uint8_t* CWifi::BSSID(uint8_t* bssid) {
/* -------------------------------------------------------------------------- */    
   if(ni != nullptr) {
      return ni->getBSSID(bssid);
   }
   return nullptr;
}

/* -------------------------------------------------------------------------- */ 
int32_t CWifi::RSSI() {
/* -------------------------------------------------------------------------- */    
   if(ni != nullptr) {
      return ni->getRSSI();
   }
   return 0;
}

/* -------------------------------------------------------------------------- */ 
uint8_t CWifi::encryptionType() {
/* -------------------------------------------------------------------------- */    
   if(ni != nullptr) {
      return ni->getEncryptionType();
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::status() {
/* -------------------------------------------------------------------------- */   
   return CLwipIf::getInstance().getWifiStatus(); 
}

/* -------------------------------------------------------------------------- */
int CWifi::hostByName(const char* aHostname, IPAddress& aResult) {
/* -------------------------------------------------------------------------- */   
   return CLwipIf::getInstance().getHostByName(aHostname,aResult);
}

/* -------------------------------------------------------------------------- */
void CWifi::lowPowerMode() {
/* -------------------------------------------------------------------------- */   
   CLwipIf::getInstance().setLowPowerMode();
}

/* -------------------------------------------------------------------------- */
void CWifi::noLowPowerMode() {
/* -------------------------------------------------------------------------- */   
   CLwipIf::getInstance().resetLowPowerMode();
}

uint8_t CWifi::reasonCode() {
   return 0;
}

unsigned long CWifi::getTime() {
   return 0;
}



void CWifi::setTimeout(unsigned long timeout) {
   (void)(timeout);  
}


CWifi WiFi;

