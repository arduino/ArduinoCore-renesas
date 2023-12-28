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


#define DHCP_CHECK_NONE (0)
#define DHCP_CHECK_RENEW_FAIL (1)
#define DHCP_CHECK_RENEW_OK (2)
#define DHCP_CHECK_REBIND_FAIL (3)
#define DHCP_CHECK_REBIND_OK (4)

#define TIMED_OUT -1
#define INVALID_SERVER -2
#define TRUNCATED -3
#define INVALID_RESPONSE -4


ip_addr_t* u8_to_ip_addr(uint8_t* ipu8, ip_addr_t* ipaddr);

uint32_t ip_addr_to_u32(ip_addr_t* ipaddr);

/* Base class implements DHCP, derived class will switch it on or off */
class CNetIf {
protected:
    struct netif ni;

#ifdef LWIP_DHCP
    bool dhcp_acquired;
#endif

    // IPAddress _dnsServerAddress;

    // Driver interface pointer
    NetworkDriver *driver = nullptr;
public:
    CNetIf();
    virtual ~CNetIf();
    /*
     * The begin function is called by the user in the sketch to initialize the network interface
     * that he is planning on using in the sketch.
     */
    virtual void begin(
        const IPAddress &ip = INADDR_NONE,
        const IPAddress &nm = INADDR_NONE,
        const IPAddress &gw = INADDR_NONE);

    /*
     * This method performs interface specific tasks (if any)
     */
    virtual void task();

#ifdef LWIP_DHCP
    /* --------------
     * DHCP functions
     * -------------- */
    // starts DHCP and tries to acquire addresses, return true if request was made successfully (ususally memory issues)
    bool dhcpStart();
    // stops DHCP
    void dhcpStop();
    // tells DHCP server that the interface uses a statically provided ip address
    void dhcpNotUsed();
    // force DHCP renewal, returns false on error (ususally memory issues)
    bool dhcpRenew();
    // force DHCP release, usually called before dhcp stop (ususally memory issues)
    bool dhcpRelease();
    // tells if DHCP has acquired addresses or not
    bool isDhcpAcquired();
#endif

    virtual void setLinkUp();
    virtual void setLinkDown();

    virtual void up();
    virtual void down();

    bool isLinkUp() { return (bool)netif_is_link_up(&ni); }

    struct netif* getNi() { return &ni; }

    uint32_t getIpAdd() { return ip4_addr_get_u32(&(ni.ip_addr)); }
    uint32_t getNmAdd() { return ip4_addr_get_u32(&(ni.netmask)); }
    uint32_t getGwAdd() { return ip4_addr_get_u32(&(ni.gw)); }

    void setHostname(const char* name)
    {
        memset(hostname, 0x00, MAX_HOSTNAME_DIM);
        memcpy(hostname, name, strlen(name) < MAX_HOSTNAME_DIM ? strlen(name) : MAX_HOSTNAME_DIM);
    }


    virtual int getMacAddress(uint8_t* mac) = 0;
};

/* -------------------------------------------------------------------------- */
class CEth : public CNetIf {
    /* -------------------------------------------------------------------------- */
protected:
    /*
     * this function is used to initialize the netif structure of lwip
     */
    static err_t init(struct netif* ni) override;

    /*
     * This function is passed to lwip and used to send a buffer to the driver in order to transmit it
     */
    static err_t output(struct netif* ni, struct pbuf* p) override;
    static const char eth_ifname_prefix = 'e';
    static uint8_t eth_id;
public:
    CEth();
    virtual ~CEth();
    virtual void begin(
        const IPAddress &ip = INADDR_NONE,
        const IPAddress &nm = INADDR_NONE,
        const IPAddress &gw = INADDR_NONE) override;

    virtual void task() override;

    virtual int getMacAddress(uint8_t* mac) override {
        UNUSED(mac); // FIXME not implemented
        return 1;
    }

private:
    /*
     * This function is passed to the driver class and it is meant to
     * take a pointer to a buffer, and pass it to lwip to process it
     */
    void consume_callback(uint8_t* buffer, uint32_t len);
};

