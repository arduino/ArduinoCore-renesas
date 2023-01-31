#include "storage_common.h"
#include "FileHandle.h"

#ifdef STORAGE_DEBUG

char debug_buffer[STORAGE_BUFF_DIM];

#endif

uint32_t core_util_atomic_incr_u32(volatile uint32_t *valuePtr, uint32_t delta)
{
    return *valuePtr += delta;
}

uint32_t core_util_atomic_decr_u32(volatile uint32_t *valuePtr, uint32_t delta)
{
    return *valuePtr -= delta;
}

uint8_t core_util_atomic_load_u8(const volatile uint8_t *valuePtr) {
    return *valuePtr;
}

void core_util_atomic_store_u8(volatile uint8_t *valuePtr, uint8_t desiredValue) {
    *valuePtr = desiredValue;
}



extern void delay(uint32_t ms);
extern unsigned long millis();

// timeout -1 forever, or milliseconds
int poll(pollfh fhs[], unsigned nfhs, int timeout)
{
    /*
     * TODO Proper wake-up mechanism.
     * In order to correctly detect availability of read/write a FileHandle, we needed
     * a select or poll mechanisms. We opted for poll as POSIX defines in
     * http://pubs.opengroup.org/onlinepubs/009695399/functions/poll.html Currently,
     * mbed::poll() just spins and scans filehandles looking for any events we are
     * interested in. In future, his spinning behaviour will be replaced with
     * condition variables.
     */
    uint64_t start_time = 0;
    if (timeout > 0) {
        start_time = millis();
    }

    int count = 0;
    for (;;) {
        /* Scan the file handles */
        for (unsigned n = 0; n < nfhs; n++) {
            FileHandle *fh = fhs[n].fh;
            short mask = fhs[n].events | POLLERR | POLLHUP | POLLNVAL;
            if (fh) {
                fhs[n].revents = fh->poll(mask) & mask;
            } else {
                fhs[n].revents = POLLNVAL;
            }
            if (fhs[n].revents) {
                count++;
            }
        }

        if (count) {
            break;
        }

        /* Nothing selected - this is where timeout handling would be needed */
        if (timeout == 0 || (timeout > 0 && int64_t(millis() - start_time) > timeout)) {
            break;
        }
        // TODO - proper blocking
        // wait for condition variable, wait queue whatever here
        delay(1000);
    }
    return count;
}


