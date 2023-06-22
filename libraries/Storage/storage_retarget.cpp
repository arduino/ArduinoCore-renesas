/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <time.h>

#include "FilePath.h"
#include "storage_common.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#ifndef SSIZE_MAX
#define SSIZE_MAX INT_MAX
#endif
#include <stdio.h>
#include <errno.h>
#include "storage_retarget.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

static PlatformMutex _mutex;

/* DIR is typedeffed to struct DIR_impl in header */
struct DIR_impl {
    DirHandle *handle;
    struct dirent entry;
};

#if defined(__ARMCC_VERSION)
#   include <arm_compat.h>
#   include <rt_sys.h>
#   include <rt_misc.h>
#   include <stdint.h>
#   define PREFIX(x)    _sys##x
#   define OPEN_MAX     _SYS_OPEN
#   ifdef __MICROLIB
asm(" .global __use_full_stdio\n");
#   endif

#elif defined(__ICCARM__)
#   include <yfuns.h>
#   define PREFIX(x)        _##x
#   define OPEN_MAX         16

#   define STDIN_FILENO     0
#   define STDOUT_FILENO    1
#   define STDERR_FILENO    2

#else
#   include <sys/syslimits.h>
#   define PREFIX(x)    x
#endif

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
#   define RETARGET_OPEN_MAX OPEN_MAX
#else
#   define RETARGET_OPEN_MAX        3
#endif // MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

#define FILE_HANDLE_RESERVED    ((FileHandle*)0xFFFFFFFF)

/**
 * Macros for setting console flow control.
 */
#define CONSOLE_FLOWCONTROL_RTS     1
#define CONSOLE_FLOWCONTROL_CTS     2
#define CONSOLE_FLOWCONTROL_RTSCTS  3
#define mbed_console_concat_(x) CONSOLE_FLOWCONTROL_##x
#define mbed_console_concat(x) mbed_console_concat_(x)
#define CONSOLE_FLOWCONTROL mbed_console_concat(MBED_CONF_TARGET_CONSOLE_UART_FLOW_CONTROL)



// Microlib currently does not allow re-defining the pathnames for the
// standard I/O device handles (STDIN, STDOUT, and STDERR).
// It uses the default pathname ":tt" at library initialization to identify
// them all.
#if !defined(__MICROLIB)
extern const char __stdin_name[]  = "/stdin";
extern const char __stdout_name[] = "/stdout";
extern const char __stderr_name[] = "/stderr";
#endif

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

/* newlib has the filehandle field in the FILE struct as a short, so
 * we can't just return a Filehandle* from _open and instead have to
 * put it in a filehandles array and return the index into that array
 */
static FileHandle *filehandles[RETARGET_OPEN_MAX] = { FILE_HANDLE_RESERVED, FILE_HANDLE_RESERVED, FILE_HANDLE_RESERVED };
static PlatformMutex filehandle_mutex;
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

static char stdio_in_prev[RETARGET_OPEN_MAX];
static char stdio_out_prev[RETARGET_OPEN_MAX];

//namespace mbed {
void mbed_set_unbuffered_stream(std::FILE *_file);

void remove_filehandle(FileHandle *file)
{
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    filehandle_mutex.lock();
    /* Remove all open filehandles for this */
    for (unsigned int fh_i = 0; fh_i < sizeof(filehandles) / sizeof(*filehandles); fh_i++) {
        if (filehandles[fh_i] == file) {
            filehandles[fh_i] = NULL;
        }
    }
    filehandle_mutex.unlock();
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
}
//}

class Sink : public FileHandle {
public:
    virtual ssize_t write(const void *buffer, size_t size);
    virtual ssize_t read(void *buffer, size_t size);
    virtual off_t seek(off_t offset, int whence = SEEK_SET)
    {
        return ESPIPE;
    }
    virtual off_t size()
    {
        return -EINVAL;
    }
    virtual int isatty()
    {
        return true;
    }
    virtual int close()
    {
        return 0;
    }
};

ssize_t Sink::write(const void *buffer, size_t size)
{
    // Just swallow the data - this is historical non-DEVICE_SERIAL behaviour
    return size;
}

ssize_t Sink::read(void *buffer, size_t size)
{
    // Produce 1 zero byte - historical behaviour returned 1 without touching
    // the buffer
    unsigned char *buf = static_cast<unsigned char *>(buffer);
    buf[0] = 0;
    return 1;
}

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
 FileHandle *mbed_target_override_console(int fd)
{
    return NULL;
}

