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
#define WL_MAC_ADDR_LENGTH 6

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
    /**
     * @brief Default constructor for the CWifi class.
     */
    CWifi();

    /**
     * @brief Get firmware version
     */
    static const char* firmwareVersion();

    /** 
     * @brief Start WiFi connection for OPEN networks.
     * 
     * @param `ssid` a pointer to the SSID string.
     */
    int begin(const char* ssid);

    /** 
     * @brief start WiFi connection with passphrase the most secure
     * supported mode will be automatically selected.
     *
     * @param `ssid` Pointer to the SSID string.
     * @param `passphrase` Passphrase. Valid characters in a passphrase
     * must be between ASCII 32-126 (decimal).
     */
    int begin(const char* ssid, const char *passphrase);

    /**
     * @brief Starts a Wi-Fi Access Point with the specified SSID.
     *
     * This function initializes a Wi-Fi Access Point (AP) with the given SSID.
     * It defaults to an open network (no password) and uses channel 1 for the AP.
     *
     * @param `ssid` The SSID (network name) of the Access Point.
     * 
     * @return `1` if the Access Point is successfully started. `0` if the Access Point initialization failed.
     */
    uint8_t beginAP(const char *ssid);
    uint8_t beginAP(const char *ssid);

    /**
     * @brief Starts a Wi-Fi Access Point (AP) with the specified SSID and channel.
     *
     * This function initializes a Wi-Fi Access Point (AP) with the provided SSID 
     * and channel. It defaults to using no password (open network).
     *
     * @param `ssid` The SSID (name) of the Wi-Fi Access Point.
     * @param `channel` The channel on which the Access Point will operate.
     * 
     * @return `1` if the Access Point is successfully started. `0` if the Access Point failed to start.
     */
    uint8_t beginAP(const char *ssid, uint8_t channel);

    /**
     * @brief Starts a Wi-Fi Access Point (AP) with the specified SSID and passphrase.
     *
     * This function initializes a Wi-Fi Access Point (AP) using the provided SSID 
     * and passphrase, defaulting to channel 1.
     *
     * @param `ssid` The SSID (name) of the Wi-Fi Access Point.
     * @param `passphrase` The passphrase for securing the Access Point. If empty, the 
     * network will be open (no password).
     * 
     * @return `1` if the Access Point is successfully started. `0` if the Access Point failed to start.
     */
    uint8_t beginAP(const char *ssid, const char* passphrase);

    /**
     * @brief Initializes a Wi-Fi Access Point (AP) with the specified SSID, passphrase, and channel.
     *
     * This function configures the device as a Wi-Fi Access Point (AP) with the provided parameters. 
     * It sets the mode to AP and attempts to start the network.
     *
     * @param `ssid` The SSID (name) of the Wi-Fi Access Point.
     * @param `passphrase` The passphrase for securing the Access Point. If empty, the network will be open.
     * @param `channel` The Wi-Fi channel on which the Access Point will operate (1-14, depending on region).
     * 
     * @return `WL_AP_LISTENING` if the Access Point is successfully started. `WL_AP_FAILED` if the Access Point failed to start.
     */
    uint8_t beginAP(const char *ssid, const char* passphrase, uint8_t channel);


    /**
     * @brief Change IP configuration settings disabling the DHCP client
     *
     * @param `local_ip` The static IP address to assign to the device.
     */
    void config(IPAddress local_ip);

    /**
     * @brief Configures the network settings for the Wi-Fi connection with a specified DNS server.
     *
     * Sets the device's IP configuration using the specified local IP address and DNS server.
     * The gateway and subnet mask are set to default values based on the provided local IP.
     *
     * @param `local_ip` The static IP address to assign to the device.
     * @param `dns_server` The primary DNS server to use for domain name resolution.
     */
    void config(IPAddress local_ip, IPAddress dns_server);

    /**
     * @brief Configures the network settings for the Wi-Fi connection with a specified gateway and DNS server.
     *
     *
     * @param `local_ip` The static IP address to assign to the device.
     * @param `dns_server` The primary DNS server to use for domain name resolution.
     * @param `gateway` The Static gateway used for configuration.
     */
    void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway);

    /**
     * @brief Configures the network settings for the Wi-Fi connection with a specified subnet mask, gateway, and DNS server.
     *
     *
     * @param `local_ip` The static IP address to assign to the device.
     * @param `dns_server` The primary DNS server to use for domain name resolution.
     * @param `gateway` The static gateway used for configuration.
     * @param `subnet` The static subnet mask to use for the network.
     */
    void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);
    
    /**
     * @brief Sets the primary DNS server for the Wi-Fi connection.
     *
     * @param `dns_server1` The primary DNS server to use for domain name resolution.
     */
    void setDNS(IPAddress dns_server1);

    /**
     * @brief Sets the primary and secondary DNS servers for the Wi-Fi connection.
     *
     * @param `dns_server1` sets the IP configuration for DNS server 1
     * @param `dns_server2` sets the IP configuration for DNS server 2
     */
    void setDNS(IPAddress dns_server1, IPAddress dns_server2);

    /**
     * @brief Sets the hostname for for DHCP requests.
     *
     * @param `name` sets the hostname.
     */
    void setHostname(const char* name);

    /**
     * @brief Disconnects from the current Wi-Fi network.
     *
     * @return `1` if the disconnection was successful, `0` otherwise.
     */
    int disconnect(void);

    /**
     * @brief Resets and disables the Wi-Fi module.
     */
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

