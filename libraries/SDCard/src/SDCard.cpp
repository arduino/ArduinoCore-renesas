#include "SDCard.h"



SDCardBlockMedia::SDCardBlockMedia():
   sd(PIN_SDHI_CLK, PIN_SDHI_CMD, PIN_SDHI_D0, PIN_SDHI_D1, PIN_SDHI_D2, PIN_SDHI_D3, PIN_SDHI_CD, PIN_SDHI_WP), 
   fs("SDCard", NULL),
   _first_dir_entry_found(false)
{

}

int SDCardBlockMedia::mount()
{
#ifndef USE_4BIT_SDCARD
    return 0;
#endif
    uint8_t err = 0;
    if(sd.is_card_present()) {
        err = fs.mount(&sd);
    }
    return handleError(err);
}

int SDCardBlockMedia::open()
{
    uint8_t err = 0;

    return handleError(err);
}

int SDCardBlockMedia::unmount()
{
    uint8_t err = 0;

    return handleError(err);
}

int SDCardBlockMedia::format(char* volumeName, UINT numFat,
                             UINT hiddenSectors, UINT totSectors, UINT bytesPerSector,
                             UINT sectorsPerCluster, UINT heads,
                            UINT sectorsPerTrack, UINT dirEntries)
{
    uint8_t err = 0;
    return handleError(err);
}

int SDCardBlockMedia::createFile(char* fileName)
{
    uint8_t err = 0;

    return handleError(err);
}

int SDCardBlockMedia::deleteFile(char* fileName)
{
    uint8_t err = 0;

    return handleError(err);
}

int SDCardBlockMedia::openFile(fs_file_t *file, char* fileName, FileAccessType access)
{
    uint8_t err = 0;

  

    return handleError(err);
}

int SDCardBlockMedia::closeFile(fs_file_t *file)
{
    uint8_t err = 0;

    return handleError(err);
}

int SDCardBlockMedia::writeFile(fs_file_t *file, char* fileName, uint8_t *buf, uint32_t len)
{
    uint8_t err = 0;
    
    return handleError(err);
}

int SDCardBlockMedia::readFile(fs_file_t *file, char* fileName, uint8_t *buf, uint32_t len, uint32_t* readSize)
{
    uint8_t err = 0;
    
    return handleError(err);
}

int SDCardBlockMedia::createDirectory(char* dirName)
{
    uint8_t err = 0;
    return err;
}

int SDCardBlockMedia::deleteDirectory(char* dirName)
{
    uint8_t err = 0;
    return handleError(err);
    
}

int SDCardBlockMedia::getEntryName(char *entryName)
{
    uint8_t err = 0;
    
    return handleError(err);
}

SDCardStatus SDCardBlockMedia::getStatus()
{
    
}

SDCardError SDCardBlockMedia::getError()
{
    
}

int SDCardBlockMedia::handleError(uint8_t err)
{
    if (err != 0) {
        
        return 0;
    }
    

    return 1;
}

SDCardBlockMedia SDCard;


