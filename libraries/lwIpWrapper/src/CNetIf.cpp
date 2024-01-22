#include "CNetIf.h"
#include <functional>
#include "utils.h"

// TODO make better documentation on how this works
// TODO hostname should be defined at network stack level and shared among ifaces
// TODO buffer management (allocation/deallocation/trim/etc.) should be properly handled by a wrapper class and be transparent wrt the user
// TODO the device could be moving and as a consequence it may be nice to rescan APs to get one with the best rssi
// TODO implement stop softAP and include it in the destructor of the class
// TODO split netif definition in different files
// TODO implement WIFINetworkDriver that is then being used by both Wifi station and softAP. This will allow to use both at the same time
// TODO adapt network statistics collection
// TODO define enum for error collection and return them instead of int value
// FIXME Wifi driver requires interrupt safety in order to work properly in the timer

extern "C" void dhcps_start(struct netif *netif);

err_t _netif_init(struct netif* ni);
err_t _netif_output(struct netif* ni, struct pbuf* p);

#if LWIP_DNS
static void _getHostByNameCBK(const char *name, const ip_addr_t *ipaddr, void *callback_arg);
#endif // LWIP_DNS

#ifdef LWIP_USE_TIMER
static void timer_cb(timer_callback_args_t* arg);
#endif // LWIP_USE_TIMER

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

    // SYS_ARCH_PROTECT(zerocopy_pbuf_free);

    // FIXME pbufs may be allocated in a different memory pool, deallocate them accordingly
    zcpbuf->buffer_free(zcpbuf->buffer);
    zcpbuf->buffer = nullptr;
    mem_free(zcpbuf); // TODO understand if pbuf_free deletes the pbuf
    // SYS_ARCH_UNPROTECT(zerocopy_pbuf_free);
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

#ifdef LWIP_USE_TIMER
    uint8_t type = 8;
    int8_t ch = FspTimer::get_available_timer(type);

    if (ch < 0) {
        ch = FspTimer::get_available_timer(type, true);
    }

    /*
     * FIXME update this comment
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

#ifdef LWIP_USE_TIMER
static void timer_cb(timer_callback_args_t* arg) {
    CLwipIf* context = (CLwipIf*)arg->p_context;

    context->task();
}
#endif

void CLwipIf::task() {
    for(CNetIf* iface: this->ifaces) {
        iface->task();
    }

    arduino::lock();
    sys_check_timeouts();
    arduino::unlock();
}

void CLwipIf::setDefaultIface(CNetIf* iface) {
    // TODO check if the iface is in the vector

    netif_set_default(&iface->ni);
}

void CLwipIf::add_iface(CNetIf* iface) {
    // if it is the first interface set it as the default route
    if(this->ifaces.empty()) {
        netif_set_default(&iface->ni);

#ifdef LWIP_USE_TIMER
        timer.setup_overflow_irq();
        timer.open();
        timer.start();
#endif
    }

    // add the interface if not already present in the vector
    this->ifaces.push_back(iface);
}

CLwipIf::~CLwipIf() {
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

IPAddress CLwipIf::getDns(int n) {
    const ip_addr_t* dns = dns_getserver(n);

    return toArduinoIP(dns);
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
    /*
     * according to lwip documentation: addr is a pointer to a ip_addr_t where to store the address if it is already cached
     * in the dns_table (only valid if ERR_OK is returned!); thus this won't be the same ip_addr_t passed to the callback,
     * there is no need to allocate it in the heap and delete it afterwards.
     * on the contrary the struct dns_cbk must be allocated in the heap
     */
    ip_addr_t addr;
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

CNetIf::CNetIf(NetworkDriver *driver)
: driver(driver)
#ifdef LWIP_DHCP
    , dhcp_acquired(false)
#endif
{
    // NETIF_STATS_INIT(this->stats);

    if(driver != nullptr) {
        // driver->stats = this->stats;
        // TODO check that this calls are effective
        driver->setLinkDownCallback(std::bind(&CNetIf::linkDownCallback, this));
        driver->setLinkUpCallback(std::bind(&CNetIf::linkUpCallback, this));
    }
}

