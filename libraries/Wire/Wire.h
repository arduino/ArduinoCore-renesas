/*
  Copyright (c) 2016 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#define _TIMEVAL_DEFINED
#define _SYS_SELECT_H

#include "Arduino.h"
#include "api/HardwareI2C.h"
#include "api/Stream.h"
#include <inttypes.h>

#include "bsp_api.h"
#include "hal_data.h"

#ifdef __cplusplus

#ifndef __ARDUINO_WIRE_IMPLEMENTATION__
#define __ARDUINO_WIRE_IMPLEMENTATION__

#define BUFFER_LENGTH 256

// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

typedef void (*voidFuncPtrParamInt)(int);

class TwoWire : public arduino::HardwareI2C
{
  public:
    TwoWire(i2c_master_ctrl_t *g_i2c_master_ctrl
           ,const i2c_master_cfg_t *g_i2c_master_cfg, int ch);
    TwoWire(int sda, int scl, int ch);
    TwoWire(bsp_io_port_pin_t sda, bsp_io_port_pin_t scl, int ch);
    void begin();
    void begin(uint8_t);
    void begin(int);
    void end();
    void setClock(uint32_t);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(bool);
    size_t requestFrom(uint8_t, size_t);
    size_t requestFrom(uint8_t, size_t, bool);
	  size_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive( void (*)(int) );
    void onRequest( void (*)(void) );

    void setCallbackEvent(i2c_master_event_t cb_event);

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;

  private:

    void configureI2C(bsp_io_port_pin_t sda, bsp_io_port_pin_t scl, int ch);

    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    i2c_master_ctrl_t *_g_i2c_master_ctrl;
    const i2c_master_cfg_t *_g_i2c_master_cfg;
    i2c_master_cfg_t _i2c_config;
    dtc_instance_ctrl_t* _dtc_ctrl;

    bsp_io_port_pin_t _sda;
    bsp_io_port_pin_t _scl;
    int               _channel;

    iic_master_instance_ctrl_t _i2c_master_ctrl;
    i2c_master_cfg_t           _i2c_master_cfg;

    static uint8_t transmitting;
    static void (*user_onRequest)(void);
    static void (*user_onReceive)(int);
    static void onRequestService(void);
    static void onReceiveService(uint8_t*, int);

    uint8_t twi_readFrom(uint8_t, uint8_t*, uint8_t, uint8_t);
    uint8_t twi_writeTo(uint8_t, uint8_t*, uint8_t, uint8_t, uint8_t);
};

#if WIRE_HOWMANY > 0
extern TwoWire Wire;
#endif
#if WIRE_HOWMANY > 1
extern TwoWire Wire1;
#endif
#if WIRE_HOWMANY > 2
extern TwoWire Wire2;
#endif


#endif
#endif