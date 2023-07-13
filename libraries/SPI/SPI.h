/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino.h>
#include "api/HardwareSPI.h"

#include "bsp_api.h"

#include "r_spi.h"
#include "r_sci_spi.h"
#include "r_spi_api.h"

#include <tuple>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

extern "C" {
    void sci_spi_callback(spi_callback_args_t *p_args);
}

namespace arduino {

/**************************************************************************************
 * DEFINE
 **************************************************************************************/

#define SPI_MAX_SPI_CHANNELS (3)   // SPI0 to SPI2
#define SPI_MAX_SCI_CHANNELS (10)  // SCI0 to SCI9

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

using SPI_open_f          = fsp_err_t (*)(spi_ctrl_t * p_api_ctrl, spi_cfg_t const * const p_cfg);
using SPI_close_f         = fsp_err_t (*)(spi_ctrl_t * const p_api_ctrl);
using SPI_writeThenRead_f = fsp_err_t (*)(spi_ctrl_t * const p_api_ctrl, void const * p_src, void * p_dest, uint32_t const length, spi_bit_width_t const bit_width);

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

#define MODE_DONTCARE   0
#define MODE_SPI        1
#define MODE_SCI        2

class ArduinoSPI : public SPIClass
{
public:
    ArduinoSPI(int const miso_pin, int const mosi_pin, int const sck_pin, uint8_t const periph_mode);

    virtual uint8_t transfer(uint8_t data);
    virtual uint16_t transfer16(uint16_t data);
    virtual void transfer(void *buf, size_t count);

    // Transaction Functions
    virtual void usingInterrupt(int __attribute__((unused)) interruptNumber) { }
    virtual void notUsingInterrupt(int __attribute__((unused)) interruptNumber) { }
    virtual void beginTransaction(arduino::SPISettings settings);
    virtual void endTransaction(void) { }

    // SPI Configuration methods
    virtual void attachInterrupt() { }
    virtual void detachInterrupt() { }

    virtual void begin();
    virtual void end();


private:
    arduino::SPISettings const DEFAULT_SPI_SETTINGS = arduino::SPISettings(1000000, MSBFIRST, arduino::SPI_MODE0);
    arduino::SPISettings _settings = arduino::SPISettings(0, MSBFIRST, arduino::SPI_MODE0);

    int const _miso_pin;
    int const _mosi_pin;
    int const _sck_pin;
    uint8_t const _periph_mode;

    int _channel;
    int _cb_event_idx;

    bool _is_sci;
    bool _is_initialized;

    SPI_open_f _open;
    SPI_close_f _close;
    SPI_writeThenRead_f _write_then_read;

    spi_instance_ctrl_t _spi_ctrl;
    sci_spi_instance_ctrl_t _spi_sci_ctrl;

    spi_cfg_t _spi_cfg;

    spi_extended_cfg_t _spi_ext_cfg;
    sci_spi_extended_cfg_t _sci_spi_ext_cfg;

    static std::tuple<bool, int, bool> cfg_pins(int const max_index, int const miso_pin, int const mosi_pin, int const sck_pin, uint8_t const periph_mode);

    void configSpiSettings(arduino::SPISettings const & settings);
    void configSpi(arduino::SPISettings const & settings);
    void configSpiSci(arduino::SPISettings const & settings);
    static std::tuple<spi_clk_phase_t, spi_clk_polarity_t, spi_bit_order_t> toFspSpiConfig(arduino::SPISettings const & settings);
};

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

} /* arduino */

/**************************************************************************************
 * EXTERN DECLARATION
 **************************************************************************************/

#if SPI_HOWMANY > 0
extern arduino::ArduinoSPI SPI;
#endif
#if SPI_HOWMANY > 1
#ifdef SPI1
#undef SPI1
#endif
extern arduino::ArduinoSPI SPI1;
#endif

#endif
