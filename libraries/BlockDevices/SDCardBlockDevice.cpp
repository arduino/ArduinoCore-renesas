


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

volatile bool SDCardBlockDevice::card_inserted = false;
volatile bool SDCardBlockDevice::initialized = false;
volatile CmdStatus SDCardBlockDevice::st = CmdStatus::IN_PROGRESS;
            
extern "C" void r_sdhi_transfer_callback(sdhi_instance_ctrl_t *p_ctrl);

void g_sdmmc_dmac_callback(dmac_callback_args_t *p_args)
{
    r_sdhi_transfer_callback ((sdhi_instance_ctrl_t*) p_args->p_context);
}

#ifdef SDHI_DEBUG
#define PRINT_SIZE 32
extern void print_uint8(uint8_t n);

void print_buffer(uint8_t *b, uint32_t _size) {
 
  if(b != nullptr){   
      Serial.println("");
      for(int i = 0; i < _size; i++) {
        if(i != 0 && i % PRINT_SIZE == 0) {
          
         if(i != 0)
            Serial.println();
        }
        print_uint8(*(b + i)); 
        
      }
      Serial.println();
      Serial.println("");
  }
  
}
#endif




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
   write_block_size((bd_size_t)0),
   opened(false) {

   R_IOPORT_PinCfg(NULL, g_pin_cfg[ck].pin,  (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[cmd].pin, (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[d0].pin,  (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[d1].pin,  (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[d2].pin,  (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[d3].pin,  (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_DRIVE_HIGH | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   
   R_IOPORT_PinCfg(NULL, g_pin_cfg[cd].pin,  (uint32_t) ( IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));
   R_IOPORT_PinCfg(NULL, g_pin_cfg[wp].pin,  (uint32_t) ( IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SDHI_MMC));   
   
   

   
   cfg.bus_width = SDMMC_BUS_WIDTH_4_BITS;//SDMMC_BUS_WIDTH_4_BITS;
   cfg.channel = 0;
   cfg.p_callback = SDCardBlockDevice::SDCardBlockDeviceCbk;
   cfg.p_context = NULL;
   cfg.block_size = 512;
   cfg.card_detect = SDMMC_CARD_DETECT_CD;
   cfg.write_protect = SDMMC_WRITE_PROTECT_NONE;
   cfg.p_extend = NULL;
   #ifdef USE_DMAC
   cfg.p_lower_lvl_transfer = &g_transfer0;
   #endif
   #ifdef USE_DTC
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
   #endif


   #ifdef USE_DTC
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
         SDCardBlockDevice::card_inserted = false;
         SDCardBlockDevice::initialized = false;
         break;

      case SDMMC_EVENT_CARD_INSERTED: // Card inserted event
         SDCardBlockDevice::card_inserted = true;
         SDCardBlockDevice::initialized = false;
         break;
      case SDMMC_EVENT_RESPONSE: // Response event

         break;

      case SDMMC_EVENT_SDIO: // IO event

         break;

      case SDMMC_EVENT_TRANSFER_COMPLETE:  // Read or write complete
         SDCardBlockDevice::st = CmdStatus::SUCCESS;
         break;
      case SDMMC_EVENT_TRANSFER_ERROR: // Read or write failed
         SDCardBlockDevice::st = CmdStatus::FAILED;
         break;

      case  SDMMC_EVENT_ERASE_COMPLETE: // Erase completed
         SDCardBlockDevice::st = CmdStatus::SUCCESS;
         break;  

      case  SDMMC_EVENT_ERASE_BUSY: // Erase completed
         SDCardBlockDevice::st = CmdStatus::FAILED;
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
   fsp_err_t rv = FSP_SUCCESS;
   
   IRQManager::getInstance().addPeripheral(IRQ_SDCARD,&cfg);
   #ifdef USE_DMAC
   IRQManager::getInstance().addDMA(g_transfer0_extend);
   #endif

   #ifdef SDHI_DEBUG
   Serial.println("[CALL]: R_SDHI_Open");   
   #endif

   if(!opened) { 
      /* when opened is called and SD is opened and initialized, it reset all
         also initialization, so once opened... do not open again... */
      if(FSP_SUCCESS == R_SDHI_Open (&ctrl, &cfg)) {
         opened = true;
      }
   }
   
   if(opened && rv == FSP_SUCCESS && !SDCardBlockDevice::card_inserted) {
      R_BSP_SoftwareDelay(1U, BSP_DELAY_UNITS_MILLISECONDS);
      sdmmc_status_t status;
      status.card_inserted = 0;
      rv = R_SDHI_StatusGet (&ctrl, &status);
      if(status.card_inserted) {
         SDCardBlockDevice::card_inserted = true;
      } 
   }

   if(opened && rv == FSP_SUCCESS && !SDCardBlockDevice::initialized) {
      R_BSP_SoftwareDelay(1U, BSP_DELAY_UNITS_MILLISECONDS);
      #ifdef SDHI_DEBUG
      Serial.print("[CALL]: +++++++++++++ R_SDHI_MediaInit");
      #endif
      rv =  R_SDHI_MediaInit(&ctrl, &sd_card_info);
         #ifdef SDHI_DEBUG
         Serial.print(" result ");
         Serial.print((int)rv);
         #endif

      if(rv == FSP_SUCCESS) {
         #ifdef SDHI_DEBUG
         Serial.println(" - MediaInit SUCCESS");
         #endif
         SDCardBlockDevice::initialized = true;
         read_block_size = sd_card_info.sector_size_bytes;
         erase_block_size = sd_card_info.sector_size_bytes;
         write_block_size = sd_card_info.sector_size_bytes;
         total_size = sd_card_info.sector_count * sd_card_info.sector_size_bytes;
      }
   }

   #ifdef SDHI_DEBUG
   Serial.print("[LOG] Card inserted is: ");
   Serial.println(SDCardBlockDevice::card_inserted);  
   Serial.print("[LOG] Card initialized is: ");
   Serial.println(SDCardBlockDevice::initialized);     
   Serial.print("[LOG] Block size is: ");
   Serial.println(sd_card_info.sector_size_bytes);  
   Serial.print("[LOG] Number of block is: ");
   Serial.println(sd_card_info.sector_count);  
   Serial.print("[LOG] Total size is: ");
   Serial.print(total_size / (1024 * 1024));
   Serial.println(" MB");   
   Serial.println("******************************************************************************************");
   #endif
   

   return (int)rv;
}



/* -------------------------------------------------------------------------- */
/*                                  CLOSE                                     */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::close() {
   fsp_err_t rv = FSP_ERR_INVALID_ADDRESS;
   rv = R_SDHI_Close (&ctrl);
   opened = false;
   return (int)rv;
}

/* -------------------------------------------------------------------------- */
/* READ at the "logical" address 'add' 'size' bytes and copy them at the address
   specified by buffer
   NOTE: buffer MUST be equal or greater than 'size'                          */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::read(void *buffer, bd_addr_t add, bd_size_t _size) {
   fsp_err_t rv = FSP_ERR_INVALID_ADDRESS;
   
   #ifdef SDHI_DEBUG
   Serial.println("[CALL]: SDCardBlockDevice::read");
   #endif
   //Serial.println("--> A");
   if(open() == BLOCK_DEVICE_OK) {
      //Serial.println("--> B");
      if(SDCardBlockDevice::initialized) {
         //Serial.println("--> C");
         uint8_t *block = new uint8_t[read_block_size]; 
         if(block != nullptr) {
            //Serial.println("--> D");
            int64_t internal_size = (int64_t)_size;
            uint32_t block_num_start = (add / read_block_size);
            uint32_t byte_left_in_block =  read_block_size - (add % read_block_size);
            uint32_t start_copy_from = (add % read_block_size);
            uint8_t *dest = (uint8_t *)buffer; 
            #ifdef SDHI_DEBUG
            memset(block,0xAB,read_block_size);
            Serial.print("[LOG] block start is: ");
            Serial.println(block_num_start);  
            Serial.print("[LOG] byte_left_in_block is: ");
            Serial.println(byte_left_in_block);  
            Serial.print("[LOG] start_copy_from is: ");
            Serial.println(start_copy_from);
            #endif
            rv = FSP_SUCCESS;
            while(internal_size > 0 && rv == FSP_SUCCESS) {
               //Serial.println("--> E");
               uint32_t bytes_to_copy = (internal_size > byte_left_in_block) ? byte_left_in_block : (uint32_t)internal_size;
               #ifdef SDHI_DEBUG
               Serial.print("[LOG] bytes_to_copy is: ");
               Serial.println(bytes_to_copy); 
               Serial.print("[CALL] _____________________ R_SDHI_Read on block ");
               Serial.print(block_num_start);  
               #endif
               SDCardBlockDevice::st = CmdStatus::IN_PROGRESS;
               rv = R_SDHI_Read (&ctrl, block, block_num_start, 1);
               while(SDCardBlockDevice::st == CmdStatus::IN_PROGRESS) {
                  #ifdef SDHI_DEBUG
                  //Serial.println("reading ...");  
                  #endif
               }
               #ifdef SDHI_DEBUG
               Serial.print(" retun value ");
               Serial.print(rv); 
               if(rv == FSP_SUCCESS) {
                  Serial.println(" - SUCCESS");
                  //print_buffer(block, read_block_size);
               }
               Serial.println();
               #endif
               if(rv == FSP_SUCCESS) {
                  #ifdef SDHI_DEBUG
                  //Serial.println("------ MEMCPY ------");
                  #endif
                  memcpy(dest,block + start_copy_from, bytes_to_copy);
                  internal_size -= bytes_to_copy;
                  byte_left_in_block = read_block_size;
                  dest += bytes_to_copy;
                  start_copy_from = 0;
                  block_num_start++;
               }
            }
            //Serial.println("Delete");
            delete []block;
         }
      }
   }
   //Serial.println("--> Z");
   return (int)rv; 
}

/* -------------------------------------------------------------------------- */
/* WRITE 'size' byte form buffer to the "logical" address specified by 'addr' 
   NOTE: buffer MUST be equal or greater than 'size'                          */
/* -------------------------------------------------------------------------- */
int SDCardBlockDevice::write(const void *buffer, bd_addr_t add, bd_size_t _size) {
   fsp_err_t rv = FSP_ERR_INVALID_ADDRESS;
   #ifdef SDHI_DEBUG
   Serial.println("[CALL]: SDCardBlockDevice::write");
   #endif
   if(open() == BLOCK_DEVICE_OK) {
      if(SDCardBlockDevice::initialized) {
         uint8_t *block = new uint8_t[read_block_size]; 
         if(block != nullptr) {
            int64_t internal_size = (int64_t)_size;
            uint32_t block_num_start = (add / read_block_size);
            uint32_t byte_left_in_block =  read_block_size - (add % read_block_size);
            uint32_t start_copy_from = (add % read_block_size);
            uint8_t *source = (uint8_t *)buffer; 
            #ifdef SDHI_DEBUG
            memset(block,0xAB,read_block_size);
            Serial.print("[LOG] block start is: ");
            Serial.println(block_num_start);  
            Serial.print("[LOG] byte_left_in_block is: ");
            Serial.println(byte_left_in_block);  
            Serial.print("[LOG] start_copy_from is: ");
            Serial.println(start_copy_from);
            #endif
            rv = FSP_SUCCESS;
            while(internal_size > 0 && rv == FSP_SUCCESS) {
               uint32_t bytes_to_copy = (internal_size > byte_left_in_block) ? byte_left_in_block : (uint32_t)internal_size;
               #ifdef SDHI_DEBUG
               Serial.print("[LOG] bytes_to_copy is: ");
               Serial.println(bytes_to_copy); 
               Serial.print("[CALL] _____________________ R_SDHI_Read on block ");
               Serial.print(block_num_start);  
               #endif
               SDCardBlockDevice::st = CmdStatus::IN_PROGRESS;
               rv = R_SDHI_Read (&ctrl, block, block_num_start, 1);
               while(SDCardBlockDevice::st == CmdStatus::IN_PROGRESS) {
                  #ifdef SDHI_DEBUG
                  //Serial.println("reading ...");  
                  #endif
               }
               #ifdef SDHI_DEBUG
               Serial.print(" retun value ");
               Serial.print(rv); 
               if(rv == FSP_SUCCESS) {
                  Serial.println(" - SUCCESS");
                  //print_buffer(block, read_block_size);
               }
               Serial.println();
               #endif
               if(rv == FSP_SUCCESS) {
                  memcpy(block + start_copy_from, source, bytes_to_copy);
                  #ifdef SDHI_DEBUG 
                  Serial.print("[CALL] ^^^^^^^^^^^^^^^^^^^^^^^ R_SDHI_Write on block ");
                  Serial.print(block_num_start);  
                  #endif
                  SDCardBlockDevice::st = CmdStatus::IN_PROGRESS;
                  rv = R_SDHI_Write (&ctrl, block, block_num_start, 1);
                  while(SDCardBlockDevice::st == CmdStatus::IN_PROGRESS) {
                     #ifdef SDHI_DEBUG
                     //Serial.println("reading ...");  
                     #endif
                  }
                  #ifdef SDHI_DEBUG
                  Serial.print(" retun value ");
                  Serial.print(rv); 
                  if(rv == FSP_SUCCESS) {
                     Serial.println(" - SUCCESS");
                     //print_buffer(block, read_block_size);
                  }
                  Serial.println();
                  #endif
                  internal_size -= bytes_to_copy;
                  byte_left_in_block = read_block_size;
                  source += bytes_to_copy;
                  start_copy_from = 0;
                  block_num_start++;
               }
            }
            delete []block;
         }
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
   #ifdef SDHI_DEBUG
   Serial.println("[CALL]: SDCardBlockDevice::erase");
   #endif
   fsp_err_t rv = FSP_ERR_INVALID_ADDRESS; 
   if(open() == BLOCK_DEVICE_OK) {
      if(SDCardBlockDevice::initialized) {
         int64_t internal_size = (int64_t)_size;
         uint32_t block_num_start = (add / read_block_size);
         uint32_t byte_left_in_block =  read_block_size - (add % read_block_size);
         #ifdef SDHI_DEBUG
         Serial.print("[LOG] block start is: ");
         Serial.println(block_num_start);  
         Serial.print("[LOG] byte_left_in_block is: ");
         Serial.println(byte_left_in_block);  
         #endif
         rv = FSP_SUCCESS;
         while(internal_size > 0 && rv == FSP_SUCCESS) {
            #ifdef SDHI_DEBUG
            Serial.print("[CALL] ~~~~~~~~~~~~~~~~~~~~~~~~~ R_SDHI_Erase on block ");
            Serial.print(block_num_start);  
            #endif
            rv = R_SDHI_Erase (&ctrl, block_num_start, 1 );
            
            if(rv == FSP_SUCCESS) {
               int32_t timeout = 10000;
               sdmmc_status_t status;
               status.transfer_in_progress = 0;             
               do {
                  rv = R_SDHI_StatusGet (&ctrl, &status);
                  R_BSP_SoftwareDelay(10U, BSP_DELAY_UNITS_MILLISECONDS);
                  timeout--;
                  #ifdef SDHI_DEBUG
                  Serial.println("WAITING for erasing to finish ...");  
                  #endif
               } while(status.transfer_in_progress && timeout > 0);

               if(status.transfer_in_progress) {
                  rv = FSP_ERR_TIMEOUT;
               }
            }

            internal_size -= byte_left_in_block;
            byte_left_in_block = read_block_size;
            block_num_start++;
         }
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


/* -------------------------------------------------------------------------- */
/*                 AVAILABLE - tell if the SD is available or not             */
/* -------------------------------------------------------------------------- */
bool SDCardBlockDevice::available() {
   if(open() == BLOCK_DEVICE_OK) {
      if(SDCardBlockDevice::initialized) {
         return true;
      }
   }
   return false;
}


const char *SDCardBlockDevice::get_type() const {
    return "SDCARD";
}


#endif