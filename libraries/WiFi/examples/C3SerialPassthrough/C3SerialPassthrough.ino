/*
  SerialPassthroughESP32-C3 - Use esptool to flash the ES32-C3 module on Santiago and Portenta H33

  Copyright (c) 2022 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

unsigned long baud = 9600;

int rts = -1;
int dtr = -1;


#ifdef ARDUINO_PORTENTA_C33

#warning Compiling for Portenta H33
#ifndef SerialNina
#define SerialNina      Serial5
#endif

#ifndef NINA_GPIO0
#define NINA_GPIO0      (100)
#endif

#ifndef NINA_RESETN
#define NINA_RESETN     (101)
#endif

#else

#warning Compiling for Santiago

#ifndef SerialNina
#define SerialNina      Serial2
#endif

#ifndef NINA_GPIO0
#define NINA_GPIO0      (28)
#endif

#ifndef NINA_RESETN
#define NINA_RESETN     (29)
#endif

#endif

void setup() {
  Serial.begin(baud, SERIAL_8N1);
  SerialNina.begin(baud);
  while (!Serial);

  pinMode(NINA_GPIO0, OUTPUT);
  pinMode(NINA_RESETN, OUTPUT);

  digitalWrite(NINA_GPIO0, HIGH);
  delay(100);
  digitalWrite(NINA_RESETN, HIGH);
  digitalWrite(NINA_RESETN, LOW);
  digitalWrite(NINA_RESETN, HIGH);

  rts = Serial.rts();
  dtr = Serial.dtr();
}

void loop() {

  auto _rts = Serial.rts();
  auto _dtr = Serial.dtr();

  if ((rts != _rts) || (dtr != _dtr)) {
    digitalWrite(NINA_RESETN, _rts ? LOW : HIGH);
    rts = _rts;
    digitalWrite(NINA_GPIO0, _dtr ? LOW : HIGH);
    dtr = _dtr;
  }

  int len = 0;
  uint8_t auc_buffer[488];
  while (Serial.available() && len < sizeof(auc_buffer)) {
    auc_buffer[len++] = Serial.read();
  }
  if (len) {
    SerialNina.write(auc_buffer, len);
  }

  len = 0;
  while (SerialNina.available() && len < sizeof(auc_buffer)) {
    auc_buffer[len++] = SerialNina.read();
  }
  if (len) {
    Serial.write(auc_buffer, len);
  }

  // check if the USB virtual serial wants a new baud rate
  if (Serial.baud() != baud) {
    //rts = -1;
    //dtr = -1;

    baud = Serial.baud();
    SerialNina.end();
    SerialNina.begin(baud);
  }
}