__attribute__((weak)) FileHandle *mbed_override_console(int fd)
{
    return NULL;
}

static FileHandle *default_console()
{
#if MBED_CONF_TARGET_CONSOLE_UART && DEVICE_SERIAL

#  if MBED_CONF_PLATFORM_STDIO_BUFFERED_SERIAL
    static const serial_pinmap_t console_pinmap = get_uart_pinmap(CONSOLE_TX, CONSOLE_RX);
    static BufferedSerial console(console_pinmap, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
#   if   CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_RTS
    static const serial_fc_pinmap_t fc_pinmap = get_uart_fc_pinmap(STDIO_UART_RTS, NC);
    console.serial_set_flow_control(SerialBase::RTS, fc_pinmap);
#   elif CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_CTS
    static const serial_fc_pinmap_t fc_pinmap = get_uart_fc_pinmap(NC, STDIO_UART_CTS);
    console.serial_set_flow_control(SerialBase::CTS, fc_pinmap);
#   elif CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_RTSCTS
    static const serial_fc_pinmap_t fc_pinmap = get_uart_fc_pinmap(STDIO_UART_RTS, STDIO_UART_CTS);
    console.serial_set_flow_control(SerialBase::RTSCTS, fc_pinmap);
#   endif
#  else
    static const serial_pinmap_t console_pinmap = get_uart_pinmap(CONSOLE_TX, CONSOLE_RX);
    static DirectSerial console(console_pinmap, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
#  endif
#else // MBED_CONF_TARGET_CONSOLE_UART && DEVICE_SERIAL
    static Sink console;
#endif
    return &console;
}

/* Locate the default console for stdout, stdin, stderr */
static FileHandle *get_console(int fd)
{
    FileHandle *fh = mbed_override_console(fd);
    if (fh) {
        return fh;
    }
    fh = mbed_target_override_console(fd);
    if (fh) {
        return fh;
    }
    return default_console();
}
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

//namespace mbed {
/* Deal with the fact C library may not _open descriptors 0, 1, 2 - auto bind */
FileHandle *mbed_file_handle(int fd)
{
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    if (fd >= RETARGET_OPEN_MAX) {
        return NULL;
    }
    FileHandle *fh = filehandles[fd];
    if (fh == FILE_HANDLE_RESERVED && fd < 3) {
        filehandles[fd] = fh = get_console(fd);
    }
    return fh;
#else
    return nullptr;
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
}
//}


#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
/**
 * Sets errno when file opening fails.
 * Wipes out the filehandle too.
 *
 * @param error is a negative error code returned from an mbed function and
 *              will be negated to store a positive error code in errno
 */
static int handle_open_errors(int error, unsigned filehandle_idx)
{
    errno = -error;
    // Free file handle
    filehandles[filehandle_idx] = NULL;
    return -1;
}

static inline int openflags_to_posix(int openflags)
{
    int posix = openflags;
#ifdef __ARMCC_VERSION
    if (openflags & OPEN_PLUS) {
        posix = O_RDWR;
    } else if (openflags & OPEN_W) {
        posix = O_WRONLY;
    } else if (openflags & OPEN_A) {
        posix = O_WRONLY | O_APPEND;
    } else {
        posix = O_RDONLY;
    }
    /* a, w, a+, w+ all create if file does not already exist */
    if (openflags & (OPEN_A | OPEN_W)) {
        posix |= O_CREAT;
    }
    /* w and w+ truncate */
    if (openflags & OPEN_W) {
        posix |= O_TRUNC;
    }
#elif defined(__ICCARM__)
    switch (openflags & _LLIO_RDWRMASK) {
        case _LLIO_RDONLY:
            posix = O_RDONLY;
            break;
        case _LLIO_WRONLY:
            posix = O_WRONLY;
            break;
        case _LLIO_RDWR  :
            posix = O_RDWR  ;
            break;
    }
    if (openflags & _LLIO_CREAT) {
        posix |= O_CREAT;
    }
    if (openflags & _LLIO_APPEND) {
        posix |= O_APPEND;
    }
    if (openflags & _LLIO_TRUNC) {
        posix |= O_TRUNC;
    }
#elif defined(TOOLCHAIN_GCC)
    posix &= ~O_BINARY;
#endif
    return posix;
}

static int reserve_filehandle()
{
    // find the first empty slot in filehandles, after the slots reserved for stdin/stdout/stderr
    filehandle_mutex.lock();
    int fh_i;
    for (fh_i = 3; fh_i < RETARGET_OPEN_MAX; fh_i++) {
        /* Take a next free filehandle slot available. */
        if (filehandles[fh_i] == NULL) {
            break;
        }
    }
    if (fh_i >= RETARGET_OPEN_MAX) {
        /* Too many file handles have been opened */
        errno = EMFILE;
        filehandle_mutex.unlock();
        return -1;
    }
    filehandles[fh_i] = FILE_HANDLE_RESERVED;
    filehandle_mutex.unlock();

    return fh_i;
}


int bind_to_fd(FileHandle *fh)
{
    int fildes = reserve_filehandle();
    if (fildes < 0) {
        return fildes;
    }

    filehandles[fildes] = fh;
    stdio_in_prev[fildes] = 0;
    stdio_out_prev[fildes] = 0;

    return fildes;
}

static int unbind_from_fd(int fd, FileHandle *fh)
{
    if (filehandles[fd] == fh) {
        filehandles[fd] = NULL;
        return 0;
    } else {
        errno = EBADF;
        return -1;
    }
}

#ifndef __IAR_SYSTEMS_ICC__
/* IAR provides fdopen itself */
extern "C" std::FILE *fdopen(int fildes, const char *mode)
{
    // This is to avoid scanf and the bloat it brings.
    char buf[1 + sizeof fildes]; /* @(integer) */
    static_assert(sizeof buf == 5, "Integers should be 4 bytes.");
    buf[0] = '@';
    memcpy(buf + 1, &fildes, sizeof fildes);

    std::FILE *stream = std::fopen(buf, mode);
    /* newlib-nano doesn't appear to ever call _isatty itself, so
     * happily fully buffers an interactive stream. Deal with that here.
     */
    if (stream && isatty(fildes)) {
        mbed_set_unbuffered_stream(stream);
    }
    return stream;
}
#endif

//namespace mbed {
std::FILE *fdopen(FileHandle *fh, const char *mode)
{
    // First reserve the integer file descriptor
    int fd = bind_to_fd(fh);
    if (fd < 0) {
        return NULL;
    }
    // Then bind that to the C stream. If successful, C library
    // takes ownership and responsibility to close.
    std::FILE *stream = ::fdopen(fd, mode);
    if (!stream) {
        unbind_from_fd(fd, fh);
    }
    return stream;
}
//}
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY


/* @brief   standard c library fopen() retargeting function.
 *
 * This function is invoked by the standard c library retargeting to handle fopen()
 *
 * @return
 *  On success, a valid FILEHANDLE is returned.
 *  On failure, -1 is returned and errno is set to an appropriate value e.g.
 *   ENOENT     file not found (default errno setting)
 *   EMFILE     the maximum number of open files was exceeded.
 *
 * */
extern "C" FILEHANDLE PREFIX(_open)(const char *name, int openflags)
{
#if defined(__MICROLIB)
    // Use the mode requested to select the standard I/O device handle to return.
    if (std::strcmp(name, ":tt") == 0) {
        if (openflags & OPEN_W) {
            return STDOUT_FILENO;
        } else if (openflags & OPEN_A) {
            return STDERR_FILENO;
        } else {
            return STDIN_FILENO;
        }
    }
#else
    /* Use the posix convention that stdin,out,err are filehandles 0,1,2.
     */
    if (strcmp(name, __stdin_name) == 0) {
        mbed_file_handle(STDIN_FILENO);
        return STDIN_FILENO;
    } else if (strcmp(name, __stdout_name) == 0) {
        mbed_file_handle(STDOUT_FILENO);
        return STDOUT_FILENO;
    } else if (strcmp(name, __stderr_name) == 0) {
        mbed_file_handle(STDERR_FILENO);
        return STDERR_FILENO;
    }
#endif
#ifndef __IAR_SYSTEMS_ICC__
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    /* FILENAME: "@(integer)" gives an already-allocated descriptor */
    if (name[0] == '@') {
        int fd;
        memcpy(&fd, name + 1, sizeof fd);
        return fd;
    }
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
#endif
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    return open(name, openflags_to_posix(openflags));
#else
    return -1;
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
}

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
extern "C" int open(const char *name, int oflag, ...)
{
    int fildes = reserve_filehandle();
    if (fildes < 0) {
        return fildes;
    }

    FileHandle *res = NULL;
    FilePath path(name);

    if (!path.exists()) {
        /* The first part of the filename (between first 2 '/') is not a
         * registered mount point in the namespace.
         */
        return handle_open_errors(-ENODEV, fildes);
    }

    if (path.isFile()) {
        res = path.file();
    } else {
        FileSystemHandle *fs = path.fileSystem();
        if (fs == NULL) {
            return handle_open_errors(-ENODEV, fildes);
        }
        int err = fs->open(&res, path.fileName(), oflag);
        if (err) {
            return handle_open_errors(err, fildes);
        }
    }

    filehandles[fildes] = res;
    stdio_in_prev[fildes] = 0;
    stdio_out_prev[fildes] = 0;

    return fildes;
}
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

extern "C" int PREFIX(_close)(FILEHANDLE fh)
{
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    return close(fh);
#else
    return 0;
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
}

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
extern "C" int close(int fildes)
{
    FileHandle *fhc = mbed_file_handle(fildes);
    filehandles[fildes] = NULL;
    if (fhc == NULL) {
        errno = EBADF;
        return -1;
    }

    int err = fhc->close();
    if (err < 0) {
        errno = -err;
        return -1;
    } else {
        return 0;
    }
}
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

static bool convert_crlf(int fd)
{
#if MBED_CONF_PLATFORM_STDIO_CONVERT_TTY_NEWLINES
    return isatty(fd);
#elif MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
    return fd < 3 && isatty(fd);
#else
    return false;
#endif
}

#if defined(__ICCARM__)
extern "C" size_t    __write(int        fh, const unsigned char *buffer, size_t length)
{
#else
extern "C" int PREFIX(_write)(FILEHANDLE fh, const unsigned char *buffer, unsigned int length, int mode)
{
#endif

#if defined(MBED_TRAP_ERRORS_ENABLED) && MBED_TRAP_ERRORS_ENABLED && defined(MBED_CONF_RTOS_PRESENT)
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_PROHIBITED_IN_ISR_CONTEXT), "Error - writing to a file in an ISR or critical section\r\n", fh);
    }
#endif

    if (length > SSIZE_MAX) {
        errno = EINVAL;
        return -1;
    }

    ssize_t slength = length;
    ssize_t written = 0;

    if (convert_crlf(fh)) {
        // local prev is previous in buffer during seek
        // stdio_out_prev[fh] is last thing actually written
        char prev = stdio_out_prev[fh];
        // Seek for '\n' without preceding '\r'; if found flush
        // preceding and insert '\r'. Continue until end of input.
        for (ssize_t cur = 0; cur < slength; cur++) {
            if (buffer[cur] == '\n' && prev != '\r') {
                ssize_t r;
                // flush stuff preceding the \n
                if (cur > written) {
                    r = write(fh, buffer + written, cur - written);
                    if (r < 0) {
                        return -1;
                    }
                    written += r;
                    if (written < cur) {
                        // For some reason, didn't write all - give up now
                        goto finish;
                    }
                    stdio_out_prev[fh] = prev;
                }
                // insert a \r now, leaving the \n still to be written
                r = write(fh, "\r", 1);
                if (r < 0) {
                    return -1;
                }
                if (r < 1) {
                    goto finish;
                }
                stdio_out_prev[fh] = '\r';
            }
            prev = buffer[cur];
        }
    }

    // Flush remaining from conversion, or the whole thing if no conversion
    if (written < slength) {
        ssize_t r = write(fh, buffer + written, slength - written);
        if (r < 0) {
            return -1;
        }
        written += r;
        if (written > 0) {
            stdio_out_prev[fh] = buffer[written - 1];
        }
    }

finish:
#ifdef __ARMCC_VERSION
    if (written >= 0) {
        return slength - written;
    } else {
        return written;
    }
#else
    return written;
#endif
}

