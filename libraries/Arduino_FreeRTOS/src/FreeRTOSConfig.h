/* generated configuration header file - do not edit */
#ifndef FREERTOSCONFIG_H_
#define FREERTOSCONFIG_H_
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#ifndef configUSE_PREEMPTION
#define configUSE_PREEMPTION (1)
#endif
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
#define configUSE_PORT_OPTIMISED_TASK_SELECTION (0)
#endif
#ifndef configUSE_TICKLESS_IDLE
#define configUSE_TICKLESS_IDLE (0)
#endif
#ifndef configUSE_IDLE_HOOK
#define configUSE_IDLE_HOOK (1)
#endif
#ifndef configUSE_MALLOC_FAILED_HOOK
#define configUSE_MALLOC_FAILED_HOOK (0)
#endif
#ifndef configUSE_DAEMON_TASK_STARTUP_HOOK
#define configUSE_DAEMON_TASK_STARTUP_HOOK (0)
#endif
#ifndef configUSE_TICK_HOOK
#define configUSE_TICK_HOOK (0)
#endif
#ifndef configCPU_CLOCK_HZ
#define configCPU_CLOCK_HZ (SystemCoreClock)
#endif
#ifndef configTICK_RATE_HZ
#define configTICK_RATE_HZ (1000)
#endif
#ifndef configMAX_PRIORITIES
#define configMAX_PRIORITIES (5)
#endif
#ifndef configMINIMAL_STACK_SIZE
#define configMINIMAL_STACK_SIZE (128)
#endif
#ifndef configMINIMAL_SECURE_STACK_SIZE
#define configMINIMAL_SECURE_STACK_SIZE (256U)  /* Unused in RA port. */
#endif
#ifndef configMAX_TASK_NAME_LEN
#define configMAX_TASK_NAME_LEN (16)
#endif
#ifndef configUSE_TRACE_FACILITY
#define configUSE_TRACE_FACILITY (0)
#endif
#ifndef configUSE_STATS_FORMATTING_FUNCTIONS
#define configUSE_STATS_FORMATTING_FUNCTIONS (0)
#endif
#ifndef configUSE_16_BIT_TICKS
#define configUSE_16_BIT_TICKS (0)
#endif
#ifndef configIDLE_SHOULD_YIELD
#define configIDLE_SHOULD_YIELD (1)
#endif
#ifndef configUSE_TASK_NOTIFICATIONS
#define configUSE_TASK_NOTIFICATIONS (1)
#endif
#ifndef configUSE_MUTEXES
#define configUSE_MUTEXES (0)
#endif
#ifndef configUSE_RECURSIVE_MUTEXES
#define configUSE_RECURSIVE_MUTEXES (0)
#endif
#ifndef configUSE_COUNTING_SEMAPHORES
#define configUSE_COUNTING_SEMAPHORES (1)
#endif
#ifndef configUSE_ALTERNATIVE_API
#define configUSE_ALTERNATIVE_API (0U)
#endif
#ifndef configCHECK_FOR_STACK_OVERFLOW
#define configCHECK_FOR_STACK_OVERFLOW (0)
#endif
#ifndef configQUEUE_REGISTRY_SIZE
#define configQUEUE_REGISTRY_SIZE (10)
#endif
#ifndef configUSE_QUEUE_SETS
#define configUSE_QUEUE_SETS (0)
#endif
#ifndef configUSE_TIME_SLICING
#define configUSE_TIME_SLICING (0)
#endif
#ifndef configUSE_NEWLIB_REENTRANT
#define configUSE_NEWLIB_REENTRANT (0)
#endif
#ifndef configENABLE_BACKWARD_COMPATIBILITY
#define configENABLE_BACKWARD_COMPATIBILITY (0)
#endif
#ifndef configNUM_THREAD_LOCAL_STORAGE_POINTERS
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS (5)
#endif
#ifndef configSTACK_DEPTH_TYPE
#define configSTACK_DEPTH_TYPE uint32_t
#endif
#ifndef configMESSAGE_BUFFER_LENGTH_TYPE
#define configMESSAGE_BUFFER_LENGTH_TYPE size_t
#endif
#ifndef configSUPPORT_STATIC_ALLOCATION
#define configSUPPORT_STATIC_ALLOCATION (0)
#endif
#ifndef configSUPPORT_DYNAMIC_ALLOCATION
#define configSUPPORT_DYNAMIC_ALLOCATION (1)
#endif
#ifndef configTOTAL_HEAP_SIZE
# if defined(ARDUINO_PORTENTA_C33)
#   define configTOTAL_HEAP_SIZE (0x8000)
# elif (defined(ARDUINO_MINIMA) || defined(ARDUINO_UNOWIFIR4))
#   define configTOTAL_HEAP_SIZE (0x2000) /* R7FA4M1AB has 32 kByte RAM total, so we only allocate 8 kByte RAM for FreeRTOS heap. */
# else
#  error "Define the total allowed heap size dependent on your MCU's available RAM."
# endif
#endif
#ifndef configAPPLICATION_ALLOCATED_HEAP
#define configAPPLICATION_ALLOCATED_HEAP (4)
#endif
#ifndef configGENERATE_RUN_TIME_STATS
#define configGENERATE_RUN_TIME_STATS (0)
#endif
#ifndef configUSE_CO_ROUTINES
#define configUSE_CO_ROUTINES (0)
#endif
#ifndef configUSE_TIMERS
#define configUSE_TIMERS (1)
#endif
#ifndef configTIMER_TASK_PRIORITY
#define configTIMER_TASK_PRIORITY (3)
#endif
#ifndef configTIMER_QUEUE_LENGTH
#define configTIMER_QUEUE_LENGTH (10)
#endif
#ifndef configTIMER_TASK_STACK_DEPTH
#define configTIMER_TASK_STACK_DEPTH (128)
#endif
#ifndef configLIBRARY_LOWEST_INTERRUPT_PRIORITY
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY ((1 << __NVIC_PRIO_BITS) - 1)
#endif
#ifndef configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY ((1))
#endif
#ifndef configMAX_SYSCALL_INTERRUPT_PRIORITY
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - __NVIC_PRIO_BITS))
#endif
#ifndef configMAX_API_CALL_INTERRUPT_PRIORITY
#define configMAX_API_CALL_INTERRUPT_PRIORITY (configMAX_SYSCALL_INTERRUPT_PRIORITY)
#endif
#ifndef configASSERT
#define configASSERT( x ) assert(x)
#endif
#ifndef configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS (0)
#endif
#ifndef INCLUDE_vTaskPrioritySet
#define INCLUDE_vTaskPrioritySet (1)
#endif
#ifndef INCLUDE_uxTaskPriorityGet
#define INCLUDE_uxTaskPriorityGet (1)
#endif
#ifndef INCLUDE_vTaskDelete
#define INCLUDE_vTaskDelete (1)
#endif
#ifndef INCLUDE_vTaskSuspend
#define INCLUDE_vTaskSuspend (1)
#endif
#ifndef INCLUDE_xResumeFromISR
#define INCLUDE_xResumeFromISR (1)
#endif
#ifndef INCLUDE_vTaskDelayUntil
#define INCLUDE_vTaskDelayUntil (1)
#endif
#ifndef INCLUDE_vTaskDelay
#define INCLUDE_vTaskDelay (1)
#endif
#ifndef INCLUDE_xTaskGetSchedulerState
#define INCLUDE_xTaskGetSchedulerState (1)
#endif
#ifndef INCLUDE_xTaskGetCurrentTaskHandle
#define INCLUDE_xTaskGetCurrentTaskHandle (1)
#endif
#ifndef INCLUDE_uxTaskGetStackHighWaterMark
#define INCLUDE_uxTaskGetStackHighWaterMark (0)
#endif
#ifndef INCLUDE_xTaskGetIdleTaskHandle
#define INCLUDE_xTaskGetIdleTaskHandle (0)
#endif
#ifndef INCLUDE_eTaskGetState
#define INCLUDE_eTaskGetState (0)
#endif
#ifndef INCLUDE_xEventGroupSetBitFromISR
#define INCLUDE_xEventGroupSetBitFromISR (1)
#endif
#ifndef INCLUDE_xTimerPendFunctionCall
#define INCLUDE_xTimerPendFunctionCall (0)
#endif
#ifndef INCLUDE_xTaskAbortDelay
#define INCLUDE_xTaskAbortDelay (0)
#endif
#ifndef INCLUDE_xTaskGetHandle
#define INCLUDE_xTaskGetHandle (0)
#endif
#ifndef INCLUDE_xTaskResumeFromISR
#define INCLUDE_xTaskResumeFromISR (1)
#endif
#ifndef RM_FREERTOS_PORT_CFG_HW_STACK_MONITOR_ENABLE
#define RM_FREERTOS_PORT_CFG_HW_STACK_MONITOR_ENABLE  (0)
#endif
#ifndef configPRINT_STRING
#define configPRINT_STRING(x)                      (printf(x))
#endif
#ifndef configLOGGING_INCLUDE_TIME_AND_TASK_NAME
#define configLOGGING_INCLUDE_TIME_AND_TASK_NAME   (0)
#endif
#ifndef configLOGGING_MAX_MESSAGE_LENGTH
#define configLOGGING_MAX_MESSAGE_LENGTH           (192)
#endif

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif /* FREERTOSCONFIG_H_ */
