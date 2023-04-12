#ifndef _ARDUINO_LWIP_NETIF_H_
#define _ARDUINO_LWIP_NETIF_H_

#define LWIP_USE_TIMER

#include "Arduino.h"

#include <map>
#include <string>
#include <queue>
#include "CCtrlWrapper.h"
#include "CEspControl.h"
#include "IPAddress.h"
#include "lwip/include/lwip/netif.h"
#include "lwip/include/lwip/init.h"
#include "lwip/include/lwip/dhcp.h"
#include "lwip/include/lwip/ip_addr.h"
#include "lwip/include/lwip/timeouts.h"
#include "lwip/include/netif/ethernet.h"

#ifdef LWIP_USE_TIMER
#include "FspTimer.h"
#endif

#define MAC_ADDRESS_DIM            6
#define NETWORK_INTERFACES_MAX_NUM 3

#define WIFI_INIT_TIMEOUT_MS  10000

/* DEFAULT ADDRESS FOR ETHERNET CONFIGURATION */

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ETH_0   (uint8_t) 192
#define IP_ETH_1   (uint8_t) 168
#define IP_ETH_2   (uint8_t) 0
#define IP_ETH_3   (uint8_t) 10

/*NETMASK*/
#define NM_ETH_0   (uint8_t) 255
#define NM_ETH_1   (uint8_t) 255
#define NM_ETH_2   (uint8_t) 255
#define NM_ETH_3   (uint8_t) 0

/*Gateway Address*/
#define GW_ETH_0   (uint8_t) 192
#define GW_ETH_1   (uint8_t) 168
#define GW_ETH_2   (uint8_t) 0
#define GW_ETH_3   (uint8_t) 1

/* DEFAULT ADDRESS FOR WIFI STATION CONFIGURATION */

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_WST_0   (uint8_t) 192
#define IP_WST_1   (uint8_t) 168
#define IP_WST_2   (uint8_t) 0
#define IP_WST_3   (uint8_t) 10

/*NETMASK*/
#define NM_WST_0   (uint8_t) 255
#define NM_WST_1   (uint8_t) 255
#define NM_WST_2   (uint8_t) 255
#define NM_WST_3   (uint8_t) 0

/*Gateway Address*/
#define GW_WST_0   (uint8_t) 192
#define GW_WST_1   (uint8_t) 168
#define GW_WST_2   (uint8_t) 0
#define GW_WST_3   (uint8_t) 1

/* DEFAULT ADDRESS FOR WIFI SOFT AP CONFIGURATION */

/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_WSA_0   (uint8_t) 192
#define IP_WSA_1   (uint8_t) 168
#define IP_WSA_2   (uint8_t) 0
#define IP_WSA_3   (uint8_t) 10

/*NETMASK*/
#define NM_WSA_0   (uint8_t) 255
#define NM_WSA_1   (uint8_t) 255
#define NM_WSA_2   (uint8_t) 255
#define NM_WSA_3   (uint8_t) 0

/*Gateway Address*/
#define GW_WSA_0   (uint8_t) 192
#define GW_WSA_1   (uint8_t) 168
#define GW_WSA_2   (uint8_t) 0
#define GW_WSA_3   (uint8_t) 1

#define ETH_IFNAME0 'e'
#define ETH_IFNAME1 't'

#define WST_IFNAME0 'w'
#define WST_IFNAME1 'f'

#define WSA_IFNAME0 'w'
#define WSA_IFNAME1 'a'


typedef enum {
  NI_WIFI_STATION,
  NI_WIFI_SOFTAP, 
  NI_ETHERNET  
} NetIfType_t;


#define MAX_DHCP_TRIES 4


using namespace std;

class CNetIf;

using NetIfRxCb_f = int (*)(CNetIf *);
using LwipInit_f =  err_t (*)(struct netif *netif);
using LwipInput_f = err_t (*)(struct pbuf *p, struct netif *inp);


typedef enum {
  DHCP_IDLE,
  DHCP_START,
  DHCP_WAIT,
  DHCP_GOT,
  DHCP_RELEASE,
  DHCP_STOP
} DhcpSt_t;

/* Base class implements DHCP, derived class will switch it on or off */
/* -------------------------------------------------------------------------- */
class CNetIf {
/* -------------------------------------------------------------------------- */   
protected:
   int    id;
   struct netif ni;

   ip_addr_t ip;
   ip_addr_t nm;
   ip_addr_t gw;

   DhcpSt_t dhcp_st;
   bool dhcp_acquired;
   void dhcp_task();
   