extern "C" ssize_t write(int fildes, const void *buf, size_t length)
{
#if MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    if (fildes != STDOUT_FILENO && fildes != STDERR_FILENO) {
        errno = EBADF;
        return -1;
    }

    const unsigned char *buffer = static_cast<const unsigned char *>(buf);

    for (size_t i = 0; i < length; i++) {
        minimal_console_putc(buffer[i]);
    }

    ssize_t ret = length;
#else
    FileHandle *fhc = mbed_file_handle(fildes);
    if (fhc == NULL) {
        errno = EBADF;
        return -1;
    }

    ssize_t ret = fhc->write(buf, length);
#endif // MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return ret;
    }
}

#if MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
/* Write one character to a serial interface */
__attribute__((weak)) int minimal_console_putc(int c)
{
#if MBED_CONF_TARGET_CONSOLE_UART && DEVICE_SERIAL
    do_serial_init_once();
    serial_putc(&stdio_uart, c);
#endif // MBED_CONF_TARGET_CONSOLE_UART && DEVICE_SERIAL
    return c;
}
#endif // MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

#if defined (__ARMCC_VERSION)
extern "C" void PREFIX(_exit)(int return_code)
{
    while (1) {}
}

extern "C" void _ttywrch(int ch)
{
    char c = ch;
    write(STDOUT_FILENO, &c, 1);
}
#endif

