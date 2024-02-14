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
  WiFiServer();
  WiFiServer(int p);
  WiFiClient available();
  WiFiClient accept();
  void begin(int port);
  void begin();
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
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
