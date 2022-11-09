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

#include <IRQManager.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

/**************************************************************************************
 * PROTOTYPE DEFINITIONS
 **************************************************************************************/

extern "C" void can_callback(can_callback_args_t * p_args);

/**************************************************************************************
 * INTERNAL FUNCTIONS
 **************************************************************************************/

template <CanMtuSize CAN_MTU_SIZE> CAN_open_f            func_open();
template <CanMtuSize CAN_MTU_SIZE> CAN_close_f           func_close();
template <CanMtuSize CAN_MTU_SIZE> CAN_write_f           func_write();
template <CanMtuSize CAN_MTU_SIZE> CAN_read_f            func_read();
template <CanMtuSize CAN_MTU_SIZE> CAN_info_get_f        func_info_get();
template <CanMtuSize CAN_MTU_SIZE> CAN_mode_transition_f func_mode_transition();

#if IS_CAN_FD
template<> inline CAN_open_f            func_open           <CanMtuSize::FD>() { return R_CANFD_Open; }
template<> inline CAN_close_f           func_close          <CanMtuSize::FD>() { return R_CANFD_Close; }
template<> inline CAN_write_f           func_write          <CanMtuSize::FD>() { return R_CANFD_Write; }
template<> inline CAN_read_f            func_read           <CanMtuSize::FD>() { return R_CANFD_Read; }
template<> inline CAN_info_get_f        func_info_get       <CanMtuSize::FD>() { return R_CANFD_InfoGet; }
template<> inline CAN_mode_transition_f func_mode_transition<CanMtuSize::FD>() { return R_CANFD_ModeTransition; }
#else
template<> inline CAN_open_f            func_open           <CanMtuSize::Classic>() { return R_CAN_Open; }
template<> inline CAN_close_f           func_close          <CanMtuSize::Classic>() { return R_CAN_Close; }
template<> inline CAN_write_f           func_write          <CanMtuSize::Classic>() { return R_CAN_Write; }
template<> inline CAN_read_f            func_read           <CanMtuSize::Classic>() { return R_CAN_Read; }
template<> inline CAN_info_get_f        func_info_get       <CanMtuSize::Classic>() { return R_CAN_InfoGet; }
template<> inline CAN_mode_transition_f func_mode_transition<CanMtuSize::Classic>() { return R_CAN_ModeTransition; }
#endif

template <CanMtuSize CAN_MTU_SIZE> can_operation_mode_t func_operation_mode_normal();
#if IS_CAN_FD
template<> inline can_operation_mode_t func_operation_mode_normal<CanMtuSize::FD>() { return CAN_OPERATION_MODE_GLOBAL_OPERATION; }
#else
template<> inline can_operation_mode_t func_operation_mode_normal<CanMtuSize::Classic>() { return CAN_OPERATION_MODE_NORMAL; }
#endif

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

