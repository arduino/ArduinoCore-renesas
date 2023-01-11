/*
  Copyright (c) 2022 Arduino LLC.  All right reserved.
  Author: Daniele Aimo (d.aimo@arduino.cc)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ARDUINO_EEPROM_API_H
#define ARDUINO_EEPROM_API_H



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

enum class ReadStatus { 
   UNABLE_TO_ALLOC,
   ALLOCATED,
   ALREADY_ALLOCATED
};

class ceeprom {
public:
   static ceeprom& getInstance();
   ceeprom(ceeprom const&)               = delete;
   void operator=(ceeprom const&)        = delete;
   ~ceeprom();
   bool open();
   void close();
   bool write();
   /* erase block @ address containing "logical" index*/
   bool erase_block(uint32_t index);
   #ifdef USE_ONLY_FOR_DEBUG
   void delete_ram_page();
   #endif
   uint8_t read_byte(uint32_t index);
   ReadStatus read_block(uint32_t index, uint32_t &bytes_to_end_of_block);
   /* write a byte **IN RAM** into the block read from flash */
   void write_byte(uint8_t v, uint32_t index);
   /* after a reset the access to the data flash is blocked 
      it is necessary to call the open function to re-enable it 
      open variable keeps track of that */
   bool opened; 
private:
   ceeprom();

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

   uint8_t *                    page;     
   bool                         write_page;
   bool                         erase_page;
   uint32_t                     page_address;                

   uint32_t                     get_physical_address(uint32_t index);
   /* erase block @ address */
   bool                         erase(uint32_t address);

};


#endif