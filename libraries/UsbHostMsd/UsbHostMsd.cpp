/* ########################################################################## */
/* - File: UsbHostMsd.h
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
#include "UsbHostMsd.h"


extern "C" int usb_host_msd_get_device_address();
extern "C" uint8_t usb_host_msd_get_default_lun();
extern "C" uint8_t usb_host_msd_get_lun_num();
extern "C" uint32_t usb_host_msd_get_num_of_blocks(uint8_t lun);
extern "C" uint32_t usb_host_msd_get_block_size(uint8_t lun);
extern "C" void usb_host_msd_attach_mnt_cbk(void (*fnc)(void));
extern "C" void usb_host_msd_attach_umnt_cbk(void (*fnc)(void));

/* -------------------------------------------------------------------------- */
/*                               CONSTRUCTOR                                  */
/* -------------------------------------------------------------------------- */
USBHostMSD::USBHostMSD() : selected_lun(-1), block_size(0) { }

/* -------------------------------------------------------------------------- */
/*                                   INIT                                     */
/* -------------------------------------------------------------------------- */
int USBHostMSD::init() {
   return BLOCK_DEVICE_OK;
}

/* -------------------------------------------------------------------------- */
/*                                   OPEN                                     */
/* -------------------------------------------------------------------------- */
int USBHostMSD::open() {
   return BLOCK_DEVICE_OK;
}

/* -------------------------------------------------------------------------- */
/*                                  DEINIT                                    */
/* -------------------------------------------------------------------------- */
int USBHostMSD::deinit() {
   return BLOCK_DEVICE_OK;
}

/* -------------------------------------------------------------------------- */
/*                                   CLOSE                                    */
/* -------------------------------------------------------------------------- */
int USBHostMSD::close() {
   return BLOCK_DEVICE_OK;
}

/* -------------------------------------------------------------------------- */
/*                         PROGRAM - 'remapped' on write                      */
/* -------------------------------------------------------------------------- */
int USBHostMSD::program(const void *buffer, bd_addr_t addr, bd_size_t _size) {
   return write(buffer, addr, _size);
}

/* -------------------------------------------------------------------------- */
/*    CONNECTED (if mount callback has been called and address is assigned)   */
/* -------------------------------------------------------------------------- */
bool USBHostMSD::connected() {
    if(usb_host_msd_get_device_address() >= 0) {
        if(tuh_msc_mounted(usb_host_msd_get_device_address())){
            block_size = (bd_size_t)usb_host_msd_get_block_size(get_lun());
            num_of_blocks = usb_host_msd_get_num_of_blocks(get_lun());
            total_size = block_size * num_of_blocks;
            return true;
        }
    }
    return false;
}

/* -------------------------------------------------------------------------- */
/*                 CONNECT - just remapped on CONNECTED                       */
/* -------------------------------------------------------------------------- */
bool USBHostMSD::connect() {
    return connected();
}

/* -------------------------------------------------------------------------- */
/*                                      */
/* -------------------------------------------------------------------------- */
static bool in_progress = false;
bool complete_cb(uint8_t dev_addr, tuh_msc_complete_data_t const * cb_data) {
    in_progress = false;
    return true;
}

/* -------------------------------------------------------------------------- */
/*                                WRITE                                     */
/* -------------------------------------------------------------------------- */
int USBHostMSD::write(const void *buffer, bd_addr_t addr, bd_size_t size) {
    uint32_t block_number, count;
    uint8_t *buf = (uint8_t *)buffer;
    if (connected()) {
        while(!tuh_msc_mounted(usb_host_msd_get_device_address())) {
            delay(10);
        }

        block_number =  addr / block_size;
        count = size / block_size;

        in_progress = true;
        if(tuh_msc_write10(usb_host_msd_get_device_address(), get_lun(), buf, block_number, count, complete_cb, 0)) {
            while (in_progress) {
                tuh_task();
            }
        }
        return 0;
    }
    return -1;
}

/* -------------------------------------------------------------------------- */
/*                                READ                                        */
/* -------------------------------------------------------------------------- */
int USBHostMSD::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    uint32_t block_number, count;
    uint8_t *buf = (uint8_t *)buffer;
    if (connected()) {
        while(!tuh_msc_mounted(usb_host_msd_get_device_address())) {
            delay(10);
        }

        block_number =  addr / block_size;
        count = size / block_size;

        in_progress = true;
        if(tuh_msc_read10(usb_host_msd_get_device_address(), get_lun(), buf, block_number, count, complete_cb, 0)) {
            while (in_progress) {
                tuh_task();
            }
        }
        return 0;
    }
    return -1;
}

/* -------------------------------------------------------------------------- */
/*        GET THE SELECTED lun or the default one if none is selected         */
/* -------------------------------------------------------------------------- */
uint8_t USBHostMSD::get_lun() {
    if(selected_lun == -1) {
        return usb_host_msd_get_default_lun();
    }
    else {
        return selected_lun;
    }
}

/* -------------------------------------------------------------------------- */
/* SELECT a LUN it return true if the selected lun is correct (lower than the max)         */
/* -------------------------------------------------------------------------- */
bool USBHostMSD::select_lun(uint8_t lun) {
    if(lun < usb_host_msd_get_lun_num()) {
        selected_lun = lun;
        return true;
    }
    return false;
}

/* -------------------------------------------------------------------------- */
/*           GET THE MAX NUMBER OF LUN OF THE DEVICE                          */
/* -------------------------------------------------------------------------- */
uint8_t USBHostMSD::get_lun_num() {
    return usb_host_msd_get_lun_num();
}

/* -------------------------------------------------------------------------- */
/*                                ERASE                                       */
/* -------------------------------------------------------------------------- */
int USBHostMSD::erase(bd_addr_t addr, bd_size_t size) {
    return 0;
}

/* -------------------------------------------------------------------------- */
/*                             READ SIZE                                      */
/* -------------------------------------------------------------------------- */
bd_size_t USBHostMSD::get_read_size() const {
    return (bd_size_t)block_size;
}

/* -------------------------------------------------------------------------- */
/*                             PROGRAM SIZE                                   */
/* -------------------------------------------------------------------------- */
bd_size_t USBHostMSD::get_program_size() const {
    return (bd_size_t)block_size;
}

/* -------------------------------------------------------------------------- */
/*                             ERASE   SIZE                                   */
/* -------------------------------------------------------------------------- */
bd_size_t USBHostMSD::get_erase_size() const {
    return (bd_size_t)block_size;
}

/* -------------------------------------------------------------------------- */
/*                             TOTAL   SIZE                                   */
/* -------------------------------------------------------------------------- */
bd_size_t USBHostMSD::size() const {
    return (bd_size_t)total_size;
}

/* -------------------------------------------------------------------------- */
/*                                 TYPE                                       */
/* -------------------------------------------------------------------------- */
const char *USBHostMSD::get_type() const {
    return "USBMSD";
}

bool USBHostMSD::attach_detected_callback(void (*cbk)()) {
    usb_host_msd_attach_mnt_cbk(cbk);
    return true;
}

bool USBHostMSD::attach_removed_callback(void (*cbk)()) {
    usb_host_msd_attach_umnt_cbk(cbk);
    return true;
}