#if defined(__ICCARM__)
extern "C" size_t    __read(int        fh, unsigned char *buffer, size_t       length)
{
#else
extern "C" int PREFIX(_read)(FILEHANDLE fh, unsigned char *buffer, unsigned int length, int mode)
{
#endif

#if defined(MBED_TRAP_ERRORS_ENABLED) && MBED_TRAP_ERRORS_ENABLED && defined(MBED_CONF_RTOS_PRESENT)
    if (core_util_is_isr_active() || !core_util_are_interrupts_enabled()) {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_PROHIBITED_IN_ISR_CONTEXT), "Error - reading from a file in an ISR or critical section\r\n", fh);
    }
#endif

    if (length > SSIZE_MAX) {
        errno = EINVAL;
        return -1;
    }

    ssize_t bytes_read = 0;

    if (convert_crlf(fh)) {
        while (true) {
            char c;
            ssize_t r = read(fh, &c, 1);
            if (r < 0) {
                return -1;
            }
            if (r == 0) {
                return bytes_read;
            }
            if ((c == '\r' && stdio_in_prev[fh] != '\n') ||
                    (c == '\n' && stdio_in_prev[fh] != '\r')) {
                stdio_in_prev[fh] = c;
                *buffer = '\n';
                break;
            } else if ((c == '\r' && stdio_in_prev[fh] == '\n') ||
                       (c == '\n' && stdio_in_prev[fh] == '\r')) {
                stdio_in_prev[fh] = c;
                continue;
            } else {
                stdio_in_prev[fh] = c;
                *buffer = c;
                break;
            }
        }
        bytes_read = 1;
    } else {
        bytes_read = read(fh, buffer, length);
    }

