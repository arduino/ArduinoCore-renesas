/*
 * Copyright (c) 2022 by Alexander Entinger <a.entinger@arduino.cc>
 * CAN library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef ARDUINO_CORE_RENESAS_CANFD_LIBRARY_H_
#define ARDUINO_CORE_RENESAS_CANFD_LIBRARY_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino.h>

#ifdef ARDUINO_PORTENTA_C33

#include "api/HardwareCAN.h"

#include  <tuple>

#include "bsp_api.h"

#include "r_canfd.h"

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

class R7FA6M5_CAN final : public HardwareCAN
{
public:
  R7FA6M5_CAN(int const can_tx_pin, int const can_rx_pin);
  virtual ~R7FA6M5_CAN() { }


  bool begin(CanBitRate const can_bitrate) override;
  void end() override;


  int enableInternalLoopback();
  int disableInternalLoopback();


  int write(CanMsg const & msg) override;
  size_t available() override;
  CanMsg read() override;


  inline bool isError(int & err_code) const { err_code = _err_code; return _is_error; }
  inline void clearError() { _is_error = false; _err_code = 0; }

  /* This function is used by the library and should NOT be called by the user. */
  void onCanFDCallback(can_callback_args_t * p_args);


private:
  int const _can_tx_pin;
  int const _can_rx_pin;
  bool _is_error;
  int _err_code;
  SyncCanMsgRingbuffer _can_rx_buf;

  canfd_instance_ctrl_t _canfd_ctrl;
  can_bit_timing_cfg_t _canfd_bit_timing_cfg;
  canfd_afl_entry_t _canfd_afl[CANFD_CFG_AFL_CH0_RULE_NUM];
  canfd_global_cfg_t _canfd_global_cfg;
  canfd_extended_cfg_t _canfd_extended_cfg;
  can_cfg_t _canfd_cfg;

  static std::tuple<bool, int> cfg_pins(int const max_index, int const can_tx_pin, int const can_rx_pin);
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

#endif /* ARDUINO_PORTENTA_C33 */

/**************************************************************************************
 * EXTERN DECLARATION
 **************************************************************************************/

#if CANFD_HOWMANY > 0
extern arduino::R7FA6M5_CAN CAN;
#endif

#if CANFD_HOWMANY > 1
extern arduino::R7FA6M5_CAN CAN1;
#endif

#endif /* ARDUINO_CORE_RENESAS_CANFD_LIBRARY_H_ */
