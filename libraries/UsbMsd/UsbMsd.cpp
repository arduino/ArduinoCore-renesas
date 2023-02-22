#include "UsbMsd.h"

/* -------------------------------------------------------------------------- */
/* extern "C" functions --> defined in tu_msd.c                               */
/* -------------------------------------------------------------------------- */
#ifdef DEBUG_MSD
extern "C" int mylogadd(const char *fmt, ...) ;
#endif

void usb_msd_set_dev_ptr(USBMSD *prt);

/* -------------------------------------------------------------------------- */
/* Add Mass Storage USB capability when present                               */
/* -------------------------------------------------------------------------- */

void __USBInstallMSD() {}


USBMSD::USBMSD(std::initializer_list<BlockDevice *> args) : msd_buffer(nullptr), msd_buffer_size(0) {
    _bd = new BlockDevice *[args.size()];
    num = args.size();

    for(int i = 0; i < num; i++) {
        _bd[i] = args.begin()[i];
    }
    usb_msd_set_dev_ptr(this);
}

/* -------------------------------------------------------------------------- */
/* CONSTRUCTOR -> initalize block device and pass a "hook" to the tu_msd module */
/* -------------------------------------------------------------------------- */
USBMSD::USBMSD(BlockDevice *bd) : msd_buffer(nullptr), msd_buffer_size(0) {
    _bd = new BlockDevice *[0];
    _bd[0] = bd;
    num = 1;
    usb_msd_set_dev_ptr(this);

}

bool USBMSD::begin(uint8_t lun) {
    if(lun < num){

       if(_bd[lun]->init() == 0) {

         return true;
       }
    }
    return false;
    
}

/* -------------------------------------------------------------------------- */
/* DISTRUCTOR                                                                 */
/* -------------------------------------------------------------------------- */
USBMSD::~USBMSD() {
    delete []_bd;
    if(msd_buffer != nullptr) {
        delete []msd_buffer;
    }
}

/* -------------------------------------------------------------------------- */
/* AVAILABLE                                                                  */
/* -------------------------------------------------------------------------- */
bool USBMSD::available(uint8_t lun) {
    if(!begin(lun))
        return false;
    if(lun < num) {
        return _bd[lun]->available();
    }   
    return false;
}

/* -------------------------------------------------------------------------- */
/* GET BLOCK COUNT                                                            */
/* -------------------------------------------------------------------------- */
uint32_t USBMSD::get_block_count(uint8_t lun) {
    if(!begin(lun))
        return 0;
    if(lun < num) {
        return (_bd[lun]->size() / _bd[lun]->get_erase_size());
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
/* GET BLOCK SIZE                                                             */
/* -------------------------------------------------------------------------- */
uint16_t USBMSD::get_block_size(uint8_t lun) {
    if(!begin(lun))
        return 0;
    return _bd[lun]->get_erase_size();
}

bool USBMSD::allocate_msd_buffer(uint32_t s) {
    if(msd_buffer != nullptr && s == msd_buffer_size) {
        return true;
    }

    if(msd_buffer != nullptr) {
        delete []msd_buffer;
        msd_buffer = nullptr;
    }

    msd_buffer = new uint8_t[s];

    if(msd_buffer != nullptr) {
        msd_buffer_size = s;
        return true;
    }

    return false;
}

/* -------------------------------------------------------------------------- */
/* READ                                                                       */
/* -------------------------------------------------------------------------- */
int USBMSD::read(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    
    if(!begin(lun)) {
        #ifdef DEBUG_MSD
        mylogadd("READ FAILED 1 %i %i %i %i", lun, lba, offset, bufsize) ;
        #endif
        return MSD_ERROR;
    }

    uint32_t size_to_return = bufsize;
    bd_size_t block_size = _bd[lun]->get_erase_size();
    bd_addr_t requested_address = (bd_addr_t)((lba * block_size) + offset);
    uint32_t block_offset = (requested_address % block_size);
    bd_addr_t block_address = requested_address - block_offset;
    uint8_t *buf_out = (uint8_t *)buffer;

    #ifdef DEBUG_MSD
    //mylogadd("READ %i %i %i %i - %i", lun, lba, offset, bufsize, requested_address) ;
    #endif


    if(!allocate_msd_buffer(block_size)) {
        return MSD_ERROR;
    }

    int retval = 0;

    while(bufsize > 0 && retval == 0) {
        retval = _bd[lun]->read(msd_buffer, block_address, block_size);
        uint32_t bytes_to_copy = bufsize > (block_size - block_offset) ? (block_size - block_offset) : bufsize;
        
        for(int i = 0; i < bytes_to_copy; i++) {
            *(buf_out+i) = *(msd_buffer + block_offset + i);
        }


        //memcpy(buf_out, msd_buffer + block_offset, bytes_to_copy);
        block_offset = 0;
        block_address += block_size;
        bufsize -= bytes_to_copy;
        buf_out += bytes_to_copy;
    } 

    if(retval == 0) {
        return size_to_return;
    }
    else {
        #ifdef DEBUG_MSD
        mylogadd("READ FAILED %i %i %i error %i bufsize %i", lun, lba, offset, retval, bufsize) ;
        #endif
        return MSD_ERROR;
    }
}




/* -------------------------------------------------------------------------- */
/* WRITE                                                                      */
/* -------------------------------------------------------------------------- */
int USBMSD::write(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
    
    
    

    if(lun >= num) {
        #ifdef DEBUG_MSD
        mylogadd("WRITE FAILED 1 %i %i %i", lun, lba, offset) ;
        #endif
        return MSD_ERROR;
    }

    if(!begin(lun)) {
        #ifdef DEBUG_MSD
        mylogadd("WRITE FAILED 2 %i %i %i", lun, lba, offset) ;
        #endif
        return MSD_ERROR;
    }
    
    uint32_t size_to_return = bufsize;
    bd_size_t block_size = _bd[lun]->get_erase_size();
    bd_addr_t requested_address = (bd_addr_t)((lba * block_size) + offset);
    uint32_t block_offset = (requested_address % block_size);
    bd_addr_t block_address = requested_address - block_offset;

    #ifdef DEBUG_MSD
    mylogadd("WRITE %i %i %i %i - %X", lun, lba, offset, bufsize, requested_address) ;
    #endif


    
    uint8_t *buf_out = (uint8_t *)buffer;

    if(!allocate_msd_buffer(block_size)) {
        return MSD_ERROR;
    }
    
    int err = 0;  

    while(bufsize > 0 && err == 0) {
        err = _bd[lun]->read(msd_buffer, block_address, block_size);
        uint32_t bytes_to_copy = bufsize > (block_size - block_offset) ? (block_size - block_offset) : bufsize;
		for(int i = 0; i < bytes_to_copy; i++) {
            *(msd_buffer + block_offset + i) = *(buf_out+i); 
        }
        if(err == 0) {
            _bd[lun]->erase(block_address,block_size);
        }
        if(err == 0) {
            err = _bd[lun]->program(msd_buffer, block_address, block_size);
        }
        block_offset = 0;
        block_address += block_size;
        bufsize -= bytes_to_copy;
        buf_out += bytes_to_copy;
    } 

    if(err == 0) {
        return size_to_return;
    }
    #ifdef DEBUG_MSD
    mylogadd("WRITE FAILED %i %i %i", lun, lba, offset) ;
    #endif
    return err;
}