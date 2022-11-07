/*
    Shared USB for the Raspberry Pi Pico RP2040
    Allows for multiple endpoints to share the USB controller
    Copyright (c) 2021 Earle F. Philhower, III <earlephilhower@yahoo.com>
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

#if !defined(USE_TINYUSB) && !defined(NO_USB)

#include <Arduino.h>
#include "IRQManager.h"
#include "USB.h"

#include "tusb.h"
#include "class/hid/hid_device.h"
#include "class/audio/audio.h"
#include "class/midi/midi.h"

#include "r_usb_basic.h"
#include "r_usb_basic_api.h"
#include "r_usb_pcdc_api.h"

#define USBD_DESC_LEN (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN)

#define USBD_ITF_CDC (0) // needs 2 interfaces
#define USBD_ITF_MAX (2)

#define USBD_CDC_EP_CMD (0x81)
#define USBD_CDC_EP_OUT (0x02)
#define USBD_CDC_EP_IN (0x82)
#define USBD_CDC_CMD_MAX_SIZE (8)
#if (CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE)
#define USBD_CDC_IN_OUT_MAX_SIZE (512)
#else
#define USBD_CDC_IN_OUT_MAX_SIZE (64)
#endif

#define USBD_STR_0 (0x00)
#define USBD_STR_MANUF (0x01)
#define USBD_STR_PRODUCT (0x02)
#define USBD_STR_SERIAL (0x03)
#define USBD_STR_CDC (0x04)


#define EPNUM_HID   0x83

#define EPNUM_MIDI   0x01


const uint8_t *tud_descriptor_device_cb(void) {
    static tusb_desc_device_t usbd_desc_device = {
        .bLength = sizeof(tusb_desc_device_t),
        .bDescriptorType = TUSB_DESC_DEVICE,
        .bcdUSB = 0x0200,
        .bDeviceClass = TUSB_CLASS_CDC,
        .bDeviceSubClass = MISC_SUBCLASS_COMMON,
        .bDeviceProtocol = MISC_PROTOCOL_IAD,
        .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
        .idVendor = USB_VID,
        .idProduct = USB_PID,
        .bcdDevice = 0x0100,
        .iManufacturer = USBD_STR_MANUF,
        .iProduct = USBD_STR_PRODUCT,
        .iSerialNumber = USBD_STR_SERIAL,
        .bNumConfigurations = 1
    };
    if (__USBInstallSerial && !__USBInstallKeyboard && !__USBInstallMouse && !__USBInstallMIDI) {
        // Can use as-is, this is the default USB case
        return (const uint8_t *)&usbd_desc_device;
    }
    // Need a multi-endpoint config which will require changing the PID to help Windows not barf
    if (__USBInstallKeyboard) {
        usbd_desc_device.idProduct |= 0x8000;
    }
    if (__USBInstallMouse) {
        usbd_desc_device.idProduct |= 0x4000;
    }
    if (__USBInstallMIDI) {
        usbd_desc_device.idProduct |= 0x2000;
    }
    // Set the device class to 0 to indicate multiple device classes
    usbd_desc_device.bDeviceClass = 0;
    usbd_desc_device.bDeviceSubClass = 0;
    usbd_desc_device.bDeviceProtocol = 0;
    return (const uint8_t *)&usbd_desc_device;
}

int __USBGetKeyboardReportID() {
    return 1;
}

int __USBGetMouseReportID() {
    return __USBInstallKeyboard ? 2 : 1;
}

static int      __hid_report_len = 0;
static uint8_t *__hid_report     = nullptr;

static uint8_t *GetDescHIDReport(int *len) {
    if (len) {
        *len = __hid_report_len;
    }
    return __hid_report;
}

void __SetupDescHIDReport() {
    if (__USBInstallKeyboard && __USBInstallMouse) {
        uint8_t desc_hid_report[] = {
            TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(1)),
            TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(2))
        };
        __hid_report = (uint8_t *)malloc(sizeof(desc_hid_report));
        if (__hid_report) {
            __hid_report_len = sizeof(desc_hid_report);
            memcpy(__hid_report, desc_hid_report, __hid_report_len);
        }
    } else if (__USBInstallKeyboard && ! __USBInstallMouse) {
        uint8_t desc_hid_report[] = {
            TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(1))
        };
        __hid_report = (uint8_t *)malloc(sizeof(desc_hid_report));
        if (__hid_report) {
            __hid_report_len = sizeof(desc_hid_report);
            memcpy(__hid_report, desc_hid_report, __hid_report_len);
        }
    } else if (! __USBInstallKeyboard &&  __USBInstallMouse) {
        uint8_t desc_hid_report[] = {
            TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(1))
        };
        __hid_report = (uint8_t *)malloc(sizeof(desc_hid_report));
        if (__hid_report) {
            __hid_report_len = sizeof(desc_hid_report);
            memcpy(__hid_report, desc_hid_report, __hid_report_len);
        }
    } else {
        __hid_report = nullptr;
        __hid_report_len = 0;
    }
}

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance) {
    (void) instance;
    return GetDescHIDReport(nullptr);
}

static uint8_t *usbd_desc_cfg = nullptr;
const uint8_t *tud_descriptor_configuration_cb(uint8_t index) {
    (void)index;
    return usbd_desc_cfg;
}

void __SetupUSBDescriptor() {
    if (!usbd_desc_cfg) {
        bool hasHID = __USBInstallKeyboard || __USBInstallMouse;

        uint8_t interface_count = (__USBInstallSerial ? 2 : 0) + (hasHID ? 1 : 0) + (__USBInstallMIDI ? 2 : 0);

        uint8_t cdc_desc[TUD_CDC_DESC_LEN] = {
            // Interface number, string index, protocol, report descriptor len, EP In & Out address, size & polling interval
            TUD_CDC_DESCRIPTOR(USBD_ITF_CDC, USBD_STR_CDC, USBD_CDC_EP_CMD, USBD_CDC_CMD_MAX_SIZE, USBD_CDC_EP_OUT, USBD_CDC_EP_IN, USBD_CDC_IN_OUT_MAX_SIZE)
        };

        int hid_report_len;
        GetDescHIDReport(&hid_report_len);
        uint8_t hid_itf = __USBInstallSerial ? 2 : 0;
        uint8_t hid_desc[TUD_HID_DESC_LEN] = {
            // Interface number, string index, protocol, report descriptor len, EP In & Out address, size & polling interval
            TUD_HID_DESCRIPTOR(hid_itf, 0, HID_ITF_PROTOCOL_NONE, hid_report_len, EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 10)
        };

        uint8_t midi_itf = hid_itf + (hasHID ? 1 : 0);
        uint8_t midi_desc[TUD_MIDI_DESC_LEN] = {
            // Interface number, string index, EP Out & EP In address, EP size
            TUD_MIDI_DESCRIPTOR(midi_itf, 0, EPNUM_MIDI, 0x80 | EPNUM_MIDI, 64)
        };

        int usbd_desc_len = TUD_CONFIG_DESC_LEN + (__USBInstallSerial ? sizeof(cdc_desc) : 0) + (hasHID ? sizeof(hid_desc) : 0) + (__USBInstallMIDI ? sizeof(midi_desc) : 0);

        uint8_t tud_cfg_desc[TUD_CONFIG_DESC_LEN] = {
            // Config number, interface count, string index, total length, attribute, power in mA
            TUD_CONFIG_DESCRIPTOR(1, interface_count, USBD_STR_0, usbd_desc_len, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 500)
        };

        // Combine to one descriptor
        usbd_desc_cfg = (uint8_t *)malloc(usbd_desc_len);
        if (usbd_desc_cfg) {
            memset(usbd_desc_cfg, 0, usbd_desc_len);
            uint8_t *ptr = usbd_desc_cfg;
            memcpy(ptr, tud_cfg_desc, sizeof(tud_cfg_desc));
            ptr += sizeof(tud_cfg_desc);
            if (__USBInstallSerial) {
                memcpy(ptr, cdc_desc, sizeof(cdc_desc));
                ptr += sizeof(cdc_desc);
            }
            if (hasHID) {
                memcpy(ptr, hid_desc, sizeof(hid_desc));
                ptr += sizeof(hid_desc);
            }
            if (__USBInstallMIDI) {
                memcpy(ptr, midi_desc, sizeof(midi_desc));
            }
        }
    }
}

static void utox8(uint32_t val, char* s) {
	for (int i = 0; i < 8; i++) {
		int d = val & 0XF;
		val = (val >> 4);

		s[7 - i] = d > 9 ? 'A' + d - 10 : '0' + d;
	}
}

const uint16_t *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void) langid;
#define DESC_STR_MAX (33)
    static uint16_t desc_str[DESC_STR_MAX];

    static char idString[32 * 2 + 1];

    static const char *const usbd_desc_str[] = {
        [USBD_STR_0] = "",
        [USBD_STR_MANUF] = "Arduino",
        [USBD_STR_PRODUCT] = USB_NAME,
        [USBD_STR_SERIAL] = idString,
        [USBD_STR_CDC] = "CDC Port",
    };

    if (!idString[0]) {
        const bsp_unique_id_t* t = R_BSP_UniqueIdGet();
        utox8(t->unique_id_words[0], &idString[0]);
        utox8(t->unique_id_words[1], &idString[8]);
        utox8(t->unique_id_words[2], &idString[16]);
        utox8(t->unique_id_words[3], &idString[24]);
    }

    uint8_t len;
    if (index == 0) {
        desc_str[1] = 0x0409; // supported language is English
        len = 1;
    } else {
        if (index >= sizeof(usbd_desc_str) / sizeof(usbd_desc_str[0])) {
            return NULL;
        }
        const char *str = usbd_desc_str[index];
        for (len = 0; len < DESC_STR_MAX - 1 && str[len]; ++len) {
            desc_str[1 + len] = str[len];
        }
    }

    // first byte is length (including header), second byte is string type
    desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * len + 2);

    return desc_str;
}

void __USBStart() __attribute__((weak));

/* Key code for writing PRCR register. */
#define BSP_PRV_PRCR_KEY	 (0xA500U)
#define BSP_PRV_PRCR_PRC1_UNLOCK ((BSP_PRV_PRCR_KEY) | 0x2U)
#define BSP_PRV_PRCR_LOCK	 ((BSP_PRV_PRCR_KEY) | 0x0U)

