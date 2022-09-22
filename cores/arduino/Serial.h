/*
  Serial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#define _TIMEVAL_DEFINED
#define _SYS_SELECT_H

#include "api/RingBuffer.h"
#include "Arduino.h"
#include "api/HardwareSerial.h"

#ifdef __cplusplus

#ifndef __ARDUINO_UART_IMPLEMENTATION__
#define __ARDUINO_UART_IMPLEMENTATION__

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
// NOTE: a "power of 2" buffer size is reccomended to dramatically
//       optimize all the modulo operations for ring buffers.
// WARNING: When buffer sizes are increased to > 256, the buffer index
// variables are automatically increased in size, but the extra
// atomicity guards needed for that are not implemented. This will
// often work, but occasionally a race condition can occur that makes
// Serial behave erratically. See https://github.com/arduino/Arduino/issues/2405


#define SERIAL_TX_BUFFER_SIZE 512
#define SERIAL_RX_BUFFER_SIZE 512

#define MAX_UARTS    10

typedef enum {
  TX_STARTED,
  TX_STOPPED
} TxStatus_t;


class UART : public arduino::HardwareSerial {
  public:
    static UART *g_uarts[MAX_UARTS]; 
    static void WrapperCallback(uart_callback_args_t *p_args);
    
    UART(uint8_t _pin_tx, uint8_t _pin_rx);
    void begin(unsigned long);
    void begin(unsigned long, uint16_t config);
    void end();
    void setPins(int tx, int rx, int rts = 0, int cts = 0);
    void setPins(bsp_io_port_pin_t tx, bsp_io_port_pin_t rx,
                 bsp_io_port_pin_t rts = (bsp_io_port_pin_t)0, bsp_io_port_pin_t cts = (bsp_io_port_pin_t)0);
    int available(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(uint8_t c);
    using Print::write; 
    operator bool(); // { return true; }

  protected:
    bool                      init_ok;

  private:          
    uint8_t                   tx_pin;
    uint8_t                   rx_pin;
    bool                      cfg_pins(int max_index);
    inline void               inc(volatile int &x,int _max) { x = ++x % _max; } 
    inline int                previous(volatile int x, int _max) { return ((--x) >= 0) ? x : _max -1; }
    
    int                       channel;
    volatile TxStatus_t       tx_st;
    void                      set_tx_status(TxStatus_t st) { tx_st = st; }
    TxStatus_t                get_tx_status() { return tx_st; }
    /* rx STUFF.... */
    volatile int              rx_head_index = 0;
    volatile int              rx_tail_index = 0;
    int                       get_rx_head_index() { return rx_head_index; }
    int                       get_rx_tail_index() { return rx_tail_index; }
    void                      inc_rx_head_index() { inc(rx_head_index,SERIAL_RX_BUFFER_SIZE); }
    void                      inc_rx_tail_index() { inc(rx_tail_index,SERIAL_RX_BUFFER_SIZE); }
    int                       get_prev_of_rx_tail() { return previous(rx_tail_index,SERIAL_TX_BUFFER_SIZE); }
    void                      put_in_rx_buffer(uint8_t c) {rx_buffer[rx_head_index] = c;}
    uint8_t                   rx_buffer[SERIAL_RX_BUFFER_SIZE];
    /* tx STUFF.... */
    uint8_t                   tx_buffer[SERIAL_TX_BUFFER_SIZE];
    volatile int              tx_head_index = -1;
    volatile int              tx_tail_index = -1;
    int                       get_tx_head_index() { return tx_head_index; }
    int                       get_tx_tail_index() { return tx_tail_index; }
    void                      inc_tx_head_index() { inc(tx_head_index,SERIAL_TX_BUFFER_SIZE); }
    void                      inc_tx_tail_index() { inc(tx_tail_index,SERIAL_TX_BUFFER_SIZE); }
    int                       get_prev_of_tx_tail() { return previous(tx_tail_index,SERIAL_TX_BUFFER_SIZE); }
    void                      put_in_tx_buffer(uint8_t c) {tx_buffer[tx_head_index] = c;}
    uint8_t                   *get_next_ptr_to_tx() { return (tx_buffer + tx_tail_index); }
    sci_uart_instance_ctrl_t  uart_ctrl;
    uart_cfg_t                uart_cfg;
    baud_setting_t            uart_baud;
    sci_uart_extended_cfg_t   uart_cfg_extend;

    uart_ctrl_t*              get_ctrl() { return &uart_ctrl; }
    
    bool                      setUpUartIrqs(uart_cfg_t &cfg);
};

    
   
    
    
    


extern UART _UART1_;
extern UART _UART2_;
extern UART _UART3_;
extern UART _UART4_;
extern UART _UART5_;

#endif
#endif
