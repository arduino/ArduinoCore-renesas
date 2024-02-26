/*
  SE05X AES Encrypt and Decrypt

  This sketch uses the SE05X to encrypt and decrypt given data
  with an AES key, which was uploaded in the SE05XWriteAESKey
  example. This sketch encrypts 32 bytes with AES ECB Mode
  and then decrypt this data for comparing the results.

  Circuit:
   - Portenta C33
*/

#include <SE05X.h>


const int AES_KEY_ID = 666;
const byte data[32] = {
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
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


  size_t encrypted_data_len = 32;
  size_t decrypted_data_len = 32;
  byte encrypted_data[32];
  byte decrypted_data[32];

  int status1 = SE05X.AES_ECB_encrypt(AES_KEY_ID,           data, sizeof(data), encrypted_data, &encrypted_data_len);
  int status2 = SE05X.AES_ECB_decrypt(AES_KEY_ID, encrypted_data, sizeof(data), decrypted_data, &decrypted_data_len);
  print_hex(data,32);
  print_hex(encrypted_data,32);
  print_hex(decrypted_data,32);
}

void loop() {

}