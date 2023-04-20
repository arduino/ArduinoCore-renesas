#include <Arduino_FreeRTOS.h>

#include "blinky_thread.h"

extern void blinky_thread_create(void);
extern TaskHandle_t blinky_thread;
uint32_t g_fsp_common_thread_count;
bool g_fsp_common_initialized;
SemaphoreHandle_t g_fsp_common_initialized_semaphore;
#if configSUPPORT_STATIC_ALLOCATION
StaticSemaphore_t g_fsp_common_initialized_semaphore_memory;
#endif
void g_hal_init(void);
/*********************************************************************************************************************
 * @brief This is a weak example initialization error function. It should be overridden by defining a user function
 * with the prototype below.
 * - void rtos_startup_err_callback(void * p_instance, void * p_data)
 *
 * @param[in] p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void rtos_startup_err_callback(void *p_instance, void *p_data)
{
  /** Suppress compiler warning for not using parameters. */
  FSP_PARAMETER_NOT_USED (p_instance);
  FSP_PARAMETER_NOT_USED (p_data);

  /** An error has occurred. Please check function arguments for more information. */
  BSP_CFG_HANDLE_UNRECOVERABLE_ERROR (0);
}

void rtos_startup_common_init(void);
void rtos_startup_common_init(void)
{
  /* First thread will take care of common initialization. */
  BaseType_t err;
  err = xSemaphoreTake (g_fsp_common_initialized_semaphore, portMAX_DELAY);
  if (pdPASS != err)
  {
    /* Check err, problem occurred. */
    rtos_startup_err_callback (g_fsp_common_initialized_semaphore, 0);
  }

  /* Only perform common initialization if this is the first thread to execute. */
  if (false == g_fsp_common_initialized)
  {
    /* Later threads will not run this code. */
    g_fsp_common_initialized = true;

    /* Perform common module initialization. */
    //g_hal_init ();

    /* Now that common initialization is done, let other threads through. */
    /* First decrement by 1 since 1 thread has already come through. */
    g_fsp_common_thread_count--;
    while (g_fsp_common_thread_count > 0)
    {
      err = xSemaphoreGive (g_fsp_common_initialized_semaphore);
      if (pdPASS != err)
      {
        /* Check err, problem occurred. */
        rtos_startup_err_callback (g_fsp_common_initialized_semaphore, 0);
      }
      g_fsp_common_thread_count--;
    }
  }
}


void setup()
{
  g_fsp_common_thread_count = 0;
  g_fsp_common_initialized = false;

  /* Create semaphore to make sure common init is done before threads start running. */
  g_fsp_common_initialized_semaphore =
#if configSUPPORT_STATIC_ALLOCATION
    xSemaphoreCreateCountingStatic(
#else
    xSemaphoreCreateCounting (
#endif
      256,
      1
#if configSUPPORT_STATIC_ALLOCATION
      , &g_fsp_common_initialized_semaphore_memory
#endif
    );

  if (NULL == g_fsp_common_initialized_semaphore)
  {
    rtos_startup_err_callback (g_fsp_common_initialized_semaphore, 0);
  }

  /* Init RTOS tasks. */
  blinky_thread_create ();

  /* Start the scheduler. */
  vTaskStartScheduler ();
}

void loop()
{

}

#if configSUPPORT_STATIC_ALLOCATION
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                uint32_t *pulIdleTaskStackSize) BSP_WEAK_REFERENCE;
                void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer,
                uint32_t *pulTimerTaskStackSize) BSP_WEAK_REFERENCE;

                /* If configSUPPORT_STATIC_ALLOCATION is set to 1, the application must provide an
                * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
                * used by the Idle task. */
                void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                                    StackType_t **  ppxIdleTaskStackBuffer,
                                                    uint32_t * pulIdleTaskStackSize )
                {
                    /* If the buffers to be provided to the Idle task are declared inside this
                    * function then they must be declared static - otherwise they will be allocated on
                    * the stack and so not exists after this function exits. */
                    static StaticTask_t xIdleTaskTCB;
                    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

                    /* Pass out a pointer to the StaticTask_t structure in which the Idle
                    * task's state will be stored. */
                    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

                    /* Pass out the array that will be used as the Idle task's stack. */
                    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

                    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
                    * Note that, as the array is necessarily of type StackType_t,
                    * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
                    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
                }

                /* If configSUPPORT_STATIC_ALLOCATION is set to 1, the application must provide an
                * implementation of vApplicationGetTimerTaskMemory() to provide the memory that is
                * used by the RTOS daemon/time task. */
                void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                                     StackType_t **  ppxTimerTaskStackBuffer,
                                                     uint32_t *      pulTimerTaskStackSize )
                {
                    /* If the buffers to be provided to the Timer task are declared inside this
                    * function then they must be declared static - otherwise they will be allocated on
                    * the stack and so not exists after this function exits. */
                    static StaticTask_t xTimerTaskTCB;
                    static StackType_t uxTimerTaskStack[ configMINIMAL_STACK_SIZE ];

                    /* Pass out a pointer to the StaticTask_t structure in which the Idle
                    * task's state will be stored. */
                    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

                    /* Pass out the array that will be used as the Timer task's stack. */
                    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

                    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
                    * Note that, as the array is necessarily of type StackType_t,
                    * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
                    *pulTimerTaskStackSize = configMINIMAL_STACK_SIZE;
                }
#endif
