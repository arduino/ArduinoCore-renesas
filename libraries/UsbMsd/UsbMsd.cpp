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


USBMSD::USBMSD(std::initializer_list<BlockDevice *> args) {
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
USBMSD::USBMSD(BlockDevice *bd) {
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



/* -------------------------------------------------------------------------- */
/* READ                                                                       */
/* -------------------------------------------------------------------------- */
int USBMSD::read(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
   
    if(!begin(lun)) {
        #ifdef DEBUG_MSD
        mylogadd("READ FAILED 1 %i %i %i", lun, lba, offset) ;
        #endif
        return MSD_ERROR;
    }

    bd_addr_t add = (bd_addr_t)((lba * _bd[lun]->get_erase_size()) + offset);
    int retval = 1;
    if(lun < num) {
        retval = _bd[lun]->read(buffer, add, (bd_size_t)bufsize);
    }
    if(retval == 0) {
        return bufsize;
    }
    else {
        #ifdef DEBUG_MSD
        mylogadd("READ FAILED %i %i %i", lun, lba, offset) ;
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

    bd_addr_t add = (bd_addr_t)((lba * _bd[lun]->get_erase_size()) + offset);

    bd_size_t block_size = _bd[lun]->get_erase_size();
    uint8_t *bk = new uint8_t[block_size];
    uint8_t *buff = buffer;

    int err = MSD_ERROR;

    if(bk != nullptr) {
        int64_t internal_size = (int64_t)bufsize;
        bd_addr_t block_address = block_size * (add / block_size);
        
        uint32_t byte_left_in_block =  block_size - (add % block_size);
        
        err = 0;
        while(internal_size > 0 && err == 0) {
            /* 
             * READ the block as erase dimension (4096 bytes) 
             */
            bool erase_needed = false;
            err = _bd[lun]->read(bk, block_address, block_size);
            
            /*
             * Write in RAM
             */
            if(err == 0) {
                uint32_t bytes_to_copy = (uint32_t)((internal_size > byte_left_in_block) ? byte_left_in_block : internal_size);
                uint8_t *ptr = bk + block_size - byte_left_in_block;
                for(int i = 0; i < bytes_to_copy; i++) {
                    if(*(ptr+i) != _bd[lun]->get_erase_value()) {
                        erase_needed = true;
                    }
                    *(ptr + i) = *(buffer + i);
                }
                if(erase_needed) {
                    err = _bd[lun]->erase(block_address,block_size);
                }
            }
            if(err == 0) {   
                err = _bd[lun]->program(bk, block_address, block_size);
            }
            block_address += block_size;
            buff += byte_left_in_block;
            internal_size -= byte_left_in_block;
            byte_left_in_block = block_size;
        }
        delete []bk;
    }

    if(err == 0) {
        return bufsize;
    }
    #ifdef DEBUG_MSD
    mylogadd("WRITE FAILED %i %i %i", lun, lba, offset) ;
    #endif
    return err;
}