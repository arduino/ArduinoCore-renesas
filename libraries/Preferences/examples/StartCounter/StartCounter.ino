/*
 Microcontroller startup counter example with ESP32 Preferences library.

 This simple example demonstrates using the Preferences library to store how many times
 the microcontroller has booted. The Preferences library is a wrapper around the Non-volatile
 storage on ESP32 processor.

 created for arduino-esp32 09 Feb 2017
 by Martin Sloup (Arcao)
*/

#include <Preferences.h>

Preferences preferences;

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Open Preferences with my-app namespace. Each application module, library, etc
  // has to use a namespace name to prevent key name collisions. We will open storage in
  // RW-mode (second parameter has to be false).
  // Note: Namespace name is limited to 15 chars.
  if (!preferences.begin("my-app", false)) {
    Serial.println("Cannot initialize preferences");
    Serial.println("Make sure your WiFi firmware version is greater than 0.3.0");
    while(1) {};
  }

  // Remove all preferences under the opened namespace
  //preferences.clear();

  // Or remove the counter key only
  //preferences.remove("counter");

  // Get the counter value, if the key does not exist, return a default value of 0
  // Note: Key name is limited to 15 chars.
  unsigned int counter = preferences.getUInt("counter", 0);

  // Increase counter by 1
  counter++;

  // Print the counter to Serial Monitor
  Serial.print("Current counter value: ");
  Serial.println(counter);

  // Store the counter to the Preferences
  preferences.putUInt("counter", counter);

  // Close the Preferences
  preferences.end();

  // Wait 10 seconds
  Serial.println("Restarting in 10 seconds...");
  delay(10000);

  // Reset
  NVIC_SystemReset();
}

void loop() {}
