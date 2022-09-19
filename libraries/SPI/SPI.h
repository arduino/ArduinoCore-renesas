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

//extern const spi_extended_cfg_t g_spi0_ext_cfg;
//extern const sci_spi_extended_cfg_t g_spi1_cfg_extend;

namespace arduino {

class ArduinoSPI : public SPIClass
{
public:
    ArduinoSPI(spi_ctrl_t *g_spi_ctrl 
              ,const spi_cfg_t *g_spi_cfg
              ,const spi_extended_cfg_t *g_spi_ext_cfg, int ch);
    ArduinoSPI(spi_ctrl_t *g_spi_ctrl 
              ,const spi_cfg_t *g_spi_cfg
              ,const sci_spi_extended_cfg_t *g_spi_ext_cfg, int ch);
    ArduinoSPI(int ch, bool isSci = false);

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
    void setPins(int miso, int mosi, int sck, int cs = 0);
    void setPins(bsp_io_port_pin_t miso, bsp_io_port_pin_t mosi,
               bsp_io_port_pin_t sck, bsp_io_port_pin_t cs = (bsp_io_port_pin_t)0);

private:
    void enableSciSpiIrqs();

    arduino::SPISettings const DEFAULT_SPI_SETTINGS = arduino::SPISettings(1000000, MSBFIRST, arduino::SPI_MODE0);
    static uint8_t initialized;
    static uint8_t interruptMode; // 0=none, 1=mask, 2=global
    static uint8_t interruptMask; // which interrupts to mask
    static uint8_t interruptSave; // temp storage, to restore state

    spi_ctrl_t *_g_spi_ctrl;
    const spi_cfg_t *_g_spi_cfg;
    const spi_extended_cfg_t *_g_spi_ext_cfg;
    const sci_spi_extended_cfg_t *_g_sci_spi_ext_cfg;

    spi_clk_phase_t _clk_phase;
    spi_clk_polarity_t _clk_polarity;
    spi_bit_order_t _bit_order;

    bsp_io_port_pin_t _miso, _mosi, _sck, _cs;
    int _channel;
    int _cb_event_idx;

    bool _is_sci;
};

}

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
