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

#include "Arduino.h"
#include "api/HardwareSerial.h"

#ifdef __cplusplus

#ifndef __ARDUINO_UART_IMPLEMENTATION__
#define __ARDUINO_UART_IMPLEMENTATION__

#include "r_sci_uart.h"
#include "r_uart_api.h"

#include "SafeRingBuffer.h"

#undef SERIAL_BUFFER_SIZE
#define SERIAL_BUFFER_SIZE 512

#define MAX_UARTS    10

typedef enum {
  TX_STARTED,
  TX_STOPPED
} TxStatus_t;


class UART : public arduino::HardwareSerial {
  public:
    static UART *g_uarts[MAX_UARTS]; 
    static void WrapperCallback(uart_callback_args_t *p_args);

    UART(int _pin_tx, int _pin_rx, int pin_rts = -1, int pin_cts = -1);
    void begin(unsigned long);
    void begin(unsigned long, uint16_t config);
    void end();
    int available(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(uint8_t c);
    size_t write(uint8_t* c, size_t len);
    size_t write_raw(uint8_t* c, size_t len);
    using Print::write; 
    operator bool(); // { return true; }

  private:          
    int                       tx_pin;
    int                       rx_pin;
    int                       rts_pin = -1;
    int                       cts_pin = -1;
    bool                      cfg_pins(int max_index);

    int                       channel;
    arduino::SafeRingBufferN<SERIAL_BUFFER_SIZE> rxBuffer;
    arduino::SafeRingBufferN<SERIAL_BUFFER_SIZE> txBuffer;

    volatile bool tx_empty;
    volatile bool tx_complete;

    sci_uart_instance_ctrl_t  uart_ctrl;
    uart_cfg_t                uart_cfg;
    baud_setting_t            uart_baud;
    sci_uart_extended_cfg_t   uart_cfg_extend;

    uart_ctrl_t*              get_ctrl() { return &uart_ctrl; }
    
    bool                      setUpUartIrqs(uart_cfg_t &cfg);

  protected:
    bool                      init_ok;
};

    
   
    
    
    


extern UART _UART1_;
extern UART _UART2_;
extern UART _UART3_;
extern UART _UART4_;
extern UART _UART5_;

#endif
#endif
