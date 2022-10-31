#include "STM32Ethernet.h"
#include "Dhcp.h"

int EthernetClass::begin(unsigned long timeout, unsigned long responseTimeout)
{
  static DhcpClass s_dhcp;
  _dhcp = &s_dhcp;
  add_eth0_interface(MACAddressDefault(), NULL, NULL, NULL);

  // Now try to get our config info from a DHCP server
  int ret = _dhcp->beginWithDHCP(mac_address, timeout, responseTimeout);
  if (ret == 1) {
    _dnsServerAddress = _dhcp->getDnsServerIp();
  }

  return ret;
}

void EthernetClass::begin(IPAddress local_ip)
{
  IPAddress subnet(255, 255, 255, 0);
  begin(local_ip, subnet);
}

void EthernetClass::begin(IPAddress local_ip, IPAddress subnet)
{
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  begin(local_ip, subnet, gateway);
}

void EthernetClass::begin(IPAddress local_ip, IPAddress subnet, IPAddress gateway)
{
  // Assume the DNS server will be the same machine than gateway
  begin(local_ip, subnet, gateway, gateway);
}

void EthernetClass::begin(IPAddress local_ip, IPAddress subnet, IPAddress gateway, IPAddress dns_server)
{
  add_eth0_interface(MACAddressDefault(), local_ip.raw_address(), gateway.raw_address(), subnet.raw_address());
  /* If there is a local DHCP informs it of our manual IP configuration to
  prevent IP conflict */
  set_dhcp_not_used();
  _dnsServerAddress = dns_server;
}

int EthernetClass::begin(uint8_t *mac_address, unsigned long timeout, unsigned long responseTimeout)
{
  static DhcpClass s_dhcp;
  _dhcp = &s_dhcp;

  add_eth0_interface(mac_address, NULL, NULL, NULL);

  // Now try to get our config info from a DHCP server
  int ret = _dhcp->beginWithDHCP(mac_address, timeout, responseTimeout);
  if (ret == 1) {
    _dnsServerAddress = _dhcp->getDnsServerIp();
  }
  MACAddress(mac_address);
  return ret;
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip)
{
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress dns_server = local_ip;
  dns_server[3] = 1;
  begin(mac_address, local_ip, dns_server);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server)
{
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  begin(mac_address, local_ip, dns_server, gateway);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
  IPAddress subnet(255, 255, 255, 0);
  begin(mac_address, local_ip, dns_server, gateway, subnet);
}

void EthernetClass::begin(uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
  add_eth0_interface(mac, local_ip.raw_address(), gateway.raw_address(), subnet.raw_address());
  /* If there is a local DHCP informs it of our manual IP configuration to
  prevent IP conflict */
  set_dhcp_not_used();
  _dnsServerAddress = dns_server;
  MACAddress(mac);
}

EthernetLinkStatus EthernetClass::linkStatus()
{
  return (!is_eth0_initialized()) ? Unknown : (is_eth0_link_up() ? LinkON : LinkOFF);
}

int EthernetClass::maintain()
{
  int rc = DHCP_CHECK_NONE;

  if (_dhcp != NULL) {
    //we have a pointer to dhcp, use it
    rc = _dhcp->checkLease();
    switch (rc) {
      case DHCP_CHECK_NONE:
        //nothing done
        break;
      case DHCP_CHECK_RENEW_OK:
      case DHCP_CHECK_REBIND_OK:
        _dnsServerAddress = _dhcp->getDnsServerIp();
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
void EthernetClass::schedule(void)
{
  lwip_task();
}

uint8_t *EthernetClass::MACAddressDefault(void)
{
  if ((mac_address[0] + mac_address[1] + mac_address[2] + mac_address[3] + mac_address[4] + mac_address[5]) == 0) {
    
    mac_address[0] = 0xAA;
    mac_address[1] = 0xBB;
    mac_address[2] = 0xCC;
    mac_address[3] = 0xDD;
    mac_address[4] = 0xEE;
    mac_address[5] = 0xFF;
  }
  return mac_address;
}

void EthernetClass::MACAddress(uint8_t *mac)
{
  mac_address[0] = mac[0];
  mac_address[1] = mac[1];
  mac_address[2] = mac[2];
  mac_address[3] = mac[3];
  mac_address[4] = mac[4];
  mac_address[5] = mac[5];
}

uint8_t *EthernetClass::MACAddress(void)
{
  return mac_address;
}

IPAddress EthernetClass::localIP()
{
  return IPAddress(get_eth0_ip_add());
}

IPAddress EthernetClass::subnetMask()
{
  return IPAddress(get_eth0_gw_add());
}

IPAddress EthernetClass::gatewayIP()
{
  return IPAddress(get_eth0_nm_add());
}

IPAddress EthernetClass::dnsServerIP()
{
  return _dnsServerAddress;
}

EthernetClass Ethernet;
