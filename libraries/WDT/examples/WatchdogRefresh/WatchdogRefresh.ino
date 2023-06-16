/*
  Watchdog Refresh

  This sketch shows how to enable the watchdog and
  refresh the timer to avoid resets

  Watchdog intervals are limited to 7 timeout periods
  the library will select the best clock divisor and timeout
  according to the selected wdtInterval.

  UNO R4 min wdtInterval 1ms / max wdtInterval 5592ms
  Comment out Serial.print() in the setup to make it work with
  small intervals

  Portenta C33 min wdtInterval 1ms / max wdtInterval 2684ms

  Circuit:
   - Portenta C33
   - UNO R4
*/

#include <WDT.h>

const long ledInterval = 1000;
unsigned long ledMillis = 0;
bool ledState = true;
const long wdtInterval = 2684;
unsigned long wdtMillis = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);

  if(wdtInterval < 1) {
    Serial.println("Invalid watchdog interval");
    while(1){}
  }

  if(WDT.begin(wdtInterval)) {
    Serial.print("WDT interval: ");
    WDT.refresh();
    Serial.print(WDT.getTimeout());
    WDT.refresh();
    Serial.println(" ms");
    WDT.refresh();
  } else {
    Serial.println("Error initializing watchdog");
    while(1){}
  }
}

void loop() {
  if(millis() - ledMillis >= ledInterval) {
    digitalWrite(LED_BUILTIN, ledState);
    ledState = !ledState;
    ledMillis = millis();
  }

  if(millis() - wdtMillis >= wdtInterval - 1) {
    WDT.refresh(); // Comment this line to stop refreshing the watchdog
    wdtMillis = millis();
  }

}
