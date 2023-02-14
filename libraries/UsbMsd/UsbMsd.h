#ifndef _ARDUINO_USB_MASS_STORAGE_DEVICE_H
#define _ARDUINO_USB_MASS_STORAGE_DEVICE_H

#include "Arduino.h"
#include "blockDevice.h"

#define MSD_ERROR -1

class USBMSD {

private:
    BlockDevice *_bd;
    
public:    
    USBMSD() = delete;
    USBMSD(BlockDevice *bd);
    virtual ~USBMSD();
    int read(void *buffer, bd_addr_t addr, bd_size_t size);
    int write(const void *buffer, bd_addr_t addr, bd_size_t size);
    bool available();
    uint32_t get_block_count();
    uint16_t get_block_size();
    int read(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);
    int write(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize);
    bool begin();


};


#endif 