#ifdef __ARMCC_VERSION
    if (bytes_read < 0) {
        return -1;
    } else if (bytes_read == 0) {
        return 0x80000000 | length; // weird EOF indication
    } else {
        return (ssize_t)length - bytes_read;
    }
#else
    return bytes_read;
#endif
}

extern "C" ssize_t read(int fildes, void *buf, size_t length)
{
#if MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    if (fildes != STDIN_FILENO && fildes != STDERR_FILENO) {
        errno = EBADF;
        return -1;
    }

    if (length == 0) {
        return 0;
    }

    unsigned char *buffer = static_cast<unsigned char *>(buf);

    buffer[0] = minimal_console_getc();

    ssize_t ret = 1;

#else
    FileHandle *fhc = mbed_file_handle(fildes);
    if (fhc == NULL) {
        errno = EBADF;
        return -1;
    }

    ssize_t ret = fhc->read(buf, length);
#endif // MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    if (ret < 0) {
        errno = -ret;
        return -1;
    } else {
        return ret;
    }
}

#if MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
/* Read a character from the serial interface */
__attribute__((weak)) int minimal_console_getc()
{
#if MBED_CONF_TARGET_CONSOLE_UART && DEVICE_SERIAL
    do_serial_init_once();
    return serial_getc(&stdio_uart);
#else
    return 0;
#endif // MBED_CONF_TARGET_CONSOLE_UART && DEVICE_SERIAL
}
#endif // MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY


#ifdef __ARMCC_VERSION
extern "C" int PREFIX(_istty)(FILEHANDLE fh)
#else
extern "C" int _isatty(FILEHANDLE fh)
#endif
{
    return isatty(fh);
}


