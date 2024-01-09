#include "WiFiC3.h"


extern "C" void dhcps_start(struct netif *netif);



/* -------------------------------------------------------------------------- */
CWifi::CWifi() : _timeout(50000) {
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
    WiFiStation.begin();
    return WiFiStation.connectToAP(ssid, nullptr);
}


/* -------------------------------------------------------------------------- */
int CWifi::begin(const char* ssid, const char *passphrase) {
/* -------------------------------------------------------------------------- */
    WiFiStation.begin();
    return WiFiStation.connectToAP(ssid, passphrase);
}

/* passphrase is needed so a default one will be set */
/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid) {
/* -------------------------------------------------------------------------- */
    WiFiSoftAP.begin();
    return WiFiSoftAP.startSoftAp(ssid); // FIXME put default password here
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid, uint8_t channel) {
/* -------------------------------------------------------------------------- */
    WiFiSoftAP.begin();
    return WiFiSoftAP.startSoftAp(ssid, nullptr, channel);
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid, const char* passphrase) {
/* -------------------------------------------------------------------------- */
    WiFiSoftAP.begin();
    return WiFiSoftAP.startSoftAp(ssid, passphrase);
}

/* -------------------------------------------------------------------------- */
uint8_t CWifi::beginAP(const char *ssid, const char* passphrase, uint8_t channel) {
/* -------------------------------------------------------------------------- */
    WiFiSoftAP.begin();
    return WiFiSoftAP.startSoftAp(ssid, passphrase, channel);
}



/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip) { // FIXME
/* -------------------------------------------------------------------------- */
    // IPAddress _nm(255, 255, 255, 0);
    // IPAddress _gw = local_ip;
    // _gw[3] = 1;

    // _config(local_ip, _gw, _nm);
}

extern uint8_t *IpAddress2uint8(IPAddress a);

/* -------------------------------------------------------------------------- */
void CWifi::_config(IPAddress local_ip, IPAddress gateway, IPAddress subnet) { // FIXME
/* -------------------------------------------------------------------------- */
    // _useStaticIp = local_ip != INADDR_NONE;
    // if(ni != nullptr) {
    //     ni->DhcpStop();
    //     ni->DhcpNotUsed();
    //     IP_ADDR4(&ni->ip, local_ip[0], local_ip[1], local_ip[2], local_ip[3]);
    //     IP_ADDR4(&ni->gw, gateway[0], gateway[1], gateway[2], gateway[3]);
    //     IP_ADDR4(&ni->nm, subnet[0], subnet[1], subnet[2], subnet[3]);
    // }
    // else {
    //     CNetIf::default_ip = local_ip;
    //     CNetIf::default_nm = subnet;
    //     CNetIf::default_gw = gateway;
    //     CNetIf::default_dhcp_server_ip = local_ip;
    // }
}

/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip, IPAddress dns_server) { // FIXME
/* -------------------------------------------------------------------------- */
    config(local_ip);
    // CLwipIf::getInstance().addDns(dns_server);
}

/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway) {
/* -------------------------------------------------------------------------- */
    IPAddress _nm(255, 255, 255, 0);
    _config(local_ip, gateway, _nm);
    // CLwipIf::getInstance().addDns(dns_server);
}

/* -------------------------------------------------------------------------- */
void CWifi::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet) {
/* -------------------------------------------------------------------------- */
    _config(local_ip, gateway, subnet);
    // CLwipIf::getInstance().addDns(dns_server);
}

/* -------------------------------------------------------------------------- */
void CWifi::setDNS(IPAddress dns_server1) {
/* -------------------------------------------------------------------------- */
    // CLwipIf::getInstance().addDns(dns_server1);
}

/* -------------------------------------------------------------------------- */
void CWifi::setDNS(IPAddress dns_server1, IPAddress dns_server2) {
/* -------------------------------------------------------------------------- */
    // CLwipIf::getInstance().addDns(dns_server1);
    // CLwipIf::getInstance().addDns(dns_server2);
}

/* -------------------------------------------------------------------------- */
void CWifi::setHostname(const char* name) {
/* -------------------------------------------------------------------------- */
//     if(ni != nullptr) {
//         ni->setHostname(name);
//     }
}

/* -------------------------------------------------------------------------- */
int CWifi::disconnect() {
/* -------------------------------------------------------------------------- */
    WiFiStation.disconnectFromAp();
}

/* -------------------------------------------------------------------------- */
void CWifi::end(void) {
/* -------------------------------------------------------------------------- */

}

/* -------------------------------------------------------------------------- */
uint8_t* CWifi::macAddress(uint8_t* mac) {
/* -------------------------------------------------------------------------- */
    if(WiFiStation.getMacAddress(mac) == WL_MAC_ADDR_LENGTH) {
        return mac;
    }
    memset(mac,0x00,6);
    return mac;
}

/* -------------------------------------------------------------------------- */
int8_t CWifi::scanNetworks() {
/* -------------------------------------------------------------------------- */
    return WiFiStation.scanForAp();;
}

/* -------------------------------------------------------------------------- */
IPAddress CWifi::localIP() {
/* -------------------------------------------------------------------------- */
    return WiFiStation.localIP();
}

/* -------------------------------------------------------------------------- */
IPAddress CWifi::subnetMask() {
/* -------------------------------------------------------------------------- */
    return WiFiStation.subnetMask();
}

/* -------------------------------------------------------------------------- */
IPAddress CWifi::gatewayIP() {
/* -------------------------------------------------------------------------- */
    return WiFiStation.gatewayIP();
}

/* -------------------------------------------------------------------------- */
IPAddress CWifi::dnsIP(int n) {
    return CLwipIf::getInstance().getDns(n);
}

/* -------------------------------------------------------------------------- */
const char* CWifi::SSID() {
    return WiFiStation.getSSID();
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
int32_t CWifi::RSSI() {
    return WiFiStation.getRSSI();
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
uint8_t CWifi::encryptionType() {
    return WiFiStation.getEncryptionType();
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
uint8_t* CWifi::BSSID(uint8_t* bssid) {
    return WiFiStation.getBSSID(bssid);
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
uint8_t CWifi::channel() {
    return WiFiStation.getChannel();
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
uint8_t CWifi::status() { // FIXME
/* -------------------------------------------------------------------------- */
    // return CLwipIf::getInstance().getWifiStatus();
}

/* -------------------------------------------------------------------------- */
int CWifi::hostByName(const char* aHostname, IPAddress& aResult) {
/* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getHostByName(aHostname, aResult);
}

/* -------------------------------------------------------------------------- */
void CWifi::lowPowerMode() {
/* -------------------------------------------------------------------------- */
    WiFiStation.setLowPowerMode();
}

/* -------------------------------------------------------------------------- */
void CWifi::noLowPowerMode() {
/* -------------------------------------------------------------------------- */
    WiFiStation.resetLowPowerMode();
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
