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

#if defined(ARDUINO_MINIMA) || defined(ARDUINO_UNOWIFIR4)

#include "api/HardwareCAN.h"

#include "bsp_api.h"

#include "r_can.h"

#include "SyncCanMsgRingbuffer.h"

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

typedef arduino::CanMsg CanMsg;

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class R7FA4M1_CAN final : public HardwareCAN
{
public:
  R7FA4M1_CAN(int const can_tx_pin, int const can_rx_pin);
  virtual ~R7FA4M1_CAN() { }


  bool begin(CanBitRate const can_bitrate) override;
  void end() override;

  void setFilterMask_Standard(uint32_t const mask);
  void setFilterMask_Extended(uint32_t const mask);
  void setFilterId_Standard(size_t const mailbox, uint32_t const id);
  void setFilterId_Extended(size_t const mailbox, uint32_t const id);

  int enableInternalLoopback();
  int disableInternalLoopback();


  int write(CanMsg const & msg) override;
  size_t available() override;
  CanMsg read() override;


  inline bool isError(int & err_code) const { err_code = _err_code; return _is_error; }
  inline void clearError() { _is_error = false; _err_code = 0; }

  /* This function is used by the library and should NOT be called by the user. */
  void onCanCallback(can_callback_args_t * p_args);

  static size_t constexpr CAN_MAX_NO_STANDARD_MAILBOXES = 8U;
  static size_t constexpr CAN_MAX_NO_EXTENDED_MAILBOXES = 8U;

private:
  static size_t constexpr CAN_MAX_NO_MAILBOXES = 32U;
  static size_t constexpr CAN_MAX_STANDARD_MAILBOX_OFFSET = 24U;
  static size_t constexpr CAN_MAX_EXTENDED_MAILBOX_OFFSET = 16U;

  int const _can_tx_pin;
  int const _can_rx_pin;
  bool _is_error;
  int _err_code;
  SyncCanMsgRingbuffer _can_rx_buf;

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

#endif /* defined(ARDUINO_MINIMA) || defined(ARDUINO_UNOWIFIR4) */

/**************************************************************************************
 * EXTERN DECLARATION
 **************************************************************************************/

#if CAN_HOWMANY > 0
extern arduino::R7FA4M1_CAN CAN;
#endif

#if CAN_HOWMANY > 1
extern arduino::R7FA4M1_CAN CAN1;
#endif

#endif /* ARDUINO_CORE_RENESAS_CAN_LIBRARY_H_ */
