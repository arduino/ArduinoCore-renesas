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

#include "R7FA6M5_CAN.h"

#ifdef ARDUINO_PORTENTA_C33

#include <IRQManager.h>

#include "CanUtil.h"

/**************************************************************************************
 * PROTOTYPE DEFINITIONS
 **************************************************************************************/

extern "C" void canfd_callback(can_callback_args_t * p_args);

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

namespace arduino
{

extern "C" canfd_afl_entry_t CANFD0_AFL[CANFD_CFG_AFL_CH0_RULE_NUM];
extern "C" canfd_afl_entry_t CANFD1_AFL[CANFD_CFG_AFL_CH1_RULE_NUM];

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

R7FA6M5_CAN::R7FA6M5_CAN(int const can_tx_pin, int const can_rx_pin)
: _can_tx_pin{can_tx_pin}
, _can_rx_pin{can_rx_pin}
, _is_error{false}
, _err_code{0}
, _can_rx_buf{}
, _canfd_bit_timing_cfg{}
, _canfd_afl{}
, _canfd_global_cfg
{
  .global_interrupts = CANFD_CFG_GLOBAL_ERR_SOURCES,
  .global_config     = (CANFD_CFG_TX_PRIORITY | CANFD_CFG_DLC_CHECK | (BSP_CFG_CANFDCLK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC ? R_CANFD_CFDGCFG_DCS_Msk : 0U) | CANFD_CFG_FD_OVERFLOW),
  .rx_fifo_config    =
  {
    ((CANFD_CFG_RXFIFO0_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO0_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO0_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO0_INT_MODE) | (CANFD_CFG_RXFIFO0_ENABLE)),
    ((CANFD_CFG_RXFIFO1_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO1_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO1_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO1_INT_MODE) | (CANFD_CFG_RXFIFO1_ENABLE)),
#if !BSP_FEATURE_CANFD_LITE
    ((CANFD_CFG_RXFIFO2_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO2_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO2_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO2_INT_MODE) | (CANFD_CFG_RXFIFO2_ENABLE)),
    ((CANFD_CFG_RXFIFO3_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO3_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO3_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO3_INT_MODE) | (CANFD_CFG_RXFIFO3_ENABLE)),
    ((CANFD_CFG_RXFIFO4_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO4_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO4_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO4_INT_MODE) | (CANFD_CFG_RXFIFO4_ENABLE)),
    ((CANFD_CFG_RXFIFO5_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO5_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO5_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO5_INT_MODE) | (CANFD_CFG_RXFIFO5_ENABLE)),
    ((CANFD_CFG_RXFIFO6_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO6_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO6_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO6_INT_MODE) | (CANFD_CFG_RXFIFO6_ENABLE)),
    ((CANFD_CFG_RXFIFO7_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO7_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO7_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO7_INT_MODE) | (CANFD_CFG_RXFIFO7_ENABLE)),
#endif
  },
  .rx_mb_config      = (CANFD_CFG_RXMB_NUMBER | (CANFD_CFG_RXMB_SIZE << R_CANFD_CFDRMNB_RMPLS_Pos)),
  .global_err_ipl    = CANFD_CFG_GLOBAL_ERR_IPL,
  .rx_fifo_ipl       = CANFD_CFG_RX_FIFO_IPL,
}
, _canfd_extended_cfg
{
  .p_afl              = nullptr,
  //.txmb_txi_enable    = ((1ULL << 9) | (1ULL << 0) | 0ULL),
  .txmb_txi_enable    = 0xFFFFFFFFFFFFFFFF,
  .error_interrupts   = (R_CANFD_CFDC_CTR_EWIE_Msk | R_CANFD_CFDC_CTR_EPIE_Msk | R_CANFD_CFDC_CTR_BOEIE_Msk | R_CANFD_CFDC_CTR_BORIE_Msk | R_CANFD_CFDC_CTR_OLIE_Msk | 0U),
  .p_data_timing      = nullptr,
  .delay_compensation = (1),
  .p_global_cfg       = &_canfd_global_cfg,
}
, _canfd_cfg
{
  .channel      = 0,
  .p_bit_timing = &_canfd_bit_timing_cfg,
  .p_callback   = canfd_callback,
  .p_context    = this,
  .p_extend     = &_canfd_extended_cfg,
  .ipl          = (12),
  .error_irq    = FSP_INVALID_VECTOR,
  .rx_irq       = FSP_INVALID_VECTOR,
  .tx_irq       = FSP_INVALID_VECTOR,
}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

bool R7FA6M5_CAN::begin(CanBitRate const can_bitrate)
{
  bool init_ok = true;

  /* Configure the pins for CAN.
   */
  int const max_index = PINS_COUNT;
  auto [cfg_init_ok, cfg_channel] = cfg_pins(max_index, _can_tx_pin, _can_rx_pin);
  init_ok &= cfg_init_ok;
  _canfd_cfg.channel = cfg_channel;

  /* Set the pointer to the right filtering structure. */
  if (_canfd_cfg.channel == 0)
    _canfd_extended_cfg.p_afl = CANFD0_AFL;
  else if (_canfd_cfg.channel == 1)
    _canfd_extended_cfg.p_afl = CANFD1_AFL;
  else
    init_ok &= false;

  /* Configure the interrupts.
   */
  CanFdIrqReq_t irq_req
  {
    .ctrl = &_canfd_ctrl,
    .cfg = &_canfd_cfg,
  };
  init_ok &= IRQManager::getInstance().addPeripheral(IRQ_CANFD, &irq_req);

  /* There is only one global error channel shared between both CAN0 and CAN1
   * peripheral. If you are using e2Studio the error channel is configured via
   * a project-wide define, however this is an issue for as as we do not know
   * if both CAN instances are actually going to be used by the end-user. It may
   * happen that only CAN1.begin() is called, if in this scenario CAN0 is configured
   * as the source of the global error channel a hard-fault occurs as soon as
   * an error occurs (this is a design-issue with the FSP layer). The line below
   * ensures that there's always a valid error channel and that no hard-fault
   * can occur.
   */
  _canfd_extended_cfg.global_err_channel = cfg_channel;

  /* Calculate the CAN bitrate based on the value of this functions parameter.
   */
  static uint32_t const F_CAN_CLK_Hz = 24*1000*1000UL;
  static uint32_t const TQ_MIN     =  5;
  static uint32_t const TQ_MAX     = 49;
  static uint32_t const TSEG_1_MIN = 2;
  static uint32_t const TSEG_1_MAX = 39;
  static uint32_t const TSEG_2_MIN = 2;
  static uint32_t const TSEG_2_MAX = 10;

  auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2] =
    util::calc_can_bit_timing(can_bitrate, F_CAN_CLK_Hz, TQ_MIN, TQ_MAX, TSEG_1_MIN, TSEG_1_MAX, TSEG_2_MIN, TSEG_2_MAX);
  init_ok &= is_valid_baudrate;

