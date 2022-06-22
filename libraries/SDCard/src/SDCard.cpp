#include "SDCard.h"

FX_MEDIA g_fx_media0;

SDCardBlockMedia::SDCardBlockMedia(const rm_filex_block_media_instance_t *block_media_instance,
                                   rm_filex_block_media_instance_ctrl_t *block_media_ctrl,
                                   const rm_filex_block_media_cfg_t *block_media_cfg):
    _block_media_instance(block_media_instance),
    _block_media_ctrl(block_media_ctrl),
    _block_media_cfg(block_media_cfg),
    _first_dir_entry_found(false),
    _sd_error(0)
{
}

int SDCardBlockMedia::mount()
{
    uint8_t err = RM_FILEX_BLOCK_MEDIA_Open(_block_media_ctrl, _block_media_cfg);

    /* Initialize FileX */
    fx_system_initialize();

    return handleError(err);
}

int SDCardBlockMedia::open()
{
    uint8_t err = fx_media_open(&g_fx_media0,
                         "g_fx_media_ptr",
                         RM_FILEX_BLOCK_MEDIA_BlockDriver,
                         (void *) _block_media_instance,
                         _media_memory,
                         sizeof(_media_memory));

    return handleError(err);
}

int SDCardBlockMedia::unmount()
{
    uint8_t err = fx_media_close(&g_fx_media0);

    return handleError(err);
}

int SDCardBlockMedia::format(char* volumeName, UINT numFat,
                             UINT hiddenSectors, UINT totSectors, UINT bytesPerSector,
                             UINT sectorsPerCluster, UINT heads,
                            UINT sectorsPerTrack, UINT dirEntries)
{
    uint8_t err = fx_media_format(&g_fx_media0,
                         RM_FILEX_BLOCK_MEDIA_BlockDriver,
                         (void *)_block_media_instance,
                         _media_memory,
                         SD_MEDIA_BLOCK_SIZE,
                         volumeName,
                         numFat, dirEntries, hiddenSectors,
                         totSectors, bytesPerSector,
                         sectorsPerCluster, heads, sectorsPerTrack);

    return handleError(err);
}

int SDCardBlockMedia::createFile(char* fileName)
{
    uint8_t err = fx_file_create(&g_fx_media0, fileName);

    return handleError(err);
}

int SDCardBlockMedia::deleteFile(char* fileName)
{
    uint8_t err = fx_file_delete(&g_fx_media0, fileName);

    return handleError(err);
}

int SDCardBlockMedia::openFile(FX_FILE *filePtr, char* fileName, FileAccessType access)
{
    UINT fileAccessType = 0;
    if (access == OPEN_FOR_WRITE) {
        fileAccessType = FX_OPEN_FOR_WRITE;
    } else if (access == OPEN_FOR_READ) {
        fileAccessType = FX_OPEN_FOR_READ;
    } else if (access == OPEN_FOR_READ_FAST) {
        fileAccessType = FX_OPEN_FOR_READ_FAST;
    } else {
        //Invalid type
        return 0;
    }

    uint8_t err = fx_file_open(&g_fx_media0, filePtr, fileName, fileAccessType);

    return handleError(err);
}

int SDCardBlockMedia::closeFile(FX_FILE *filePtr)
{
    uint8_t err = fx_file_close(filePtr);

    return handleError(err);
}

int SDCardBlockMedia::writeFile(FX_FILE *filePtr, char* fileName, uint8_t *buf, uint32_t len)
{
    uint8_t err = 0;
    err = fx_file_open(&g_fx_media0, filePtr, fileName, FX_OPEN_FOR_WRITE);
    if (err != FX_SUCCESS) {
        return handleError(err);
    }
    err = fx_file_write(filePtr, buf, len);
    if (err != FX_SUCCESS) {
        return handleError(err);
    }
    err = fx_file_close(filePtr);
    return handleError(err);
}

