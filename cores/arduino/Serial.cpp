/*
  Serial.cpp - wrapper over mbed RawSerial
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2018-2019 Arduino SA

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "Arduino.h"
#include "Serial.h"

#ifdef Serial
#undef Serial
#endif


UART::UART(
  sci_uart_instance_ctrl_t uart_ctrl,
  const uart_cfg_t* uart_config,
  dtc_instance_ctrl_t* dtc_ctrl) :
        _uart_ctrl(uart_ctrl),
        _uart_config(uart_config),
        _dtc_ctrl(dtc_ctrl)
{ }


void UART::begin(unsigned long baudrate, uint16_t config) {

  fsp_err_t err;

  _config = *_uart_config;
  switch(config){
      case SERIAL_8N1:
          _config.data_bits = UART_DATA_BITS_8;
          _config.parity = UART_PARITY_OFF;
          _config.stop_bits = UART_STOP_BITS_1;
          break;
      case SERIAL_8N2:
          _config.data_bits = UART_DATA_BITS_8;
          _config.parity = UART_PARITY_OFF;
          _config.stop_bits = UART_STOP_BITS_2;
          break;
      case SERIAL_8E1:
          _config.data_bits = UART_DATA_BITS_8;
          _config.parity = UART_PARITY_EVEN;
          _config.stop_bits = UART_STOP_BITS_1;
          break;
      case SERIAL_8E2:
          _config.data_bits = UART_DATA_BITS_8;
          _config.parity = UART_PARITY_EVEN;
          _config.stop_bits = UART_STOP_BITS_2;
          break;
      case SERIAL_8O1:
          _config.data_bits = UART_DATA_BITS_8;
          _config.parity = UART_PARITY_ODD;
          _config.stop_bits = UART_STOP_BITS_1;
          break;
      case SERIAL_8O2:
          _config.data_bits = UART_DATA_BITS_8;
          _config.parity = UART_PARITY_ODD;
          _config.stop_bits = UART_STOP_BITS_2;
          break;
  }

  const bool bit_mod = true;
  const uint32_t err_rate = 5;

  err = R_SCI_UART_BaudCalculate(baudrate, bit_mod, err_rate, &_baud);
  err = R_SCI_UART_Open (&_uart_ctrl, &_config);
  if(err != FSP_SUCCESS) while(1);
  err = R_SCI_UART_BaudSet(&_uart_ctrl, (void *) &_baud);
  if(err != FSP_SUCCESS) while(1);
  err = R_SCI_UART_Read(&_uart_ctrl, _rx_buffer, SERIAL_RX_BUFFER_SIZE);
  if(err != FSP_SUCCESS) while(1);
  _begin = true;
}

void UART::begin(unsigned long baudrate) {
  begin(baudrate, SERIAL_8N1);
}

void UART::end() {
  R_SCI_UART_Close (&_uart_ctrl);
}

int UART::available() {
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + get_rx_buffer_head() - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}

int UART::peek() {
  if (get_rx_buffer_head() == _rx_buffer_tail) {
    return -1;
  } else {
    return _rx_buffer[_rx_buffer_tail];
  }
}

int UART::read() {
  if (get_rx_buffer_head() == _rx_buffer_tail) {
    return -1;
  } else {
    unsigned char c = _rx_buffer[_rx_buffer_tail];
    _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
    return c;
  }
}

void UART::flush() {
  while (_tx_buffer_head != _tx_buffer_tail){};
  while (_sending){};
}

size_t UART::write(uint8_t c) {
  tx_buffer_index_t i = (tx_buffer_index_t)((_tx_buffer_head + 1) % SERIAL_TX_BUFFER_SIZE);
  if (_begin) {
    while (i == _tx_buffer_tail){};
    _tx_buffer[_tx_buffer_head] = c;
    _tx_buffer_head = i;

    if (!_sending) {
      _sending = true;
      _tx_udr_empty_irq();
    }
  } else {
    if (i != _tx_buffer_tail) {
      _tx_buffer[_tx_buffer_head] = c;
      _tx_buffer_head = i;
    }
  }
  return 1;
}

UART::operator bool() {
	return true;
}

void UART::_tx_udr_empty_irq(void)
{
  if (_tx_buffer_head != _tx_buffer_tail) {
      R_SCI_UART_Write (&_uart_ctrl, &_tx_buffer[_tx_buffer_tail], 1);
    _tx_buffer_tail = (tx_buffer_index_t)((_tx_buffer_tail + 1) % SERIAL_TX_BUFFER_SIZE);
  } else {
    _sending = false;
  }
}


rx_buffer_index_t UART::get_rx_buffer_head()
{
  transfer_properties_t p_properties;
  fsp_err_t err;
  err = R_DTC_InfoGet(_dtc_ctrl, &p_properties);
  if(err != FSP_SUCCESS) while(1);
  return (rx_buffer_index_t)(SERIAL_RX_BUFFER_SIZE - p_properties.transfer_length_remaining);
}


#if SERIAL_HOWMANY > 0

UART _UART1_(g_uart0_ctrl, &g_uart0_cfg, &g_transfer21_ctrl);

extern "C"
void isr_uart0(uart_callback_args_t *p_args)
{
    switch (p_args->event){
        case UART_EVENT_RX_COMPLETE:
        {
            R_SCI_UART_Read(&Serial1._uart_ctrl, Serial1._rx_buffer, SERIAL_RX_BUFFER_SIZE);
            break;
        }
        case UART_EVENT_ERR_PARITY:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:
        {
            break;
        }
        case UART_EVENT_TX_COMPLETE:
        {
            Serial1._tx_udr_empty_irq();
            break;
        }
        case UART_EVENT_RX_CHAR:
        case UART_EVENT_BREAK_DETECT:
        case UART_EVENT_TX_DATA_EMPTY:
        {
            break;
        }
    }
}

#if SERIAL_HOWMANY > 1
UART _UART2_(g_uart1_ctrl, &g_uart1_cfg, &g_transfer22_ctrl);

extern "C"
void isr_uart1(uart_callback_args_t *p_args)
{
    switch (p_args->event){
        case UART_EVENT_RX_COMPLETE:
        {
            R_SCI_UART_Read(&Serial2._uart_ctrl, Serial2._rx_buffer, SERIAL_RX_BUFFER_SIZE);
            break;
        }
        case UART_EVENT_ERR_PARITY:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:
        {
            break;
        }
        case UART_EVENT_TX_COMPLETE:
        {
            Serial2._tx_udr_empty_irq();
            break;
        }
        case UART_EVENT_RX_CHAR:
        case UART_EVENT_BREAK_DETECT:
        case UART_EVENT_TX_DATA_EMPTY:
        {
            break;
        }
    }
}

#if SERIAL_HOWMANY > 2
UART _UART3_(g_uart2_ctrl, &g_uart2_cfg, &g_transfer23_ctrl);

extern "C"
void isr_uart2(uart_callback_args_t *p_args)
{
    switch (p_args->event){
        case UART_EVENT_RX_COMPLETE:
        {
            R_SCI_UART_Read(&Serial3._uart_ctrl, Serial3._rx_buffer, SERIAL_RX_BUFFER_SIZE);
            break;
        }
        case UART_EVENT_ERR_PARITY:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:
        {
            break;
        }
        case UART_EVENT_TX_COMPLETE:
        {
            Serial3._tx_udr_empty_irq();
            break;
        }
        case UART_EVENT_RX_CHAR:
        case UART_EVENT_BREAK_DETECT:
        case UART_EVENT_TX_DATA_EMPTY:
        {
            break;
        }
    }
}

#if SERIAL_HOWMANY > 3
UART _UART4_(g_uart3_ctrl, &g_uart3_cfg, &g_transfer24_ctrl);

extern "C"
void isr_uart3(uart_callback_args_t *p_args)
{
    switch (p_args->event){
        case UART_EVENT_RX_COMPLETE:
        {
            R_SCI_UART_Read(&Serial4._uart_ctrl, Serial4._rx_buffer, SERIAL_RX_BUFFER_SIZE);
            break;
        }
        case UART_EVENT_ERR_PARITY:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:
        {
            break;
        }
        case UART_EVENT_TX_COMPLETE:
        {
            Serial4._tx_udr_empty_irq();
            break;
        }
        case UART_EVENT_RX_CHAR:
        case UART_EVENT_BREAK_DETECT:
        case UART_EVENT_TX_DATA_EMPTY:
        {
            break;
        }
    }
}

#if SERIAL_HOWMANY > 4
UART _UART5_(g_uart4_ctrl, &g_uart4_cfg, &g_transfer18_ctrl);

extern "C"
void isr_uart4(uart_callback_args_t *p_args)
{
    switch (p_args->event){
        case UART_EVENT_RX_COMPLETE:
        {
            R_SCI_UART_Read(&Serial5._uart_ctrl, Serial5._rx_buffer, SERIAL_RX_BUFFER_SIZE);
            break;
        }
        case UART_EVENT_ERR_PARITY:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:
        {
            break;
        }
        case UART_EVENT_TX_COMPLETE:
        {
            Serial5._tx_udr_empty_irq();
            break;
        }
        case UART_EVENT_RX_CHAR:
        case UART_EVENT_BREAK_DETECT:
        case UART_EVENT_TX_DATA_EMPTY:
        {
            break;
        }
    }
}

#endif

#endif

#endif

#endif

#endif