   void setAddr(ip_addr_t *dst, const uint8_t* src, const uint8_t* def);
public:
   CNetIf();
   virtual ~CNetIf();
   /* DHCP functions */
   void DhcpStop();
   void DhcpNotUsed();
   void DhcpStart();
   bool isDhcpAcquired(); 
   /* getters / setters */
   void setId(int _id) { id = _id; }
   int getId() {return id;} 
   
   struct netif *getNi()    { return &ni; }
   ip_addr_t *getIp()       { return &ip; }
   ip_addr_t *getNm()       { return &nm; }
   ip_addr_t *getGw()       { return &gw; }

   /* add */
   virtual void begin(const uint8_t* _ip, 
                      const uint8_t* _gw, 
                      const uint8_t* _nm) = 0;
   virtual void task() = 0;  

   virtual void setIp(const uint8_t *_ip) = 0;
   virtual void setNm(const uint8_t *_nm) = 0;
   virtual void setGw(const uint8_t *_gw) = 0;
};



/* -------------------------------------------------------------------------- */
class CEthernet : public CNetIf {
/* -------------------------------------------------------------------------- */   
   protected:


   public:
   CEthernet();
   virtual ~CEthernet();
   virtual void begin(const uint8_t* _ip, 
                      const uint8_t* _gw, 
                      const uint8_t* _nm) override;
   virtual void task() override;


   void setIp(const uint8_t *_ip) override;
   void setNm(const uint8_t *_nm) override;
   void setGw(const uint8_t *_gw) override;


};



/* -------------------------------------------------------------------------- */
class CWifiStation : public CNetIf {
/* -------------------------------------------------------------------------- */   
   protected:


   public:
   CWifiStation();
   virtual ~CWifiStation();
   virtual void begin(const uint8_t* _ip, 
                      const uint8_t* _gw, 
                      const uint8_t* _nm) override;
   virtual void task() override;

   void setIp(const uint8_t *_ip) override;
   void setNm(const uint8_t *_nm) override;
   void setGw(const uint8_t *_gw) override;

};

/* -------------------------------------------------------------------------- */
class CWifiSoftAp : public CNetIf {
/* -------------------------------------------------------------------------- */   
   public:
   CWifiSoftAp();
   virtual ~CWifiSoftAp();
   virtual void begin(const uint8_t* _ip, 
                      const uint8_t* _gw, 
                      const uint8_t* _nm) override;
   virtual void task() override;

   void setIp(const uint8_t *_ip) override;
   void setNm(const uint8_t *_nm) override;
   void setGw(const uint8_t *_gw) override;
};


/* -------------------------------------------------------------------------- */
class CLwipIf {
/* -------------------------------------------------------------------------- */   
private:
   static CNetIf * net_ifs[NETWORK_INTERFACES_MAX_NUM];
   

   /* initialize lwIP and timer */
   CLwipIf();
   
   /* timer */
   #ifdef LWIP_USE_TIMER
   FspTimer timer;
   static void timer_cb(timer_callback_args_t *arg);
   #endif


   CNetIf *_get(NetIfType_t t);

   CNetIf *setUpEthernet(const uint8_t* _ip, 
                         const uint8_t* _gw, 
                         const uint8_t* _nm);

   static bool wifi_hw_initialized;
   static int initEventCb(CCtrlMsgWrapper *resp);

   static void initWifiHw();


public:
   static CLwipIf& getInstance();
   CLwipIf(CLwipIf const&)            = delete;
   void operator=(CLwipIf const&)     = delete;
   ~CLwipIf();
   
   /* 
    * these functions are passed to netif_add function as 'init' function for
    * that network interface
    */

   static err_t initEth(struct netif* ni);
   static err_t initWifiStation(struct netif* ni);
   static err_t initWifiSoftAp(struct netif* ni);

   /* 
    * these functions are passed to are set to netif->linkoutput function during
    * the execution of the previous init function
    */

   static err_t ouputEth(struct netif *ni, struct pbuf *p);
   static err_t outputWifiStation(struct netif *netif, struct pbuf *p);
   static err_t outputWifiSoftAp(struct netif *netif, struct pbuf *p);
   
   /* when you 'get' a network interface, you get a pointer to one of the pointers
      held by net_ifs array
      if the array element then an attempt to set up the network interface is made
      this function actually calls the private function setUp... and that ones
      call the private _get */


   CNetIf *get(NetIfType_t type, 
               const uint8_t* _ip = nullptr, 
               const uint8_t* _gw = nullptr, 
               const uint8_t* _nm = nullptr);
   
   bool setMacAddress(NetIfType_t type, uint8_t* mac);
   int getMacAddress(NetIfType_t type, uint8_t* mac);

   

   void lwip_task();
};



#endif