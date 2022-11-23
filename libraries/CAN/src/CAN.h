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

#if CAN_HOWMANY > 0

#include "bsp_api.h"

#include "r_can.h"

#include "CanMsg.h"
#include "CanMsgRingbuffer.h"

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

enum class CanBitRate : int
{
  BR_100k  =  100000,
  BR_125k  =  125000,
  BR_250k  =  250000,
  BR_500k  =  500000,
  BR_1000k = 1000000,
};

typedef arduino::CanMsg CanMsg;

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoCAN
{
public:
  ArduinoCAN(int const can_tx_pin, int const can_rx_pin, int const can_stby_pin);


  bool begin(CanBitRate const can_bitrate);
  void end();


  int enableInternalLoopback();
  int disableInternalLoopback();


  int write(CanMsg const & msg);
  size_t available() const;
  CanMsg read();


  inline bool isError(int & err_code) const { err_code = _err_code; return _is_error; }
  inline void clearError() { _is_error = false; _err_code = 0; }

  /* This function is used by the library and should NOT be called by the user. */
  void onCanCallback(can_callback_args_t * p_args);


private:
  static size_t constexpr CAN_MAX_NO_MAILBOXES = 32U;

  int const _can_tx_pin;
  int const _can_rx_pin;
  int const _can_stby_pin;
  bool _is_error;
  int _err_code;
  CanMsgRingbuffer _can_rx_buf;

  can_instance_ctrl_t _can_ctrl;
  can_bit_timing_cfg_t _can_bit_timing_cfg;
  uint32_t _can_mailbox_mask[CAN_MAX_NO_MAILBOXES / 4];
  can_mailbox_t _can_mailbox[CAN_MAX_NO_MAILBOXES];
  can_extended_cfg_t _can_extended_cfg;
  can_cfg_t _can_cfg;

  static bool cfg_pins(int const max_index, int const can_tx_pin, int const can_rx_pin);
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

#endif /* CAN_HOWMANY > 0 */

/**************************************************************************************
 * EXTERN DECLARATION
 **************************************************************************************/

#if CAN_HOWMANY > 0
extern arduino::ArduinoCAN CAN;
#endif

#if CAN_HOWMANY > 1
extern arduino::ArduinoCAN CAN1;
#endif

#endif /* ARDUINO_CORE_RENESAS_CAN_LIBRARY_H_ */
