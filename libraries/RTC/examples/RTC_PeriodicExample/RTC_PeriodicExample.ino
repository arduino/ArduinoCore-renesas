/*
 * RTC_PeriodicExample
 *
 * This example demonstrates how to use the periodic callback functionality of the RTC
 * (Real Time Clock) on the Portenta C33.
 *
 * It blinks the built-in LED at progressively faster and slower rates repeatedly.
 *
 * Find the full UNO R4 WiFi RTC documentation here:
 * https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc
 */

// Include the RTC library
#include "RTC.h"

#define BLINK_DELAY 2000

// This is the callback function to be passed to RTC.setPeriodicCallback()
void periodicCallback()
{
  static bool ledState = false;
  if (ledState == true) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  ledState = !ledState;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
    
  // Initialize the RTC
  RTC.begin();

  // RTC.setTime() must be called for RTC.setPeriodicCallback to work, but it doesn't matter
  // what date and time it's set to
  RTCTime mytime(25, Month::AUGUST, 2022, 14, 37, 00, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(mytime);
}

void loop() {
  RTC.setPeriodicCallback(periodicCallback, Period::ONCE_EVERY_1_SEC);
  delay(BLINK_DELAY);
  RTC.setPeriodicCallback(periodicCallback, Period::N2_TIMES_EVERY_SEC);
  delay(BLINK_DELAY);
  RTC.setPeriodicCallback(periodicCallback, Period::N4_TIMES_EVERY_SEC);
  delay(BLINK_DELAY);
  RTC.setPeriodicCallback(periodicCallback, Period::N8_TIMES_EVERY_SEC);
  delay(BLINK_DELAY);
  RTC.setPeriodicCallback(periodicCallback, Period::N16_TIMES_EVERY_SEC);
  delay(BLINK_DELAY);
  RTC.setPeriodicCallback(periodicCallback, Period::N32_TIMES_EVERY_SEC);
  delay(BLINK_DELAY);
  RTC.setPeriodicCallback(periodicCallback, Period::N16_TIMES_EVERY_SEC);
  delay(BLINK_DELAY);
  RTC.setPeriodicCallback(periodicCallback, Period::N8_TIMES_EVERY_SEC);
  delay(BLINK_DELAY);
  RTC.setPeriodicCallback(periodicCallback, Period::N4_TIMES_EVERY_SEC);
  delay(BLINK_DELAY);
  RTC.setPeriodicCallback(periodicCallback, Period::N2_TIMES_EVERY_SEC);
  delay(BLINK_DELAY);
  RTC.setPeriodicCallback(periodicCallback, Period::ONCE_EVERY_1_SEC);
  delay(BLINK_DELAY);
}
