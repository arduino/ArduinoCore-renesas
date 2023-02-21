/* ########################################################################## */
/* - File: tu_msd.c
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


#include "tusb.h"
#include "UsbMsd.h"

#define MAX_LUN 1

#ifdef DEBUG_MSD
extern "C" int mylogadd(const char *fmt, ...) ;
#endif

static const uint8_t VENDOR_ID[8] = {'A', 'R', 'D', 'U', 'I', 'N', 'O', ' '};
static const uint8_t PRODUCT_ID[16] = {'M','A','S','S',' ','S','T','O','R','A','G','E',' ','D','E','V'};
static const uint8_t PRODUCT_REV[4] = {'0','0','0','1'};

static USBMSD *dev_ptr = NULL;
static bool device_available = false;

extern "C" bool tud_msc_set_sense(uint8_t lun, uint8_t sense_key, uint8_t add_sense_code, uint8_t add_sense_qualifier);

#ifdef cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
void usb_msd_set_dev_ptr(USBMSD *ptr) {
/* -------------------------------------------------------------------------- */   
   dev_ptr = ptr;
   if(dev_ptr != NULL) {
      device_available = true;
   }
   else {
      device_available = false;
   }
}

//--------------------------------------------------------------------+
// Application Callbacks (WEAK is optional)
//--------------------------------------------------------------------+

// Invoked when received SCSI READ10 command
// - Address = lba * BLOCK_SIZE + offset
//   - offset is only needed if CFG_TUD_MSC_EP_BUFSIZE is smaller than BLOCK_SIZE.
//
// - Application fill the buffer (up to bufsize) with address contents and return number of read byte. If
//   - read < bufsize : These bytes are transferred first and callback invoked again for remaining data.
//
//   - read == 0      : Indicate application is not ready yet e.g disk I/O busy.
//                      Callback invoked again with the same parameters later on.
//
//   - read < 0       : Indicate application error e.g invalid address. This request will be STALLed
//                      and return failed status in command status wrapper phase.
int32_t tud_msc_read10_cb (uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
   if(device_available) {
      int rv = dev_ptr->read(lun,lba, offset, buffer, bufsize);
      return rv;
   }
   else {
      return 0;
   }
}

// Invoked when received SCSI WRITE10 command
// - Address = lba * BLOCK_SIZE + offset
//   - offset is only needed if CFG_TUD_MSC_EP_BUFSIZE is smaller than BLOCK_SIZE.
//
// - Application write data from buffer to address contents (up to bufsize) and return number of written byte. If
//   - write < bufsize : callback invoked again with remaining data later on.
//
//   - write == 0      : Indicate application is not ready yet e.g disk I/O busy.
//                       Callback invoked again with the same parameters later on.
//
//   - write < 0       : Indicate application error e.g invalid address. This request will be STALLed
//                       and return failed status in command status wrapper phase.
//
// TODO change buffer to const uint8_t*
int32_t tud_msc_write10_cb (uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
   if(device_available) {
      int rv = dev_ptr->write(lun,lba, offset, buffer, bufsize);
      return rv;
   }
   else {
      return 0;
   }
}

// Invoked when received SCSI_CMD_INQUIRY
// Application fill vendor id, product id and revision with string up to 8, 16, 4 characters respectively
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
   memcpy(vendor_id,VENDOR_ID,8);
   memcpy(product_id,PRODUCT_ID,16);
   memcpy(product_rev,PRODUCT_REV,4);
}


/* -------------------------------------------------------------------------- */
// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
/* -------------------------------------------------------------------------- */
bool tud_msc_test_unit_ready_cb(uint8_t lun) {
/* -------------------------------------------------------------------------- */   
   if(device_available) {
      if(dev_ptr->available(lun)) {
         /* available no error */
         tud_msc_set_sense(0, 0, 0, 0);

         #ifdef DEBUG_MSD
         mylogadd("    OK!");
         #endif
         return true;
      }
      else {
         #ifdef DEBUG_MSD
         mylogadd(" ERROR");
         #endif
         /* Not Ready - medium not present */
         tud_msc_set_sense(0, 2, 0x3A, 0x00);
      }
   }
   return false;
}

// Invoked when received SCSI_CMD_READ_CAPACITY_10 and SCSI_CMD_READ_FORMAT_CAPACITY to determine the disk size
// Application update block count and block size
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size) {
   if(device_available) {
      *block_count = dev_ptr->get_block_count(lun);
      *block_size = dev_ptr->get_block_size(lun);
   }
   else {
      *block_count = 0;
      *block_count = 0;
   }
}

/**
 * Invoked when received an SCSI command not in built-in list below.
 * - READ_CAPACITY10, READ_FORMAT_CAPACITY, INQUIRY, TEST_UNIT_READY, START_STOP_UNIT, MODE_SENSE6, REQUEST_SENSE
 * - READ10 and WRITE10 has their own callbacks
 *
 * \param[in]   lun         Logical unit number
 * \param[in]   scsi_cmd    SCSI command contents which application must examine to response accordingly
 * \param[out]  buffer      Buffer for SCSI Data Stage.
 *                            - For INPUT: application must fill this with response.
 *                            - For OUTPUT it holds the Data from host
 * \param[in]   bufsize     Buffer's length.
 *
 * \return      Actual bytes processed, can be zero for no-data command.
 * \retval      negative    Indicate error e.g unsupported command, tinyusb will \b STALL the corresponding
 *                          endpoint and return failed status in command status wrapper phase.
 */
const uint8_t inquiry[] = { 0x00, 0x80, 0x00, 0x01,
                          36 - 4, 0x80, 0x00, 0x00,
                          'M', 'B', 'E', 'D', '.', 'O', 'R', 'G',
                          'M', 'B', 'E', 'D', ' ', 'U', 'S', 'B', ' ', 'D', 'I', 'S', 'K', ' ', ' ', ' ',
                          '1', '.', '0', ' ',};
const uint8_t sense6[] = { 0x03, 0x00, 0x00, 0x00 };                   

int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize) {
  int32_t resplen = 0;
  
  #ifdef DEBUG_MSD
  mylogadd("SCSI cb %X", scsi_cmd[0]);
  #endif

  switch (scsi_cmd[0]) {
    case 0x12: //INQUIRY
      resplen = sizeof(inquiry);
      memcpy(buffer, inquiry, (size_t) resplen);
      break;

    case 0x1A: //MODE_SENSE6      
      resplen = sizeof(sense6);
      memcpy(buffer, sense6, (size_t) resplen);
      break;  

    case 0x1E: //MEDIA REMOVAL
      resplen = 0;
      break;

    default:
      // Set Sense = Invalid Command Operation
      tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);

      // negative means error -> tinyusb could stall and/or response with failed status
      resplen = -1;
    break;
  }

  // return resplen must not larger than bufsize
  if ( resplen > bufsize ) resplen = bufsize;

  return (int32_t) resplen;
}

/*------------- Optional callbacks -------------*/

// Invoked when received GET_MAX_LUN request, required for multiple LUNs implementation
uint8_t tud_msc_get_maxlun_cb(void) {
   if(device_available) {
      return dev_ptr->get_lun();
   }
   else {
      return 0;
   }
}

bool tud_msc_is_writable_cb(uint8_t lun) {
   return true;
}


#ifdef cplusplus
}
#endif