#if 0 //defined(AZURE_RTOS_THREADX)
static void tud_task_forever(ULONG thread_input) {
    while (1) {
        tud_task();
        delay(100);
    }
}
#endif

void _usbfs_interrupt_handler(void)
{
  IRQn_Type irq = R_FSP_CurrentIrqGet();
  R_BSP_IrqStatusClear(irq);

#if CFG_TUSB_RHPORT0_MODE & OPT_MODE_HOST
  tuh_int_handler(0);
#endif

#if CFG_TUSB_RHPORT0_MODE & OPT_MODE_DEVICE
  tud_int_handler(0);
#endif
  tud_task();
}

void _usbhs_interrupt_handler(void)
{
  IRQn_Type irq = R_FSP_CurrentIrqGet();
  R_BSP_IrqStatusClear(irq);

#if CFG_TUSB_RHPORT1_MODE & OPT_MODE_HOST
  tuh_int_handler(1);
#endif

#if CFG_TUSB_RHPORT1_MODE & OPT_MODE_DEVICE
  tud_int_handler(1);
#endif
  tud_task();
}

extern "C" {
    void tud_set_irq_usbfs(IRQn_Type q);
    void tud_set_irq_usbhs(IRQn_Type q);
}

void __USBStart() {
    USBIrqCfg_t usb_irq_cfg;

    if (tusb_inited()) {
        // Already called
        return;
    }

    /* Enable USB_BASE */
    R_SYSTEM->PRCR = (uint16_t) BSP_PRV_PRCR_PRC1_UNLOCK;
    R_MSTP->MSTPCRB &= ~(1U << 11U);
    R_MSTP->MSTPCRB &= ~(1U << 12U);
    R_SYSTEM->PRCR = (uint16_t) BSP_PRV_PRCR_LOCK;

#ifdef CFG_TUSB_RHPORT0_MODE
#if (CFG_TUSB_RHPORT0_MODE != 0)
    usb_irq_cfg.num_of_irqs_required = 4;
    usb_irq_cfg.address_of_handler = (uint32_t)_usbfs_interrupt_handler;
    IRQManager::getInstance().addPeripheral(IRQ_USB,(void*)&usb_irq_cfg);
    tud_set_irq_usbfs((IRQn_Type)(usb_irq_cfg.first_irq_number));
#endif
#endif

#ifdef CFG_TUSB_RHPORT1_MODE
#if (CFG_TUSB_RHPORT1_MODE != 0)
    usb_irq_cfg.num_of_irqs_required = 3;
    usb_irq_cfg.address_of_handler = (uint32_t)_usbhs_interrupt_handler;
    IRQManager::getInstance().addPeripheral(IRQ_USB_HS,(void*)&usb_irq_cfg);
    tud_set_irq_usbhs((IRQn_Type)(usb_irq_cfg.first_irq_number));
#endif
#endif

    __SetupDescHIDReport();
    __SetupUSBDescriptor();

    tud_init(BOARD_TUD_RHPORT);

#if 0 //defined(AZURE_RTOS_THREADX)
    static TX_BYTE_POOL byte_pool_0;
    static TX_THREAD thread;
    static uint8_t memory_area[1024];
    static char* pointer;
    tx_byte_pool_create(&byte_pool_0, "byte pool 0", memory_area, sizeof(memory_area));
    tx_byte_allocate(&byte_pool_0, (void**)&pointer, 512, TX_NO_WAIT);

    tx_thread_create(&thread, "tud_task", tud_task_forever, 1, pointer, 1024, 6, 6, 4, TX_AUTO_START);
#endif
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
extern "C" uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    // TODO not implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
extern "C" void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    // TODO set LED based on CAPLOCK, NUMLOCK etc...
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}

#endif