/* generated configuration header file - do not edit */
#ifndef TX_USER_H_
#define TX_USER_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TX_TIMER_TICKS_PER_SECOND
#define TX_TIMER_TICKS_PER_SECOND (1000)
#endif
#ifndef TX_MAX_PRIORITIES
#define TX_MAX_PRIORITIES (32)
#endif
#ifndef TX_MINIMUM_STACK
#define TX_MINIMUM_STACK (200)
#endif
#ifndef TX_TIMER_THREAD_STACK_SIZE
#define TX_TIMER_THREAD_STACK_SIZE (1024)
#endif
#ifndef TX_TIMER_THREAD_PRIORITY
#define TX_TIMER_THREAD_PRIORITY (0)
#endif

/* TX_ENABLE_STACK_CHECKING is not configurable because the hardware stack monitor (SPMON or PSPLIM) should be used instead. */
#define TX_DISABLE_PREEMPTION_THRESHOLD
#ifndef TX_ENABLE_REDUNDANT_CLEARING
/* TX_DISABLE_REDUNDANT_CLEARING is not configurable because FSP projects zero the .bss section during startup. */
#define TX_DISABLE_REDUNDANT_CLEARING
#endif
#define TX_TIMER_PROCESS_IN_ISR

#ifdef TX_NO_TIMER
/* If TX_NO_TIMER is used, the define TX_TIMER_PROCESS_IN_ISR must also be used. */
#ifndef TX_TIMER_PROCESS_IN_ISR
#define TX_TIMER_PROCESS_IN_ISR
#endif
#endif
#define TX_DISABLE_NOTIFY_CALLBACKS

#define RM_THREADX_PORT_CFG_HW_STACK_MONITOR_ENABLE  (0)
#define TX_PORT_CFG_SYSTICK_IPL  ((0))
#define TX_PORT_MAX_IPL  ((0))
#ifdef TX_ENABLE_EVENT_TRACE
#define TX_USER_TRACE_BUFFER_DECLARE static uint8_t g_tx_trace_buffer[65536]BSP_ALIGN_VARIABLE(4) BSP_PLACE_IN_SECTION(".bss");
#define TX_USER_ENABLE_TRACE  tx_trace_enable(&g_tx_trace_buffer[0], 65536, 30);
#else
#define TX_USER_TRACE_BUFFER_DECLARE
#define TX_USER_ENABLE_TRACE
#endif

#if defined(TX_ENABLE_IAR_LIBRARY_SUPPORT) && !defined(__ICCARM__)
/* IAR library support is only for the IAR compiler. */
#undef TX_ENABLE_IAR_LIBRARY_SUPPORT
#endif
#ifndef _RA_TZ_NONSECURE
/* Treat as secure unless this is a non-secure project. */
#define TX_SINGLE_MODE_SECURE
#endif
#define TX_THREAD_EXTENSION_1 int bsd_errno;

#ifdef __cplusplus
}
#endif
#endif /* TX_USER_H_ */
