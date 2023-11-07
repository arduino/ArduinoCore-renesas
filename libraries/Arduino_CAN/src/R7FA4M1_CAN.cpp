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

#include "R7FA4M1_CAN.h"

#if defined(ARDUINO_MINIMA) || defined(ARDUINO_UNOWIFIR4)

#include <IRQManager.h>

#include "CanUtil.h"

/**************************************************************************************
 * DEFINE
 **************************************************************************************/

#define CAN_DEFAULT_MASK                    (0x1FFFFFFFU)

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

R7FA4M1_CAN::R7FA4M1_CAN(int const can_tx_pin, int const can_rx_pin)
: _can_tx_pin{can_tx_pin}
, _can_rx_pin{can_rx_pin}
, _is_error{false}
, _err_code{0}
, _can_rx_buf{}
, _can_bit_timing_cfg{}
, _can_mailbox_mask
{
  CAN_DEFAULT_MASK,
  CAN_DEFAULT_MASK,
  CAN_DEFAULT_MASK,
  CAN_DEFAULT_MASK,
  0,                /* Use no id filtering -> a CAN frame with any ID will be stored in receive mailbox group #0. */
  CAN_DEFAULT_MASK,
  0,                /* Use no id filtering -> a CAN frame with any ID will be stored in receive mailbox group #2. */
  CAN_DEFAULT_MASK
}
, _can_mailbox
{
  /* Mailbox Group #0 */
  { .mailbox_id =  0, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  1, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  2, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  3, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  /* Mailbox Group #1 */
  { .mailbox_id =  4, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  5, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  6, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  7, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  /* Mailbox Group #2 */
  { .mailbox_id =  8, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id =  9, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 10, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 11, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  /* Mailbox Group #3 */
  { .mailbox_id = 12, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 13, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 14, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  { .mailbox_id = 15, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT},
  /* We only use the very first receive mailbox for receiving. */
  /* Mailbox Group #4 */
  { .mailbox_id =  0, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id =  1, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id =  2, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id =  3, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  /* Mailbox Group #5 */
  { .mailbox_id =  4, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id =  5, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id =  6, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id =  7, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  /* Mailbox Group #6 */
  { .mailbox_id =  8, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id =  9, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 10, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 11, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  /* Mailbox Group #7 */
  { .mailbox_id = 12, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 13, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 14, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
  { .mailbox_id = 15, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE }
}
, _can_extended_cfg
{
  .clock_source   = CAN_CLOCK_SOURCE_PCLKB,
  .p_mailbox_mask = _can_mailbox_mask,
  .p_mailbox      = _can_mailbox,
  .global_id_mode = CAN_GLOBAL_ID_MODE_MIXED,
  .mailbox_count  = CAN_MAX_NO_MAILBOXES,
  .message_mode   = CAN_MESSAGE_MODE_OVERWRITE,
  .p_fifo_int_cfg = nullptr,
  .p_rx_fifo_cfg  = nullptr,
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

bool R7FA4M1_CAN::begin(CanBitRate const can_bitrate)
{
  bool init_ok = true;

  /* Configure the pins for CAN.
   */
  int const max_index = PINS_COUNT;
  init_ok &= cfg_pins(max_index, _can_tx_pin, _can_rx_pin);

  /* Configure the interrupts.
   */
  CanIrqReq_t irq_req
  {
    .ctrl = &_can_ctrl,
    .cfg = &_can_cfg,
  };
  init_ok &= IRQManager::getInstance().addPeripheral(IRQ_CAN, &irq_req);

  /* Calculate the CAN bitrate based on the value of this functions parameter.
   */
  static uint32_t const F_CAN_CLK_Hz = 24*1000*1000UL;
  static uint32_t const TQ_MIN     = 8;
  static uint32_t const TQ_MAX     = 25;
  static uint32_t const TSEG_1_MIN = 4;
  static uint32_t const TSEG_1_MAX = 16;
  static uint32_t const TSEG_2_MIN = 2;
  static uint32_t const TSEG_2_MAX = 8;

  auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2] =
    util::calc_can_bit_timing(can_bitrate, F_CAN_CLK_Hz, TQ_MIN, TQ_MAX, TSEG_1_MIN, TSEG_1_MAX, TSEG_2_MIN, TSEG_2_MAX);
  init_ok &= is_valid_baudrate;

  if (is_valid_baudrate) {
    _can_bit_timing_cfg.baud_rate_prescaler = baud_rate_prescaler;
    _can_bit_timing_cfg.time_segment_1 = time_segment_1;
    _can_bit_timing_cfg.time_segment_2 = time_segment_2;
    _can_bit_timing_cfg.synchronization_jump_width = 1;
  }

  /* Initialize the peripheral's FSP driver. */
  if (R_CAN_Open(&_can_ctrl, &_can_cfg) != FSP_SUCCESS)
    init_ok = false;

  return init_ok;
}

void R7FA4M1_CAN::end()
{
  R_CAN_Close(&_can_ctrl);
}


void R7FA4M1_CAN::setFilterMask_Standard(uint32_t const mask)
{
  _can_mailbox_mask[6] = mask;
  _can_mailbox_mask[7] = mask;
}

void R7FA4M1_CAN::setFilterMask_Extended(uint32_t const mask)
{
  _can_mailbox_mask[4] = mask;
  _can_mailbox_mask[5] = mask;
}

void R7FA4M1_CAN::setFilterId_Standard(size_t const mailbox, uint32_t const id)
{
  if (mailbox > CAN_MAX_NO_STANDARD_MAILBOXES)
    return;

  size_t const mailbox_idx = CAN_MAX_STANDARD_MAILBOX_OFFSET + mailbox;
  _can_mailbox[mailbox_idx].mailbox_id = id;
}

void R7FA4M1_CAN::setFilterId_Extended(size_t const mailbox, uint32_t const id)
{
  if (mailbox > CAN_MAX_NO_EXTENDED_MAILBOXES)
    return;

  size_t const mailbox_idx = CAN_MAX_EXTENDED_MAILBOX_OFFSET + mailbox;
  _can_mailbox[mailbox_idx].mailbox_id = id;
}

int R7FA4M1_CAN::enableInternalLoopback()
{
  if(fsp_err_t const rc = R_CAN_ModeTransition(&_can_ctrl, CAN_OPERATION_MODE_NORMAL, CAN_TEST_MODE_LOOPBACK_EXTERNAL); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

int R7FA4M1_CAN::disableInternalLoopback()
{
  if(fsp_err_t const rc = R_CAN_ModeTransition(&_can_ctrl, CAN_OPERATION_MODE_NORMAL, CAN_TEST_MODE_DISABLED); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

int R7FA4M1_CAN::write(CanMsg const & msg)
{
  bool const is_standard_id = msg.isStandardId();

  can_frame_t can_msg = {
    /* id               = */ is_standard_id ? msg.getStandardId()  : msg.getExtendedId(),
    /* id_mode          = */ is_standard_id ? CAN_ID_MODE_STANDARD : CAN_ID_MODE_EXTENDED,
    /* type             = */ CAN_FRAME_TYPE_DATA,
    /* data_length_code = */ min(msg.data_length, CAN_DATA_BUFFER_LENGTH),
    /* options          = */ 0
  };

  memcpy(can_msg.data, msg.data, can_msg.data_length_code);

  if(fsp_err_t const rc = R_CAN_Write(&_can_ctrl,
                                      is_standard_id ? CAN_MAILBOX_ID_0 : CAN_MAILBOX_ID_16,
                                      &can_msg); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

size_t R7FA4M1_CAN::available()
{
  return _can_rx_buf.available();
}

CanMsg R7FA4M1_CAN::read()
{
  return _can_rx_buf.dequeue();
}

void R7FA4M1_CAN::onCanCallback(can_callback_args_t * p_args)
{
  switch (p_args->event)
  {
    case CAN_EVENT_TX_COMPLETE: break;
    case CAN_EVENT_RX_COMPLETE: // Currently driver don't support this. This is unreachable code for now.
    {
      /* Extract the received CAN message. */
      CanMsg const msg
      (
        (p_args->frame.id_mode == CAN_ID_MODE_STANDARD) ? CanStandardId(p_args->frame.id) : CanExtendedId(p_args->frame.id),
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

bool R7FA4M1_CAN::cfg_pins(int const max_index, int const can_tx_pin, int const can_rx_pin)
{
  /* Verify if indices are good. */
  if (can_tx_pin < 0 || can_rx_pin < 0 || can_tx_pin >= max_index || can_rx_pin >= max_index) {
    return false;
  }

  /* Getting configuration from table. */
  const uint16_t * cfg = nullptr;
  auto cfg_can_tx = getPinCfgs(can_tx_pin, PIN_CFG_REQ_CAN_TX);
  auto cfg_can_rx = getPinCfgs(can_rx_pin, PIN_CFG_REQ_CAN_RX);

  /* Verify if configurations are good. */
  if (cfg_can_tx[0] == 0 || cfg_can_rx[0] == 0) {
    return false;
  }

  /* Verify if channel is the same for all pins. */
  uint32_t const ch_can_tx = GET_CHANNEL(cfg_can_tx[0]);
  uint32_t const ch_can_rx = GET_CHANNEL(cfg_can_rx[0]);
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
  R7FA4M1_CAN * this_ptr = (R7FA4M1_CAN *)(p_args->p_context);
  this_ptr->onCanCallback(p_args);
}

#endif /* defined(ARDUINO_MINIMA) || defined(ARDUINO_UNOWIFIR4) */

/**************************************************************************************
 * OBJECT INSTANTIATION
 **************************************************************************************/

#if CAN_HOWMANY > 0
arduino::R7FA4M1_CAN CAN(PIN_CAN0_TX, PIN_CAN0_RX);
#endif

#if CAN_HOWMANY > 1
arduino::R7FA4M1_CAN CAN1(PIN_CAN1_TX, PIN_CAN1_RX);
#endif
