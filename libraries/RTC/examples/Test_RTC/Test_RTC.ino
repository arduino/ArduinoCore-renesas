/*
  Test RTC

  A test sketch showcasing all RTC functionalities related to the RTC module,
  including setting the time, handling interrupts, and reading time values.

  Find the full UNO R4 WiFi RTC documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc
*/

// Include the RTC library
#include "RTC.h"

// Define the pin to toggle on interrupt
const int PIN_ON_INTERRUPT  = D7;

bool periodicFlag = false;
bool alarmFlag = false;

// Callback function for periodic interrupt
void periodic_cbk() {
  periodicFlag = true;
}

// Callback function for alarm interrupt
void alarm_cbk() {
  alarmFlag = true;
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  // Wait for serial connection
  while (!Serial) {

  }

  // Set LED pins as outputs
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_ON_INTERRUPT, OUTPUT);

  // Initialize the RTC
  RTC.begin();

  // Set a specific initial time (August 25, 2022, 14:37:00 Thursday)
  RTCTime mytime(25, Month::AUGUST, 2022, 14, 37, 00, DayOfWeek::THURSDAY, SaveLight::SAVING_TIME_ACTIVE);

  // Set the initial time if RTC is not running.
  // The RTC may be still running if the board was reset, or if VRTC pin was powered
  if (!RTC.isRunning()) {
    RTC.setTime(mytime);
  }

  // Create an alarm time set to 35 seconds
  RTCTime alarmtime;
  alarmtime.setSecond(35);

  // Create an AlarmMatch object to match seconds
  AlarmMatch am;
  am.addMatchSecond();

  // Set the periodic callback function to run once every 2 seconds
  if (!RTC.setPeriodicCallback(periodic_cbk, Period::ONCE_EVERY_2_SEC)) {
    Serial.println("ERROR: periodic callback not set");
  }

  // Set the alarm callback function with the alarm time and matching condition
  if (!RTC.setAlarmCallback(alarm_cbk, alarmtime, am)) {
    Serial.println("ERROR: alarm callback not set");
  }
}

void loop() {
  static bool status = false;

  RTCTime currenttime;

  if(periodicFlag){
    // Print message indicating periodic interrupt
    Serial.println("PERIODIC INTERRUPT");

    static bool clb_st = false;

    // Toggle the LED based on callback state
    if (clb_st) {
      digitalWrite(PIN_ON_INTERRUPT, HIGH);
    }
    else {
      digitalWrite(PIN_ON_INTERRUPT, LOW);
    }

    clb_st = !clb_st;  // Toggle callback state

    periodicFlag = false;
  }

  if(alarmFlag){
    // Print message indicating alarm interrupt
    Serial.println("ALARM INTERRUPT");

    alarmFlag = false;
  }


  // Check if RTC is running and print status
  if (status) {

    // Toggle LED and display RTC status if 'status' is true
    if (RTC.isRunning()) {
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
