/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino_FreeRTOS.h>

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

static StaticTask_t blinky_thread_memory;
static uint8_t blinky_thread_stack[512] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.blinky_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
static TaskHandle_t blinky_thread;
const rm_freertos_port_parameters_t blinky_thread_parameters = { .p_context = (void*) NULL, };

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  /* Init RTOS tasks. */
  blinky_thread = xTaskCreateStatic (
    blinky_thread_func,
    (const char*) "Blinky Thread", 512 / 4, // In words, not bytes
    (void*) &blinky_thread_parameters, //pvParameters
    1,
    (StackType_t*) &blinky_thread_stack,
    (StaticTask_t*) &blinky_thread_memory
  );

  /* Start the scheduler. */
  vTaskStartScheduler ();
}

void loop()
{

}

void blinky_thread_func(void *pvParameters)
{
  /* blinky_thread.setup() */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  /* blinky_thread.loop() */
  for(;;)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    vTaskDelay(configTICK_RATE_HZ);
  }
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
