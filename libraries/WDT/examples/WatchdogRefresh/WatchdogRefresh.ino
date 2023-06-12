/*
  Watchdog Refresh

  This sketch shows how to enable the watchdog and
  refresh the timer to avoid resets

  Circuit:
   - Portenta C33
*/

#include <WDT.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  wdt_cfg_t p_cfg; 

  p_cfg.timeout = WDT_TIMEOUT_16384;
  p_cfg.clock_division = WDT_CLOCK_DIVISION_8192;
  p_cfg.window_start = WDT_WINDOW_START_100;
  p_cfg.window_end = WDT_WINDOW_END_0;
  p_cfg.reset_control = WDT_RESET_CONTROL_RESET;
  p_cfg.stop_control = WDT_STOP_CONTROL_ENABLE;

  WDT.begin(p_cfg);
}

void loop() {
  Serial.println("Still Alive...");
  // Comment the line above to stop refreshing the watchdog
  WDT.refresh();
  delay(1000);
}
