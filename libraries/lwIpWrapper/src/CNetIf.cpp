#include "CNetIf.h"
#include <functional>

IPAddress CNetIf::default_ip("192.168.0.10");
IPAddress CNetIf::default_nm("255.255.255.0");
IPAddress CNetIf::default_gw("192.168.0.1");
IPAddress CNetIf::default_dhcp_server_ip("192.168.4.1");

CNetIf* CLwipIf::net_ifs[] = { nullptr };
bool CLwipIf::wifi_hw_initialized = false;
bool CLwipIf::connected_to_access_point = false;
WifiStatus_t CLwipIf::wifi_status = WL_IDLE_STATUS;
bool CLwipIf::pending_eth_rx = false;

FspTimer CLwipIf::timer;

ip_addr_t* u8_to_ip_addr(uint8_t* ipu8, ip_addr_t* ipaddr)
{
    IP_ADDR4(ipaddr, ipu8[0], ipu8[1], ipu8[2], ipu8[3]);
    return ipaddr;
}

uint32_t ip_addr_to_u32(ip_addr_t* ipaddr)
{
    return ip4_addr_get_u32(ipaddr);
}

static uint8_t Encr2wl_enc(int enc)
{
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

/* -------------------------------------------------------------------------- */
CLwipIf::CLwipIf()
    : eth_initialized(false)
    , dns_num(-1)
    , willing_to_start_sync_req(false)
    , async_requests_ongoing(true)
{
    /* -------------------------------------------------------------------------- */
    /* Initialize lwIP stack, singletone implementation guarantees that lwip is
       initialized just once  */
    lwip_init();

/* START THE TIMER FOR LWIP tasks - #CORE_DEPENDENT_STUFF */
#ifdef LWIP_USE_TIMER
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
    timer.begin(TIMER_MODE_PERIODIC, type, ch, 100.0, 50.0, timer_cb);
    timer.setup_overflow_irq();
    timer.open();
    timer.start();
#endif
}

/* -------------------------------------------------------------------------- */
void CLwipIf::lwip_task()
{
    /* -------------------------------------------------------------------------- */
    if (CLwipIf::wifi_hw_initialized)
        CEspControl::getInstance().communicateWithEsp();

    if (net_ifs[NI_ETHERNET] != nullptr) {
        net_ifs[NI_ETHERNET]->task();
    }

    if (net_ifs[NI_WIFI_STATION] != nullptr) {
        net_ifs[NI_WIFI_STATION]->task();
    }

    if (net_ifs[NI_WIFI_SOFTAP] != nullptr) {
        net_ifs[NI_WIFI_SOFTAP]->task();
    }

    /* Handle LwIP timeouts */
    sys_check_timeouts();

    if (willing_to_start_sync_req) {
        timer.disable_overflow_irq();
        willing_to_start_sync_req = false;
        async_requests_ongoing = false;
    }
}

/* -------------------------------------------------------------------------- */
/* GET INSTANCE SINGLETONE FUNCTION */
/* -------------------------------------------------------------------------- */
CLwipIf& CLwipIf::getInstance()
{
    /* -------------------------------------------------------------------------- */
    static CLwipIf instance;
    return instance;
}

/* -------------------------------------------------------------------------- */
CLwipIf::~CLwipIf()
{
    /* -------------------------------------------------------------------------- */
    for (int i = 0; i < NETWORK_INTERFACES_MAX_NUM; i++) {
        if (net_ifs[i] != nullptr) {
            delete net_ifs[i];
            net_ifs[i] = nullptr;
        }
    }
}

/* -------------------------------------------------------------------------- */
int CLwipIf::disconnectEventcb(CCtrlMsgWrapper *resp) {
    (void)resp;
    if(CLwipIf::connected_to_access_point) {
        wifi_status = WL_DISCONNECTED;
        if(net_ifs[NI_WIFI_STATION] != nullptr) {
            net_ifs[NI_WIFI_STATION]->setLinkDown();
        }
    }
    return ESP_CONTROL_OK;
}


/* -------------------------------------------------------------------------- */
int CLwipIf::initEventCb(CCtrlMsgWrapper *resp) {
    (void)resp;
    CLwipIf::wifi_hw_initialized = true;
    return ESP_CONTROL_OK;
}


/* -------------------------------------------------------------------------- */
int CLwipIf::setWifiMode(WifiMode_t mode) {
/* -------------------------------------------------------------------------- */   
      CLwipIf::getInstance().startSyncRequest();
      int rv = CEspControl::getInstance().setWifiMode(mode);
      CLwipIf::getInstance().restartAsyncRequest();
      return rv;
}

/* -------------------------------------------------------------------------- */
bool CLwipIf::initWifiHw(bool asStation)
{
    /* -------------------------------------------------------------------------- */
    bool rv = true;

    if (!CLwipIf::wifi_hw_initialized) {

        CEspControl::getInstance().listenForStationDisconnectEvent(CLwipIf::disconnectEventcb);
        CEspControl::getInstance().listenForInitEvent(CLwipIf::initEventCb);
        if (CEspControl::getInstance().initSpiDriver() == 0) {
            wifi_status = WL_NO_SSID_AVAIL;
        }

        if (wifi_status == WL_NO_SSID_AVAIL) {
            int time_num = 0;
            while (time_num < WIFI_INIT_TIMEOUT_MS && !CLwipIf::wifi_hw_initialized) {
                CEspControl::getInstance().communicateWithEsp();
                R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
                time_num++;
            }

            if (asStation) {
                int res = CLwipIf::getInstance().setWifiMode(WIFI_MODE_STA);

                if (res == ESP_CONTROL_OK) {
                    CLwipIf::getInstance().scanForAp();
                }
            } else {
                CEspControl::getInstance().setWifiMode(WIFI_MODE_AP);
            }
        }
    }

    if (wifi_status != WL_SCAN_COMPLETED) {
        rv = false;
    }

    return rv;
}

/* -------------------------------------------------------------------------- */
/* Sort of factory method, dependig on the requested type it setUp a different
   Network interface and returns it to the caller */
/* -------------------------------------------------------------------------- */
CNetIf* CLwipIf::get(NetIfType_t type,
    IPAddress _ip,
    IPAddress _gw,
    IPAddress _nm)
{
    /* -------------------------------------------------------------------------- */
    static int id = 0;
    CNetIf* rv = nullptr;
    bool isStation = true;
    bool isEth = false;

    if (type >= 0 && type < NETWORK_INTERFACES_MAX_NUM) {
        if (net_ifs[type] == nullptr) {
            switch (type) {
            case NI_WIFI_STATION:
                net_ifs[type] = new CWifiStation();
                isStation = true;
                break;

            case NI_WIFI_SOFTAP:
                net_ifs[type] = new CWifiSoftAp();
                isStation = false;
                break;

            case NI_ETHERNET:
                net_ifs[type] = new CEth();
                isEth = true;
                break;
            default:
                break;
            }

            if (net_ifs[type] != nullptr) {
                if (!isEth) {
                    CLwipIf::initWifiHw(isStation);
                    net_ifs[type]->begin(_ip, _gw, _nm);
                    net_ifs[type]->setId(0);
                } else {
                    eth_init();
                    net_ifs[type]->begin(_ip, _gw, _nm);
                    eth_initialized = true;
                }
            }
        }
        rv = net_ifs[type];
    }
    return rv;
}

/* -------------------------------------------------------------------------- */
void CEth::handleEthRx()
{
    /*
     * This function is called by the ethernet driver, when a frame is receiverd,
     * as a callback inside an interrupt context.
     * It is required to be as fast as possible and not perform busy waits.
     *
     * The idea is the following:
     * - take the rx buffer pointer
     * - try to allocate a pbuf of the desired size
     *   - if it is possible copy the the buffer inside the pbuf and give it to lwip netif
     * - release the buffer
     *
     * If the packet is discarded the upper TCP/IP layers should handle the retransmission of the lost packets.
     * This should not happen really often if the buffers and timers are designed taking into account the
     * desired performance
     */
    __disable_irq();

    volatile uint32_t rx_frame_dim = 0;
    volatile uint8_t* rx_frame_buf = eth_input(&rx_frame_dim);
    if (rx_frame_dim > 0 && rx_frame_buf != nullptr) {
        struct pbuf* p=nullptr;

        p = pbuf_alloc(PBUF_RAW, rx_frame_dim, PBUF_RAM);

        if (p != NULL) {
            /* Copy ethernet frame into pbuf */
            pbuf_take((struct pbuf*)p, (uint8_t*)rx_frame_buf, (uint32_t)rx_frame_dim);

            if (ni.input((struct pbuf*)p, &ni) != ERR_OK) {
                pbuf_free((struct pbuf*)p);
            }
        }

        eth_release_rx_buffer();
    }
    __enable_irq();
}

/* -------------------------------------------------------------------------- */
err_t CEth::init(struct netif* _ni)
{
    /* -------------------------------------------------------------------------- */
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    _ni->hostname = "C33-onEth";
#endif /* LWIP_NETIF_HOSTNAME */

    _ni->name[0] = ETH_IFNAME0;
    _ni->name[1] = ETH_IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    _ni->output = etharp_output;
    _ni->linkoutput = CEth::output;

    /* set MAC hardware address */
    _ni->hwaddr_len = eth_get_mac_address(_ni->hwaddr);

    /* maximum transfer unit */
    _ni->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    _ni->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    return ERR_OK;
}

/* -------------------------------------------------------------------------- */
err_t CEth::output(struct netif* _ni, struct pbuf *p) {
/* -------------------------------------------------------------------------- */
    /*
     * This function is called inside the lwip timeout engine. Since we are working inside
     * an environment without threads it is required to not lock. For this reason we should
     * avoid busy waiting and instead discard the transmission. Lwip will handle the retransmission
     * of the packet.
     */
    (void)_ni;

    err_t errval = ERR_OK;

    if(eth_output_can_transimit()) {
        uint16_t tx_buf_dim = 0;

        // TODO analyze the race conditions that may arise from sharing a non synchronized buffer
        uint8_t *tx_buf = eth_get_tx_buffer(&tx_buf_dim);
        assert (p->tot_len <= tx_buf_dim);

        uint16_t bytes_actually_copied = pbuf_copy_partial(p, tx_buf, p->tot_len, 0);

        if (bytes_actually_copied > 0 && !eth_output(tx_buf, bytes_actually_copied)) {
            errval = ERR_IF;
        }
    } else {
        errval = ERR_INPROGRESS;
    }
    return errval;
}

/* -------------------------------------------------------------------------- */
err_t CWifiStation::output(struct netif* _ni, struct pbuf *p) {
/* -------------------------------------------------------------------------- */   
    (void)_ni;
    err_t errval = ERR_IF;
    uint8_t *buf = new uint8_t[p->tot_len];
    if (buf != nullptr) {
        uint16_t bytes_actually_copied = pbuf_copy_partial(p, buf, p->tot_len, 0);
        if (bytes_actually_copied > 0) {
            int ifn = 0;
            if (CLwipIf::net_ifs[NI_WIFI_STATION] != nullptr) {
                ifn = CLwipIf::net_ifs[NI_WIFI_STATION]->getId();
            }

#ifdef DEBUG_OUTPUT_DISABLED
            Serial.println("Bytes LWIP wants to send: ");

            for (int i = 0; i < bytes_actually_copied; i++) {
                Serial.print(buf[i], HEX);
                Serial.print(" ");
            }
            Serial.println();
#endif

            if (CEspControl::getInstance().sendBuffer(ESP_STA_IF, ifn, buf, bytes_actually_copied) == ESP_CONTROL_OK) {
                errval = ERR_OK;
            }
        }
        delete[] buf;
    }

    return errval;
}

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

    CLwipIf::getInstance().startSyncRequest();
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

    CLwipIf::getInstance().startSyncRequest();
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

