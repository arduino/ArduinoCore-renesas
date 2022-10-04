
#include "SerialUSB.h"
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

void _SerialUSB::begin(unsigned long baud) {
    (void) baud; //ignored

    if (_running) {
        return;
    }

    _running = true;
}

void _SerialUSB::end() {
    // TODO
}

int _SerialUSB::peek() {
    if (!_running) {
        return 0;
    }

    uint8_t c;
    return tud_cdc_peek(&c) ? (int) c : -1;
}

int _SerialUSB::read() {
    if (!_running) {
        return -1;
    }

    if (tud_cdc_connected() && tud_cdc_available()) {
        return tud_cdc_read_char();
    }
    return -1;
}

int _SerialUSB::available() {
    if (!_running) {
        return 0;
    }

    return tud_cdc_available();
}

int _SerialUSB::availableForWrite() {
    if (!_running) {
        return 0;
    }

    return tud_cdc_write_available();
}

void _SerialUSB::flush() {
    if (!_running) {
        return;
    }

    tud_cdc_write_flush();
}

size_t _SerialUSB::write(uint8_t c) {
    return write(&c, 1);
}

size_t _SerialUSB::write(const uint8_t *buf, size_t length) {
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
                last_avail_time = millis();
            } else {
                tud_task();
                tud_cdc_write_flush();
                if (!tud_cdc_connected() ||
                        (!tud_cdc_write_available() && millis() > last_avail_time + 1000 /* 1 second */)) {
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

_SerialUSB::operator bool() {
    if (!_running) {
        return false;
    }

    tud_task();
    return tud_cdc_connected();
}

/* Key code for writing PRCR register. */
#define BSP_PRV_PRCR_KEY	              (0xA500U)
#define BSP_PRV_PRCR_PRC1_UNLOCK          ((BSP_PRV_PRCR_KEY) | 0x2U)
#define BSP_PRV_PRCR_LOCK	              ((BSP_PRV_PRCR_KEY) | 0x0U)

#define BOOT_DOUBLE_TAP_DATA              (*((volatile uint32_t *) &R_SYSTEM->VBTBKR[0]))
#define DOUBLE_TAP_MAGIC                  0x07738135

int _SerialUSB::_bps, _SerialUSB::_bits, _SerialUSB::_parity, _SerialUSB::_stop;
bool _SerialUSB::_dtr, _SerialUSB::_rts;

static void CheckSerialReset() {
    #if NO_1200_BPS_RESET
        return;
    #endif

    if ((_SerialUSB::_bps == 1200) && (! _SerialUSB::_dtr)) {
        R_SYSTEM->PRCR = (uint16_t) BSP_PRV_PRCR_PRC1_UNLOCK;
        BOOT_DOUBLE_TAP_DATA = DOUBLE_TAP_MAGIC;
        R_SYSTEM->PRCR = (uint16_t) BSP_PRV_PRCR_LOCK;
        ((R_USB_FS0_Type*)R_USB_FS0_BASE)->SYSCFG_b.DPRPU = 0;
        NVIC_SystemReset();
        while (1); // WDT will fire here
    }
}

extern "C" void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
    (void) itf;
    _SerialUSB::_dtr = dtr ? true : false;
    _SerialUSB::_rts = rts ? true : false;
    CheckSerialReset();
}

extern "C" void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* p_line_coding) {
    (void) itf;
    _SerialUSB::_bps = p_line_coding->bit_rate;
    _SerialUSB::_parity = p_line_coding->parity;
    _SerialUSB::_stop = p_line_coding->stop_bits;
    _SerialUSB::_bits = p_line_coding->data_bits;
    CheckSerialReset();
}

_SerialUSB SerialUSB;

void arduino::serialEventRun(void) {
    if (serialEvent && SerialUSB.available()) {
        serialEvent();
    }
}