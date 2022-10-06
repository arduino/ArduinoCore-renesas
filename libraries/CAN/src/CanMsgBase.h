/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef ARDUINO_CORE_RENESAS_CAN_MSG_BASE_H_
#define ARDUINO_CORE_RENESAS_CAN_MSG_BASE_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <cstdlib>
#include <cstdint>

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

enum class CanMtuSize : size_t
{
  Classic = 8,
  FD = 64,
};

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class CanMsgBase
{
public:
  CanMsgBase(uint32_t const id_, uint8_t const data_length_)
  : id{id_}
  , data_length{data_length_}
  { }

  uint32_t id;
  uint8_t  data_length;
};

#endif /* ARDUINO_CORE_RENESAS_CAN_MSG_BASE_H_ */
