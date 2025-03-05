#include <EthernetC33.h>
#include <EthernetClock.h>
/*
 * The old implementation of the begin set a default mac address:
 * this does not make any sense.
 * Default mac address is in the hardware, when lwip start that mac
 * address is passed to lwip
 * If mac address needs to be changed then call the appropriate function
 * of lwIpIf before to get the interface 
 */

/* -------------------------------------------------------------------------- */
int CEthernet::begin(unsigned long timeout, unsigned long responseTimeout) {
/* -------------------------------------------------------------------------- */  
    
  ethernetTimer = new EthernetClock();
  ethernetTimer->start();
  delay(2);
  (void)responseTimeout;

  int rv = 0;


  ni = CLwipIf::getInstance().get(NI_ETHERNET);
  if(ni != nullptr) {
    ni->DhcpSetTimeout(timeout);
    rv = (int)ni->DhcpStart();
  }
  
  return rv;
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(IPAddress local_ip) {
/* -------------------------------------------------------------------------- */
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress dns_server = local_ip;
  dns_server[3] = 1;
  return begin(local_ip, dns_server);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(IPAddress local_ip, IPAddress dns_server) {
/* -------------------------------------------------------------------------- */  
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  return begin(local_ip, dns_server, gateway);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(IPAddress local_ip, IPAddress dns_server, IPAddress gateway) {
/* -------------------------------------------------------------------------- */  
  IPAddress subnet(255, 255, 255, 0);
  return begin(local_ip, dns_server, gateway, subnet);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet) {
/* -------------------------------------------------------------------------- */  
  
  ethernetTimer = new EthernetClock();
  ethernetTimer->start();
  delay(2);
  
  if (ni != nullptr) {
    ni->config(local_ip, gateway, subnet);
  } else {
    ni = CLwipIf::getInstance().get(NI_ETHERNET, local_ip, gateway, subnet);
    if (ni == nullptr) {
      return 0;
    }
  }

  /* If there is a local DHCP informs it of our manual IP configuration to prevent IP conflict */
  ni->DhcpNotUsed();
  CLwipIf::getInstance().addDns(dns_server);
  return 1;
}

/* -------------------------------------------------------------------------- */
void CEthernet::setDNS(IPAddress dns_server) {
/* -------------------------------------------------------------------------- */  
  CLwipIf::getInstance().addDns(dns_server);
} 

/* -------------------------------------------------------------------------- */
int CEthernet::begin(uint8_t *mac, unsigned long timeout, unsigned long responseTimeout) {
/* -------------------------------------------------------------------------- */  
  CLwipIf::getInstance().setMacAddress(NI_ETHERNET, mac);
  return begin(timeout, responseTimeout);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(uint8_t *mac_address, IPAddress local_ip) {
/* -------------------------------------------------------------------------- */
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress dns_server = local_ip;
  dns_server[3] = 1;
  return begin(mac_address, local_ip, dns_server);
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
int CEthernet::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway) {
/* -------------------------------------------------------------------------- */  
  IPAddress subnet(255, 255, 255, 0);
  return begin(mac_address, local_ip, dns_server, gateway, subnet);
}

/* -------------------------------------------------------------------------- */
int CEthernet::begin(uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet, unsigned long timeout, unsigned long responseTimeout) {
/* -------------------------------------------------------------------------- */  
  (void)responseTimeout;
  (void)timeout;
  CLwipIf::getInstance().setMacAddress(NI_ETHERNET, mac);
  return begin(local_ip, dns_server, gateway, subnet);
}

/* -------------------------------------------------------------------------- */
EthernetLinkStatus CEthernet::linkStatus() {
/* -------------------------------------------------------------------------- */  
  if(ni != nullptr) {
    return (!CLwipIf::getInstance().isEthInitialized()) ? Unknown : (ni->isLinkUp() ? LinkON : LinkOFF);
  }
  return Unknown;
}

/* -------------------------------------------------------------------------- */
EthernetHardwareStatus CEthernet::hardwareStatus() {
/* -------------------------------------------------------------------------- */
  return EthernetLwip;
}

/* -------------------------------------------------------------------------- */
int CEthernet::disconnect() {
/* -------------------------------------------------------------------------- */
  ethernetTimer->stop();
  delete(ethernetTimer);
  ethernetTimer = NULL;
  return 1;
}

/* -------------------------------------------------------------------------- */
int CEthernet::maintain() {
/* -------------------------------------------------------------------------- */  
  int rc = DHCP_CHECK_NONE;

  if (ni != NULL) {
    //we have a pointer to dhcp, use it
    rc = ni->checkLease();
    switch (rc) {
      case DHCP_CHECK_NONE:
        //nothing done
        break;
      case DHCP_CHECK_RENEW_OK:
      case DHCP_CHECK_REBIND_OK:
	//_dnsServerAddress = _dhcp->getDnsServerIp();
        break;
      default:
        //this is actually a error, it will retry though
        break;
    }
  }
  return rc;
}

/*
 * This function updates the LwIP stack and can be called to be sure to update
 * the stack (e.g. in case of a long loop).
 */
void CEthernet::schedule(void) {
  if (ni != NULL) {
    ni->task();
  }
}



uint8_t *CEthernet::MACAddress(void) {
  CLwipIf::getInstance().getMacAddress(NI_ETHERNET, mac_address);
  return mac_address;
}

void CEthernet::MACAddress(uint8_t *mac) {
  CLwipIf::getInstance().getMacAddress(NI_ETHERNET, mac);
}

IPAddress CEthernet::localIP() {
  if(ni != nullptr) {
      return IPAddress(ni->getIpAdd());   
   }
   return IPAddress((uint32_t)0);
}

IPAddress CEthernet::subnetMask() {
  if(ni != nullptr) {
      return IPAddress(ni->getNmAdd());   
   }
   return IPAddress((uint32_t)0);
}

IPAddress CEthernet::gatewayIP() {
  if(ni != nullptr) {
      return IPAddress(ni->getGwAdd());   
   }
   return IPAddress((uint32_t)0);
}

IPAddress CEthernet::dnsServerIP() {
  return CLwipIf::getInstance().getDns();
}

/* -------------------------------------------------------------------------- */
int CEthernet::ping(IPAddress ip, uint8_t ttl) {
/* -------------------------------------------------------------------------- */
  return CLwipIf::getInstance().ping(ip, ttl);
}

/* -------------------------------------------------------------------------- */
int CEthernet::ping(const String &hostname, uint8_t ttl)
/* -------------------------------------------------------------------------- */
{
  return ping(hostname.c_str(), ttl);
}

/* -------------------------------------------------------------------------- */
int CEthernet::ping(const char* host, uint8_t ttl) {
/* -------------------------------------------------------------------------- */
  IPAddress ip;
  if(CLwipIf::getInstance().getHostByName(host,ip)) {
    return CLwipIf::getInstance().ping(ip, ttl);
  }
  return -1;
}

CEthernet Ethernet;
