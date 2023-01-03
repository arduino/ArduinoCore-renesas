#include "RTC.h"

const int LED_ON_INTERRUPT  = 22;

void periodic_cbk() {
  static bool clb_st = false;
  if(clb_st) {
    digitalWrite(LED_ON_INTERRUPT,HIGH);
  }
  else {
    digitalWrite(LED_ON_INTERRUPT,LOW);
  }
  clb_st = !clb_st;
 
  Serial.println("PERIODIC INTERRUPT");
}

void alarm_cbk() {
  Serial.println("ALARM INTERRUPT");
}

void setup() {
  Serial.begin(9600);
  while(!Serial) {
    
  }
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_ON_INTERRUPT, OUTPUT);
  
  RTC.begin();
  RTCTime mytime(25, Month::AUGUST, 2022, 14, 37, 00, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_ACTIVE);

  if(!RTC.isRunning()) {
    RTC.setTime(mytime);
  }

  RTCTime alarmtime;
  alarmtime.setSecond(35);

  AlarmMatch am;
  am.addMatchSecond();

  if(!RTC.setPeriodicCallback(periodic_cbk, Period::ONCE_EVERY_2_SEC)) {
    Serial.println("ERROR: periodic callback not set");
  }
  
  if(!RTC.setAlarmCallback(alarm_cbk, alarmtime, am)) {
    Serial.println("ERROR: alarm callback not set");
  }

}

void loop() {
  static bool status = false;
  
  RTCTime currenttime;
  if(status) {
    
    if(RTC.isRunning()) {
      Serial.println("RTC is running");
    }
    else {
      Serial.println("RTC is not running");
    }
    
    /* GET CURRENT TIME FROM RTC */
    RTC.getTime(currenttime);
    
    /* PRINT CURRENT TIME on Serial */
    Serial.print("Current time: ");
    /* DATE */
    Serial.print(currenttime.getDayOfMonth());
    Serial.print("/");
    Serial.print(Month2int(currenttime.getMonth()));
    Serial.print("/");
    Serial.print(currenttime.getYear());
    Serial.print(" - ");
    
    /* ORE:MINUTI:SECONDI */
    Serial.print(currenttime.getHour());
    Serial.print(":");
    Serial.print(currenttime.getMinutes());
    Serial.print(":");
    Serial.println(currenttime.getSeconds());
    
    digitalWrite(LED_BUILTIN, HIGH); 
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);  
  }

  status = !status;
  delay(1000);
}
