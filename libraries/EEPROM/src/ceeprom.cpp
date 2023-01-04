#include "ceeprom.h"

/*
  ceeprom.cpp - emulator of EEPROM using FLASH on ARDUINO Renesas FSP products
  
  Copyright (c): 2022 Arduino srl.  All right reserved.
  Author: Daniele Aimo (d.aimo@arduino.cc)

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
const uint32_t flash_base_address = (uint32_t)EEPROM_BASE_ADDRESS;

ceeprom& ceeprom::getInstance() {
    static ceeprom    instance;
    return instance;
}



/* -------------------------------------------------------------------------- */
/*                               CONSTRUCTOR                                  */
/* -------------------------------------------------------------------------- */
ceeprom::ceeprom() {
   FLASH_open                = nullptr;
   FLASH_write               = nullptr;
   FLASH_erase               = nullptr;
   FLASH_blank_check         = nullptr;
   FLASH_close               = nullptr;
#ifdef UNUSED_FSP_API_FUNCTIONS
   FLASH_get_info            = nullptr;
   FLASH_get_status          = nullptr;
   FLASH_set_window_access   = nullptr;
   FLASH_clear_window_access = nullptr;
   FLASH_set_code_id         = nullptr;
   FLASH_reset               = nullptr;
   FLASH_update_ck_freq      = nullptr;
   FLASH_select_startup_area = nullptr;
   FLASH_swap_bank           = nullptr;
   FLASH_set_callback        = nullptr;
#endif

#if (ARDUINO_FLASH_TYPE == LP_FLASH)
   FLASH_open                = R_FLASH_LP_Open;
   FLASH_write               = R_FLASH_LP_Write;
   FLASH_erase               = R_FLASH_LP_Erase;
   FLASH_blank_check         = R_FLASH_LP_BlankCheck;
   FLASH_close               = R_FLASH_LP_Close;
#ifdef UNUSED_FSP_API_FUNCTIONS
   FLASH_get_info            = R_FLASH_LP_InfoGet;
   FLASH_get_status          = R_FLASH_LP_StatusGet;
   FLASH_set_window_access   = R_FLASH_LP_AccessWindowSet;
   FLASH_clear_window_access = R_FLASH_LP_AccessWindowClear;
   FLASH_set_code_id         = R_FLASH_LP_IdCodeSet;
   FLASH_reset               = R_FLASH_LP_Reset;
   FLASH_update_ck_freq      = R_FLASH_LP_UpdateFlashClockFreq;
   FLASH_select_startup_area = R_FLASH_LP_StartUpAreaSelect;
   FLASH_swap_bank           = R_FLASH_LP_BankSwap;
   FLASH_set_callback        = R_FLASH_LP_CallbackSet;
#endif   
#endif

#if (ARDUINO_FLASH_TYPE == HP_FLASH)
   FLASH_open                = R_FLASH_HP_Open;
   FLASH_write               = R_FLASH_HP_Write;
   FLASH_erase               = R_FLASH_HP_Erase;
   FLASH_blank_check         = R_FLASH_HP_BlankCheck;
   FLASH_close               = R_FLASH_HP_Close;
#ifdef UNUSED_FSP_API_FUNCTIONS
   FLASH_get_info            = R_FLASH_HP_InfoGet;
   FLASH_get_status          = R_FLASH_HP_StatusGet;
   FLASH_set_window_access   = R_FLASH_HP_AccessWindowSet;
   FLASH_clear_window_access = R_FLASH_HP_AccessWindowClear;
   FLASH_set_code_id         = R_FLASH_HP_IdCodeSet;
   FLASH_reset               = R_FLASH_HP_Reset;
   FLASH_update_ck_freq      = R_FLASH_HP_UpdateFlashClockFreq;
   FLASH_select_startup_area = R_FLASH_HP_StartUpAreaSelect;
   FLASH_swap_bank           = R_FLASH_HP_BankSwap;
   FLASH_set_callback        = R_FLASH_HP_CallbackSet;
#endif   
#endif

   cfg.data_flash_bgo         = false; 
   cfg.p_callback             = nullptr; 
   cfg.p_context              = nullptr; 
   cfg.p_extend               = nullptr;
   cfg.ipl                    = (BSP_IRQ_DISABLED);
   cfg.irq                    = FSP_INVALID_VECTOR;
   cfg.err_ipl                = (BSP_IRQ_DISABLED);
   cfg.err_irq                = FSP_INVALID_VECTOR;

   page                       = nullptr;
   write_page                 = false;
   opened                     = false;

   
}

