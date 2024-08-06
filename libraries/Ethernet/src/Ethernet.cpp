#include <EthernetC33.h>
#include <EthernetClock.h>

/* -------------------------------------------------------------------------- */
void CEthernet::initializeTimer() {
/* -------------------------------------------------------------------------- */
	
    if (!ethernetTimer) {
        ethernetTimer = new (std::nothrow) EthernetClock();
	if (ethernetTimer) {
	    ethernetTimer->start();
	    delay(2);
        }
    }
}


/* -------------------------------------------------------------------------- */
int CEthernet::begin(unsigned long timeout, unsigned long responseTimeout) {
/* -------------------------------------------------------------------------- */  
    
    initializeTimer();
    (void)responseTimeout;

    ni = CLwipIf::getInstance().get(NI_ETHERNET);
    if (ni) {
        ni->DhcpSetTimeout(timeout);
        return static_cast<int>(ni->DhcpStart());
    }
    return 0;
}

/* -------------------------------------------------------------------------- */  
int CEthernet::configureStaticIP(const IPAddress& local_ip, const IPAddress& dns_server, const IPAddress& gateway, const IPAddress& subnet) {
/* -------------------------------------------------------------------------- */  
    initializeTimer();
    
    if (ni) {
        ni->config(local_ip, gateway, subnet);
    } else {
        ni = CLwipIf::getInstance().get(NI_ETHERNET, local_ip, gateway, subnet);
        if (!ni) return 0;
    }

    ni->DhcpNotUsed();
    CLwipIf::getInstance().addDns(dns_server);
    return 1;
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(const IPAddress& local_ip) {
/* -------------------------------------------------------------------------- */
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress dns_server = local_ip;
  dns_server[3] = 1;
  return begin(local_ip, dns_server);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(const IPAddress& local_ip, const IPAddress& dns_server) {
/* -------------------------------------------------------------------------- */  
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  return begin(local_ip, dns_server, gateway);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(const IPAddress& local_ip, const IPAddress& dns_server, const IPAddress& gateway) {
/* -------------------------------------------------------------------------- */  
  IPAddress subnet(255, 255, 255, 0);
  return begin(local_ip, dns_server, gateway, subnet);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(const IPAddress& local_ip, const IPAddress& dns_server, const IPAddress& gateway, const IPAddress& subnet) {
/* -------------------------------------------------------------------------- */  
  
  return configureStaticIP(local_ip, dns_server, gateway, subnet);
}

/* -------------------------------------------------------------------------- */
inline void CEthernet::setDNS(const IPAddress& dns_server) {
/* -------------------------------------------------------------------------- */  
  CLwipIf::getInstance().addDns(dns_server);
} 

/* -------------------------------------------------------------------------- */
int CEthernet::begin(const uint8_t* mac, unsigned long timeout, unsigned long responseTimeout) {
/* -------------------------------------------------------------------------- */  
  CLwipIf::getInstance().setMacAddress(NI_ETHERNET, mac);
  return begin(timeout, responseTimeout);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(const uint8_t* mac_address, const IPAddress& local_ip) {
/* -------------------------------------------------------------------------- */
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
   
  static const uint8_t GATEWAY_OFFSET = 1;
  IPAddress gateway(local_ip);
  gateway[3] = GATEWAY_OFFSET;
  return begin(mac_address, local_ip, dns_server, gateway);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server) {
/* -------------------------------------------------------------------------- */  
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  return begin(mac_address, local_ip, dns_server, gateway);
}

/* -------------------------------------------------------------------------- */
CEthernet::begin(const uint8_t* mac_address, const IPAddress& local_ip, const IPAddress& dns_server, const IPAddress& gateway) {
/* -------------------------------------------------------------------------- */  
  static const IPAddress DEFAULT_SUBNET(255, 255, 255, 0);
  return begin(mac_address, local_ip, dns_server, gateway, DEFAULT_SUBNET);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(const uint8_t* mac, const IPAddress& local_ip, const IPAddress& dns_server, const IPAddress& gateway, const IPAddress& subnet, unsigned long timeout, unsigned long responseTimeout) {
/* -------------------------------------------------------------------------- */  
    CLwipIf::getInstance().setMacAddress(NI_ETHERNET, mac);
    return begin(local_ip, dns_server, gateway, subnet);
}

/* -------------------------------------------------------------------------- */
EthernetLinkStatus CEthernet::linkStatus() const {
/* -------------------------------------------------------------------------- */  
    if (!ni) return Unknown;
    if (!CLwipIf::getInstance().isEthInitialized()) return Unknown;
    return ni->isLinkUp() ? LinkON : LinkOFF;
}

/* -------------------------------------------------------------------------- */
EthernetHardwareStatus CEthernet::hardwareStatus() const {
/* -------------------------------------------------------------------------- */
  return EthernetLwip;
}

/* -------------------------------------------------------------------------- */
int CEthernet::disconnect() {
/* -------------------------------------------------------------------------- */
    if (ethernetTimer) {
        ethernetTimer->stop();
        delete ethernetTimer;
        ethernetTimer = nullptr;
    }
    ni = nullptr;
    return 1;
}

/* -------------------------------------------------------------------------- */
int CEthernet::maintain() {
/* -------------------------------------------------------------------------- */  
  return ni ? ni->checkLease() : DHCP_CHECK_NONE;
}

/* -------------------------------------------------------------------------- */  
void CEthernet::schedule(void) {
/* -------------------------------------------------------------------------- */  
  if (ni) ni->task();
}

/* -------------------------------------------------------------------------- */
const uint8_t* CEthernet::MACAddress() const {
/* -------------------------------------------------------------------------- */	
    CLwipIf::getInstance().getMacAddress(NI_ETHERNET, const_cast<uint8_t*>(mac_address));
    return mac_address;
}

/* -------------------------------------------------------------------------- */
void CEthernet::MACAddress(uint8_t* mac) const {
/* -------------------------------------------------------------------------- */
    CLwipIf::getInstance().getMacAddress(NI_ETHERNET, mac);
}

/* -------------------------------------------------------------------------- */
IPAddress CEthernet::localIP() const {
/* -------------------------------------------------------------------------- */
    return ni ? IPAddress(ni->getIpAdd()) : IPAddress(static_cast<uint32_t>(0));
}

/* -------------------------------------------------------------------------- */
IPAddress CEthernet::subnetMask() const {
/* -------------------------------------------------------------------------- */
    return ni ? IPAddress(ni->getNmAdd()) : IPAddress(static_cast<uint32_t>(0));
}

/* -------------------------------------------------------------------------- */
IPAddress CEthernet::gatewayIP() const {
/* -------------------------------------------------------------------------- */
    return ni ? IPAddress(ni->getGwAdd()) : IPAddress(static_cast<uint32_t>(0));
}

/* -------------------------------------------------------------------------- */
IPAddress CEthernet::dnsServerIP() const {
/* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getDns();
}

CEthernet Ethernet;
