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
#ifdef DO_NOT_USE
void __USBInstallMSD() {}
#endif

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

    bd_size_t block_size = _bd[lun]->get_erase_size();
    bd_addr_t block_address = (bd_addr_t)((lba * block_size) + offset);

    Serial1.print("R BLOCK ADDRESS ");
    Serial1.print(block_address) ;
    Serial1.print(" "); 
    Serial1.println(block_address, HEX) ; 
    int retval = 1;
    if(lun < num) {
        retval = _bd[lun]->read(buffer, block_address, (bd_size_t)bufsize);
    }
    if(retval == 0) {
        return bufsize;
    }
    else {
        #ifdef DEBUG_MSD
        mylogadd("READ FAILED %i %i %i error %i bufsize %i", lun, lba, offset, retval, bufsize) ;
        #endif
        return MSD_ERROR;
    }
}


uint8_t write_buff[512];

void print_u8(uint8_t t) {
    if(t < 10) {
        Serial1.print('0');
    }
    Serial1.print(t,HEX);
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


    bd_size_t block_size = _bd[lun]->get_erase_size();
    bd_addr_t block_address = (bd_addr_t)((lba * block_size) + offset);

    if(offset != 0) {
        while(1) {
            Serial.println("OFFSET != 0");
            delay(1000);
        }
    }

    if(bufsize != block_size) {
        while(1) {
            Serial.println("BUFFSIZE != _bd[lun]->get_erase_size()");
            delay(1000);
        }
    }

    if(block_size != 512) {
        while(1) {
            Serial.println("BUFFSIZE != 512");
            delay(1000);
        }
    }

    for(int i = 0;i < 512; i++) {
        write_buff[i] = buffer[i];
    }

    Serial1.print("W BLOCK ADDRESS ");
    Serial1.print(block_address) ;
    Serial1.print(" "); 
    Serial1.print(block_address, HEX) ; 
    Serial1.print(" ");
    Serial1.println((uint32_t)write_buff);  
      
    int err = _bd[lun]->erase(block_address,block_size);
    if(err == 0) {
        err = _bd[lun]->program(write_buff, block_address, block_size);
    }
   
    int index = 0;
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 32; j++) {
            print_u8(write_buff[index]);
            Serial1.print(" ");
            index++;
        }
        Serial1.println();
        
         
    }



    if(err == 0) {
        return bufsize;
    }
    #ifdef DEBUG_MSD
    mylogadd("WRITE FAILED %i %i %i", lun, lba, offset) ;
    #endif
    return err;
}