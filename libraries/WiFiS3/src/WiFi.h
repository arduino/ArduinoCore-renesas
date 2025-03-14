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

#define WIFI_FIRMWARE_LATEST_VERSION "0.5.2"

#ifndef WIFI_MAX_SSID_COUNT
  #define WIFI_MAX_SSID_COUNT 10
#endif

class CAccessPoint {
public:
    CAccessPoint() {}
    CAccessPoint(const CAccessPoint &obj)
    {
        strcpy(ssid, obj.ssid);
        rssi = obj.rssi;
        channel = obj.channel;
        encryption_mode = obj.encryption_mode;
        memcpy(uint_bssid, obj.uint_bssid, sizeof(uint_bssid));
    }
    CAccessPoint &operator=(const CAccessPoint &obj) {
        strcpy(ssid, obj.ssid);
        rssi = obj.rssi;
        channel = obj.channel;
        encryption_mode = obj.encryption_mode;
        memcpy(uint_bssid, obj.uint_bssid, sizeof(uint_bssid));
        return *this;
    }
    char ssid[WL_SSID_MAX_LENGTH + 1]; // +1 for null terminator
    uint8_t uint_bssid[6];
    int rssi;
    uint8_t channel;
    uint8_t encryption_mode;
};


/**
 * @brief Class to manage Wi-Fi connectivity and operations.
 * 
 * The CWifi class provides an interface to manage Wi-Fi operations such as connecting 
 * to networks, setting up an access point, retrieving network information, and more. 
 * It interfaces with a modem to execute commands related to Wi-Fi functionality and manages 
 * connection settings such as IP address, DNS, and other network configurations.
 */
