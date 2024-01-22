#pragma once

#define UNUSED(x) (void)(x)

#define USE_LWIP_AS_LIBRARY
#include "Arduino.h"

#include "CCtrlWrapper.h"
#include "CEspControl.h"
#include "IPAddress.h"
#include "EthernetDriver.h"
#include <string>
#include <interface.h>

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

enum LinkStatus {
    Unknown,
    LinkON,
    LinkOFF
};

enum EthernetHardwareStatus {
    EthernetNoHardware,
    EthernetLwip = 7
};

#define MAX_CLIENT MEMP_NUM_TCP_PCB
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

class CLwipIf;

/* Base class implements DHCP, derived class will switch it on or off */
class CNetIf: public NetworkInterface {
public:
    CNetIf(NetworkDriver *driver=nullptr);
    virtual ~CNetIf() {}
    /*
     * The begin function is called by the user in the sketch to initialize the network interface
     * that he is planning on using in the sketch.
     */
    virtual int begin(
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

    inline int disconnect() { this->down(); return 0; }

    inline LinkStatus linkStatus() { return netif_is_link_up(&ni) ? LinkON : LinkOFF; }

    bool isLinkUp() { return (bool)netif_is_link_up(&ni); }

    struct netif* getNi() { return &ni; }

    uint32_t getIpAdd() { return ip4_addr_get_u32(&(ni.ip_addr)); }
    uint32_t getNmAdd() { return ip4_addr_get_u32(&(ni.netmask)); }
    uint32_t getGwAdd() { return ip4_addr_get_u32(&(ni.gw)); }

    // FIXME when dhcp has not provided an ip address yet return IPADDR_NONE
    IPAddress localIP()     { return IPAddress(this->getIpAdd()); }
    IPAddress subnetMask()  { return IPAddress(this->getNmAdd()); }
    IPAddress gatewayIP()   { return IPAddress(this->getGwAdd()); }
    IPAddress dnsServerIP() { /* FIXME understand where dns should be managed */}

    void config(IPAddress _ip, IPAddress _gw, IPAddress _nm);

    virtual int getMacAddress(uint8_t* mac) = 0;
    virtual int setMacAddress(uint8_t* mac) = 0;

    friend CLwipIf;
protected:
    struct netif ni;

#ifdef LWIP_DHCP
    volatile bool dhcp_acquired;
#endif

    /*
     * this function is used to initialize the netif structure of lwip
     */
    virtual err_t init(struct netif* ni) = 0;

    /*
     * This function is passed to lwip and used to send a buffer to the driver in order to transmit it
     */
    virtual err_t output(struct netif* ni, struct pbuf* p) = 0;

    // the following functions are used to call init and output from lwip in the object context in the C code
    friend err_t _netif_init(struct netif* ni);
    friend err_t _netif_output(struct netif* ni, struct pbuf* p);

    // IPAddress _dnsServerAddress;

    // Driver interface pointer
    NetworkDriver *driver = nullptr;

    void linkDownCallback();
    void linkUpCallback();
};

class CEth : public CNetIf {
public:
    CEth(NetworkDriver *driver=nullptr);
    // virtual ~CEth();
    virtual int begin(
        const IPAddress &ip = INADDR_NONE,
        const IPAddress &nm = INADDR_NONE,
        const IPAddress &gw = INADDR_NONE,
        const IPAddress &dns = INADDR_NONE);

    // The following are overloaded begin methods kept for retrocompatibility with other Arduino cores
    // Initialise the Ethernet shield to use the provided MAC address and gain the rest of the
    // configuration through DHCP.
    // Returns 0 if the DHCP configuration failed, and 1 if it succeeded
    virtual int begin(
        uint8_t *mac_address,
        const IPAddress &local_ip           = INADDR_NONE,
        const IPAddress &dns_server         = INADDR_NONE,
        const IPAddress &gateway            = INADDR_NONE,
        const IPAddress &subnet             = INADDR_NONE,
        const unsigned long timeout         = 60000,
        const unsigned long responseTimeout = 4000);

    virtual int begin(
        uint8_t *mac_address,
        const unsigned long timeout         = 60000,
        const unsigned long responseTimeout = 4000);


    virtual int getMacAddress(uint8_t* mac) override {
        UNUSED(mac); // FIXME not implemented
        return 1;
    }

    virtual int setMacAddress(uint8_t* mac) override {
        UNUSED(mac); // FIXME not implemented
        return 1;
    }


    int maintain() {} // Deprecated method for retrocompatibility
    void schedule(void) {} // Deprecated method for retrocompatibility

    inline EthernetHardwareStatus hardwareStatus() { return EthernetLwip; }
protected:
    /*
     * this function is used to initialize the netif structure of lwip
     */
    err_t init(struct netif* ni) override;

    /*
     * This function is passed to lwip and used to send a buffer to the driver in order to transmit it
     */
    err_t output(struct netif* ni, struct pbuf* p) override;

