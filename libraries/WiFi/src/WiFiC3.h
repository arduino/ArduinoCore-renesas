#pragma once

#include "CNetIf.h"

#define WIFI_FIRMWARE_LATEST_VERSION "1.5.0"

// TODO Instantiate the drivers for wifi with default configuration parameters
// ESPHostFGDriver WifiDriver;

// Instantiate a global variable from CWifiStation calling it WiFi

inline CWifiStation WiFiStation;
inline CWifiSoftAp WiFiSoftAP;

class CWifi {
private:
    void _config(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
    unsigned long _timeout;
    bool _useStaticIp = false;
public:
    CWifi();

    /*
     * Get firmware version
     */
    static const char* firmwareVersion();


    /*
     * Start WiFi connection for OPEN networks
     * param ssid: Pointer to the SSID string.
     */
    int begin(const char* ssid);

    /* Start WiFi connection with passphrase
     * the most secure supported mode will be automatically selected
     *
     * param ssid: Pointer to the SSID string.
     * param passphrase: Passphrase. Valid characters in a passphrase
     *        must be between ASCII 32-126 (decimal).
     */
    int begin(const char* ssid, const char *passphrase);

    /* connect as Access Point with  a standard passphrase */
    uint8_t beginAP(const char *ssid);
    uint8_t beginAP(const char *ssid, uint8_t channel);
    uint8_t beginAP(const char *ssid, const char* passphrase);
    uint8_t beginAP(const char *ssid, const char* passphrase, uint8_t channel);

    /* Change IP configuration settings disabling the DHCP client
        *
        * param local_ip:  Static IP configuration
        */
    void config(IPAddress local_ip);

    /* Change IP configuration settings disabling the DHCP client
     *
     * param local_ip:  Static IP configuration
     * param dns_server:     IP configuration for DNS server 1
     */
    void config(IPAddress local_ip, IPAddress dns_server);

    /* Change IP configuration settings disabling the DHCP client
     *
     * param local_ip:  Static IP configuration
     * param dns_server:     IP configuration for DNS server 1
     * param gateway :  Static gateway configuration
     */
    void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway);

    /* Change IP configuration settings disabling the DHCP client
     *
     * param local_ip:  Static IP configuration
     * param dns_server:     IP configuration for DNS server 1
     * param gateway:   Static gateway configuration
     * param subnet:    Static Subnet mask
     */
    void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);

    /* Change DNS IP configuration
     *
     * param dns_server1: IP configuration for DNS server 1
     */
    void setDNS(IPAddress dns_server1);

    /* Change DNS IP configuration
     *
     * param dns_server1: IP configuration for DNS server 1
     * param dns_server2: IP configuration for DNS server 2
     *
     */
    void setDNS(IPAddress dns_server1, IPAddress dns_server2);


    /* Set the hostname used for DHCP requests
     *
     * param name: hostname to set
     *
     */
    void setHostname(const char* name);

    /*
     * Disconnect from the network
     *
     * return: one value of wl_status_t enum
     */
    int disconnect(void);

    void end(void);

    /*
     * Get the interface MAC address.
     *
     * return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
     *
     * the value returned by this function is meaningfull only if called
     * afert a begin (both begin or beginAP) or a ScanNetwork function
     * otherwise an empty mac address is returned
     */
    uint8_t* macAddress(uint8_t* mac);

    /*
     * Get the interface IP address.
     *
     * return: IP address value
     */
    IPAddress localIP();

    /*
     * Get the interface subnet mask address.
     *
     * return: subnet mask address value
     */
    IPAddress subnetMask();

    /*
     * Get the gateway IP address.
     *
     * return: gateway IP address value
     */
    IPAddress gatewayIP();

    /*
     * Get the DNS server IP address.
     *
     * return: DNS server IP address value
     */
    IPAddress dnsIP(int n = 0);

    /*
     * Return the current SSID associated with the network
     *
     * return: ssid string
     */
    const char* SSID();

    /*
      * Return the current BSSID associated with the network.
      * It is the MAC address of the Access Point
      *
      * return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
      */
    uint8_t* BSSID(uint8_t* bssid);

    /*
      * Return the current RSSI/Received Signal Strength in dBm)
      * associated with the network
      *
      * return: signed value
      */
    int32_t RSSI();

    /*
     * Return the Encryption Type associated with the network
     *
     * return: one value of wl_enc_type enum
     */
    uint8_t encryptionType();

    /*
     * Start scan WiFi networks available
     *
     * return: Number of discovered networks
     */
    int8_t scanNetworks();
    /*
     * Return the SSID discovered during the network scan.
     *
     * param networkItem: specify from which network item want to get the information
     *
     * return: SSID string of the specified item on the networks scanned list
     */
    const char*   SSID(uint8_t networkItem);

    /*
     * Return the encryption type of the networks discovered during the scanNetworks
     *
     * param networkItem: specify from which network item want to get the information
     *
     * return: encryption type (enum wl_enc_type) of the specified item on the networks scanned list
     *    enum wl_enc_type :
     *    ENC_TYPE_WEP,
     *    ENC_TYPE_WPA,
     *    ENC_TYPE_WPA2,
     *    ENC_TYPE_WPA2_ENTERPRISE,
     *    ENC_TYPE_WPA3,
     *    ENC_TYPE_NONE,
     *    ENC_TYPE_AUTO,
     *    ENC_TYPE_UNKNOWN = 255
     */
    uint8_t encryptionType(uint8_t networkItem);

    uint8_t* BSSID(uint8_t networkItem, uint8_t* bssid);
    uint8_t channel(uint8_t networkItem);

    /*
     * Return the RSSI of the networks discovered during the scanNetworks
     *
     * param networkItem: specify from which network item want to get the information
     *
     * return: signed value of RSSI of the specified item on the networks scanned list
     */
    int32_t RSSI(uint8_t networkItem);


    uint8_t channel();

    /*
     * Return Connection status.
     *
     * return: one of the value defined in wl_status_t
     */
    uint8_t status();

    /*
     * Return The deauthentication reason code.
     *
     * return: the deauthentication reason code
     */
    uint8_t reasonCode();

    /*
     * Resolve the given hostname to an IP address.
     * param aHostname: Name to be resolved
     * param aResult: IPAddress structure to store the returned IP address
     * result: 1 if aIPAddrString was successfully converted to an IP address,
     *          else error code
     */
    int hostByName(const char* aHostname, IPAddress& aResult);

    unsigned long getTime();

    void lowPowerMode();
    void noLowPowerMode();

    void setTimeout(unsigned long timeout);
};

inline CWifi WiFi;

#include "WiFiClient.h"
#include "WiFiServer.h"
#include "WiFiUdp.h"
