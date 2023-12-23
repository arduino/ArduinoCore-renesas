#ifndef _ARDUINO_LWIP_NETIF_H_
#define _ARDUINO_LWIP_NETIF_H_

// #define LWIP_USE_TIMER
#define UNUSED(x) (void)(x)

#define USE_LWIP_AS_LIBRARY
#include "Arduino.h"

#include "CCtrlWrapper.h"
#include "CEspControl.h"
#include "IPAddress.h"
#include "EthernetDriver.h"
#include <string>
#ifdef USE_LWIP_AS_LIBRARY
#include "lwip/include/lwip/dhcp.h"
#include "lwip/include/lwip/dns.h"
#include "lwip/include/lwip/init.h"
#include "lwip/include/lwip/ip_addr.h"
#include "lwip/include/lwip/opt.h"
#include "lwip/include/lwip/prot/dhcp.h"
#include "lwip/include/lwip/tcp.h"
#include "lwip/include/lwip/timeouts.h"
#include "lwip/include/lwip/udp.h"
#include "lwip/include/netif/ethernet.h"
#else
#include "lwIP_Arduino.h"
#endif

#define LWIP_USE_TIMER

#ifdef LWIP_USE_TIMER
#include "FspTimer.h"
#endif

#define MAX_SOFAT_CONNECTION_DEF 5

#define MAC_ADDRESS_DIM 6
#define NETWORK_INTERFACES_MAX_NUM 3
#define MAX_HOSTNAME_DIM 253

#define WIFI_INIT_TIMEOUT_MS 10000

#define WL_MAC_ADDR_LENGTH 6

/* DEFAULT ADDRESS FOR ETHERNET CONFIGURATION */

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

/* Encryption modes */
enum wl_enc_type {
    ENC_TYPE_WEP,
    ENC_TYPE_WPA,
    ENC_TYPE_TKIP = ENC_TYPE_WPA,
    ENC_TYPE_WPA2,
    ENC_TYPE_CCMP = ENC_TYPE_WPA2,
    ENC_TYPE_WPA2_ENTERPRISE,
    ENC_TYPE_WPA3,
    ENC_TYPE_NONE,
    ENC_TYPE_AUTO,

    ENC_TYPE_UNKNOWN = 255
};

typedef enum {
    NI_WIFI_STATION,
    NI_WIFI_SOFTAP,
    NI_ETHERNET
} NetIfType_t;

#define MAX_CLIENT 32
#define MAX_DHCP_TRIES 4
#define TIMEOUT_DNS_REQUEST 10000U

class CNetIf;

using NetIfRxCb_f = int (*)(CNetIf*);
using LwipInit_f = err_t (*)(struct netif* netif);
using LwipInput_f = err_t (*)(struct pbuf* p, struct netif* inp);

#define DHCP_CHECK_NONE (0)
#define DHCP_CHECK_RENEW_FAIL (1)
#define DHCP_CHECK_RENEW_OK (2)
#define DHCP_CHECK_REBIND_FAIL (3)
#define DHCP_CHECK_REBIND_OK (4)

#define TIMED_OUT -1
#define INVALID_SERVER -2
#define TRUNCATED -3
#define INVALID_RESPONSE -4

typedef enum {
    DHCP_IDLE_STATUS,
    DHCP_START_STATUS,
    DHCP_WAIT_STATUS,
    DHCP_GOT_STATUS,
    DHCP_RELEASE_STATUS,
    DHCP_STOP_STATUS
} DhcpSt_t;

ip_addr_t* u8_to_ip_addr(uint8_t* ipu8, ip_addr_t* ipaddr);

uint32_t ip_addr_to_u32(ip_addr_t* ipaddr);

/* Base class implements DHCP, derived class will switch it on or off */
/* -------------------------------------------------------------------------- */
class CNetIf {
    /* -------------------------------------------------------------------------- */
protected:
    int id;
    struct netif ni;
#if LWIP_NETIF_HOSTNAME
    char hostname[MAX_HOSTNAME_DIM];
#endif

    ip_addr_t ip;
    ip_addr_t nm;
    ip_addr_t gw;

    /* these can be overridden by a config() function called before begin() */
    static IPAddress default_ip;
    static IPAddress default_nm;
    static IPAddress default_gw;
    static IPAddress default_dhcp_server_ip;

