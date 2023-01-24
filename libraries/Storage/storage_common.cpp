#include "storage_common.h"

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
