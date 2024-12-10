/*
  WiFiUdp.h - Library for Arduino Wifi shield.
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

#ifndef wifiudp_h
#define wifiudp_h

#include <Udp.h>
#include "Arduino.h"  
#include "Arduino.h"  
#include "IPAddress.h"

#include "WiFiTypes.h"
#include "Modem.h"
#include "FifoBuffer.h"

/**
 * @brief A class for handling UDP communication over a Wi-Fi network.
 * 
 * The WiFiUDP class is an extension of the UDP class that enables sending and receiving UDP packets
 * over a Wi-Fi network. It provides functions for initialization, packet transmission, and reception
 * tailored for Wi-Fi connectivity.
 */
class WiFiUDP : public UDP {
private:
  int _sock; 
  static constexpr uint32_t RX_BUFFER_DIM = 1461;
  FifoBuffer<uint8_t,RX_BUFFER_DIM> rx_buffer;

protected:
  virtual int _read();
  virtual void read_if_needed(size_t s);
  
public:
  /**
   * @brief Default constructor for the WiFiUDP class.
   */
  WiFiUDP();

  /**
   * @brief Starts a UDP socket on the specified local port.
   * 
   * @param `uint16_t` The local port number to bind the UDP socket to.
   * 
   * @return Returns `1` if the socket is successfully opened, or 
   * `0` if the socket is already in use or could not be opened.
   */
  virtual uint8_t begin(uint16_t);
  
  /**
   * @brief Starts a UDP socket bound to a specific IP address and port.
   * 
   * @param `a` The local IP address to bind the UDP socket to.
   * @param `p` The local port number to bind the UDP socket to.
   * 
   * @return Returns `1` if the socket is successfully opened, or 
   * `0` if the socket is already in use or could not be opened.
   */
  virtual uint8_t begin(IPAddress a, uint16_t p);
  
  /**
   * @brief Starts a UDP multicast socket bound to a specific IP address and port.
   * 
   * @param `IPAddress` The multicast IP address to bind the UDP socket to.
   * @param `uint16_t` The port number to bind the UDP socket to.
   * 
   * @return Returns `1` if the socket is successfully opened, or 
   * `0` if the socket is already in use or could not be opened.
   */
  virtual uint8_t beginMulticast(IPAddress, uint16_t);
  
  /**
   * @brief Stops the UDP socket and releases its resources.
   */
  virtual void stop();

  /**
   * @brief Begins constructing a multicast UDP packet for sending.
   * 
   * @return Returns `1` if the packet preparation is successful.
   * Or `0` if there is an error or the socket is not initialized.
   */
  virtual int beginMulticastPacket();
  
  /**
   * @brief Begins constructing a UDP packet for sending to a specific IP address and port.
   * 
   * @param `ip` The destination IP address as an `IPAddress` object.
   * @param `port` The destination port number.
   * 
   * @return Returns `1` if the packet preparation is successful. 
   * Or `0` if there is an error or the socket is not initialized.
   */
  virtual int beginPacket(IPAddress ip, uint16_t port);

  /**
   * @brief Begins constructing a UDP packet for sending to a specific hostname and port.
   * 
   * @param `host` The destination hostname as a null-terminated string.
   * @param `port` The destination port number.
   * 
   * @return Returns `1` if the packet preparation is successful.
   * Or `0` if there is an error or the socket is not initialized.
   */
  virtual int beginPacket(const char *host, uint16_t port);

  /**
   * @brief Completes the construction of a UDP packet and sends it to the specified destination.
   * 
   * @return Returns `1` if the packet is successfully transmitted.
   * Or `0` if there is an error or the socket is not initialized.
   */
  virtual int endPacket();

  /**
   * @brief Sends a single byte of data to the currently opened UDP packet.
   * 
   * @param `b` The byte of data to send.
   * 
   * @return Returns `1` if the byte was successfully written.
   * Or `0` if there was an error or the packet was not properly initialized.
   */
  virtual size_t write(uint8_t);