extern "C" int isatty(int fildes)
{
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    FileHandle *fhc = mbed_file_handle(fildes);
    if (fhc == NULL) {
        errno = EBADF;
        return 0;
    }

    int tty = fhc->isatty();
    if (tty < 0) {
        errno = -tty;
        return 0;
    } else {
        return tty;
    }
#else
    // Is attached to an interactive device
    return 1;
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
}

extern "C"
#if defined(__ARMCC_VERSION)
int _sys_seek(FILEHANDLE fh, long offset)
#elif defined(__ICCARM__)
long __lseek(int fh, long offset, int whence)
#else
int _lseek(FILEHANDLE fh, int offset, int whence)
#endif
{
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
#if defined(__ARMCC_VERSION)
    int whence = SEEK_SET;
#endif

    off_t off = lseek(fh, offset, whence);
    // Assuming INT_MAX = LONG_MAX, so we don't care about prototype difference
    // coverity[result_independent_of_operands]
    if (off > INT_MAX) {
        // Be cautious in case off_t is 64-bit
        errno = EOVERFLOW;
        return -1;
    }
    return off;
#else
    // Not supported
    return -1;
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
}

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
extern "C" off_t lseek(int fildes, off_t offset, int whence)
{
    FileHandle *fhc = mbed_file_handle(fildes);
    if (fhc == NULL) {
        errno = EBADF;
        return -1;
    }

    off_t off = fhc->seek(offset, whence);
    if (off < 0) {
        errno = -off;
        return -1;
    }
    return off;
}

extern "C" int ftruncate(int fildes, off_t length)
{
    FileHandle *fhc = mbed_file_handle(fildes);
    if (fhc == NULL) {
        errno = EBADF;
        return -1;
    }

    int err = fhc->truncate(length);
    if (err < 0) {
        errno = -err;
        return -1;
    } else {
        return 0;
    }
}

#ifdef __ARMCC_VERSION
extern "C" int PREFIX(_ensure)(FILEHANDLE fh)
{
    return fsync(fh);
}
#endif
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

extern "C" int fsync(int fildes)
{
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    FileHandle *fhc = mbed_file_handle(fildes);
    if (fhc == NULL) {
        errno = EBADF;
        return -1;
    }

    int err = fhc->sync();
    if (err < 0) {
        errno = -err;
        return -1;
    } else {
        return 0;
    }
#else
    return -1;
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
}


#ifdef __ARMCC_VERSION
extern "C" long PREFIX(_flen)(FILEHANDLE fh)
{
#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    FileHandle *fhc = mbed_file_handle(fh);
    if (fhc == NULL) {
        errno = EBADF;
        return -1;
    }

    off_t size = fhc->size();
    if (size < 0) {
        errno = -size;
        return -1;
    }
    if (size > LONG_MAX) {
        errno = EOVERFLOW;
        return -1;
    }
    return size;
#else
    // Not supported
    return -1;
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
}
#endif

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
#if !defined(__ARMCC_VERSION) && !defined(__ICCARM__)
extern "C" int _fstat(int fh, struct stat *st)
{
    return fstat(fh, st);
}
#endif

extern "C" int fstat(int fildes, struct stat *st)
{
    FileHandle *fhc = mbed_file_handle(fildes);
    if (fhc == NULL) {
        errno = EBADF;
        return -1;
    }

    st->st_mode = fhc->isatty() ? S_IFCHR : S_IFREG;
    st->st_size = fhc->size();
    return 0;
}

extern "C" int fcntl(int fildes, int cmd, ...)
{
    FileHandle *fhc = mbed_file_handle(fildes);
    if (fhc == NULL) {
        errno = EBADF;
        return -1;
    }

    switch (cmd) {
        case F_GETFL: {
            int flags = 0;
            if (!fhc->is_blocking()) {
                flags |= O_NONBLOCK;
            }
            return flags;
        }
        case F_SETFL: {
            va_list ap;
            va_start(ap, cmd);
            int flags = va_arg(ap, int);
            va_end(ap);
            int ret = fhc->set_blocking(!(flags & O_NONBLOCK));
            if (ret < 0) {
                errno = -ret;
                return -1;
            }

            return 0;
        }

        default: {
            errno = EINVAL;
            return -1;
        }
    }
}

