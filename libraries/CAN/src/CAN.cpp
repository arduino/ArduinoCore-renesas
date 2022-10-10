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

#include "CAN.h"

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

ArduinoCAN::ArduinoCAN(int const can_tx_pin, int const can_rx_pin)
: _can_tx_pin{can_tx_pin}
, _can_rx_pin{can_rx_pin}
, _open{nullptr}
, _close{nullptr}
, _write{nullptr}
, _read{nullptr}
, _info_get{nullptr}
, tx_complete{false}
, rx_complete{false}
, err_status{false}
, _time_out{500}
, _rx_info
{
  []()
  {
    can_info_t rx_info_init;

    rx_info_init.error_code = 0;
    rx_info_init.error_count_receive = 0;
    rx_info_init.error_count_transmit = 0;
    rx_info_init.rx_fifo_status = 0;
    rx_info_init.rx_mb_status = 0;
    rx_info_init.status = 0;

    return rx_info_init;
  } ()
}
{

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

bool ArduinoCAN::begin(CanMtuSize const can_mtu_size)
{
  if (can_mtu_size == CanMtuSize::Classic)
  {
    _open     = R_CAN_Open;
    _close    = R_CAN_Close;
    _write    = R_CAN_Write;
    _read     = R_CAN_Read;
    _info_get = R_CAN_InfoGet;
  }
  else
  {
#if defined __has_include
#  if __has_include ("r_canfd.h")
    _open     = R_CANFD_Open;
    _close    = R_CANFD_Close;
    _write    = R_CANFD_Write;
    _read     = R_CANFD_Read;
    _info_get = R_CANFD_InfoGet;
#  endif
#endif
  }

//  pinMode(CAN_STDBY, OUTPUT);
//  digitalWrite(CAN_STDBY, LOW);

  if (_open(&_can_ctrl, &_can_cfg) != FSP_SUCCESS)
    return false;
  else
    return true;
}

void ArduinoCAN::end()
{
  _close(&_can_ctrl);
}

uint8_t ArduinoCAN::write(CanMsg const & msg)
{
  can_frame_t can_msg = {msg.id,
                         CAN_ID_MODE_STANDARD,
                         CAN_FRAME_TYPE_DATA,
                         msg.data_length,
                         0};

  memcpy((uint8_t*)&can_msg.data[0], (uint8_t*)&msg.data[0], msg.data_length);

  if(_write(&_can_ctrl, 0, &can_msg) != FSP_SUCCESS) {
    return 0;
  }
  /* Wait here for an event from callback */
  while ((true != tx_complete) && (_time_out--));
  if (0 == _time_out)
  {
    return 0;
  }

  /* Reset flag bit */
  tx_complete = false;
  _time_out = 500;
  return 1;
}

uint8_t ArduinoCAN::read(CanMsg & msg)
{
  uint32_t rx_status = 0;
  while(!rx_status && (_time_out--)) {
    // Get the status information for CAN transmission
    if (_info_get(&_can_ctrl, &_rx_info) != FSP_SUCCESS) {
    _time_out = 500;
      return 0;
    }
    rx_status = _rx_info.rx_mb_status;
  }
  _time_out = 500;

  if (rx_status) {
    /* Read the input frame received */
    can_frame_t can_msg;
    if (_read(&_can_ctrl, 0, &can_msg) != FSP_SUCCESS) {
      return 0;
    }
    msg.id = can_msg.id;
    msg.data_length = can_msg.data_length_code;
    memcpy((uint8_t*)&msg.data[0], (uint8_t*)&can_msg.data[0], can_msg.data_length_code);
    return 1;
  }
  return 0;
}

/**************************************************************************************
 * CALLBACKS FOR FSP FRAMEWORK
 **************************************************************************************/

void canfd0_callback(can_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        case CAN_EVENT_TX_COMPLETE:
        {
            CAN.tx_complete = true;        //set flag bit
            break;
        }
        case CAN_EVENT_RX_COMPLETE: // Currently driver don't support this. This is unreachable code for now.
        {
            CAN.rx_complete = true;
            break;
        }
        case CAN_EVENT_ERR_WARNING:             //error warning event
        case CAN_EVENT_ERR_PASSIVE:             //error passive event
        case CAN_EVENT_ERR_BUS_OFF:             //error Bus Off event
        case CAN_EVENT_BUS_RECOVERY:            //Bus recovery error event
        case CAN_EVENT_MAILBOX_MESSAGE_LOST:    //overwrite/overrun error event
        case CAN_EVENT_ERR_BUS_LOCK:            // Bus lock detected (32 consecutive dominant bits).
        case CAN_EVENT_ERR_CHANNEL:             // Channel error has occurred.
        case CAN_EVENT_TX_ABORTED:              // Transmit abort event.
        case CAN_EVENT_ERR_GLOBAL:              // Global error has occurred.
        case CAN_EVENT_TX_FIFO_EMPTY:           // Transmit FIFO is empty.
        {
            CAN.err_status = true;          //set flag bit
            break;
        }
    }
}
/*
void __attribute__((weak)) canfd1_callback(can_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        case CAN_EVENT_TX_COMPLETE:
        {
            CAN1.tx_complete = true;        //set flag bit
            break;
        }
        case CAN_EVENT_RX_COMPLETE: // Currently driver don't support this. This is unreachable code for now.
        {
            CAN1.rx_complete = true;
            break;
        }
        case CAN_EVENT_ERR_WARNING:             //error warning event
        case CAN_EVENT_ERR_PASSIVE:             //error passive event
        case CAN_EVENT_ERR_BUS_OFF:             //error Bus Off event
        case CAN_EVENT_BUS_RECOVERY:            //Bus recovery error event
        case CAN_EVENT_MAILBOX_MESSAGE_LOST:    //overwrite/overrun error event
        case CAN_EVENT_ERR_BUS_LOCK:            // Bus lock detected (32 consecutive dominant bits).
        case CAN_EVENT_ERR_CHANNEL:             // Channel error has occurred.
        case CAN_EVENT_TX_ABORTED:              // Transmit abort event.
        case CAN_EVENT_ERR_GLOBAL:              // Global error has occurred.
        case CAN_EVENT_TX_FIFO_EMPTY:           // Transmit FIFO is empty.
        {
            CAN1.err_status = true;          //set flag bit
            break;
        }
    }
}
*/
/**************************************************************************************
 * OBJECT INSTANTIATION
 **************************************************************************************/

#if CAN_HOWMANY > 0
ArduinoCAN CAN(PIN_CAN0_TX, PIN_CAN0_RX);
#endif

#if CAN_HOWMANY > 1
ArduinoCAN CAN1(PIN_CAN1_TX, PIN_CAN1_RX);
#endif
