/*
  WiFiServer.h - Library for Arduino Wifi shield.
  Copyright (c) 2018 Arduino SA. All rights reserved.
  Copyright (c) 2011-2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef wifiserver_h
#define wifiserver_h

#include "Server.h"
#include "Modem.h"
#include "WiFiClient.h"


/**
 * @brief A class that provides server functionality for WiFi-based communication.
 * 
 * The WiFiServer class inherits from the Server class and extends its functionality 
 * to create and manage a server over a WiFi connection. This class allows for accepting 
 * incoming client connections, handling data communication, and closing connections 
 * in a networked environment.
 */
class WiFiServer : public Server {
private:
  int _sock;
  int _port;
  WiFiClient client;
  
public:
  /**
   * @brief Initializes objects of the WiFiServer class.
   */
  WiFiServer();

  /**
   * @brief Constructs a WiFiServer object with the specified port.
   * 
   * @param `p` The port number on which the server will listen for incoming connections.
   */
  WiFiServer(int p);

  /**
   * @brief Checks if there are any incoming client connections waiting to be accepted.
   * 
   * This function queries the server to check if there is a client waiting to be 
   * accepted. If a client is available, it returns a `WiFiClient` object representing 
   * the client. It uses the modem to query the server for an available client 
   * socket and accepts the connection if a valid client is found.
   *
   * @return Returns a WiFiClient object representing the next client connection that is available
   * for processing.
   */
  WiFiClient available();

  /**
   * @brief Accepts an incoming client connection on the server.
   * 
   * @return Returns a WiFiClient object representing the accepted client.
   */
  WiFiClient accept();

  /**
   * @brief Starts the Wi-Fi server and binds it to the specified port.
   * 
   * @param `port` is the port number which the server will listen to for incoming connections.
   */
  void begin(int port);
  
  /**
   * @brief Starts the Wi-Fi server and binds it to the default port.
   *
   * This function initializes the server and binds it to a default port.
   */
  void begin();

  /**
  * @brief Writes a single byte to all connected clients.
  *
  * @param `b` is the byte to be sent to the clients.
  */
  virtual size_t write(uint8_t);

  /**
   * @brief Writes data to all connected clients.
   *
   * This function sends a buffer of data to all clients connected to the server. 
   * It writes the specified number of bytes to the server 
   * socket and returns the count of successfully written bytes.
   *
   * @param `buf` is a pointer to the buffer containing the data to be sent.
   * `size` is the number of bytes to write from the buffer.
   * 
   * @return The number of bytes successfully written. Returns 0 if the 
   * data could not be sent.
   */
  virtual size_t write(const uint8_t *buf, size_t size);

  /**
   * @brief Ends the Wi-Fi server and closes the server socket.
   *
   * This function terminates the server by sending a command to the modem to 
   * close the server socket. It sets the server socket variable to an invalid 
   * state (`-1`) to indicate that the server is no longer active.
   *
   */
  void end();

  /**
   * @brief Converts the WiFiSSLClient object to a boolean value.
   * 
   * This operator allows a WiFiSSLClient object to be implicitly or explicitly
   * converted to a boolean. It checks whether the client socket is valid (i.e., 
   * `_sock != -1`).
   * 
   * @return `true` if the server socket is valid (server is running), `false` otherwise.
   */
  explicit operator bool();

  /**
   * @brief Compares two WiFiServer objects for equality.
   * 
   * This virtual operator compares the underlying socket (`_sock`) of two `WiFiServer` 
   * objects to determine if they refer to the same server connection.
   * 
   * @param WiFiServer object to compare against.
   * 
   * @return `true` if both WiFiServer objects have the same socket; `false` otherwise.
   */
  virtual bool operator==(const WiFiServer&);

  /**
   * @brief Compares two WiFiServer objects for inequality.
   * 
   * This virtual operator compares the underlying socket (`_sock`) of two WiFiServer
   * objects. It returns `true` if the objects do not refer to the same server connection 
   * (i.e., they have different socket values), and `false` otherwise.
   * 
   * @param `whs` The WiFiServer object to compare against.
   * @return `true` if the WiFiServer objects have different sockets; `false` otherwise.
   */
  virtual bool operator!=(const WiFiServer& whs)
  {
    return !this->operator==(whs);
  };

  /**
   * @brief Inherits the `write` method from the `Print` class.
   * 
   * This allows the WiFiSSLClient class to use the `write` method defined in the 
   * `Print` class.
   */
  using Print::write;

  /**
   * @brief Grants WiFiClient class access to private and protected members of WiFiServer.
   */
  friend class WiFiClient;
};

#endif
