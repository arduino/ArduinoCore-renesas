/* ########################################################################## */
/* - File: DataFlashBlockDevice.cpp
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
#include "DataFlashBlockDevice.h"

/* -------------------------------------------------------------------------- */
/*                               CONSTRUCTOR                                  */
/* -------------------------------------------------------------------------- */
DataFlashBlockDevice::DataFlashBlockDevice() : 
   base_address((uint32_t)FLASH_BASE_ADDRESS),
   total_size((bd_size_t)FLASH_TOTAL_SIZE),
   block_size((bd_size_t)FLASH_BLOCK_SIZE),
   opened(false) {
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
}


/* -------------------------------------------------------------------------- */
/*                    STATIC SINGLETONE - GET INSTANCE                        */
/* -------------------------------------------------------------------------- */
DataFlashBlockDevice& DataFlashBlockDevice::getInstance() {
    static DataFlashBlockDevice    instance;
    return instance;
}

/* -------------------------------------------------------------------------- */
/*                              DISTRUCTOR                                    */
/* -------------------------------------------------------------------------- */
DataFlashBlockDevice::~DataFlashBlockDevice() {

}

/* -------------------------------------------------------------------------- */
/*                              INIT - it calls open                          */
/* -------------------------------------------------------------------------- */
int DataFlashBlockDevice::init() {
   return open();
}

/* -------------------------------------------------------------------------- */
/*                            DEINIT - it calls close                         */
/* -------------------------------------------------------------------------- */
int DataFlashBlockDevice::deinit() {
   return close();
}

/* -------------------------------------------------------------------------- */
/*                         PROGRAM - 'remapped' on write                      */
/* -------------------------------------------------------------------------- */
int DataFlashBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t _size) {
   return write(buffer, addr, _size);
}

/* -------------------------------------------------------------------------- */
/*                                  OPEN                                      */
/* -------------------------------------------------------------------------- */
int DataFlashBlockDevice::open() {
   fsp_err_t rv = FSP_ERR_UNSUPPORTED;
   if(!opened) {
      if(FLASH_open) {
         rv = FLASH_open(&ctrl,&cfg);
         if(rv == FSP_SUCCESS) {
            opened = true;
         }
      }
   }
   return (int)rv;
}

/* -------------------------------------------------------------------------- */
/*                                  CLOSE                                     */
/* -------------------------------------------------------------------------- */
int DataFlashBlockDevice::close() {
   fsp_err_t rv = FSP_ERR_UNSUPPORTED;
   if(opened) {
      if(FLASH_close) {
         rv = FLASH_close(&ctrl);
         opened = false;
      }
   }
   return (int)rv;
}

/* -------------------------------------------------------------------------- */
/* READ at the "logical" address 'add' 'size' bytes and copy them at the address
   specified by buffer
   NOTE: buffer MUST be equal or greater than 'size'                          */
/* -------------------------------------------------------------------------- */
int DataFlashBlockDevice::read(void *buffer, bd_addr_t add, bd_size_t _size) {
   fsp_err_t rv = FSP_ERR_INVALID_POINTER;

   if(!is_address_correct(add + _size - 1) || !is_address_correct(add)) {
      #ifdef DATA_FLASH_DEBUG
      Serial.println("[DBG]");
      Serial.println("[DBG] !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! INVALID ADDRESS");
      Serial.println("[DBG]");
      #endif
      return (int)FSP_ERR_INVALID_ADDRESS;
   }

   uint8_t *source = (uint8_t *)get_physical_address(add);

   uint8_t *dest = (uint8_t *)buffer;
   bool clean_up = false;
   
   if(buffer != nullptr) {
      if(!opened) {
         clean_up = true;
         rv = (fsp_err_t)open();
      }
   
      for(uint32_t i = 0; i < _size; i++) {
         *(dest + i) = *(source + i);
      }

      if(clean_up) {
         close();
      }
   }
   return (int)rv; 

}

/* -------------------------------------------------------------------------- */
/* WRITE 'size' byte form buffer to the "logical" address specified by 'addr' 
   NOTE: buffer MUST be equal or greater than 'size'                          */
/* -------------------------------------------------------------------------- */
int DataFlashBlockDevice::write(const void *buffer, bd_addr_t addr, bd_size_t _size) {
   fsp_err_t rv = FSP_ERR_INVALID_POINTER;
   bool clean_up = false;

   if(!is_address_correct(addr + _size - 1) || !is_address_correct(addr)) {
      #ifdef DATA_FLASH_DEBUG
      Serial.println("[DBG]");
      Serial.println("[DBG] !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! INVALID ADDRESS");
      Serial.println("[DBG]");
      #endif
      return (int)FSP_ERR_INVALID_ADDRESS;
   }


   if(!opened) {
      clean_up = true;
      rv = (fsp_err_t)open();
   }
   
   if(FLASH_write != nullptr){
      rv = FLASH_write(&ctrl, (uint32_t)buffer, get_physical_address(addr), (uint32_t)_size);
      #ifdef DATA_FLASH_DEBUG
      Serial.print("[DBG] Write result ");
      Serial.println((uint32_t)rv);
      #endif
      
   }
   
   if(clean_up) {
      close();
   }

   
   return (int)rv;
}