int CNetIf::begin(const IPAddress &ip, const IPAddress &nm, const IPAddress &gw) {
    CLwipIf::getInstance(); // This call is required in order to setup the network stack

    if(driver != nullptr) {
        driver->begin();
    }

    ip_addr_t _ip = fromArduinoIP(ip);
    ip_addr_t _nm = fromArduinoIP(nm);
    ip_addr_t _gw = fromArduinoIP(gw);

    char name[3] = {
        ni.name[0],
        ni.name[1],
        ni.num + '0',
    };
    if(netif_find(name) == nullptr) {

        // netif add copies the ip addresses into the netif, no need to store them also in the object
        struct netif *_ni = netif_add(
            &this->ni,
            &_ip, &_nm, &_gw, // ip addresses are being copied and not taken as reference, use a local defined variable
            this,
            _netif_init,
            ethernet_input
        );

        if(_ni == nullptr) {
            return -1;
        }
    } else {
        if (netif_is_link_up(&this->ni)) {
            netif_set_down(&this->ni);
        }

        // TODO check for any changes detected and update the iface
    }
    netif_set_up(&this->ni);

    // add the interface to the network stack
    CLwipIf::getInstance().add_iface(this); // TODO remove interface when it is needed (??)
    netif_set_link_up(&this->ni);

#ifdef LWIP_DHCP
    // dhcp is started when begin gets ip == nullptr
    if(ip != INADDR_NONE) {
        this->dhcpNotUsed();
    } else {
        this->dhcpStart();


        CLwipIf::getInstance().sync_timer();
        while(!this->isDhcpAcquired()) {
            CLwipIf::getInstance().task();
        }
        CLwipIf::getInstance().enable_timer();
    }

#endif

    return this->isDhcpAcquired()? 1 : 0;
}

void CNetIf::task() {
#ifdef LWIP_DHCP
    // TODO add timeout
    if(!this->dhcp_acquired && dhcp_supplied_address(&this->ni)) {
        dhcp_acquired = true;
    }

#endif
    if(driver != nullptr) {
        driver->poll();
    }
}


err_t _netif_init(struct netif* ni) {
    CNetIf *iface = (CNetIf*)ni->state;

    return iface->init(ni); // This function call can be a jmp instruction
}

err_t _netif_output(struct netif* ni, struct pbuf* p) {
    CNetIf *iface = (CNetIf*)ni->state;

    return iface->output(ni, p); // This function call can be a jmp instruction
}

void CNetIf::up() {
    netif_set_up(&this->ni);
}

void CNetIf::down() {
    netif_set_down(&this->ni);
}

void CNetIf::setLinkUp() {
    this->up();
}

void CNetIf::setLinkDown() {
    this->down();
}

void CNetIf::linkUpCallback() {
    netif_set_link_up(&this->ni);
}

void CNetIf::linkDownCallback() {
    netif_set_link_down(&this->ni);
}

/* ##########################################################################
 *                               DHCP related functions
 * ########################################################################## */

void CNetIf::config(IPAddress _ip, IPAddress _gw, IPAddress _nm) {
#ifdef LWIP_DHCP
    dhcpStop();
    dhcpStart();
    dhcpNotUsed();
#endif

    ip_addr_t ip = fromArduinoIP(_ip);
    ip_addr_t nm = fromArduinoIP(_gw);
    ip_addr_t gw = fromArduinoIP(_nm);

    netif_set_addr(&ni, &ip, &nm, &gw);

    if (netif_is_link_up(&ni)) {
        netif_set_down(&ni);
        netif_set_up(&ni);
    }
}


#ifdef LWIP_DHCP

void CNetIf::dhcpNotUsed() {
    dhcp_inform(&this->ni);
}

bool CNetIf::isDhcpAcquired() {
    // if(dhcp_acquired) {
    //     Serial.println(ip_2_ip4(ni.ip_addr).addr, HEX);
    // }
    return dhcp_acquired;
}

bool CNetIf::dhcpStart() {
    return dhcp_start(&this->ni) == ERR_OK;
}

void CNetIf::dhcpStop() {
    this->dhcpRelease();
    dhcp_stop(&this->ni);
}
bool CNetIf::dhcpRelease() {
    return dhcp_release(&this->ni) == ERR_OK;
}

bool CNetIf::dhcpRenew() {
    return dhcp_renew(&this->ni) == ERR_OK;
}

#endif

