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
#include "IRQManager.h"


#ifdef Serial
#undef Serial
#endif

static bool _sending[10];

#define MAX_UARTS    10

static UART *_uarts[MAX_UARTS];


void uart_callback(uart_callback_args_t *p_args)
{
    /* This callback function is not used but it is referenced into 
       FSP configuration so that (for the moment it is necessary to keep it) */
}

/* -------------------------------------------------------------------------- */
void UART::WrapperCallback(uart_callback_args_t *p_args) {
/* -------------------------------------------------------------------------- */  

  uint32_t channel = p_args->channel;
  
  UART *uart_prt = _uarts[channel];

  uint8_t myrx[2];

    switch (p_args->event){
        case UART_EVENT_RX_COMPLETE: // This is called when all the "expected" data are received
        case UART_EVENT_ERR_PARITY:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:
        {
            break;
        }
        case UART_EVENT_TX_COMPLETE:
        {
          if(uart_prt->get_tx_tail_index() != uart_prt->get_tx_head_index() ) {
            uart_prt->inc_tx_tail_index();
            R_SCI_UART_Write (uart_prt->get_ctrl(), uart_prt->get_next_ptr_to_tx(), 1);
          }
          else {
            uart_prt->set_tx_status(TX_STOPPED);
          }
          break;
        }
        case UART_EVENT_RX_CHAR:
        {
          if(uart_prt->get_rx_head_index() == uart_prt->get_prev_of_rx_tail()) {
            // drop receiving byte... not enough space in the buffer...
          }
          else {
            uart_prt->put_in_rx_buffer(p_args->data);
            uart_prt->inc_rx_head_index();
          }
          break;
        }
        case UART_EVENT_BREAK_DETECT:
        case UART_EVENT_TX_DATA_EMPTY:
        {
            break;
        }
    }

}

/* -------------------------------------------------------------------------- */
UART::UART(int ch) :
  tx_st(TX_STOPPED),
  channel(ch)
/* -------------------------------------------------------------------------- */
{ 
  _uarts[channel] = this;
}

/* -------------------------------------------------------------------------- */
bool UART::setUpUartIrqs(uart_cfg_t &cfg) {
/* -------------------------------------------------------------------------- */  
  bool rv = false;

  if(channel == UART1_CHANNEL) {
    rv = IRQManager::getInstance().addPeripheral(UART_SCI2,cfg);
  }
  
  return rv;
} 

/* -------------------------------------------------------------------------- */
size_t UART::write(uint8_t c) {
/* -------------------------------------------------------------------------- */  
  
  while(tx_head_index == previous(tx_tail_index, SERIAL_TX_BUFFER_SIZE)) { }
  inc(tx_head_index, SERIAL_TX_BUFFER_SIZE);
  tx_buffer[tx_head_index] = c;
  if(get_tx_status() == TX_STOPPED) {
    set_tx_status(TX_STARTED);
    inc(tx_tail_index, SERIAL_TX_BUFFER_SIZE);
    R_SCI_UART_Write (&uart_ctrl, tx_buffer + tx_tail_index, 1);
  }

  return 1;
}

/* -------------------------------------------------------------------------- */
UART::operator bool() {
/* -------------------------------------------------------------------------- */  
	return true;
}