/* -------------------------------------------------------------------------- */
/* Tells if the "logical" address add is correct (meaning it is equal or greater 
   of 'base_address' and lower of 'base_address' + 'size')                    */
/* -------------------------------------------------------------------------- */
bool DataFlashBlockDevice::is_address_correct(bd_addr_t add) {
   uint32_t ph_add = get_physical_address(add);
   if(ph_add >= (uint32_t)base_address && ph_add < ((uint32_t)base_address + (uint32_t)total_size))
      return true;

   return false;
}

/* -------------------------------------------------------------------------- */
/* Calculate the "physical" -initial- (starting) address of the flash block 
   containing the "logical" address 'add'                                     */
/* -------------------------------------------------------------------------- */
uint32_t DataFlashBlockDevice::get_block_starting_address(bd_addr_t add) {
   return ((uint32_t)base_address + (uint32_t)add) & ~(block_size - 1);
}

/* -------------------------------------------------------------------------- */
/* Calculate the "physical" address of the "logical" address 'add'            */
/* -------------------------------------------------------------------------- */
uint32_t DataFlashBlockDevice::get_physical_address(bd_addr_t add) {
   return (uint32_t)base_address + (uint32_t)add;
}

/* -------------------------------------------------------------------------- */
/* Check if the block containing the "logical" addres 'add' has been correctly
   blanked (erased)                                                           */
/* -------------------------------------------------------------------------- */
int DataFlashBlockDevice::check_blank(bd_addr_t add) {
   fsp_err_t rv = FSP_ERR_UNSUPPORTED; /* just to initialize to a value different from success */
   flash_result_t blank_check_result;

   uint32_t address = get_block_starting_address(add);

   if(FLASH_blank_check != nullptr)
      rv = FLASH_blank_check(&ctrl, address, block_size, &blank_check_result);

   return (rv == FSP_SUCCESS && blank_check_result == FLASH_RESULT_BLANK) ? BLOCK_DEVICE_OK : (int)rv;
}

/* -------------------------------------------------------------------------- */
/*    ERASE THE BLOCK that contains the "logical" address 'add'
      PAY ATTENTION: all data contained in the block (1024 bytes) will be
      erased! 
      PAY ATTENTION: for the correct evaluation of the blanck check is necessary
      that the address is the **FIRST** of the block (it is for this reason
      the function is private)
   -------------------------------------------------------------------------- */
int DataFlashBlockDevice::erase_block(bd_addr_t add) {
   fsp_err_t rv = FSP_ERR_UNSUPPORTED;
   
   uint32_t address = get_block_starting_address(add);

   if(FLASH_erase != nullptr) {
      rv = FLASH_erase(&ctrl, address, 1);
   }

   return (int)rv;
}

/* -------------------------------------------------------------------------- */
/*                                  ERASE                                     */
/* -------------------------------------------------------------------------- */
/* Note: erase can be performed only on a whole block. This function erase all
         the blocks that "fits" from addr to size */
int DataFlashBlockDevice::erase(bd_addr_t add, bd_size_t _size) {
   
   fsp_err_t rv = FSP_ERR_UNSUPPORTED; 

   if(!is_address_correct(add + _size - 1) || !is_address_correct(add)) {
      #ifdef DATA_FLASH_DEBUG
      Serial.println("[DBG]");
      Serial.println("[DBG] !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! INVALID ADDRESS");
      Serial.println("[DBG]");
      #endif
      return (int)FSP_ERR_INVALID_ADDRESS;
   }

   bool clean_up = false;
   int64_t delete_size = (int64_t)_size;
   
   if(!opened) {
      clean_up = true;
      rv = (fsp_err_t)open();
   }


   uint32_t erased_size = block_size - (add % block_size);

   if(rv == BLOCK_DEVICE_OK) {
      do {
         rv = (fsp_err_t)erase_block(add);
         delete_size -= erased_size;
         rv = (fsp_err_t)check_blank(add);
         erased_size = block_size;
      } while(rv == BLOCK_DEVICE_OK && delete_size > 0);
   }

   if(clean_up) {
      close();
   }
   return (int)rv;
}

/* -------------------------------------------------------------------------- */
/*                             GET BLOCK SIZEs                                */
/* -------------------------------------------------------------------------- */
bd_size_t DataFlashBlockDevice::get_program_size() const {
   return block_size;
}

bd_size_t DataFlashBlockDevice::get_erase_size() const {
   return block_size;
}

bd_size_t DataFlashBlockDevice::get_read_size() const {
   return block_size;
}
  
/* -------------------------------------------------------------------------- */
/*                             GET TOTAL SIZE OF FLASH AVAILABLE              */
/* -------------------------------------------------------------------------- */
bd_size_t DataFlashBlockDevice::size() const {
   return total_size;
}

const char *DataFlashBlockDevice::get_type() const {
    return "DATAFLASH";
}
