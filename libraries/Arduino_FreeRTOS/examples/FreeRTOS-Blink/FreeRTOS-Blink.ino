/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino_FreeRTOS.h>

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

TaskHandle_t blinky_thread_hdl;

/**************************************************************************************
 * SETUP/LOOP
 **************************************************************************************/

void setup()
{
  Serial.begin(115200);
  while (!Serial) { }

  /* Init RTOS tasks. */
  auto const rc = xTaskCreate
    (
      blinky_thread_func,
      static_cast<const char*>("Blinky Thread"),
      512 / 4, /* usStackDepth in words */
      nullptr, /* pvParameters */
      1, /* uxPriority */
      &blinky_thread_hdl /* pxCreatedTask */
    );

  if (rc != pdPASS)
    return;

  Serial.println("Starting scheduler ...");
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
