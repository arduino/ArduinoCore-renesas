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

// Weak function definitions for each type of endpoint
extern void __USBInstallSerial() __attribute__((weak));
extern void __USBInstallKeyboard() __attribute__((weak));
extern void __USBInstallMouse() __attribute__((weak));
extern void __USBInstallMIDI() __attribute__((weak));
extern void __USBInstallMSD() __attribute__((weak));

// HID report ID inquiry (report ID will vary depending on the number/type of other HID)
extern uint8_t* __USBGetHIDReport(size_t* len) __attribute__((weak));

// Called by main() to init the USB HW/SW.
void __USBStart();