template <CanMtuSize CAN_MTU_SIZE>
ArduinoCAN<CAN_MTU_SIZE>::ArduinoCAN(int const can_tx_pin, int const can_rx_pin, int const can_stby_pin)
: _can_tx_pin{can_tx_pin}
, _can_rx_pin{can_rx_pin}
, _can_stby_pin{can_stby_pin}
, _is_error{false}
, _err_code{0}
, _can_rx_buf{}
, _open{nullptr}
, _close{nullptr}
, _write{nullptr}
, _read{nullptr}
, _info_get{nullptr}
, _mode_transition{nullptr}
, _can_bit_timing_cfg
{
  /* Actual bitrate: 250000 Hz. Actual Bit Time Ratio: 75 %. */
  .baud_rate_prescaler = 1 + 3 /* Division value of baud rate prescaler */,
  .time_segment_1 = 11,
  .time_segment_2 = 4,
  .synchronization_jump_width = 4
}
, _can_mailbox_mask
{
  0x1FFFFFFF,
  0x1FFFFFFF,
  0x1FFFFFFF,
  0x1FFFFFFF,
  0x1FFFFFFF,
  0x1FFFFFFF,
  0x1FFFFFFF,
  0x1FFFFFFF
}
, _can_mailbox
{
  { .mailbox_id =  0, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  1, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  2, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  3, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  4, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  5, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  6, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  7, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  8, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  9, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 10, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 11, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 12, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 13, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 14, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 15, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 16, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 17, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 18, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 19, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 20, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 21, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 22, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 23, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 24, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 25, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 26, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 27, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 28, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 29, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 30, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 31, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE }
}
, _can_rx_fifo_cfg
{
  .rx_fifo_mask1 = 0x1FFFFFFF,
  .rx_fifo_mask2 = 0x1FFFFFFF,
  .rx_fifo_id1 =
  {
      .mailbox_id              =  0,
      .id_mode                 =  CAN_ID_MODE_EXTENDED,
      .frame_type              =  CAN_FRAME_TYPE_DATA,
      .mailbox_type            =  CAN_MAILBOX_RECEIVE
  },
  .rx_fifo_id2 =
  {
      .mailbox_id              =  1,
      .id_mode                 =  CAN_ID_MODE_EXTENDED,
      .frame_type              =  CAN_FRAME_TYPE_DATA,
      .mailbox_type            =  CAN_MAILBOX_RECEIVE
  }
}
, _can_extended_cfg
{
  .clock_source   = CAN_CLOCK_SOURCE_CANMCLK,
  .p_mailbox_mask = _can_mailbox_mask,
  .p_mailbox      = _can_mailbox,
  .global_id_mode = CAN_GLOBAL_ID_MODE_EXTENDED,
  .mailbox_count  = CAN_MAX_NO_MAILBOXES,
  .message_mode   = CAN_MESSAGE_MODE_OVERWRITE,
  .p_fifo_int_cfg = nullptr,
  .p_rx_fifo_cfg  = &_can_rx_fifo_cfg,
}
, _can_cfg
{
  .channel        = 0,
  .p_bit_timing   = &_can_bit_timing_cfg,
  .p_callback     = can_callback,
  .p_context      = this,
  .p_extend       = &_can_extended_cfg,
  .ipl            = (12),
  .error_irq      = FSP_INVALID_VECTOR,
  .rx_irq         = FSP_INVALID_VECTOR,
  .tx_irq         = FSP_INVALID_VECTOR,
}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

template <CanMtuSize CAN_MTU_SIZE>
bool ArduinoCAN<CAN_MTU_SIZE>::begin(CanBitRate const /* can_bitrate */)
{
  bool init_ok = true;

  /* Configure the pins for CAN.
   */
  int const max_index = g_pin_cfg_size / sizeof(g_pin_cfg[0]);
  init_ok &= cfg_pins(max_index, _can_tx_pin, _can_rx_pin);

  /* Determine the right function pointers depending
   * on whether we have a CAN FD capable module or not.
   */
  _open            = func_open<CAN_MTU_SIZE>();
  _close           = func_close<CAN_MTU_SIZE>();
  _write           = func_write<CAN_MTU_SIZE>();
  _read            = func_read<CAN_MTU_SIZE>();
  _info_get        = func_info_get<CAN_MTU_SIZE>();
  _mode_transition = func_mode_transition<CAN_MTU_SIZE>();

  /* Perform a sanity check if valid function pointers could
   * have been assigned.
   */
  if (!_open || !_close || !_write || !_read || !_info_get) {
    init_ok = false;
  }

  /* Configure the interrupts.
   */
  CanIrqReq_t irq_req
  {
    .ctrl = &_can_ctrl,
    .cfg = &_can_cfg,
  };
  init_ok &= IRQManager::getInstance().addPeripheral(IRQ_CAN, &irq_req);

  /* Enable the CAN transceiver, if it should be needing
   * software enablement via a STBY pin.
   */
  if (_can_stby_pin >= 0)
  {
    pinMode(_can_stby_pin, OUTPUT);
    digitalWrite(_can_stby_pin, LOW);
  }

  if (_open(&_can_ctrl, &_can_cfg) != FSP_SUCCESS)
    init_ok = false;

  return init_ok;
}

template <CanMtuSize CAN_MTU_SIZE>
void ArduinoCAN<CAN_MTU_SIZE>::end()
{
  _close(&_can_ctrl);
}

