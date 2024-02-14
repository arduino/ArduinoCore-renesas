/*
  SE05X HMAC

  This sketch uses the SE05X to securely store and use an HMAC Key
  and calculates the HMAC-SHA512 sum of a given message

  Circuit:
   - Portenta C33
*/

#include <SE05X.h>

const int HMAC_KEY_ID = 667;
const byte hmac_key[32] = {
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};


void print_hex(const byte in[], size_t len) {
  for (size_t i = 0; i < len; i++) {
    Serial.print(in[i] >> 4, HEX);
    Serial.print(in[i] & 0x0f, HEX);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (!SE05X.begin()) {
    Serial.println("Error with secure element");
    while(1);
  }

  SE05X.deleteBinaryObject(HMAC_KEY_ID); //remove key if already exists
  
  SE05X.writeHMACKey(HMAC_KEY_ID, hmac_key, sizeof(hmac_key));
  String message = "This is a test for the Arduino Portenta C33 to test HMAC with SHA512 algorithm. This are some bytes of data, which should work";


  byte buffer[64];
  for(int i = 0; i < 64; i++)
    buffer[i] = 0;

  size_t len = 64; // 64 byte = 512 bit, length for SHA512
  SE05X.HMAC_Generate(HMAC_KEY_ID, kSE05x_MACAlgo_HMAC_SHA512, (const byte*)message.c_str(), message.length(), buffer, &len);
  print_hex(buffer,len);
}

void loop() {

}
