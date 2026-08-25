/*
 * RTC_AutomaticExample
 *
 * This example sets the RTC (Real Time Clock) on the Portenta C33 automatically by
 * retrieving the date and time from the computer you upload the sketch from, at the
 * point when you start the upload.
 *
 * Next, it gets the current time from the RTC and prints it to the Serial Monitor.
 * It then sets an RTC alarm to fire every time the seconds value of the time is zero.
 * The alarm, which now goes off once a minute, triggers a callback that prints the
 * current time to the Serial Monitor.
 *
 * Find the full UNO R4 WiFi RTC documentation here:
 * https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc
 */

// Include the RTC library
#include "RTC.h"

bool alarmFlag = false;

DayOfWeek convertDayOfWeek(String s)
{
  if (s == String("Mon"))
  {
    return DayOfWeek::MONDAY;
  }
  if (s == String("Tue"))
  {
    return DayOfWeek::TUESDAY;
  }
  if (s == String("Wed"))
  {
    return DayOfWeek::WEDNESDAY;
  }  
  if (s == String("Thu"))
  {
    return DayOfWeek::THURSDAY;
  }  
  if (s == String("Fri"))
  {
    return DayOfWeek::FRIDAY;
  }  
  if (s == String("Sat"))
  {
    return DayOfWeek::SATURDAY;
  }  
  if (s == String("Sun"))
  {
    return DayOfWeek::SUNDAY;
  }  
}

Month convertMonth(String s)
{
  if (s == String("Jan"))
  {
    return Month::JANUARY;
  }
  if (s == String("Feb"))
  {
    return Month::FEBRUARY;
  }
  if (s == String("Mar"))
  {
    return Month::MARCH;
  }
  if (s == String("Apr"))
  {
    return Month::APRIL;
  }
  if (s == String("May"))
  {
    return Month::MAY;
  }
  if (s == String("Jun"))
  {
    return Month::JUNE;
  }
  if (s == String("Jul"))
  {
    return Month::JULY;
  }
  if (s == String("Aug"))
  {
    return Month::AUGUST;
  }
  if (s == String("Sep"))
  {
    return Month::SEPTEMBER;
  }
  if (s == String("Oct"))
  {
    return Month::OCTOBER;
  }  
  if (s == String("Nov"))
  {
    return Month::NOVEMBER;
  }
  if (s == String("Dec"))
  {
    return Month::DECEMBER;
  }  
}

RTCTime currentRTCTime()
{
  // Get a compilation timestamp of the format: Wed May 10 08:54:31 2023
  // __TIMESTAMP__ is a GNU C extension macro
  // We can't use the standard macros __DATE__ and __TIME__ because they don't provide the day of the week
  String timeStamp = __TIMESTAMP__;
  // Extract the day of the week
  int pos1 = timeStamp.indexOf(" ");
  DayOfWeek dayOfWeek = convertDayOfWeek(timeStamp.substring(0, pos1));
  // Extract the month
  ++pos1;
  int pos2 = timeStamp.indexOf(" ", pos1);
  Month month = convertMonth(timeStamp.substring(pos1, pos2));
  // Extract the day
  pos1 = ++pos2;
  pos2 = timeStamp.indexOf(" ", pos1);
  int day = timeStamp.substring(pos1, pos2).toInt();
  // Extract the hour
  pos1 = ++pos2;
  pos2 = timeStamp.indexOf(":", pos1);
  int hour = timeStamp.substring(pos1, pos2).toInt();
  // Extract the minute
  pos1 = ++pos2;
  pos2 = timeStamp.indexOf(":", pos1);
  int minute = timeStamp.substring(pos1, pos2).toInt();
  // Extract the second
  pos1 = ++pos2;
  pos2 = timeStamp.indexOf(" ", pos1);
  int second = timeStamp.substring(pos1, pos2).toInt();
  // Extract the year
  pos1 = ++pos2;
  pos2 = timeStamp.indexOf(" ", pos1);
  int year = timeStamp.substring(pos1, pos2).toInt();

  return RTCTime(day, month, year, hour, minute, second, dayOfWeek, SaveLight::SAVING_TIME_INACTIVE);
}

void alarmCallback() {
 alarmFlag = true;
}

void setup()
{
  Serial.begin(9600);
  while (!Serial) ;
  
  // Initialize the RTC
  RTC.begin();

  // Get the current date and time when the sketch is uploaded and set the RTC
  RTCTime timeToSet = currentRTCTime();
  RTC.setTime(timeToSet);

  // Retrieve the date and time from the RTC and print them
  Serial.println("The RTC was just set to: ");
  RTCTime currentTime;
  RTC.getTime(currentTime); 
  Serial.print(currentTime.getYear());
  Serial.print("-");
  Serial.print(Month2int(currentTime.getMonth()));
  Serial.print("-");
  Serial.print(currentTime.getDayOfMonth());
  Serial.print(" ");
  Serial.print(currentTime.getHour());
  Serial.print(":");
  Serial.print(currentTime.getMinutes());
  Serial.print(":");
  Serial.println(currentTime.getSeconds());

  // Create an alarm time with the seconds value set to zero
  RTCTime alarmTime;
  alarmTime.setSecond(0);
  // Tell the RTC to only match on the seconds value
  AlarmMatch alarmMatch;
  alarmMatch.addMatchSecond();
  // Set the alarm callback function
  RTC.setAlarmCallback(alarmCallback, alarmTime, alarmMatch);
}

void loop()
{
  if(alarmFlag){
  Serial.println("An alarm was triggered at:");
  RTCTime currentTime;
  RTC.getTime(currentTime); 
  Serial.print(currentTime.getYear());
  Serial.print("-");
  Serial.print(Month2int(currentTime.getMonth()));
  Serial.print("-");
  Serial.print(currentTime.getDayOfMonth());
  Serial.print(" ");
  Serial.print(currentTime.getHour());
  Serial.print(":");
  Serial.print(currentTime.getMinutes());
  Serial.print(":");
  Serial.println(currentTime.getSeconds());
  alarmFlag = false;
  }
}
