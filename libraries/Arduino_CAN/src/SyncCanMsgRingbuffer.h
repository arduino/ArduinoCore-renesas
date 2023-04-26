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

#include "api/HardwareCAN.h"

#include "sync.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class SyncCanMsgRingbuffer
{
public:
  SyncCanMsgRingbuffer() : _can_msg_buf{} { }


  bool isFull() const { synchronized { _can_msg_buf.isFull(); } }
  void enqueue(CanMsg const & msg) { synchronized { _can_msg_buf.enqueue(msg); } }

  bool isEmpty() const { synchronized { return _can_msg_buf.isEmpty(); } }
  CanMsg dequeue() { synchronized { return _can_msg_buf.dequeue(); } }

  size_t available() const { synchronized { return _can_msg_buf.available(); } }

private:
  CanMsgRingbuffer _can_msg_buf;
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

#endif /* CAN_MSG_RING_BUFFER_HPP_ */
