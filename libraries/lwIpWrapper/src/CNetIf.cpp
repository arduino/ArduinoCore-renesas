#include "CNetIf.h"
#include <functional>

// TODO make better documentation on how this works
// TODO hostname should be defined at network stack level and shared among ifaces
// TODO buffer management (allocation/deallocation/trim/etc.) should be properly handled by a wrapper class and be transparent wrt the user
// TODO the device could be moving and as a consequence it may be nice to rescan APs to get one with the best rssi
// TODO implement setLowPowerMode and resetLowPowerMode in WIFI driver
// TODO implement stop softAP and include it in the destructor of the class
// TODO split netif definition in different files
// TODO implement WIFINetworkDriver that is then being used by both Wifi station and softAP. This will allow to use both at the same time

err_t _netif_init(struct netif* ni);
err_t _netif_output(struct netif* ni, struct pbuf* p);

#if LWIP_DNS
static void _getHostByNameCBK(const char *name, const ip_addr_t *ipaddr, void *callback_arg);
#endif // LWIP_DNS

// Custom Pbuf definition used to handle RX zero copy
// TODO Move this in a separate file (understand if it is required)
typedef struct zerocopy_pbuf {
    struct pbuf_custom p;
    uint8_t* buffer;
    uint32_t size;
    void(*buffer_free)(void*);
} zerocopy_pbuf_t;

static void zerocopy_pbuf_mem_free(struct pbuf *p) {
    // SYS_ARCH_DECL_PROTECT(zerocopy_pbuf_free);
    zerocopy_pbuf_t* zcpbuf = (zerocopy_pbuf_t*) p;

    // arduino::lock();
    // SYS_ARCH_PROTECT(zerocopy_pbuf_free);

    // FIXME pbufs may be allocated in a different memory pool, deallocate them accordingly
    zcpbuf->buffer_free(zcpbuf->buffer);
    zcpbuf->buffer = nullptr;
    mem_free(zcpbuf); // TODO understand if pbuf_free deletes the pbuf
    // SYS_ARCH_UNPROTECT(zerocopy_pbuf_free);

    // arduino::unlock();
}

static inline zerocopy_pbuf_t* get_zerocopy_pbuf(uint8_t *buffer, uint32_t size, void(*buffer_free)(void*) = mem_free) {
    zerocopy_pbuf_t* p = (zerocopy_pbuf_t*)mem_malloc(sizeof(zerocopy_pbuf_t));
    p->buffer = buffer;
    p->size = size;
    p->p.custom_free_function = zerocopy_pbuf_mem_free;
    p->buffer_free = buffer_free;
    return p;
}

static uint8_t Encr2wl_enc(int enc) {
    if (enc == WIFI_AUTH_OPEN) {
        return ENC_TYPE_NONE;
    } else if (enc == WIFI_AUTH_WEP) {
        return ENC_TYPE_WEP;
    } else if (enc == WIFI_AUTH_WPA_PSK) {
        return ENC_TYPE_WPA;
    } else if (enc == WIFI_AUTH_WPA2_PSK) {
        return ENC_TYPE_WPA2;
    } else if (enc == WIFI_AUTH_WPA_WPA2_PSK) {
        return ENC_TYPE_WPA2;
    } else if (enc == WIFI_AUTH_WPA2_ENTERPRISE) {
        return ENC_TYPE_WPA2_ENTERPRISE;
    } else if (enc == WIFI_AUTH_WPA3_PSK) {
        return ENC_TYPE_WPA3;
    } else if (enc == WIFI_AUTH_WPA2_WPA3_PSK) {
        return ENC_TYPE_WPA3;
    } else {
        return ENC_TYPE_UNKNOWN;
    }
}

CLwipIf::CLwipIf() {

    /* Initialize lwIP stack, singletone implementation guarantees that lwip is
       initialized just once  */
    lwip_init();

#ifdef NETWORKSTACK_USE_TIMER
    uint8_t type = 8;
    int8_t ch = FspTimer::get_available_timer(type);

    if (ch < 0) {
        ch = FspTimer::get_available_timer(type, true);
    }

    /*
     * NOTE Timer and buffer size
     * The frequency for the timer highly influences the memory requirements for the desired transfer speed
     * You can calculate the buffer size required to achieve that performance from the following formula:
     * buffer_size[byte] = Speed[bit/s] * timer_frequency[Hz]^-1 / 8
     *
     * In the case of portenta C33, the maximum speed achievable was measured with
     * iperf2 tool (provided by lwip) and can reach up to 12Mbit/s.
     * Further improvements can be made, but if we desire to reach that speed the buffer size
     * and the timer frequency should be designed accordingly.
     * buffer = 12 * 10^6 bit/s * (100Hz)^-1 / 8 = 15000 Byte = 15KB
     *
     * Since this is a constrained environment we could accept performance loss and
     * delegate lwip to handle lost packets.
     */
    timer.begin(TIMER_MODE_PERIODIC, type, ch, 100.0, 0, timer_cb, this); // TODO make the user decide how to handle these parameters
#endif
}

void CLwipIf::task() {
    for(CNetIf* iface: this->ifaces) { // FIXME is this affecting performances?
        iface->task();
    }

    arduino::lock();
    sys_check_timeouts();
    arduino::unlock();
}

void CLwipIf::setDefaultIface(CNetif* iface) {
    // TODO check if the iface is in the vector

    netif_set_default(&iface->ni);
}

void CLwipIf::add_iface(CNetif* iface) {
    // if it is the first interface set it as the default route
    if(this->ifaces.empty()) {
        netif_set_default(&iface->ni); // TODO let the user decide which is the default one

#ifdef NETWORKSTACK_USE_TIMER
        timer.setup_overflow_irq();
        timer.open();
        timer.start();
#endif
    }

    // add the interface if not already present in the vector
    this->ifaces.push_back(iface);
}

