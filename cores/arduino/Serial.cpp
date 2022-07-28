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

static tx_buffer_index_t _tx_buffer_head[10];
static tx_buffer_index_t _tx_buffer_tail[10];
static rx_buffer_index_t _rx_buffer_tail[10];

static unsigned char *_rx_buffer[10];
static unsigned char *_tx_buffer[10];

static bool _sending[10];


UART::UART(
  sci_uart_instance_ctrl_t *uart_ctrl,
  const uart_cfg_t* uart_config,
  dtc_instance_ctrl_t* dtc_ctrl, int ch) :
        _uart_ctrl(uart_ctrl),
        _uart_config(uart_config),
        _dtc_ctrl(dtc_ctrl),
        _channel(ch)
{ }


UART::UART(int ch) :
  _channel(ch)
{ }


void UART::begin(unsigned long baudrate, uint16_t config) {

  _uart_ctrl = (uart_ctrl_t*)(SciTable[_channel].uart_instance->p_ctrl);
  _uart_config = (const uart_cfg_t *)(SciTable[_channel].uart_instance->p_cfg);

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

  enableUartIrqs();

  uint8_t *tx_array = new uint8_t[SERIAL_TX_BUFFER_SIZE];
  uint8_t *rx_array = new uint8_t[SERIAL_RX_BUFFER_SIZE];

  _tx_buffer[_channel] = tx_array;
  _rx_buffer[_channel] = rx_array;

  err = R_SCI_UART_BaudCalculate(baudrate, bit_mod, err_rate, &_baud);
  err = R_SCI_UART_Open (_uart_ctrl, &_config);
  if(err != FSP_SUCCESS) while(1);
  err = R_SCI_UART_BaudSet(_uart_ctrl, (void *) &_baud);
  if(err != FSP_SUCCESS) while(1);
  err = R_SCI_UART_Read(_uart_ctrl, _rx_buffer[_channel], SERIAL_RX_BUFFER_SIZE);
  if(err != FSP_SUCCESS) while(1);
  _begin = true;
}

void UART::begin(unsigned long baudrate) {
  begin(baudrate, SERIAL_8N1);
}

void UART::end() {
  R_SCI_UART_Close (_uart_ctrl);
}

void UART::setPins(int tx, int rx, int rts, int cts)
{
  setPins(digitalPinToBspPin(tx), digitalPinToBspPin(rx),
          digitalPinToBspPin(rts), digitalPinToBspPin(cts));
}

