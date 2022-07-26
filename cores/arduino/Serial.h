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

#include "bsp_api.h"
#include "hal_data.h"

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

#define SERIAL_TX_BUFFER_SIZE 128
#define SERIAL_RX_BUFFER_SIZE 128

typedef uint8_t tx_buffer_index_t;

typedef uint8_t rx_buffer_index_t;

#undef SERIAL_5N1
#undef SERIAL_6N1
#undef SERIAL_7N1
#undef SERIAL_8N1
#undef SERIAL_5N2
#undef SERIAL_6N2
#undef SERIAL_7N2
#undef SERIAL_8N2
#undef SERIAL_5E1
#undef SERIAL_6E1
#undef SERIAL_7E1
#undef SERIAL_8E1
#undef SERIAL_5E2
#undef SERIAL_6E2
#undef SERIAL_7E2
#undef SERIAL_8E2
#undef SERIAL_5O1
#undef SERIAL_6O1
#undef SERIAL_7O1
#undef SERIAL_8O1
#undef SERIAL_5O2
#undef SERIAL_6O2
#undef SERIAL_7O2
#undef SERIAL_8O2

// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

class UART : public arduino::HardwareSerial {
  public:
    UART(sci_uart_instance_ctrl_t *_uart_ctrl,
         const uart_cfg_t* _uart_config,
         dtc_instance_ctrl_t* _dtc_ctrl, int ch);
    UART(int ch);

    void begin(unsigned long);
    void begin(unsigned long, uint16_t config);
    void end();
    void setPins(int tx, int rx, int rts = 0, int cts = 0);
    void setPins(bsp_io_port_pin_t tx, bsp_io_port_pin_t rx,
                 bsp_io_port_pin_t rts = (bsp_io_port_pin_t)0, bsp_io_port_pin_t cts = (bsp_io_port_pin_t)0);
    int available(void);
    int peek(void);
    int read(void);
    //int availableForWrite(void);
    void flush(void);
    size_t write(uint8_t c);
    //size_t write(unsigned long n) { return write((uint8_t)n); }
    //size_t write(long n) { return write((uint8_t)n); }
    //size_t write(unsigned int n) { return write((uint8_t)n); }
    //size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool(); // { return true; }


    void _tx_udr_empty_irq(void);
    rx_buffer_index_t get_rx_buffer_head();

    uart_ctrl_t* _uart_ctrl;
    const uart_cfg_t* _uart_config;
    uart_cfg_t _config;
    baud_setting_t _baud;
    dtc_instance_ctrl_t* _dtc_ctrl;

  protected:
    bool _written;
    volatile bool _begin;

  private:
    void          enableUartIrqs();
    int           _channel;
};

extern UART _UART1_;
extern UART _UART2_;
extern UART _UART3_;
extern UART _UART4_;
extern UART _UART5_;

#endif
#endif
