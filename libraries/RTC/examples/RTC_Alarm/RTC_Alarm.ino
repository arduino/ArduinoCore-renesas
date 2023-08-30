/*
 * RTC_Alarm
 *
 * This example demonstrates how to use the alarm functionality of the RTC
 * (Real Time Clock) on the Portenta C33 and UNO R4 Minima / WiFi.
 *
 * It turns on the built-in LED when the alarm
 * is triggered, which is once every minute in this example.
 * In addition, inside the loop, we print the state of the alarm
 * continuously, which is either 0 (LOW) or 1 (HIGH).
 *
 * Note that the Portenta C33's LED is inverted and will be lit when
 * the state is 0 (LOW).
 * 
 * Find the full UNO R4 WiFi RTC documentation here:
 * https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc
 */

unsigned long previousMillis = 0;
const long interval = 1000;
bool ledState = false;

// Include the RTC library
#include "RTC.h"

void setup() {
  //initialize Serial Communication
  Serial.begin(9600);

  //define LED as output
  pinMode(LED_BUILTIN, OUTPUT);

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

  //sets the alarm callback
  RTC.setAlarmCallback(alarmCallback, alarmTime, matchTime);
}

void loop() {

  // in the loop, we continuously print the alarm's current state
  // this is for debugging only and has no effect on the alarm whatsoever
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    Serial.print("Alarm state: ");
    Serial.println(ledState);
  }
}

// this function activates every minute
// and changes the ledState boolean
void alarmCallback() {
  if (!ledState) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  ledState = !ledState;
}
