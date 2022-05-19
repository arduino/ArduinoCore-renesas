#include "api/HardwareSerial.h"

class USBSerial: public HardwareSerial {
	// Arduino APIs
    void begin(unsigned long);

    void begin(unsigned long baudrate, uint16_t config) {
    }

    void end() {
    }

    int available(void) {
    }

    int peek(void) {
    }

    int read(void) {
    	size_t actual_len;
    	char c;
    	ux_device_class_cdc_acm_read(g_cdc, &c, 1, &actual_len);
    	return c;
    }

    void flush(void) {}

    size_t write(uint8_t c) {
    	return write(&c, 1);
    }

    size_t write(const uint8_t* buf, size_t size) {
    	size_t actual_len;
    	int status = ux_device_class_cdc_acm_write(g_cdc, buf, size, &actual_len);
    	if (UX_SUCCESS != status) {
    		return 0;
    	}
    	return actual_len;
    }
    using Print::write; // pull in write(str) and write(buf, size) from Print

    operator bool() {
        // call delay() to force rescheduing during while !Serial pattern
        delay(1);
        return usbx_pcdc_configured();
    }

    uint32_t baud() {
    }
    uint8_t stopbits() {
    }
    uint8_t paritytype() {
    }
    uint8_t numbits() {
    }
    bool dtr() {
    }
    bool rts() {
    }
}