/* -------------------------------------------------------------------------- */
class CWifiStation : public CNetIf {
    /* -------------------------------------------------------------------------- */
protected:
    static const char wifistation_ifname_prefix = 'w';
    static uint8_t wifistation_id;

    /*
     * this function is used to initialize the netif structure of lwip
     */
    static err_t init(struct netif* ni) override;

    /*
     * This function is passed to lwip and used to send a buffer to the driver in order to transmit it
     */
    static err_t output(struct netif* ni, struct pbuf* p) override;

    WifiApCfg_t access_point_cfg;
public:
    CWifiStation();
    virtual ~CWifiStation();
    virtual void begin(
        const IPAddress &ip = INADDR_NONE,
        const IPAddress &nm = INADDR_NONE,
        const IPAddress &gw = INADDR_NONE) override;

    int connectToAP(const char* ssid, const char *passphrase=nullptr);
    int disconnectFromAp();
    int scanForAp();

    virtual void task() override;

    virtual int getMacAddress(uint8_t* mac) override;

    virtual const char* getSSID();
    virtual uint8_t* getBSSID(uint8_t* bssid);
    virtual int32_t getRSSI();
    virtual uint8_t getEncryptionType();
};

/* -------------------------------------------------------------------------- */
class CWifiSoftAp : public CNetIf {
    /* -------------------------------------------------------------------------- */
protected:
    static const char wifistation_ifname_prefix = 'w';
    static uint8_t wifistation_id;
    /*
     * this function is used to initialize the netif structure of lwip
     */
    static err_t init(struct netif* ni);

    /*
     * This function is passed to lwip and used to send a buffer to the driver in order to transmit it
     */
    static err_t output(struct netif* ni, struct pbuf* p);

    SoftApCfg_t soft_ap_cfg;
public:
    CWifiSoftAp();
    virtual ~CWifiSoftAp();
    virtual void begin(
        const IPAddress &ip = INADDR_NONE,
        const IPAddress &nm = INADDR_NONE,
        const IPAddress &gw = INADDR_NONE) override;
    virtual void task() override;

    virtual int getMacAddress(uint8_t* mac) override;

    virtual const char* getSSID();
    virtual uint8_t* getBSSID(uint8_t* bssid);
    virtual int32_t getRSSI();
    virtual uint8_t getEncryptionType();
};

class CLwipIf {
public:
    CLwipIf(CLwipIf const&) = delete;
    void operator=(CLwipIf const&) = delete;


    static CLwipIf& getInstance() {
        //FIXME this doesn't to seem good
        static CLwipIf instance; // this is private in case we need to synch the access to the singleton
        return instance;
    }

    // run polling tasks from all the LWIP Network Interfaces
    // this needs to be called in the loop() if we are not running it
    // with a timer
    void task();

    // Function that provides a Client of the correct kind given the protocol provided in url
    // Client* connect(std::string url);
    // void request(std::string url, std::function<void(uint8_t*, size_t)>);

    // function for setting an iface as default
    void setDefaultIface(CNetIf* iface);

    // functions that handle DNS resolution
    // DNS servers are also set by dhcp
#if LWIP_DNS
    // add a dns server, priority set to 0 means it is the first being queryed, -1 means the last
    uint8_t addDnsServer(const IPAddress& aDNSServer, int8_t priority=-1);
    void clearDnsServers();

    // DNS resolution works with a callback if the resolution doesn't return immediately
    int getHostByName(const char* aHostname, IPAddress& aResult, bool execute_task=false); // blocking call
    int getHostByName(const char* aHostname, std::function<void(const IPAddress&)> cbk); // callback version
#endif
private:
    CLwipIf();

    // TODO define a Timer for calling tasks

    std::vector<CNetIf*> ifaces;

    virtual void add_iface(CNetIf* iface);
    // virtual void del_iface(CNetIf* iface);

    // lwip stores the netif in a linked list called: netif_list

    friend class CNetIf;

#ifdef NETWORKSTACK_USE_TIMER
    FspTimer timer;
#endif
};

#endif