int SDCardBlockMedia::readFile(FX_FILE *filePtr, char* fileName, uint8_t *buf, uint32_t len, uint32_t* readSize)
{
    uint8_t err = 0;
    err = fx_file_open(&g_fx_media0, filePtr, fileName, FX_OPEN_FOR_READ);
    if (err != FX_SUCCESS) {
        return handleError(err);
    }
    err = fx_file_read(filePtr, buf, len, readSize);
    if (err != FX_SUCCESS) {
        return handleError(err);
    }
    err = fx_file_close(filePtr);
    return handleError(err);
}

int SDCardBlockMedia::createDirectory(char* dirName)
{
    fx_directory_default_set(&g_fx_media0, "firstdir");
    uint8_t err = fx_directory_create(&g_fx_media0, dirName);
    //return handleError(err);
    return err;
}

int SDCardBlockMedia::deleteDirectory(char* dirName)
{
    uint8_t err = fx_directory_delete(&g_fx_media0, dirName);
    return handleError(err);
    return err;
}

int SDCardBlockMedia::getEntryName(char *entryName)
{
    uint8_t err = 0;
    if (_first_dir_entry_found) {
        err = fx_directory_next_entry_find(&g_fx_media0, entryName);
    } else {
        err = fx_directory_first_entry_find(&g_fx_media0, entryName);
        _first_dir_entry_found = true;
    }
    return handleError(err);
}

SDCardStatus SDCardBlockMedia::getStatus()
{
    rm_block_media_status_t sd_status;
    uint8_t err = RM_BLOCK_MEDIA_SDMMC_StatusGet(&g_rm_block_media0_ctrl, &sd_status);
    if (err != FX_SUCCESS) {
        return SD_ERROR;
    }

    if (sd_status.busy) {
        return SD_BUSY;
    } else if (sd_status.initialized) {
        return SD_INITIALIZED;
    } else if (sd_status.media_inserted) {
        return SD_INSERTED;
    }
    return SD_IDLE;
}

SDCardError SDCardBlockMedia::getError()
{
    SDCardError sdError = SD_OK;
    switch (_sd_error) {
        case FX_SUCCESS:
            sdError = SD_OK;
            break;
        case FX_BOOT_ERROR:
            sdError = SD_BOOT_ERROR;
            break;
        case FX_FAT_READ_ERROR:
            sdError = SD_FAT_READ_ERROR;
            break;
        case FX_NOT_A_FILE:
            sdError = SD_FILE_ERROR;
            break;
        case FX_ACCESS_ERROR:
            sdError = SD_ACCESS_ERROR;
            break;
        case FX_FILE_CORRUPT:
            sdError = SD_FILE_CORRUPTED;
            break;
        case FX_END_OF_FILE:
            sdError = SD_END_OF_FILE;
            break;
        case FX_NO_MORE_SPACE:
            sdError = SD_FULL;
            break;
        case FX_ALREADY_CREATED:
            sdError = SD_FILE_ALREADY_CREATED;
            break;
        case FX_INVALID_NAME:
            sdError = SD_INVALID_FILE_NAME;
            break;
        case FX_INVALID_PATH:
            sdError = SD_INVALID_PATH;
            break;
        case FX_BUFFER_ERROR:
            sdError = SD_BUFFER_ERROR;
            break;
        case FX_WRITE_PROTECT:
            sdError = SD_WRITE_PROTECT;
            break;
        case FX_SECTOR_INVALID:
            sdError = SD_INVALID_SECTOR;
            break;
        case FX_IO_ERROR:
            sdError = SD_IO_ERROR;
            break;
        case FX_INVALID_STATE:
            sdError = SD_INVALID_STATE;
            break;
        default:
            sdError = SD_GENERIC_ERROR;
            break;
    }
    return sdError;
}

int SDCardBlockMedia::handleError(uint8_t err)
{
    if (err != FX_SUCCESS) {
        _sd_error = err;
        return 0;
    }
    _sd_error = FX_SUCCESS;
    return 1;
}

SDCardBlockMedia SDCard(&g_rm_filex_block_media_0_instance,
                        &g_rm_filex_block_media_0_ctrl,
                        &g_rm_filex_block_media_0_cfg);

void g_rm_filex_block_media_0_callback(rm_filex_block_media_callback_args_t *p_args) {
  
}
