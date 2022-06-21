#include "SDCard.h"

FX_MEDIA g_fx_media0;

SDCardBlockMedia::SDCardBlockMedia(const rm_filex_block_media_instance_t *block_media_instance,
                                   rm_filex_block_media_instance_ctrl_t *block_media_ctrl,
                                   const rm_filex_block_media_cfg_t *block_media_cfg):
    _block_media_instance(block_media_instance),
    _block_media_ctrl(block_media_ctrl),
    _block_media_cfg(block_media_cfg)
{
}

int SDCardBlockMedia::mount()
{
    int status = RM_FILEX_BLOCK_MEDIA_Open(_block_media_ctrl, _block_media_cfg);

    /* Initialize FileX */
    fx_system_initialize();

    return status;
}

int SDCardBlockMedia::open()
{
    return fx_media_open(&g_fx_media0,
                         "g_fx_media_ptr",
                         RM_FILEX_BLOCK_MEDIA_BlockDriver,
                         (void *) _block_media_instance,
                         _media_memory,
                         sizeof(_media_memory));
}

int SDCardBlockMedia::unmount()
{
    return fx_media_close(&g_fx_media0);
}

int SDCardBlockMedia::format(char* volumeName, UINT numFat,
                             UINT hiddenSectors, UINT totSectors, UINT bytesPerSector,
                             UINT sectorsPerCluster, UINT heads,
                            UINT sectorsPerTrack, UINT dirEntries)
{
    return fx_media_format(&g_fx_media0,
                         RM_FILEX_BLOCK_MEDIA_BlockDriver,
                         (void *)_block_media_instance,
                         _media_memory,
                         SD_MEDIA_BLOCK_SIZE,
                         volumeName,
                         numFat, dirEntries, hiddenSectors,
                         totSectors, bytesPerSector,
                         sectorsPerCluster, heads, sectorsPerTrack);
}

int SDCardBlockMedia::createFile(char* file_name) {
    return fx_file_create(&g_fx_media0, file_name);
}

int SDCardBlockMedia::deleteFile(char* file_name) {
    return fx_file_delete(&g_fx_media0, file_name);
}

int SDCardBlockMedia::openFile(FX_FILE *file_ptr, char* file_name, FileAccessType access) {
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
    return fx_file_open(&g_fx_media0, file_ptr, file_name, file_access_type);
}

int SDCardBlockMedia::closeFile(FX_FILE *file_ptr) {
    return fx_file_close(file_ptr);
}

int SDCardBlockMedia::writeFile(FX_FILE *file_ptr, char* file_name, uint8_t *buf, ULONG len) {
    int status = 0;
    status |= fx_file_open(&g_fx_media0, file_ptr, file_name, FX_OPEN_FOR_WRITE);
    status |= fx_file_write(file_ptr, buf, len);
    status |= fx_file_close(file_ptr);
    return status;
}

int SDCardBlockMedia::readFile(FX_FILE *file_ptr, char* file_name, uint8_t *buf, ULONG len, ULONG* read_size) {
    int status = 0;
    status |= fx_file_open(&g_fx_media0, file_ptr, file_name, FX_OPEN_FOR_READ);
    status |= fx_file_read(file_ptr, buf, len, read_size);
    status |= fx_file_close(file_ptr);
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
                        &g_rm_filex_block_media_0_cfg);

void g_rm_filex_block_media_0_callback(rm_filex_block_media_callback_args_t *p_args) {
  
}
