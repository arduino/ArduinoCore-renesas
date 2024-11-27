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

class WiFiClient : public Client {

public:
  WiFiClient();
  /* this constructor is not intended to be called outside of the Server class */
  WiFiClient(int s);
  WiFiClient(const WiFiClient& c);
  ~WiFiClient();

  /**
   * @brief Establish a connection to a remote server using the specified IP address and port number.
   */
  virtual int connect(IPAddress ip, uint16_t port);

  /**
   * @brief Establish a connection to a remote server.
   * 
   * @param Uses the specified domain name or IP address `host` and `port` for port number.
   */
  virtual int connect(const char *host, uint16_t port);
  
  /** 
   * @brief Writes a single byte of data to the connected client.
   */
  virtual size_t write(uint8_t);

  /** 
   * @brief Writes a buffer of data to the connected client.
   * 
   * @param Takes a pointer to a buffer `buf` containing the data to be written
   * and the size of the buffer `size` as parameters.
   * 
   * @return The function writes the data in the buffer to the connected client
   * and returns the number of bytes written.
   */
  virtual size_t write(const uint8_t *buf, size_t size);

  /**
   * @brief Checks how many bytes are available for reading from the connected client.
   */
  virtual int available();

  /**
   * @brief Reads a single byte of data from the connected client.
   */
  virtual int read();

  /**
   * @brief Reads a specified number of bytes from the connected client into a buffer.
   */
  virtual int read(uint8_t *buf, size_t size);
  
  /**
   * @brief Peeks at the next byte of incoming data without removing it from the internal buffer.
   */
  virtual int peek();

  /**
   * @brief Clears any buffered outgoing data that has not been sent to the connected client.
   */
  virtual void flush();

  /**
   * @brief Closes the connection to the remote server
   * and releases any resources associated with it.
   */
  virtual void stop();

  /**
   * @brief Checks whether the client is currently connected to a remote server.
   * 
   * @return Returns a `uint8_t` value indicating the connection status of the client.
   */
  virtual uint8_t connected();

  virtual operator bool() {
    return _sock != -1;
  }
  virtual bool operator==(const WiFiClient&);
  virtual bool operator!=(const WiFiClient& whs)
  {
      return !this->operator==(whs);
  };

  /**
   * @return Returns the IP address of the remote server to which the client is connected.
   */
  virtual IPAddress remoteIP();

  /**
   * @return Returns the port number of the remote server for the connetec client.
   */
  virtual uint16_t remotePort();

  /**
   * @brief Sets the connection timeout value for the client.
   * @param It takes an integer `timeout` as a parameter which determines how long the
   * client will wait for a connection to be established before timing out.
   */
  void setConnectionTimeout(int timeout) {
    _connectionTimeout = timeout;
  }

  friend class WiFiServer;

  using Print::write;

protected:
  int _sock;
  int _connectionTimeout = 0;
  void getSocket();
  static constexpr uint32_t RX_BUFFER_DIM = 1024;
  std::shared_ptr<FifoBuffer<uint8_t,RX_BUFFER_DIM>> rx_buffer;
  int _read();
  void read_if_needed(size_t s);
  bool destroy_at_distructor;


};

#endif


