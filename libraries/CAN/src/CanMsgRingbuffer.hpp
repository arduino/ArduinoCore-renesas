/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef CAN_MSG_RING_BUFFER_HPP_
#define CAN_MSG_RING_BUFFER_HPP_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <cstdint>

#include "CanMsgBase.hpp"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

template <typename T, size_t const RING_BUFFER_SIZE>
class CanMsgRingbuffer
{
public:

  CanMsgRingbuffer();

  inline bool isFull() const { return (_num_elems == RING_BUFFER_SIZE); }
  void enqueue(T const & msg);

  inline bool isEmpty() const { return (_num_elems == 0); }
  T dequeue();


private:
  T _buf[RING_BUFFER_SIZE];
  volatile size_t _head;
  volatile size_t _tail;
  volatile size_t _num_elems;

  inline size_t next(size_t const idx) const { return ((idx + 1) % RING_BUFFER_SIZE); }
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

/**************************************************************************************
 * TEMPLATE IMPLEMENTATION
 **************************************************************************************/

#include "CanMsgRingbuffer.ipp"

#endif /* CAN_MSG_RING_BUFFER_HPP_ */
