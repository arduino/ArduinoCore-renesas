/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef ARDUINOCORE_RENESAS_HARDWARECAN_H
#define ARDUINOCORE_RENESAS_HARDWARECAN_H

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "CanMsg.h"

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

enum class CanBitRate : int
{
  BR_125k  =  125000,
  BR_250k  =  250000,
  BR_500k  =  500000,
  BR_1000k = 1000000,
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class HardwareCAN
{
public:
  virtual ~HardwareCAN() {}


  virtual bool begin(CanBitRate const can_bitrate) = 0;
  virtual void end() = 0;


  virtual int write(CanMsg const &msg) = 0;
  virtual size_t available() = 0;
  virtual CanMsg read() = 0;
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

#endif /* ARDUINOCORE_RENESAS_HARDWARECAN_H */