/* -------------------------------------------------------------------------- */
/*                              DISTRUCTOR                                    */
/* -------------------------------------------------------------------------- */
ceeprom::~ceeprom() {
   close();
   if(page != nullptr) {
      delete []page;
      page = nullptr;
   }
}

/* -------------------------------------------------------------------------- */
/*                                  CLOSE                                     */
/* -------------------------------------------------------------------------- */
void ceeprom::close() {
   if(FLASH_close) {
      FLASH_close(&ctrl);
      opened = false;
   }
}

/* -------------------------------------------------------------------------- */
/*                                  OPEN                                      */
/* -------------------------------------------------------------------------- */
bool ceeprom::open() { 
   bool rv = false;
   fsp_err_t err = FSP_ERR_UNSUPPORTED;
   if(FLASH_open && !opened) {
      err = FLASH_open(&ctrl,&cfg);
      rv = (FSP_SUCCESS == err) ? true : false;
      opened = rv;
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
/*                   WRITE SINGLE BYTE (in **RAM** page)                      */
/* -------------------------------------------------------------------------- */
void ceeprom::write_byte(uint8_t v, uint32_t index) {
   #ifdef EEPROM_DEBUG
   Serial.print("Write byte");      
   #endif
   while(index >= EEPROM_BLOCK) {
      index -= EEPROM_BLOCK;
   }
   if(page != nullptr) {
      if(!erase_page) {
         if(*(page + index) != 0xFF) {
            erase_page = true;
            #ifdef EEPROM_DEBUG
            Serial.print("Erase scheduled");      
            #endif
         }
      }
      if(!write_page) {   
         if(*(page + index) != v) {
            write_page = true;
            #ifdef EEPROM_DEBUG
            Serial.print("Write scheduled");      
            #endif
         }
      }
      *(page + index) = v;
   }
}

/* -------------------------------------------------------------------------- */
/*    ERASE THE BLOCK that contains the "physical" address 
      PAY ATTENTION: all data contained in the block (1024 bytes) will be
      erased! 
      PAY ATTENTION: for the correct evaluation of the blanck check is necessary
      that the address is the **FIRST** of the block (it is for this reason
      the function is private)
   -------------------------------------------------------------------------- */
bool ceeprom::erase(uint32_t address) {
/* -------------------------------------------------------------------------- */   
   fsp_err_t err = FSP_ERR_UNSUPPORTED; /* just to initialize to a value different from success */
   flash_result_t blank_check_result; 
   
   #ifdef EEPROM_DEBUG
   Serial.print("Erasing block @ address: ");      
   Serial.println(address, HEX);
   #endif
   open();
   if(FLASH_erase != nullptr) {
      err = FLASH_erase(&ctrl, address, 1);
      if(err == FSP_SUCCESS) {
         if(FLASH_blank_check != nullptr)
            err = FLASH_blank_check(&ctrl, address, EEPROM_BLOCK, &blank_check_result);
      }
   }
   close();
   if(err == FSP_SUCCESS && blank_check_result == FLASH_RESULT_BLANK) {
      return true;
   }
   else {
      return false;
   }

}


/* -------------------------------------------------------------------------- */
/*    ERASE THE BLOCK that contains the "logical" address index
      PAY ATTENTION: all data contained in the block (1024 bytes) will be
      erased!                                                                 */
/* -------------------------------------------------------------------------- */
bool ceeprom::erase_block(uint32_t index) {
   uint32_t block_start = get_physical_address(index) & ~(EEPROM_BLOCK - 1);
   return erase(block_start);
}

#ifdef USE_ONLY_FOR_DEBUG
/* NOTE: DO NOT USE THIS FUNCTION UNLESS YOU REALLY UNDERSTAND WHAT I DOING 
   THE USE this function can disrupt the FLASH writing algorithm 
   It has been defined only for debug */
/* -------------------------------------------------------------------------- */
void ceeprom::delete_ram_page() {
/* -------------------------------------------------------------------------- */   
   delete []page;
   page = nullptr;
}
#endif

/* -------------------------------------------------------------------------- */
/*                          WRITE PAGE (in **FLASH**)                         */
/* -------------------------------------------------------------------------- */
bool ceeprom::write() {
   bool rv = false;
   fsp_err_t err = FSP_ERR_UNSUPPORTED; /* just to initialize to a value different from success */
   bool actual_write = true;
   if(page != nullptr) {
      if(open() || opened) {
         if(write_page) {
            if(erase_page) {
               if(!erase(page_address)) {
                  /* if the page must be erase but the erasing fails avoid the writing */
                  actual_write = false;
               }
            }

            if(actual_write) {
               if(FLASH_write != nullptr){
                  #ifdef EEPROM_DEBUG
                  Serial.println("----------------------- Block write ----------------------------------");
                  for(int i = 0; i < EEPROM_BLOCK; i++) {
                     Serial.print((unsigned int)page[i],HEX);
                     Serial.print(" ");
                     if(i != 0 && i % 32 == 0) {
                        Serial.println();
                     }
                  }
                  Serial.println();
                  Serial.println("----------------------- Block write END -------------------------------");
                  #endif
                  err = FLASH_write(&ctrl, (uint32_t)page, page_address,EEPROM_BLOCK);
               }
               
               if(err == FSP_SUCCESS) {
                  rv = true;
               }
            }

            close();
         }
         else {
            rv = true;
         }
      }
      delete []page;
      page = nullptr;
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
/*                              READ SINGLE BYTE                              */
/* -------------------------------------------------------------------------- */
uint8_t ceeprom::read_byte(uint32_t index) {
/* -------------------------------------------------------------------------- */   
   open();
   uint8_t *ptr = (uint8_t *)get_physical_address(index);
   #ifdef EEPROM_DEBUG
   Serial.print("Reading single byte form flash: ");
   Serial.println((int)*ptr,HEX);
   #endif
   return *ptr;
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
ReadStatus ceeprom::read_block(uint32_t index, uint32_t &bytes_to_end_of_block) {
/* -------------------------------------------------------------------------- */   
   open();
   uint32_t block_start = get_physical_address(index) & ~(EEPROM_BLOCK - 1);
   /* tell how many byte can be writte in this block starting from that address */
   bytes_to_end_of_block = block_start + EEPROM_BLOCK - get_physical_address(index);

   #ifdef EEPROM_DEBUG
   Serial.print("Reading block @ address: ");      
   Serial.println(block_start, HEX);
   #endif
   
   if(page == nullptr) {
      page = new uint8_t[EEPROM_BLOCK];
      if(page != nullptr) {
         /* flag to remember if the page has to be deleted or not */
         write_page = false;
         erase_page = false;
         page_address = block_start;
         uint8_t *ptr = (uint8_t *)block_start;
         for(int i = 0; i < EEPROM_BLOCK; i++) {
            *(page + i) = *(ptr + i);
         }

         #ifdef EEPROM_DEBUG
         Serial.println("----------------------- Block read ----------------------------------");
         for(int i = 0; i < EEPROM_BLOCK; i++) {
            Serial.print((unsigned int)page[i],HEX);
            Serial.print(" ");
            if(i != 0 && i % 32 == 0) {
               Serial.println();
            }
         }
         Serial.println();
         Serial.println("----------------------- Block read END -------------------------------");
         #endif
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


/* -------------------------------------------------------------------------- */
uint32_t ceeprom::get_physical_address(uint32_t index) {
/* -------------------------------------------------------------------------- */   
   uint32_t add = flash_base_address + index;
   return add;
}

