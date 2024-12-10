# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class ` [`CAccessPoint`](#class_c_access_point) | 
`class ` [`CWifi`](#class_c_wifi) | Class to manage Wi-Fi connectivity and operations.
`class ` [`ModemClass`](#class_modem_class) | A class that provides methods to interact with a modem.
`class ` [`WiFiClient`](#class_wi_fi_client) | Represents a Wi-Fi client that connects to a remote server over a Wi-Fi network.
`class ` [`WiFiFileSystem`](#class_wi_fi_file_system) | Class that handles the WiFi file system operations.
`class ` [`WiFiServer`](#class_wi_fi_server) | A class that provides server functionality for WiFi-based communication.
`class ` [`WiFiSSLClient`](#class_wi_fi_s_s_l_client) | A specialized client class for secure SSL/TLS connections.
`class ` [`WiFiUDP`](#class_wi_fi_u_d_p) | A class for handling UDP communication over a Wi-Fi network.

# class `CAccessPoint` <a id="class_c_access_point" class="anchor"></a>

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`ssid`](#class_c_access_point_1ac0161f0bae3c82f5caed8b05898fc49e) |  |
| [`bssid`](#class_c_access_point_1ad1bc1d6e1bee842a46ffdc408fa59947) |  |
| [`uint_bssid`](#class_c_access_point_1a5cf478e57a7138a1490b231b2aeb9c3e) |  |
| [`rssi`](#class_c_access_point_1a7b5640ed8c2ed9f62e38051ffe8fe6a0) |  |
| [`channel`](#class_c_access_point_1aa95219f17673f68cf3b1c6e17345b0e4) |  |
| [`encryption_mode`](#class_c_access_point_1ac293ee77db560b0cda65d91374249412) |  |

## Members

### `ssid` <a id="class_c_access_point_1ac0161f0bae3c82f5caed8b05898fc49e" class="anchor"></a>

```cpp
std::string ssid
```

<hr />

### `bssid` <a id="class_c_access_point_1ad1bc1d6e1bee842a46ffdc408fa59947" class="anchor"></a>

```cpp
std::string bssid
```

<hr />

### `uint_bssid` <a id="class_c_access_point_1a5cf478e57a7138a1490b231b2aeb9c3e" class="anchor"></a>

```cpp
uint8_t uint_bssid
```

<hr />

### `rssi` <a id="class_c_access_point_1a7b5640ed8c2ed9f62e38051ffe8fe6a0" class="anchor"></a>

```cpp
std::string rssi
```

<hr />

### `channel` <a id="class_c_access_point_1aa95219f17673f68cf3b1c6e17345b0e4" class="anchor"></a>

```cpp
std::string channel
```

<hr />

### `encryption_mode` <a id="class_c_access_point_1ac293ee77db560b0cda65d91374249412" class="anchor"></a>

```cpp
std::string encryption_mode
```

<hr />

# class `CWifi` <a id="class_c_wifi" class="anchor"></a>

Class to manage Wi-Fi connectivity and operations.

The [CWifi](#class_c_wifi) class provides an interface to manage Wi-Fi operations such as connecting to networks, setting up an access point, retrieving network information, and more. It interfaces with a modem to execute commands related to Wi-Fi functionality and manages connection settings such as IP address, DNS, and other network configurations.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`CWifi`](#class_c_wifi_1a64b74dbe44a73d1b7fd4618b5b606c02) | Default constructor for the [CWifi](#class_c_wifi) class. |
| [`begin`](#class_c_wifi_1a9c4fc0f23a197e9dc5ddf02bc1793c35) | Start WiFi connection for OPEN networks. |
| [`begin`](#class_c_wifi_1a4a2bf8820b00494648188c7c06871a26) | start WiFi connection with passphrase the most secure supported mode will be automatically selected. |
| [`beginAP`](#class_c_wifi_1adeaffb4fe03faf22e7f69697d5e83492) |  |
| [`beginAP`](#class_c_wifi_1ae9059d5f875a96b5cdf19d2cf87e9848) | Starts a Wi-Fi Access Point (AP) with the specified SSID and channel. |
| [`beginAP`](#class_c_wifi_1aee76c2fbb9f83a3f085b034e80c1025c) | Starts a Wi-Fi Access Point (AP) with the specified SSID and passphrase. |
| [`beginAP`](#class_c_wifi_1a9e1257e9b28cd3489209d7c7d5ea8845) | Initializes a Wi-Fi Access Point (AP) with the specified SSID, passphrase, and channel. |
| [`config`](#class_c_wifi_1a75742aab24859611b36e74e08602dc47) | Change IP configuration settings disabling the DHCP client. |
| [`config`](#class_c_wifi_1ac799224e1f9c65e4808e2c8deb802ead) | Configures the network settings for the Wi-Fi connection with a specified DNS server. |
| [`config`](#class_c_wifi_1acd5a6e9bb901fb4dbaec311378f99eb6) | Configures the network settings for the Wi-Fi connection with a specified gateway and DNS server. |
| [`config`](#class_c_wifi_1afc80a6fd238b6d0e59aebb2d27ec8e9e) | Configures the network settings for the Wi-Fi connection with a specified subnet mask, gateway, and DNS server. |
| [`setDNS`](#class_c_wifi_1a4043b10b6a83cb3add95e127c79056a0) | Sets the primary DNS server for the Wi-Fi connection. |
| [`setDNS`](#class_c_wifi_1adaa95a2dd3a8bab54080669e612f9d37) | Sets the primary and secondary DNS servers for the Wi-Fi connection. |
| [`setHostname`](#class_c_wifi_1aece8d82cb1f1a55c0c22a606d66f8ea7) | Sets the hostname for for DHCP requests. |
| [`disconnect`](#class_c_wifi_1a59591cd8450c695f3588df818fb58d34) | Disconnects from the current Wi-Fi network. |
| [`end`](#class_c_wifi_1a50a8e7ca94f40d31ea5c9b2def045575) | Resets and disables the Wi-Fi module. |
| [`macAddress`](#class_c_wifi_1a58ebcdba481cc413f32f06226dba8481) | Retrieves the MAC address of the device. |
| [`localIP`](#class_c_wifi_1ae6885ef712d13bfcfe54a23f28b2aecb) | Retrieves the local IP address of the device. |
| [`subnetMask`](#class_c_wifi_1afcdd6b286e90258b5296c8f30f4e7c81) | Retrieves the subnet mask of the device. |
| [`gatewayIP`](#class_c_wifi_1a775e014fc255686df09b1f40f6cb9453) | Retrieves the gateway IP address of the device. |
| [`dnsIP`](#class_c_wifi_1a3ee65c20906927f5772b96386d6b5700) | Retrieves the DNS IP address. |
| [`softAPIP`](#class_c_wifi_1a6e64918a2784cb53588f1553c97dc333) | Retrieves the IP address of the soft access point (AP). |
| [`SSID`](#class_c_wifi_1a46c576561933d9f067121b81ab8b3806) | Retrieves the SSID of the current Wi-Fi connection or SoftAP. |
| [`BSSID`](#class_c_wifi_1a9b153ec8de3bd6e95255e94140314ad2) | Retrieves the BSSID (MAC address) of the currently connected Wi-Fi network. |
| [`RSSI`](#class_c_wifi_1a39a8f1b67aee1a1f52d8c0f2085ebb52) | Retrieves the RSSI (Received Signal Strength Indicator) of the currently connected Wi-Fi network. |
| [`softAPSSID`](#class_c_wifi_1accf0212b364b4686a4d47f7e6f58485b) | Retrieves the SSID (Service Set Identifier) of the SoftAP (Software Access Point) mode. |
| [`scanNetworks`](#class_c_wifi_1af31d8a0baaa568178e2091b7a9e830a1) | Scans for available Wi-Fi networks and stores the information in the `access_points` list. |
| [`SSID`](#class_c_wifi_1a452fefff75ab36097d6f4b6ad401ffcb) | Retrieves the SSID of a specific Wi-Fi network from the scan results. |
| [`encryptionType`](#class_c_wifi_1ab434b47d2935bf6d0b58ec50d78c3799) | Returns the encryption type for a specified network. |
| [`encryptionType`](#class_c_wifi_1a8da9fb03f5349f7887d3cdda3268ed8c) | Returns the encryption type of the currently connected Wi-Fi network. |
| [`BSSID`](#class_c_wifi_1a10793238a64c312f2652b710c693022e) | Retrieves the BSSID of a specific Wi-Fi network. |
| [`channel`](#class_c_wifi_1a871866aab35d3c10b98e27e51b37d8b6) | Retrieves the channel number of a specific Wi-Fi network. |
| [`RSSI`](#class_c_wifi_1a86ecf1c49591c18296a6d1b1400954e5) | Retrieves the RSSI (Received Signal Strength Indicator) of the networks discovered during the scanNetworks. |
| [`status`](#class_c_wifi_1aeeddb0fe0a897d043415a97d363f68e0) | Retrieves the current connection status of the Wi-Fi connection. |
| [`reasonCode`](#class_c_wifi_1a453afce0d716bb1b92c3d73ba4f621e4) | Retrieves The deauthentication reason code. |
| [`hostByName`](#class_c_wifi_1a97ef95bf4e58c3c11a88ab03a4702425) | Resolves a hostname to an IP address. |
| [`getTime`](#class_c_wifi_1a9c3f3a794a063c20b9ef81851226a625) | Retrieves the current time from the modem. |
| [`setTimeout`](#class_c_wifi_1a53250475083a7537de61d2fc14217345) | Sets the timeout value for the WiFi connection. |
| [`firmwareVersion`](#class_c_wifi_1a4b2746f36f799c1e075d8f083b8bd6ee) | Get firmware version. |

## Members

### `CWifi` <a id="class_c_wifi_1a64b74dbe44a73d1b7fd4618b5b606c02" class="anchor"></a>

```cpp
CWifi()
```

Default constructor for the [CWifi](#class_c_wifi) class.

<hr />

### `begin` <a id="class_c_wifi_1a9c4fc0f23a197e9dc5ddf02bc1793c35" class="anchor"></a>

```cpp
int begin(const char * ssid)
```

Start WiFi connection for OPEN networks.

#### Parameters
* `ssid` a pointer to the SSID string.
<hr />

### `begin` <a id="class_c_wifi_1a4a2bf8820b00494648188c7c06871a26" class="anchor"></a>

```cpp
int begin(const char * ssid, const char * passphrase)
```

start WiFi connection with passphrase the most secure supported mode will be automatically selected.

#### Parameters
* `ssid` Pointer to the SSID string. 

* `passphrase` Passphrase. Valid characters in a passphrase must be between ASCII 32-126 (decimal).
<hr />

### `beginAP` <a id="class_c_wifi_1adeaffb4fe03faf22e7f69697d5e83492" class="anchor"></a>

```cpp
uint8_t beginAP(const char * ssid)
```

<hr />

### `beginAP` <a id="class_c_wifi_1ae9059d5f875a96b5cdf19d2cf87e9848" class="anchor"></a>

```cpp
uint8_t beginAP(const char * ssid, uint8_t channel)
```

Starts a Wi-Fi Access Point (AP) with the specified SSID and channel.

This function initializes a Wi-Fi Access Point (AP) with the provided SSID and channel. It defaults to using no password (open network).

#### Parameters
* `ssid` The SSID (name) of the Wi-Fi Access Point. 

* `channel` The channel on which the Access Point will operate.

#### Returns
`1` if the Access Point is successfully started. `0` if the Access Point failed to start.
<hr />

### `beginAP` <a id="class_c_wifi_1aee76c2fbb9f83a3f085b034e80c1025c" class="anchor"></a>

```cpp
uint8_t beginAP(const char * ssid, const char * passphrase)
```

Starts a Wi-Fi Access Point (AP) with the specified SSID and passphrase.

This function initializes a Wi-Fi Access Point (AP) using the provided SSID and passphrase, defaulting to channel 1.

#### Parameters
* `ssid` The SSID (name) of the Wi-Fi Access Point. 

* `passphrase` The passphrase for securing the Access Point. If empty, the network will be open (no password).

#### Returns
`1` if the Access Point is successfully started. `0` if the Access Point failed to start.
<hr />

### `beginAP` <a id="class_c_wifi_1a9e1257e9b28cd3489209d7c7d5ea8845" class="anchor"></a>

```cpp
uint8_t beginAP(const char * ssid, const char * passphrase, uint8_t channel)
```

Initializes a Wi-Fi Access Point (AP) with the specified SSID, passphrase, and channel.

This function configures the device as a Wi-Fi Access Point (AP) with the provided parameters. It sets the mode to AP and attempts to start the network.

#### Parameters
* `ssid` The SSID (name) of the Wi-Fi Access Point. 

* `passphrase` The passphrase for securing the Access Point. If empty, the network will be open. 

* `channel` The Wi-Fi channel on which the Access Point will operate (1-14, depending on region).

#### Returns
`WL_AP_LISTENING` if the Access Point is successfully started. `WL_AP_FAILED` if the Access Point failed to start.
<hr />

### `config` <a id="class_c_wifi_1a75742aab24859611b36e74e08602dc47" class="anchor"></a>

```cpp
void config(IPAddress local_ip)
```

Change IP configuration settings disabling the DHCP client.

#### Parameters
* `local_ip` The static IP address to assign to the device.
<hr />

### `config` <a id="class_c_wifi_1ac799224e1f9c65e4808e2c8deb802ead" class="anchor"></a>

```cpp
void config(IPAddress local_ip, IPAddress dns_server)
```

Configures the network settings for the Wi-Fi connection with a specified DNS server.

Sets the device's IP configuration using the specified local IP address and DNS server. The gateway and subnet mask are set to default values based on the provided local IP.

#### Parameters
* `local_ip` The static IP address to assign to the device. 

* `dns_server` The primary DNS server to use for domain name resolution.
<hr />

### `config` <a id="class_c_wifi_1acd5a6e9bb901fb4dbaec311378f99eb6" class="anchor"></a>

```cpp
void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
```

Configures the network settings for the Wi-Fi connection with a specified gateway and DNS server.

#### Parameters
* `local_ip` The static IP address to assign to the device. 

* `dns_server` The primary DNS server to use for domain name resolution. 

* `gateway` The Static gateway used for configuration.
<hr />

### `config` <a id="class_c_wifi_1afc80a6fd238b6d0e59aebb2d27ec8e9e" class="anchor"></a>

```cpp
void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
```

Configures the network settings for the Wi-Fi connection with a specified subnet mask, gateway, and DNS server.

#### Parameters
* `local_ip` The static IP address to assign to the device. 

* `dns_server` The primary DNS server to use for domain name resolution. 

* `gateway` The static gateway used for configuration. 

* `subnet` The static subnet mask to use for the network.
<hr />

### `setDNS` <a id="class_c_wifi_1a4043b10b6a83cb3add95e127c79056a0" class="anchor"></a>

```cpp
void setDNS(IPAddress dns_server1)
```

Sets the primary DNS server for the Wi-Fi connection.

#### Parameters
* `dns_server1` The primary DNS server to use for domain name resolution.
<hr />

### `setDNS` <a id="class_c_wifi_1adaa95a2dd3a8bab54080669e612f9d37" class="anchor"></a>

```cpp
void setDNS(IPAddress dns_server1, IPAddress dns_server2)
```

Sets the primary and secondary DNS servers for the Wi-Fi connection.

#### Parameters
* `dns_server1` sets the IP configuration for DNS server 1 

* `dns_server2` sets the IP configuration for DNS server 2
<hr />

### `setHostname` <a id="class_c_wifi_1aece8d82cb1f1a55c0c22a606d66f8ea7" class="anchor"></a>

```cpp
void setHostname(const char * name)
```

Sets the hostname for for DHCP requests.

#### Parameters
* `name` sets the hostname.
<hr />

### `disconnect` <a id="class_c_wifi_1a59591cd8450c695f3588df818fb58d34" class="anchor"></a>

```cpp
int disconnect(void)
```

Disconnects from the current Wi-Fi network.

#### Returns
`1` if the disconnection was successful, `0` otherwise.
<hr />

### `end` <a id="class_c_wifi_1a50a8e7ca94f40d31ea5c9b2def045575" class="anchor"></a>

```cpp
void end(void)
```

Resets and disables the Wi-Fi module.

<hr />

### `macAddress` <a id="class_c_wifi_1a58ebcdba481cc413f32f06226dba8481" class="anchor"></a>

```cpp
uint8_t * macAddress(uint8_t * mac)
```

Retrieves the MAC address of the device.

This function retrieves the MAC address of the device based on its current operating mode. The value returned by this function is meaningfull only if called afert a begin (both begin or beginAP) or a ScanNetwork function otherwise an empty mac address is returned.

#### Parameters
* `_mac` A pointer to a uint8_t array where the MAC address will be stored.

#### Returns
uint8_t* The pointer to the array containing the MAC address.
<hr />

### `localIP` <a id="class_c_wifi_1ae6885ef712d13bfcfe54a23f28b2aecb" class="anchor"></a>

```cpp
IPAddress localIP()
```

Retrieves the local IP address of the device.

This function retrieves the local IP address of the device. It checks the current mode (station or soft AP) and retrieves the appropriate IP address based on the mode.

#### Returns
`IPAddress` The local IP address of the device.
<hr />

### `subnetMask` <a id="class_c_wifi_1afcdd6b286e90258b5296c8f30f4e7c81" class="anchor"></a>

```cpp
IPAddress subnetMask()
```

Retrieves the subnet mask of the device.

This function retrieves the subnet mask of the device by querying the modem for the subnet information. It sends a command to the modem to fetch the subnet mask and returns it as an `IPAddress` object.

#### Returns
`IPAddress` The subnet mask address value of the device.
<hr />

### `gatewayIP` <a id="class_c_wifi_1a775e014fc255686df09b1f40f6cb9453" class="anchor"></a>

```cpp
IPAddress gatewayIP()
```

Retrieves the gateway IP address of the device.

This function retrieves the gateway IP address of the device by querying the modem for the gateway information. It sends a command to the modem to fetch the gateway IP address and returns it as an `IPAddress` object.

#### Returns
`IPAddress` The gateway IP address value of the device.
<hr />

### `dnsIP` <a id="class_c_wifi_1a3ee65c20906927f5772b96386d6b5700" class="anchor"></a>

```cpp
IPAddress dnsIP(int n)
```

Retrieves the DNS IP address.

#### Parameters
* `n` The index of the DNS server to retrieve, `0` for the primary DNS server and `1` for the secondary DNS server.

#### Returns
`IPAddress` The DNS IP address as an `IPAddress` object, or `0.0.0.0` if not found.
<hr />

### `softAPIP` <a id="class_c_wifi_1a6e64918a2784cb53588f1553c97dc333" class="anchor"></a>

```cpp
IPAddress softAPIP()
```

Retrieves the IP address of the soft access point (AP).

#### Returns
`IPAddress` The IP address of the soft AP as an `IPAddress` object, or `0.0.0.0` if not found.
<hr />

### `SSID` <a id="class_c_wifi_1a46c576561933d9f067121b81ab8b3806" class="anchor"></a>

```cpp
const char * SSID()
```

Retrieves the SSID of the current Wi-Fi connection or SoftAP.

#### Returns
The SSID of the current Wi-Fi connection or SoftAP as a string. If unable to retrieve the SSID, returns an empty string.
<hr />

### `BSSID` <a id="class_c_wifi_1a9b153ec8de3bd6e95255e94140314ad2" class="anchor"></a>

```cpp
uint8_t * BSSID(uint8_t * bssid)
```

Retrieves the BSSID (MAC address) of the currently connected Wi-Fi network.

#### Parameters
* `bssid` A pointer to an array where the BSSID will be stored. The array must be large enough to hold the MAC address.

#### Returns
A pointer to the `bssid` array containing the retrieved BSSID. If the BSSID cannot be retrieved, returns `nullptr`.
<hr />

### `RSSI` <a id="class_c_wifi_1a39a8f1b67aee1a1f52d8c0f2085ebb52" class="anchor"></a>

```cpp
int32_t RSSI()
```

Retrieves the RSSI (Received Signal Strength Indicator) of the currently connected Wi-Fi network.

#### Returns
The RSSI value representing the signal strength. A higher (less negative) value indicates a stronger signal. If the RSSI cannot be retrieved, returns 0.
<hr />

### `softAPSSID` <a id="class_c_wifi_1accf0212b364b4686a4d47f7e6f58485b" class="anchor"></a>

```cpp
const char * softAPSSID()
```

Retrieves the SSID (Service Set Identifier) of the SoftAP (Software Access Point) mode.

#### Returns
The SSID of the SoftAP. If the SSID cannot be retrieved, an empty string is returned.
<hr />

### `scanNetworks` <a id="class_c_wifi_1af31d8a0baaa568178e2091b7a9e830a1" class="anchor"></a>

```cpp
int8_t scanNetworks()
```

Scans for available Wi-Fi networks and stores the information in the `access_points` list.

This function initiates a scan for nearby Wi-Fi networks and retrieves details such as SSID, BSSID, RSSI (signal strength), channel, and encryption mode for each detected access point.

#### Returns
The number of networks found during the scan. Returns a negative value in case of an error.
<hr />

### `SSID` <a id="class_c_wifi_1a452fefff75ab36097d6f4b6ad401ffcb" class="anchor"></a>

```cpp
const char * SSID(uint8_t networkItem)
```

Retrieves the SSID of a specific Wi-Fi network from the scan results.

#### Parameters
* `networkItem` The index of the network in the list of scanned access points.

#### Returns
The SSID of the specified network, or `nullptr` if the index is out of bounds.
<hr />

### `encryptionType` <a id="class_c_wifi_1ab434b47d2935bf6d0b58ec50d78c3799" class="anchor"></a>

```cpp
uint8_t encryptionType(uint8_t networkItem)
```

Returns the encryption type for a specified network.

This function retrieves the encryption type of a specific Wi-Fi access point based on its index in the list of scanned networks.

#### Parameters
* `networkItem` The index of the network in the list of available access points.

#### Returns
The encryption type in numeric form (e.g., 0 for no encryption, 1 for WEP, 2 for WPA, etc.). Returns 0 if the network item index is invalid.
<hr />

### `encryptionType` <a id="class_c_wifi_1a8da9fb03f5349f7887d3cdda3268ed8c" class="anchor"></a>

```cpp
uint8_t encryptionType()
```

Returns the encryption type of the currently connected Wi-Fi network.

#### Returns
The encryption type in numeric form (e.g., 0 for no encryption, 1 for WEP, 2 for WPA, etc.). Returns `ENC_TYPE_UNKNOWN` if the encryption type cannot be determined.
<hr />

### `BSSID` <a id="class_c_wifi_1a10793238a64c312f2652b710c693022e" class="anchor"></a>

```cpp
uint8_t * BSSID(uint8_t networkItem, uint8_t * bssid)
```

Retrieves the BSSID of a specific Wi-Fi network.

This function retrieves the BSSID (MAC address) of the Wi-Fi network at the specified index from the list of available networks. The BSSID is stored in the provided array of 6 bytes.

#### Parameters
* `networkItem` The index of the Wi-Fi network in the list of available networks. 

* `bssid` A pointer to a byte array (of size 6) where the BSSID will be stored.

#### Returns
A pointer to the `bssid` array if the BSSID is successfully retrieved, or `nullptr` if the network index is out of range.
<hr />

### `channel` <a id="class_c_wifi_1a871866aab35d3c10b98e27e51b37d8b6" class="anchor"></a>

```cpp
uint8_t channel(uint8_t networkItem)
```

Retrieves the channel number of a specific Wi-Fi network.

#### Parameters
* `networkItem` The index of the Wi-Fi network in the list of available networks.

#### Returns
The channel number of the specified network, or `0` if the network index is out of range.
<hr />

### `RSSI` <a id="class_c_wifi_1a86ecf1c49591c18296a6d1b1400954e5" class="anchor"></a>

```cpp
int32_t RSSI(uint8_t networkItem)
```

Retrieves the RSSI (Received Signal Strength Indicator) of the networks discovered during the scanNetworks.

#### Parameters
* `networkItem` The index of the Wi-Fi network in the list of available networks.

#### Returns
The RSSI value of the specified network in dBm, or `-1000` if the network index is out of range.
<hr />

### `status` <a id="class_c_wifi_1aeeddb0fe0a897d043415a97d363f68e0" class="anchor"></a>

```cpp
uint8_t status()
```

Retrieves the current connection status of the Wi-Fi connection.

#### Returns
One of the values defined in wl_status_t
<hr />

### `reasonCode` <a id="class_c_wifi_1a453afce0d716bb1b92c3d73ba4f621e4" class="anchor"></a>

```cpp
uint8_t reasonCode()
```

Retrieves The deauthentication reason code.

#### Returns
The deauthentication reason code.
<hr />

### `hostByName` <a id="class_c_wifi_1a97ef95bf4e58c3c11a88ab03a4702425" class="anchor"></a>

```cpp
int hostByName(const char * aHostname, IPAddress & aResult)
```

Resolves a hostname to an IP address.

#### Parameters
* `aHostname` The hostname to resolve (e.g., "www.example.com"). 

* `aResult` IPAddress structure to store the returned IP address result: 1 if aIPAddrString was successfully converted to an IP address, else error code

#### Returns
Returns `1` if the hostname was successfully resolved, `0` otherwise.
<hr />

### `getTime` <a id="class_c_wifi_1a9c3f3a794a063c20b9ef81851226a625" class="anchor"></a>

```cpp
unsigned long getTime()
```

Retrieves the current time from the modem.

#### Returns
The current time value in seconds, or `0` if the time could not be retrieved.
<hr />

### `setTimeout` <a id="class_c_wifi_1a53250475083a7537de61d2fc14217345" class="anchor"></a>

```cpp
void setTimeout(unsigned long timeout)
```

Sets the timeout value for the WiFi connection.

#### Parameters
* `timeout` The timeout value in milliseconds.
<hr />

### `firmwareVersion` <a id="class_c_wifi_1a4b2746f36f799c1e075d8f083b8bd6ee" class="anchor"></a>

```cpp
static const char * firmwareVersion()
```

Get firmware version.

<hr />

# class `ModemClass` <a id="class_modem_class" class="anchor"></a>

A class that provides methods to interact with a modem.

This class is responsible for providing an interface to communicate with a modem through serial communication. It includes methods for initialization, sending and receiving data, and handling modem configurations.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`beginned`](#class_modem_class_1ad21db7f5eb0b759c9b6c02af03b8f964) | Flag indicating whether the system has been initialized. |
| [`enable_dbg`](#class_modem_class_1a77cc6948341d08f0285233c67ef1e928) |  |
| [`ModemClass`](#class_modem_class_1acce8d910277f180f32607ed8e84f090b) | Constructor for the [ModemClass](#class_modem_class), which initializes the modem with the specified transmit (TX) and receive (RX) pins. |
| [`ModemClass`](#class_modem_class_1af222286fa398b86d02bd6230e0c42039) | Constructor for the [ModemClass](#class_modem_class), which initializes the modem with the specified UART interface. |
| [`~ModemClass`](#class_modem_class_1a458f1f4e43e27fdc522caaf4c40d9579) | Destructor for [ModemClass](#class_modem_class). |
| [`begin`](#class_modem_class_1a36c32d41d4111e5fc63d8bf1a017368f) | Initializes the modem communication with a specified baud rate. |
| [`end`](#class_modem_class_1a434e5c4b8c67d90c10eff5e20da56556) | Ends the modem communication. |
| [`write`](#class_modem_class_1acd102543a8544be0686128fea53c9a11) | Sends a formatted command string to a device and stores the response. |
| [`write_nowait`](#class_modem_class_1aa9923ac664a16c2aaf5024de9de1ef03) | Used to send a command to the modem without waiting for a response. |
| [`passthrough`](#class_modem_class_1aa8750f17b14472819ffb8016118f869b) | Sends binary data directly to the modem without any processing or interpretation. |
| [`avoid_trim_results`](#class_modem_class_1ac7aff51f7bb09bbfd7190af1715274f8) | Disables automatic trimming of results for one operation. |
| [`read_using_size`](#class_modem_class_1a83d1105d39191e2c902331b3f0c4c8f5) | Enables a specific mode of reading where the size of the data to be read is considered for processing. |
| [`debug`](#class_modem_class_1a6e6c472f405698e25db0d90a1de359f4) |  |
| [`noDebug`](#class_modem_class_1a0b73ad5e52ad405a3be764d948cd8522) | Used to disable debugging output for the modem communication. |
| [`debug`](#class_modem_class_1a0d2808ffeccd83db3e26cc3568e33eeb) |  |
| [`timeout`](#class_modem_class_1a240fc0451c5152615f192b675beabadc) | Sets the timeout value for communication operations. |

## Members

### `beginned` <a id="class_modem_class_1ad21db7f5eb0b759c9b6c02af03b8f964" class="anchor"></a>

```cpp
bool beginned
```

Flag indicating whether the system has been initialized.

<hr />

### `enable_dbg` <a id="class_modem_class_1a77cc6948341d08f0285233c67ef1e928" class="anchor"></a>

```cpp
bool enable_dbg
```

<hr />

### `ModemClass` <a id="class_modem_class_1acce8d910277f180f32607ed8e84f090b" class="anchor"></a>

```cpp
ModemClass(int tx, int rx)
```

Constructor for the [ModemClass](#class_modem_class), which initializes the modem with the specified transmit (TX) and receive (RX) pins.

#### Parameters
* `Initializes` an instance of the [ModemClass](#class_modem_class) class with specific transmit `tx` and receive `rx` pins for communication.
<hr />

### `ModemClass` <a id="class_modem_class_1af222286fa398b86d02bd6230e0c42039" class="anchor"></a>

```cpp
ModemClass(UART * _serial)
```

Constructor for the [ModemClass](#class_modem_class), which initializes the modem with the specified UART interface.

#### Parameters
* `_serial` is a pointer to the UART object that will be used for communication with the modem.
<hr />

### `~ModemClass` <a id="class_modem_class_1a458f1f4e43e27fdc522caaf4c40d9579" class="anchor"></a>

```cpp
~ModemClass()
```

Destructor for [ModemClass](#class_modem_class).

<hr />

### `begin` <a id="class_modem_class_1a36c32d41d4111e5fc63d8bf1a017368f" class="anchor"></a>

```cpp
void begin(int badurate)
```

Initializes the modem communication with a specified baud rate.

#### Parameters
* `badurate` sets the baud rate for the serial connection.
<hr />

### `end` <a id="class_modem_class_1a434e5c4b8c67d90c10eff5e20da56556" class="anchor"></a>

```cpp
void end()
```

Ends the modem communication.

<hr />

### `write` <a id="class_modem_class_1acd102543a8544be0686128fea53c9a11" class="anchor"></a>

```cpp
bool write(const std::string & cmd, std::string & str, const char * fmt, ...)
```

Sends a formatted command string to a device and stores the response.

This function formats a command string using the provided format and arguments, sends it to a device, and waits for a response, which is stored in the `str` string.

#### Parameters
* `cmd` A string representing the command to be sent to the device. 

* `str` A reference to a string that will hold the device's response. 

* `fmt` A format string for constructing the command.

#### Returns
`true` if the command was successfully sent and a response was received, `false` otherwise.
<hr />

### `write_nowait` <a id="class_modem_class_1aa9923ac664a16c2aaf5024de9de1ef03" class="anchor"></a>

```cpp
void write_nowait(const std::string & cmd, std::string & str, const char * fmt, ...)
```

Used to send a command to the modem without waiting for a response.

#### Parameters
* `It` takes a command string `cmd`, a string `str` where the response will be stored, and a format string `fmt` along with additional arguments.
<hr />

### `passthrough` <a id="class_modem_class_1aa8750f17b14472819ffb8016118f869b" class="anchor"></a>

```cpp
bool passthrough(const uint8_t * data, size_t size)
```

Sends binary data directly to the modem without any processing or interpretation.

#### Parameters
* `It` takes a pointer to the binary `data` and the `size` of the data as arguments. Used for sending raw binary commands or data to the modem for operations that require direct communication without any additional formatting or parsing.
<hr />

### `avoid_trim_results` <a id="class_modem_class_1ac7aff51f7bb09bbfd7190af1715274f8" class="anchor"></a>

```cpp
inline void avoid_trim_results()
```

Disables automatic trimming of results for one operation.

This function disables the automatic trimming of results for one operation. After it is called, the results will not be trimmed automatically until the function is called again.
<hr />

### `read_using_size` <a id="class_modem_class_1a83d1105d39191e2c902331b3f0c4c8f5" class="anchor"></a>

```cpp
inline void read_using_size()
```

Enables a specific mode of reading where the size of the data to be read is considered for processing.

<hr />

### `debug` <a id="class_modem_class_1a6e6c472f405698e25db0d90a1de359f4" class="anchor"></a>

```cpp
inline void debug(Stream & u, uint8_t level)
```

<hr />

### `noDebug` <a id="class_modem_class_1a0b73ad5e52ad405a3be764d948cd8522" class="anchor"></a>

```cpp
inline void noDebug()
```

Used to disable debugging output for the modem communication.

<hr />

### `debug` <a id="class_modem_class_1a0d2808ffeccd83db3e26cc3568e33eeb" class="anchor"></a>

```cpp
inline void debug(bool e)
```

<hr />

### `timeout` <a id="class_modem_class_1a240fc0451c5152615f192b675beabadc" class="anchor"></a>

```cpp
inline void timeout(size_t timeout_ms)
```

Sets the timeout value for communication operations.

#### Parameters
* `Can` be called with a specified timeout value in milliseconds.
<hr />

# class `WiFiClient` <a id="class_wi_fi_client" class="anchor"></a>

```cpp
class WiFiClient
  : public Client
```

Represents a Wi-Fi client that connects to a remote server over a Wi-Fi network.

The [WiFiClient](#class_wi_fi_client) class allows for network communication over Wi-Fi, providing methods for establishing connections, sending and receiving data, and managing the client’s socket state. This class is used to manage client connections in a Wi-Fi network, either for communication or for network data transfer.

It inherits from the Client class, providing basic socket communication functionality.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`WiFiClient`](#class_wi_fi_client_1aa22ef0fd15a3e2b8ac45eceb0df3bc74) | Default constructor for the [WiFiClient](#class_wi_fi_client) class. |
| [`WiFiClient`](#class_wi_fi_client_1a53ed2903b15a61a52c3b90406978be89) | Constructor to initialize a [WiFiClient](#class_wi_fi_client) object with a specific socket. |
| [`WiFiClient`](#class_wi_fi_client_1a1b176d034c1239bdcffa6db13ef5e485) | Copy constructor for the [WiFiClient](#class_wi_fi_client) class. |
| [`~WiFiClient`](#class_wi_fi_client_1a6410fb12d526d541c436136b18faa0db) | Destructor for the [WiFiClient](#class_wi_fi_client) class. |
| [`connect`](#class_wi_fi_client_1a0c10a429fa90cf0589daa9742e10e0ee) | Establishes a connection to a server using an IP address and port. |
| [`connect`](#class_wi_fi_client_1a00fe1cf5622b3d941c99c944cc2df0e8) | Establishes a connection to a server using a hostname and port. |
| [`write`](#class_wi_fi_client_1a1bbd17136c9320c9eecb5a98a5fc1720) | Sends a single byte of data to the connected server. |
| [`write`](#class_wi_fi_client_1a03638bc1d73e73e9cc5cddf6a4a9c320) | Writes a buffer of data to the connected server. |
| [`available`](#class_wi_fi_client_1ae3f72b88b376090eb24f1fe1c16212d7) | Checks the number of bytes available for reading from the server. |
| [`read`](#class_wi_fi_client_1a8207089f4496ef5b429277fa8cc2d259) | Reads a single byte of data from the server. |
| [`read`](#class_wi_fi_client_1abd26fb1cce208783b3392507e448d0d2) | Reads multiple bytes of data from the server into a buffer. |
| [`peek`](#class_wi_fi_client_1a892443057ba2c24ae094c5a87e50cff5) | Peeks at the next byte of incoming data without removing it from the internal buffer. |
| [`flush`](#class_wi_fi_client_1a54739070fd04288ced179019ee212530) | Flushes the write buffer of the client. |
| [`stop`](#class_wi_fi_client_1acb61f8e0ecd50e40ce6cbcb7106ce1b1) | Closes the connection to the server and clears the receive buffer. |
| [`connected`](#class_wi_fi_client_1af083fe27b94aebec37f140c0b973f974) | Checks if the client is connected to a server. |
| [`operator bool`](#class_wi_fi_client_1adaf93736006b5bb2426e9816de4207eb) | Implicit conversion operator to `bool`. |
| [`operator==`](#class_wi_fi_client_1ac1e068fb2468d84536e93f6e1b51b099) | Equality operator for comparing two `[WiFiClient](#class_wi_fi_client)` objects. |
| [`operator!=`](#class_wi_fi_client_1a31b6e43ab5ab9d6fe511778a5a1f173c) | Inequality operator for comparing two `[WiFiClient](#class_wi_fi_client)` objects. |
| [`remoteIP`](#class_wi_fi_client_1a46d45c8d326b62256f85d55eaa337df0) | Retrieves the remote IP address of the server the client is connected to. |
| [`remotePort`](#class_wi_fi_client_1a38ca1399ebf6570d2c852c9062ec92d8) | Retrieves the remote port number of the server the client is connected to. |
| [`setConnectionTimeout`](#class_wi_fi_client_1af32938f36f09c9121e85e38338d432d7) | Sets the connection timeout for the client. |

## Members

### `WiFiClient` <a id="class_wi_fi_client_1aa22ef0fd15a3e2b8ac45eceb0df3bc74" class="anchor"></a>

```cpp
WiFiClient()
```

Default constructor for the [WiFiClient](#class_wi_fi_client) class.

<hr />

### `WiFiClient` <a id="class_wi_fi_client_1a53ed2903b15a61a52c3b90406978be89" class="anchor"></a>

```cpp
WiFiClient(int s)
```

Constructor to initialize a [WiFiClient](#class_wi_fi_client) object with a specific socket.

#### Parameters
* `s` is the socket descriptor to associate with this client.
<hr />

### `WiFiClient` <a id="class_wi_fi_client_1a1b176d034c1239bdcffa6db13ef5e485" class="anchor"></a>

```cpp
WiFiClient(const WiFiClient & c)
```

Copy constructor for the [WiFiClient](#class_wi_fi_client) class.

#### Parameters
* `c` is the `[WiFiClient](#class_wi_fi_client)` object to copy.
<hr />

### `~WiFiClient` <a id="class_wi_fi_client_1a6410fb12d526d541c436136b18faa0db" class="anchor"></a>

```cpp
~WiFiClient()
```

Destructor for the [WiFiClient](#class_wi_fi_client) class.

<hr />

### `connect` <a id="class_wi_fi_client_1a0c10a429fa90cf0589daa9742e10e0ee" class="anchor"></a>

```cpp
virtual int connect(IPAddress ip, uint16_t port)
```

Establishes a connection to a server using an IP address and port.

#### Parameters
* `ip` as the IP address of the server to connect to. 

* `port` as the port number on the server to connect to.

#### Returns
`1` on a successful connection, `0` on failure.
<hr />

### `connect` <a id="class_wi_fi_client_1a00fe1cf5622b3d941c99c944cc2df0e8" class="anchor"></a>

```cpp
virtual int connect(const char * host, uint16_t port)
```

Establishes a connection to a server using a hostname and port.

#### Parameters
* `host` is a pointer to a null-terminated string containing the hostname of the server. 

* `port` is the port number on the server to connect to.

#### Returns
`1` if the connection was successful, `0` otherwise.
<hr />

### `write` <a id="class_wi_fi_client_1a1bbd17136c9320c9eecb5a98a5fc1720" class="anchor"></a>

```cpp
virtual size_t write(uint8_t)
```

Sends a single byte of data to the connected server.

#### Parameters
* `b` being the byte of data to send.

#### Returns
The number of bytes successfully written, which is `1` on success or `0` on failure.
<hr />

### `write` <a id="class_wi_fi_client_1a03638bc1d73e73e9cc5cddf6a4a9c320" class="anchor"></a>

```cpp
virtual size_t write(const uint8_t * buf, size_t size)
```

Writes a buffer of data to the connected server.

#### Parameters
* `Takes` a pointer to a buffer `buf` containing the data to be written and the size of the buffer `size` as parameters.

#### Returns
The number of bytes successfully written, or `0` if the write operation fails.
<hr />

### `available` <a id="class_wi_fi_client_1ae3f72b88b376090eb24f1fe1c16212d7" class="anchor"></a>

```cpp
virtual int available()
```

Checks the number of bytes available for reading from the server.

#### Returns
The number of bytes available to read. Returns `0` if no data is available, or if the socket is invalid.
<hr />

### `read` <a id="class_wi_fi_client_1a8207089f4496ef5b429277fa8cc2d259" class="anchor"></a>

```cpp
virtual int read()
```

Reads a single byte of data from the server.

#### Returns
The byte read as an `int` (0–255). Returns `-1` if no data is available or if an error occurs.
<hr />

### `read` <a id="class_wi_fi_client_1abd26fb1cce208783b3392507e448d0d2" class="anchor"></a>

```cpp
virtual int read(uint8_t * buf, size_t size)
```

Reads multiple bytes of data from the server into a buffer.

This function retrieves data from the server's receive buffer and stores it into the provided array. It attempts to read up to the specified number of bytes (`size`).

#### Parameters
* `buf` is a pointer to the buffer where the data will be stored. 

* `size` is the maximum number of bytes to read.

#### Returns
The number of bytes successfully read into the buffer. Returns `0` if no data is available or if the socket is invalid.
<hr />

### `peek` <a id="class_wi_fi_client_1a892443057ba2c24ae094c5a87e50cff5" class="anchor"></a>

```cpp
virtual int peek()
```

Peeks at the next byte of incoming data without removing it from the internal buffer.

#### Returns
The next byte as an `int` (0–255). Returns `-1` if no data is available or if the socket is invalid.
<hr />

### `flush` <a id="class_wi_fi_client_1a54739070fd04288ced179019ee212530" class="anchor"></a>

```cpp
virtual void flush()
```

Flushes the write buffer of the client.

This function ensures that any data buffered for transmission is sent to the connected server. If there is any pending data in the send buffer, it is transmitted over the network.
<hr />

### `stop` <a id="class_wi_fi_client_1acb61f8e0ecd50e40ce6cbcb7106ce1b1" class="anchor"></a>

```cpp
virtual void stop()
```

Closes the connection to the server and clears the receive buffer.

<hr />

### `connected` <a id="class_wi_fi_client_1af083fe27b94aebec37f140c0b973f974" class="anchor"></a>

```cpp
virtual uint8_t connected()
```

Checks if the client is connected to a server.

#### Returns
Returns 1 if the client is connected, 0 otherwise.
<hr />

### `operator bool` <a id="class_wi_fi_client_1adaf93736006b5bb2426e9816de4207eb" class="anchor"></a>

```cpp
inline virtual operator bool()
```

Implicit conversion operator to `bool`.

Converts the `[WiFiClient](#class_wi_fi_client)` object to a `bool` value indicating whether the client is connected or not.

#### Returns
`true` if the client socket is open and valid, `false` otherwise.
<hr />

### `operator==` <a id="class_wi_fi_client_1ac1e068fb2468d84536e93f6e1b51b099" class="anchor"></a>

```cpp
virtual bool operator==(const WiFiClient &)
```

Equality operator for comparing two `[WiFiClient](#class_wi_fi_client)` objects.

Compares the current `[WiFiClient](#class_wi_fi_client)` object with another `[WiFiClient](#class_wi_fi_client)` object to determine if they represent the same socket connection.

#### Parameters
* `The` `[WiFiClient](#class_wi_fi_client)` object to compare with. 

#### Returns
`true` if both `[WiFiClient](#class_wi_fi_client)` objects represent the same socket, `false` otherwise.
<hr />

### `operator!=` <a id="class_wi_fi_client_1a31b6e43ab5ab9d6fe511778a5a1f173c" class="anchor"></a>

```cpp
inline virtual bool operator!=(const WiFiClient & whs)
```

Inequality operator for comparing two `[WiFiClient](#class_wi_fi_client)` objects.

Compares the current `[WiFiClient](#class_wi_fi_client)` object with another `[WiFiClient](#class_wi_fi_client)` object to determine if they represent different socket connections.

#### Parameters
* `whs` is the `[WiFiClient](#class_wi_fi_client)` object to compare with. 

#### Returns
`true` if both [WiFiClient](#class_wi_fi_client) objects represent different sockets, `false` if they represent the same socket.
<hr />

### `remoteIP` <a id="class_wi_fi_client_1a46d45c8d326b62256f85d55eaa337df0" class="anchor"></a>

```cpp
virtual IPAddress remoteIP()
```

Retrieves the remote IP address of the server the client is connected to.

#### Returns
The IP address of the remote server. If not connected, returns IPAddress(0, 0, 0, 0).
<hr />

### `remotePort` <a id="class_wi_fi_client_1a38ca1399ebf6570d2c852c9062ec92d8" class="anchor"></a>

```cpp
virtual uint16_t remotePort()
```

Retrieves the remote port number of the server the client is connected to.

#### Returns
Returns the port number of the remote server. If not connected, returns 0.
<hr />

### `setConnectionTimeout` <a id="class_wi_fi_client_1af32938f36f09c9121e85e38338d432d7" class="anchor"></a>

```cpp
inline void setConnectionTimeout(int timeout)
```

Sets the connection timeout for the client.

#### Parameters
* `timeout` is the timeout value in milliseconds.
<hr />

# class `WiFiFileSystem` <a id="class_wi_fi_file_system" class="anchor"></a>

Class that handles the WiFi file system operations.

This class provides functionality for managing files on a WiFi-connected device, including mounting, reading, writing, and configuring the file system.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`WiFiFileSystem`](#class_wi_fi_file_system_1af4d9123dd9631c69853070498afb41a2) | Initializes objects of the [WiFiFileSystem](#class_wi_fi_file_system) class. |
| [`mount`](#class_wi_fi_file_system_1aa5baed674db176f774488b50fa35c3d1) | Mounts the file system and optionally formats it on failure. |
| [`writefile`](#class_wi_fi_file_system_1a0121ce8d9a1b9d4a53e7b7ea7ecc4c1c) | Writes data to a file in the file system. |
| [`readfile`](#class_wi_fi_file_system_1acd3d52b7a20f5f65b7ca28dbb7b04343) | Reads a file from the file system and prints its content. |

## Members

### `WiFiFileSystem` <a id="class_wi_fi_file_system_1af4d9123dd9631c69853070498afb41a2" class="anchor"></a>

```cpp
WiFiFileSystem()
```

Initializes objects of the [WiFiFileSystem](#class_wi_fi_file_system) class.

<hr />

### `mount` <a id="class_wi_fi_file_system_1aa5baed674db176f774488b50fa35c3d1" class="anchor"></a>

```cpp
void mount(bool format_on_fault)
```

Mounts the file system and optionally formats it on failure.

#### Parameters
* `If` `format_on_fault` is set to `true`, the file system will be formatted if a fault occurs during mounting.
<hr />

### `writefile` <a id="class_wi_fi_file_system_1a0121ce8d9a1b9d4a53e7b7ea7ecc4c1c" class="anchor"></a>

```cpp
size_t writefile(const char * name, const char * data, size_t size, int operation)
```

Writes data to a file in the file system.

#### Parameters
* `name` is the name of the file to which the data will be written. A pointer `data` to the data that will be written to the file. `size` is the number of bytes to write from the provided data buffer. `operation` determines the type of file operation to perform (e.g., create, overwrite).

#### Returns
Returns the number of bytes successfully written. Returns 0 if the operation fails.
<hr />

### `readfile` <a id="class_wi_fi_file_system_1acd3d52b7a20f5f65b7ca28dbb7b04343" class="anchor"></a>

```cpp
void readfile(const char * name)
```

Reads a file from the file system and prints its content.

It sends the read request to the modem, which fetches the data. The content is printed to the serial output in chunks, with each chunk being processed until the entire file is read.

#### Parameters
* `name` is the name of the file to be read from the file system.
<hr />

# class `WiFiServer` <a id="class_wi_fi_server" class="anchor"></a>

```cpp
class WiFiServer
  : public Server
```

A class that provides server functionality for WiFi-based communication.

The [WiFiServer](#class_wi_fi_server) class inherits from the Server class and extends its functionality to create and manage a server over a WiFi connection. This class allows for accepting incoming client connections, handling data communication, and closing connections in a networked environment.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`WiFiServer`](#class_wi_fi_server_1acfa8226decf3a818dfbd45ec7940280a) | Initializes objects of the [WiFiServer](#class_wi_fi_server) class. |
| [`WiFiServer`](#class_wi_fi_server_1a63b71b18a7011b40fb86d893bc4c72fd) | Constructs a [WiFiServer](#class_wi_fi_server) object with the specified port. |
| [`available`](#class_wi_fi_server_1abfd839b75fa3c40bd5e22c4a122ed800) | Checks if there are any incoming client connections waiting to be accepted. |
| [`accept`](#class_wi_fi_server_1ad29b9a043c87bad43140cc3066210088) | Accepts an incoming client connection on the server. |
| [`begin`](#class_wi_fi_server_1a52e68fa8b767579e5055de9ff072f08c) | Starts the Wi-Fi server and binds it to the specified port. |
| [`begin`](#class_wi_fi_server_1a953b3d2a3ea0b0582be9535b6aa908d9) | Starts the Wi-Fi server and binds it to the default port. |
| [`write`](#class_wi_fi_server_1ad3d206415f9733dee2170f136b909e54) | Writes a single byte to all connected clients. |
| [`write`](#class_wi_fi_server_1a6bef9499519bcedd59379024c4e7b360) | Writes data to all connected clients. |
| [`end`](#class_wi_fi_server_1a26d44e7107a4121589f96b505c73593d) | Ends the Wi-Fi server and closes the server socket. |
| [`operator bool`](#class_wi_fi_server_1a1cca17be23aad6b7dce5fc2315226e5d) | Converts the [WiFiSSLClient](#class_wi_fi_s_s_l_client) object to a boolean value. |
| [`operator==`](#class_wi_fi_server_1a6e3fc602a5e129d19e6fa5076419511f) | Compares two [WiFiServer](#class_wi_fi_server) objects for equality. |
| [`operator!=`](#class_wi_fi_server_1a3b34f02716dd23e22b85585b4d648169) | Compares two [WiFiServer](#class_wi_fi_server) objects for inequality. |

## Members

### `WiFiServer` <a id="class_wi_fi_server_1acfa8226decf3a818dfbd45ec7940280a" class="anchor"></a>

```cpp
WiFiServer()
```

Initializes objects of the [WiFiServer](#class_wi_fi_server) class.

<hr />

### `WiFiServer` <a id="class_wi_fi_server_1a63b71b18a7011b40fb86d893bc4c72fd" class="anchor"></a>

```cpp
WiFiServer(int p)
```

Constructs a [WiFiServer](#class_wi_fi_server) object with the specified port.

#### Parameters
* `p` The port number on which the server will listen for incoming connections.
<hr />

### `available` <a id="class_wi_fi_server_1abfd839b75fa3c40bd5e22c4a122ed800" class="anchor"></a>

```cpp
WiFiClient available()
```

Checks if there are any incoming client connections waiting to be accepted.

This function queries the server to check if there is a client waiting to be accepted. If a client is available, it returns a `[WiFiClient](#class_wi_fi_client)` object representing the client. It uses the modem to query the server for an available client socket and accepts the connection if a valid client is found.

#### Returns
Returns a [WiFiClient](#class_wi_fi_client) object representing the next client connection that is available for processing.
<hr />

### `accept` <a id="class_wi_fi_server_1ad29b9a043c87bad43140cc3066210088" class="anchor"></a>

```cpp
WiFiClient accept()
```

Accepts an incoming client connection on the server.

#### Returns
Returns a [WiFiClient](#class_wi_fi_client) object representing the accepted client.
<hr />

### `begin` <a id="class_wi_fi_server_1a52e68fa8b767579e5055de9ff072f08c" class="anchor"></a>

```cpp
void begin(int port)
```

Starts the Wi-Fi server and binds it to the specified port.

#### Parameters
* `port` is the port number which the server will listen to for incoming connections.
<hr />

### `begin` <a id="class_wi_fi_server_1a953b3d2a3ea0b0582be9535b6aa908d9" class="anchor"></a>

```cpp
void begin()
```

Starts the Wi-Fi server and binds it to the default port.

This function initializes the server and binds it to a default port.
<hr />

### `write` <a id="class_wi_fi_server_1ad3d206415f9733dee2170f136b909e54" class="anchor"></a>

```cpp
virtual size_t write(uint8_t)
```

Writes a single byte to all connected clients.

#### Parameters
* `b` is the byte to be sent to the clients.
<hr />

### `write` <a id="class_wi_fi_server_1a6bef9499519bcedd59379024c4e7b360" class="anchor"></a>

```cpp
virtual size_t write(const uint8_t * buf, size_t size)
```

Writes data to all connected clients.

This function sends a buffer of data to all clients connected to the server. It writes the specified number of bytes to the server socket and returns the count of successfully written bytes.

#### Parameters
* `buf` is a pointer to the buffer containing the data to be sent. `size` is the number of bytes to write from the buffer.

#### Returns
The number of bytes successfully written. Returns 0 if the data could not be sent.
<hr />

### `end` <a id="class_wi_fi_server_1a26d44e7107a4121589f96b505c73593d" class="anchor"></a>

```cpp
void end()
```

Ends the Wi-Fi server and closes the server socket.

This function terminates the server by sending a command to the modem to close the server socket. It sets the server socket variable to an invalid state (`-1`) to indicate that the server is no longer active.
<hr />

### `operator bool` <a id="class_wi_fi_server_1a1cca17be23aad6b7dce5fc2315226e5d" class="anchor"></a>

```cpp
explicit operator bool()
```

Converts the [WiFiSSLClient](#class_wi_fi_s_s_l_client) object to a boolean value.

This operator allows a [WiFiSSLClient](#class_wi_fi_s_s_l_client) object to be implicitly or explicitly converted to a boolean. It checks whether the client socket is valid (i.e., `_sock != -1`).

#### Returns
`true` if the server socket is valid (server is running), `false` otherwise.
<hr />

### `operator==` <a id="class_wi_fi_server_1a6e3fc602a5e129d19e6fa5076419511f" class="anchor"></a>

```cpp
virtual bool operator==(const WiFiServer &)
```

Compares two [WiFiServer](#class_wi_fi_server) objects for equality.

This virtual operator compares the underlying socket (`_sock`) of two `[WiFiServer](#class_wi_fi_server)` objects to determine if they refer to the same server connection.

#### Parameters
* `[WiFiServer](#class_wi_fi_server)` object to compare against.

#### Returns
`true` if both [WiFiServer](#class_wi_fi_server) objects have the same socket; `false` otherwise.
<hr />

### `operator!=` <a id="class_wi_fi_server_1a3b34f02716dd23e22b85585b4d648169" class="anchor"></a>

```cpp
inline virtual bool operator!=(const WiFiServer & whs)
```

Compares two [WiFiServer](#class_wi_fi_server) objects for inequality.

This virtual operator compares the underlying socket (`_sock`) of two [WiFiServer](#class_wi_fi_server) objects. It returns `true` if the objects do not refer to the same server connection (i.e., they have different socket values), and `false` otherwise.

#### Parameters
* `whs` The [WiFiServer](#class_wi_fi_server) object to compare against. 

#### Returns
`true` if the [WiFiServer](#class_wi_fi_server) objects have different sockets; `false` otherwise.
<hr />

# class `WiFiSSLClient` <a id="class_wi_fi_s_s_l_client" class="anchor"></a>

```cpp
class WiFiSSLClient
  : public WiFiClient
```

A specialized client class for secure SSL/TLS connections.

The [WiFiSSLClient](#class_wi_fi_s_s_l_client) class extends the functionality of the [WiFiClient](#class_wi_fi_client) class to provide secure communication over SSL/TLS protocols. It ensures encrypted and authenticated communication between the client and a remote server.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`WiFiSSLClient`](#class_wi_fi_s_s_l_client_1aeecf408e130c75ca84c9e41f7cf708aa) | Initializes objects of the [WiFiSSLClient](#class_wi_fi_s_s_l_client) class. |
| [`~WiFiSSLClient`](#class_wi_fi_s_s_l_client_1afe721b778749143fe61fdfff259218af) |  |
| [`connect`](#class_wi_fi_s_s_l_client_1a82ed27a660bb90559c0467920182c947) | Establishes a secure SSL connection to a specified IP address and port. |
| [`connect`](#class_wi_fi_s_s_l_client_1a46375064a7f581ba072e67bdc68be494) | Establishes a secure SSL connection to a specified host and port. |
| [`setCACert`](#class_wi_fi_s_s_l_client_1ad97df1f2253445c9fec5f4471afcdbf1) | Sets the Certificate Authority (CA) for SSL/TLS verification. |
| [`setEccSlot`](#class_wi_fi_s_s_l_client_1a9720117b29a35bc2a43c133f76fd8f8e) | Sets the ECC (Elliptic Curve Cryptography) key slot and certificate for establishing secure SSL connections. |
| [`write`](#class_wi_fi_s_s_l_client_1aa998458d525200ce36277d637008f87c) | Writes a single byte of data to the SSL connection. |
| [`write`](#class_wi_fi_s_s_l_client_1afa24293a9551bbcca1b565da2607eb2b) | Writes a buffer of data to the SSL connection. |
| [`available`](#class_wi_fi_s_s_l_client_1ae52872cae6f3aa8b53c50ebe2373eb81) | Checks the number of bytes available for reading from the SSL connection. |
| [`read`](#class_wi_fi_s_s_l_client_1adac00db4d021b38a513c5ae97e6305ec) | Reads data from the SSL connection into the receive buffer. |
| [`read`](#class_wi_fi_s_s_l_client_1a163b81ae7656797ed010cdcb0b576e58) | Reads a specified number of bytes from the SSL connection into a buffer. |
| [`peek`](#class_wi_fi_s_s_l_client_1aafdaf6405d3cbc7807b0ac04fc511061) | Peeks at the next byte available from the SSL connection without removing it. |
| [`flush`](#class_wi_fi_s_s_l_client_1ae894698f8e8b90ebd298fa66fedadd32) | Flushes the write buffer of the SSL connection. |
| [`stop`](#class_wi_fi_s_s_l_client_1a66113af6fbc85f0dbb73f8d276b8a77a) | Terminates the SSL/TLS connection and clears the receive buffer. |
| [`connected`](#class_wi_fi_s_s_l_client_1a5e993c746855bb67c744d27baa6cf1bb) | Checks if the SSL/TLS connection is active. |
| [`operator bool`](#class_wi_fi_s_s_l_client_1a46888795cc1562c33fad408b57d2ad40) | Implicit conversion operator to check if the SSL client is connected. |
| [`operator==`](#class_wi_fi_s_s_l_client_1aa0bdf11dd3e6ef48133967dc0a036004) | Comparison operator to check equality between two `[WiFiSSLClient](#class_wi_fi_s_s_l_client)` objects. |
| [`operator!=`](#class_wi_fi_s_s_l_client_1a2cdd8020168fae9e08d3c6d00b30b065) | Inequality operator to compare two `[WiFiSSLClient](#class_wi_fi_s_s_l_client)` objects. |
| [`remoteIP`](#class_wi_fi_s_s_l_client_1acff0aa8078124dff0c0ff3bfee7cfd83) | Retrieves the remote IP address of the WiFi SSL client. |
| [`remotePort`](#class_wi_fi_s_s_l_client_1aea76ab94b3cdfec17ab6e73c7b169da7) | Retrieves the remote port number of the WiFi SSL client. |

## Members

### `WiFiSSLClient` <a id="class_wi_fi_s_s_l_client_1aeecf408e130c75ca84c9e41f7cf708aa" class="anchor"></a>

```cpp
WiFiSSLClient()
```

Initializes objects of the [WiFiSSLClient](#class_wi_fi_s_s_l_client) class.

<hr />

### `~WiFiSSLClient` <a id="class_wi_fi_s_s_l_client_1afe721b778749143fe61fdfff259218af" class="anchor"></a>

```cpp
~WiFiSSLClient()
```

<hr />

### `connect` <a id="class_wi_fi_s_s_l_client_1a82ed27a660bb90559c0467920182c947" class="anchor"></a>

```cpp
virtual int connect(IPAddress ip, uint16_t port)
```

Establishes a secure SSL connection to a specified IP address and port.

#### Parameters
* `It` takes an `IPAddress` object representing the IP address of the server and a `uint16_t` port number as parameters.

#### Returns
Returns a status code indicating the success or failure of the connection.
<hr />

### `connect` <a id="class_wi_fi_s_s_l_client_1a46375064a7f581ba072e67bdc68be494" class="anchor"></a>

```cpp
virtual int connect(const char * host, uint16_t port)
```

Establishes a secure SSL connection to a specified host and port.

#### Parameters
* `host` is the hostname or IP address of the server to connect to. `port` is the port number to connect to.

#### Returns
Returns `1` if the connection is successfully established, `0` otherwise.
<hr />

### `setCACert` <a id="class_wi_fi_s_s_l_client_1ad97df1f2253445c9fec5f4471afcdbf1" class="anchor"></a>

```cpp
void setCACert(const char * root_ca)
```

Sets the Certificate Authority (CA) for SSL/TLS verification.

#### Parameters
* `root_ca` is a pointer to a null-terminated string containing the root CA certificate in PEM format. If set to `nullptr`, the default root CA bundle will be used.
<hr />

### `setEccSlot` <a id="class_wi_fi_s_s_l_client_1a9720117b29a35bc2a43c133f76fd8f8e" class="anchor"></a>

```cpp
void setEccSlot(int ecc508KeySlot, const byte cert, int certLength)
```

Sets the ECC (Elliptic Curve Cryptography) key slot and certificate for establishing secure SSL connections.

#### Parameters
* `int` ecc508KeySlot specifies the ECC key slot to be used for the SSL connection. 

* `const` byte cert[] is a pointer to the certificate data in the form of an array of bytes. 

* `int` certLength specifies the length of the certificate data array.
<hr />

### `write` <a id="class_wi_fi_s_s_l_client_1aa998458d525200ce36277d637008f87c" class="anchor"></a>

```cpp
virtual size_t write(uint8_t)
```

Writes a single byte of data to the SSL connection.

#### Parameters
* `b` is the byte to be sent.

#### Returns
The number of bytes successfully written. Returns `1` if the byte was sent successfully, or `0` if an error occurred.
<hr />

### `write` <a id="class_wi_fi_s_s_l_client_1afa24293a9551bbcca1b565da2607eb2b" class="anchor"></a>

```cpp
virtual size_t write(const uint8_t * buf, size_t size)
```

Writes a buffer of data to the SSL connection.

#### Parameters
* `buf` is a pointer to the buffer containing the data to be sent. 

* `size` is the number of bytes to send from the buffer.

#### Returns
Returns `size` if the data is successfully sent, or `0` if the transmission fails or the socket is invalid.
<hr />

### `available` <a id="class_wi_fi_s_s_l_client_1ae52872cae6f3aa8b53c50ebe2373eb81" class="anchor"></a>

```cpp
virtual int available()
```

Checks the number of bytes available for reading from the SSL connection.

#### Returns
Returns the number of bytes available to read from the SSL connection without blocking.
<hr />

### `read` <a id="class_wi_fi_s_s_l_client_1adac00db4d021b38a513c5ae97e6305ec" class="anchor"></a>

```cpp
virtual int read()
```

Reads data from the SSL connection into the receive buffer.

#### Returns
Returns the number of bytes successfully read into the buffer. Returns `0` if no data is received, or `-1` if the socket is invalid or an error occurs.
<hr />

### `read` <a id="class_wi_fi_s_s_l_client_1a163b81ae7656797ed010cdcb0b576e58" class="anchor"></a>

```cpp
virtual int read(uint8_t * buf, size_t size)
```

Reads a specified number of bytes from the SSL connection into a buffer.

#### Parameters
* `buf` is a pointer to the buffer where the read data will be stored. `size` is the maximum number of bytes to read into the buffer.

#### Returns
The number of bytes successfully read. Returns `0` if no data is available or an error occurs.
<hr />

### `peek` <a id="class_wi_fi_s_s_l_client_1aafdaf6405d3cbc7807b0ac04fc511061" class="anchor"></a>

```cpp
virtual int peek()
```

Peeks at the next byte available from the SSL connection without removing it.

This function queries the modem to retrieve the next byte available in the SSL/TLS connection, allowing the byte to remain in the buffer for future reads.

#### Returns
The next byte available as an integer value (0–255), or `-1` if the socket is invalid or no data is available.
<hr />

### `flush` <a id="class_wi_fi_s_s_l_client_1ae894698f8e8b90ebd298fa66fedadd32" class="anchor"></a>

```cpp
virtual void flush()
```

Flushes the write buffer of the SSL connection.

This function clears the write buffer, ensuring that any pending data is sent over the SSL/TLS connection. It uses the modem to handle the flush operation.
<hr />

### `stop` <a id="class_wi_fi_s_s_l_client_1a66113af6fbc85f0dbb73f8d276b8a77a" class="anchor"></a>

```cpp
virtual void stop()
```

Terminates the SSL/TLS connection and clears the receive buffer.

<hr />

### `connected` <a id="class_wi_fi_s_s_l_client_1a5e993c746855bb67c744d27baa6cf1bb" class="anchor"></a>

```cpp
virtual uint8_t connected()
```

Checks if the SSL/TLS connection is active.

This function determines if the SSL/TLS client is still connected by querying the modem for the connection status. It checks the validity of the socket before proceeding with the query.

#### Returns
Returns `1` if the client is connected, `0` otherwise.
<hr />

### `operator bool` <a id="class_wi_fi_s_s_l_client_1a46888795cc1562c33fad408b57d2ad40" class="anchor"></a>

```cpp
inline virtual operator bool()
```

Implicit conversion operator to check if the SSL client is connected.

#### Returns
`true` if the socket is valid (i.e., connected), `false` otherwise.
<hr />

### `operator==` <a id="class_wi_fi_s_s_l_client_1aa0bdf11dd3e6ef48133967dc0a036004" class="anchor"></a>

```cpp
virtual bool operator==(const WiFiSSLClient &)
```

Comparison operator to check equality between two `[WiFiSSLClient](#class_wi_fi_s_s_l_client)` objects.

#### Parameters
* `WiFiSSLClient` object to compare.

#### Returns
`true` if both [WiFiSSLClient](#class_wi_fi_s_s_l_client) objects are equivalent (i.e., they have the same socket), `false` otherwise.
<hr />

### `operator!=` <a id="class_wi_fi_s_s_l_client_1a2cdd8020168fae9e08d3c6d00b30b065" class="anchor"></a>

```cpp
inline virtual bool operator!=(const WiFiSSLClient & whs)
```

Inequality operator to compare two `[WiFiSSLClient](#class_wi_fi_s_s_l_client)` objects.

This operator compares the current `[WiFiSSLClient](#class_wi_fi_s_s_l_client)` object with another `[WiFiSSLClient](#class_wi_fi_s_s_l_client)` object to determine if they are not equal, based on their underlying socket or connection.

#### Parameters
* `whs` The [WiFiSSLClient](#class_wi_fi_s_s_l_client) object to compare with. 

#### Returns
`true` if the two [WiFiSSLClient](#class_wi_fi_s_s_l_client) objects do not represent the same connection (i.e., have different sockets), `false` otherwise.
<hr />

### `remoteIP` <a id="class_wi_fi_s_s_l_client_1acff0aa8078124dff0c0ff3bfee7cfd83" class="anchor"></a>

```cpp
virtual IPAddress remoteIP()
```

Retrieves the remote IP address of the WiFi SSL client.

This function queries the modem for the remote IP address associated with the current connection.

#### Returns
The remote IP address of the client. Returns `0.0.0.0` if the socket is not valid or the query fails.
<hr />

### `remotePort` <a id="class_wi_fi_s_s_l_client_1aea76ab94b3cdfec17ab6e73c7b169da7" class="anchor"></a>

```cpp
virtual uint16_t remotePort()
```

Retrieves the remote port number of the WiFi SSL client.

This function queries the modem to obtain the remote port number associated with the current connection.

#### Returns
Returns the remote port number of the client. Returns `0` if the socket is not valid or the query fails.
<hr />

# class `WiFiUDP` <a id="class_wi_fi_u_d_p" class="anchor"></a>

```cpp
class WiFiUDP
  : public UDP
```

A class for handling UDP communication over a Wi-Fi network.

The [WiFiUDP](#class_wi_fi_u_d_p) class is an extension of the UDP class that enables sending and receiving UDP packets over a Wi-Fi network. It provides functions for initialization, packet transmission, and reception tailored for Wi-Fi connectivity.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
| [`WiFiUDP`](#class_wi_fi_u_d_p_1a84c016f902e4999f74b2356d2af483ea) | Default constructor for the [WiFiUDP](#class_wi_fi_u_d_p) class. |
| [`begin`](#class_wi_fi_u_d_p_1af48f0679da52268c8d3d74110fa56035) | Starts a UDP socket on the specified local port. |
| [`begin`](#class_wi_fi_u_d_p_1a830cf425860621f6c71f785012b07ff6) | Starts a UDP socket bound to a specific IP address and port. |
| [`beginMulticast`](#class_wi_fi_u_d_p_1a470bcdb3a2fffdfecf2d5945a2345789) | Starts a UDP multicast socket bound to a specific IP address and port. |
| [`stop`](#class_wi_fi_u_d_p_1a87ee974d0e6ec204364d5e73396be9fa) | Stops the UDP socket and releases its resources. |
| [`beginMulticastPacket`](#class_wi_fi_u_d_p_1a96258bfc994f75c2144ede2afcbc0636) | Begins constructing a multicast UDP packet for sending. |
| [`beginPacket`](#class_wi_fi_u_d_p_1a2e51a25f349ed007b2fa7f78dca43a89) | Begins constructing a UDP packet for sending to a specific IP address and port. |
| [`beginPacket`](#class_wi_fi_u_d_p_1a448452411940224e708b9d907d56fa74) | Begins constructing a UDP packet for sending to a specific hostname and port. |
| [`endPacket`](#class_wi_fi_u_d_p_1ab157246011e8f79564881f68a9bd1b11) | Completes the construction of a UDP packet and sends it to the specified destination. |
| [`write`](#class_wi_fi_u_d_p_1ab36596bdae87541922d3b2653517500b) | Sends a single byte of data to the currently opened UDP packet. |
| [`write`](#class_wi_fi_u_d_p_1ae6a547561e192e3446f3190a429bbbd5) | Sends a buffer of data to the currently opened UDP packet. |
| [`parsePacket`](#class_wi_fi_u_d_p_1a52641a292b2c9ea8494bc91c22bba2ac) | Start processing the next available incoming packet. |
| [`available`](#class_wi_fi_u_d_p_1a72f5e44ece70dd7a2a96647a897951d2) | Checks if there are any available UDP packets to read. |
| [`read`](#class_wi_fi_u_d_p_1a4474f72712418b90f2415581a4e5b9e5) | Read a single byte from the current packet. |
| [`read`](#class_wi_fi_u_d_p_1aacde0f39adec12a6151b42a89badea02) | Reads data from the UDP receive buffer into a provided buffer. |
| [`read`](#class_wi_fi_u_d_p_1a04e1b1de54485392307a90ae3c703529) | Reads data from the UDP receive buffer into a character buffer. |
| [`peek`](#class_wi_fi_u_d_p_1adf9cda99b5319dd9c58a2dd50075639b) | Peeks at the next byte available in the UDP buffer without moving on to the next byte. |
| [`flush`](#class_wi_fi_u_d_p_1acd8e5a16383d8b47c13536007ee04a71) | Finish reading the current packet. |
| [`operator==`](#class_wi_fi_u_d_p_1a31b0064b29cee88cf5aa7eb056f2b3a8) | Compares two [WiFiUDP](#class_wi_fi_u_d_p) objects for equality. |
| [`operator!=`](#class_wi_fi_u_d_p_1a9fc23a50af71ef9b34c73c7dee897c01) | Compares two [WiFiUDP](#class_wi_fi_u_d_p) objects for inequality. |
| [`remoteIP`](#class_wi_fi_u_d_p_1a840ae72c440bed1c3e429f177cb41740) | Retrieves the remote IP address of the host who sent the current incoming packet. |
| [`remotePort`](#class_wi_fi_u_d_p_1a2ebb4b0e8fc1c4c147bd5936ff7ab250) | Return the port of the host who sent the current incoming packet. |

## Members

### `WiFiUDP` <a id="class_wi_fi_u_d_p_1a84c016f902e4999f74b2356d2af483ea" class="anchor"></a>

```cpp
WiFiUDP()
```

Default constructor for the [WiFiUDP](#class_wi_fi_u_d_p) class.

<hr />

### `begin` <a id="class_wi_fi_u_d_p_1af48f0679da52268c8d3d74110fa56035" class="anchor"></a>

```cpp
virtual uint8_t begin(uint16_t)
```

Starts a UDP socket on the specified local port.

#### Parameters
* `uint16_t` The local port number to bind the UDP socket to.

#### Returns
Returns `1` if the socket is successfully opened, or `0` if the socket is already in use or could not be opened.
<hr />

### `begin` <a id="class_wi_fi_u_d_p_1a830cf425860621f6c71f785012b07ff6" class="anchor"></a>

```cpp
virtual uint8_t begin(IPAddress a, uint16_t p)
```

Starts a UDP socket bound to a specific IP address and port.

#### Parameters
* `a` The local IP address to bind the UDP socket to. 

* `p` The local port number to bind the UDP socket to.

#### Returns
Returns `1` if the socket is successfully opened, or `0` if the socket is already in use or could not be opened.
<hr />

### `beginMulticast` <a id="class_wi_fi_u_d_p_1a470bcdb3a2fffdfecf2d5945a2345789" class="anchor"></a>

```cpp
virtual uint8_t beginMulticast(IPAddress, uint16_t)
```

Starts a UDP multicast socket bound to a specific IP address and port.

#### Parameters
* `IPAddress` The multicast IP address to bind the UDP socket to. 

* `uint16_t` The port number to bind the UDP socket to.

#### Returns
Returns `1` if the socket is successfully opened, or `0` if the socket is already in use or could not be opened.
<hr />

### `stop` <a id="class_wi_fi_u_d_p_1a87ee974d0e6ec204364d5e73396be9fa" class="anchor"></a>

```cpp
virtual void stop()
```

Stops the UDP socket and releases its resources.

<hr />

### `beginMulticastPacket` <a id="class_wi_fi_u_d_p_1a96258bfc994f75c2144ede2afcbc0636" class="anchor"></a>

```cpp
virtual int beginMulticastPacket()
```

Begins constructing a multicast UDP packet for sending.

#### Returns
Returns `1` if the packet preparation is successful. Or `0` if there is an error or the socket is not initialized.
<hr />

### `beginPacket` <a id="class_wi_fi_u_d_p_1a2e51a25f349ed007b2fa7f78dca43a89" class="anchor"></a>

```cpp
virtual int beginPacket(IPAddress ip, uint16_t port)
```

Begins constructing a UDP packet for sending to a specific IP address and port.

#### Parameters
* `ip` The destination IP address as an `IPAddress` object. 

* `port` The destination port number.

#### Returns
Returns `1` if the packet preparation is successful. Or `0` if there is an error or the socket is not initialized.
<hr />

### `beginPacket` <a id="class_wi_fi_u_d_p_1a448452411940224e708b9d907d56fa74" class="anchor"></a>

```cpp
virtual int beginPacket(const char * host, uint16_t port)
```

Begins constructing a UDP packet for sending to a specific hostname and port.

#### Parameters
* `host` The destination hostname as a null-terminated string. 

* `port` The destination port number.

#### Returns
Returns `1` if the packet preparation is successful. Or `0` if there is an error or the socket is not initialized.
<hr />

### `endPacket` <a id="class_wi_fi_u_d_p_1ab157246011e8f79564881f68a9bd1b11" class="anchor"></a>

```cpp
virtual int endPacket()
```

Completes the construction of a UDP packet and sends it to the specified destination.

#### Returns
Returns `1` if the packet is successfully transmitted. Or `0` if there is an error or the socket is not initialized.
<hr />

### `write` <a id="class_wi_fi_u_d_p_1ab36596bdae87541922d3b2653517500b" class="anchor"></a>

```cpp
virtual size_t write(uint8_t)
```

Sends a single byte of data to the currently opened UDP packet.

#### Parameters
* `b` The byte of data to send.

#### Returns
Returns `1` if the byte was successfully written. Or `0` if there was an error or the packet was not properly initialized.
<hr />

### `write` <a id="class_wi_fi_u_d_p_1ae6a547561e192e3446f3190a429bbbd5" class="anchor"></a>

```cpp
virtual size_t write(const uint8_t * buffer, size_t size)
```

Sends a buffer of data to the currently opened UDP packet.

#### Parameters
* `buffer` A pointer to the buffer containing the data to send. 

* `size` The number of bytes from the buffer to write.

#### Returns
Returns the number of bytes successfully written if the operation is successful. Or `0` if the data was not successfully written, or if the packet was not properly initialized.
<hr />

### `parsePacket` <a id="class_wi_fi_u_d_p_1a52641a292b2c9ea8494bc91c22bba2ac" class="anchor"></a>

```cpp
virtual int parsePacket()
```

Start processing the next available incoming packet.

#### Returns
Returns the size of the incoming packet, or `0` if no packet is available.
<hr />

### `available` <a id="class_wi_fi_u_d_p_1a72f5e44ece70dd7a2a96647a897951d2" class="anchor"></a>

```cpp
virtual int available()
```

Checks if there are any available UDP packets to read.

#### Returns
Returns the number of available bytes if packets are available, or `0` if no packets are available.
<hr />

### `read` <a id="class_wi_fi_u_d_p_1a4474f72712418b90f2415581a4e5b9e5" class="anchor"></a>

```cpp
virtual int read()
```

Read a single byte from the current packet.

#### Returns
Returns the number of bytes read into the buffer, or `-1` if an error occurs.
<hr />

### `read` <a id="class_wi_fi_u_d_p_1aacde0f39adec12a6151b42a89badea02" class="anchor"></a>

```cpp
virtual int read(unsigned char * buffer, size_t len)
```

Reads data from the UDP receive buffer into a provided buffer.

#### Parameters
* `buffer` A pointer to the buffer where the received data will be stored. 

* `size` The number of bytes to read from the UDP buffer.

#### Returns
The number of bytes successfully read into the buffer. If less than `size` bytes are read, it indicates that the buffer was exhausted early.
<hr />

### `read` <a id="class_wi_fi_u_d_p_1a04e1b1de54485392307a90ae3c703529" class="anchor"></a>

```cpp
inline virtual int read(char * buffer, size_t len)
```

Reads data from the UDP receive buffer into a character buffer.

#### Parameters
* `buffer` A pointer to the character buffer where the received data will be stored. 

* `len` The number of bytes to read from the UDP buffer.

#### Returns
The number of bytes successfully read into the buffer. If less than `len` bytes are read, it indicates that the buffer was exhausted early.
<hr />

### `peek` <a id="class_wi_fi_u_d_p_1adf9cda99b5319dd9c58a2dd50075639b" class="anchor"></a>

```cpp
virtual int peek()
```

Peeks at the next byte available in the UDP buffer without moving on to the next byte.

#### Returns
The value of the next byte in the UDP buffer, or `-1` if no data is available.
<hr />

### `flush` <a id="class_wi_fi_u_d_p_1acd8e5a16383d8b47c13536007ee04a71" class="anchor"></a>

```cpp
virtual void flush()
```

Finish reading the current packet.

<hr />

### `operator==` <a id="class_wi_fi_u_d_p_1a31b0064b29cee88cf5aa7eb056f2b3a8" class="anchor"></a>

```cpp
virtual bool operator==(const WiFiUDP &)
```

Compares two [WiFiUDP](#class_wi_fi_u_d_p) objects for equality.

This function compares two `[WiFiUDP](#class_wi_fi_u_d_p)` objects by checking if their associated socket values (`_sock`) are the same.

#### Parameters
* `WiFiUDP&` The [WiFiUDP](#class_wi_fi_u_d_p) object to compare with the current object.

#### Returns
`true` if the socket values are equal, `false` otherwise.
<hr />

### `operator!=` <a id="class_wi_fi_u_d_p_1a9fc23a50af71ef9b34c73c7dee897c01" class="anchor"></a>

```cpp
inline virtual bool operator!=(const WiFiUDP & whs)
```

Compares two [WiFiUDP](#class_wi_fi_u_d_p) objects for inequality.

This function compares two `[WiFiUDP](#class_wi_fi_u_d_p)` objects by checking if their associated socket values (`_sock`) are different.

#### Parameters
* `whs` The [WiFiUDP](#class_wi_fi_u_d_p) object to compare with the current object. 

#### Returns
`true` if the socket values are different, `false` otherwise.
<hr />

### `remoteIP` <a id="class_wi_fi_u_d_p_1a840ae72c440bed1c3e429f177cb41740" class="anchor"></a>

```cpp
virtual IPAddress remoteIP()
```

Retrieves the remote IP address of the host who sent the current incoming packet.

#### Returns
An `IPAddress` object representing the remote IP address. If the socket is not valid or the address cannot be retrieved, it returns `IPAddress(0, 0, 0, 0)`.
<hr />

### `remotePort` <a id="class_wi_fi_u_d_p_1a2ebb4b0e8fc1c4c147bd5936ff7ab250" class="anchor"></a>

```cpp
virtual uint16_t remotePort()
```

Return the port of the host who sent the current incoming packet.

#### Returns
The remote port number as a `uint16_t`. If the socket is not valid or the port cannot be retrieved, it returns `0`.
<hr />

