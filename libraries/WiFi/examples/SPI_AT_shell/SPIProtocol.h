#include "Arduino.h"
#include "SPI.h"

//#define

SPIClass& spi = SPI1;
SPISettings spiSettings = SPISettings(4000000, MSBFIRST, SPI_MODE0);

#ifdef ARDUINO_PORTENTA_C33
int cs = 103;
byte hs = 100;
#else
int cs = 31;
byte hs = 28;
#endif

volatile bool readflag = false;

void writeSpi(void *data, int size) {

  digitalWrite(cs, LOW);

  delayMicroseconds(1);
  spi.beginTransaction(spiSettings);

  spi.transfer(data, size);

  //spi.transfer(data, size);
  spi.endTransaction();
  digitalWrite(cs, HIGH);
}

uint32_t readSpi() {
  uint32_t read = 0x00;

  digitalWrite(cs, LOW);

  delayMicroseconds(1);

  spi.beginTransaction(spiSettings);


  for (int i = 0; i < 4; i++) {
    read <<= 8;
    read |= spi.transfer(0);
    read = spi.transfer(0);
  }

  spi.endTransaction();

  digitalWrite(cs, HIGH);
  return read;
}

void readSpi(uint8_t *data, int size, uint8_t *readBuffer) {
  uint8_t read[4096];

  digitalWrite(cs, LOW);

  delayMicroseconds(1);

  spi.beginTransaction(spiSettings);

  for (int i = 0; i < size; i++) {
    readBuffer[i] = spi.transfer(data[i]);
  }
  spi.endTransaction();

  digitalWrite(cs, HIGH);
}

void isrRead() {
  readflag = true;
}

void spiBegin() {
  pinMode(cs, OUTPUT);
  pinMode(hs, INPUT);
  digitalWrite(cs, HIGH);
  spi.begin();
  attachInterrupt(hs, isrRead, RISING);
}

void notifyWrite() {
  // Notify write
  uint8_t bufferw[7];
  bufferw[0] = 0x01;
  bufferw[1] = 0x00;
  bufferw[2] = 0x00;
  bufferw[3] = 0XFE;
  bufferw[4] = 0X10;
  bufferw[5] = 0x04;
  bufferw[6] = 0;
  writeSpi((void *)bufferw, sizeof(bufferw));
}


void queryTxStatus() {
  uint8_t buffers[7];
  buffers[0] = 0x02;
  buffers[1] = 0x04;
  buffers[2] = 0x00;
  buffers[3] = 0xFF;
  buffers[4] = 0xFF;
  buffers[5] = 0xFF;
  buffers[6] = 0xFF;
  writeSpi((void *)buffers, sizeof(buffers));
}

void txSpiRequest(uint8_t * data, uint16_t * size) {
  // write data
  uint8_t bufferc[4096];
  bufferc[0] = 0x03;
  bufferc[1] = 0x00;
  bufferc[2] = 0x00;
  for (int i = 0; i < (*size) / 8; i++) {
    bufferc[i + 3] = (uint8_t) data[i];
  }
  writeSpi((void *)bufferc, (*size) / 8 + 3);
}

void txDone() {
  // write done
  uint8_t bufferd[3];
  bufferd[0] = 0x07;
  bufferd[1] = 0x00;
  bufferd[2] = 0x00;
  writeSpi((void *)bufferd, sizeof(bufferd));
}

void writeToSlave(uint8_t *data, uint16_t * size) {
  notifyWrite();
  delay(10);
  if (!readflag) {
    if (digitalRead(hs)) {
      Serial.println("no readflag but hs high");
    } else {
      Serial.println("no resp");
      return;
    }
  }
  readflag = false;

  // query status
  queryTxStatus();

  txSpiRequest(data, size);
  txDone();
}



uint8_t queryRxStatus() {
  uint8_t buffers[7];
  uint8_t raedableByte = 0x00;
  buffers[0] = 0x02;
  buffers[1] = 0x04;
  buffers[2] = 0x00;
  buffers[3] = 0xFF;
  buffers[4] = 0xFF;
  buffers[5] = 0xFF;
  buffers[6] = 0xFF;

  uint8_t readBuffer[4096];
  readSpi(buffers, sizeof(buffers), readBuffer);

  raedableByte = readBuffer[5];
  return raedableByte;
}

void sendRxSpiRequest(uint8_t *rxBuffer, uint8_t raedableByte) {
  // read data
  uint8_t bufferc[4096];
  bufferc[0] = 0x04;
  bufferc[1] = 0x00;
  bufferc[2] = 0x00;
  for (int i = 0; i < raedableByte; i++) {
    bufferc[3 + i] = 0xFF;
  }

  readSpi(bufferc, 3 + raedableByte, rxBuffer);
}

void rxDone() {
  // read done
  uint8_t bufferd[3];
  bufferd[0] = 0x08;
  bufferd[1] = 0x00;
  bufferd[2] = 0x00;
  writeSpi((void *)bufferd, sizeof(bufferd));
}

uint8_t readFromSlave(uint8_t * rxBuffer) {
  if (!readflag) {
    return 0;
  }
  readflag = false;


  uint8_t raedableByte = queryRxStatus();
  uint8_t bufferc[4096];
  sendRxSpiRequest(rxBuffer, raedableByte);
  for (int i = 0; i < raedableByte; i++) {
    rxBuffer[i] = rxBuffer[3 + i];
  }
  rxDone();
  return raedableByte;
}
