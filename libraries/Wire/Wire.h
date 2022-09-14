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
#include "IRQManager.h"
#include "api/HardwareI2C.h"
#include "api/Stream.h"
#include <inttypes.h>

#include "bsp_api.h"
#include "hal_data.h"


#include "r_i2c_slave_api.h"
#include "r_i2c_master_api.h"

using I2C_masterOpen_f          = fsp_err_t (*)(i2c_master_ctrl_t *const p_api_ctrl, i2c_master_cfg_t const *const p_cfg);
using I2C_masterRead_f          = fsp_err_t (*)(i2c_master_ctrl_t *const p_api_ctrl, uint8_t *const p_dest, uint32_t const bytes, bool const restart);
using I2C_masterWrite_f         = fsp_err_t (*)(i2c_master_ctrl_t *const p_api_ctrl, uint8_t *constp_src, uint32_t const bytes, bool const restart);
using I2C_masterAbort_f         = fsp_err_t (*)(i2c_master_ctrl_t *const p_api_ctrl);
using I2C_masterSetSlaveAdd_f   = fsp_err_t (*)(i2c_master_ctrl_t *const p_api_ctrl, uint32_t const slave, i2c_master_addr_mode_t const addr_mode);
using I2C_masterSetCallBack_f   = fsp_err_t (*)(i2c_master_ctrl_t *const p_api_ctrl, void(*p_callback)(i2c_master_callback_args_t *), void const *const p_context, i2c_master_callback_args_t *const p_callback_memory);
using I2C_masterGetStatus_f     = fsp_err_t (*)(i2c_master_ctrl_t *const p_api_ctrl, i2c_master_status_t *p_status);
using I2C_masterClose_f         = fsp_err_t (*)(i2c_master_ctrl_t *const p_api_ctrl);

using I2C_slaveOpen_f           = fsp_err_t (*)(i2c_slave_ctrl_t *const p_api_ctrl, i2c_slave_cfg_t const *const p_cfg);
using I2C_slaveRead_f           = fsp_err_t (*)(i2c_slave_ctrl_t *const p_api_ctrl, uint8_t *const p_dest, uint32_t const bytes);
using I2C_slaveWrite_f          = fsp_err_t (*)(i2c_slave_ctrl_t *const p_api_ctrl, uint8_t *const p_src, uint32_t const bytes);
using I2C_slaveSetCallBack_f    = fsp_err_t (*)(i2c_slave_ctrl_t *const p_api_ctrl, void(*p_callback)(i2c_slave_callback_args_t *), void const *const p_context, i2c_slave_callback_args_t *const p_callback_memory);
using I2C_slaveClose_f          = fsp_err_t (*)(i2c_slave_ctrl_t *const p_api_ctrl);

using I2C_onRxCallback_f        = void (*)(int);
using I2C_onTxCallback_f        = void (*)(void);


#define I2C_BUFFER_LENGTH      256

/* 11 channels are possible assuming 2 channels from "actual" I2C peripherals and
   9 channesl from SCI I2C master peripherals 
   This define is used to set up an array of pointer to Wire objects 
   first 2 position of this array will be used for actual I2C peripherals and 
   the following 9 for SCI I2C peripherals 
   That is the meaning of SCI_I2C_CHANNEL_OFFSET */
#define SCI_I2C_CHANNEL_OFFSET 2
#define MAX_I2CS               11

#ifdef __cplusplus

#ifndef __ARDUINO_WIRE_IMPLEMENTATION__
#define __ARDUINO_WIRE_IMPLEMENTATION__



// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

typedef enum {
  ADDRESS_MODE_7_BITS,
  ADDRESS_MODE_10_BITS
} WireAddressMode_t;

typedef enum {
  SPEED_STANDARD = 0,  //100 kHz
  SPEED_FAST,      //400 kHz
  SPEED_VERY_FAST  //1 Mhz
} WireSpeed_t;


typedef enum {
  WIRE_STATUS_UNSET,
  WIRE_STATUS_RX_COMPLETED,
  WIRE_STATUS_TX_COMPLETED,
  WIRE_STATUS_TRANSACTION_ABORTED,
  WIRE_STATUS_RX_REQUEST,
  WIRE_STATUS_TX_REQUEST,
  WIRE_STATUS_GENERAL_CALL
} WireStatus_t;

class TwoWire : public arduino::HardwareI2C {
  
  public:
    TwoWire(int scl_pin, int sda_pin, WireSpeed_t wp = SPEED_STANDARD, WireAddressMode_t am = ADDRESS_MODE_7_BITS, bool prefer_sci = false);
    void begin();
    void begin(uint8_t);
    void begin(uint16_t);
    void begin(int);
    void end();
    void setClock(uint32_t);

    void beginTransmission(uint32_t);
    void beginTransmission(uint16_t);
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

    void setBusStatus(WireStatus_t);

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write;

  private:
    
    static TwoWire *g_Wires[MAX_I2CS];
    static void WireMasterCallback(i2c_master_callback_args_t *);
    static void WireSlaveCallback(i2c_slave_callback_args_t *);
    
    unsigned int timeout;

    bool init_ok;
    uint8_t scl_pin;
    uint8_t sda_pin;
    bool is_master;
    int channel;
    bool require_sci;
    bool is_sci;
    WireAddressMode_t address_mode;
    WireSpeed_t speed_mode;

    WireStatus_t bus_status;
    
    iic_master_extended_cfg_t m_i2c_extend;
    iic_master_instance_ctrl_t m_i2c_ctrl;
    i2c_master_cfg_t m_i2c_cfg;

    iic_slave_instance_ctrl_t s_i2c_ctrl;
    i2c_slave_cfg_t  s_i2c_cfg;
    uint16_t slave_address;

    uint32_t master_tx_address;

    bool transmission_begun;
    

    I2C_masterOpen_f            m_open;
    I2C_masterRead_f            m_read;
    I2C_masterWrite_f           m_write;
    I2C_masterAbort_f           m_abort;
    I2C_masterSetSlaveAdd_f     m_setSlaveAdd;
    I2C_masterSetCallBack_f     m_setCallback;
    I2C_masterGetStatus_f       m_getStatus;
    I2C_masterClose_f           m_close;

    I2C_slaveOpen_f             s_open;
    I2C_slaveRead_f             s_read;
    I2C_slaveWrite_f            s_write;
    I2C_slaveSetCallBack_f      s_setCallback;
    I2C_slaveClose_f            s_close;

    uint8_t rx_buffer[I2C_BUFFER_LENGTH];
    uint8_t tx_buffer[I2C_BUFFER_LENGTH];
    uint8_t tx_index;
    uint8_t rx_index;
    uint8_t rx_extract_index;

    uint8_t read_from(uint8_t, uint8_t*, uint8_t, unsigned int, bool);
    uint8_t write_to(uint8_t, uint8_t*, uint8_t, unsigned int, bool);

    bool cfg_pins(int max_index);

    I2C_onRxCallback_f  rx_callback;
    I2C_onTxCallback_f  tx_callback;
    
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