  /**
   * @brief Sends a buffer of data to the currently opened UDP packet.
   * 
   * @param `buffer` A pointer to the buffer containing the data to send.
   * @param `size` The number of bytes from the buffer to write.
   * 
   * @return Returns the number of bytes successfully written if the operation is successful.
   * Or `0` if the data was not successfully written, or if the packet was not properly initialized.
   */
  virtual size_t write(const uint8_t *buffer, size_t size);
  
  /**
   * @brief Inherits the `write` method from the Print class.
   * 
   * This allows the WiFiSSLClient class to use the `write` method defined in the 
   * `Print` class.
   */
  using Print::write;

  /** 
   * @brief Start processing the next available incoming packet
   * 
   * @return Returns the size of the incoming packet, or `0` if no packet is available.
   */
  virtual int parsePacket();

  /** 
   * @brief Checks if there are any available UDP packets to read.
   * 
   * @return Returns the number of available bytes if packets are available, or `0` if no packets are available.
   */
  virtual int available();

  /** 
   * @brief Read a single byte from the current packet
   * 
   * @return Returns the number of bytes read into the buffer, or `-1` if an error occurs.
   */
  virtual int read();

  /** 
   * @brief Reads data from the UDP receive buffer into a provided buffer.
   * 
   * @param `buffer` A pointer to the buffer where the received data will be stored.
   * @param `size` The number of bytes to read from the UDP buffer.
   * 
   * @return The number of bytes successfully read into the buffer.
   * If less than `size` bytes are read, it indicates that the buffer was exhausted early.
   */
  virtual int read(unsigned char* buffer, size_t len);


  /** 
   * @brief Reads data from the UDP receive buffer into a character buffer.
   * 
   * @param `buffer` A pointer to the character buffer where the received data will be stored.
   * @param `len` The number of bytes to read from the UDP buffer.
   * 
   * @return The number of bytes successfully read into the buffer.
   * If less than `len` bytes are read, it indicates that the buffer was exhausted early.
   */
  virtual int read(char* buffer, size_t len) { return read((unsigned char*)buffer, len); };

  /** 
   * @brief Peeks at the next byte available in the UDP buffer without moving on to the next byte.
   * 
   * @return The value of the next byte in the UDP buffer, or `-1` if no data is available.
   */
  virtual int peek();

  /** 
   * @brief Finish reading the current packet
   */
  virtual void flush();	// Finish reading the current packet

  /** 
   * @brief Compares two WiFiUDP objects for equality.
   * 
   * This function compares two `WiFiUDP` objects by checking if their associated 
   * socket values (`_sock`) are the same.
   * 
   * @param `WiFiUDP&` The WiFiUDP object to compare with the current object.
   * 
   * @return `true` if the socket values are equal, `false` otherwise.
   */
  virtual bool operator==(const WiFiUDP&);
  
  /** 
   * @brief Compares two WiFiUDP objects for inequality.
   * 
   * This function compares two `WiFiUDP` objects by checking if their associated 
   * socket values (`_sock`) are different.
   * 
   * @param `whs` The WiFiUDP object to compare with the current object.
   * @return `true` if the socket values are different, `false` otherwise.
   */
  virtual bool operator!=(const WiFiUDP& whs)
  {
    return !this->operator==(whs);
  };

  /** 
   * @brief Retrieves the remote IP address of the host who sent the current incoming packet.
   * 
   * @return An `IPAddress` object representing the remote IP address. If the socket
   * is not valid or the address cannot be retrieved, it returns `IPAddress(0, 0, 0, 0)`.
   */
  virtual IPAddress remoteIP();
  
  /** 
   * @brief Return the port of the host who sent the current incoming packet.
   * 
   * @return The remote port number as a `uint16_t`. If the socket is not valid or
   * the port cannot be retrieved, it returns `0`.
   */
  virtual uint16_t remotePort();
  
};

#endif
