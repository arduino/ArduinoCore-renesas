/* ########################################################################## */
/* - File: BlockDevice.h
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
#include "../Storage/storage_common.h"

#define  BLOCK_DEVICE_OK (0)
#define  BD_ERROR_OK      (0)
#define  BD_ERROR_DEVICE_ERROR  (-4001)
#define  BD_ERROR_WRITE_PROTECTED (4002)

typedef uint64_t bd_addr_t;
typedef uint64_t bd_size_t;
typedef pin_size_t pin_t;

/* -------------------------------------------------------------------------- */
/* Abstract Base BlockDevice class (defines the INTERFACE for all the 
   subclass)                                                                  */
/* -------------------------------------------------------------------------- */   

class BlockDevice {
private:
   virtual int open() { return 0; }
   virtual int close() { return 0; }
   virtual int write(const void *buffer, bd_addr_t addr, bd_size_t size) {
      return program(buffer, addr, size);
   }


public:
   BlockDevice() = default;
   BlockDevice(BlockDevice const&) = delete;
   void operator=(BlockDevice const&) = delete;

   static BlockDevice *get_default_instance();

   virtual ~BlockDevice() = default; 
   /* initialize a block device */
   virtual int init() = 0;
   /* deinitialize a block device*/
   virtual int deinit() = 0;
   /* Ensure data on storage is in sync with the driver (0 == OK) */
   virtual int sync() { return 0;}
   /* Read blocks from a block device */
   virtual int read(void *buffer, bd_addr_t addr, bd_size_t size) = 0;
   /* Write blocks */
   virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size) = 0;
   virtual int erase(bd_addr_t addr, bd_size_t size) = 0;
   virtual int trim(bd_addr_t addr, bd_size_t size) {
      (void)addr;
      (void)size;
      return 0;
   }
   

   virtual bd_size_t get_read_size() const = 0;
   virtual bd_size_t get_program_size() const = 0;
   virtual bd_size_t get_erase_size() const { return get_program_size();}
   virtual bd_size_t get_erase_size(bd_addr_t addr) const {
      (void)addr;
      return get_erase_size();
   }


   virtual int get_erase_value() const { return -1; }

   virtual bool available() { return true; }

    
   virtual bd_size_t size() const = 0; 
   virtual bool is_valid_read(bd_addr_t addr, bd_size_t size) const {
     return ( addr % get_read_size() == 0 && size % get_read_size() == 0 && addr + size <= this->size());
   }

    
   virtual bool is_valid_program(bd_addr_t addr, bd_size_t size) const {
     return ( addr % get_program_size() == 0 && size % get_program_size() == 0 && addr + size <= this->size());
   }

   
   virtual bool is_valid_erase(bd_addr_t addr, bd_size_t size) const {
     return ( addr % get_erase_size(addr) == 0 && (addr + size) % get_erase_size(addr + size - 1) == 0 && addr + size <= this->size());
   }


   virtual const char *get_type() const = 0;
};

#endif
