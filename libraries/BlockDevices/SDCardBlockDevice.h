/* ########################################################################## */
/* - File: SDCardBlockDevice.h
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
#ifndef ARDUINO_SDMMC_FLASH_BLOCK_DEVICE
#define ARDUINO_SDMMC_FLASH_BLOCK_DEVICE

/* base class for block devices */
#include "BlockDevice.h"

/* Arduino.h to include the defines of the flash type LP or HP*/
#include "Arduino.h"
#include "storage_retarget.h"

#define DMAC_CHANNEL_USED 2
//#define SDHI_DEBUG
#define USE_DMAC


#ifdef HAS_SDHI

#include "r_sdhi.h"
#ifdef USE_DMAC
#include "r_dmac.h"
#else
#include "r_dtc.h"
#endif

enum class CmdStatus {
  IN_PROGRESS,
  FAILED,
  SUCCESS
} ;


/* NOTE: please note that the current implementation is suited only for 1 SD CARD
   Block device instance: this is mainly due to the fact to the presence of the
   static variables (for example initialized, cart_inserted and so on)
   if necessary this class has to be updated using the channel and "duplicating"
   these information */

/* -------------------------------------------------------------------------- */
/* CLASS SDCardBlockDevice - to access micro internal flash                    */
/* -------------------------------------------------------------------------- */

class SDCardBlockDevice : public BlockDevice {
private:
  sdmmc_device_t sd_card_info;
  bd_addr_t base_address;
  bd_size_t total_size;
  bd_size_t read_block_size;
  bd_size_t erase_block_size;
  bd_size_t write_block_size;
  sdhi_instance_ctrl_t  ctrl;
  sdmmc_cfg_t           cfg;
  
  #ifdef USE_DMAC
  dmac_instance_ctrl_t g_transfer0_ctrl;
  transfer_info_t      g_transfer0_info;
  dmac_extended_cfg_t  g_transfer0_extend;
  transfer_cfg_t       g_transfer0_cfg;
  transfer_instance_t  g_transfer0;
  #endif
  
  #ifdef USE_DTC
  dtc_instance_ctrl_t   dtc_ctrl;
  dtc_extended_cfg_t    dtc_ext_cfg;    
  transfer_info_t       dtc_info;
  transfer_cfg_t        dtc_cfg;
  transfer_instance_t   dtc_instance;
  #endif
  pin_t ck;
  pin_t cmd;
  pin_t d0;
  pin_t d1;
  pin_t d2;
  pin_t d3;
  pin_t cd;
  pin_t wp;
  static volatile bool initialized;
  static volatile bool card_inserted;
  static volatile CmdStatus st;
  static void SDCardBlockDeviceCbk(sdmmc_callback_args_t *);
  virtual int write(const void *buffer, bd_addr_t addr, bd_size_t size) override;
  virtual int open() override;
  virtual int close() override;
  bool opened;
  fsp_err_t wait_for_completition();
public:

  SDCardBlockDevice( pin_t _ck, 
                     pin_t _cmd, 
                     pin_t _d0, 
                     pin_t _d1, 
                     pin_t _d2, 
                     pin_t _d3,
                     pin_t _cd,
                     pin_t _wp);
  
  SDCardBlockDevice(SDCardBlockDevice const&) = delete;
  void operator=(SDCardBlockDevice const&) = delete;
  virtual ~SDCardBlockDevice(); 

  
  virtual int init() override;
  virtual int deinit() override; 
  virtual int read(void *buffer, bd_addr_t addr, bd_size_t size) override;
  virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size) override;
  virtual int erase(bd_addr_t addr, bd_size_t size) override;
  virtual bd_size_t get_program_size() const override;
  virtual bd_size_t get_erase_size() const override;
  virtual bd_size_t get_read_size() const override;
  virtual bd_size_t size() const override;
  virtual const char *get_type() const override;
  virtual bool available() override;
  bool is_card_present() { return card_inserted; }
  bool is_card_initialized() { return initialized; }

};

#endif

#endif
