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

#include "CANFD.h"

#if CANFD_HOWMANY > 0

#include <IRQManager.h>

/**************************************************************************************
 * PROTOTYPE DEFINITIONS
 **************************************************************************************/

extern "C" void can_callback(can_callback_args_t * p_args);

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

ArduinoCANFD::ArduinoCANFD(int const can_tx_pin, int const can_rx_pin, int const can_stby_pin)
: _can_tx_pin{can_tx_pin}
, _can_rx_pin{can_rx_pin}
, _can_stby_pin{can_stby_pin}
, _is_error{false}
, _err_code{0}
, _can_rx_buf{}
, _can_bit_timing_cfg
{
  /* Actual bitrate: 250000 Hz. Actual Bit Time Ratio: 75 %. */
  .baud_rate_prescaler = 1 + 3 /* Division value of baud rate prescaler */,
  .time_segment_1 = 11,
  .time_segment_2 = 4,
  .synchronization_jump_width = 4
}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

bool ArduinoCANFD::begin(CanBitRate const /* can_bitrate */)
{
  bool init_ok = true;

  /* Configure the pins for CAN.
   */
  int const max_index = g_pin_cfg_size / sizeof(g_pin_cfg[0]);
  init_ok &= cfg_pins(max_index, _can_tx_pin, _can_rx_pin);

  /* Configure the interrupts.
   */
  CanFdIrqReq_t irq_req
  {
    .ctrl = &_can_ctrl,
    .cfg = &_can_cfg,
  };
  init_ok &= IRQManager::getInstance().addPeripheral(IRQ_CANFD, &irq_req);

  /* Enable the CAN transceiver, if it should be needing
   * software enablement via a STBY pin.
   */
  if (_can_stby_pin >= 0)
  {
    pinMode(_can_stby_pin, OUTPUT);
    digitalWrite(_can_stby_pin, LOW);
  }

  if (R_CANFD_Open(&_can_ctrl, &_can_cfg) != FSP_SUCCESS)
    init_ok = false;

  return init_ok;
}

void ArduinoCANFD::end()
{
  R_CANFD_Close(&_can_ctrl);
}

int ArduinoCANFD::enableInternalLoopback()
{
  if(fsp_err_t const rc = R_CANFD_ModeTransition(&_can_ctrl, CAN_OPERATION_MODE_GLOBAL_OPERATION, CAN_TEST_MODE_LOOPBACK_EXTERNAL); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

int ArduinoCANFD::disableInternalLoopback()
{
  if(fsp_err_t const rc = R_CANFD_ModeTransition(&_can_ctrl, CAN_OPERATION_MODE_GLOBAL_OPERATION, CAN_TEST_MODE_DISABLED); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

int ArduinoCANFD::write(CanMsg const & msg)
{
  can_frame_t can_msg = {
    /* id               = */ msg.id,
    /* id_mode          = */ CAN_ID_MODE_EXTENDED,
    /* type             = */ CAN_FRAME_TYPE_DATA,
    /* data_length_code = */ min(msg.data_length, CAN_DATA_BUFFER_LENGTH),
    /* options          = */ 0
  };

  memcpy(can_msg.data, msg.data, can_msg.data_length_code);

  if(fsp_err_t const rc = R_CANFD_Write(&_can_ctrl, 0, &can_msg); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

size_t ArduinoCANFD::available() const
{
  return _can_rx_buf.available();
}

CanMsg ArduinoCANFD::read()
{
  return _can_rx_buf.dequeue();
}

void ArduinoCANFD::onCanCallback(can_callback_args_t * p_args)
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

bool ArduinoCANFD::cfg_pins(int const max_index, int const can_tx_pin, int const can_rx_pin)
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

/**************************************************************************************
 * CALLBACKS FOR FSP FRAMEWORK
 **************************************************************************************/

extern "C" void can_callback(can_callback_args_t * p_args)
{
  ArduinoCANFD * this_ptr = (ArduinoCANFD *)(p_args->p_context);
  this_ptr->onCanCallback(p_args);
}

#endif /* CANFD_HOWMANY > 0 */

/**************************************************************************************
 * OBJECT INSTANTIATION
 **************************************************************************************/

#if CANFD_HOWMANY > 0
arduino::ArduinoCANFD CAN(PIN_CAN0_TX, PIN_CAN0_RX, PIN_CAN0_STBY);
#endif

#if CANFD_HOWMANY > 1
arduino::ArduinoCANFD CAN1(PIN_CAN1_TX, PIN_CAN1_RX, PIN_CAN1_STBY);
#endif
