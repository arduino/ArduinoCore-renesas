#ifndef _ARDUINO_LWIP_NETIF_H_
#define _ARDUINO_LWIP_NETIF_H_

//#define LWIP_USE_TIMER

#include "Arduino.h"

#include <map>
#include <string>
#include <queue>
#include "CCtrlWrapper.h"
#include "CEspControl.h"
#include "IPAddress.h"
#include "lwip/include/lwip/ip_addr.h"
#include "lwip/include/lwip/dhcp.h"
#include "lwip/include/lwip/udp.h"
#include "lwip/include/lwip/tcp.h"
#include "lwip/include/lwip/opt.h"
#include "lwip/include/lwip/init.h"
#include "lwip/include/lwip/timeouts.h"
#include "lwip/include/lwip/dns.h"
#include "lwip/include/lwip/prot/dhcp.h"
#include "lwip/include/netif/ethernet.h"

#define LWIP_USE_TIMER

#ifdef LWIP_USE_TIMER
#include "FspTimer.h"
#endif

#define MAC_ADDRESS_DIM            6
#define NETWORK_INTERFACES_MAX_NUM 3
#define MAX_HOSTNAME_DIM           253

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
   WL_NO_SHIELD = 255,
   WL_NO_MODULE = WL_NO_SHIELD,
   WL_IDLE_STATUS = 0,
   WL_NO_SSID_AVAIL,
   WL_SCAN_COMPLETED,
   WL_CONNECTED,
   WL_CONNECT_FAILED,
   WL_CONNECTION_LOST,
   WL_DISCONNECTED,
   WL_AP_LISTENING,
   WL_AP_CONNECTED,
   WL_AP_FAILED
} WifiStatus_t;



typedef enum {
  NI_WIFI_STATION,
  NI_WIFI_SOFTAP, 
  NI_ETHERNET  
} NetIfType_t;

#define MAX_CLIENT  32
#define MAX_DHCP_TRIES 4
#define TIMEOUT_DNS_REQUEST 10000U


using namespace std;

class CNetIf;

using NetIfRxCb_f = int (*)(CNetIf *);
using LwipInit_f =  err_t (*)(struct netif *netif);
using LwipInput_f = err_t (*)(struct pbuf *p, struct netif *inp);

#define DHCP_CHECK_NONE         (0)
#define DHCP_CHECK_RENEW_FAIL   (1)
#define DHCP_CHECK_RENEW_OK     (2)
#define DHCP_CHECK_REBIND_FAIL  (3)
#define DHCP_CHECK_REBIND_OK    (4)


#define TIMED_OUT        -1
#define INVALID_SERVER   -2
#define TRUNCATED        -3
#define INVALID_RESPONSE -4

typedef enum {
  DHCP_IDLE_STATUS,
  DHCP_START_STATUS,
  DHCP_WAIT_STATUS,
  DHCP_GOT_STATUS,
  DHCP_RELEASE_STATUS,
  DHCP_STOP_STATUS
} DhcpSt_t;


ip_addr_t *u8_to_ip_addr(uint8_t *ipu8, ip_addr_t *ipaddr);


uint32_t ip_addr_to_u32(ip_addr_t *ipaddr) ;

/* Base class implements DHCP, derived class will switch it on or off */
/* -------------------------------------------------------------------------- */
class CNetIf {
/* -------------------------------------------------------------------------- */   
protected:
   int    id;
   struct netif ni;
   #if LWIP_NETIF_HOSTNAME
   char hostname[MAX_HOSTNAME_DIM];
   #endif

   ip_addr_t ip;
   ip_addr_t nm;
   ip_addr_t gw;

   unsigned long dhcp_timeout;
   DhcpSt_t dhcp_st;
   bool dhcp_started;
   bool dhcp_acquired;
   uint8_t _dhcp_lease_state;
   void dhcp_task();
   void dhcp_reset();
   bool dhcp_request();
   uint8_t dhcp_get_lease_state();
   
   IPAddress _dnsServerAddress;
   
   void setAddr(ip_addr_t *dst, const uint8_t* src, const uint8_t* def);
public:
   CNetIf();
   virtual ~CNetIf();
   /* -------------- 
    * DHCP functions 
    * -------------- */
   bool DhcpIsStarted() { return dhcp_started; }
   void DhcpSetTimeout(unsigned long t);
   /* stops DHCP */
   void DhcpStop();
   /* tells DHCP is not used on that interface */
   void DhcpNotUsed();
   /* starts DHCP and tries to acquire addresses, return true if acquired, false otherwise */
   bool DhcpStart();
   /* tells if DHCP has acquired addresses or not */
   bool isDhcpAcquired(); 
   int checkLease();

   


   
   /* getters / setters */
   void setId(int _id) { id = _id; }
   int getId() {return id;} 
   
