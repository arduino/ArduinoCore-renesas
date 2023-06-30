/* ########################################################################## */
/* - File: DataFlashBlockDevice.h
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
#ifndef ARDUINO_FLASH_BLOCK_DEVICE
#define ARDUINO_FLASH_BLOCK_DEVICE

//#define DATA_FLASH_DEBUG

/* base class for block devices */
#include "BlockDevice.h"

/* Arduino.h to include the defines of the flash type LP or HP*/
#include "Arduino.h"

#include "fsp_common_api.h"
/* -------------------------------------------------------------------------- */
/*                              LP FLASH                                      */
/* -------------------------------------------------------------------------- */
#if (ARDUINO_FLASH_TYPE == LP_FLASH)
#include "r_flash_lp.h"
#endif

/* -------------------------------------------------------------------------- */
/*                              HP FLASH                                      */
/* -------------------------------------------------------------------------- */
#if (ARDUINO_FLASH_TYPE == HP_FLASH)
#include "r_flash_hp.h"
#endif

/* -------------------------------------------------------------------------- */
/*                        FUNCTION POINTE TYPES                               */
/* Note: the class uses function pointer to FSP function because FSP defines 
         2 different sets of flash access functions (one for LP flash and one
         for HP flash)                                                        */  
/* -------------------------------------------------------------------------- */
using FLASH_open_f                 = fsp_err_t (*)(flash_ctrl_t * const p_ctrl, flash_cfg_t const * const p_cfg);
using FLASH_write_f                = fsp_err_t (*)(flash_ctrl_t * const p_ctrl, uint32_t const src_address, uint32_t const flash_address, uint32_t const num_bytes);
using FLASH_erase_f                = fsp_err_t (*)(flash_ctrl_t * const p_ctrl, uint32_t const address, uint32_t const num_blocks);
using FLASH_blank_check_f          = fsp_err_t (*)(flash_ctrl_t * const p_ctrl, uint32_t const address, uint32_t const num_bytes, flash_result_t * const p_blank_check_result);
using FLASH_get_info_f             = fsp_err_t (*)(flash_ctrl_t * const p_ctrl, flash_info_t * const p_info);
using FLASH_close_f                = fsp_err_t (*)(flash_ctrl_t * const p_ctrl);
using FLASH_get_status_f           = fsp_err_t (*)(flash_ctrl_t * const p_ctrl, flash_status_t * const p_status);
using FLASH_set_window_access_f    = fsp_err_t (*)(flash_ctrl_t * const p_ctrl, uint32_t const start_addr, uint32_t const end_addr);
using FLASH_clear_window_access_f  = fsp_err_t (*)(flash_ctrl_t * const p_ctrl);
using FLASH_set_code_id_f          = fsp_err_t (*)(flash_ctrl_t * const p_ctrl, uint8_t const * const p_id_bytes, flash_id_code_mode_t mode);
using FLASH_reset_f                = fsp_err_t (*)(flash_ctrl_t * const p_ctrl);
using FLASH_update_ck_freq_f       = fsp_err_t (*)(flash_ctrl_t * const p_ctrl);
using FLASH_select_startup_area_f  = fsp_err_t (*)(flash_ctrl_t * const p_ctrl, flash_startup_area_swap_t swap_type, bool is_temporary);
using FLASH_swap_bank_f            = fsp_err_t (*)(flash_ctrl_t * const p_ctrl);
using FLASH_set_callback_f         = fsp_err_t (*)(flash_ctrl_t * const p_api_ctrl, void (* p_callback)(flash_callback_args_t *), void const * const p_context, flash_callback_args_t * const p_callback_memory);


/* -------------------------------------------------------------------------- */
/* CLASS DataFlashBlockDevice - to access micro internal flash                    */
/* -------------------------------------------------------------------------- */

class DataFlashBlockDevice : public BlockDevice {
private:
   DataFlashBlockDevice();
   int erase_block(bd_addr_t add);
   int check_blank(bd_addr_t add);
   uint32_t get_block_starting_address(bd_addr_t add);
   uint32_t get_physical_address(bd_addr_t add);
   bool is_address_correct(bd_addr_t add);
   virtual int write(const void *buffer, bd_addr_t addr, bd_size_t size) override;
   virtual int open() override;
   virtual int close() override;

   bd_addr_t base_address;
   bd_size_t total_size;
   bd_size_t block_size;

   /* pointers to FSP functions to be independent from the FSP API (LP or HP) */
   FLASH_open_f                 FLASH_open;
   FLASH_write_f                FLASH_write;
   FLASH_erase_f                FLASH_erase;
   FLASH_blank_check_f          FLASH_blank_check;
   FLASH_close_f                FLASH_close;
   #ifdef UNUSED_FSP_API_FUNCTIONS
   FLASH_get_info_f             FLASH_get_info;
   FLASH_get_status_f           FLASH_get_status;
   FLASH_set_window_access_f    FLASH_set_window_access;
   FLASH_clear_window_access_f  FLASH_clear_window_access;
   FLASH_set_code_id_f          FLASH_set_code_id;
   FLASH_reset_f                FLASH_reset;
   FLASH_update_ck_freq_f       FLASH_update_ck_freq;
   FLASH_select_startup_area_f  FLASH_select_startup_area;
   FLASH_swap_bank_f            FLASH_swap_bank;
   FLASH_set_callback_f         FLASH_set_callback;
   #endif
   
   #if (ARDUINO_FLASH_TYPE == LP_FLASH)
   flash_lp_instance_ctrl_t     ctrl;
   #endif
   
   #if (ARDUINO_FLASH_TYPE == HP_FLASH)
   flash_hp_instance_ctrl_t     ctrl;
   #endif

   flash_cfg_t                  cfg;
   bool                         opened;

public:
   static DataFlashBlockDevice& getInstance();

   DataFlashBlockDevice(DataFlashBlockDevice const&) = delete;
   void operator=(DataFlashBlockDevice const&) = delete;
   virtual ~DataFlashBlockDevice(); 
   virtual int init() override;
   virtual int deinit() override;
   virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size) override;
   virtual int read(void *buffer, bd_addr_t addr, bd_size_t size) override;
   virtual int erase(bd_addr_t addr, bd_size_t size) override;
   virtual bd_size_t get_program_size() const override;
   virtual bd_size_t get_erase_size() const override;
   virtual bd_size_t get_read_size() const override;
   virtual bd_size_t size() const override;
   virtual const char *get_type() const override;

};


#endif
