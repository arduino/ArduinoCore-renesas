/*
  WiFiClient.cpp - Library for Arduino Wifi shield.
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

#ifndef wificlient_h
#define wificlient_h
#include "Arduino.h"	
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"

#include "WiFiTypes.h"
#include "Modem.h"
#include "FifoBuffer.h"
#include <memory>

#define RX_BUFFER_DIM 1024

class WiFiClient : public Client {

public:
  WiFiClient();
  /* this constructor is not intended to be called outside of the Server class */
  WiFiClient(int s);
  WiFiClient(const WiFiClient& c);
  ~WiFiClient();
  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int available();
  virtual int read();
  virtual int read(uint8_t *buf, size_t size);
  virtual int peek();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool() {
    return _sock != -1;
  }
  virtual bool operator==(const WiFiClient&);
  virtual bool operator!=(const WiFiClient& whs)
  {
      return !this->operator==(whs);
  };
  virtual IPAddress remoteIP();
  virtual uint16_t remotePort();

  void setConnectionTimeout(int timeout) {
    _connectionTimeout = timeout;
  }

  friend class WiFiServer;
  
  using Print::write;

protected:
  int _sock;
  int _connectionTimeout = 0;
  void getSocket();
  std::shared_ptr<FifoBuffer<uint8_t,RX_BUFFER_DIM>> rx_buffer;
  int _read();
  void read_if_needed(size_t s);
  void clear_buffer();
  bool destroy_at_distructor;


};

#endif


