/* generated configuration header file - do not edit */
#ifndef FX_USER_H_
#define FX_USER_H_
#ifdef __cplusplus
extern "C" {
#endif

#if (+0)
#define FX_MAX_LONG_NAME_LEN ()
#endif
#if (+0)
#define FX_MAX_LAST_NAME_LEN ()
#endif
#if (1024+0)
#define FX_MAX_SECTOR_CACHE (1024)
#endif
#if (+0)
#define FX_FAT_MAP_SIZE ()
#endif
#if (1024+0)
#define FX_MAX_FAT_CACHE (1024)
#endif
#if (+0)
#define FX_UPDATE_RATE_IN_SECONDS ()
#define FX_UPDATE_RATE_IN_TICKS (FX_UPDATE_RATE_IN_SECONDS * TX_TIMER_TICKS_PER_SECOND)
#endif
#if (+0)
#define FX_FAULT_TOLERANT_BOOT_INDEX ()
#endif

#define FX_SINGLE_THREAD

#define FX_STANDALONE_ENABLE

#ifdef __cplusplus
}
#endif
#endif /* FX_USER_H_ */