    unsigned long dhcp_timeout;
    DhcpSt_t dhcp_st;
    bool dhcp_started;
    volatile bool dhcp_acquired;
    uint8_t _dhcp_lease_state;
    void dhcp_task();
    void dhcp_reset();
    bool dhcp_request();
    uint8_t dhcp_get_lease_state();

    IPAddress _dnsServerAddress;

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

    virtual void setLinkUp();
    virtual void setLinkDown();
    bool isLinkUp() { return (bool)netif_is_link_up(&ni); }

    /* getters / setters */
    void setId(int _id) { id = _id; }
    int getId() { return id; }

    struct netif* getNi() { return &ni; }

    uint32_t getIpAdd() { return ip4_addr_get_u32(&(ni.ip_addr)); }
    uint32_t getNmAdd() { return ip4_addr_get_u32(&(ni.netmask)); }
    uint32_t getGwAdd() { return ip4_addr_get_u32(&(ni.gw)); }

    void setHostname(const char* name)
    {
        memset(hostname, 0x00, MAX_HOSTNAME_DIM);
        memcpy(hostname, name, strlen(name) < MAX_HOSTNAME_DIM ? strlen(name) : MAX_HOSTNAME_DIM);
    }

    /* add */
    virtual void begin(IPAddress _ip,
        IPAddress _gw,
        IPAddress _nm)
        = 0;
    virtual void task() = 0;

    virtual int getMacAddress(uint8_t* mac) = 0;

    /* default dummy implementation because ethernet does not have that */
    virtual const char* getSSID() { return nullptr; }
    virtual uint8_t* getBSSID(uint8_t* bssid) { return nullptr; }
    virtual int32_t getRSSI() { return 0; }
    virtual uint8_t getEncryptionType() { return 0; }

   friend class CWifi;
};

/* -------------------------------------------------------------------------- */
class CEth : public CNetIf {
    /* -------------------------------------------------------------------------- */
protected:
    /*
     * this function is used to initialize the netif structure of lwip
     */
    static err_t init(struct netif* ni);

    /*
     * This function is passed to lwip and used to send a buffer to the driver in order to transmit it
     */
    static err_t output(struct netif* ni, struct pbuf* p);
public:
    CEth();
    virtual ~CEth();
    virtual void begin(IPAddress _ip,
        IPAddress _gw,
        IPAddress _nm) override;
    virtual void task() override;

    virtual int getMacAddress(uint8_t* mac)
    {
        UNUSED(mac);
        return 1;
    }

    virtual void handleEthRx();
};

/* -------------------------------------------------------------------------- */
class CWifiStation : public CNetIf {
    /* -------------------------------------------------------------------------- */
protected:
    /*
     * this function is used to initialize the netif structure of lwip
     */
    static err_t init(struct netif* ni);

    /*
     * This function is passed to lwip and used to send a buffer to the driver in order to transmit it
     */
    static err_t output(struct netif* ni, struct pbuf* p);
public:
    CWifiStation();
    virtual ~CWifiStation();
    virtual void begin(IPAddress _ip,
        IPAddress _gw,
        IPAddress _nm) override;
    virtual void task() override;

    virtual int getMacAddress(uint8_t* mac) override;

    virtual const char* getSSID() override;
    virtual uint8_t* getBSSID(uint8_t* bssid) override;
    virtual int32_t getRSSI() override;
    virtual uint8_t getEncryptionType() override;
};

/* -------------------------------------------------------------------------- */
class CWifiSoftAp : public CNetIf {
    /* -------------------------------------------------------------------------- */
protected:
    /*
     * this function is used to initialize the netif structure of lwip
     */
    static err_t init(struct netif* ni);

    /*
     * This function is passed to lwip and used to send a buffer to the driver in order to transmit it
     */
    static err_t output(struct netif* ni, struct pbuf* p);
public:
    CWifiSoftAp();
    virtual ~CWifiSoftAp();
    virtual void begin(IPAddress _ip,
        IPAddress _gw,
        IPAddress _nm) override;
    virtual void task() override;

    virtual int getMacAddress(uint8_t* mac) override;