CLwipIf::~CLwipIf() {
    // TODO free iface array
}


int CLwipIf::setWifiMode(WifiMode_t mode) {
    // TODO adapt this
    // CLwipIf::getInstance().startSyncRequest();
    // int rv = CEspControl::getInstance().setWifiMode(mode);
    // CLwipIf::getInstance().restartAsyncRequest();
    // return rv;
}

/* ***************************************************************************
 *                               DNS related functions
 * ****************************************************************************/

#if LWIP_DNS

struct dns_callback {
    std::function<void(const IPAddress&)> cbk;
};

static void _getHostByNameCBK(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    dns_callback* cbk = (dns_callback*)callback_arg;

    cbk->cbk(toArduinoIP(ipaddr));

    delete cbk;
}

// add a dns server, priority set to 0 means it is the first being queryed, -1 means the last
uint8_t CLwipIf::addDnsServer(const IPAddress& aDNSServer, int8_t priority) {
    // TODO test this function with all the possible cases of dns server position
    if(priority == -1) {
        // lwip has an array for dns servers that can be iterated with dns_getserver(num)
        // when a dns server is set to any value, it means it is the last

        for(priority=0;
            priority<DNS_MAX_SERVERS && !ip_addr_isany_val(*dns_getserver(priority));
            priority++) {}
    }

    if(priority >= DNS_MAX_SERVERS) {
        // unable to add another dns server, because priority is more than the dns server available space
        return -1;
    }

    ip_addr_t ip = fromArduinoIP(aDNSServer);

    dns_setserver(priority, &ip);
}

void CLwipIf::clearDnsServers() {
    for(uint8_t i=0; i<DNS_MAX_SERVERS; i++) {
        dns_setserver(i, IP_ANY_TYPE);
    }
}

// DNS resolution works with a callback if the resolution doesn't return immediately
int CLwipIf::getHostByName(const char* aHostname, IPAddress& aResult, bool execute_task) {
    /* this has to be a blocking call but we need to understand how to handle wait time
     * - we can have issues when running concurrently from different contextes,
     *   meaning that issues may arise if we run task() method of this class from an interrupt
     *   context and the "userspace".
     * - this function is expected to be called in the application layer, while the lwip stack is
     *   being run in an interrupt context, otherwise this call won't work because it will block
     *   everything
     * - this function shouldn't be called when lwip is run in the same context as the application
     */
    volatile bool completed = false;

    uint8_t res = this->getHostByName(aHostname, [&aResult, &completed](const IPAddress& ip){
        aResult = ip;
        completed = true;
    });

    while(res == 1 && !completed) { // DNS timeouts seems to be handled by lwip, no need to put one here
        delay(1);
        if(execute_task) {
            this->task();
        }
    }

    return res == 1 ? 0 : res;
}

// TODO instead of returning int return an enum value
int CLwipIf::getHostByName(const char* aHostname, std::function<void(const IPAddress&)> cbk) {
    ip_addr_t addr; // TODO understand if this needs to be in the heap
    uint8_t res = 0;

    dns_callback* dns_cbk = new dns_callback;
    dns_cbk->cbk = cbk;
    err_t err = dns_gethostbyname(aHostname, &addr, _getHostByNameCBK, dns_cbk);

    switch(err) {
    case ERR_OK:
        // the address was already present in the local cache
        cbk(toArduinoIP(&addr));

        delete dns_cbk;
        break;
    case ERR_INPROGRESS:
        // the address is not present in the local cache, return and wait for the address resolution to complete
        res = 1;
        break;
    case ERR_ARG: // there are issues in the arguments passed
    default:
        delete dns_cbk;
        res = -1;
    }

    return res;
}
#endif

/* ##########################################################################
 *                      BASE NETWORK INTERFACE CLASS
 * ########################################################################## */

/* -------------------------------------------------------------------------- */
err_t CWifiStation::init(struct netif* _ni)
{
    /* -------------------------------------------------------------------------- */
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    _ni->hostname = "C33-WifiSta";
#endif /* LWIP_NETIF_HOSTNAME */

    _ni->name[0] = WST_IFNAME0;
    _ni->name[1] = WST_IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    _ni->output = etharp_output;
    _ni->linkoutput = CWifiStation::output;

    /* maximum transfer unit */
    _ni->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    _ni->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    /* set MAC hardware address */
    _ni->hwaddr_len = CLwipIf::getInstance().getMacAddress(NI_WIFI_STATION, _ni->hwaddr);

    return ERR_OK;
}

/* -------------------------------------------------------------------------- */
err_t CWifiSoftAp::output(struct netif* _ni, struct pbuf* p)
{
    /* -------------------------------------------------------------------------- */
    (void)_ni;
    err_t errval = ERR_IF;

    uint8_t* buf = new uint8_t[p->tot_len];

    if (buf != nullptr) {
        uint16_t bytes_actually_copied = pbuf_copy_partial(p, buf, p->tot_len, 0);
        if (bytes_actually_copied > 0) {
            int ifn = 0;
            if (CLwipIf::net_ifs[NI_WIFI_SOFTAP] != nullptr) {
                ifn = CLwipIf::net_ifs[NI_WIFI_SOFTAP]->getId();
            }

            if (CEspControl::getInstance().sendBuffer(ESP_AP_IF, ifn, buf, bytes_actually_copied) == ESP_CONTROL_OK) {
                errval = ERR_OK;
            }
        }
        delete[] buf;
    }

    return errval;
}

