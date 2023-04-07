#ifndef _ARDUINO_LWIP_NETIF_H_
#define _ARDUINO_LWIP_NETIF_H_

#define LWIP_USE_TIMER

#include "Arduino.h"

#include <map>
#include <string>
#include <queue>

#include "IPAddress.h"
#include "lwip/include/lwip/netif.h"
#include "lwip/include/lwip/init.h"
#include "lwip/include/lwip/dhcp.h"
#include "lwip/include/lwip/ip_addr.h"

#ifdef LWIP_USE_TIMER
#include "FspTimer.h"
#endif

#define NETWORK_INTERFACES_MAX_NUM 3

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
   
   void setAddr(ip_addr_t *dst, const uint8_t* src);
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
   void setIp(const uint8_t *_ip) { setAddr(&ip, _ip); }
   void setNm(const uint8_t *_nm) { setAddr(&nm, _nm); }
   void setGw(const uint8_t *_gw) { setAddr(&gw, _gw); }
   struct netif *getNi()    { return &ni; }
   ip_addr_t *getIp()       { return &ip; }
   ip_addr_t *getNm()       { return &nm; }
   ip_addr_t *getGw()       { return &gw; }

   /* add */
   virtual void add() = 0;
   virtual void task() = 0;  
};



/* -------------------------------------------------------------------------- */
class CEthernet : public CNetIf {
/* -------------------------------------------------------------------------- */   
   protected:


   public:
   CEthernet();
   virtual ~CEthernet();
   virtual void add() override {}
   virtual void task() override {}


};



/* -------------------------------------------------------------------------- */
class CWifiStation : public CNetIf {
/* -------------------------------------------------------------------------- */   
   protected:


   public:
   CWifiStation();
   virtual ~CWifiStation();
   virtual void add() override;
   virtual void task() override;

};

/* -------------------------------------------------------------------------- */
class CWifiSoftAp : public CNetIf {
/* -------------------------------------------------------------------------- */   
   public:
   CWifiSoftAp();
   virtual ~CWifiSoftAp();
   virtual void add() override;
   virtual void task() override;
};


/* -------------------------------------------------------------------------- */
class CLwipIf {
/* -------------------------------------------------------------------------- */   
private:
   CNetIf * net_ifs[NETWORK_INTERFACES_MAX_NUM];
   

   /* initialize lwIP and timer */
   CLwipIf();
   
   /* timer */
   #ifdef LWIP_USE_TIMER
   FspTimer timer;
   static void timer_cb(timer_callback_args_t *arg);
   #endif

   CNetIf *setUpWifiStation(const uint8_t* _ip, 
                            const uint8_t* _gw, 
                            const uint8_t* _nm);

   CNetIf *setUpWifiSoftAp(const uint8_t* _ip, 
                           const uint8_t* _gw, 
                           const uint8_t* _nm);

   


   CNetIf *setUpEthernet(const uint8_t* _ip, 
                         const uint8_t* _gw, 
                         const uint8_t* _nm);

public:
   static CLwipIf& getInstance();
   CLwipIf(CLwipIf const&)            = delete;
   void operator=(CLwipIf const&)     = delete;
   ~CLwipIf();
   
   
   CNetIf *get(NetIfType_t type, 
               const uint8_t* _ip = nullptr, 
               const uint8_t* _gw = nullptr, 
               const uint8_t* _nm = nullptr);
   


   

   void lwip_task();
};



#endif