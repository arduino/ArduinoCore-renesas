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

#ifndef WIFI_FILE_SYSTEM_H
#define WIFI_FILE_SYSTEM_H

#include "Arduino.h"	
#include "WiFiCommands.h"
#include "Modem.h"

class WiFiFileSystem {

public:
   WiFiFileSystem();
   void mount(bool format_on_fault = false);
   size_t writefile(const char* name, const char* data, size_t size, int operation = WIFI_FILE_WRITE);
   void readfile(const char* name);

};

#endif /* WIFI_FILE_SYSTEM_H */
