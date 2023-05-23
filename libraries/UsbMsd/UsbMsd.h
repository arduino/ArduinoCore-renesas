#ifndef _ARDUINO_USB_MASS_STORAGE_DEVICE_H
#define _ARDUINO_USB_MASS_STORAGE_DEVICE_H

#include "Arduino.h"
#include "BlockDevice.h"

#define MSD_ERROR -1

//#define DEBUG_MSD

class USBMSD {

private:
    BlockDevice **_bd;
    uint8_t *msd_buffer;
    uint32_t msd_buffer_size;
    int num;
    
public:    
    USBMSD() = delete;
    USBMSD(BlockDevice *bd);
    USBMSD(std::initializer_list<BlockDevice *> args);
    bool allocate_msd_buffer(uint32_t s);
    virtual ~USBMSD();
    int read(uint8_t lun, void *buffer, bd_addr_t addr, bd_size_t size);
    int write(uint8_t lun,const void *buffer, bd_addr_t addr, bd_size_t size);
    bool available(uint8_t lun);
    uint32_t get_block_count(uint8_t lun);
    uint16_t get_block_size(uint8_t lun);
    int read(uint8_t lun,uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);
    int write(uint8_t lun,uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize);
    bool begin(uint8_t lun);
    int get_lun() {return num;}


};


#endif 