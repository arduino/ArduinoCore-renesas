
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

bool _SerialUSB::connected() {
    return (tud_cdc_connected() || ignore_dtr);
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

    if (connected() && tud_cdc_available()) {
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

#include "device/usbd_pvt.h"

size_t _SerialUSB::write(const uint8_t *buf, size_t size) {
   if (!_running) {
        return 0;
    }

    if (!connected()) {
        return 0;
    }
    usbd_int_set(false);
    size_t to_send = size, so_far = 0;
    while(to_send){
        size_t space = tud_cdc_write_available();
        if(!space){
            usbd_int_set(true);
            tud_cdc_write_flush();
            continue;
        }
        if(space > to_send){
            space = to_send;
        }
        size_t sent = tud_cdc_write( buf+so_far, space);
        usbd_int_set(true);
        if(sent){
            so_far += sent;
            to_send -= sent;
            tud_cdc_write_flush();
        } else {
            size = so_far;
            break;
        }
    }
    return size;
}

_SerialUSB::operator bool() {
    static bool first_call = true;

    if (!_running) {
        return false;
    }

    tud_task();
    bool rv = connected();

    if(rv && first_call) {
        delay(10);
        first_call = false;
    }
    return rv;
}

/* Key code for writing PRCR register. */
#define BSP_PRV_PRCR_KEY	              (0xA500U)
#define BSP_PRV_PRCR_PRC1_UNLOCK          ((BSP_PRV_PRCR_KEY) | 0x2U)
#define BSP_PRV_PRCR_LOCK	              ((BSP_PRV_PRCR_KEY) | 0x0U)

#ifdef NO_BACKUP_REGISTERS
#define BOOT_DOUBLE_TAP_DATA              (*((volatile uint32_t *)0x20007FF0))
#else
#define BOOT_DOUBLE_TAP_DATA              (*((volatile uint32_t *) &R_SYSTEM->VBTBKR[0]))
#endif
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

#ifndef NO_WATCHDOG
#include "r_wdt.h"
#endif

/* This variable is used within the Arduino_FreeRTOS library
 * within the microcontroller specific code (port.c) to determine
 * whether to go to low power sleep when a reset request
 * has been triggered.
 */
bool is_watchdog_reset_in_progress_for_upload = false;

extern "C" void tud_dfu_runtime_reboot_to_dfu_cb(void)
{
    R_SYSTEM->PRCR = (uint16_t) BSP_PRV_PRCR_PRC1_UNLOCK;
    BOOT_DOUBLE_TAP_DATA = DOUBLE_TAP_MAGIC;
    R_SYSTEM->PRCR = (uint16_t) BSP_PRV_PRCR_LOCK;
#ifndef NO_WATCHDOG
    wdt_instance_ctrl_t p_ctrl; wdt_cfg_t p_cfg;
    p_cfg.timeout = WDT_TIMEOUT_16384;
    p_cfg.clock_division = WDT_CLOCK_DIVISION_256;
    p_cfg.window_start = WDT_WINDOW_START_100;
    p_cfg.window_end = WDT_WINDOW_END_0;
    p_cfg.reset_control = WDT_RESET_CONTROL_RESET;
    p_cfg.stop_control = WDT_STOP_CONTROL_ENABLE;
    int err = R_WDT_Open(&p_ctrl, &p_cfg);
    R_WDT_Refresh(&p_ctrl);
    is_watchdog_reset_in_progress_for_upload = true;
    if (err == FSP_ERR_ALREADY_OPEN) {
        // loop here since the watchdog is already being used by the application
        // (which will very likely kick it as soon as we return)
        while (1);
    }
#else
    NVIC_SystemReset();
#endif
    //while (1);
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