/* ##########################################################################
 *                      ETHERNET NETWORK INTERFACE CLASS
 * ########################################################################## */
const char CEth::eth_ifname[] = "en";

CEth::CEth(NetworkDriver *driver)
: CNetIf(driver) {
    // driver.stats = &this->stats;
}

int CEth::begin(const IPAddress &ip, const IPAddress &nm, const IPAddress &gw, const IPAddress &dns) {
    // The driver needs a callback to consume the incoming buffer
    this->driver->setConsumeCallback(
        std::bind(&CEth::consume_callback,
            this, std::placeholders:: _1, std::placeholders::_2));

    // Call the begin function on the Parent class to init the interface
    // netif_set_link_up(&this->ni);
    auto res = CNetIf::begin(ip, nm, gw);

    CLwipIf::getInstance().addDnsServer(dns);

    return res;
}

int CEth::begin(
        uint8_t *mac_address,
        const IPAddress &local_ip,
        const IPAddress &dns_server,
        const IPAddress &gateway,
        const IPAddress &subnet,
        const unsigned long timeout,
        const unsigned long responseTimeout) {

    this->setMacAddress(mac_address);

    return this->begin(local_ip, subnet, gateway, dns_server);
}

int CEth::begin(
        uint8_t *mac_address,
        const unsigned long timeout,
        const unsigned long responseTimeout) {

    this->setMacAddress(mac_address);

    return this->begin();
}


err_t CEth::init(struct netif* ni) {
    // Setting up netif
#if LWIP_NETIF_HOSTNAME
    ni->hostname                       = "C33_eth";
#endif
    ni->name[0]                        = CEth::eth_ifname[0];
    ni->name[1]                        = CEth::eth_ifname[1];
    ni->mtu                            = 1500; // TODO get this from the network
    ni->flags                          |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    memcpy(ni->hwaddr, this->driver->getMacAddress(), 6); // FIXME handle this using a constant
    ni->hwaddr_len                     = 6;

    ni->output                         = etharp_output;
    ni->linkoutput                     = _netif_output;

    return ERR_OK;
}

err_t CEth::output(struct netif* ni, struct pbuf* p) {
    err_t errval = ERR_OK;

    /* TODO check if this makes sense, I may get a pbuf chain
     * it could happen that if I get a pbuf chain
     * - there are enough tx_buffers available to accomodate all the packets in the chain
     * - most of the chain is enqueued for delivery, but a certain point the driver.send call returns error
     *   then lwip is supposed to handle that, that may be an issue
     */
    struct pbuf *q = p;
    do {
        // NETIF_STATS_INCREMENT_TX_TRANSMIT_CALLS(this->stats);
        // NETIF_STATS_TX_TIME_START(this->stats);
        auto err = driver->send((uint8_t*)q->payload, q->len);
        if(err != 0) {
            // NETIF_STATS_INCREMENT_ERROR(this->stats, err);
            // NETIF_STATS_INCREMENT_TX_TRANSMIT_FAILED_CALLS(this->stats);
            errval = ERR_IF;
            break;
        }
        // NETIF_STATS_INCREMENT_TX_BYTES(this->stats, q->len);
        // NETIF_STATS_TX_TIME_AVERAGE(this->stats);
        q = q->next;
    } while(q != nullptr && errval != ERR_OK);

    return errval;
}

void CEth::consume_callback(uint8_t* buffer, uint32_t len) {
    // TODO understand if this callback can be moved into the base class

    const uint16_t trimmed_size = len;

    // zerocopy_pbuf_t *custom_pbuf = get_zerocopy_pbuf(buffer, 1536);
    zerocopy_pbuf_t *custom_pbuf = get_zerocopy_pbuf(buffer, trimmed_size);

    // mem_trim should be passed as an argument, since it depends on the kind of allocation performed
    void* buf = mem_trim(buffer, trimmed_size);

    // TODO consider allocating a custom pool for RX or use PBUF_POOL
    struct pbuf *p = pbuf_alloced_custom(
        PBUF_RAW, len, PBUF_RAM, &custom_pbuf->p, buffer, trimmed_size);

    err_t err = this->ni.input((struct pbuf*)p, &this->ni);
    if (err != ERR_OK) {
        // NETIF_STATS_INCREMENT_ERROR(this->stats, err);

        // NETIF_STATS_INCREMENT_RX_NI_INPUT_FAILED_CALLS(this->stats);
        // NETIF_STATS_INCREMENT_RX_INTERRUPT_FAILED_CALLS(this->stats);
        pbuf_free((struct pbuf*)p);
    } else {
        // NETIF_STATS_INCREMENT_RX_BYTES(this->stats, p->len);
    }
}