/* -------------------------------------------------------------------------- */
err_t CWifiSoftAp::init(struct netif* _ni)
{
    /* -------------------------------------------------------------------------- */
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    _ni->hostname = "C33-WifiSta";
#endif /* LWIP_NETIF_HOSTNAME */

    _ni->name[0] = WSA_IFNAME0;
    _ni->name[1] = WSA_IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    _ni->output = etharp_output;
    _ni->linkoutput = CWifiSoftAp::output;

    /* maximum transfer unit */
    _ni->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    _ni->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    /* set MAC hardware address */
    _ni->hwaddr_len = CLwipIf::getInstance().getMacAddress(NI_WIFI_SOFTAP, _ni->hwaddr);

    return ERR_OK;
}

/* -------------------------------------------------------------------------- */
bool CLwipIf::setMacAddress(NetIfType_t type, uint8_t* mac)
{
    /* -------------------------------------------------------------------------- */

    CLwipIf::getInstance().startSyncRequest();
    WifiMac_t MAC;
    CNetUtilities::macArray2macStr(MAC.mac, mac);

    if (type == NI_WIFI_STATION) {
        MAC.mode = WIFI_MODE_STA;
        if (CEspControl::getInstance().setWifiMacAddress(MAC) != ESP_CONTROL_OK) {
            return false;
        }

    } else if (type == NI_WIFI_SOFTAP) {
        MAC.mode = WIFI_MODE_AP;
        if (CEspControl::getInstance().setWifiMacAddress(MAC) != ESP_CONTROL_OK) {
            return false;
        }
    } else {
        eth_set_mac_address(mac);
    }

    CLwipIf::getInstance().restartAsyncRequest();
    return true;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::getMacAddress(NetIfType_t type, uint8_t* mac)
{
    /* -------------------------------------------------------------------------- */
    int rv = 0;
    WifiMac_t MAC;

    CLwipIf::getInstance().startSyncRequest();

    if (type == NI_WIFI_STATION) {
        MAC.mode = WIFI_MODE_STA;
        if (CEspControl::getInstance().getWifiMacAddress(MAC) == ESP_CONTROL_OK) {
            CNetUtilities::macStr2macArray(mac, MAC.mac);
            rv = MAC_ADDRESS_DIM;
        }
    } else if (type == NI_WIFI_SOFTAP) {
        MAC.mode = WIFI_MODE_AP;
        if (CEspControl::getInstance().getWifiMacAddress(MAC) == ESP_CONTROL_OK) {
            CNetUtilities::macStr2macArray(mac, MAC.mac);
            rv = MAC_ADDRESS_DIM;
        }
    } else {
        eth_get_mac_address(mac);
        rv = MAC_ADDRESS_DIM;
    }

    CLwipIf::getInstance().restartAsyncRequest();
    return rv;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::scanForAp()
{
    /* -------------------------------------------------------------------------- */
    access_points.clear();
    CLwipIf::getInstance().startSyncRequest();
    int res = CEspControl::getInstance().getAccessPointScanList(access_points);
    CLwipIf::getInstance().restartAsyncRequest();
    if (res == ESP_CONTROL_OK) {
        wifi_status = WL_SCAN_COMPLETED;
    } else {
        wifi_status = WL_NO_SSID_AVAIL;
    }
    return res;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::getApNum() { return access_points.size(); }
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
const char* CLwipIf::getSSID(uint8_t i)
{
    /* -------------------------------------------------------------------------- */
    if (access_points.size() > 0 && i < access_points.size()) {
        return (const char*)access_points[i].ssid;
    }
    return nullptr;
}

/* -------------------------------------------------------------------------- */
int32_t CLwipIf::getRSSI(uint8_t i)
{
    /* -------------------------------------------------------------------------- */
    if (access_points.size() > 0 && i < access_points.size()) {
        return (int32_t)access_points[i].rssi;
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
uint8_t CLwipIf::getEncrType(uint8_t i)
{
    /* -------------------------------------------------------------------------- */
    if (access_points.size() > 0 && i < access_points.size()) {
        return Encr2wl_enc(access_points[i].encryption_mode);
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
uint8_t* CLwipIf::getBSSID(uint8_t i, uint8_t* bssid)
{
    /* -------------------------------------------------------------------------- */
    if (access_points.size() > 0 && i < access_points.size()) {
        CNetUtilities::macStr2macArray(bssid, (const char*)access_points[i].bssid);
        return bssid;
    }
    return nullptr;
}

/* -------------------------------------------------------------------------- */
uint8_t CLwipIf::getChannel(uint8_t i)
{
    /* -------------------------------------------------------------------------- */
    if (access_points.size() > 0 && i < access_points.size()) {
        return (uint8_t)access_points[i].channel;
    }
    return 0;
}
/* -------------------------------------------------------------------------- */
int CLwipIf::connectToAp(const char* ssid, const char* pwd)
{
    /* -------------------------------------------------------------------------- */
    WifiApCfg_t ap;
    int rv = ESP_CONTROL_CTRL_ERROR;
    bool found = false;
    uint8_t index = 0;
    for (uint8_t i = 0; i < access_points.size() && !found; i++) {
        if (strcmp(ssid, (const char*)access_points[i].ssid) == 0) {
            found = true;
            index = i;
        }
    }

    if (found) {
        memset(ap.ssid, 0x00, SSID_LENGTH);
        memcpy(ap.ssid, access_points[index].ssid, SSID_LENGTH);
        memset(ap.pwd, 0x00, PASSWORD_LENGTH);
        if (pwd != nullptr) {
            memcpy(ap.pwd, pwd, (strlen(pwd) < PASSWORD_LENGTH) ? strlen(pwd) : PASSWORD_LENGTH);
        }
        memset(ap.bssid, 0x00, BSSID_LENGTH);
        memcpy(ap.bssid, access_points[index].bssid, BSSID_LENGTH);

        CLwipIf::getInstance().startSyncRequest();
        if (CEspControl::getInstance().connectAccessPoint(ap) == ESP_CONTROL_OK) {
            CLwipIf::connected_to_access_point = true;
            wifi_status = WL_CONNECTED;
            CEspControl::getInstance().getAccessPointConfig(access_point_cfg);

            rv = ESP_CONTROL_OK;
            /* when we get the connection to access point we are sure we are STATION
               and we are connected */
            if (CLwipIf::net_ifs[NI_WIFI_STATION] != nullptr) {
                CLwipIf::net_ifs[NI_WIFI_STATION]->setLinkUp();
            }

      }
      else {

         wifi_status = WL_CONNECT_FAILED;
         CLwipIf::connected_to_access_point = false;
      }

      CLwipIf::getInstance().restartAsyncRequest();
   }
   else {
      /* in case SSID was available scan again for access point 
         (perhaps a wifi hostpoint has been added) */
      CLwipIf::getInstance().scanForAp();
      //Serial.println("SSID not found in the list of available AP");
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
const char* CLwipIf::getSSID()
{
    /* -------------------------------------------------------------------------- */
    return (const char*)access_point_cfg.ssid;
}

/* -------------------------------------------------------------------------- */
uint8_t* CLwipIf::getBSSID(uint8_t* bssid)
{
    /* -------------------------------------------------------------------------- */
    CNetUtilities::macStr2macArray(bssid, (const char*)access_point_cfg.bssid);
    return bssid;
}

/* -------------------------------------------------------------------------- */
uint32_t CLwipIf::getRSSI()
{
    /* -------------------------------------------------------------------------- */
    return (uint32_t)access_point_cfg.rssi;
}

/* -------------------------------------------------------------------------- */
uint8_t CLwipIf::getEncrType()
{
    /* -------------------------------------------------------------------------- */
    return Encr2wl_enc(access_point_cfg.encryption_mode);
}

/* -------------------------------------------------------------------------- */
int CLwipIf::disconnectFromAp()
{
    /* -------------------------------------------------------------------------- */
    wifi_status = WL_DISCONNECTED;
    CLwipIf::getInstance().startSyncRequest();
    int rv = CEspControl::getInstance().disconnectAccessPoint();
    CLwipIf::getInstance().restartAsyncRequest();
    wifi_status = WL_DISCONNECTED;
    if (net_ifs[NI_WIFI_STATION] != nullptr) {
        net_ifs[NI_WIFI_STATION]->setLinkDown();
    }
    return rv;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::startSoftAp(const char* ssid, const char* passphrase, uint8_t channel)
{
    /* -------------------------------------------------------------------------- */
    CLwipIf::getInstance().startSyncRequest();
    SoftApCfg_t cfg;
    memset(cfg.ssid, 0x00, SSID_LENGTH);
    memcpy(cfg.ssid, ssid, (strlen(ssid) < SSID_LENGTH) ? strlen(ssid) : SSID_LENGTH);
    memset(cfg.pwd, 0x00, PASSWORD_LENGTH);
    if (passphrase == nullptr) {
        memcpy(cfg.pwd, "arduinocc", strlen("arduinocc"));
    } else {
        memcpy(cfg.pwd, passphrase, strlen(passphrase) < PASSWORD_LENGTH ? strlen(passphrase) : PASSWORD_LENGTH);
    }
    channel = (channel == 0) ? 1 : channel;
    cfg.channel = (channel > MAX_CHNL_NO) ? MAX_CHNL_NO : channel;
    cfg.max_connections = MAX_SOFAT_CONNECTION_DEF;
    cfg.encryption_mode = WIFI_AUTH_WPA_WPA2_PSK;
    cfg.bandwidth = WIFI_BW_HT40;
    cfg.ssid_hidden = false;

    int rv = CEspControl::getInstance().startSoftAccessPoint(cfg);
    if (rv == ESP_CONTROL_OK) {
        CEspControl::getInstance().getSoftAccessPointConfig(soft_ap_cfg);
        wifi_status = WL_AP_LISTENING;
        if (net_ifs[NI_WIFI_SOFTAP] != nullptr) {
            net_ifs[NI_WIFI_SOFTAP]->setLinkUp();
        }
    } else {
        wifi_status = WL_AP_FAILED;
    }
    CLwipIf::getInstance().restartAsyncRequest();

    return rv;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::setLowPowerMode()
{
    /* -------------------------------------------------------------------------- */
    CLwipIf::getInstance().startSyncRequest();
    int rv = CEspControl::getInstance().setPowerSaveMode(1);
    CLwipIf::getInstance().restartAsyncRequest();
    return rv;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::resetLowPowerMode()
{
    /* -------------------------------------------------------------------------- */
    CLwipIf::getInstance().startSyncRequest();
    int rv = CEspControl::getInstance().setPowerSaveMode(0);
    CLwipIf::getInstance().restartAsyncRequest();
    return rv;
}

#ifdef LWIP_USE_TIMER
/* -------------------------------------------------------------------------- */
void CLwipIf::timer_cb(timer_callback_args_t *arg) {
/*  -------------------------------------------------------------------------- */   
  (void)arg;
  CLwipIf::getInstance().lwip_task();
}
#endif

/* ***************************************************************************
 *                               DNS related functions
 * ****************************************************************************/

/* -------------------------------------------------------------------------- */
void CLwipIf::dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
/* -------------------------------------------------------------------------- */   
  (void)name;
  if (ipaddr != NULL) {
    *((uint32_t *)callback_arg) = ip4_addr_get_u32(ipaddr);
  } else {
    *((uint32_t *)callback_arg) = 0;
  }
}

/* -------------------------------------------------------------------------- */
int8_t CLwipIf::get_ip_address_from_hostname(const char* hostname, uint32_t* ipaddr)
{
    /* -------------------------------------------------------------------------- */
    ip_addr_t iphost;
    err_t err;
    unsigned long dns_request_sent = 0;
    int8_t ret = 0;

    *ipaddr = 0;
    err = dns_gethostbyname(hostname, &iphost, &dns_callback, ipaddr);

    switch (err) {
    case ERR_OK:
        *ipaddr = ip4_addr_get_u32(&iphost);
        ret = 1;
        break;

    case ERR_INPROGRESS:
        dns_request_sent = millis();
        while (*ipaddr == 0) {
            lwip_task();
            if ((millis() - dns_request_sent) >= TIMEOUT_DNS_REQUEST) {
                ret = -1;
                break;
            }
        }

        if (ret == 0) {
            if (*ipaddr == 0) {
                ret = -2;
            } else {
                ret = 1;
            }
        }
        break;

    case ERR_ARG:
        ret = -4;
        break;

    default:
        ret = -4;
        break;
    }

    return ret;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::getHostByName(const char* aHostname, IPAddress& aResult)
{
    /* -------------------------------------------------------------------------- */
    int ret = 0;
    uint32_t ipResult = 0;

    // See if it's a numeric IP address
    if (inet2aton(aHostname, aResult)) {
        // It is, our work here is done
        return 1;
    }

    if (getDns(0) == IPAddress(0, 0, 0, 0)) {
        return INVALID_SERVER;
    }

#if LWIP_DNS
    ret = get_ip_address_from_hostname(aHostname, &ipResult);
    aResult = IPAddress(ipResult);
#endif
    return ret;
}

/* -------------------------------------------------------------------------- */
int CLwipIf::inet2aton(const char* address, IPAddress& result)
{
    /* -------------------------------------------------------------------------- */
    uint16_t acc = 0; // Accumulator
    uint8_t dots = 0;

    if (address == NULL) {
        return 0;
    }

    while (*address) {
        char c = *address++;
        if (c >= '0' && c <= '9') {
            acc = acc * 10 + (c - '0');
            if (acc > 255) {
                // Value out of [0..255] range
                return 0;
            }
        } else if (c == '.') {
            if (dots == 3) {
                // Too much dots (there must be 3 dots)
                return 0;
            }
            result[dots++] = acc;
            acc = 0;
        } else {
            // Invalid char
            return 0;
        }
    }

    if (dots != 3) {
        // Too few dots (there must be 3 dots)
        return 0;
    }
    result[3] = acc;
    return 1;
}

/* -------------------------------------------------------------------------- */
void CLwipIf::beginDns(IPAddress aDNSServer)
{
    /* -------------------------------------------------------------------------- */
    addDns(aDNSServer);
}



/* -------------------------------------------------------------------------- */
void CLwipIf::addDns(IPAddress aDNSServer)
{
    /* -------------------------------------------------------------------------- */
#if LWIP_DNS
    ip_addr_t ip;
    dns_num++;
    /* DNS initialized by DHCP when call dhcp_start() */
    bool dhcp_started = false;

    for (int i = 0; i < NETWORK_INTERFACES_MAX_NUM; i++) {
        if (net_ifs[i] != nullptr) {
            if (net_ifs[i]->DhcpIsStarted()) {
                dhcp_started = true;
                break;
            }
        }
    }

    if (!dhcp_started) {
        dns_init();
        IP_ADDR4(&ip, aDNSServer[0], aDNSServer[1], aDNSServer[2], aDNSServer[3]);
        dns_setserver(dns_num, &ip);
    }
#endif
}

/* -------------------------------------------------------------------------- */
IPAddress CLwipIf::getDns(int _num)
{
    /* -------------------------------------------------------------------------- */
#if LWIP_DNS
    const ip_addr_t* tmp = dns_getserver(_num);
    return IPAddress(ip4_addr_get_u32(tmp));
#else
    IPAddress(0, 0, 0, 0);
#endif
}




/* -------------------------------------------------------------------------- */
const char* CLwipIf::getSSID(NetIfType_t type)
{
    /* -------------------------------------------------------------------------- */
    if (type == NI_WIFI_STATION) {
        return (char*)access_point_cfg.ssid;
    } else if (type == NI_WIFI_SOFTAP) {
        return (char*)soft_ap_cfg.ssid;
    } else {
        return nullptr;
    }
}

/* -------------------------------------------------------------------------- */
uint8_t* CLwipIf::getBSSID(NetIfType_t type, uint8_t* bssid)
{
    /* -------------------------------------------------------------------------- */
    if (type == NI_WIFI_STATION) {
        CNetUtilities::macStr2macArray(bssid, (const char*)access_point_cfg.bssid);
        return bssid;
    } else if (type == NI_WIFI_SOFTAP) {
        CNetUtilities::macStr2macArray(bssid, (const char*)soft_ap_cfg.out_mac);
        return bssid;
    } else {
        return nullptr;
    }
}

/* -------------------------------------------------------------------------- */
int32_t CLwipIf::getRSSI(NetIfType_t type)
{
    /* -------------------------------------------------------------------------- */
    if (type == NI_WIFI_STATION) {
        return access_point_cfg.rssi;
    } else {
        return 0;
    }
}

/* -------------------------------------------------------------------------- */
uint8_t CLwipIf::getEncryptionType(NetIfType_t type)
{
    /* -------------------------------------------------------------------------- */
    if (type == NI_WIFI_STATION) {
        return Encr2wl_enc(access_point_cfg.encryption_mode);
    } else if (type == NI_WIFI_SOFTAP) {
        return Encr2wl_enc((uint8_t)soft_ap_cfg.encryption_mode);
    } else {
        return ENC_TYPE_UNKNOWN;
    }
}

/* ########################################################################## */
/*                      BASE NETWORK INTERFACE CLASS                          */
/* ########################################################################## */

/* -------------------------------------------------------------------------- */
CNetIf::CNetIf()
    : dhcp_timeout(30000)
    , dhcp_started(false)
    , dhcp_acquired(false)
    , id(0)
    , dhcp_st(DHCP_IDLE_STATUS)
    , _dhcp_lease_state(DHCP_CHECK_NONE)
{
    /* -------------------------------------------------------------------------- */
    memset(hostname, 0x00, MAX_HOSTNAME_DIM);
    hostname[0] = 'C';
    hostname[1] = '3';
    hostname[2] = '3';
#if LWIP_NETIF_HOSTNAME
    ni.hostname = (const char*)&hostname;
#endif
#ifdef CNETWORK_INTERFACE_DEBUG
    Serial.println("[CNET]: CNetIf constructor");
#endif
}

/* -------------------------------------------------------------------------- */
CNetIf::~CNetIf()
{
    /* -------------------------------------------------------------------------- */
#ifdef CNETWORK_INTERFACE_DEBUG
    Serial.println("[CNET]: CNetIf destructor");
#endif
}

/* ***************************************************************************
 *                               DHCP related functions
 * ****************************************************************************/

/* -------------------------------------------------------------------------- */
void CNetIf::DhcpNotUsed()
{
    /* -------------------------------------------------------------------------- */
    DhcpStop();
    dhcp_inform(getNi());
}

/* -------------------------------------------------------------------------- */
int CNetIf::checkLease()
{
    /* -------------------------------------------------------------------------- */
    int rc = DHCP_CHECK_NONE;

    task();
    rc = dhcp_get_lease_state();

    if (rc != _dhcp_lease_state) {
        switch (_dhcp_lease_state) {
        case DHCP_CHECK_NONE:
            _dhcp_lease_state = rc;
            rc = DHCP_CHECK_NONE;
            break;

        case DHCP_CHECK_RENEW_OK:
            _dhcp_lease_state = rc;
            if (rc == DHCP_CHECK_NONE) {
                rc = DHCP_CHECK_RENEW_OK;
            } else {
                rc = DHCP_CHECK_RENEW_FAIL;
            }
            break;

        case DHCP_CHECK_REBIND_OK:
            _dhcp_lease_state = rc;
            if (rc == DHCP_CHECK_NONE) {
                rc = DHCP_CHECK_REBIND_OK;
            } else {
                rc = DHCP_CHECK_REBIND_FAIL;
            }
            break;

        default:
            _dhcp_lease_state = DHCP_CHECK_NONE;
            break;
        }
    }

    return rc;
}

/* -------------------------------------------------------------------------- */
uint8_t CNetIf::dhcp_get_lease_state()
{
    /* -------------------------------------------------------------------------- */
    uint8_t res = 0;
    struct dhcp* dhcp = (struct dhcp*)netif_get_client_data(getNi(), LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

    if (dhcp->state == 5 /*DHCP_STATE_RENEWING*/) {
        res = 2;
    } else if (dhcp->state == 4 /* DHCP_STATE_REBINDING */) {
        res = 4;
    }
    return res;
}

/* -------------------------------------------------------------------------- */
bool CNetIf::dhcp_request()
{
    /* -------------------------------------------------------------------------- */
    /* make a DHCP request: it runs till an address is acquired or a timeout
       expires */
    unsigned long startTime = millis();
    bool acquired = false;

    do {
        acquired = isDhcpAcquired();
        if (!acquired && ((millis() - startTime) > dhcp_timeout)) {
            break;
        }

    } while (!acquired);

    return acquired;
}

/* -------------------------------------------------------------------------- */
void CNetIf::dhcp_reset()
{
    /* -------------------------------------------------------------------------- */
    /* it resets the DHCP status to IDLE */
    while (dhcp_st != DHCP_IDLE_STATUS) {
        task();
    }
}

/* -------------------------------------------------------------------------- */
void CNetIf::DhcpSetTimeout(unsigned long t)
{
    /* -------------------------------------------------------------------------- */
    dhcp_timeout = t;
}

/* -------------------------------------------------------------------------- */
bool CNetIf::isDhcpAcquired()
{
    return dhcp_acquired;
}
/* -------------------------------------------------------------------------- */
bool CNetIf::DhcpStart()
{
    /* first stop / reset */
    DhcpStop();
    /* then actually start */
    dhcp_started = true;
    dhcp_st = DHCP_START_STATUS;
    return dhcp_request();
}
/* -------------------------------------------------------------------------- */
void CNetIf::DhcpStop()
{
    /* -------------------------------------------------------------------------- */
    dhcp_started = false;
    if (dhcp_st == DHCP_IDLE_STATUS) {
        return;
    }
    if (dhcp_st == DHCP_GOT_STATUS && netif_is_link_up(getNi())) {
        dhcp_st = DHCP_RELEASE_STATUS;
    } else {
        dhcp_st = DHCP_STOP_STATUS;
    }
    dhcp_reset();
}
/* -------------------------------------------------------------------------- */
void CNetIf::dhcp_task()
{
    /* -------------------------------------------------------------------------- */

    struct dhcp* lwip_dhcp;
    static unsigned long DHCPStartTime;

    switch (dhcp_st) {
    case DHCP_IDLE_STATUS:
        /* nothing to do... wait for DhcpStart() to start the process */
        break;
    case DHCP_START_STATUS:
        if (netif_is_link_up(getNi())) {
            ip_addr_set_zero_ip4(&(getNi()->ip_addr));
            ip_addr_set_zero_ip4(&(getNi()->netmask));
            ip_addr_set_zero_ip4(&(getNi()->gw));
            /* start lwIP dhcp */
            dhcp_start(getNi());

            DHCPStartTime = millis();
            dhcp_st = DHCP_WAIT_STATUS;
        }
        break;
    case DHCP_WAIT_STATUS:
        if (netif_is_link_up(getNi())) {
            if (dhcp_supplied_address(getNi())) {
                dhcp_acquired = true;
                dhcp_st = DHCP_GOT_STATUS;
            } else if (millis() - DHCPStartTime > 1000) {
                /* TIMEOUT */
                lwip_dhcp = (struct dhcp*)netif_get_client_data(getNi(), LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
                if (lwip_dhcp->tries > MAX_DHCP_TRIES) {
                    dhcp_st = DHCP_STOP_STATUS;
                }
            }
        } else {
            dhcp_st = DHCP_START_STATUS;
        }
        break;
    case DHCP_GOT_STATUS:
        if (!netif_is_link_up(getNi())) {
            dhcp_st = DHCP_STOP_STATUS;
        }

        break;
    case DHCP_RELEASE_STATUS:
        dhcp_release(getNi());
        dhcp_acquired = false;
        dhcp_st = DHCP_STOP_STATUS;
        break;
    case DHCP_STOP_STATUS:
        dhcp_acquired = false;
        dhcp_stop(getNi());
        if (dhcp_started) {
            dhcp_st = DHCP_START_STATUS;
        } else {
            dhcp_st = DHCP_IDLE_STATUS;
        }
        break;
    }
}

/* -------------------------------------------------------------------------- */
void CNetIf::setLinkUp()
{
    /* -------------------------------------------------------------------------- */
    netif_set_link_up(&ni);
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&ni);
}

/* -------------------------------------------------------------------------- */
void CNetIf::setLinkDown()
{
    /* -------------------------------------------------------------------------- */
    netif_set_link_down(&ni);
    /* When the netif is fully configured this function must be called.*/
    netif_set_down(&ni);
}

/* ########################################################################## */
/*                      ETHERNET NETWORK INTERFACE CLASS                      */
/* ########################################################################## */
CEth::CEth() { }
CEth::~CEth() { }

/* -------------------------------------------------------------------------- */
void CEth::begin(IPAddress _ip, IPAddress _gw, IPAddress _nm)
{
    if (_ip == INADDR_NONE) {
      _ip = default_ip;
      _nm = default_nm;
      _gw = default_gw;
    }
    IP_ADDR4(&ip, _ip[0], _ip[1], _ip[2], _ip[3]);
    IP_ADDR4(&nm, _nm[0], _nm[1], _nm[2], _nm[3]);
    IP_ADDR4(&gw, _gw[0], _gw[1], _gw[2], _gw[3]);

    netif_add(&ni, &ip, &nm, &gw, NULL, CEth::init, ethernet_input);
    netif_set_default(&ni);

    if (netif_is_link_up(&ni)) {
        /* When the netif is fully configured this function must be called */
        netif_set_up(&ni);
    } else {
        /* When the netif link is down this function must be called */
        netif_set_down(&ni);
    }

#if LWIP_NETIF_LINK_CALLBACK
    /* Set the link callback function, this function is called on change of link status */
    // netif_set_link_callback(&eth0if, eht0if_link_toggle_cbk);
#endif /* LWIP_NETIF_LINK_CALLBACK */
    /*
     * set the callback function that is called when an ethernet frame is physically
     * received, it is important that the callbacks are set before the initializiation
     */
    eth_set_rx_frame_cbk(std::bind(&CEth::handleEthRx, this));
    eth_set_link_on_cbk(std::bind(&CEth::setLinkUp, this));
    eth_set_link_off_cbk(std::bind(&CEth::setLinkDown, this));
}

/* -------------------------------------------------------------------------- */
void CEth::task()
{
    /* -------------------------------------------------------------------------- */

    eth_execute_link_process();

#if LWIP_DHCP
    static unsigned long dhcp_last_time_call = 0;
    if (dhcp_last_time_call == 0 || millis() - dhcp_last_time_call > DHCP_FINE_TIMER_MSECS) {
        dhcp_task();
        dhcp_last_time_call = millis();
    }
#endif
}

/* ########################################################################## */
/*                    CWifiStation NETWORK INTERFACE CLASS                    */
/* ########################################################################## */

CWifiStation::CWifiStation() { }
CWifiStation::~CWifiStation() { }

void CWifiStation::begin(IPAddress _ip, IPAddress _gw, IPAddress _nm)
{
    if (_ip == INADDR_NONE) {
      _ip = default_ip;
      _nm = default_nm;
      _gw = default_gw;
    }
    IP_ADDR4(&ip, _ip[0], _ip[1], _ip[2], _ip[3]);
    IP_ADDR4(&nm, _nm[0], _nm[1], _nm[2], _nm[3]);
    IP_ADDR4(&gw, _gw[0], _gw[1], _gw[2], _gw[3]);

    netif_add(&ni, &ip, &nm, &gw, NULL, CWifiStation::init, ethernet_input);
    netif_set_default(&ni);

    if (netif_is_link_up(&ni)) {
        /* When the netif is fully configured this function must be called */
        netif_set_up(&ni);
    } else {
        /* When the netif link is down this function must be called */
        netif_set_down(&ni);
    }

#if LWIP_NETIF_LINK_CALLBACK
    /* Set the link callback function, this function is called on change of link status */
    // netif_set_link_callback(&eth0if, eht0if_link_toggle_cbk);
#endif /* LWIP_NETIF_LINK_CALLBACK */
}

/* -------------------------------------------------------------------------- */
void CWifiStation::task()
{
    /* -------------------------------------------------------------------------- */
    /* get messages and process it  */
    uint8_t if_num;
    uint16_t dim = 0;
    uint8_t* buf = nullptr;
    struct pbuf* p = nullptr;

    /* shall we verify something about if_num??? */
    do {
        dim = CEspControl::getInstance().peekStationRxMsgSize();
        if (dim > 0)
        {
            p = pbuf_alloc(PBUF_RAW, dim, PBUF_RAM);
            if (p != nullptr)
            {
                buf = CEspControl::getInstance().getStationRx(if_num, dim);
                /* Copy ethernet frame into pbuf */
                pbuf_take((struct pbuf*)p, (uint8_t*)buf, (uint32_t)dim);
                if (ni.input(p, &ni) != ERR_OK) {
                    pbuf_free(p);
                }
                delete[] buf;
            }
        }
    } while(dim > 0 && p != nullptr);


#if LWIP_DHCP
    static unsigned long dhcp_last_time_call = 0;
    if (dhcp_last_time_call == 0 || millis() - dhcp_last_time_call > DHCP_FINE_TIMER_MSECS) {
        dhcp_task();
        dhcp_last_time_call = millis();
    }
#endif
}

/* -------------------------------------------------------------------------- */
int CWifiStation::getMacAddress(uint8_t* mac) {
/* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getMacAddress(NI_WIFI_STATION, mac);
}

/* -------------------------------------------------------------------------- */
const char* CWifiStation::getSSID()
{
    /* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getSSID(NI_WIFI_STATION);
}

/* -------------------------------------------------------------------------- */
uint8_t* CWifiStation::getBSSID(uint8_t* bssid)
{
    /* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getBSSID(NI_WIFI_STATION, bssid);
}

/* -------------------------------------------------------------------------- */
int32_t CWifiStation::getRSSI()
{
    /* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getRSSI(NI_WIFI_STATION);
}

/* -------------------------------------------------------------------------- */
uint8_t CWifiStation::getEncryptionType()
{
    /* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getEncryptionType(NI_WIFI_STATION);
}

/* ########################################################################## */
/*                      CWifiSoftAp NETWORK INTERFACE CLASS                   */
/* ########################################################################## */

CWifiSoftAp::CWifiSoftAp() { }
CWifiSoftAp::~CWifiSoftAp() { }

void CWifiSoftAp::begin(IPAddress _ip, IPAddress _gw, IPAddress _nm)
{
    if (_ip == INADDR_NONE) {
      _ip = default_dhcp_server_ip;
      _nm = default_nm;
      _gw = default_dhcp_server_ip;
    }
    IP_ADDR4(&ip, _ip[0], _ip[1], _ip[2], _ip[3]);
    IP_ADDR4(&nm, _nm[0], _nm[1], _nm[2], _nm[3]);
    IP_ADDR4(&gw, _gw[0], _gw[1], _gw[2], _gw[3]);

    netif_add(&ni, &ip, &nm, &gw, NULL, CWifiSoftAp::init, ethernet_input);
    netif_set_default(&ni);
    if (netif_is_link_up(&ni)) {
        /* When the netif is fully configured this function must be called */
        netif_set_up(&ni);
    } else {
        /* When the netif link is down this function must be called */
        netif_set_down(&ni);
    }

#if LWIP_NETIF_LINK_CALLBACK
    /* Set the link callback function, this function is called on change of link status */
    // netif_set_link_callback(&eth0if, eht0if_link_toggle_cbk);
#endif /* LWIP_NETIF_LINK_CALLBACK */
}

/* -------------------------------------------------------------------------- */
void CWifiSoftAp::task()
{
    /* -------------------------------------------------------------------------- */
    /* get messages and process it
     * TODO change the algorithm and make it similar to WiFiStation */
    uint8_t if_num;
    uint16_t dim;
    uint8_t* buf = nullptr;
    /* shall we verify something about if_num??? */
    do {

        buf = CEspControl::getInstance().getSoftApRx(if_num, dim);

        if (buf != nullptr) {
            struct pbuf* p = pbuf_alloc(PBUF_RAW, dim, PBUF_RAM);
            if (p != NULL) {
                /* Copy ethernet frame into pbuf */
                pbuf_take((struct pbuf*)p, (uint8_t*)buf, (uint32_t)dim);
                delete[] buf;

                if (ni.input(p, &ni) != ERR_OK) {
                    pbuf_free(p);
                }
            }
        }
    } while(buf != nullptr);

#if LWIP_DHCP
    static unsigned long dhcp_last_time_call = 0;
    if (dhcp_last_time_call == 0 || millis() - dhcp_last_time_call > DHCP_FINE_TIMER_MSECS) {
        dhcp_task();
        dhcp_last_time_call = millis();
    }
#endif
}

/* -------------------------------------------------------------------------- */
int CWifiSoftAp::getMacAddress(uint8_t* mac)
{
    /* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getMacAddress(NI_WIFI_SOFTAP, mac);
}

/* -------------------------------------------------------------------------- */
const char* CWifiSoftAp::getSSID()
{
    /* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getSSID(NI_WIFI_SOFTAP);
}

/* -------------------------------------------------------------------------- */
uint8_t* CWifiSoftAp::getBSSID(uint8_t* bssid)
{
    /* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getBSSID(NI_WIFI_SOFTAP, bssid);
}

/* -------------------------------------------------------------------------- */
int32_t CWifiSoftAp::getRSSI()
{
    /* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getRSSI(NI_WIFI_SOFTAP);
}

/* -------------------------------------------------------------------------- */
uint8_t CWifiSoftAp::getEncryptionType()
{
    /* -------------------------------------------------------------------------- */
    return CLwipIf::getInstance().getEncryptionType(NI_WIFI_SOFTAP);
}


#if DHCPS_DEBUG == 1
char b_dbg[512];
extern "C" void printDbg(const char* fmt, ...)
{

    memset(b_dbg, 0x00, 256);
    va_list va;
    va_start(va, fmt);
    vsprintf(b_dbg, fmt, va);
    va_end(va);

    Serial.println(b_dbg);
}
#endif

