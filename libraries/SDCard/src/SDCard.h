#ifndef _SDCARD_BLOCK_DEVICE_H_
#define _SDCARD_BLOCK_DEVICE_H_

#include <Arduino.h>

#include "FATFileSystem.h"
#include "SDCardBlockDevice.h"

#ifdef __cplusplus

#define SD_MEDIA_BLOCK_SIZE 512 /*!< Block size supported for SD card is 512 bytes  */
#define FILE_NAME_MAX 255

typedef enum {
  OPEN_FOR_WRITE     = 0,
  OPEN_FOR_READ      = 1,
  OPEN_FOR_READ_FAST = 2,
} FileAccessType;

typedef enum {
  SD_IDLE            = 0,
  SD_INITIALIZED     = 1,
  SD_BUSY            = 2,
  SD_INSERTED        = 3,
  SD_ERROR           = 4,
} SDCardStatus;

typedef enum {
  SD_OK                   = 0,
  SD_BOOT_ERROR           = 1,
  SD_FAT_READ_ERROR       = 2,
  SD_FILE_ERROR           = 3,
  SD_ACCESS_ERROR         = 4,
  SD_FILE_CORRUPTED       = 5,
  SD_END_OF_FILE          = 6,
  SD_FULL                 = 7,
  SD_FILE_ALREADY_CREATED = 8,
  SD_INVALID_FILE_NAME    = 9,
  SD_INVALID_PATH         = 10,
  SD_BUFFER_ERROR         = 11,
  SD_WRITE_PROTECT        = 12,
  SD_INVALID_SECTOR       = 13,
  SD_IO_ERROR             = 14,
  SD_INVALID_STATE        = 15,
  SD_GENERIC_ERROR        = 16,
} SDCardError;

class SDCardBlockMedia {
public:
    SDCardBlockMedia();

    /** Initialize and mount the SD card block media
     *
     *  @return                  1 on success or 0 on failure
     */
    int mount();

    /** Deinitialize and unmount the SD card block media
     *
     *  @return                  1 on success or 0 on failure
     */
    int unmount();

    /** Open the SD card block media
     *
     *  @return                  1 on success or 0 on failure
     */
    int open();

    /** Format the SD card block media
     *
     *  @param volumeName        Volume name
     *  @param numFat            Number of FAT tables
     *  @param hiddenSectors     Number of hidden sectors
     *  @param totSectors        Total number of sectors
     *  @param bytesPerSector    Number of bytes per sector
     *  @param sectorsPerCluster Number of sectors per cluster
     *  @param heads             Number of heads
     *  @param sectorsPerTrack   Number of sectors per track
     *  @param dirEntries        Number of directory entries
     *  @return                  1 on success or 0 on failure
     */
    int format(char* volumeName, UINT numFat,
               UINT hiddenSectors, UINT totSectors, UINT bytesPerSector,
               UINT sectorsPerCluster = 1, UINT heads = 1, UINT sectorsPerTrack = 1, UINT dirEntries = 128);

    /** Create a file in the SD card block media
     *
     *  @param fileName          Name of the file
     *  @return                  1 on success or 0 on failure
     */
    int createFile(char* fileName);

    /** Delete a file from the SD card block media
     *
     *  @param fileName          Name of the file
     *  @return                  1 on success or 0 on failure
     */
    int deleteFile(char* fileName);

    /** Open a file in the SD card block media
     *
     *  @param filePtr           File pointer
     *  @param fileName          Name of the file
     *  @param access            File access type
     *  @return                  1 on success or 0 on failure
     */
    int openFile(fs_file_t *file, char* fileName, FileAccessType access);

    int closeFile(fs_file_t *file);

    /** Open the file, write content into it and close it
     *
     *  @param filePtr           File pointer
     *  @param fileName          Name of the file
     *  @param buf               Buffer of bytes to be written
     *  @param len               NUmber of bytes to write
     *  @return                  1 on success or 0 on failure
     */
    int writeFile(fs_file_t *file, char* fileName, uint8_t *buf, uint32_t len);

    /** Open the file, read its content and close it
     *
     *  @param filePtr           File pointer
     *  @param fileName          Name of the file
     *  @param buf               Buffer of bytes read
     *  @param len               Number of bytes to read
     *  @param readSize          Actual number of bytes read
     *  @return                  1 on success or 0 on failure
     */
    int readFile(fs_file_t *file, char* fileName, uint8_t *buf, uint32_t len, uint32_t* readSize);

    /** Create a directory in the SD card block media
     *
     *  @param dirName           Name of the directory
     *  @return                  1 on success or 0 on failure
     */
    int createDirectory(char* dirName);

    /** Delete a directory from the SD card block media
     *
     *  @param dirName           Name of the directory
     *  @return                  1 on success or 0 on failure
     */
    int deleteDirectory(char* dirName);

    /** Get the name of an entry from the current working directory of the SD card block media
     *  Entries are pointed in a sequential order.
     *
     *  @param entryName         Name of the detected entry
     *  @return                  1 on success or 0 on failure
     */
    int getEntryName(char *entryName);

    /** Get the SD card status
     *
     *  @return                  SDCardStatus value
     */
    SDCardStatus getStatus();

    /** Get the latest SD card error
     *
     *  @return                  SDCardError value
     */
    SDCardError getError();

private:
    FATFileSystem fs;
    SDCardBlockDevice sd;
    bool _first_dir_entry_found;

    int handleError(uint8_t err);

};

extern SDCardBlockMedia SDCard;

#endif
#endif /* _SDCARD_BLOCK_DEVICE_H_ */
