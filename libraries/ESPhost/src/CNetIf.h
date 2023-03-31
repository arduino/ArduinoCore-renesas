#ifndef _ARDUINO_LWIP_NETIF_H_
#define _ARDUINO_LWIP_NETIF_H_

#include "Arduino.h"

#include <map>
#include <string>
#include <queue>

#include "IPAddress.h"
#include "lwip/include/lwip/netif.h"
#include "lwip/include/lwip/init.h"
#include "lwip/include/lwip/dhcp.h"
#include "lwip/include/lwip/ip_addr.h"

#define MAX_DHCP_TRIES 4


using namespace std;

class CNetIf;

using NetIfRxCb_f = int (*)(CNetIf *);
using LwipInit_f =  err_t (*)(struct netif *netif);
using LwipInput_f = err_t (*)(struct pbuf *p, struct netif *inp);

#define NET_IP_ADD_TYPE 0
#define NET_GW_ADD_TYPE 1
#define NET_NM_ADD_TYPE 2 


typedef enum {
  DHCP_IDLE,
  DHCP_START,
  DHCP_WAIT,
  DHCP_GOT,
  DHCP_RELEASE,
  DHCP_STOP
} DhcpSt_t;


/* -------------------------------------------------------------------------- */
class CNetIf {
/* -------------------------------------------------------------------------- */   
private:
   static int id;
   NetIfRxCb_f rx_cb;
   struct netif ni;

   ip_addr_t ip;
   ip_addr_t nm;
   ip_addr_t gw;
   
   /* DHCP variables */
   DhcpSt_t dhcp_st;
   bool dhcp_acquired;
   void dhcp_task();


   void setAddr(ip_addr_t *dst, const uint8_t* src);
public:
   void setIp(const uint8_t *_ip) { setAddr(&ip, _ip); }
   void setNm(const uint8_t *_nm) { setAddr(&nm, _nm); }
   void setGw(const uint8_t *_gw) { setAddr(&gw, _gw); }
   struct netif *getNi()    { return &ni; }
   ip_addr_t *getIp()       { return &ip; }
   ip_addr_t *getNm()       { return &nm; }
   ip_addr_t *getGw()       { return &gw; }

   void task();
   

   void DhcpStop();
   void DhcpNotUsed();
   void DhcpStart();
   bool isDhcpAcquired(); 



   CNetIf();
   CNetIf(NetIfRxCb_f _rx_cb);
   int tx(uint8_t *buffer, uint32_t dim);

};

/* -------------------------------------------------------------------------- */
class CLwipIf {
/* -------------------------------------------------------------------------- */   
private:
   CLwipIf();
   
   std::map<string,CNetIf *> net_ifs;
   
   void init_lwip();
   bool is_lwip_initialized;



public:
   static CLwipIf& getInstance();
   CLwipIf(CLwipIf const&)            = delete;
   void operator=(CLwipIf const&)     = delete;
   ~CLwipIf();
   
   /* call when DHCP is used */
   CNetIf *add(string name, 
               LwipInit_f init, 
               LwipInput_f input);
   /* call when DHCP is not used and address are explicity set */
   CNetIf *add(string name, 
               const uint8_t* _ip,
               const uint8_t* _gw, 
               const uint8_t* _nm, 
               LwipInit_f init, 
               LwipInput_f input);
   CNetIf close(string name);
   CNetIf *get(string name);

   void lwip_task();
};



#endif