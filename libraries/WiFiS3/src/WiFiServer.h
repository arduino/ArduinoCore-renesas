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



class WiFiServer : public Server {
private:
  int _sock;
  int _port;
  WiFiClient client;
  
public:
  /**
   * @brief Initializes objects of the `WiFiServer` class.
   */
  WiFiServer();
  WiFiServer(int p);

  /**
   * @brief Checks if there are any incoming client connections waiting to be accepted.
   * 
   * This function queries the server to check if there is a client waiting to be 
   * accepted. If a client is available, it returns a `WiFiClient` object representing 
   * the client. It uses the modem to query the server for an available client 
   * socket and accepts the connection if a valid client is found.
   *
   * @return Returns a `WiFiClient` object representing the next client connection that is available
   * for processing.
   */
  WiFiClient available();

  /**
   * @brief Accepts an incoming client connection on the server.
   * 
   * @return Returns a `WiFiClient` object representing the accepted client.
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
  explicit operator bool();
  virtual bool operator==(const WiFiServer&);
  virtual bool operator!=(const WiFiServer& whs)
  {
    return !this->operator==(whs);
  };

  using Print::write;

  friend class WiFiClient;
};

#endif
