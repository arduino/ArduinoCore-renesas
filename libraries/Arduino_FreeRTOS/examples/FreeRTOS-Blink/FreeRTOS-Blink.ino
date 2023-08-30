/*
  The code demonstrates the usage of FreeRTOS (Real-Time Operating System) to run concurrent tasks.

  One task is responsible for running the loop() logic (in a thread-safe manner), 
  while the other task blinks an LED using the built-in LED on non-Portenta boards or 
  the RGB LED on the Portenta C33 board.
*/

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino_FreeRTOS.h>

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

TaskHandle_t loop_task, blinky_task;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(115200);
  while (!Serial) { }

#if defined(ARDUINO_PORTENTA_C33)
  /* Only the Portenta C33 has an RGB LED. */
  pinMode(LEDR, OUTPUT);
  digitalWrite(LEDR, LOW);
#endif

  /* Init a task that calls 'loop'
   * since after the call to
   * 'vTaskStartScheduler' we'll never
   * get out of setup() and therefore
   * would never get to loop(), as we
   * are leaving the default execution
   * flow.
   */
  auto const rc_loop = xTaskCreate
    (
      loop_thread_func,
      static_cast<const char*>("Loop Thread"),
      512 / 4,   /* usStackDepth in words */
      nullptr,   /* pvParameters */
      1,         /* uxPriority */
      &loop_task /* pxCreatedTask */
    );

  if (rc_loop != pdPASS) {
    Serial.println("Failed to create 'loop' thread");
    return;
  }

  auto const rc_blinky = xTaskCreate
    (
      blinky_thread_func,
      static_cast<const char*>("Blinky Thread"),
      512 / 4,     /* usStackDepth in words */
      nullptr,     /* pvParameters */
      1,           /* uxPriority */
      &blinky_task /* pxCreatedTask */
    );

  if (rc_blinky != pdPASS) {
    Serial.println("Failed to create 'loop' thread");
    return;
  }

  Serial.println("Starting scheduler ...");
  /* Start the scheduler. */
  vTaskStartScheduler();
  /* We'll never get here. */
  for( ;; );
}

void loop()
{
#if defined(ARDUINO_PORTENTA_C33)
  /* Only the Portenta C33 has an RGB LED. */
  digitalWrite(LEDR, !digitalRead(LEDR));
#else
  Serial.println(millis());
#endif
  vTaskDelay(configTICK_RATE_HZ/4);
}

void loop_thread_func(void *pvParameters)
{
  for(;;)
  {
    loop();
    taskYIELD();
  }
}

void blinky_thread_func(void *pvParameters)
{
  /* setup() */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  /* loop() */
  for(;;)
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    vTaskDelay(configTICK_RATE_HZ);
  }
}