   struct netif *getNi()    { return &ni; }
   ip_addr_t *getIp()       { return &ip; }
   ip_addr_t *getNm()       { return &nm; }
   ip_addr_t *getGw()       { return &gw; }

   uint32_t getIpAdd() { return ip4_addr_get_u32(&(ni.ip_addr));}
   uint32_t getNmAdd() { return ip4_addr_get_u32(&(ni.gw));}
   uint32_t getGwAdd() { return ip4_addr_get_u32(&(ni.netmask));}

   void setHostname(const char* name) {
      memset(hostname,0x00,MAX_HOSTNAME_DIM);
      memcpy(hostname,name,strlen(name) < MAX_HOSTNAME_DIM ? strlen(name) : MAX_HOSTNAME_DIM);
   }

   /* add */
   virtual void begin(const uint8_t* _ip, 
                      const uint8_t* _gw, 
                      const uint8_t* _nm) = 0;
   virtual void task() = 0;  

   virtual void setIp(const uint8_t *_ip) = 0;
   virtual void setNm(const uint8_t *_nm) = 0;
   virtual void setGw(const uint8_t *_gw) = 0;

   virtual int getMacAddress(uint8_t *mac) = 0;
};



/* -------------------------------------------------------------------------- */
class CEth: public CNetIf {
/* -------------------------------------------------------------------------- */   
   protected:


   public:
   CEth();
   virtual ~CEth();
   virtual void begin(const uint8_t* _ip, 
                      const uint8_t* _gw, 
                      const uint8_t* _nm) override;
   virtual void task() override;


   void setIp(const uint8_t *_ip) override;
   void setNm(const uint8_t *_nm) override;
   void setGw(const uint8_t *_gw) override;


   virtual int getMacAddress(uint8_t *mac) {}

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

   virtual int getMacAddress(uint8_t *mac) override;

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

   virtual int getMacAddress(uint8_t *mac) override;
};




/* -------------------------------------------------------------------------- */
class CLwipIf {
/* -------------------------------------------------------------------------- */   
private:
   static CNetIf * net_ifs[NETWORK_INTERFACES_MAX_NUM];
   static WifiStatus_t wifi_status;

   bool willing_to_start_sync_req;
   bool async_requests_ongoing;

   /* initialize lwIP and timer */
   CLwipIf();
   
   /* timer */
   #ifdef LWIP_USE_TIMER
   static FspTimer timer;
   static void timer_cb(timer_callback_args_t *arg);
   #endif
   
   vector<AccessPoint_t> access_points;
   WifiApCfg_t access_point_cfg;

   CNetIf *_get(NetIfType_t t);

   CNetIf *setUpEthernet(const uint8_t* _ip, 
                         const uint8_t* _gw, 
                         const uint8_t* _nm);

   static bool wifi_hw_initialized;
   static bool connected_to_access_point;
   static int initEventCb(CCtrlMsgWrapper *resp);
   static bool initWifiHw(bool asStation);
   static int disconnectEventcb(CCtrlMsgWrapper *resp);

   int dns_num;
   static void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg);
   int8_t get_ip_address_from_hostname(const char *hostname, uint32_t *ipaddr);
   int inet2aton(const char *aIPAddrString, IPAddress &aResult);
   
public:
   static CLwipIf& getInstance();
   CLwipIf(CLwipIf const&)            = delete;
   void operator=(CLwipIf const&)     = delete;
   ~CLwipIf();

   void startSyncRequest() {
      if(async_requests_ongoing) {
         synchronized {
           willing_to_start_sync_req = true;
         }
         while(willing_to_start_sync_req) {
            delay(1);
         }
      }
   }

   void restartAsyncRequest() {
      async_requests_ongoing = true;
      timer.enable_overflow_irq();
   }

   /* --------------
    * DNS functions
    * -------------- */
   
   int getHostByName(const char *aHostname, IPAddress &aResult);
   void beginDns(IPAddress aDNSServer);
   void addDns(IPAddress aDNSServer);
   IPAddress getDns(int _num = 0);
   
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
   
   /* this function set the mac address of the corresponding interface to mac 
      and set this value for lwip */
   bool setMacAddress(NetIfType_t type, uint8_t* mac = nullptr);
   int getMacAddress(NetIfType_t type, uint8_t* mac);

   
   int scanForAp();
   int getApNum();
   const char *getSSID(uint8_t i);
   int32_t getRSSI(uint8_t i);
   uint8_t getEncrType(uint8_t i);
   uint8_t *getBSSID(uint8_t i, uint8_t *bssid);
   uint8_t getChannel(uint8_t i);
   int connectToAp(const char *ssid, const char *pwd);
   int disconnectFromAp();
   const char* getSSID();
   uint8_t *getBSSID(uint8_t *bssid);
   uint32_t getRSSI();
   uint8_t getEncrType();

   WifiStatus_t getWifiStatus() { return wifi_status; }

   

   void lwip_task();
};



#endif