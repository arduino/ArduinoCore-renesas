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

#include "SPI.h"

extern const spi_extended_cfg_t g_spi0_ext_cfg;
extern const spi_extended_cfg_t g_spi1_ext_cfg;
extern const sci_spi_extended_cfg_t g_spi2_cfg_extend;

uint8_t ArduinoSPI::initialized = 0;
uint8_t ArduinoSPI::interruptMode = 0;
uint8_t ArduinoSPI::interruptMask = 0;
uint8_t ArduinoSPI::interruptSave = 0;

ArduinoSPI::ArduinoSPI(spi_ctrl_t *g_spi_ctrl
                      ,const spi_cfg_t *g_spi_cfg
                      ,const spi_extended_cfg_t *g_spi_ext_cfg):
  _g_spi_ctrl(g_spi_ctrl)
, _g_spi_cfg(g_spi_cfg)
, _g_spi_ext_cfg(g_spi_ext_cfg)
, _g_spi_callback_event(SPI_EVENT_TRANSFER_ABORTED)
, _clk_phase(SPI_CLK_PHASE_EDGE_ODD)
, _clk_polarity(SPI_CLK_POLARITY_LOW)
, _bit_order(SPI_BIT_ORDER_MSB_FIRST)
, _is_sci(false)
{
}

ArduinoSPI::ArduinoSPI(spi_ctrl_t *g_spi_ctrl
                      ,const spi_cfg_t *g_spi_cfg
                      ,const sci_spi_extended_cfg_t *g_spi_ext_cfg):
  _g_spi_ctrl(g_spi_ctrl)
, _g_spi_cfg(g_spi_cfg)
, _g_sci_spi_ext_cfg(g_spi_ext_cfg)
, _g_spi_callback_event(SPI_EVENT_TRANSFER_ABORTED)
, _clk_phase(SPI_CLK_PHASE_EDGE_ODD)
, _clk_polarity(SPI_CLK_POLARITY_LOW)
, _bit_order(SPI_BIT_ORDER_MSB_FIRST)
, _is_sci(true)
{
}

ArduinoSPI::ArduinoSPI(int miso, int mosi, int sck, int cs, int ch, bool isSci):
  _miso(digitalPinToBspPin(miso)),
  _mosi(digitalPinToBspPin(mosi)),
  _sck(digitalPinToBspPin(sck)),
  _cs(digitalPinToBspPin(cs)),
  _channel(ch),
  _is_sci(isSci)
{
  configureSPI(digitalPinToBspPin(miso), digitalPinToBspPin(mosi),
               digitalPinToBspPin(sck), digitalPinToBspPin(cs), ch, isSci);
}

ArduinoSPI::ArduinoSPI(bsp_io_port_pin_t miso, bsp_io_port_pin_t mosi,
                       bsp_io_port_pin_t sck, bsp_io_port_pin_t cs, int ch, bool isSci):
  _miso(miso),
  _mosi(mosi),
  _sck(sck),
  _cs(cs),
  _channel(ch),
  _is_sci(isSci)
{
  configureSPI(miso, mosi, sck, cs, ch, isSci);
}