/* ########################################################################## */
/*                    CWifiStation NETWORK INTERFACE CLASS                    */
/* ########################################################################## */
const char CWifiStation::wifistation_ifname[] = "ws";

CWifiStation::CWifiStation()
: hw_init(false) {
}

CWifiStation::~CWifiStation() {

}

int CWifiStation::begin(const IPAddress &ip, const IPAddress &nm, const IPAddress &gw) {
    return CNetIf::begin(ip, nm, gw);
}

int CWifiStation::connectToAP(const char* ssid, const char *passphrase) {
    WifiApCfg_t ap;
    int rv = ESP_CONTROL_CTRL_ERROR; // FIXME this should be set with an error meaning AP not found
    bool found = false;
    int8_t best_index = -1; // this index is used to find the ap with the best rssi
    int time_num = 0;

    CEspControl::getInstance().listenForStationDisconnectEvent([this] (CCtrlMsgWrapper *resp) -> int {
        netif_set_link_down(&this->ni);
        wifi_status = WL_DISCONNECTED;
        return ESP_CONTROL_OK;
    });
    CEspControl::getInstance().listenForInitEvent([this] (CCtrlMsgWrapper *resp) -> int {
        // Serial.println("init");
        this->hw_init = true;
        return ESP_CONTROL_OK;
    });

    if ((rv=CEspControl::getInstance().initSpiDriver()) != 0  && !hw_init) {
        rv = -1; // FIXME put a proper error code
        goto exit;
    }
    wifi_status = WL_NO_SSID_AVAIL;

    while (time_num < 100 && !hw_init) { // TODO #define WIFI_INIT_TIMEOUT_MS 10000
        CEspControl::getInstance().communicateWithEsp();
        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
        time_num++;
    }

    CLwipIf::getInstance().sync_timer();
    rv = CEspControl::getInstance().setWifiMode(WIFI_MODE_STA);
    CLwipIf::getInstance().enable_timer();

    if((rv=this->scanForAp()) != WL_SCAN_COMPLETED) {
        rv = -2;
        goto exit;
    }

    // find the AP with the best rssi
    for (uint8_t i = 0; i < access_points.size(); i++) {
        if(strcmp(ssid, (const char*)access_points[i].ssid) == 0
            && (best_index == -1 || access_points[best_index].rssi < access_points[i].rssi)
            ) {
            best_index=i;
        }
    }
    if(best_index != -1) {
        strncpy((char*)ap.ssid, ssid, SSID_LENGTH);

        if(passphrase != nullptr) {
            auto slen = strlen(passphrase)+1;
            strncpy((char*)ap.pwd, passphrase, (slen < PASSWORD_LENGTH) ? slen : PASSWORD_LENGTH);
        } else {
            ap.pwd[0] = '\0';
        }

        memset(ap.bssid, 0x00, BSSID_LENGTH);
        memcpy(ap.bssid, access_points[best_index].bssid, BSSID_LENGTH);

        CLwipIf::getInstance().sync_timer();
        rv=CEspControl::getInstance().connectAccessPoint(ap);

        if (rv == ESP_CONTROL_OK) {
            CEspControl::getInstance().getAccessPointConfig(access_point_cfg);
            wifi_status = WL_CONNECTED;


            netif_set_link_up(&this->ni);
        } else {
            wifi_status = WL_CONNECT_FAILED;
        }
        CLwipIf::getInstance().enable_timer();
    }

exit:
    return rv;
}

int CWifiStation::scanForAp() {
    access_points.clear();

    CLwipIf::getInstance().sync_timer();

    int res = CEspControl::getInstance().getAccessPointScanList(access_points);
    CLwipIf::getInstance().enable_timer();

    if (res == ESP_CONTROL_OK) {
        wifi_status = WL_SCAN_COMPLETED;
    } else {
        wifi_status = WL_NO_SSID_AVAIL;
    }


    return wifi_status;
}

