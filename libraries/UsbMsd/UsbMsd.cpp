#include "UsbMsd.h"


#define BLOCK_SIZE 512


/* -------------------------------------------------------------------------- */
/* Add Mass Storage USB capability when present                               */
/* -------------------------------------------------------------------------- */
void __USBInstallMSD() {}




/* -------------------------------------------------------------------------- */
/* extern "C" functions --> defined in tu_msd.c                               */
/* -------------------------------------------------------------------------- */
void usb_msd_set_dev_ptr(USBMSD *prt);

/* -------------------------------------------------------------------------- */
/* CONSTRUCTOR -> initalize block device and pass a "hook" to the tu_msd module */
/* -------------------------------------------------------------------------- */
USBMSD::USBMSD(BlockDevice *bd) {
    _bd = bd;
    
    usb_msd_set_dev_ptr(this);
}

bool USBMSD::begin() {
    _bd->init();
}

/* -------------------------------------------------------------------------- */
/* DISTRUCTOR                                                                 */
/* -------------------------------------------------------------------------- */
USBMSD::~USBMSD() {

}

/* -------------------------------------------------------------------------- */
/* AVAILABLE                                                                  */
/* -------------------------------------------------------------------------- */
bool USBMSD::available() {
    begin();
    return _bd->available();
}

/* -------------------------------------------------------------------------- */
/* GET BLOCK COUNT                                                            */
/* -------------------------------------------------------------------------- */
uint32_t USBMSD::get_block_count() {
    begin();
    return (_bd->size() / BLOCK_SIZE);
}

/* -------------------------------------------------------------------------- */
/* GET BLOCK SIZE                                                             */
/* -------------------------------------------------------------------------- */
uint16_t USBMSD::get_block_size() {
    begin();
    return BLOCK_SIZE;
}

extern "C" int mylogadd(const char *fmt, ...) ;

/* -------------------------------------------------------------------------- */
/* READ                                                                       */
/* -------------------------------------------------------------------------- */
int USBMSD::read(uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {
    //mylogadd("READ %i %i %i", lba, offset, bufsize) ;
    begin();
    bd_addr_t add = (bd_addr_t)((lba * BLOCK_SIZE) + offset);
    int retval = _bd->read(buffer, add, (bd_size_t)bufsize);
    if(retval == 0) {
        return bufsize;
    }
    else {
        return MSD_ERROR;
    }
}
#define PRINT_SIZE 32

/* -------------------------------------------------------------------------- */
extern void print_uint8(uint8_t n);

/* -------------------------------------------------------------------------- */
extern void print_buffer(uint8_t *buff, uint32_t _size);


/* -------------------------------------------------------------------------- */
/* WRITE                                                                      */
/* -------------------------------------------------------------------------- */
int USBMSD::write(uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {
    //mylogadd("WRITE %i %i %i", lba, offset, bufsize) ;
    begin();
    bd_addr_t add = (bd_addr_t)((lba * BLOCK_SIZE) + offset);

    //mylogadd("   add %i" , add);

    bd_size_t block_size = _bd->get_erase_size();
    uint8_t *bk = new uint8_t[block_size];
    uint8_t *buff = buffer;

    int err = MSD_ERROR;

    if(bk != nullptr) {
        int64_t internal_size = (int64_t)bufsize;
        bd_addr_t block_address = block_size * (add / block_size);
        //mylogadd("   block_address %i" , block_address);
        uint32_t byte_left_in_block =  block_size - (add % block_size);
        //Serial.print("block_size ");
        //Serial.println(block_size);

        //Serial.print("byte_left_in_block ");
        //Serial.println(byte_left_in_block);
        err = 0;
        while(internal_size > 0 && err == 0) {
            /* 
             * READ the block as erase dimension (4096 bytes) 
             */
            bool erase_needed = false;
            err = _bd->read(bk, block_address, block_size);
            //Serial.println("-----prima----");
            //print_buffer(bk, block_size);
            /*
             * Write in RAM
             */
            if(err == 0) {
                uint32_t bytes_to_copy = (uint32_t)((internal_size > byte_left_in_block) ? byte_left_in_block : internal_size);
                uint8_t *ptr = bk + block_size - byte_left_in_block;
                for(int i = 0; i < bytes_to_copy; i++) {
                    if(*(ptr+i) != _bd->get_erase_value()) {
                        erase_needed = true;
                    }
                    *(ptr + i) = *(buffer + i);
                }
                if(erase_needed) {
                    err = _bd->erase(block_address,block_size);
                }
            }
            if(err == 0) {   
                //Serial.println("-----dopo----");
                //print_buffer(bk, block_size);
                err = _bd->program(bk, block_address, block_size);
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
    return err;
}