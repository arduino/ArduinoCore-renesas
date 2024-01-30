#include "I2S.h"

int16_t buffer[1024];

void setup() {
    Serial.begin(2000000);
    I2S.begin();
}

void loop() {
    while (!I2S.available()) {}
    auto howMany = I2S.read(buffer);
    Serial.write((uint8_t*)buffer, howMany);
}
