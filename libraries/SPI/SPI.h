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

#include <Arduino.h>
#include "api/HardwareSPI.h"

#include "bsp_api.h"
#include "hal_data.h"

extern const spi_extended_cfg_t g_spi0_ext_cfg;
extern const sci_spi_extended_cfg_t g_spi1_cfg_extend;

class ArduinoSPI : public arduino::SPIClass
{
public:
    ArduinoSPI(spi_ctrl_t *g_spi_ctrl 
              ,const spi_cfg_t *g_spi_cfg
              ,const spi_extended_cfg_t *g_spi_ext_cfg);
    ArduinoSPI(spi_ctrl_t *g_spi_ctrl 
              ,const spi_cfg_t *g_spi_cfg
              ,const sci_spi_extended_cfg_t *g_spi_ext_cfg);
    virtual uint8_t transfer(uint8_t data);
    virtual uint16_t transfer16(uint16_t data);
    virtual void transfer(void *buf, size_t count);

    // Transaction Functions
    virtual void usingInterrupt(int interruptNumber);
    virtual void notUsingInterrupt(int interruptNumber);
    virtual void beginTransaction(arduino::SPISettings settings);
    virtual void endTransaction(void);

    // SPI Configuration methods
    virtual void attachInterrupt();
    virtual void detachInterrupt();

    virtual void begin();
    virtual void end();

private:
    arduino::SPISettings settings = arduino::SPISettings(0, MSBFIRST, arduino::SPI_MODE0);
    static uint8_t initialized;
    static uint8_t interruptMode; // 0=none, 1=mask, 2=global
    static uint8_t interruptMask; // which interrupts to mask
    static uint8_t interruptSave; // temp storage, to restore state

    spi_ctrl_t *_g_spi_ctrl;
    const spi_cfg_t *_g_spi_cfg;
    const spi_extended_cfg_t *_g_spi_ext_cfg;
    const sci_spi_extended_cfg_t *_g_sci_spi_ext_cfg;
    spi_event_t _g_spi_callback_event;

    spi_clk_phase_t _clk_phase;
    spi_clk_polarity_t _clk_polarity;
    spi_bit_order_t _bit_order;

    bool _is_sci;
};

#if SPI_HOWMANY > 0
extern ArduinoSPI SPI;
#endif
#if SPI_HOWMANY > 1
#ifdef SPI1
#undef SPI1
#endif
extern ArduinoSPI SPI1;
#endif

#endif
