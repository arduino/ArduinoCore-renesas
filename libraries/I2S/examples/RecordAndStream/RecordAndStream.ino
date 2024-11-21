#include <I2S.h>

void setup() {
    Serial.begin(9600);
    while (!Serial) {

    }

    // Resolution, sample rate, number of samples per channel, queue depth.
    if (!I2S.begin(I2S_MODE_INOUT, 32000, 256, 8)) {
        Serial.println("Failed to start I2S");
        while (1);
    }
}

void loop() {
    if (I2S.available()) {
        SampleBuffer rxbuf = I2S.read();
        SampleBuffer txbuf = I2S.dequeue();
        for (size_t i=0; i<rxbuf.size(); i++) {
            txbuf[i] = rxbuf[i];
        }
        rxbuf.release();
        I2S.write(txbuf);
    }
}
