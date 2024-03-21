#ifndef ARDUINO_WiFi_S3_H
#define ARDUINO_WiFi_S3_H


#include <vector>

#include "WiFiCommands.h"
#include "WiFiTypes.h"
#include "Modem.h"

#include "WiFiClient.h"
#include "WiFiServer.h"
#include "WiFiUdp.h"
#include "WiFiSSLClient.h"

#define DEFAULT_IP_AP_ADDRESS           IPAddress(192,168,4,1)
#define DEFAULT_GW_AP_ADDRESS           IPAddress(192,168,1,1)
#define DEFAULT_NM_AP_ADDRESS           IPAddress(255,255,255,0)


#define WIFI_FIRMWARE_LATEST_VERSION "0.4.1"

class CAccessPoint {
   public:
      std::string ssid;
      std::string bssid;
      uint8_t uint_bssid[6];
      std::string rssi;
      std::string channel;
      std::string encryption_mode;
};




class CWifi {
private: 
   void _config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, IPAddress dns2);
   unsigned long _timeout;
   std::vector<CAccessPoint> access_points;
   std::string ssid;
   std::string apssid;
   

   IPAddress ip_ap = DEFAULT_IP_AP_ADDRESS;
   IPAddress gw_ap = DEFAULT_GW_AP_ADDRESS;
   IPAddress nm_ap = DEFAULT_NM_AP_ADDRESS;




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

   IPAddress dnsIP(int n = 0);

    /*
     * Get the interface the AP IP address.
     *
     * return: IP address value
     */
    IPAddress softAPIP();

    /*
     * Return the current SSID associated with the network
     *
     * return: ssid string
     */
    const char* SSID();

    /*
      * Return the current BSSID associated with the network.
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
     * Return the current SSID associated with to the soft AP
     *
     * return: ssid string
     */
    const char* softAPSSID();


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

    
      
     */
    uint8_t encryptionType(uint8_t networkItem);
    uint8_t encryptionType();

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

    void setTimeout(unsigned long timeout);

    
};

extern CWifi WiFi;




#endif

