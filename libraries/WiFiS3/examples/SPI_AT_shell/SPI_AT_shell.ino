#include "Arduino.h"
#include "SPIProtocol.h"

uint16_t sizec = 0;
uint8_t txBuffer[4096];
uint8_t rxBuffer[4096];

//void isrRead() {
//  readflag = true;
//}

#ifdef ARDUINO_PORTENTA_H33

#ifndef NINA_GPIO0
#define NINA_GPIO0      (100)
#endif

#ifndef NINA_RESETN
#define NINA_RESETN     (101)
#endif

#else


#ifndef NINA_GPIO0
#define NINA_GPIO0      (28)
#endif

#ifndef NINA_RESETN
#define NINA_RESETN     (29)
#endif

#endif
void setup() {
  Serial.begin(115200);

  pinMode(NINA_GPIO0, OUTPUT);
  pinMode(NINA_RESETN, OUTPUT);

  digitalWrite(NINA_GPIO0, HIGH);
  delay(100);
  digitalWrite(NINA_RESETN, HIGH);
  digitalWrite(NINA_RESETN, LOW);
  digitalWrite(NINA_RESETN, HIGH);

  spiBegin();
}


void loop() {
  while (Serial.available()) {
    txBuffer[sizec] = Serial.read();
    sizec++;
  }
  if (sizec > 0) {
    Serial.println("sending");
    sizec *= 8;
    writeToSlave(txBuffer, &sizec);
    sizec = 0;
  }
  uint8_t raedableByte = readFromSlave(rxBuffer);
  if (raedableByte > 0) {
    for (int i = 0; i < raedableByte; i++) {
      Serial.print(char(rxBuffer[i]));
    }
  }
}
