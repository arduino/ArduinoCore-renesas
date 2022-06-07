#ifndef _SDCARD_BLOCK_DEVICE_H_
#define _SDCARD_BLOCK_DEVICE_H_

#include <Arduino.h>

#include "bsp_api.h"
#include "hal_data.h"

#ifdef __cplusplus

#define SD_MEDIA_BLOCK_SIZE 512 /*!< Block size supported for SD card is 512 bytes  */

typedef enum {
  OPEN_FOR_WRITE     = 0,
  OPEN_FOR_READ      = 1,
  OPEN_FOR_READ_FAST = 2,
} FileAccessType;

class SDCardBlockMedia {
public:
    SDCardBlockMedia(const rm_filex_block_media_instance_t *_block_media_instance,
                     rm_filex_block_media_instance_ctrl_t *_block_media_ctrl,
                     const rm_filex_block_media_cfg_t *_block_media_cfg,
                     FX_MEDIA *_media_ptr);

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    int init();

    int open();

    int format(char* volumeName, UINT numFat,
               UINT hiddenSectors, UINT totSectors, UINT bytesPerSector,
               UINT sectorsPerCluster = 1, UINT heads = 1, UINT sectorsPerTrack = 1, UINT dirEntries = 128);

    int createFile(char* file_name);

    int deleteFile(char* file_name);

    int openFile(char* file_name, FileAccessType access);

    int closeFile(char* file_name);

    /** Write content into a file
     *
     *  @param file_name  Name of the file
     *  @param buff       Buffer of bytes to be written
     *  @param len        Size of bytes to write
     *  @return           0 on success, error code on failure
     */
    int writeFile(char* file_name, uint8_t *buf, uint8_t len);

    int readFile(char* file_name, uint8_t *buf, uint8_t len, uint32_t* read_size);

    /** Deinitialize a block device
     *
     *  @return         0 on success or an error code on failure
     */
    int close();

    int getStatus();

private:
    sdmmc_card_type_t _card_type;
    bool _write_protected;    ///< true = Card is write protected
    /*uint32_t _read_size;
    uint32_t _program_size;
    uint32_t _erase_size;
    uint32_t _block_size;
    uint32_t _capacity_in_blocks;*/
    //BSP_SD_CardInfo _current_card_info;
    uint8_t _sd_state;

    const sdmmc_cfg_t _sdmmc_cfg;
    const sdmmc_ctrl_t _sdmmc_ctrl;

    const rm_filex_block_media_instance_t *_block_media_instance;
    rm_filex_block_media_instance_ctrl_t *_block_media_ctrl;
    const rm_filex_block_media_cfg_t *_block_media_cfg;
    FX_MEDIA *_media_ptr;
    uint8_t _media_memory[SD_MEDIA_BLOCK_SIZE] BSP_ALIGN_VARIABLE(4);

    FX_FILE *file;
};

extern SDCardBlockMedia SDCard;

#endif
#endif /* _SDCARD_BLOCK_DEVICE_H_ */
