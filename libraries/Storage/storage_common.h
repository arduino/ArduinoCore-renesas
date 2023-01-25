#ifndef ARDUINO_STORAGE_COMMON_H
#define ARDUINO_STORAGE_COMMON_H

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STORAGE_DEBUG
#define STORAGE_ASSERT




/* -------------------------------------------------------------------------- */
/*                                 STORAGE DEBUG                              */
/* -------------------------------------------------------------------------- */
#ifdef STORAGE_DEBUG

#define STORAGE_BUFF_DIM 512

extern char debug_buffer[STORAGE_BUFF_DIM];

/** Output a debug message
 *
 * @param format printf-style format string, followed by variables
 */
static inline void debug(const char *fmt, ...)
{
    
    memset(debug_buffer,0x00,256);
    va_list va;
    va_start (va, fmt);
    vsnprintf (debug_buffer,STORAGE_BUFF_DIM, fmt, va);
    va_end (va);
    if(Serial)
        Serial.println(debug_buffer);
}


/** Conditionally output a debug message
 *
 * NOTE: If the condition is constant false (== 0) and the compiler optimization
 * level is greater than 0, then the whole function will be compiled away.
 *
 * @param condition output only if condition is true (!= 0)
 * @param format printf-style format string, followed by variables
 */
static inline void debug_if(int condition, const char *fmt, ...)
{
#if DEVICE_STDIO_MESSAGES && !defined(NDEBUG)
    if (condition) {
        memset(debug_buffer,0x00,256);
        va_list va;
        va_start (va, fmt);
        vsnprintf (debug_buffer,STORAGE_BUFF_DIM, fmt, va);
        va_end (va);
        if(Serial)
            Serial.println(debug_buffer);
    }
#endif
}

#else 

static inline void debug_if(int condition, const char *format, ...) {

}

static inline void debug(const char *format, ...) {
    
}

#endif //ifdef STORAGE_DEBUG

/* -------------------------------------------------------------------------- */
/*                                 STORAGE ASSERTS                            */
/* -------------------------------------------------------------------------- */

#ifdef STORAGE_ASSERT
    #define MBED_ASSERT(expr)  do { if (!(expr)) { \
        debug("ASSERT FAILED at line %d in file %s",__LINE__,__FILE__); }} while(0)
#else
    #define MBED_ASSERT(expr)
#endif 

/* -------------------------------------------------------------------------- */
/*                               DUMMY MUTEX                                  */
/* -------------------------------------------------------------------------- */



#ifdef __cplusplus
}
#endif

template<typename T>
class NonCopyable {
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
public:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};


class PlatformMutex: private NonCopyable<PlatformMutex> {
public:
    
    PlatformMutex()
    {
    }

    
    ~PlatformMutex()
    {
    }

   
    void lock()
    {
    }

    
    void unlock()
    {
    }
};

#define POLLIN         0x0001 ///< Data may be read without blocking
#define POLLOUT        0x0010 ///< Data may be written without blocking
#define POLLERR        0x1000 ///< An error has occurred on the device or stream
#define POLLHUP        0x2000 ///< The device has been disconnected
#define POLLNVAL       0x4000 ///< The specified file handle value is invalid

#define NAME_MAX 255    ///< Maximum size of a name in a file path

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2


struct dirent {
    char d_name[NAME_MAX + 1]; ///< Name of file
    uint8_t d_type;          ///< Type of file
};

enum {
    DT_UNKNOWN, ///< The file type could not be determined.
    DT_FIFO,    ///< This is a named pipe (FIFO).
    DT_CHR,     ///< This is a character device.
    DT_DIR,     ///< This is a directory.
    DT_BLK,     ///< This is a block device.
    DT_REG,     ///< This is a regular file.
    DT_LNK,     ///< This is a symbolic link.
    DT_SOCK,    ///< This is a UNIX domain socket.
};

#define     _IFMT   0170000 //< type of file
#define     _IFSOCK 0140000 //< socket
#define     _IFLNK  0120000 //< symbolic link
#define     _IFREG  0100000 //< regular
#define     _IFBLK  0060000 //< block special
#define     _IFDIR  0040000 //< directory
#define     _IFCHR  0020000 //< character special
#define     _IFIFO  0010000 //< fifo special

