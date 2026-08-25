/*
   This file is part of the WiFiS3 library.
   Copyright (c) 2023 Arduino SA. All rights reserved.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef WIFISSLCLIENT_H
#define WIFISSLCLIENT_H

#include "WiFiCommands.h"
#include "WiFiClient.h"
#include "Modem.h"
#include "WiFiFileSystem.h"

/**
 * @brief A specialized client class for secure SSL/TLS connections.
 * 
 * The WiFiSSLClient class extends the functionality of the WiFiClient class to provide secure
 * communication over SSL/TLS protocols. It ensures encrypted and authenticated communication
 * between the client and a remote server.
 */
class WiFiSSLClient : public WiFiClient {

public:
   /**
    * @brief Initializes objects of the WiFiSSLClient class.
    */
   WiFiSSLClient();
   ~WiFiSSLClient();

   /**
    * @brief Establishes a secure SSL connection to a specified IP address and port.
    * 
    * @param It takes an `IPAddress` object representing the IP address of the server
    * and a `uint16_t` port number as parameters.
    *
    * @return Returns a status code indicating the success or failure of the 
    * connection.
    */
   virtual int connect(IPAddress ip, uint16_t port);

   /**
    * @brief Establishes a secure SSL connection to a specified host and port.
    * 
    * @param `host` is the hostname or IP address of the server to connect to.
    * `port` is the port number to connect to.
    * 
    * @return Returns `1` if the connection is successfully established, `0` otherwise.
    */
   virtual int connect(const char* host, uint16_t port);

   /**
    * @brief Sets the Certificate Authority (CA) for SSL/TLS verification.
    *
    * @param `root_ca` is a pointer to a null-terminated string containing the root 
    * CA certificate in PEM format. If set to `nullptr`, the default root 
    * CA bundle will be used.
    */
   void setCACert(const char* root_ca);

   /**
    * @brief Sets the ECC (Elliptic Curve Cryptography) key slot and 
    * certificate for establishing secure SSL connections.
    * 
    * @param `int ecc508KeySlot` specifies the ECC key slot to be used for the SSL connection.
    * @param `const byte cert[]` is a pointer to the certificate data in the form of an array of bytes.
    * @param `int certLength` specifies the length of the certificate data array.
    */
   void setEccSlot(int ecc508KeySlot, const byte cert[], int certLength);
   
   /**
    * @brief Writes a single byte of data to the SSL connection.
    * 
    * @param `b` is the byte to be sent.
    * 
    * @return The number of bytes successfully written. Returns `1` if the byte 
    * was sent successfully, or `0` if an error occurred.
    */
   virtual size_t write(uint8_t);

   /**
    * @brief Writes a buffer of data to the SSL connection.
    *
    * @param `buf` is a pointer to the buffer containing the data to be sent.
    * @param `size` is the number of bytes to send from the buffer.
    *
    * @return Returns `size` if the data is successfully sent, 
    * or `0` if the transmission fails or the socket is invalid.
    */
   virtual size_t write(const uint8_t *buf, size_t size);
   
   /**
    * @brief Checks the number of bytes available for reading from the SSL connection.
    * 
    * @return Returns the number of bytes available to read from the SSL connection without blocking.
    */
   virtual int available();

   /**
    * @brief Reads data from the SSL connection into the receive buffer.
    *
    * @return Returns the number of bytes successfully read into the buffer. Returns 
    * `0` if no data is received, or `-1` if the socket is invalid or an error occurs.
    */
   virtual int read();

   /**
    * @brief Reads a specified number of bytes from the SSL connection into a buffer.
    *
    * @param `buf` is a pointer to the buffer where the read data will be stored.
    * `size` is the maximum number of bytes to read into the buffer.
    *
    * @return The number of bytes successfully read. Returns `0` if no data is 
    * available or an error occurs.
    */
   virtual int read(uint8_t *buf, size_t size);

   /**
    * @brief Peeks at the next byte available from the SSL connection without removing it.
    *
    * This function queries the modem to retrieve the next byte available in the 
    * SSL/TLS connection, allowing the byte to remain in the buffer for future reads.
    *
    * @return The next byte available as an integer value (0–255), or `-1` if 
    * the socket is invalid or no data is available.
    */
   virtual int peek();

   /**
    * @brief Flushes the write buffer of the SSL connection.
    * 
    * This function clears the write buffer, ensuring that any pending data is sent 
    * over the SSL/TLS connection. It uses the modem to handle the flush operation.
    */
   virtual void flush();

   /**
    * @brief Terminates the SSL/TLS connection and clears the receive buffer.
    */
   virtual void stop();

   /**
    * @brief Checks if the SSL/TLS connection is active.
    *
    * This function determines if the SSL/TLS client is still connected by querying 
    * the modem for the connection status. It checks the validity of the socket 
    * before proceeding with the query.
    *
    * @return Returns `1` if the client is connected, `0` otherwise.
    */
   virtual uint8_t connected();

   /**
    * @brief Implicit conversion operator to check if the SSL client is connected.
    * 
    * @return `true` if the socket is valid (i.e., connected), `false` otherwise.
    */
   virtual operator bool() {
      return _sock != -1;
   }

   /**
    * @brief Comparison operator to check equality between two `WiFiSSLClient` objects.
    * 
    * @param `WiFiSSLClient` object to compare.
    * 
    * @return `true` if both WiFiSSLClient objects are equivalent (i.e., they have the same socket),
    * `false` otherwise.
    */
   virtual bool operator==(const WiFiSSLClient&);

   /**
    * @brief Inequality operator to compare two `WiFiSSLClient` objects.
    * 
    * This operator compares the current `WiFiSSLClient` object with another `WiFiSSLClient` object
    * to determine if they are not equal, based on their underlying socket or connection.
    * 
    * @param `whs` The WiFiSSLClient object to compare with.
    * @return `true` if the two WiFiSSLClient objects do not represent the same connection (i.e., have different sockets),
    * `false` otherwise.
    */
   virtual bool operator!=(const WiFiSSLClient& whs)
   {
      return !this->operator==(whs);
   };

   /**
    * @brief Retrieves the remote IP address of the WiFi SSL client.
    *
    * This function queries the modem for the remote IP address associated with 
    * the current connection.
    *
    * @return The remote IP address of the client. Returns `0.0.0.0` if the 
    * socket is not valid or the query fails.
    */
   virtual IPAddress remoteIP();

   /**
    * @brief Retrieves the remote port number of the WiFi SSL client.
    *
    * This function queries the modem to obtain the remote port number associated 
    * with the current connection.
    *
    * @return Returns the remote port number of the client. Returns `0` if the socket 
    * is not valid or the query fails.
    */
   virtual uint16_t remotePort();


   /**
    * @brief Declares WiFiServer as a friend class.
    * 
    * This allows the WiFiServer class to access private and protected members 
    * of the WiFiSSLClient class.
    */
   friend class WiFiServer;

   /**
    * @brief Inherits the `write` method from the Print class.
    * 
    * This allows the WiFiSSLClient class to use the `write` method defined in the 
    * Print class.
    */
   using Print::write;

private:
   void getSocket();
   int _read();
   void read_if_needed(size_t s);
   const char* _root_ca = nullptr;
   int _ecc_slot = -1;
   const byte* _ecc_cert = nullptr;
   int _ecc_cert_len = 0;

};

#endif /* WIFISSLCLIENT_H */