// disconnect
int CWifiStation::disconnectFromAp() {
    CLwipIf::getInstance().sync_timer();

    auto res = CEspControl::getInstance().disconnectAccessPoint();

    CLwipIf::getInstance().enable_timer();

    return res;
}

err_t CWifiStation::init(struct netif* ni) {
    // Setting up netif
#if LWIP_NETIF_HOSTNAME
    ni->hostname                       = "C33-WifiSta";
#endif
    ni->name[0]                        = CWifiStation::wifistation_ifname[0];
    ni->name[1]                        = CWifiStation::wifistation_ifname[1];
    ni->mtu                            = 1500; // FIXME get this from the network
    ni->flags                          |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    WifiMac_t MAC;
    MAC.mode = WIFI_MODE_STA;
    CEspControl::getInstance().getWifiMacAddress(MAC);
    CNetUtilities::macStr2macArray(ni->hwaddr, MAC.mac);
    ni->hwaddr_len = 6; // FIXME this should be a macro defined somewhere
    // ni->hwaddr_len = CLwipIf::getInstance().getMacAddress(NI_WIFI_STATION, ni->hwaddr);

    ni->output                         = etharp_output;
    ni->linkoutput                     = _netif_output;

    return ERR_OK;
}

err_t CWifiStation::output(struct netif* _ni, struct pbuf* p) {
    // FIXME set ifn
    int ifn = 0; // interface number in CNetIf.cpp seems to not be set anywhere
    uint8_t *buf = nullptr;
    uint16_t size=p->tot_len;
    err_t errval = ERR_IF;
    int err = ESP_CONTROL_OK;

    // NETIF_STATS_INCREMENT_TX_TRANSMIT_CALLS(this->stats);
    // NETIF_STATS_TX_TIME_START(this->stats);

    // p may be a chain of pbufs
    if(p->next != nullptr) {
        buf = (uint8_t*) malloc(size*sizeof(uint8_t));
        if(buf == nullptr) {\
            // NETIF_STATS_INCREMENT_ERROR(this->stats, ERR_MEM);
            // NETIF_STATS_INCREMENT_TX_TRANSMIT_FAILED_CALLS(this->stats);
            errval = ERR_MEM;
            goto exit;
        }

        // copy the content of pbuf
        assert(pbuf_copy_partial(p, buf, size, 0) == size);
    } else {
        buf = (uint8_t*)p->payload;
    }

    // sendBuffer makes a memcpy of buffer
    // TODO send buffer should handle the buffer deletion and avoid a memcpy
    if ((err = CEspControl::getInstance().sendBuffer(
            ESP_STA_IF, ifn, buf, size)) == ESP_CONTROL_OK) {
        errval = ERR_OK;
        // NETIF_STATS_INCREMENT_TX_BYTES(this->stats, size);
        // NETIF_STATS_TX_TIME_AVERAGE(this->stats);
    } else {
        // NETIF_STATS_INCREMENT_ERROR(this->stats, err);
        // NETIF_STATS_INCREMENT_TX_TRANSMIT_FAILED_CALLS(this->stats);
    }

exit:
    if(p->next != nullptr && buf != nullptr) {
        free(buf);
    }
    return errval;
}

void CWifiStation::task() {
    // calling the base class task, in order to make thigs work
    CNetIf::task();

    uint8_t if_num = 0;
    uint16_t dim = 0;
    uint8_t* buffer = nullptr;
    struct pbuf* p = nullptr;

    // NETIF_STATS_RX_TIME_START(this->stats);
    // TODO do not perform this when not connected to an AP
    if(hw_init) {
        CEspControl::getInstance().communicateWithEsp();

        // TODO handling buffer this way may be harmful for the memory
        buffer = CEspControl::getInstance().getStationRx(if_num, dim);
    }

    // empty the ESP32 queue
    while(buffer != nullptr) {
        // FIXME this section is redundant and should be generalized toghether with CEth::consume_callback
        // NETIF_STATS_INCREMENT_RX_INTERRUPT_CALLS(this->stats);

        zerocopy_pbuf_t *custom_pbuf = get_zerocopy_pbuf(buffer, dim, free);

        // TODO consider allocating a custom pool for RX or use PBUF_POOL
        struct pbuf *p = pbuf_alloced_custom(
            PBUF_RAW, dim, PBUF_RAM, &custom_pbuf->p, buffer, dim);

        err_t err = this->ni.input((struct pbuf*)p, &this->ni);
        if (err != ERR_OK) {
            // NETIF_STATS_INCREMENT_ERROR(this->stats, err);

            // NETIF_STATS_INCREMENT_RX_NI_INPUT_FAILED_CALLS(this->stats);
            // NETIF_STATS_INCREMENT_RX_INTERRUPT_FAILED_CALLS(this->stats);
            pbuf_free((struct pbuf*)p);
        } else {
            // NETIF_STATS_INCREMENT_RX_BYTES(this->stats, p->len);
        }

        buffer = CEspControl::getInstance().getStationRx(if_num, dim);
    }
    // NETIF_STATS_RX_TIME_AVERAGE(this->stats);
}

