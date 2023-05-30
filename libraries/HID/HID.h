#ifndef __HID_H__
#define __HID_H__

#include "Arduino.h"

#define _USING_HID

class HIDSubDescriptor {
public:
  HIDSubDescriptor *next = NULL;
  HIDSubDescriptor(const void *d, const uint16_t l) : data(d), length(l) { }

  const void* data;
  const uint16_t length;
};

class HID_
{
public:
  int begin(void);
  int SendReport(uint8_t id, const void* data, int len);
  void AppendDescriptor(HIDSubDescriptor* node);
  uint8_t* GetDescriptor(size_t len);
  uint16_t descriptorSize = 0;
  uint8_t* descriptor = nullptr;

private:

  HIDSubDescriptor* rootNode;

  uint8_t protocol;
  uint8_t idle;
};

// Replacement for global singleton.
// This function prevents static-initialization-order-fiasco
// https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
HID_& HID();

#ifdef ARDUINO_UNOWIFIR4
#undef Serial
#define Serial SerialUSB
#endif

#endif //__HID_H__