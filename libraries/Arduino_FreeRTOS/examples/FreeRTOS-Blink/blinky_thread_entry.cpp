#include "blinky_thread.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

extern "C" void blinky_thread_entry (void * pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    for(;;)
    {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      vTaskDelay(configTICK_RATE_HZ);
    }
}
