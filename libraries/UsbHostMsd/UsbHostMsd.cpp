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




#define CBW_SIGNATURE   0x43425355
#define CSW_SIGNATURE   0x53425355

#define DEVICE_TO_HOST  0x80
#define HOST_TO_DEVICE  0x00

#define GET_MAX_LUN             (0xFE)
#define BO_MASS_STORAGE_RESET   (0xFF)

USBHostMSD::USBHostMSD()
{
    
}



bool USBHostMSD::connected()
{
    return false;
}

bool USBHostMSD::connect()
{

   
    return false;
}

/*virtual*/ void USBHostMSD::setVidPid(uint16_t vid, uint16_t pid)
{
    // we don't check VID/PID for MSD driver
}

/*virtual*/ bool USBHostMSD::parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol) //Must return true if the interface should be parsed
{
    
    return false;
}

/*virtual*/ //bool USBHostMSD::useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir) //Must return true if the endpoint will be used
//{
    
    //return false;
//}

/*
int USBHostMSD::testUnitReady()
{
    return 0;
}


int USBHostMSD::readCapacity()
{
    return 0;
}


int USBHostMSD::SCSIRequestSense()
{
    return 0;
}


int USBHostMSD::inquiry(uint8_t lun, uint8_t page_code)
{
    
    return 0;
}

int USBHostMSD::checkResult(uint8_t res, USBEndpoint * ep)
{
    

    return 0;
}


int USBHostMSD::SCSITransfer(uint8_t * cmd, uint8_t cmd_len, int flags, uint8_t * data, uint32_t transfer_len)
{

    
    

    return 0;
}


int USBHostMSD::dataTransfer(uint8_t * buf, uint32_t block, uint8_t nbBlock, int direction)
{
   return 0;
}

int USBHostMSD::getMaxLun()
{
    return 0;
}
*/
int USBHostMSD::init()
{
    return 0;
}

int USBHostMSD::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
    uint32_t block_number, count;
    uint8_t *buf = (uint8_t *)buffer;
    if (!disk_init) {
        init();
    }
    if (!disk_init) {
        return -1;
    }
    block_number =  addr / block_size;
    count = size /block_size;

    for (uint32_t b = block_number; b < block_number + count; b++) {
        /*
        if (dataTransfer(buf, b, 1, HOST_TO_DEVICE))
            return -1;
        */
        buf += block_size;
    }
    return 0;
}

int USBHostMSD::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    uint32_t block_number, count;
    uint8_t *buf = (uint8_t *)buffer;
    if (!disk_init) {
        init();
    }
    if (!disk_init) {
        return -1;
    }
    block_number =  addr / block_size;
    count = size / block_size;

    for (uint32_t b = block_number; b < block_number + count; b++) {
        /*
        if (dataTransfer(buf, b, 1, DEVICE_TO_HOST))
            return -1;
        */
        buf += block_size;
    }
    return 0;
}

int USBHostMSD::erase(bd_addr_t addr, bd_size_t size)
{
    return 0;
}

bd_size_t USBHostMSD::get_read_size() const {
    return (disk_init ? (bd_size_t)block_size : -1);
}

bd_size_t USBHostMSD::get_program_size() const
{
    return (disk_init ? (bd_size_t)block_size : -1);
}
bd_size_t USBHostMSD::get_erase_size() const
{
    return (disk_init ? (bd_size_t)block_size : -1);
}

bd_size_t USBHostMSD::size() const
{
    //USB_DBG("FILESYSTEM: size ");
    return (disk_init ? (bd_size_t)block_size : 0);
}

const char *USBHostMSD::get_type() const
{
    return "USBMSD";
}

