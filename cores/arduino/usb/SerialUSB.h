#ifdef AZURE_RTOS_THREADX
#define _TIMEVAL_DEFINED
#define _SYS_SELECT_H
#include "main_thd.h"
#include "ux_api.h"
#include "ux_device_class_cdc_acm.h"
#include "usbx_pcdc_acm_ep.h"
#include "api/HardwareSerial.h"
#include "api/RingBuffer.h"

extern UX_SLAVE_CLASS_CDC_ACM * g_cdc;

extern "C" bool usbx_pcdc_configured();
extern "C" TX_EVENT_FLAGS_GROUP g_cdcacm_event_flags0;

class USBSerial: public arduino::HardwareSerial {

public:
	// Arduino APIs

    void begin(unsigned long) {}
    void begin(unsigned long baudrate, uint16_t config) {}
    void end() {}

    void onRx() {
    	uint8_t buf[512];
    	ULONG actual_len;
    	ux_device_class_cdc_acm_read(g_cdc, buf, sizeof(buf), &actual_len);
    	for (ULONG i = 0; i < actual_len; i++) {
    		rx_buffer.store_char(buf[i]);
    	}
    }

    int available(void) {
    	onRx();
    	return rx_buffer.available();
    }

    int peek(void) {
    	onRx();
    	return rx_buffer.peek();
    }

    int read(void) {
    	onRx();
    	return rx_buffer.read_char();
    }

    void flush(void) {}

    size_t write(uint8_t c) {
    	return write(&c, 1);
    }

    size_t write(const uint8_t* buf, size_t size) {
    	ULONG actual_len;
    	ULONG actual_flags = 0;
    	tx_event_flags_get(&g_cdcacm_event_flags0, 1, TX_OR, &actual_flags, TX_WAIT_FOREVER);
    	if (!(actual_flags & 1)) {
    		return 0;
    	}
    	int status = ux_device_class_cdc_acm_write(g_cdc, (UCHAR*)buf, size, &actual_len);
    	if (UX_SUCCESS != status) {
    		return 0;
    	}
    	return actual_len;
    }
    using Print::write; // pull in write(str) and write(buf, size) from Print

    operator bool() {
        // call delay() to force rescheduing during while !Serial pattern
        tx_thread_sleep(1);
        return usbx_pcdc_configured();
    }

    uint32_t baud() {
    	UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER line_coding;
    	ux_device_class_cdc_acm_ioctl(g_cdc, UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_CODING, &line_coding);
    	return line_coding.ux_slave_class_cdc_acm_parameter_baudrate;
    }
    uint8_t stopbits() {
    	UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER line_coding;
    	ux_device_class_cdc_acm_ioctl(g_cdc, UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_CODING, &line_coding);
    	return line_coding.ux_slave_class_cdc_acm_parameter_stop_bit;
    }
    uint8_t paritytype() {
    	UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER line_coding;
    	ux_device_class_cdc_acm_ioctl(g_cdc, UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_CODING, &line_coding);
    	return line_coding.ux_slave_class_cdc_acm_parameter_parity;
    }
    uint8_t numbits() {
    	UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER line_coding;
    	ux_device_class_cdc_acm_ioctl(g_cdc, UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_CODING, &line_coding);
    	return line_coding.ux_slave_class_cdc_acm_parameter_data_bit;
    }
    bool dtr() {
    	UX_SLAVE_CLASS_CDC_ACM_LINE_STATE_PARAMETER line_state;
    	ux_device_class_cdc_acm_ioctl(g_cdc, UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_STATE, &line_state);
    	return line_state.ux_slave_class_cdc_acm_parameter_dtr;
    }
    bool rts() {
    	UX_SLAVE_CLASS_CDC_ACM_LINE_STATE_PARAMETER line_state;
    	ux_device_class_cdc_acm_ioctl(g_cdc, UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_STATE, &line_state);
    	return line_state.ux_slave_class_cdc_acm_parameter_rts;
    }

private:
	arduino::RingBufferN<256> rx_buffer;

};

extern USBSerial SerialUSB;
#endif //AZURE_RTOS_THREADX