#include "CWifi.h"

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
   if(ni == nullptr) {
      ni = CLwipIf::getInstance().get(NI_WIFI_STATION);
      CLwipIf::getInstance().connectToAp(ssid, nullptr);
      ni->DhcpStart();
   }
   return CLwipIf::getInstance().getWifiStatus();
}


/* -------------------------------------------------------------------------- */
int CWifi::begin(const char* ssid, const char *passphrase) {
/* -------------------------------------------------------------------------- */   
   if(ni == nullptr) {
      ni = CLwipIf::getInstance().get(NI_WIFI_STATION);
      CLwipIf::getInstance().connectToAp(ssid, passphrase); 
      ni->DhcpStart();
   }
   return CLwipIf::getInstance().getWifiStatus();
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid) {
/* -------------------------------------------------------------------------- */   
   (void)(ssid);
   ni = CLwipIf::getInstance().get(NI_WIFI_SOFTAP);
   return 1;
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid, uint8_t channel) {
/* -------------------------------------------------------------------------- */   
   (void)(ssid);
   (void)(channel);
   ni = CLwipIf::getInstance().get(NI_WIFI_SOFTAP);
   return 1;
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid, const char* passphrase) {
/* -------------------------------------------------------------------------- */   
   (void)(ssid);
   (void)(passphrase);
   ni = CLwipIf::getInstance().get(NI_WIFI_SOFTAP);
   return 1;
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid, const char* passphrase, uint8_t channel) {
/* -------------------------------------------------------------------------- */   
   (void)(ssid);
   (void)(passphrase);
   (void)(channel);
   
   ni = CLwipIf::getInstance().get(NI_WIFI_SOFTAP);
   return 1;
}


/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip) {
/* -------------------------------------------------------------------------- */   
   (void)(local_ip);
}

/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip, IPAddress dns_server) {
/* -------------------------------------------------------------------------- */   
   (void)(local_ip);
   (void)(dns_server);
}

/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway) {
/* -------------------------------------------------------------------------- */   
   (void)(local_ip);
   (void)(dns_server);
   (void)(gateway);
}

/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet) {
/* -------------------------------------------------------------------------- */
   (void)(local_ip);
   (void)(dns_server);
   (void)(gateway);
   (void)(subnet);
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
      if(ni->getMacAddress(mac) == ESP_CONTROL_OK) {
         return mac;
      }
   }
   return nullptr;
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
   return CLwipIf::getInstance().getSSID(); 
}

/* -------------------------------------------------------------------------- */ 
uint8_t* CWifi::BSSID(uint8_t* bssid) {
/* -------------------------------------------------------------------------- */    
   return CLwipIf::getInstance().getBSSID(bssid);
}

/* -------------------------------------------------------------------------- */ 
int32_t CWifi::RSSI() {
/* -------------------------------------------------------------------------- */    
   return CLwipIf::getInstance().getRSSI();
}

/* -------------------------------------------------------------------------- */ 
uint8_t CWifi::encryptionType() {
/* -------------------------------------------------------------------------- */    
   return CLwipIf::getInstance().getEncrType();
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::status() {
/* -------------------------------------------------------------------------- */   
   return CLwipIf::getInstance().getWifiStatus(); 
}




void CWifi::setDNS(IPAddress dns_server1)
{
   
}

void CWifi::setDNS(IPAddress dns_server1, IPAddress dns_server2)
{
   
}







uint8_t CWifi::reasonCode()
{
   
}

int CWifi::hostByName(const char* aHostname, IPAddress& aResult)
{
   
}

unsigned long CWifi::getTime()
{
   
}

void CWifi::lowPowerMode()
{
   
}

void CWifi::noLowPowerMode()
{
   
}

int CWifi::ping(const char* hostname, uint8_t ttl)
{
  
}

int CWifi::ping(const String &hostname, uint8_t ttl)
{
   
}

int CWifi::ping(IPAddress host, uint8_t ttl)
{
   
}

void CWifi::setTimeout(unsigned long timeout)
{
  
}



CWifi WiFi;