// void CWifiStation::consume_callback(uint8_t* buffer, uint32_t len) {
//     // FIXME take what is written in task and put it in here
// }

const char* CWifiStation::getSSID() {
    return (const char*)access_point_cfg.ssid;
}

uint8_t* CWifiStation::getBSSID(uint8_t* bssid){
    CNetUtilities::macStr2macArray(bssid, (const char*)access_point_cfg.bssid);
    return bssid;
}

int32_t CWifiStation::getRSSI() {
    // TODO update the rssi on request of this method
    return (uint32_t)access_point_cfg.rssi;
}

uint8_t CWifiStation::getEncryptionType() {
    return Encr2wl_enc(access_point_cfg.encryption_mode);
}

// int CWifiStation::getMacAddress(uint8_t* mac) {
// }

uint8_t CWifiStation::getChannel() {
    return (uint8_t)access_point_cfg.channel;
}

const char* CWifiStation::getSSID(uint8_t i)
{
    if (access_points.size() > 0 && i < access_points.size()) {
        return (const char*)access_points[i].ssid;
    }
    return nullptr;
}

int32_t CWifiStation::getRSSI(uint8_t i) {
    if (access_points.size() > 0 && i < access_points.size()) {
        return (int32_t)access_points[i].rssi;
    }
    return 0;
}

uint8_t CWifiStation::getEncrType(uint8_t i) {
    if (access_points.size() > 0 && i < access_points.size()) {
        return Encr2wl_enc(access_points[i].encryption_mode);
    }
    return 0;
}

uint8_t* CWifiStation::getBSSID(uint8_t i, uint8_t* bssid) {
    if (access_points.size() > 0 && i < access_points.size()) {
        CNetUtilities::macStr2macArray(bssid, (const char*)access_points[i].bssid);
        return bssid;
    }
    return nullptr;
}

uint8_t CWifiStation::getChannel(uint8_t i) {
    if (access_points.size() > 0 && i < access_points.size()) {
        return (uint8_t)access_points[i].channel;
    }
    return 0;
}

int CWifiStation::setLowPowerMode() {
    return CEspControl::getInstance().setPowerSaveMode(1);
}

int CWifiStation::resetLowPowerMode() {
    return CEspControl::getInstance().setPowerSaveMode(1);
}

/* ########################################################################## */
/*                      CWifiSoftAp NETWORK INTERFACE CLASS                   */
/* ########################################################################## */
const char CWifiSoftAp::softap_ifname[] = "sa";

// This is required for dhcp server to assign ip addresses to AP clients
IPAddress default_nm("255.255.255.0");
IPAddress default_dhcp_server_ip("192.168.4.1");

CWifiSoftAp::CWifiSoftAp()
: hw_init(false) {

}

CWifiSoftAp::~CWifiSoftAp() { }