class CWifi {
private:
    void _config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, IPAddress dns2);
    void _sortAPlist(uint8_t num);
    unsigned long _timeout;
    CAccessPoint access_points[WIFI_MAX_SSID_COUNT];
    uint8_t _apsFound = 0;
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
    /*
     * Get firmware version U32
     *
     * Since version is made in a semver fashion, thus in an integer it will be represented as
     *  byte 1 (MSB) | byte 2 | byte 3 | byte 4
     *             0 | MAJOR  | MINOR  | PATCH
     */
    uint32_t firmwareVersionU32();

    /*
     * PING
     */
    int ping(IPAddress ip, uint8_t ttl = 128, uint8_t count = 1);
    int ping(const String &hostname, uint8_t ttl = 128, uint8_t count = 1);
    int ping(const char* host, uint8_t ttl = 128, uint8_t count = 1);

    /** 
     * @brief Start WiFi connection for OPEN networks.
     * 
     * @param `ssid` a pointer to the SSID string.
     */
    int begin(const char* ssid);

    /* Start WiFi connection with passphrase
     * the most secure supported mode will be automatically selected
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

    /**
     * @brief Retrieves the MAC address of the device.
     * 
     * This function retrieves the MAC address of the device based on its current operating mode.
     * The value returned by this function is meaningfull only if called afert a 
     * begin (both begin or beginAP) or a ScanNetwork function otherwise 
     * an empty mac address is returned.
     * 
     * @param `_mac` A pointer to a uint8_t array where the MAC address will be stored.
     * 
     * @return uint8_t* The pointer to the array with length WL_MAC_ADDR_LENGTH.
     */
    uint8_t* macAddress(uint8_t* mac);

    /**
     * @brief Retrieves the local IP address of the device.
     * 
     * This function retrieves the local IP address of the device. It checks the current mode (station or soft AP) 
     * and retrieves the appropriate IP address based on the mode.
     * 
     * @return `IPAddress` The local IP address of the device.
     */
    IPAddress localIP();

    /**
     * @brief Retrieves the subnet mask of the device.
     * 
     * This function retrieves the subnet mask of the device by querying the modem for the subnet information. 
     * It sends a command to the modem to fetch the subnet mask and returns it as an `IPAddress` object.
     * 
     * @return `IPAddress` The subnet mask address value of the device.
     */
    IPAddress subnetMask();

   /**
    * @brief Retrieves the gateway IP address of the device.
    * 
    * This function retrieves the gateway IP address of the device by querying the modem for the gateway 
    * information. It sends a command to the modem to fetch the gateway IP address and returns it as an 
    * `IPAddress` object.
    * 
    * @return `IPAddress` The gateway IP address value of the device.
    */
   IPAddress gatewayIP();

   /**
    * @brief Retrieves the DNS IP address.
    * 
    * @param `n` The index of the DNS server to retrieve, `0` for the primary DNS server 
    * and `1` for the secondary DNS server.
    * 
    * @return `IPAddress` The DNS IP address as an `IPAddress` object, or `0.0.0.0` if not found.
    */
   IPAddress dnsIP(int n = 0);

    /**
     * @brief Retrieves the IP address of the soft access point (AP).
     * 
     * @return `IPAddress` The IP address of the soft AP as an `IPAddress` object, or `0.0.0.0` if not found.
     */
    IPAddress softAPIP();

    /**
     * @brief Retrieves the SSID of the current Wi-Fi connection or SoftAP.
     * 
     * @return The SSID of the current Wi-Fi connection or SoftAP as a string.
     * If unable to retrieve the SSID, returns an empty string.
     */
    const char* SSID();

    /**
     * @brief Retrieves the BSSID (MAC address) of the currently connected Wi-Fi network.
     * 
     * @param `bssid` A pointer to an array where the BSSID will be stored. The array must be large enough to hold the MAC address.
     * 
     * @return A pointer to the `bssid` array containing the retrieved BSSID. If the BSSID cannot be retrieved, returns `nullptr`.
     */
    uint8_t* BSSID(uint8_t* bssid);

    /**
     * @brief Retrieves the RSSI (Received Signal Strength Indicator) of the currently connected Wi-Fi network.
     * 
     * @return The RSSI value representing the signal strength. A higher (less negative) value indicates a stronger signal.
     * If the RSSI cannot be retrieved, returns 0.
     */
    int32_t RSSI();

    /**
     * @brief Retrieves the SSID (Service Set Identifier) of the SoftAP (Software Access Point) mode.
     * 
     * @return The SSID of the SoftAP. If the SSID cannot be retrieved, an empty string is returned.
     */
    const char* softAPSSID();

    /**
     * @brief Scans for available Wi-Fi networks and stores the information in the `access_points` list.
     * 
     * This function initiates a scan for nearby Wi-Fi networks and retrieves details such as SSID, BSSID, 
     * RSSI (signal strength), channel, and encryption mode for each detected access point.
     * 
     * @return The number of networks found during the scan. Returns a negative value in case of an error.
     */
    int8_t scanNetworks();

    /**
     * @brief Retrieves the SSID of a specific Wi-Fi network from the scan results.
     * 
     * @param `networkItem` The index of the network in the list of scanned access points.
     * 
     * @return The SSID of the specified network, or `nullptr` if the index is out of bounds.
     */
    const char*   SSID(uint8_t networkItem);

    /**
     * @brief Returns the encryption type for a specified network.
     * 
     * This function retrieves the encryption type of a specific Wi-Fi access point
     * based on its index in the list of scanned networks.
     * 
     * @param `networkItem` The index of the network in the list of available access points.
     * 
     * @return The encryption type in numeric form (e.g., 0 for no encryption, 
     * 1 for WEP, 2 for WPA, etc.). Returns 0 if the network item index is invalid.
     */
    uint8_t encryptionType(uint8_t networkItem);

    /**
     * @brief Returns the encryption type of the currently connected Wi-Fi network.
     * 
     * @return The encryption type in numeric form (e.g., 0 for no encryption, 1 for WEP, 2 for WPA, etc.).
     * Returns `ENC_TYPE_UNKNOWN` if the encryption type cannot be determined.
     */
    uint8_t encryptionType();

    /**
     * @brief Retrieves the BSSID of a specific Wi-Fi network.
     * 
     * This function retrieves the BSSID (MAC address) of the Wi-Fi network at the specified
     * index from the list of available networks. The BSSID is stored in the provided 
     * array of 6 bytes.
     * 
     * @param `networkItem` The index of the Wi-Fi network in the list of available networks.
     * @param `bssid` A pointer to a byte array (of size 6) where the BSSID will be stored.
     * 
     * @return A pointer to the `bssid` array if the BSSID is successfully retrieved, 
     * or `nullptr` if the network index is out of range.
     */
    uint8_t* BSSID(uint8_t networkItem, uint8_t* bssid);

    /**
     * @brief Retrieves the channel number of a specific Wi-Fi network.
     * 
     * @param `networkItem` The index of the Wi-Fi network in the list of available networks.
     * 
     * @return The channel number of the specified network, or `0` if the network index 
     * is out of range.
     */
    uint8_t channel(uint8_t networkItem);

    /**
     * @brief Retrieves the RSSI (Received Signal Strength Indicator) of the networks discovered during the scanNetworks.
     * 
     * @param `networkItem` The index of the Wi-Fi network in the list of available networks.
     * 
     * @return The RSSI value of the specified network in dBm, or `-1000` if the network index 
     * is out of range.
     */
    int32_t RSSI(uint8_t networkItem);

    /**
     * @brief Retrieves the current connection status of the Wi-Fi connection.
     * 
     * @return One of the values defined in wl_status_t
     */
    uint8_t status();

    /**
     * @brief Retrieves The deauthentication reason code.
     *
     * @return The deauthentication reason code.
     */
    uint8_t reasonCode();

    /**
     * @brief Resolves a hostname to an IP address.
     * 
     * @param `aHostname` The hostname to resolve (e.g., "www.example.com").
     * @param `aResult` IPAddress structure to store the returned IP address result: 
     * 1 if aIPAddrString was successfully converted to an IP address, else error code
     * 
     * @return Returns `1` if the hostname was successfully resolved, `0` otherwise.
     */
    int hostByName(const char* aHostname, IPAddress& aResult);

    /**
     * @brief Retrieves the current time from the modem.
     * 
     * @return The current time value in seconds, or `0` if the time could not be retrieved.
     */
    unsigned long getTime();

    /**
     * @brief Sets the timeout value for the WiFi connection.
     * 
     * @param `timeout` The timeout value in milliseconds.
     */
    void setTimeout(unsigned long timeout);


};

/**
 * @brief Global instance of the CWifi class.
 * 
 * This external declaration provides access to a global instance of the `CWifi` class, which 
 * facilitates interaction with the WiFi module.
 */
extern CWifi WiFi;




#endif