extern "C" int poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
    if (nfds > RETARGET_OPEN_MAX) {
        errno = EINVAL;
        return -1;
    }

    struct pollfh fhs[RETARGET_OPEN_MAX];
    for (nfds_t n = 0; n < nfds; n++) {
        // Underlying FileHandle poll returns POLLNVAL if given NULL, so
        // we don't need to take special action.
        fhs[n].fh = mbed_file_handle(fds[n].fd);
        fhs[n].events = fds[n].events;
    }
    int ret = poll(fhs, nfds, timeout);
    for (nfds_t n = 0; n < nfds; n++) {
        fds[n].revents = fhs[n].revents;
    }
    return ret;
}
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

namespace std {
extern "C" int remove(const char *path)
{
    FilePath fp(path);
    FileSystemHandle *fs = fp.fileSystem();
    if (fs == NULL) {
        errno = ENODEV;
        return -1;
    }

    int err = fs->remove(fp.fileName());
    if (err < 0) {
        errno = -err;
        return -1;
    } else {
        return 0;
    }
}

extern "C" int rename(const char *oldname, const char *newname)
{
    FilePath fpOld(oldname);
    FilePath fpNew(newname);
    FileSystemHandle *fsOld = fpOld.fileSystem();
    FileSystemHandle *fsNew = fpNew.fileSystem();

    if (fsOld == NULL) {
        errno = ENODEV;
        return -1;
    }

    /* rename only if both files are on the same FS */
    if (fsOld != fsNew) {
        errno = EXDEV;
        return -1;
    }

    int err = fsOld->rename(fpOld.fileName(), fpNew.fileName());
    if (err < 0) {
        errno = -err;
        return -1;
    } else {
        return 0;
    }
}

extern "C" char *tmpnam(char *s)
{
    errno = EBADF;
    return NULL;
}

extern "C" FILE *tmpfile()
{
    errno = EBADF;
    return NULL;
}
} // namespace std

#ifdef __ARMCC_VERSION
extern "C" char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}
#endif

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
extern "C" DIR *opendir(const char *path)
{
    FilePath fp(path);
    FileSystemHandle *fs = fp.fileSystem();
    if (fs == NULL) {
        errno = ENODEV;
        return NULL;
    }

    DIR *dir = new DIR;
    if (!dir) {
        errno = ENOMEM;
        return NULL;
    }

    int err = fs->open(&dir->handle, fp.fileName());
    if (err < 0) {
        delete dir;
        errno = -err;
        return NULL;
    }

    return dir;
}

extern "C" struct dirent *readdir(DIR *dir)
{
    int err = dir->handle->read(&dir->entry);
    if (err < 1) {
        if (err < 0) {
            errno = -err;
        }
        return NULL;
    }

    return &dir->entry;
}

extern "C" int closedir(DIR *dir)
{
    int err = dir->handle->close();
    delete dir;
    if (err < 0) {
        errno = -err;
        return -1;
    } else {
        return 0;
    }
}

extern "C" void rewinddir(DIR *dir)
{
    dir->handle->rewind();
}

extern "C" off_t telldir(DIR *dir)
{
    return dir->handle->tell();
}

extern "C" void seekdir(DIR *dir, off_t off)
{
    dir->handle->seek(off);
}

extern "C" int mkdir(const char *path, mode_t mode)
{
    FilePath fp(path);
    FileSystemHandle *fs = fp.fileSystem();
    if (fs == NULL) {
        errno = ENODEV;
        return -1;
    }

    int err = fs->mkdir(fp.fileName(), mode);
    if (err < 0) {
        errno = -err;
        return -1;
    } else {
        return 0;
    }
}

extern "C" int stat(const char *path, struct stat *st)
{
    FilePath fp(path);
    FileSystemHandle *fs = fp.fileSystem();
    if (fs == NULL) {
        errno = ENODEV;
        return -1;
    }

    int err = fs->stat(fp.fileName(), st);
    if (err < 0) {
        errno = -err;
        return -1;
    } else {
        return 0;
    }
}

extern "C" int statvfs(const char *path, struct statvfs *buf)
{
    FilePath fp(path);
    FileSystemHandle *fs = fp.fileSystem();
    if (fs == NULL) {
        errno = ENODEV;
        return -1;
    }

    int err = fs->statvfs(fp.fileName(), buf);
    if (err < 0) {
        errno = -err;
        return -1;
    } else {
        return 0;
    }
}
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY


//namespace mbed {

void mbed_set_unbuffered_stream(std::FILE *_file)
{
#if defined (__ICCARM__)
    char buf[2];
    std::setvbuf(_file, buf, _IONBF, NULL);
#else
    setbuf(_file, NULL);
#endif
}

//} // namespace mbed
