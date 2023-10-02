/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "tusb.h"

//#define USB_DEBUG

#if CFG_TUH_MSC


typedef struct tag_usb_msc_device {
  uint32_t num_of_block;
  uint32_t block_size;
} usb_msc_device_t;



static int device_address = -1;
static uint32_t num_of_block = 0;
static uint32_t block_size = 0;
static uint8_t max_lun = 0;

static usb_msc_device_t *usb_msc_device = NULL;

int usb_host_msd_get_device_address() { return device_address; }
uint8_t usb_host_msd_get_default_lun() { return 0; }
uint8_t usb_host_msd_get_lun_num() { return 0; }


uint32_t usb_host_msd_get_num_of_blocks(uint8_t lun) { 
  if(usb_msc_device != 0) {
    if(lun < max_lun) {
      return usb_msc_device[lun].num_of_block;
    }
  }
  return 0; 
}

uint32_t usb_host_msd_get_block_size(uint8_t lun) { 
  if(usb_msc_device != 0) {
    if(lun < max_lun) {
      return usb_msc_device[lun].block_size;
    }
  }
  return 0; 
}

static void (*mount_fnc)(void) = NULL;
static void (*unmount_fnc)(void) = NULL;

void usb_host_msd_attach_mnt_cbk(void (*fnc)(void)) {
  mount_fnc = fnc;
}

void usb_host_msd_attach_umnt_cbk(void (*fnc)(void)) {
  unmount_fnc = fnc;
}

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM DECLARATION
//--------------------------------------------------------------------+
static scsi_inquiry_resp_t inquiry_resp;

bool inquiry_complete_cb(uint8_t dev_addr, tuh_msc_complete_data_t const * cb_data)
{
  msc_cbw_t const* cbw = cb_data->cbw;
  msc_csw_t const* csw = cb_data->csw;

  if (csw->status != 0)
  {
    TU_LOG2("Inquiry failed\r\n");
    return false;
  }

  // Print out Vendor ID, Product ID and Rev
  TU_LOG2("%.8s %.16s rev %.4s\r\n", inquiry_resp.vendor_id, inquiry_resp.product_id, inquiry_resp.product_rev);

  // Get capacity of device
  uint32_t const block_count = tuh_msc_get_block_count(dev_addr, cbw->lun);
  uint32_t const block_size = tuh_msc_get_block_size(dev_addr, cbw->lun);

  TU_LOG2("Disk Size: %lu MB\r\n", block_count / ((1024*1024)/block_size));
  TU_LOG2("Block Count = %lu, Block Size: %lu\r\n", block_count, block_size);

  return true;
}

//------------- IMPLEMENTATION -------------//
void tuh_msc_mount_cb(uint8_t dev_addr) {
  TU_LOG2("[EVENT]: Mass Storage Device - MOUNT -> device address %i", dev_addr); 

  uint8_t const lun = 0;
  tuh_msc_inquiry(dev_addr, lun, &inquiry_resp, inquiry_complete_cb, 0);

  max_lun = tuh_msc_get_maxlun(dev_addr);

  TU_LOG2("       : Max LUN %i", dev_addr); 

  if(max_lun > 0) {
    if(usb_msc_device != NULL) {
      free(usb_msc_device);
      usb_msc_device = NULL;
    }
    usb_msc_device = (usb_msc_device_t *)malloc(sizeof(usb_msc_device_t) * max_lun);
    
    if(usb_msc_device != NULL) {
      for(int i = 0; i < max_lun; i++) {
        usb_msc_device[i].num_of_block = tuh_msc_get_block_count(dev_addr, i);
        usb_msc_device[i].block_size = tuh_msc_get_block_size(dev_addr, i);
      }
    }
    device_address = dev_addr;

    if(mount_fnc != NULL) {
      mount_fnc();
    }
  }

  

  //uint8_t const lun = 0;
  //tuh_msc_inquiry(dev_addr, lun, &inquiry_resp, inquiry_complete_cb);
//
//  //------------- file system (only 1 LUN support) -------------//
//  uint8_t phy_disk = dev_addr-1;
//  disk_initialize(phy_disk);
//
//  if ( disk_is_ready(phy_disk) )
//  {
//    if ( f_mount(phy_disk, &fatfs[phy_disk]) != FR_OK )
//    {
//      puts("mount failed");
//      return;
//    }
//
//    f_chdrive(phy_disk); // change to newly mounted drive
//    f_chdir("/"); // root as current dir
//
//    cli_init();
//  }
}

void tuh_msc_umount_cb(uint8_t dev_addr) {
  (void) dev_addr;
  TU_LOG2("[CALL]: tuh_msc_umount_cb %i --------- UMOUNT", dev_addr);  
  if(usb_msc_device != NULL) {
    free(usb_msc_device);
    usb_msc_device = NULL;
  }

  device_address = -1;

  if (unmount_fnc != NULL) {
    unmount_fnc();
  }

//  uint8_t phy_disk = dev_addr-1;
//
//  f_mount(phy_disk, NULL); // unmount disk
//  disk_deinitialize(phy_disk);
//
//  if ( phy_disk == f_get_current_drive() )
//  { // active drive is unplugged --> change to other drive
//    for(uint8_t i=0; i<CFG_TUH_DEVICE_MAX; i++)
//    {
//      if ( disk_is_ready(i) )
//      {
//        f_chdrive(i);
//        cli_init(); // refractor, rename
//      }
//    }
//  }
}

#endif