int CWifiSoftAp::begin(const IPAddress &ip, const IPAddress &nm, const IPAddress &gw) { // TODO use provided ip address, instead of default ones
    int res = 0;
    int time_num = 0;

    CEspControl::getInstance().listenForInitEvent([this] (CCtrlMsgWrapper *resp) -> int {
        // Serial.println("init");
        this->hw_init = true;
        return ESP_CONTROL_OK;
    });

    if ((res=CEspControl::getInstance().initSpiDriver()) != 0 && !hw_init) {
        // res = -1; // FIXME put a proper error code
        goto exit;
    }

    while (time_num < 100 && !hw_init) { // TODO #define WIFI_INIT_TIMEOUT_MS 10000
        CEspControl::getInstance().communicateWithEsp();
        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
        time_num++;
    }

    CLwipIf::getInstance().sync_timer();
    res = CEspControl::getInstance().setWifiMode(WIFI_MODE_AP);
    CLwipIf::getInstance().enable_timer();

    CNetIf::begin(
        default_dhcp_server_ip,
        default_nm,
        default_dhcp_server_ip
    );
exit:
    return res;
}

// TODO scan the other access point first and then set the channel if 0
// TODO there are requirements for ssid and password
int CWifiSoftAp::startSoftAp(const char* ssid, const char* passphrase, uint8_t channel) {
    CLwipIf::getInstance().sync_timer();
    SoftApCfg_t cfg;

    strncpy((char*)cfg.ssid, ssid, SSID_LENGTH);

    if (passphrase == nullptr) {
        cfg.pwd[0] = '\0';
        cfg.encryption_mode = WIFI_AUTH_OPEN;
    } else {
        auto slen = strlen(passphrase)+1;
        strncpy((char*)cfg.pwd, passphrase, (slen < PASSWORD_LENGTH) ? slen : PASSWORD_LENGTH);

        cfg.encryption_mode = WIFI_AUTH_WPA_WPA2_PSK;
    }

    channel = (channel == 0) ? 1 : channel;
    cfg.channel = (channel > MAX_CHNL_NO) ? MAX_CHNL_NO : channel;
    cfg.max_connections = MAX_SOFAT_CONNECTION_DEF; // FIXME make user decide this parameter
    cfg.bandwidth = WIFI_BW_HT40;
    cfg.ssid_hidden = false;

    int rv = CEspControl::getInstance().startSoftAccessPoint(cfg);
    if (rv == ESP_CONTROL_OK) {
        CEspControl::getInstance().getSoftAccessPointConfig(soft_ap_cfg);
        // wifi_status = WL_AP_LISTENING;
        netif_set_link_up(&this->ni);

        // FIXME the dhcp server should be started somewhere else
        dhcps_start(&(this->ni));
    } else {
        // wifi_status = WL_AP_FAILED;
    }

    CLwipIf::getInstance().enable_timer();
    return rv;
}

err_t CWifiSoftAp::init(struct netif* ni) {
    // Setting up netif
#if LWIP_NETIF_HOSTNAME
    // TODO pass the hostname in the constructor os with a setter
    ni->hostname                       = "C33-SoftAP";
#endif
    ni->name[0]                        = CWifiSoftAp::softap_ifname[0];
    ni->name[1]                        = CWifiSoftAp::softap_ifname[1];
    ni->mtu                            = 1500; // FIXME get this from the network
    ni->flags                          |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    WifiMac_t MAC;
    MAC.mode = WIFI_MODE_AP;
    CEspControl::getInstance().getWifiMacAddress(MAC);
    CNetUtilities::macStr2macArray(ni->hwaddr, MAC.mac);
    ni->hwaddr_len = 6; // FIXME this should be a macro defined somewhere

    ni->output                         = etharp_output;
    ni->linkoutput                     = _netif_output;

    return ERR_OK;
}