void UART::setPins(bsp_io_port_pin_t tx, bsp_io_port_pin_t rx,
                      bsp_io_port_pin_t rts, bsp_io_port_pin_t cts)
{
  uint32_t peripheralCfg = 0;
  if (_channel%2 == 0) {
    peripheralCfg = (uint32_t) IOPORT_PERIPHERAL_SCI0_2_4_6_8;
  } else {
    peripheralCfg = (uint32_t) IOPORT_PERIPHERAL_SCI1_3_5_7_9;
  }
  pinPeripheral(tx, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
  pinPeripheral(rx, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
  pinPeripheral(rts, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
  pinPeripheral(cts, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
}

int UART::available() {
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + get_rx_buffer_head() - _rx_buffer_tail[_channel])) % SERIAL_RX_BUFFER_SIZE;
}

int UART::peek() {
  if (get_rx_buffer_head() == _rx_buffer_tail[_channel]) {
    return -1;
  } else {
    return _rx_buffer[_channel][_rx_buffer_tail[_channel]];
  }
}

int UART::read() {
  if (get_rx_buffer_head() == _rx_buffer_tail[_channel]) {
    return -1;
  } else {
    unsigned char c = _rx_buffer[_channel][_rx_buffer_tail[_channel]];
    _rx_buffer_tail[_channel] = (rx_buffer_index_t)(_rx_buffer_tail[_channel] + 1) % SERIAL_RX_BUFFER_SIZE;
    return c;
  }
}

void UART::flush() {
  while (_tx_buffer_head[_channel] != _tx_buffer_tail[_channel]){};
  while (_sending[_channel]){};
}

size_t UART::write(uint8_t c) {
  tx_buffer_index_t i = (tx_buffer_index_t)((_tx_buffer_head[_channel] + 1) % SERIAL_TX_BUFFER_SIZE);
  if (_begin) {
    while (i == _tx_buffer_tail[_channel]){};
    _tx_buffer[_channel][_tx_buffer_head[_channel]] = c;
    _tx_buffer_head[_channel] = i;

    if (!_sending[_channel]) {
      _sending[_channel] = true;
      _tx_udr_empty_irq();
    }
  } else {
    if (i != _tx_buffer_tail[_channel]) {
      _tx_buffer[_channel][_tx_buffer_head[_channel]] = c;
      _tx_buffer_head[_channel] = i;
    }
  }
  return 1;
}

UART::operator bool() {
	return true;
}

void UART::_tx_udr_empty_irq(void)
{
  if (_tx_buffer_head[_channel] != _tx_buffer_tail[_channel]) {
      R_SCI_UART_Write (_uart_ctrl, &_tx_buffer[_channel][_tx_buffer_tail[_channel]], 1);
    _tx_buffer_tail[_channel] = (tx_buffer_index_t)((_tx_buffer_tail[_channel] + 1) % SERIAL_TX_BUFFER_SIZE);
  } else {
    _sending[_channel] = false;
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


void UART::enableUartIrqs() {

  switch (_channel)
  {
  case 0:
    __NVIC_SetVector(SCI0_RXI_IRQn, (uint32_t)sci_uart_rxi_isr);
    __NVIC_SetVector(SCI0_TXI_IRQn, (uint32_t)sci_uart_txi_isr);
    __NVIC_SetVector(SCI0_TEI_IRQn, (uint32_t)sci_uart_tei_isr);
    break;
  case 1:
    __NVIC_SetVector(SCI1_RXI_IRQn, (uint32_t)sci_uart_rxi_isr);
    __NVIC_SetVector(SCI1_TXI_IRQn, (uint32_t)sci_uart_txi_isr);
    __NVIC_SetVector(SCI1_TEI_IRQn, (uint32_t)sci_uart_tei_isr);
    break;
  case 2:
    __NVIC_SetVector(SCI2_RXI_IRQn, (uint32_t)sci_uart_rxi_isr);
    __NVIC_SetVector(SCI2_TXI_IRQn, (uint32_t)sci_uart_txi_isr);
    __NVIC_SetVector(SCI2_TEI_IRQn, (uint32_t)sci_uart_tei_isr);
    break;
  case 3:
    __NVIC_SetVector(SCI3_RXI_IRQn, (uint32_t)sci_uart_rxi_isr);
    __NVIC_SetVector(SCI3_TXI_IRQn, (uint32_t)sci_uart_txi_isr);
    __NVIC_SetVector(SCI3_TEI_IRQn, (uint32_t)sci_uart_tei_isr);
    break;
  case 4:
    __NVIC_SetVector(SCI4_RXI_IRQn, (uint32_t)sci_uart_rxi_isr);
    __NVIC_SetVector(SCI4_TXI_IRQn, (uint32_t)sci_uart_txi_isr);
    __NVIC_SetVector(SCI4_TEI_IRQn, (uint32_t)sci_uart_tei_isr);
    break;
  case 5:
    __NVIC_SetVector(SCI5_RXI_IRQn, (uint32_t)sci_uart_rxi_isr);
    __NVIC_SetVector(SCI5_TXI_IRQn, (uint32_t)sci_uart_txi_isr);
    __NVIC_SetVector(SCI5_TEI_IRQn, (uint32_t)sci_uart_tei_isr);
    break;
  case 6:
    __NVIC_SetVector(SCI6_RXI_IRQn, (uint32_t)sci_uart_rxi_isr);
    __NVIC_SetVector(SCI6_TXI_IRQn, (uint32_t)sci_uart_txi_isr);
    __NVIC_SetVector(SCI6_TEI_IRQn, (uint32_t)sci_uart_tei_isr);
    break;
  case 7:
    __NVIC_SetVector(SCI7_RXI_IRQn, (uint32_t)sci_uart_rxi_isr);
    __NVIC_SetVector(SCI7_TXI_IRQn, (uint32_t)sci_uart_txi_isr);
    __NVIC_SetVector(SCI7_TEI_IRQn, (uint32_t)sci_uart_tei_isr);
    break;
  case 8:
    __NVIC_SetVector(SCI8_RXI_IRQn, (uint32_t)sci_uart_rxi_isr);
    __NVIC_SetVector(SCI8_TXI_IRQn, (uint32_t)sci_uart_txi_isr);
    __NVIC_SetVector(SCI8_TEI_IRQn, (uint32_t)sci_uart_tei_isr);
    break;
  case 9:
    __NVIC_SetVector(SCI9_RXI_IRQn, (uint32_t)sci_uart_rxi_isr);
    __NVIC_SetVector(SCI9_TXI_IRQn, (uint32_t)sci_uart_txi_isr);
    __NVIC_SetVector(SCI9_TEI_IRQn, (uint32_t)sci_uart_tei_isr);
    break;
  
  default:
    break;
  }

}


void uart_callback(uart_callback_args_t *p_args)
{
    uint32_t channel = p_args->channel;
    switch (p_args->event){
        case UART_EVENT_RX_COMPLETE:
        {
            R_SCI_UART_Read((uart_ctrl_t*)(SciTable[channel].uart_instance->p_ctrl), _rx_buffer[channel], SERIAL_RX_BUFFER_SIZE);
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
          if (_tx_buffer_head[channel] != _tx_buffer_tail[channel]) {
              R_SCI_UART_Write ((uart_ctrl_t*)(SciTable[channel].uart_instance->p_ctrl), &_tx_buffer[channel][_tx_buffer_tail[channel]], 1);
            _tx_buffer_tail[channel] = (tx_buffer_index_t)((_tx_buffer_tail[channel] + 1) % SERIAL_TX_BUFFER_SIZE);
          } else {
            _sending[channel] = false;
          }
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


#if SERIAL_HOWMANY > 0
UART _UART1_(&g_uart0_ctrl, &g_uart0_cfg, &g_transfer21_ctrl, 5);
#endif

#if SERIAL_HOWMANY > 1
UART _UART2_(&g_uart1_ctrl, &g_uart1_cfg, &g_transfer22_ctrl, 6);
#endif

#if SERIAL_HOWMANY > 2
UART _UART3_(&g_uart2_ctrl, &g_uart2_cfg, &g_transfer23_ctrl, 7);
#endif

#if SERIAL_HOWMANY > 3
UART _UART4_(&g_uart3_ctrl, &g_uart3_cfg, &g_transfer24_ctrl, 8);
#endif

#if SERIAL_HOWMANY > 4
UART _UART5_(&g_uart4_ctrl, &g_uart4_cfg, &g_transfer18_ctrl, 9);
#endif