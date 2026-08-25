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

/**
 * @brief Class that handles the WiFi file system operations.
 * 
 * This class provides functionality for managing files on a WiFi-connected 
 * device, including mounting, reading, writing, and configuring the file system. 
 */
class WiFiFileSystem {

public:
   /**
    * @brief Initializes objects of the WiFiFileSystem class.
    */
   WiFiFileSystem();

   /**
    * @brief Mounts the file system and optionally formats it on failure.
    *
    * @param If `format_on_fault` is set to `true`,
    * the file system will be formatted if a fault occurs during mounting.
    */
   void mount(bool format_on_fault = false);

   /**
    * @brief Writes data to a file in the file system.
    *
    * @param `name` is the name of the file to which the data will be written.
    * A pointer `data` to the data that will be written to the file.
    * `size` is the number of bytes to write from the provided data buffer.
    * `operation` determines the type of file operation to perform (e.g., create, overwrite).
    * 
    * @return Returns the number of bytes successfully written. Returns 0 if the operation fails.
    */
   size_t writefile(const char* name, const char* data, size_t size, int operation = WIFI_FILE_WRITE);

   /**
    * @brief Reads a file from the file system and prints its content.
    * 
    * It sends the read request to the modem, which fetches the data. The content 
    * is printed to the serial output in chunks, with each chunk being processed 
    * until the entire file is read. 
    * 
    * @param `name` is the name of the file to be read from the file system.
    */
   void readfile(const char* name);

};

#endif /* WIFI_FILE_SYSTEM_H */
