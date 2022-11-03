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

template <CanMtuSize CAN_MTU_SIZE>
class CanMsgBase
{
public:
  static size_t constexpr MAX_DATA_LENGTH = static_cast<size_t>(CAN_MTU_SIZE);

  CanMsgBase(uint32_t const can_id, uint8_t const can_data_len, uint8_t const * can_data_ptr)
  : id{can_id}
  , data_length{can_data_len}
  , data{0}
  {
    memcpy(data, can_data_ptr, min(can_data_len, MAX_DATA_LENGTH));
  }

  CanMsgBase() : CanMsgBase(0, 0, nullptr) { }

  uint32_t id;
  uint8_t  data_length;
  uint8_t  data[MAX_DATA_LENGTH];
};

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

#ifdef IS_CAN_FD
typedef CanMsgBase<CanMtuSize::FD> CanMsg;
#else
typedef CanMsgBase<CanMtuSize::Classic> CanMsg;
#endif

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

#endif /* ARDUINO_CORE_RENESAS_CAN_MSG_BASE_HPP_ */
