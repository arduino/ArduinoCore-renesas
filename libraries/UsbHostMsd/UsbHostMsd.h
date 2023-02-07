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

#include "FATFileSystem.h"
#include "blockDevice.h"

/**
 * A class to communicate a USB flash disk
 */
class USBHostMSD : public BlockDevice {
public:
    USBHostMSD();
    bool connected();
    bool connect();
    virtual int init();
    virtual int deinit();
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);
    virtual int erase(bd_addr_t addr, bd_size_t size);
    virtual bd_size_t get_read_size() const;
    virtual bd_size_t get_program_size() const;
    virtual bd_size_t get_erase_size() const;
    virtual bd_size_t size() const;
    virtual const char *get_type() const;
    



protected:
    //From IUSBEnumerator
    virtual void setVidPid(uint16_t vid, uint16_t pid);
    virtual bool parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol); //Must return true if the interface should be parsed
    //virtual bool useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir); //Must return true if the endpoint will be used


private:
    bool disk_init;

    virtual int open();
    virtual int close();
    virtual int write(const void *buffer, bd_addr_t addr, bd_size_t size);

    bd_size_t total_size;
    bd_size_t block_size;

};


#endif