template <CanMtuSize CAN_MTU_SIZE>
int ArduinoCAN<CAN_MTU_SIZE>::enableInternalLoopback()
{
  if(fsp_err_t const rc = _mode_transition(&_can_ctrl, func_operation_mode_normal<CAN_MTU_SIZE>(), CAN_TEST_MODE_LOOPBACK_EXTERNAL); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

template <CanMtuSize CAN_MTU_SIZE>
int ArduinoCAN<CAN_MTU_SIZE>::disableInternalLoopback()
{
  if(fsp_err_t const rc = _mode_transition(&_can_ctrl, func_operation_mode_normal<CAN_MTU_SIZE>(), CAN_TEST_MODE_DISABLED); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

template <CanMtuSize CAN_MTU_SIZE>
int ArduinoCAN<CAN_MTU_SIZE>::write(CanMsg const & msg)
{
  can_frame_t can_msg = {
    /* id               = */ msg.id,
    /* id_mode          = */ CAN_ID_MODE_EXTENDED,
    /* type             = */ CAN_FRAME_TYPE_DATA,
    /* data_length_code = */ min(msg.data_length, CAN_DATA_BUFFER_LENGTH),
    /* options          = */ 0
  };

  memcpy(can_msg.data, msg.data, can_msg.data_length_code);

  if(fsp_err_t const rc = _write(&_can_ctrl, CAN_MAILBOX_ID_0, &can_msg); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

template <CanMtuSize CAN_MTU_SIZE>
bool ArduinoCAN<CAN_MTU_SIZE>::available()
{
  return !_can_rx_buf.isEmpty();
}

template <CanMtuSize CAN_MTU_SIZE>
CanMsg ArduinoCAN<CAN_MTU_SIZE>::read()
{
  return _can_rx_buf.dequeue();
}

template <CanMtuSize CAN_MTU_SIZE>
void ArduinoCAN<CAN_MTU_SIZE>::onCanCallback(can_callback_args_t * p_args)
{
  switch (p_args->event)
  {
    case CAN_EVENT_TX_COMPLETE: break;
    case CAN_EVENT_RX_COMPLETE: // Currently driver don't support this. This is unreachable code for now.
    {
      /* Extract the received CAN message. */
      CanMsg const msg
      (
        p_args->frame.id,
        p_args->frame.data_length_code,
        p_args->frame.data
      );
      /* Store the received CAN message in the receive buffer. */
      _can_rx_buf.enqueue(msg);
    }
    break;
    case CAN_EVENT_ERR_WARNING:          /* error warning event */
    case CAN_EVENT_ERR_PASSIVE:          /* error passive event */
    case CAN_EVENT_ERR_BUS_OFF:          /* error bus off event */
    case CAN_EVENT_BUS_RECOVERY:         /* Bus recovery error event */
    case CAN_EVENT_MAILBOX_MESSAGE_LOST: /* overwrite/overrun error event */
    case CAN_EVENT_ERR_BUS_LOCK:         /* Bus lock detected (32 consecutive dominant bits). */
    case CAN_EVENT_ERR_CHANNEL:          /* Channel error has occurred. */
    case CAN_EVENT_TX_ABORTED:           /* Transmit abort event. */
    case CAN_EVENT_ERR_GLOBAL:           /* Global error has occurred. */
    case CAN_EVENT_TX_FIFO_EMPTY:        /* Transmit FIFO is empty. */
    {
      _is_error = true;
      _err_code = p_args->event;
    }
    break;
  }
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

template <CanMtuSize CAN_MTU_SIZE>
bool ArduinoCAN<CAN_MTU_SIZE>::cfg_pins(int const max_index, int const can_tx_pin, int const can_rx_pin)
{
  /* Verify if indices are good. */
  if (can_tx_pin < 0 || can_rx_pin < 0 || can_tx_pin >= max_index || can_rx_pin >= max_index) {
    return false;
  }

  /* Getting configuration from table. */
  const uint16_t * cfg = nullptr;
  cfg = g_pin_cfg[can_tx_pin].list;
  uint16_t cfg_can_tx = getPinCfg(cfg, PIN_CFG_REQ_CAN_TX, /* prefer_sci = */ false);
  cfg = g_pin_cfg[can_rx_pin].list;
  uint16_t cfg_can_rx = getPinCfg(cfg, PIN_CFG_REQ_CAN_RX, /* prefer_sci = */ false);

  /* Verify if configurations are good. */
  if (cfg_can_tx == 0 || cfg_can_rx == 0) {
    return false;
  }

  /* Verify if channel is the same for all pins. */
  uint32_t const ch_can_tx = GET_CHANNEL(cfg_can_tx);
  uint32_t const ch_can_rx = GET_CHANNEL(cfg_can_rx);
  if (ch_can_tx != ch_can_rx) {
    return false;
  }

  /* Actually configure pin functions. */
  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[can_tx_pin].pin, (uint32_t)(IOPORT_CFG_PERIPHERAL_PIN) | (uint32_t)(IOPORT_PERIPHERAL_CAN));
  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[can_rx_pin].pin, (uint32_t)(IOPORT_CFG_PERIPHERAL_PIN) | (uint32_t)(IOPORT_PERIPHERAL_CAN));

  return true;
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */
