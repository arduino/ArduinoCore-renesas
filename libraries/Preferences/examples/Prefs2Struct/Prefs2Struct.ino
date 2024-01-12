/*
This example shows how to use Preferences (nvs) to store a
structure.  Note that the maximum size of a putBytes is 496K
or 97% of the nvs partition size.  nvs has signifcant overhead,
so should not be used for data that will change often.
*/
#include <Preferences.h>
Preferences prefs;

typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t setting1;
  uint8_t setting2;
} schedule_t;

void setup() {
  Serial.begin(115200);

  if (!prefs.begin("schedule")) { // use "schedule" namespace
    Serial.println("Cannot initialize preferences");
    Serial.println("Make sure your WiFi firmware version is greater than 0.3.0");
    while(1) {};
  }
  uint8_t content[] = {9, 30, 235, 255, 20, 15, 0, 1}; // two entries
  prefs.putBytes("schedule", content, sizeof(content));
  size_t schLen = prefs.getBytesLength("schedule");
  char buffer[schLen]; // prepare a buffer for the data
  prefs.getBytes("schedule", buffer, schLen);
  if (schLen % sizeof(schedule_t)) { // simple check that data fits
    Serial.println("Data is not correct size!");
    return;
  }
  schedule_t *schedule = (schedule_t *) buffer; // cast the bytes into a struct ptr
  Serial.print(schedule[1].hour);
  Serial.print(":");
  Serial.print(schedule[1].minute);
  Serial.print(" ");
  Serial.print(schedule[1].setting1);
  Serial.print("/");
  Serial.print(schedule[1].setting2);
  Serial.println();

  schedule[2] = {8, 30, 20, 21}; // add a third entry (unsafely)

  // force the struct array into a byte array
  prefs.putBytes("schedule", schedule, 3*sizeof(schedule_t));
  schLen = prefs.getBytesLength("schedule");
  char buffer2[schLen];
  prefs.getBytes("schedule", buffer2, schLen);
  for (int x=0; x<schLen; x++) Serial.print((uint8_t)buffer[x]);
  Serial.println();
  prefs.end();
}

void loop() {}
