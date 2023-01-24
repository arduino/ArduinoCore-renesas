/* ########################################################################## */
/* - File: virtualEEPROM.h
   - Copyright (c): 2023 Arduino srl.  All right reserved.
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
#ifndef ARDUINO_VIRTUAL_EEPROM_H
#define ARDUINO_VIRTUAL_EEPROM_H

#include "DataFlashBlockDevice.h"

enum class ReadStatus { 
   UNABLE_TO_ALLOC,
   ALLOCATED,
   ALREADY_ALLOCATED
};

class veeprom {
public:
   static veeprom& getInstance();
   veeprom(veeprom const&) = delete;
   void operator=(veeprom const&) = delete;
   ~veeprom();
   
   
   bool write();
   /* erase block @ address containing "logical" index*/
   bool erase_block(uint32_t index);
   
   uint8_t read_byte(uint32_t index);
   ReadStatus read_block(uint32_t index, uint32_t &bytes_to_end_of_block);
   /* write a byte **IN RAM** into the block read from flash */
   void write_byte(uint8_t v, uint32_t index);
   int getLength();
  
private:
   veeprom();

   uint8_t *                    page;     
   bool                         write_page;
   bool                         erase_page;
   uint32_t                     page_address;                

   

};


#endif