
#include <Arduino.h>

#include "tusb.h"

#ifndef DISABLE_USB_SERIAL
// Ensure we are installed in the USB chain
void __USBInstallSerial() { /* noop */ }
#endif

// SerialEvent functions are weak, so when the user doesn't define them,
// the linker just sets their address to 0 (which is checked below).
// The Serialx_available is just a wrapper around Serialx.available(),
// but we can refer to it weakly so we don't pull in the entire
// HardwareSerial instance if the user doesn't also refer to it.
extern void serialEvent() __attribute__((weak));

void SerialUSB::begin(unsigned long baud) {
    (void) baud; //ignored

    if (_running) {
        return;
    }

    _running = true;
}

void SerialUSB::end() {
    // TODO
}

int SerialUSB::peek() {
    if (!_running) {
        return 0;
    }

    uint8_t c;
    return tud_cdc_peek(&c) ? (int) c : -1;
}

int SerialUSB::read() {
    if (!_running) {
        return -1;
    }

    if (tud_cdc_connected() && tud_cdc_available()) {
        return tud_cdc_read_char();
    }
    return -1;
}

int SerialUSB::available() {
    if (!_running) {
        return 0;
    }

    return tud_cdc_available();
}

int SerialUSB::availableForWrite() {
    if (!_running) {
        return 0;
    }

    return tud_cdc_write_available();
}

void SerialUSB::flush() {
    if (!_running) {
        return;
    }

    tud_cdc_write_flush();
}

size_t SerialUSB::write(uint8_t c) {
    return write(&c, 1);
}

size_t SerialUSB::write(const uint8_t *buf, size_t length) {
    if (!_running) {
        return 0;
    }

    static uint64_t last_avail_time;
    int written = 0;
    if (tud_cdc_connected()) {
        for (size_t i = 0; i < length;) {
            int n = length - i;
            int avail = tud_cdc_write_available();
            if (n > avail) {
                n = avail;
            }
            if (n) {
                int n2 = tud_cdc_write(buf + i, n);
                tud_task();
                tud_cdc_write_flush();
                i += n2;
                written += n2;
                last_avail_time = micros();
            } else {
                tud_task();
                tud_cdc_write_flush();
                if (!tud_cdc_connected() ||
                        (!tud_cdc_write_available() && micros() > last_avail_time + 1000000 /* 1 second */)) {
                    break;
                }
            }
        }
    } else {
        // reset our timeout
        last_avail_time = 0;
    }
    return written;
}

SerialUSB::operator bool() {
    if (!_running) {
        return false;
    }

    tud_task();
    return tud_cdc_connected();
}

#define BOOT_DOUBLE_TAP_ADDRESS           (0x20018f48ul)
#define BOOT_DOUBLE_TAP_DATA              (*((volatile uint32_t *) BOOT_DOUBLE_TAP_ADDRESS))
#define DOUBLE_TAP_MAGIC                  0x07738135

static bool _dtr = false;
static bool _rts = false;
static int _bps = 115200;
static void CheckSerialReset() {
    if ((_bps == 1200) && (!_dtr)) {
        BOOT_DOUBLE_TAP_DATA = DOUBLE_TAP_MAGIC;
        NVIC_SystemReset();
        while (1); // WDT will fire here
    }
}

extern "C" void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
    (void) itf;
    _dtr = dtr ? true : false;
    _rts = rts ? true : false;
    CheckSerialReset();
}

extern "C" void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* p_line_coding) {
    (void) itf;
    _bps = p_line_coding->bit_rate;
    CheckSerialReset();
}

SerialUSB Serial;

void arduino::serialEventRun(void) {
    if (serialEvent && Serial.available()) {
        serialEvent();
    }
}