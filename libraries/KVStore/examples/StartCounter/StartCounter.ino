/*
 * Microcontroller startup counter example with Portenta c33 kvstore library
 * This simple example demonstrates using the KVStore library to store how many times
 * the microcontroller has booted. The KVStore library is based on mbed OS KVStore library
 *
 * This example is based on Martin Sloup (Arcao) StartCounter example for arduino-esp32
 */

#include <KVStore.h>
#include <TDBStore.h>
#include <MBRBlockDevice.h>

auto root = BlockDevice::get_default_instance();
MBRBlockDevice bd(root, 3);
TDBStore kvstore(&bd);

void setup() {
  Serial.begin(115200);
  Serial.println();

  while(!Serial);

  // Init KVStore
  if (kvstore.init() != KVSTORE_SUCCESS) {
    Serial.println("Cannot initialize kvstore");
    while(1) {};
  }

  // Remove all values stored in the kvstore
  // kvstore.reset();

  // Or remove the counter key only
  // kvstore.remove("counter");

  // Get the counter value, if it doesn't exist it returns KVSTORE_ERROR_ITEM_NOT_FOUND
  unsigned int counter;
  auto res = kvstore.get("counter", (void*)&counter, sizeof(counter));

  if (res == KVSTORE_ERROR_ITEM_NOT_FOUND) {
    counter = 0;
  } else if (res == KVSTORE_SUCCESS) {
    // Increase counter by 1
    counter++;
  } else {
    Serial.print("Error getting counter from kvstore: ");
    Serial.println(res);
  }

  // Print the counter to Serial Monitor
  Serial.print("Current counter value: ");
  Serial.println(counter);

  // Store the updated counter value to the kvstore
  if (kvstore.set("counter",(void*)&counter, sizeof(counter), 0) != KVSTORE_SUCCESS) {
    Serial.println("Error setting counter from kvstore");
  }

  // Close the kvstore
  if (kvstore.deinit() != KVSTORE_SUCCESS) {
    Serial.println("Cannot deinitialize kvstore");
    while(1) {};
  }

  // Wait 10 seconds
  Serial.println("Restarting in 10 seconds...");
  delay(10000);

  // Reset
  NVIC_SystemReset();
}

void loop() {}
