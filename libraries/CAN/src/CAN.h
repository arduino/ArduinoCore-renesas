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

#include "r_can.h"
#if IS_CAN_FD
# include "r_canfd.h"
#endif

#include "CanMsgBase.h"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

using CAN_open_f            = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl, can_cfg_t const * const p_cfg);
using CAN_close_f           = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl);
using CAN_write_f           = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl, uint32_t mailbox, can_frame_t * const p_frame);
using CAN_read_f            = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl, uint32_t mailbox, can_frame_t * const p_frame);
using CAN_info_get_f        = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl, can_info_t * const p_info);
using CAN_mode_transition_f = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl, can_operation_mode_t operation_mode, can_test_mode_t test_mode);

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoCAN
{
public:
  ArduinoCAN(int const can_tx_pin, int const can_rx_pin);


  bool begin(CanMtuSize const can_mtu_size);
  void end();


  int enableInternalLoopback();
  int disableInternalLoopback();


  int write(CanMsg const & msg);
  uint8_t read(CanMsg & msg);

  bool rx_complete;

  inline bool isError(int & err_code) const { err_code = _err_code; return _is_error; }
  inline void clearError() { _is_error = false; _err_code = 0; }
  /* This function is used by the library and should NOT be called by the user. */
  inline void setError(int const err_code) { _err_code = err_code; _is_error = true; }

  static size_t constexpr CAN_MAX_NO_MAILBOXES = 32U;

private:
  int const _can_tx_pin;
  int const _can_rx_pin;
  bool _is_error;
  int _err_code;
  CanMtuSize _can_mtu_size;

  CAN_open_f _open;
  CAN_close_f _close;
  CAN_write_f _write;
  CAN_read_f _read;
  CAN_info_get_f _info_get;
  CAN_mode_transition_f _mode_transition;

#if IS_CAN_FD
  canfd_instance_ctrl_t _can_ctrl;
#else
  can_instance_ctrl_t _can_ctrl;
#endif
  can_bit_timing_cfg_t _can_bit_timing_cfg;
  uint32_t _can_mailbox_mask[CAN_MAX_NO_MAILBOXES / 4];
  can_mailbox_t _can_mailbox[CAN_MAX_NO_MAILBOXES];
  can_rx_fifo_cfg_t _can_rx_fifo_cfg;
  can_extended_cfg_t _can_extended_cfg;
  can_cfg_t _can_cfg;

  uint32_t _time_out;
  can_info_t _rx_info;

  static bool cfg_pins(int const max_index, int const can_tx_pin, int const can_rx_pin);
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

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