/* -------------------------------------------------------------------------- */
void UART::begin(unsigned long baudrate, uint16_t config) {
/* -------------------------------------------------------------------------- */  
#if SERIAL_HOWMANY > 0
  if (channel == UART1_CHANNEL) {
    /* configuring PIN */
    int pin_count = 0;
    bsp_io_port_pin_t serial_pins[4];
    for (int i=0; i<PINCOUNT_fn(); i++) {
      if (g_APinDescription[i].PeripheralConfig == SERIAL_BUS) {
        serial_pins[pin_count] = g_APinDescription[i].name;
        pin_count++;
      }
      if (pin_count == 2) break;
    }
    setPins(serial_pins[0], serial_pins[1]);
  
    uart_cfg.channel = UART1_CHANNEL; 
    uart_cfg.p_context = NULL;
    uart_cfg.p_extend = &g_uart0_cfg_extend;
    #define RA_NOT_DEFINED (1)
    #if (RA_NOT_DEFINED == g_transfer4)
      uart_cfg.p_transfer_tx       = NULL;
    #else
      uart_cfg.p_transfer_tx = &g_transfer4;
    #endif
    #if (RA_NOT_DEFINED == g_transfer21)
      uart_cfg.p_transfer_rx       = NULL;
    #else
      uart_cfg.p_transfer_rx = &g_transfer21;
    #endif
    uart_cfg.p_transfer_rx       = NULL;
  }
#endif
#if SERIAL_HOWMANY > 1
  if (channel == UART2_CHANNEL) { }
#endif
#if SERIAL_HOWMANY > 2
  if (channel == UART3_CHANNEL) { }
#endif
#if SERIAL_HOWMANY > 3
  if (channel == UART4_CHANNEL) { }
#endif
#if SERIAL_HOWMANY > 4
  if (channel == UART5_CHANNEL) { }
#endif


  switch(config){
      case SERIAL_8N1:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_OFF;
          uart_cfg.stop_bits = UART_STOP_BITS_1;
          break;
      case SERIAL_8N2:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_OFF;
          uart_cfg.stop_bits = UART_STOP_BITS_2;
          break;
      case SERIAL_8E1:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_EVEN;
          uart_cfg.stop_bits = UART_STOP_BITS_1;
          break;
      case SERIAL_8E2:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_EVEN;
          uart_cfg.stop_bits = UART_STOP_BITS_2;
          break;
      case SERIAL_8O1:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_ODD;
          uart_cfg.stop_bits = UART_STOP_BITS_1;
          break;
      case SERIAL_8O2:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_ODD;
          uart_cfg.stop_bits = UART_STOP_BITS_2;
          break;
  }
  
  uart_cfg.p_callback = UART::WrapperCallback;
  setUpUartIrqs(uart_cfg);
  
  fsp_err_t err;
  const bool bit_mod = true;
  const uint32_t err_rate = 5;

  err = R_SCI_UART_BaudCalculate(baudrate, bit_mod, err_rate, &baud);
  err = R_SCI_UART_Open (&uart_ctrl, &uart_cfg);
  if(err != FSP_SUCCESS) while(1);
  err = R_SCI_UART_BaudSet(&uart_ctrl, (void *) &baud);
  if(err != FSP_SUCCESS) while(1);
  err = R_SCI_UART_Read(&uart_ctrl, rx_buffer, 0);
  if(err != FSP_SUCCESS) while(1);
  _begin = true;
}

/* -------------------------------------------------------------------------- */
void UART::begin(unsigned long baudrate) {
/* -------------------------------------------------------------------------- */  
  begin(baudrate, SERIAL_8N1);
}

/* -------------------------------------------------------------------------- */
void UART::end() {
/* -------------------------------------------------------------------------- */  
  R_SCI_UART_Close (&uart_cfg);
}



void UART::setPins(int tx, int rx, int rts, int cts)
{
  setPins(digitalPinToBspPin(tx), digitalPinToBspPin(rx),
          digitalPinToBspPin(rts), digitalPinToBspPin(cts));
}

void UART::setPins(bsp_io_port_pin_t tx, bsp_io_port_pin_t rx,
                      bsp_io_port_pin_t rts, bsp_io_port_pin_t cts)
{
  pinPeripheral(tx, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | getPinConfig(tx));
  pinPeripheral(rx, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | getPinConfig(rx));
  if (rts != (bsp_io_port_pin_t)0) {
    pinPeripheral(rts, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | getPinConfig(rts));
    pinPeripheral(cts, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | getPinConfig(cts));
  }
}

/* -------------------------------------------------------------------------- */
int UART::available() {
/* -------------------------------------------------------------------------- */  
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + rx_head_index - rx_tail_index)) % SERIAL_RX_BUFFER_SIZE;
}

/* -------------------------------------------------------------------------- */
int UART::peek() {
/* -------------------------------------------------------------------------- */  
  if(rx_head_index == rx_tail_index) {
    return -1;
  }
  else {
    unsigned char c = rx_buffer[rx_tail_index];
    return c;
  }
}

/* -------------------------------------------------------------------------- */
int UART::read() {
/* -------------------------------------------------------------------------- */  
  if(rx_head_index == rx_tail_index) {
    return -1;
  }
  else {
    unsigned char c = rx_buffer[rx_tail_index];
    inc_rx_tail_index();
    return c;
  }
}

/* -------------------------------------------------------------------------- */
void UART::flush() {
/* -------------------------------------------------------------------------- */  
  while (tx_st == TX_STARTED){};
}