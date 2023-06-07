/*
 * This example demonstrates how to use the alarm functionality of the RTC
 * (Real Time Clock) on the Portenta C33.
 *
 * It switches the built-in LED between blue and red each time the alarm
 * is triggered, which is once every minute in this example.
 *
 */

#include "RTC.h"

void alarmCallback()
{
  static bool ledState = false;
  if (!ledState) {
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDR, LOW);
  }
  else {
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDB, LOW);
  }
  ledState = !ledState;
}

void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDB, LOW);

  // Initialize the RTC
  RTC.begin();

  // RTC.setTime() must be called for RTC.setAlarmCallback to work, but it doesn't matter
  // what date and time it's set to in this example
  RTCTime initialTime(7, Month::JUNE, 2023, 13, 03, 00, DayOfWeek::WEDNESDAY, SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(initialTime);

  // Trigger the alarm every time the seconds are zero
  RTCTime alarmTime;
  alarmTime.setSecond(0);

  // Make sure to only match on the seconds in this example - not on any other parts of the date/time
  AlarmMatch matchTime;
  matchTime.addMatchSecond();
  
  RTC.setAlarmCallback(alarmCallback, alarmTime, matchTime);
}

void loop() {
}
