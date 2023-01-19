/* ########################################################################## */
/* - File: virtualEEPROM.cpp
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
  Foundation, Inc.,51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA    */
/* ########################################################################## */
#include "virtualEEPROM.h"


/* -------------------------------------------------------------------------- */
/*                    STATIC SINGLETONE - GET INSTANCE                        */
/* -------------------------------------------------------------------------- */
veeprom& veeprom::getInstance() {
    static veeprom    instance;
    return instance;
}

/* -------------------------------------------------------------------------- */
/*                              CONSTRUCTOR                                   */
/* -------------------------------------------------------------------------- */
veeprom::veeprom() {}

/* -------------------------------------------------------------------------- */
/*                              DISTRUCTOR                                    */
/* -------------------------------------------------------------------------- */
veeprom::~veeprom() {
   if(page != nullptr) {
      delete []page;
      page = nullptr;
   }
}

/* -------------------------------------------------------------------------- */
/*                              GET LENGTH                                    */
/* -------------------------------------------------------------------------- */
int veeprom::getLength() {
   return (int)DataFlashBlockDevice::getInstance().size();
}


/* -------------------------------------------------------------------------- */
/*                              READ SINGLE BYTE                              */
/* -------------------------------------------------------------------------- */
uint8_t veeprom::read_byte(uint32_t index) {
/* -------------------------------------------------------------------------- */   
   uint8_t rv;
   DataFlashBlockDevice::getInstance().read(&rv,index,1);
   return rv;
}


/* -------------------------------------------------------------------------- */
/*                   WRITE SINGLE BYTE (in **RAM** page)                      */
/* -------------------------------------------------------------------------- */
void veeprom::write_byte(uint8_t v, uint32_t index) {
   uint32_t block_size = DataFlashBlockDevice::getInstance().get_program_size();
   
   index = index % block_size;

   if(page != nullptr) {
      if(!erase_page) {
         if(*(page + index) != 0xFF) {
            erase_page = true;
         }
      }
      if(!write_page) {   
         if(*(page + index) != v) {
            write_page = true;
         }
      }
      *(page + index) = v;
   }
}



/* -------------------------------------------------------------------------- */
/*    ERASE THE BLOCK that contains the "logical" address index
      PAY ATTENTION: all data contained in the block (1024 bytes) will be
      erased!                                                                 */
/* -------------------------------------------------------------------------- */
bool veeprom::erase_block(uint32_t index) {
   uint32_t block_size = DataFlashBlockDevice::getInstance().get_erase_size();
   uint32_t pa = index & ~(block_size - 1);
   if(DataFlashBlockDevice::getInstance().erase(pa,block_size)) {
      return false;
   }
   return true;
}

#ifdef USE_ONLY_FOR_DEBUG
/* NOTE: DO NOT USE THIS FUNCTION UNLESS YOU REALLY UNDERSTAND WHAT I DOING 
   THE USE this function can disrupt the FLASH writing algorithm 
   It has been defined only for debug */
/* -------------------------------------------------------------------------- */
void veeprom::delete_ram_page() {
/* -------------------------------------------------------------------------- */   
   delete []page;
   page = nullptr;
}
#endif

/* -------------------------------------------------------------------------- */
/*              WRITE PAGE (in **FLASH**), it erases if necessary             */
/* -------------------------------------------------------------------------- */
bool veeprom::write() {
   bool rv = true;
   bool actual_write = true;
   if(page != nullptr) {
      if(write_page) {
         uint32_t block_size = DataFlashBlockDevice::getInstance().get_program_size();
         if(erase_page) {
            #ifdef DATA_FLASH_DEBUG
            Serial.println("[DBG]");
            Serial.println("[DBG] Virtual EEPROM ERASE OPERATION");
            Serial.println("[DBG]");
            #endif
            if(DataFlashBlockDevice::getInstance().erase(page_address,block_size)) {
               /* erase returns 0 if all is ok, if != from 0 this means that 
                  the erase procedure went bad and it makes no sense to perform
                  an actual write */
               
               actual_write = false;
               rv = false;
            }
         }
         if(actual_write) {
            #ifdef DATA_FLASH_DEBUG
            Serial.println("[DBG]");
            Serial.println("[DBG] Virtual EEPROM actual WRITE OPERATION");
            Serial.println("[DBG]");
            #endif
            if(DataFlashBlockDevice::getInstance().program(page,page_address,block_size)) {
               rv = false;
            }
         }
      }
      
      delete []page;
      page = nullptr;
   }
   return rv;
}



/* -------------------------------------------------------------------------- */
/*               READ ONE BLOCK of FLASH (use to actually write!)             
   PAY ATTENTION: although this function is public (and it is because it has to
   be called by the EEPROM class) it must never been used alone because it
   allocates a page that will not be deleted untill a write_block is issued.
   read_block must be always called followed by a write (as it is done in EEPROM
   class) or followed by delete_ram_page() (which is usually not available 
   because it is quite dangerous if wronlgy used)
   */
/* -------------------------------------------------------------------------- */
ReadStatus veeprom::read_block(uint32_t index, uint32_t &bytes_to_end_of_block) {
/* -------------------------------------------------------------------------- */      
   uint32_t block_size = DataFlashBlockDevice::getInstance().get_read_size();
   page_address = index & ~(block_size - 1);
   /* tell how many byte can be writte in this block starting from that address */
   bytes_to_end_of_block = page_address + block_size - index;

   if(page == nullptr) {
      page = new uint8_t[block_size];
      if(page != nullptr) {
         /* flag to remember if the page has to be deleted or not */
         write_page = false;
         erase_page = false;
         DataFlashBlockDevice::getInstance().read(page,page_address,block_size);
         return ReadStatus::ALLOCATED;
      }
      else {
         return ReadStatus::UNABLE_TO_ALLOC;
      }
   }
   else {
      return ReadStatus::ALREADY_ALLOCATED;
   }   
}




