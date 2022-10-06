/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef ARDUINO_CORE_RENESAS_CAN_LIBRARY_H_
#define ARDUINO_CORE_RENESAS_CAN_LIBRARY_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino.h>

#include "bsp_api.h"
#include "hal_data.h"

#include "CanMsgBase.h"

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoCAN
{
public:
  ArduinoCAN(int const can_tx_pin, int const can_rx_pin);


  bool begin();
  void end();

  uint8_t write(CanMsg const & msg);
  uint8_t read(CanMsg & msg);

  bool tx_complete;
  bool rx_complete;
  bool err_status;


private:
  int const _can_tx_pin;
  int const _can_rx_pin;

  can_instance_ctrl_t _can_ctrl;
  can_cfg_t _can_cfg;

  uint32_t _time_out;
  can_info_t _rx_info;
};

/**************************************************************************************
 * EXTERN DECLARATION
 **************************************************************************************/

#if CAN_HOWMANY > 0
extern ArduinoCAN CAN;
#endif

#if CAN_HOWMANY > 1
extern ArduinoCAN CAN1;
#endif

#endif /* ARDUINO_CORE_RENESAS_CAN_LIBRARY_H_ */
