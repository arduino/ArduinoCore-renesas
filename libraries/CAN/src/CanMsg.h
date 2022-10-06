/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef ARDUINO_CORE_RENESAS_CAN_MSG_H_
#define ARDUINO_CORE_RENESAS_CAN_MSG_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "CanMsgBase.h"

#include <cstring>

#include <Arduino.h>

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class CanMsg : public CanMsgBase
{
public:
  static size_t constexpr MAX_DATA_LENGTH = static_cast<size_t>(CanMtuSize::Classic);

  CanMsg(uint32_t const id, uint8_t const data_length, uint8_t const * data_ptr)
  : CanMsgBase(id, data_length)
  {
    memcpy(data, data_ptr, min(data_length, MAX_DATA_LENGTH));
  }

  CanMsg() : CanMsg(0, 0, nullptr) { }

  uint8_t  data[MAX_DATA_LENGTH];
};

#endif /* ARDUINO_CORE_RENESAS_CAN_MSG_H_ */
