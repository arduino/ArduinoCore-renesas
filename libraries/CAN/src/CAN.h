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
 * TYPEDEF
 **************************************************************************************/

using CAN_open_f     = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl, can_cfg_t const * const p_cfg);
using CAN_close_f    = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl);
using CAN_write_f    = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl, uint32_t mailbox, can_frame_t * const p_frame);
using CAN_read_f     = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl, uint32_t mailbox, can_frame_t * const p_frame);
using CAN_info_get_f = fsp_err_t (*)(can_ctrl_t * const p_api_ctrl, can_info_t * const p_info);

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class ArduinoCAN
{
public:
  ArduinoCAN(int const can_tx_pin, int const can_rx_pin);


  bool begin(CanMtuSize const can_mtu_size);
  void end();

  uint8_t write(CanMsg const & msg);
  uint8_t read(CanMsg & msg);

  bool tx_complete;
  bool rx_complete;
  bool err_status;


private:
  int const _can_tx_pin;
  int const _can_rx_pin;
  CanMtuSize _can_mtu_size;

  CAN_open_f _open;
  CAN_close_f _close;
  CAN_write_f _write;
  CAN_read_f _read;
  CAN_info_get_f _info_get;

  can_instance_ctrl_t _can_ctrl;
  can_bit_timing_cfg_t _can_bit_timing_cfg;
  uint32_t _can_mailbox_mask[CAN_NUM_OF_MAILBOXES / 4];
  can_mailbox_t _can_mailbox[CAN_NUM_OF_MAILBOXES];
  can_fifo_interrupt_cfg_t _can_fifo_int_cfg;
  can_rx_fifo_cfg_t _can_rx_fifo_cfg;
  can_extended_cfg_t _can_extended_cfg;
  can_cfg_t _can_cfg;

  uint32_t _time_out;
  can_info_t _rx_info;

  static bool cfg_pins(int const max_index, int const can_tx_pin, int const can_rx_pin);
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
