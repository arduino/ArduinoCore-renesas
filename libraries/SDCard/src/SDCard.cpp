#include "SDCard.h"

#define BLOCK_SIZE_HC 512 /*!< Block size supported for SD card is 512 bytes  */

SDCardBlockMedia::SDCardBlockMedia(const rm_filex_block_media_instance_t *_block_media_instance,
                                   rm_filex_block_media_instance_ctrl_t *_block_media_ctrl,
                                   const rm_filex_block_media_cfg_t *_block_media_cfg,
                                   FX_MEDIA *_media_ptr) :
    _block_media_instance(_block_media_instance),
    _block_media_ctrl(_block_media_ctrl),
    _block_media_cfg(_block_media_cfg),
    _media_ptr(media_ptr)
/*
    _read_size (BLOCK_SIZE_HC), _program_size (BLOCK_SIZE_HC),
    _erase_size(BLOCK_SIZE_HC), _block_size (BLOCK_SIZE_HC),
    _capacity_in_blocks (0)*//*,
    _block_media_instance(g_rm_filex_block_media_0_instance),
    _block_media_ctrl(&g_rm_filex_block_media_0_ctrl),
    _block_media_cfg(g_rm_filex_block_media_0_cfg)*/
{
}

int SDCardBlockMedia::init()
{
    int status = RM_FILEX_BLOCK_MEDIA_Open(&_block_media_ctrl, &_block_media_cfg);

    /* Initialize FileX */
    fx_system_initialize();

    return status;
}

int SDCardBlockMedia::open()
{
    return fx_media_open(_media_ptr,
                         "g_fx_media_ptr",
                         RM_FILEX_BLOCK_MEDIA_BlockDriver,
                         (void *) &_block_media_instance,
                         _media_memory,
                         SD_MEDIA_BLOCK_SIZE);
}

int SDCardBlockMedia::format(char* volumeName, UINT numFat,
                             UINT hiddenSectors, UINT totSectors, UINT bytesPerSector,
                             UINT sectorsPerCluster, UINT heads,
                            UINT sectorsPerTrack, UINT dirEntries)
{
    return fx_media_format(_media_ptr,
                         RM_FILEX_BLOCK_MEDIA_BlockDriver,
                         (void *) &_block_media_instance,
                         _media_memory,
                         SD_MEDIA_BLOCK_SIZE,
                         volumeName,
                         numFat, dirEntries, hiddenSectors,
                         totSectors, bytesPerSector,
                         sectorsPerCluster, heads, sectorsPerTrack);
}

int SDCardBlockMedia::createFile(char* file_name) {
    return fx_file_create(_media_ptr, file_name);
}

int SDCardBlockMedia::deleteFile(char* file_name) {
    return fx_file_delete(_media_ptr, file_name);
}

int SDCardBlockMedia::openFile(char* file_name, FileAccessType access) {
    UINT file_access_type = 0;
    if (access == OPEN_FOR_WRITE) {
        file_access_type = FX_OPEN_FOR_WRITE;
    } else if (access == OPEN_FOR_READ) {
        file_access_type = FX_OPEN_FOR_READ;
    } else if (access == OPEN_FOR_READ_FAST) {
        file_access_type = FX_OPEN_FOR_READ_FAST;
    } else {
        //Invalid type
        return -1;
    }
    return fx_file_open(_media_ptr, file, file_name, file_access_type);
}

int SDCardBlockMedia::closeFile(char* file_name) {
    return fx_file_close(_media_ptr, file_name);
}

int SDCardBlockMedia::writeFile(char* file_name, uint8_t *buf, uint8_t len) {
    int status = 0;
    status |= fx_file_open(_media_ptr, file, file_name, FX_OPEN_FOR_WRITE);
    status |= fx_file_write(file, buf, len);
    status |= fx_file_close(file);
    return status;
}

int SDCardBlockMedia::readFile(char* file_name, uint8_t *buf, uint8_t len, uint32_t* read_size) {
    int status = 0;
    status |= fx_file_open(_media_ptr, file, file_name, FX_OPEN_FOR_READ);
    status |= fx_file_read(file, buf, len, read_size);
    status |= fx_file_close(file);
    return status;
}

int SDCardBlockMedia::close()
{
}

int SDCardBlockMedia::getStatus()
{
}

SDCardBlockMedia SDCard(&g_rm_filex_block_media_0_instance,
                        &g_rm_filex_block_media_0_ctrl,
                        &g_rm_filex_block_media_0_cfg,
                        &g_fx_media0);
