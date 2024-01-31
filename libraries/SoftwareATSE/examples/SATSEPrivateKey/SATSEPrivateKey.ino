/*
  Software Secure Element Private Key

  This sketch uses the Software Secure Element to generate a new EC NIST P-256 keypair
  and store it with id 999, then the public key is printed in raw format.

  Circuit:
   - UNO R4 WiFi
*/

#include <SoftwareATSE.h>
#include <Wire.h>

const int KeyId = 999;
byte rawBuf[64];

void printBufferHex(const byte input[], size_t inputLength) {
  for (size_t i = 0; i < inputLength; i++) {
    Serial.print(input[i] >> 4, HEX);
    Serial.print(input[i] & 0x0f, HEX);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!SATSE.begin()) {
    Serial.println("Failed to communicate with Software Secure Element!");
    Serial.println("Make sure your WiFi firmware version is greater than 0.3.0");
    while (1);
  }

  SATSE.generatePrivateKey(KeyId, rawBuf);
  printBufferHex(rawBuf, sizeof(rawBuf));

}

void loop() {
  SATSE.generatePublicKey(KeyId, rawBuf);
  printBufferHex(rawBuf, sizeof(rawBuf));
  delay(5000);
}
