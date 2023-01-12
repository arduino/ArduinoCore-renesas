/* ########################################################################## */
/* - File: FlashBlockDevice.cpp
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
#include "FlashBlockDevice.h"

/* -------------------------------------------------------------------------- */
/*                               CONSTRUCTOR                                  */
/* -------------------------------------------------------------------------- */
FlashBlockDevice::FlashBlockDevice() : 
   base_address((uint32_t)FLASH_BASE_ADDRESS),
   size((BdSize_t)FLASH_TOTAL_SIZE),
   block_size((BdSize_t)FLASH_BLOCK_SIZE),
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
FlashBlockDevice& FlashBlockDevice::getInstance() {
    static FlashBlockDevice    instance;
    return instance;
}

/* -------------------------------------------------------------------------- */
/*                              DISTRUCTOR                                    */
/* -------------------------------------------------------------------------- */
FlashBlockDevice::~FlashBlockDevice() {

}

/* -------------------------------------------------------------------------- */
/*                                  OPEN                                      */
/* -------------------------------------------------------------------------- */
int FlashBlockDevice::open() {
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
int FlashBlockDevice::close() {
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
int FlashBlockDevice::read(void *buffer, BdAddr_t add, BdSize_t size) {
   fsp_err_t rv = FSP_ERR_INVALID_POINTER;
   uint8_t *source = (uint8_t *)get_physical_address(add);
   uint8_t *dest = (uint8_t *)buffer;
   bool clean_up = false;
   
   if(buffer != nullptr) {
      if(!opened) {
         clean_up = true;
         rv = (fsp_err_t)open();
      }
   
      for(uint32_t i = 0; i < size; i++) {
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
int FlashBlockDevice::write(const void *buffer, BdAddr_t addr, BdSize_t size) {
   fsp_err_t rv = FSP_ERR_INVALID_POINTER;
   bool clean_up = false;

   if(!opened) {
      clean_up = true;
      rv = (fsp_err_t)open();
   }

   if(FLASH_write != nullptr){
      rv = FLASH_write(&ctrl, (uint32_t)buffer, (uint32_t)addr, (uint32_t)size);
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
bool FlashBlockDevice::is_address_correct(BdAddr_t add) {
   uint32_t ph_add = get_physical_address(add);
   if(ph_add >= (uint32_t)base_address && ph_add < ((uint32_t)base_address + (uint32_t)size))
      return true;

   return false;
}

/* -------------------------------------------------------------------------- */
/* Calculate the "physical" -initial- (starting) address of the flash block 
   containing the "logical" address 'add'                                     */
/* -------------------------------------------------------------------------- */
uint32_t FlashBlockDevice::get_block_starting_address(BdAddr_t add) {
   return ((uint32_t)base_address + (uint32_t)add) & ~(block_size - 1);
}

/* -------------------------------------------------------------------------- */
/* Calculate the "physical" address of the "logical" address 'add'            */
/* -------------------------------------------------------------------------- */
uint32_t FlashBlockDevice::get_physical_address(BdAddr_t add) {
   return (uint32_t)base_address + (uint32_t)add;
}

/* -------------------------------------------------------------------------- */
/* Check if the block containing the "logical" addres 'add' has been correctly
   blanked (erased)                                                           */
/* -------------------------------------------------------------------------- */
int FlashBlockDevice::check_blank(BdAddr_t add) {
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
int FlashBlockDevice::erase_block(BdAddr_t add) {
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
int FlashBlockDevice::erase(BdAddr_t add, BdSize_t size) {
   
   fsp_err_t rv = FSP_ERR_UNSUPPORTED; /* just to initialize to a value different from success */
   bool clean_up = false;
   int64_t delete_size = (int64_t)size;
   
   if(!opened) {
      clean_up = true;
      rv = (fsp_err_t)open();
   }

   if(rv == BLOCK_DEVICE_OK) {
      do {
         rv = (fsp_err_t)erase_block(add);
         size -= block_size;
         rv = (fsp_err_t)check_blank(add);
      } while(rv == BLOCK_DEVICE_OK && delete_size > 0);
   }

   if(clean_up) {
      close();
   }
   return (int)rv;
}

/* -------------------------------------------------------------------------- */
/*                             GET BLOCK SIZE                                 */
/* -------------------------------------------------------------------------- */
BdSize_t FlashBlockDevice::getBlockSize() const {
   return block_size;
}  

/* -------------------------------------------------------------------------- */
/*                             GET TOTAL SIZE OF FLASH AVAILABLE              */
/* -------------------------------------------------------------------------- */
BdSize_t FlashBlockDevice::getTotalSize() const {
   return size;
}