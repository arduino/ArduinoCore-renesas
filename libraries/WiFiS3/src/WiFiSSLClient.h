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
   WiFiSSLClient();
   ~WiFiSSLClient();
   virtual int connect(IPAddress ip, uint16_t port);
   virtual int connect(const char* host, uint16_t port);
   void setCACert(const char* root_ca = NULL, size_t size = 0); 
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
   int _sock;
   bool _custom_root = false;
   void getSocket();
   int _read();
   void read_if_needed(size_t s);

private:
   void upload_default_Cert();
};

#endif /* WIFISSLCLIENT_H */