#define S_IFMT      _IFMT   //< type of file
#define S_IFSOCK    _IFSOCK //< socket
#define S_IFLNK     _IFLNK  //< symbolic link
#define S_IFREG     _IFREG  //< regular
#define S_IFBLK     _IFBLK  //< block special
#define S_IFDIR     _IFDIR  //< directory
#define S_IFCHR     _IFCHR  //< character special
#define S_IFIFO     _IFIFO  //< fifo special

#define S_IRWXU     (S_IRUSR | S_IWUSR | S_IXUSR)
#define     S_IRUSR 0000400 ///< read permission, owner
#define     S_IWUSR 0000200 ///< write permission, owner
#define     S_IXUSR 0000100 ///< execute/search permission, owner
#define S_IRWXG     (S_IRGRP | S_IWGRP | S_IXGRP)
#define     S_IRGRP 0000040 ///< read permission, group
#define     S_IWGRP 0000020 ///< write permission, group
#define     S_IXGRP 0000010 ///< execute/search permission, group
#define S_IRWXO     (S_IROTH | S_IWOTH | S_IXOTH)
#define     S_IROTH 0000004 ///< read permission, other
#define     S_IWOTH 0000002 ///< write permission, other
#define     S_IXOTH 0000001 ///< execute/search permission, other

struct stat {
    dev_t     st_dev;     ///< Device ID containing file
    ino_t     st_ino;     ///< File serial number
    mode_t    st_mode;    ///< Mode of file
    nlink_t   st_nlink;   ///< Number of links to file

    uid_t     st_uid;     ///< User ID
    gid_t     st_gid;     ///< Group ID

    off_t     st_size;    ///< Size of file in bytes

    time_t    st_atime;   ///< Time of last access
    time_t    st_mtime;   ///< Time of last data modification
    time_t    st_ctime;   ///< Time of last status change
};

struct statvfs {
    unsigned long  f_bsize;    ///< Filesystem block size
    unsigned long  f_frsize;   ///< Fragment size (block size)

    fsblkcnt_t     f_blocks;   ///< Number of blocks
    fsblkcnt_t     f_bfree;    ///< Number of free blocks
    fsblkcnt_t     f_bavail;   ///< Number of free blocks for unprivileged users

    unsigned long  f_fsid;     ///< Filesystem ID

    unsigned long  f_namemax;  ///< Maximum filename length
};

/* Flags for open() and fcntl(GETFL/SETFL)
 * At present, fcntl only supports reading and writing O_NONBLOCK
 */
#ifndef O_RDONLY
#define O_RDONLY 0        ///< Open for reading
#endif
#ifndef O_WRONLY
#define O_WRONLY 1        ///< Open for writing
#endif
#ifndef O_RDWR
#define O_RDWR   2        ///< Open for reading and writing
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK 0x0004 ///< Non-blocking mode
#endif
#ifndef O_APPEND
#define O_APPEND   0x0008 ///< Set file offset to end of file prior to each write
#endif
#ifndef O_CREAT
#define O_CREAT    0x0200 ///< Create file if it does not exist
#endif
#ifndef O_TRUNC
#define O_TRUNC    0x0400 ///< Truncate file to zero length
#endif
#ifndef O_EXCL
#define O_EXCL     0x0800 ///< Fail if file exists
#endif
#ifndef O_BINARY
#define O_BINARY   0x8000 ///< Open file in binary mode
#endif
#ifndef O_ACCMODE
#define O_ACCMODE   (O_RDONLY|O_WRONLY|O_RDWR)
#endif

uint32_t core_util_atomic_incr_u32(volatile uint32_t *valuePtr, uint32_t delta);
uint32_t core_util_atomic_decr_u32(volatile uint32_t *valuePtr, uint32_t delta);


#ifndef ARDUINO_PACKED
#if defined(__ICCARM__)
#define ARDUINO_PACKED(struct) __packed struct
#else
#define ARDUINO_PACKED(struct) struct __attribute__((packed))
#endif
#endif

#endif



