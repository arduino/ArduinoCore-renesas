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
#define PRINT_SIZE        32

extern char rns_storage_dbg_buf[STORAGE_BUFF_DIM];

/** Output a debug message
 *
 * @param format printf-style format string, followed by variables
 */
static inline void rns_storage_dbg(const char *fmt, ...)
{
    memset(rns_storage_dbg_buf,0x00,256);
    va_list va;
    va_start (va, fmt);
    vsnprintf (rns_storage_dbg_buf,STORAGE_BUFF_DIM, fmt, va);
    va_end (va);
    if(Serial)
        Serial.println(rns_storage_dbg_buf);
}


/** Conditionally output a debug message
 *
 * NOTE: If the condition is constant false (== 0) and the compiler optimization
 * level is greater than 0, then the whole function will be compiled away.
 *
 * @param condition output only if condition is true (!= 0)
 * @param format printf-style format string, followed by variables
 */
static inline void rns_storage_dbg_if(int condition, const char *fmt, ...)
{
    if (condition) {
        memset(rns_storage_dbg_buf,0x00,256);
        va_list va;
        va_start (va, fmt);
        vsnprintf (rns_storage_dbg_buf,STORAGE_BUFF_DIM, fmt, va);
        va_end (va);
        if(Serial)
            Serial.println(rns_storage_dbg_buf);
    }
}

static inline void rns_storage_dbg_mem(uint8_t *b, uint32_t _size)
{
    if (b != nullptr) {
        Serial.println("");
        for(uint32_t i = 0; i < _size; i++) {
            if(i != 0 && i % PRINT_SIZE == 0) {
                if(i != 0)
                    Serial.println();
            }
            Serial.print(*(b + i) >> 4,  HEX);
            Serial.print(*(b + i) & 0x0F,HEX);
        }
        Serial.println();
        Serial.println("");
    }
}

#else

static inline void rns_storage_dbg_if(int condition, const char *format, ...) {
    (void)condition;
    (void)format;
}

static inline void rns_storage_dbg(const char *format, ...) {
    (void)format;
}

static inline void rns_storage_dbg_mem(uint8_t *b, uint32_t _size) {
    (void)b;
    (void)_size;
}

#endif // STORAGE_DEBUG

/* -------------------------------------------------------------------------- */
/*                                 STORAGE ASSERTS                            */
/* -------------------------------------------------------------------------- */

#ifdef STORAGE_ASSERT
    #define MBED_ASSERT(expr)  do { if (!(expr)) { \
        rns_storage_dbg("ASSERT FAILED at line %d in file %s",__LINE__,__FILE__); }} while(0)
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

uint32_t core_util_atomic_incr_u32(volatile uint32_t *valuePtr, uint32_t delta);
uint32_t core_util_atomic_decr_u32(volatile uint32_t *valuePtr, uint32_t delta);
uint8_t core_util_atomic_load_u8(const volatile uint8_t *valuePtr);
void core_util_atomic_store_u8(volatile uint8_t *valuePtr, uint8_t desiredValue);

#ifndef ARDUINO_PACKED
#if defined(__ICCARM__)
#define ARDUINO_PACKED(struct) __packed struct
#else
#define ARDUINO_PACKED(struct) struct __attribute__((packed))
#endif
#endif

class FileHandle;

/** \addtogroup platform-public-api */
/** @{*/

/**
 * \defgroup platform_poll poll functions
 * @{
 */

struct pollfh {
    FileHandle *fh;
    short events;
    short revents;
};

/** A mechanism to multiplex input/output over a set of file handles(file descriptors).
 * For every file handle provided, poll() examines it for any events registered for that particular
 * file handle.
 *
 * @param fhs     an array of PollFh struct carrying a FileHandle and bitmasks of events
 * @param nfhs    number of file handles
 * @param timeout timer value to timeout or -1 for loop forever
 *
 * @return number of file handles selected (for which revents is non-zero). 0 if timed out with nothing selected. -1 for error.
 */
int poll(pollfh fhs[], unsigned nfhs, int timeout);

#endif
