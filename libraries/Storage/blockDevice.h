/* ########################################################################## */
/* - File: blockDevice.h
   - Copyright (c): 2022 Arduino srl.  All right reserved.
   - Author: Daniele Aimo (d.aimo@arduino.cc)

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
     Foundation, Inc.,51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA */
/* ########################################################################## */


#ifndef ARDUINO_BLOCK_DEVICE
#define ARDUINO_BLOCK_DEVICE
#include "Arduino.h"
#include <stdint.h>

#define BLOCK_DEVICE_OK (0)

typedef uint32_t BdAddr_t;
typedef uint32_t BdSize_t;
typedef pin_size_t pin_t;

/* -------------------------------------------------------------------------- */
/* Abstract Base blockDevice class (defines the INTERFACE for all the 
   subclass)                                                                  */
/* -------------------------------------------------------------------------- */   

class CBlockDevice {
public:
   CBlockDevice() {}
   CBlockDevice(CBlockDevice const&) = delete;
   void operator=(CBlockDevice const&) = delete;
   
   virtual ~CBlockDevice() {}; 
   virtual int open() = 0;
   virtual int close() = 0;
   virtual int read(void *buffer, BdAddr_t addr, BdSize_t size) = 0;
   virtual int write(const void *buffer, BdAddr_t addr, BdSize_t size) = 0;
   virtual int erase(BdAddr_t addr, BdSize_t size) = 0;
   virtual BdSize_t getWriteBlockSize() const = 0;
   virtual BdSize_t getEraseBlockSize() const = 0;
   virtual BdSize_t getReadBlockSize() const = 0;
   virtual BdSize_t getTotalSize() const = 0;
};

#endif