    virtual const char* getSSID() override;
    virtual uint8_t* getBSSID(uint8_t* bssid) override;
    virtual int32_t getRSSI() override;
    virtual uint8_t getEncryptionType() override;
};

/* -------------------------------------------------------------------------- */
class CLwipIf {
    /* -------------------------------------------------------------------------- */
private:
    bool eth_initialized;

    int dns_num;
    bool willing_to_start_sync_req;
    bool async_requests_ongoing;

    friend CWifiStation;
    friend CWifiSoftAp;
    static CNetIf* net_ifs[NETWORK_INTERFACES_MAX_NUM];
    static WifiStatus_t wifi_status;

    /* initialize lwIP and timer */
    CLwipIf();

/* timer */
#ifdef LWIP_USE_TIMER
    static FspTimer timer;
    static void timer_cb(timer_callback_args_t* arg);
#endif

    std::vector<AccessPoint_t> access_points;
    WifiApCfg_t access_point_cfg;

    SoftApCfg_t soft_ap_cfg;

    static bool wifi_hw_initialized;
    static bool connected_to_access_point;
    static int initEventCb(CCtrlMsgWrapper* resp);
    static bool initWifiHw(bool asStation);

    static bool pending_eth_rx;

    static int disconnectEventcb(CCtrlMsgWrapper* resp);

    static void dns_callback(const char* name, const ip_addr_t* ipaddr, void* callback_arg);
    int8_t get_ip_address_from_hostname(const char* hostname, uint32_t* ipaddr);
    int inet2aton(const char* aIPAddrString, IPAddress& aResult);

public:
    static CLwipIf& getInstance();
    CLwipIf(CLwipIf const&) = delete;
    void operator=(CLwipIf const&) = delete;
    ~CLwipIf();

    bool isEthInitialized() { return eth_initialized; }

    void startSyncRequest()
    {
        if (async_requests_ongoing) {
            synchronized
            {
                willing_to_start_sync_req = true;
            }
            while (willing_to_start_sync_req) {
                delay(1);
            }
        }
    }

    void restartAsyncRequest()
    {
        async_requests_ongoing = true;
        delay(10);
        timer.enable_overflow_irq();
    }

    /* --------------
     * DNS functions
     * -------------- */

    int getHostByName(const char* aHostname, IPAddress& aResult);
    void beginDns(IPAddress aDNSServer);
    void addDns(IPAddress aDNSServer);
    IPAddress getDns(int _num = 0);

    /* when you 'get' a network interface, you get a pointer to one of the pointers
       held by net_ifs array
       if the array element then an attempt to set up the network interface is made
       this function actually calls the private function setUp... and that ones
       call the private _get */

    CNetIf* get(NetIfType_t type,
        IPAddress _ip = INADDR_NONE,
        IPAddress _gw = INADDR_NONE,
        IPAddress _nm = INADDR_NONE);

    static void ethLinkUp();
    static void ethLinkDown();

    /* this function set the mac address of the corresponding interface to mac
       and set this value for lwip */
    bool setMacAddress(NetIfType_t type, uint8_t* mac = nullptr);
    int getMacAddress(NetIfType_t type, uint8_t* mac);

    int scanForAp();
    int getApNum();
    const char* getSSID(uint8_t i);
    int32_t getRSSI(uint8_t i);
    uint8_t getEncrType(uint8_t i);
    uint8_t* getBSSID(uint8_t i, uint8_t* bssid);
    uint8_t getChannel(uint8_t i);
    int connectToAp(const char* ssid, const char* pwd);
    int disconnectFromAp();
    const char* getSSID();
    uint8_t* getBSSID(uint8_t* bssid);
    uint32_t getRSSI();
    uint8_t getEncrType();

    WifiStatus_t getWifiStatus() { return wifi_status; }

    int startSoftAp(const char* ssid, const char* passphrase, uint8_t channel);
    int setLowPowerMode();
    int resetLowPowerMode();

    const char* getSSID(NetIfType_t type);
    uint8_t* getBSSID(NetIfType_t type, uint8_t* bssid);
    int32_t getRSSI(NetIfType_t type);
    uint8_t getEncryptionType(NetIfType_t type);

    int setWifiMode(WifiMode_t mode);

    void lwip_task();
};

#endif
