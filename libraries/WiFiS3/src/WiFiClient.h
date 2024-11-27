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
  /**
   * @brief Default constructor for the WiFiClient class.
   */
  WiFiClient();

  /* this constructor is not intended to be called outside of the Server class */
  /**
   * @brief Constructor to initialize a WiFiClient object with a specific socket.
   *
   * @param `s` is the socket descriptor to associate with this client.
   */
  WiFiClient(int s);

  /**
   * @brief Copy constructor for the WiFiClient class.
   *
   * @param `c` is the `WiFiClient` object to copy.
   */
  WiFiClient(const WiFiClient& c);
  ~WiFiClient();

  /**
   * @brief Establishes a connection to a server using an IP address and port.
   *
   * @param Using `ip` as the IP address of the server to connect to.
   * And `port` as the port number on the server to connect to.
   * 
   * @return `1` on a successful connection, `0` on failure.
   */
  virtual int connect(IPAddress ip, uint16_t port);

  /**
   * @brief Establishes a connection to a server using a hostname and port.
   * 
   * @param `host` is a pointer to a null-terminated string containing the hostname of the server.
   * And `port` is the port number on the server to connect to.
   * 
   * @return `1` if the connection was successful, `0` otherwise.
   */
  virtual int connect(const char *host, uint16_t port);

  /** 
   * @brief Sends a single byte of data to the connected server.
   *
   * @param `b` being the byte of data to send.
   * 
   * @return The number of bytes successfully written, which is `1` on success or `0` on failure.
   */
  virtual size_t write(uint8_t);

  /** 
   * @brief Writes a buffer of data to the connected server.
   * 
   * @param Takes a pointer to a buffer `buf` containing the data to be written
   * and the size of the buffer `size` as parameters.
   * 
   * @return The number of bytes successfully written, or `0` if the write operation fails.
   */
  virtual size_t write(const uint8_t *buf, size_t size);

  /**
   * @brief Checks the number of bytes available for reading from the server.
   *
   * @return The number of bytes available to read. Returns `0` if no data is 
   * available, or if the socket is invalid.
   */
  virtual int available();

  /**
   * @brief Reads a single byte of data from the server.
   *
   * @return The byte read as an `int` (0–255). Returns `-1` if no data is available 
   * or if an error occurs.
   */
  virtual int read();

  /**
   * @brief Reads multiple bytes of data from the server into a buffer.
   *
   * This function retrieves data from the server's receive buffer and stores it 
   * into the provided array. It attempts to read up to the specified number of 
   * bytes (`size`).
   *
   * @param[out] `buf` is a pointer to the buffer where the data will be stored.
   * @param[in] `size` is the maximum number of bytes to read.
   *
   * @return The number of bytes successfully read into the buffer. 
   * Returns `0` if no data is available or if the socket is invalid.
   */
  virtual int read(uint8_t *buf, size_t size);

  /**
   * @brief Peeks at the next byte of incoming data without removing it from the internal buffer.
   * 
   * @return The next byte as an `int` (0–255). Returns `-1` if no data is available 
   * or if the socket is invalid.
   */
  virtual int peek();

  /**
   * @brief Flushes the write buffer of the client.
   * 
   * This function ensures that any data buffered for transmission is sent to the
   * connected server. If there is any pending data in the send buffer, it is 
   * transmitted over the network.
   */
  virtual void flush();

  /**
   * @brief Closes the connection to the server and clears the receive buffer.
   */
  virtual void stop();

  /**
   * @brief Checks if the client is connected to a server.
   * 
   * @return Returns 1 if the client is connected, 0 otherwise.
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
   * @brief Retrieves the remote IP address of the server the client is connected to.
   * 
   * @return The IP address of the remote server. If not connected, returns IPAddress(0, 0, 0, 0).
   */
  virtual IPAddress remoteIP();

  /**
   * @brief Retrieves the remote port number of the server the client is connected to.
   * 
   * @return Returns the port number of the remote server. If not connected, returns 0.
   */
  virtual uint16_t remotePort();

  /**
   * @brief Sets the connection timeout for the client.
   * 
   * @param `timeout` is the timeout value in milliseconds.
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