err_t CWifiSoftAp::output(struct netif* _ni, struct pbuf* p) {
    // FIXME set ifn
    int ifn = 0; // interface number in CNetIf.cpp seems to not be set anywhere
    uint8_t *buf = nullptr;
    uint16_t size=p->tot_len;
    err_t errval = ERR_IF;
    int err = ESP_CONTROL_OK;

    // NETIF_STATS_INCREMENT_TX_TRANSMIT_CALLS(this->stats);
    // NETIF_STATS_TX_TIME_START(this->stats);

    // p may be a chain of pbufs
    if(p->next != nullptr) {
        buf = (uint8_t*) malloc(size*sizeof(uint8_t));
        if(buf == nullptr) {
            // NETIF_STATS_INCREMENT_ERROR(this->stats, ERR_MEM);
            // NETIF_STATS_INCREMENT_TX_TRANSMIT_FAILED_CALLS(this->stats);
            errval = ERR_MEM;
            goto exit;
        }

        // copy the content of pbuf
        assert(pbuf_copy_partial(p, buf, size, 0) == size);
    } else {
        buf = (uint8_t*)p->payload;
    }

    // sendBuffer makes a memcpy of buffer
    // TODO send buffer should handle the buffer deletion and avoid a memcpy
    if ((err = CEspControl::getInstance().sendBuffer(
            ESP_AP_IF, ifn, buf, size)) == ESP_CONTROL_OK) {
        errval = ERR_OK;
        // NETIF_STATS_INCREMENT_TX_BYTES(this->stats, size);
        // NETIF_STATS_TX_TIME_AVERAGE(this->stats);
            } else {
        // NETIF_STATS_INCREMENT_ERROR(this->stats, err);
        // NETIF_STATS_INCREMENT_TX_TRANSMIT_FAILED_CALLS(this->stats);
    }

exit:
    if(p->next != nullptr && buf != nullptr) {
        free(buf);
    }
    return errval;
}

void CWifiSoftAp::task() {
    // calling the base class task, in order to make thigs work
    CNetIf::task();

    // TODO in order to make things easier this should be implemented inside of Wifi driver
    // and not override LWIPInterface method

    uint8_t if_num = 0;
    uint16_t dim = 0;
    uint8_t* buffer = nullptr;
    struct pbuf* p = nullptr;

    // NETIF_STATS_RX_TIME_START(this->stats);
    // TODO do not perform this when not connected to an AP
    if(hw_init) {
        CEspControl::getInstance().communicateWithEsp();

        // TODO handling buffer this way may be harmful for the memory
        buffer = CEspControl::getInstance().getSoftApRx(if_num, dim);
    }

    // empty the ESP32 queue
    while(buffer != nullptr) {
        // FIXME this section is redundant and should be generalized toghether with CEth::consume_callback
        // TODO understand if this should be moved into the base class
        // NETIF_STATS_INCREMENT_RX_INTERRUPT_CALLS(this->stats);

        zerocopy_pbuf_t *custom_pbuf = get_zerocopy_pbuf(buffer, dim, free);

        // TODO consider allocating a custom pool for RX or use PBUF_POOL
        struct pbuf *p = pbuf_alloced_custom(
            PBUF_RAW, dim, PBUF_RAM, &custom_pbuf->p, buffer, dim);

        err_t err = this->ni.input((struct pbuf*)p, &this->ni);
        if (err != ERR_OK) {
            // NETIF_STATS_INCREMENT_ERROR(this->stats, err);

            // NETIF_STATS_INCREMENT_RX_NI_INPUT_FAILED_CALLS(this->stats);
            // NETIF_STATS_INCREMENT_RX_INTERRUPT_FAILED_CALLS(this->stats);
            pbuf_free((struct pbuf*)p);
        } else {
            // NETIF_STATS_INCREMENT_RX_BYTES(this->stats, p->len);
        }

        buffer = CEspControl::getInstance().getStationRx(if_num, dim);
    }
    // NETIF_STATS_RX_TIME_AVERAGE(this->stats);
}

const char* CWifiSoftAp::getSSID() {
    return (const char*)soft_ap_cfg.ssid;
}

uint8_t* CWifiSoftAp::getBSSID(uint8_t* bssid){
    // CNetUtilities::macStr2macArray(bssid, (const char*)soft_ap_cfg.bssid);
    // return bssid;
}

uint8_t CWifiSoftAp::getEncryptionType() {
    return Encr2wl_enc(soft_ap_cfg.encryption_mode);
}

uint8_t CWifiSoftAp::getChannel() {
    return (uint8_t)soft_ap_cfg.channel;
}

int CWifiSoftAp::setLowPowerMode() {
    CLwipIf::getInstance().sync_timer();
    auto res = CEspControl::getInstance().setPowerSaveMode(1);
    CLwipIf::getInstance().enable_timer();

    return res;
}

int CWifiSoftAp::resetLowPowerMode() {
    CLwipIf::getInstance().sync_timer();
    auto res = CEspControl::getInstance().setPowerSaveMode(1);
    CLwipIf::getInstance().enable_timer();

    return res;
}

/* ##########################################################################
 *                      DEBUG UTILS
 * ########################################################################## */

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

