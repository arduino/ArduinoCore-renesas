


/* ########################################################################## */
/* - File: SDCardBlockDevice.cpp
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


#include "SDCardBlockDevice.h"
#include "IRQManager.h"

#ifdef HAS_SDHI

bool SDCardBlockDevice::card_inserted = false;
bool SDCardBlockDevice::initialized = false;
            
extern "C" void r_sdhi_transfer_callback(sdhi_instance_ctrl_t *p_ctrl);

void g_sdmmc_dmac_callback(dmac_callback_args_t *p_args)
{
    r_sdhi_transfer_callback ((sdhi_instance_ctrl_t*) p_args->p_context);
}


/* -------------------------------------------------------------------------- */
/*                               CONSTRUCTOR                                  */
/* -------------------------------------------------------------------------- */
SDCardBlockDevice::SDCardBlockDevice(  pin_t _ck, 
                                       pin_t _cmd, 
                                       pin_t _d0, 
                                       pin_t _d1, 
                                       pin_t _d2, 
                                       pin_t _d3,
                                       pin_t _cd,
                                       pin_t _wp) : 
   ck(_ck), cmd(_cmd), d0(_d0), d1(_d1), d2(_d2), d3(_d3), cd(_cd), wp(_wp),
   base_address((uint32_t)0),
   total_size((bd_size_t)0),
   read_block_size((bd_size_t)0),
   erase_block_size((bd_size_t)0),
   write_block_size((bd_size_t)0) {

   R_IOPORT_PinCfg(NULL, g_pin_cfg[ck].pin,  (uint32_t) (IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[cmd].pin, (uint32_t) (IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[d0].pin,  (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[d1].pin,  (uint32_t) (IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[d2].pin,  (uint32_t) (IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[d3].pin,  (uint32_t) ( IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   
   //R_IOPORT_PinCfg(NULL, g_pin_cfg[cd].pin,  (uint32_t) (  IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   //R_IOPORT_PinCfg(NULL, g_pin_cfg[wp].pin,  (uint32_t) (  IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));   
   
   

   
   cfg.bus_width = SDMMC_BUS_WIDTH_4_BITS;//SDMMC_BUS_WIDTH_4_BITS;
   cfg.channel = 0;
   cfg.p_callback = SDCardBlockDevice::SDCardBlockDeviceCbk;
   cfg.p_context = NULL;
   cfg.block_size = 512;
   cfg.card_detect = SDMMC_CARD_DETECT_NONE;
   cfg.write_protect = SDMMC_WRITE_PROTECT_NONE;
   cfg.p_extend = NULL;
   #ifdef USE_DMAC
   cfg.p_lower_lvl_transfer = &g_transfer0;
   #else
   cfg.p_lower_lvl_transfer = &dtc_instance;
   #endif
   cfg.access_ipl = (12);
   cfg.sdio_ipl = BSP_IRQ_DISABLED;
   cfg.card_ipl = (BSP_IRQ_DISABLED);
   cfg.dma_req_ipl = (12);
   cfg.access_irq = FSP_INVALID_VECTOR;
   cfg.card_irq = FSP_INVALID_VECTOR;
   cfg.sdio_irq = FSP_INVALID_VECTOR;
   cfg.dma_req_irq = FSP_INVALID_VECTOR;

   
   #ifdef USE_DMAC

   g_transfer0_info.transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED;
   g_transfer0_info.transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE;
   g_transfer0_info.transfer_settings_word_b.irq = TRANSFER_IRQ_END;
   g_transfer0_info.transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED;
   g_transfer0_info.transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
   g_transfer0_info.transfer_settings_word_b.size = TRANSFER_SIZE_4_BYTE;
   g_transfer0_info.transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL;
   g_transfer0_info.p_dest = (void*) NULL;
   g_transfer0_info.p_src = (void const*) NULL;
   g_transfer0_info.num_blocks = 0;
   g_transfer0_info.length = 128;

   g_transfer0_extend.offset = 1;
   g_transfer0_extend.src_buffer_size = 1;
   g_transfer0_extend.irq = FSP_INVALID_VECTOR;
   g_transfer0_extend.ipl = (12);
   g_transfer0_extend.channel = 0;
   g_transfer0_extend.p_callback = g_sdmmc_dmac_callback;
   g_transfer0_extend.p_context = NULL;
   g_transfer0_extend.activation_source = ELC_EVENT_SDHIMMC0_DMA_REQ;

   g_transfer0_cfg.p_info = &g_transfer0_info;
   g_transfer0_cfg.p_extend = &g_transfer0_extend;

   g_transfer0.p_ctrl = &g_transfer0_ctrl;
   g_transfer0.p_cfg = &g_transfer0_cfg;
   g_transfer0.p_api = &g_transfer_on_dmac;

   #else

   dtc_info.transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED;
   dtc_info.transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE;
   dtc_info.transfer_settings_word_b.irq = TRANSFER_IRQ_END;
   dtc_info.transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED;
   dtc_info.transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED;
   dtc_info.transfer_settings_word_b.size = TRANSFER_SIZE_4_BYTE;
   dtc_info.transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL;
   dtc_info.p_dest = (void*) NULL;
   dtc_info.p_src = (void const*) NULL;
   dtc_info.num_blocks = 0;
   dtc_info.length = 128;
    
   

   dtc_cfg.p_info = &dtc_info; 
   dtc_cfg.p_extend = &dtc_ext_cfg;

   dtc_instance.p_ctrl = &dtc_ctrl; 
   dtc_instance.p_cfg = &dtc_cfg;
   dtc_instance.p_api = &g_transfer_on_dtc;

   #endif
   
   //fsp_err_t err =  R_DTC_Open(&g_transfer_ctrl, &g_transfer_cfg);

   initialized = false;
   card_inserted = false;
}


/* -------------------------------------------------------------------------- */
/*                              DISTRUCTOR                                    */
/* -------------------------------------------------------------------------- */
SDCardBlockDevice::~SDCardBlockDevice() {
   
}

/* -------------------------------------------------------------------------- */
/*                               CALLBACK                                     */
/* -------------------------------------------------------------------------- */
void SDCardBlockDevice::SDCardBlockDeviceCbk(sdmmc_callback_args_t *arg) {
   int open_status = -1;
   if(arg != nullptr) {
      sdmmc_event_t event = arg->event;

      switch(event) {
      case SDMMC_EVENT_CARD_REMOVED: // Card removed event.
         card_inserted = false;
         break;

      case SDMMC_EVENT_CARD_INSERTED: // Card inserted event
         card_inserted = true;
         initialized = false;
         break;
      case SDMMC_EVENT_RESPONSE: // Response event

         break;

      case SDMMC_EVENT_SDIO: // IO event

         break;

      case SDMMC_EVENT_TRANSFER_COMPLETE:  // Read or write complete

         break;
      case SDMMC_EVENT_TRANSFER_ERROR: // Read or write failed

         break;

      case  SDMMC_EVENT_ERASE_COMPLETE: // Erase completed
      
         break;  

      case  SDMMC_EVENT_ERASE_BUSY: // Erase completed
      
         break;  
      }

   }

}

/* -------------------------------------------------------------------------- */
/*                              INIT - it calls open                          */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::init() {
   return open();
}

/* -------------------------------------------------------------------------- */
/*                            DEINIT - it calls close                         */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::deinit() {
   return close();
}

/* -------------------------------------------------------------------------- */
/*                         PROGRAM - 'remapped' on write                      */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::program(const void *buffer, bd_addr_t addr, bd_size_t _size) {
   return write(buffer, addr, _size);
}

/* -------------------------------------------------------------------------- */
/*                                  OPEN                                      */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::open() {
   fsp_err_t rv;

   IRQManager::getInstance().addPeripheral(IRQ_SDCARD,&cfg);
   IRQManager::getInstance().addDMA(g_transfer0_extend);

   //dtc_ext_cfg.activation_source = cfg.dma_req_irq; 
    
   




   rv = R_SDHI_Open (&ctrl, &cfg);
   
   if((fsp_err_t)rv == FSP_SUCCESS || (fsp_err_t)rv == FSP_ERR_ALREADY_OPEN) {
      sdmmc_status_t status;
      if(!card_inserted) {
         rv = R_SDHI_StatusGet (&ctrl, &status);
         if(status.card_inserted) {
            card_inserted = true;
         }
      }
   }
   
   //if(card_inserted /*&& !initialized*/) {
      R_BSP_SoftwareDelay(1U, BSP_DELAY_UNITS_MILLISECONDS);
      rv =  R_SDHI_MediaInit(&ctrl, NULL);
         //Serial.print("MEDIA INIT RESULT ");
         //Serial.println((int)rv);

      if(rv == FSP_SUCCESS) {
         //Serial.println("MEDIA INIT SUCCESS");
         initialized = true;
      }
   //}

   return (int)rv;
}



/* -------------------------------------------------------------------------- */
/*                                  CLOSE                                     */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::close() {
   fsp_err_t rv = FSP_ERR_INVALID_ADDRESS;
   rv = R_SDHI_Close (&ctrl);
   return (int)rv;
}

/* -------------------------------------------------------------------------- */
/* READ at the "logical" address 'add' 'size' bytes and copy them at the address
   specified by buffer
   NOTE: buffer MUST be equal or greater than 'size'                          */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::read(void *buffer, bd_addr_t add, bd_size_t _size) {
   fsp_err_t rv = FSP_ERR_INVALID_ADDRESS;
   if(open() == BLOCK_DEVICE_OK) {
      if(initialized) {
         
      }
   }

   

   return (int)rv; 
}

/* -------------------------------------------------------------------------- */
/* WRITE 'size' byte form buffer to the "logical" address specified by 'addr' 
   NOTE: buffer MUST be equal or greater than 'size'                          */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::write(const void *buffer, bd_addr_t add, bd_size_t _size) {
   fsp_err_t rv = FSP_ERR_INVALID_ADDRESS;
   if(open() == BLOCK_DEVICE_OK) {
      if(initialized) {
         
      }
   }
   return (int)rv;
}

/* -------------------------------------------------------------------------- */
/* Tells if the "logical" address add is correct                              */
/* -------------------------------------------------------------------------- */
bool SDCardBlockDevice::is_address_correct(bd_addr_t add) {
   return (add < total_size) ? true : false;
}

/* -------------------------------------------------------------------------- */
/*                                  ERASE                                     */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::erase(bd_addr_t add, bd_size_t _size) {
   
   fsp_err_t rv = FSP_ERR_INVALID_ADDRESS; 
   if(open() == BLOCK_DEVICE_OK) {
      if(initialized) {
         
      }
   }
   
   return (int)rv;
}

/* -------------------------------------------------------------------------- */
/*                             GET BLOCK SIZEs                                */
/* -------------------------------------------------------------------------- */
bd_size_t SDCardBlockDevice::get_program_size() const {
   return write_block_size;
}

bd_size_t SDCardBlockDevice::get_erase_size() const {
   return erase_block_size;
}

bd_size_t SDCardBlockDevice::get_read_size() const {
   return read_block_size;
}
/* -------------------------------------------------------------------------- */
/*                      GET TOTAL SIZE OF FLASH AVAILABLE                     */
/* -------------------------------------------------------------------------- */
bd_size_t SDCardBlockDevice::size() const {
   return total_size;
}

const char *SDCardBlockDevice::get_type() const {
    return "SDCARD";
}


#endif