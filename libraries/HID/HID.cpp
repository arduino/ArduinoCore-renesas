#include "HID.h"
#include "tusb.h"

uint8_t* __USBGetHIDReport(size_t* len) {
    *len = HID().descriptorSize;
    return HID().descriptor;
}

HID_& HID()
{
	static HID_ obj;
	return obj;
}

void HID_::AppendDescriptor(HIDSubDescriptor* node) {
    descriptorSize += node->length;
    descriptor = (uint8_t*)realloc(descriptor, descriptorSize);
    memcpy(&descriptor[descriptorSize - node->length], node->data, node->length);
}

static volatile bool _done = true;
extern "C" void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len) {
    _done = true;
}

int HID_::SendReport(uint8_t id, const void* data, int len)
{
    while (!_done);
    _done = false;
    return tud_hid_report(id, data, len);
}

#ifdef ARDUINO_UNOWIFIR4
void __maybe_start_usb() {
    __USBStart();
}
#endif