/*
  Portenta C33 - Test Code Flash

  The sketch shows how to use code flash and tests read write and 
  erase operations.

  This example code is in the public domain.
*/

#include <Arduino.h>
#include <BlockDevice.h>
#include <CodeFlashBlockDevice.h>

CodeFlashBlockDevice& root = CodeFlashBlockDevice::getInstance();

#define WRITE_SIZE (16 * 1024)

byte w_buffer[WRITE_SIZE];
byte r_buffer[WRITE_SIZE];

void setup() {
  Serial.begin(9600);
  while(!Serial) {
    
  }

  Serial.println("QSPIFlash Test");

  int ret = root.init();
  if(ret) {
    Serial.println("Error opening QSPIFlash device");
    while(1){}
  }

  Serial.println("QSPIFlash init done");

  randomSeed(analogRead(A0));

}

void loop() {

  // Select a random pattern to write flash
  byte pattern = random(255);

  // Select a random size for the test buffer
  int  number  = random(128, WRITE_SIZE);
  int  repeat  = number & ~(root.get_program_size() - 1);

  // Select a random start address in region 0 after the bootloader and compute block start offset
  int  address = random(0x00004000, 0x00010000 - WRITE_SIZE);
  int  offset  = address & ~(root.get_program_size() - 1);

  // Start Test
  erase_write_read_compare(pattern, offset, repeat);

  // Select a random start address in region 1 after the sketch and compute block start offset
  address = random(0x000A0000, root.size() - WRITE_SIZE);
  offset  = address & ~(root.get_program_size() - 1);
  
  // Start Test
  erase_write_read_compare(pattern, offset, repeat);
}

void erase_write_read_compare(byte pattern, int offset, int repeat) {
  Serial.print("Using pattern ");
  Serial.print(pattern, HEX);
  Serial.print(" ");
  Serial.print(repeat);
  Serial.print(" times starting from ");
  Serial.println(offset, HEX);

  memset(&w_buffer[0], pattern, repeat);

  // Double check we are using only one memory region for each test run
  if(root.get_erase_size(offset) != root.get_erase_size(offset + repeat )) {
    Serial.println("Error: different secotr size not handled");
    return;
  }
  // Start offset is aligned to program size but erase command needs to be
  // aligned to erase block size
  int erase_start = offset & ~(root.get_erase_size(offset) - 1);
  // Since we are using only one memory reagion erase size is always the same
  int erase_size = root.get_erase_size(erase_start);
  // Check if we need to delete more consecutive blocks
  // Should happen only in region 0 due to write buffer size
  int erased = 0;
  while(erase_start + erased < offset + repeat ) {
    root.erase(erase_start + erased, erase_size);
    erased += erase_size;
  }

  // Write pattern
  root.program(&w_buffer[0], offset, repeat);

  // Readback
  root.read(&r_buffer[0], offset, repeat);
  //dump_buffer(&r_buffer[0], repeat);

  // Compare
  if(memcmp(&w_buffer[0], &r_buffer[0], repeat)) {
    Serial.println("Error comparing buffers, dumping content...");
    Serial.println("Write buffer:");
    dump_buffer(&w_buffer[0], repeat);
    Serial.println("Read buffer:");
    dump_buffer(&r_buffer[0], repeat);
    while(1){}
  }
  delay(5);
}

void dump_buffer(uint8_t *b, uint32_t len) {
  if (b != nullptr) {
    Serial.println("");
    for(int i = 0; i < len; i++) {
      if(i != 0 && i % 32 == 0) {
        if(i != 0)
          Serial.println();
      }
      Serial.print(*(b + i) >> 4,  HEX);
      Serial.print(*(b + i) & 0x0F,HEX);
    }
    Serial.println();
    Serial.println("");
  }
}
