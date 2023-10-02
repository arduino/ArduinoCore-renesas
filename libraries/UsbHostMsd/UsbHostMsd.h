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

#ifndef USB_HOST_MSD_H
#define USB_HOST_MSD_H

#include "Arduino.h"

#include "BlockDevice.h"
#include "tusb.h"

/**
 * A class to communicate a USB flash disk
 */
class USBHostMSD : public BlockDevice {
public:
    USBHostMSD();
    bool connected();
    bool connect();
    virtual int init() override;
    virtual int deinit() override;
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size) override;
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size) override;
    virtual int erase(bd_addr_t addr, bd_size_t size) override;
    virtual bd_size_t get_read_size() const override; 
    virtual bd_size_t get_program_size() const override;
    virtual bd_size_t get_erase_size() const override;
    virtual bd_size_t size() const override;
    virtual const char *get_type() const override;
    virtual bool select_lun(uint8_t lun);
    virtual uint8_t get_lun_num();
    bool attach_detected_callback(void (*cbk)());
    bool attach_removed_callback(void (*cbk)());
    
private:
    uint8_t get_lun();
    int selected_lun;
    bd_size_t block_size;
    uint32_t  num_of_blocks;
    bd_size_t total_size; 

    virtual int open() override;
    virtual int close() override;
    virtual int write(const void *buffer, bd_addr_t addr, bd_size_t size) override;

    

};


#endif
