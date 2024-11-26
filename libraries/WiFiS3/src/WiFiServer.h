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
   * Initializes a `WiFiServer` object.
   */
  WiFiServer();
  WiFiServer(int p);

  /**
   * Checks if there are any incoming client connections waiting to be accepted.
   * It returns a `WiFiClient` object representing the next client connection that is available
   * for processing.
   */
  WiFiClient available();

  /**
   * Accepts and returns a new client connection that is waiting to be processed.
   * When called, it will accept the next incoming client connection and
   * return a `WiFiClient` object representing that connection.
   */
  WiFiClient accept();

  /**
   * Initializes the WiFi server on a specific port.
   * When this function is called with a port number as an argument,
   * it sets up the server to listen for incoming client connections on that specified port.
   */
  void begin(int port);
  
  void begin();
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);

  /**
   * Closes the server and release any resources associated with it.
   * When you call `end()`, it stops the server from listening for incoming client connections
   * and cleans up any allocated memory or resources.
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