  if (is_valid_baudrate) {
    _canfd_bit_timing_cfg.baud_rate_prescaler = baud_rate_prescaler;
    _canfd_bit_timing_cfg.time_segment_1 = time_segment_1;
    _canfd_bit_timing_cfg.time_segment_2 = time_segment_2;
    _canfd_bit_timing_cfg.synchronization_jump_width = 1;
  }

  /* Initialize the peripheral's FSP driver. */
  if (R_CANFD_Open(&_canfd_ctrl, &_canfd_cfg) != FSP_SUCCESS)
    init_ok = false;

  return init_ok;
}

void R7FA6M5_CAN::end()
{
  R_CANFD_Close(&_canfd_ctrl);
}

int R7FA6M5_CAN::enableInternalLoopback()
{
  if(fsp_err_t const rc = R_CANFD_ModeTransition(&_canfd_ctrl, CAN_OPERATION_MODE_GLOBAL_OPERATION, CAN_TEST_MODE_LOOPBACK_EXTERNAL); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

int R7FA6M5_CAN::disableInternalLoopback()
{
  if(fsp_err_t const rc = R_CANFD_ModeTransition(&_canfd_ctrl, CAN_OPERATION_MODE_GLOBAL_OPERATION, CAN_TEST_MODE_DISABLED); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

int R7FA6M5_CAN::write(CanMsg const & msg)
{
  bool const is_standard_id = msg.isStandardId();

  can_frame_t can_msg = {
    /* id               = */ is_standard_id ? msg.getStandardId()  : msg.getExtendedId(),
    /* id_mode          = */ is_standard_id ? CAN_ID_MODE_STANDARD : CAN_ID_MODE_EXTENDED,
    /* type             = */ CAN_FRAME_TYPE_DATA,
    /* data_length_code = */ min(msg.data_length, CAN_DATA_BUFFER_LENGTH),
    /* options          = */ 0 /* This ensures that CAN Classic is used. */
  };

  memcpy(can_msg.data, msg.data, can_msg.data_length_code);

  if(fsp_err_t const rc = R_CANFD_Write(&_canfd_ctrl, 0, &can_msg); rc != FSP_SUCCESS)
    return -rc;

  return 1;
}

size_t R7FA6M5_CAN::available()
{
  can_info_t can_info;
  if (fsp_err_t const rc = R_CANFD_InfoGet(&_canfd_ctrl, &can_info); rc != FSP_SUCCESS)
    return 0;

  if (can_info.rx_mb_status > 0)
  {
    can_frame_t frame;
    if (fsp_err_t const rc = R_CANFD_Read(&_canfd_ctrl, (can_info.rx_mb_status - 1), &frame); rc != FSP_SUCCESS)
      return 0;

    /* Extract the received CAN message. */
    CanMsg const msg
    (
      (frame.id_mode == CAN_ID_MODE_STANDARD) ? CanStandardId(frame.id) : CanExtendedId(frame.id),
      frame.data_length_code,
      frame.data
    );
    /* Store the received CAN message in the receive buffer. */
    _can_rx_buf.enqueue(msg);
  }

  return can_info.rx_mb_status;
}

CanMsg R7FA6M5_CAN::read()
{
  return _can_rx_buf.dequeue();
}

void R7FA6M5_CAN::onCanFDCallback(can_callback_args_t * p_args)
{
  switch (p_args->event)
  {
    case CAN_EVENT_TX_COMPLETE: break;
    case CAN_EVENT_RX_COMPLETE: // Currently driver don't support this. This is unreachable code for now. This is so true.
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

std::tuple<bool, int> R7FA6M5_CAN::cfg_pins(int const max_index, int const can_tx_pin, int const can_rx_pin)
{
  int channel = 0;

  /* Verify if indices are good. */
  if (can_tx_pin < 0 || can_rx_pin < 0 || can_tx_pin >= max_index || can_rx_pin >= max_index) {
    return std::make_tuple(false, channel);
  }

  /* Getting configuration from table. */
  auto cfg_can_tx = getPinCfgs(can_tx_pin, PIN_CFG_REQ_CAN_TX);
  auto cfg_can_rx = getPinCfgs(can_rx_pin, PIN_CFG_REQ_CAN_RX);

  /* Verify if configurations are good. */
  if (cfg_can_tx[0] == 0 || cfg_can_rx[0] == 0) {
    return std::make_tuple(false, channel);
  }

  /* Verify if channel is the same for all pins. */
  uint32_t const ch_can_tx = GET_CHANNEL(cfg_can_tx[0]);
  uint32_t const ch_can_rx = GET_CHANNEL(cfg_can_rx[0]);
  if (ch_can_tx != ch_can_rx) {
    return std::make_tuple(false, channel);
  }
  channel = ch_can_tx;

  /* Actually configure pin functions. */
  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[can_tx_pin].pin, (uint32_t)(IOPORT_CFG_PERIPHERAL_PIN) | (uint32_t)(IOPORT_PERIPHERAL_CAN));
  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[can_rx_pin].pin, (uint32_t)(IOPORT_CFG_PERIPHERAL_PIN) | (uint32_t)(IOPORT_PERIPHERAL_CAN));

  return std::make_tuple(true, channel);
}

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

/**************************************************************************************
 * CALLBACKS FOR FSP FRAMEWORK
 **************************************************************************************/

extern "C" void canfd_callback(can_callback_args_t * p_args)
{
  R7FA6M5_CAN * this_ptr = (R7FA6M5_CAN *)(p_args->p_context);
  this_ptr->onCanFDCallback(p_args);
}

#endif /* ARDUINO_PORTENTA_C33 */

/**************************************************************************************
 * OBJECT INSTANTIATION
 **************************************************************************************/

#if CANFD_HOWMANY > 0
arduino::R7FA6M5_CAN CAN(PIN_CAN0_TX, PIN_CAN0_RX);
#endif

#if CANFD_HOWMANY > 1
arduino::R7FA6M5_CAN CAN1(PIN_CAN1_TX, PIN_CAN1_RX);
#endif
