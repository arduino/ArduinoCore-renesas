/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef ARDUINO_CORE_RENESAS_CAN_MSG_BASE_HPP_
#define ARDUINO_CORE_RENESAS_CAN_MSG_BASE_HPP_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <Arduino.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class CanMsg : public Printable
{
public:
  static size_t constexpr MAX_DATA_LENGTH = 8;

  CanMsg(uint32_t const can_id, uint8_t const can_data_len, uint8_t const * can_data_ptr)
  : id{can_id}
  , data_length{can_data_len}
  , data{0}
  {
    memcpy(data, can_data_ptr, min(can_data_len, MAX_DATA_LENGTH));
  }

  CanMsg() : CanMsg(0, 0, nullptr) { }

  CanMsg(CanMsg const & other)
  {
    this->id          = other.id;
    this->data_length = other.data_length;
    memcpy(this->data, other.data, this->data_length);
  }

  virtual ~CanMsg() { }

  void operator = (CanMsg const & other)
  {
    if (this == &other)
      return;

    this->id          = other.id;
    this->data_length = other.data_length;
    memcpy(this->data, other.data, this->data_length);
  }

  virtual size_t printTo(Print & p) const override
  {
    char buf[20] = {0};
    size_t len = 0;

    /* Print the header. */
    len = snprintf(buf, sizeof(buf), "[%08X] (%d) : ", id, data_length);
    size_t n = p.write(buf, len);

    /* Print the data. */
    for (size_t d = 0; d < data_length; d++)
    {
      len = snprintf(buf, sizeof(buf), "%02X", data[d]);
      n += p.write(buf, len);
    }

    /* Wrap up. */
    return n;
  }

  uint32_t id;
  uint8_t  data_length;
  uint8_t  data[MAX_DATA_LENGTH];
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

#endif /* ARDUINO_CORE_RENESAS_CAN_MSG_BASE_HPP_ */
