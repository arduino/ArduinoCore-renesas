#ifndef ARDUINO_C_ETHERNET_H
#define ARDUINO_C_ETHERNET_H

#ifndef ARDUINO_PORTENTA_C33
// force discovering wth shield library
// this doesn't work ATM
// a check and a bailout should be added to all the other files too
#include "utility/w5100.h"
#endif

#include <inttypes.h>
#include "IPAddress.h"

#include "EthernetClient.h"
#include "EthernetServer.h"

#include "CNetIf.h"
#include "lwipMem.h"

enum EthernetLinkStatus {
  Unknown,
  LinkON,
  LinkOFF
};

enum EthernetHardwareStatus {
  EthernetNoHardware,
  EthernetLwip = 7
};

class CEthernet {

  private:
    CNetIf *ni;
    
    uint8_t mac_address[6];
  public:
    // Initialise the Ethernet with the internal provided MAC address and gain the rest of the
    // configuration through DHCP.
    // Returns 0 if the DHCP configuration failed, and 1 if it succeeded
    int begin(unsigned long timeout = 60000, unsigned long responseTimeout = 4000);
    EthernetLinkStatus linkStatus();
    int begin(IPAddress local_ip);
    int begin(IPAddress local_ip, IPAddress subnet);
    int begin(IPAddress local_ip, IPAddress subnet, IPAddress gateway);
    int begin(IPAddress local_ip, IPAddress subnet, IPAddress gateway, IPAddress dns_server);
    // Initialise the Ethernet shield to use the provided MAC address and gain the rest of the
    // configuration through DHCP.
    // Returns 0 if the DHCP configuration failed, and 1 if it succeeded
    int begin(uint8_t *mac_address, unsigned long timeout = 60000, unsigned long responseTimeout = 4000);
    int begin(uint8_t *mac_address, IPAddress local_ip);
    int begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server);
    int begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway);
    int begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet, unsigned long timeout = 60000, unsigned long responseTimeout = 4000);
    EthernetHardwareStatus hardwareStatus();

    void setDNS(IPAddress dns_server); 

    int disconnect(void);
    int maintain();
    void schedule(void);

   
    uint8_t *MACAddress(void);
    IPAddress localIP();
    IPAddress subnetMask();
    IPAddress gatewayIP();
    IPAddress dnsServerIP();

    friend class EthernetClient;
    friend class EthernetServer;
};

extern CEthernet Ethernet;

#endif