void ArduinoSPI::configureSPI(bsp_io_port_pin_t miso, bsp_io_port_pin_t mosi,
                              bsp_io_port_pin_t sck, bsp_io_port_pin_t cs, int ch, bool isSci) {
  uint32_t peripheralCfg = 0;
  if (isSci) {
    if (ch%2 == 0) {
      peripheralCfg = (uint32_t) IOPORT_PERIPHERAL_SCI0_2_4_6_8;
    } else {
      peripheralCfg = (uint32_t) IOPORT_PERIPHERAL_SCI1_3_5_7_9;
    }
    pinPeripheral(miso, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
    pinPeripheral(mosi, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
    pinPeripheral(sck, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
    pinPeripheral(cs, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
    // TO DO
    /*
    if (ch==0) {
      _g_spi_ctrl = &g_spi0_ctrl;
      _g_spi_cfg = &g_spi0_cfg;
      _g_spi_ext_cfg = &g_spi0_ext_cfg;
    } else if (ch==1) {
      _g_spi_ctrl = &g_spi1_ctrl;
      _g_spi_cfg = &g_i2c_master1_cfg;
      _g_sci_spi_ext_cfg = &g_spi1_cfg_extend;
    } else if (ch==2) {
      _g_spi_ctrl = &g_i2c_master2_ctrl;
      _g_i2c_master_cfg = &g_i2c_master2_cfg;
    }
    */
  } else {
    peripheralCfg = (uint32_t) IOPORT_PERIPHERAL_SPI;

    pinPeripheral(miso, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
    pinPeripheral(mosi, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
    pinPeripheral(sck, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
    pinPeripheral(cs, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);

    if (ch==0) {
      _g_spi_ctrl = &g_spi0_ctrl;
      _g_spi_cfg = &g_spi0_cfg;
      _g_spi_ext_cfg = &g_spi0_ext_cfg;
    } else if (ch==1) {
      _g_spi_ctrl = &g_spi1_ctrl;
      _g_spi_cfg = &g_spi1_cfg;
      _g_spi_ext_cfg = &g_spi1_ext_cfg;
      /*
      _g_spi_ctrl = &g_spi1_ctrl;
      _g_spi_cfg = &g_i2c_master1_cfg;
      _g_sci_spi_ext_cfg = &g_spi1_cfg_extend;
      */
    }
  }
}


void ArduinoSPI::begin()
{
  if(!initialized) {
    if (_is_sci) {
      R_SCI_SPI_Open(_g_spi_ctrl, _g_spi_cfg);
    } else {
      R_SPI_Open(_g_spi_ctrl, _g_spi_cfg);
    }
    initialized = true;
  }
}

void ArduinoSPI::end() {
  if (initialized){
      initialized = false;
  }
}

void ArduinoSPI::usingInterrupt(int interruptNumber)
{
}

void ArduinoSPI::notUsingInterrupt(int interruptNumber)
{
}

uint8_t ArduinoSPI::transfer(uint8_t data) {
  uint8_t rxbuf;
  if (_is_sci) {
    R_SCI_SPI_WriteRead(_g_spi_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_8_BITS);
  } else {
    R_SPI_WriteRead(_g_spi_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_8_BITS);
  }
  return rxbuf;
}

uint16_t ArduinoSPI::transfer16(uint16_t data) {
  uint16_t rxbuf;
  if (_is_sci) {
    R_SCI_SPI_WriteRead(_g_spi_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_16_BITS);
  } else {
    R_SPI_WriteRead(_g_spi_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_16_BITS);
  }
  return rxbuf;
}

void ArduinoSPI::transfer(void *buf, size_t count) {
  if (_is_sci) {
    R_SCI_SPI_WriteRead(_g_spi_ctrl, buf, buf, count, SPI_BIT_WIDTH_8_BITS);
  } else {
    R_SPI_WriteRead(_g_spi_ctrl, buf, buf, count, SPI_BIT_WIDTH_8_BITS);
  }
}

void ArduinoSPI::beginTransaction(arduino::SPISettings settings) {
  // data mode
  switch(settings.getDataMode()){
      case arduino::SPI_MODE0:
          _clk_polarity = SPI_CLK_POLARITY_LOW;
          _clk_phase = SPI_CLK_PHASE_EDGE_ODD;
          break;
      case arduino::SPI_MODE1:
          _clk_polarity = SPI_CLK_POLARITY_LOW;
          _clk_phase = SPI_CLK_PHASE_EDGE_EVEN;
          break;
      case arduino::SPI_MODE2:
          _clk_polarity = SPI_CLK_POLARITY_HIGH;
          _clk_phase = SPI_CLK_PHASE_EDGE_ODD;
          break;
      case arduino::SPI_MODE3:
          _clk_polarity = SPI_CLK_POLARITY_HIGH;
          _clk_phase = SPI_CLK_PHASE_EDGE_EVEN;
          break;
  }
  // bit order
  if(settings.getBitOrder() == LSBFIRST){
      _bit_order = SPI_BIT_ORDER_LSB_FIRST;
  } else {
      _bit_order = SPI_BIT_ORDER_MSB_FIRST;
  }

  // Clock settings
  if (_is_sci) {

    if (initialized) {
      R_SCI_SPI_Close(_g_spi_ctrl);
    }

    sci_spi_div_setting_t clk_div = _g_sci_spi_ext_cfg->clk_div;
    R_SCI_SPI_CalculateBitrate(settings.getClockFreq(), &clk_div, false);

    R_SCI_SPI_Open(_g_spi_ctrl, _g_spi_cfg);

    sci_spi_instance_ctrl_t * p_ctrl = (sci_spi_instance_ctrl_t *)_g_spi_ctrl;
    uint32_t spmr = p_ctrl->p_reg->SPMR;
    uint32_t scmr = p_ctrl->p_reg->SCMR;
    uint32_t smr  = R_SCI0_SMR_CM_Msk;

    /* Configure CPHA setting. */
    spmr |= (uint32_t) _clk_phase << 7;

    /* Configure CPOL setting. */
    spmr |= (uint32_t) _clk_polarity << 6;

    /* Configure Bit Order (MSB,LSB) */
    scmr |= (uint32_t) _bit_order << 3;
  
    /* Select the baud rate generator clock divider. */
    smr |= (uint32_t) clk_div.cks;

    // Update settings
    p_ctrl->p_reg->SMR  = (uint8_t) smr;
    p_ctrl->p_reg->BRR  = (uint8_t) clk_div.brr;
    p_ctrl->p_reg->SPMR = spmr;
    p_ctrl->p_reg->SCMR = scmr;

  } else {

    if (initialized) {
      R_SPI_Close(_g_spi_ctrl);
    }

    rspck_div_setting_t spck_div = _g_spi_ext_cfg->spck_div;
    R_SPI_CalculateBitrate(settings.getClockFreq(), &spck_div);

    R_SPI_Open(_g_spi_ctrl, _g_spi_cfg);

    spi_instance_ctrl_t * p_ctrl = (spi_instance_ctrl_t *)_g_spi_ctrl;
    uint32_t spcmd0 = p_ctrl->p_regs->SPCMD[0];
    uint32_t spbr = p_ctrl->p_regs->SPBR;

    /* Configure CPHA setting. */
    spcmd0 |= (uint32_t) _clk_phase;

    /* Configure CPOL setting. */
    spcmd0 |= (uint32_t) _clk_polarity << 1;

    /* Configure Bit Order (MSB,LSB) */
    spcmd0 |= (uint32_t) _bit_order << 12;

    /* Configure the Bit Rate Division Setting */
    spcmd0 &= !(((uint32_t)0xFF) << 2);
    spcmd0 |= (uint32_t) spck_div.brdv << 2;

    // Update settings
    p_ctrl->p_regs->SPCMD[0] = (uint16_t) spcmd0;
    p_ctrl->p_regs->SPBR = (uint8_t) spck_div.spbr;

  }
}

void ArduinoSPI::endTransaction(void) {

}

void ArduinoSPI::attachInterrupt() {

}

void ArduinoSPI::detachInterrupt() {

}


void __attribute__((weak)) spi0_callback(spi_callback_args_t *p_args) {}

#if SPI_HOWMANY > 0
ArduinoSPI SPI(&g_spi1_ctrl, &g_spi1_cfg, &g_spi1_ext_cfg);
void __attribute__((weak)) spi1_callback(spi_callback_args_t *p_args)
{
    if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
    {
    }
}
#endif

#if SPI_HOWMANY > 1
ArduinoSPI SPI1(&g_spi2_ctrl, &g_spi2_cfg, &g_spi2_cfg_extend);
void __attribute__((weak)) spi2_callback(spi_callback_args_t *p_args)
{
    if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
    {
    }
}
#endif