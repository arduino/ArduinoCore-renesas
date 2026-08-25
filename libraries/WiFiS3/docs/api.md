# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class ` [`WiFiClient`](#class_wi_fi_client) | Represents a Wi-Fi client that connects to a remote server over a Wi-Fi network.
`class ` [`WiFiFileSystem`](#class_wi_fi_file_system) | Class that handles the WiFi file system operations.
`class ` [`WiFiServer`](#class_wi_fi_server) | A class that provides server functionality for WiFi-based communication.
`class ` [`WiFiSSLClient`](#class_wi_fi_s_s_l_client) | A specialized client class for secure SSL/TLS connections.
`class ` [`WiFiUDP`](#class_wi_fi_u_d_p) | A class for handling UDP communication over a Wi-Fi network.

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
| [`operator==`](#class_wi_fi_client_1ac1e068fb2468d84536e93f6e1b51b099) | Equality operator for comparing two [WiFiClient](#class_wi_fi_client) objects. |
| [`operator!=`](#class_wi_fi_client_1a31b6e43ab5ab9d6fe511778a5a1f173c) | Inequality operator for comparing two [WiFiClient](#class_wi_fi_client) objects. |
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
* `c` is the [WiFiClient](#class_wi_fi_client) object to copy.
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

Converts the [WiFiClient](#class_wi_fi_client) object to a `bool` value indicating whether the client is connected or not.

#### Returns
`true` if the client socket is open and valid, `false` otherwise.
<hr />

### `operator==` <a id="class_wi_fi_client_1ac1e068fb2468d84536e93f6e1b51b099" class="anchor"></a>

```cpp
virtual bool operator==(const WiFiClient &)
```

Equality operator for comparing two [WiFiClient](#class_wi_fi_client) objects.

Compares the current [WiFiClient](#class_wi_fi_client) object with another [WiFiClient](#class_wi_fi_client) object to determine if they represent the same socket connection.

#### Parameters
* `The` [WiFiClient](#class_wi_fi_client) object to compare with. 

#### Returns
`true` if both [WiFiClient](#class_wi_fi_client) objects represent the same socket, `false` otherwise.
<hr />

### `operator!=` <a id="class_wi_fi_client_1a31b6e43ab5ab9d6fe511778a5a1f173c" class="anchor"></a>

```cpp
inline virtual bool operator!=(const WiFiClient & whs)
```

Inequality operator for comparing two [WiFiClient](#class_wi_fi_client) objects.

Compares the current [WiFiClient](#class_wi_fi_client) object with another [WiFiClient](#class_wi_fi_client) object to determine if they represent different socket connections.

#### Parameters
* `whs` is the [WiFiClient](#class_wi_fi_client) object to compare with. 

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

This function queries the server to check if there is a client waiting to be accepted. If a client is available, it returns a [WiFiClient](#class_wi_fi_client) object representing the client. It uses the modem to query the server for an available client socket and accepts the connection if a valid client is found.

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

This virtual operator compares the underlying socket (`_sock`) of two [WiFiServer](#class_wi_fi_server) objects to determine if they refer to the same server connection.

#### Parameters
* [WiFiServer](#class_wi_fi_server) object to compare against.

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
| [`operator==`](#class_wi_fi_s_s_l_client_1aa0bdf11dd3e6ef48133967dc0a036004) | Comparison operator to check equality between two [WiFiSSLClient](#class_wi_fi_s_s_l_client) objects. |
| [`operator!=`](#class_wi_fi_s_s_l_client_1a2cdd8020168fae9e08d3c6d00b30b065) | Inequality operator to compare two [WiFiSSLClient](#class_wi_fi_s_s_l_client) objects. |
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

Comparison operator to check equality between two [WiFiSSLClient](#class_wi_fi_s_s_l_client) objects.

#### Parameters
* `WiFiSSLClient` object to compare.

#### Returns
`true` if both [WiFiSSLClient](#class_wi_fi_s_s_l_client) objects are equivalent (i.e., they have the same socket), `false` otherwise.
<hr />

### `operator!=` <a id="class_wi_fi_s_s_l_client_1a2cdd8020168fae9e08d3c6d00b30b065" class="anchor"></a>

```cpp
inline virtual bool operator!=(const WiFiSSLClient & whs)
```

Inequality operator to compare two [WiFiSSLClient](#class_wi_fi_s_s_l_client) objects.

This operator compares the current [WiFiSSLClient](#class_wi_fi_s_s_l_client) object with another [WiFiSSLClient](#class_wi_fi_s_s_l_client) object to determine if they are not equal, based on their underlying socket or connection.

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

This function compares two [WiFiUDP](#class_wi_fi_u_d_p) objects by checking if their associated socket values (`_sock`) are the same.

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

This function compares two [WiFiUDP](#class_wi_fi_u_d_p) objects by checking if their associated socket values (`_sock`) are different.

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
An `IPAddress` object representing the remote IP address. If the socket is not valid or the address cannot be retrieved, it returns `IPAddress(0, 0, 0, 0).
<hr />

### `remotePort` <a id="class_wi_fi_u_d_p_1a2ebb4b0e8fc1c4c147bd5936ff7ab250" class="anchor"></a>

```cpp
virtual uint16_t remotePort()
```

Return the port of the host who sent the current incoming packet.

#### Returns
The remote port number as a `uint16_t`. If the socket is not valid or the port cannot be retrieved, it returns `0`.
<hr />

