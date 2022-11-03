/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "CAN.hpp"

#include <Arduino.h>

/**************************************************************************************
 * CALLBACKS FOR FSP FRAMEWORK
 **************************************************************************************/

extern "C" void can_callback(can_callback_args_t * p_args)
{
#if IS_CAN_FD
  ArduinoCAN<CanMtuSize::FD> * this_ptr = (ArduinoCAN<CanMtuSize::FD> *)(p_args->p_context);
#else
  ArduinoCAN<CanMtuSize::Classic> * this_ptr = (ArduinoCAN<CanMtuSize::Classic> *)(p_args->p_context);
#endif
  this_ptr->onCanCallback(p_args);
}

/**************************************************************************************
 * OBJECT INSTANTIATION
 **************************************************************************************/

#if CAN_HOWMANY > 0
# if IS_CAN_FD
arduino::ArduinoCAN<CanMtuSize::FD>      CAN(PIN_CAN0_TX, PIN_CAN0_RX, PIN_CAN0_STBY);
# else
arduino::ArduinoCAN<CanMtuSize::Classic> CAN(PIN_CAN0_TX, PIN_CAN0_RX, PIN_CAN0_STBY);
# endif
#endif

#if CAN_HOWMANY > 1
# if IS_CAN_FD
arduino::ArduinoCAN<CanMtuSize::FD>      CAN1(PIN_CAN1_TX, PIN_CAN1_RX, PIN_CAN1_STBY);
# else
arduino::ArduinoCAN<CanMtuSize::Classic> CAN1(PIN_CAN1_TX, PIN_CAN1_RX, PIN_CAN1_STBY);
# endif
#endif