    static const char eth_ifname[];
private:
    /*
     * This function is passed to the driver class and it is meant to
     * take a pointer to a buffer, and pass it to lwip to process it
     */
    void consume_callback(uint8_t* buffer, uint32_t len);
};

class CWifiStation : public CNetIf {
public:
    CWifiStation();
    virtual ~CWifiStation();
    virtual int begin(
        const IPAddress &ip = INADDR_NONE,
        const IPAddress &nm = INADDR_NONE,
        const IPAddress &gw = INADDR_NONE) override;

    int connectToAP(const char* ssid, const char *passphrase=nullptr);
    int disconnectFromAp();
    int scanForAp();

    virtual void task() override;

    virtual int getMacAddress(uint8_t* mac) override {
        // FIXME not implemented
    }

    virtual int setMacAddress(uint8_t* mac) override {
        UNUSED(mac); // FIXME not implemented
        return 1;
    }

    virtual const char* getSSID();
    virtual uint8_t* getBSSID(uint8_t* bssid);
    virtual int32_t getRSSI();
    virtual uint8_t getEncryptionType();
    virtual uint8_t getChannel();

    const char* getSSID(uint8_t i);
    int32_t getRSSI(uint8_t i);
    uint8_t getEncrType(uint8_t i);
    uint8_t* getBSSID(uint8_t i, uint8_t* bssid);
    uint8_t getChannel(uint8_t i);

    int setLowPowerMode();
    int resetLowPowerMode();

    inline WifiStatus_t status() {
        return wifi_status;
    }
protected:
    static const char wifistation_ifname[];

    /*
     * this function is used to initialize the netif structure of lwip
     */
    err_t init(struct netif* ni) override;

    /*
     * This function is passed to lwip and used to send a buffer to the driver in order to transmit it
     */
    err_t output(struct netif* ni, struct pbuf* p) override;

private:
    std::vector<AccessPoint_t> access_points;
    WifiApCfg_t access_point_cfg;
    bool hw_init; // TODO this should be moved to the wifi driver class
    WifiStatus_t wifi_status = WL_IDLE_STATUS; // TODO this should be moved to the wifi driver class
};

class CWifiSoftAp : public CNetIf {
public:
    CWifiSoftAp();
    virtual ~CWifiSoftAp();
    virtual int begin(
        const IPAddress &ip = INADDR_NONE,
        const IPAddress &nm = INADDR_NONE,
        const IPAddress &gw = INADDR_NONE) override;
    virtual void task() override;

    int startSoftAp(const char* ssid, const char* passphrase=nullptr, uint8_t channel=0);
    int stopSoftAp();

    virtual int getMacAddress(uint8_t* mac) override {
        // FIXME not implemented
    }

    virtual int setMacAddress(uint8_t* mac) override {
        UNUSED(mac); // FIXME not implemented
        return 1;
    }

    virtual const char* getSSID();
    virtual uint8_t* getBSSID(uint8_t* bssid);
    virtual uint8_t getEncryptionType();
    virtual uint8_t getChannel();

    int setLowPowerMode();
    int resetLowPowerMode();
protected:
    static const char softap_ifname[];
    /*
     * this function is used to initialize the netif structure of lwip
     */
    err_t init(struct netif* ni);

    /*
     * This function is passed to lwip and used to send a buffer to the driver in order to transmit it
     */
    err_t output(struct netif* ni, struct pbuf* p);

private:
    std::vector<AccessPoint_t> access_points;
    SoftApCfg_t soft_ap_cfg;
    bool hw_init; // TODO this should be moved to the wifi driver class
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
    // TODO get iface method

    // functions that handle DNS resolution
    // DNS servers are also set by dhcp
#if LWIP_DNS
    // add a dns server, priority set to 0 means it is the first being queried, -1 means the last
    uint8_t addDnsServer(const IPAddress& aDNSServer, int8_t priority=-1);
    void clearDnsServers();

    IPAddress getDns(int n);

    // DNS resolution works with a callback if the resolution doesn't return immediately
    int getHostByName(const char* aHostname, IPAddress& aResult, bool execute_task=false); // blocking call
    int getHostByName(const char* aHostname, std::function<void(const IPAddress&)> cbk); // callback version
#endif
private:
    CLwipIf();
    ~CLwipIf();

    // TODO define a Timer for calling tasks

    std::vector<CNetIf*> ifaces;

    virtual void add_iface(CNetIf* iface);
    // virtual void del_iface(CNetIf* iface);

    // lwip stores the netif in a linked list called: netif_list

    friend class CNetIf;
    friend class CWifiSoftAp;
    friend class CWifiStation;

#ifdef LWIP_USE_TIMER
    FspTimer timer;

    inline void sync_timer() {
        timer.disable_overflow_irq();
        this->task();
    }

    inline void enable_timer() {
        timer.enable_overflow_irq();
    }
#else // LWIP_USE_TIMER
    inline void sync_timer() {
        this->task();
    }

    inline void enable_timer() { }
#endif // LWIP_USE_TIMER
};

extern CEth Ethernet;
extern CWifiStation WiFiStation;
extern CWifiSoftAp WiFiSoftAP;
