/*
  SE05X WriteAESKey

  This sketch uses the SE05X to securely store an AES key. When the key is uploaded to
  SE05X, the sketch on the Arduino should be overwritten. 

  Circuit:
   - Portenta C33
*/

#include <SE05X.h>

const int AES_KEY_ID = 666;
const byte aes_key[32] = {
  0xFF, 0x33, 0xFF, 0x33, 0xFF, 0x33, 0xFF, 0x33, 0xFF, 0x33, 0xFF, 0x33, 0x33, 0x33, 0xFF, 0x33,
  0xFF, 0x33, 0xFF, 0x33, 0xFF, 0x33, 0xFF, 0x33, 0xFF, 0x33, 0xFF, 0x33, 0x33, 0x33, 0xFF, 0x44
};

void setup() {
  Serial.begin(9600);
  
  if (!SE05X.begin()) {
    Serial.println("Error with secure element");
    while(1);
  }

  SE05X.deleteBinaryObject(AES_KEY_ID);
  SE05X.writeAESKey(AES_KEY_ID, aes_key, sizeof(aes_key));
}

void loop() {

}
