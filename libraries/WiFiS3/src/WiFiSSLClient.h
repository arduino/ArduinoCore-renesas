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


class WiFiSSLClient : public WiFiClient {

public:
   /**
    * Initializes objects of the `WiFiSSLClient` class.
    */
   WiFiSSLClient();
   ~WiFiSSLClient();

   /**
    * Establishes a secure SSL connection to a specified IP address and port.
    * It takes an `IPAddress` object representing the IP address of the server
    * and a `uint16_t` port number as parameters.
    */
   virtual int connect(IPAddress ip, uint16_t port);

   /**
    * Establishes a secure SSL connection to a specified host (domain name) and port.
    */
   virtual int connect(const char* host, uint16_t port);

   /**
    * Sets the Certificate Authority (CA) certificate for establishing secure SSL connections.
    */
   void setCACert(const char* root_ca);

   /**
    * Sets the ECC (Elliptic Curve Cryptography) key slot and 
    * certificate for establishing secure SSL connections.
    * `int ecc508KeySlot` specifies the ECC key slot to be used for the SSL connection.
    * `const byte cert[]` is a pointer to the certificate data in the form of an array of bytes.
    * `int certLength` specifies the length of the certificate data array.
    */
   void setEccSlot(int ecc508KeySlot, const byte cert[], int certLength);
   
   /**
    * Allows writing a single byte of data to the SSL connection.
    * The `uint8_t` parameter represents the byte of data to be written.
    */
   virtual size_t write(uint8_t);

   /**
    * Allows writing a buffer of data to the SSL connection.
    */
   virtual size_t write(const uint8_t *buf, size_t size);
   
   /**
    * Checks the number of bytes available for reading from the SSL connection.
    * It returns the number of bytes available to read from the SSL connection without blocking.
    */
   virtual int available();

   /**
    * Reads a single byte of data from the SSL connection.
    */
   virtual int read();

   /**
    * Reads a buffer of data from the SSL connection.
    */
   virtual int read(uint8_t *buf, size_t size);

   /**
    * Peeks at the next byte of incoming data without removing it
    * from the internal buffer of the SSL connection.
    */
   virtual int peek();

   /**
    * Flushes any pending data in the SSL connection.
    * When called, this function ensures that any buffered outgoing data is
    * sent over the SSL connection immediately.
    */
   virtual void flush();

   /**
    * Closes the SSL connection.
    * When called, this function will terminate the SSL connection and
    * release any associated resources.
    */
   virtual void stop();
   
   virtual uint8_t connected();
   virtual operator bool() {
      return _sock != -1;
   }
   virtual bool operator==(const WiFiSSLClient&);
   virtual bool operator!=(const WiFiSSLClient& whs)
   {
      return !this->operator==(whs);
   };
   virtual IPAddress remoteIP();
   virtual uint16_t remotePort();

   friend class WiFiServer;

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
