/*
  Software Secure Element SignAndVerify

  This sketch uses the Software Secure Element to generate a new EC NIST P-256 keypair
  and store it with id 999, then input buffer SHA256 is signed with the private
  key and verified with the public key.

  Circuit:
   - UNO R4 WiFi
*/

#include <SoftwareATSE.h>

const byte input[64] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
};

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

  const int KeyId = 999;
  byte pubKey[256];

  SATSE.generatePrivateKey(KeyId, pubKey);

  // print the public key
  Serial.print("Public key is:                ");
  printBufferHex(pubKey, 64);

  // print the input
  Serial.print("Input is:                     ");
  printBufferHex(input, sizeof(input));

  // calculate the input SHA256
  byte sha256[256];
  size_t sha256Len;
  SATSE.SHA256(input, sizeof(input), sha256);
  Serial.print("Input SHA256 is:              ");
  printBufferHex(sha256, 32);

  // calculate the signature, input MUST be SHA256
  byte signature[256];
  SATSE.ecSign(KeyId, sha256, signature);

  // print the signature
  Serial.print("Signature using KeyId ");
  Serial.print(KeyId);
  Serial.print(" is: ");
  printBufferHex(signature, 64);

  Serial.println();

  // To make the signature verifcation fail, uncomment the next line:
  //  signature[0] = 0x00;

  // validate the signature
  if (SATSE.ecdsaVerify(sha256, signature, pubKey)) {
    Serial.println("Verified signature successfully :D");
  } else {
    Serial.println("oh no! failed to verify signature :(");
  }
}

void loop() {

}
