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

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "SPI.h"

#include <IRQManager.h>

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/

using namespace arduino;

/**************************************************************************************
 * EXTERN GLOBAL CONSTANTS
 **************************************************************************************/

extern const PinMuxCfg_t g_pin_cfg[];
extern const size_t g_pin_cfg_size;

/**************************************************************************************
 * GLOBAL MEMBER VARIABLES
 **************************************************************************************/

static spi_event_t _spi_cb_event[SPI_MAX_SPI_CHANNELS + SPI_MAX_SCI_CHANNELS] = {SPI_EVENT_TRANSFER_ABORTED};

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

ArduinoSPI::ArduinoSPI(int const miso_pin, int const mosi_pin, int const sck_pin, uint8_t const periph_mode)
: _miso_pin{miso_pin}
, _mosi_pin{mosi_pin}
, _sck_pin{sck_pin}
, _periph_mode{periph_mode}
, _channel{0}
, _cb_event_idx{0}
, _is_sci(false)
, _is_initialized{false}
, _open{nullptr}
, _close{nullptr}
, _write_then_read{nullptr}
{
  _spi_cfg.rxi_irq        = FSP_INVALID_VECTOR;
  _spi_cfg.txi_irq        = FSP_INVALID_VECTOR;
  _spi_cfg.tei_irq        = FSP_INVALID_VECTOR;
  _spi_cfg.eri_irq        = FSP_INVALID_VECTOR;

}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

void ArduinoSPI::begin()
{
  bool init_ok = true;

  /* Configure the pins and auto-determine channel and
   * whether or not we are using a SCI.
   */
  int const max_index = g_pin_cfg_size / sizeof(g_pin_cfg[0]);
  auto [cfg_pins_ok, cfg_channel, cfg_is_sci] = cfg_pins(max_index, _miso_pin, _mosi_pin, _sck_pin, _periph_mode);
  init_ok &= cfg_pins_ok;
  _channel = cfg_channel;
  _is_sci = cfg_is_sci;

  /* Set the approbriate function pointers depending on
   * wheter this is a SCI or not.
   */
  if (_is_sci)
  {
    _open               = R_SCI_SPI_Open;
    _close              = R_SCI_SPI_Close;
    _write_then_read    = R_SCI_SPI_WriteRead;

    _cb_event_idx       = (_channel + SPI_MAX_SPI_CHANNELS);

    _spi_cfg.p_extend   = &_sci_spi_ext_cfg;
    _spi_cfg.p_callback = sci_spi_callback;
  }
  else
  {
    _open               = R_SPI_Open;
    _close              = R_SPI_Close;
    _write_then_read    = R_SPI_WriteRead;

    _cb_event_idx       = _channel;

    _spi_cfg.p_extend   = &_spi_ext_cfg;
    _spi_cfg.p_callback = spi_callback;
  }

  /* SPI configuration for SPI HAL driver. */
  _spi_cfg.channel        = _channel;

  _spi_cfg.rxi_ipl        = (12);
  _spi_cfg.txi_ipl        = (12);
  _spi_cfg.tei_ipl        = (12);
  _spi_cfg.eri_ipl        = (12);

  _spi_cfg.operating_mode = SPI_MODE_MASTER;

  _spi_cfg.clk_phase      = SPI_CLK_PHASE_EDGE_ODD;
  _spi_cfg.clk_polarity   = SPI_CLK_POLARITY_LOW;

  _spi_cfg.mode_fault     = SPI_MODE_FAULT_ERROR_DISABLE;
  _spi_cfg.bit_order      = SPI_BIT_ORDER_MSB_FIRST;
  _spi_cfg.p_transfer_tx  = NULL;
  _spi_cfg.p_transfer_rx  = NULL;
  _spi_cfg.p_context      = NULL;


  /** Extended SPI configuration for SPI HAL driver. */
  _spi_ext_cfg.spi_clksyn         = SPI_SSL_MODE_CLK_SYN;
  _spi_ext_cfg.spi_comm           = SPI_COMMUNICATION_FULL_DUPLEX;
  _spi_ext_cfg.ssl_polarity       = SPI_SSLP_LOW;
  _spi_ext_cfg.ssl_select         = SPI_SSL_SELECT_SSL0;
  _spi_ext_cfg.mosi_idle          = SPI_MOSI_IDLE_VALUE_FIXING_DISABLE;
  _spi_ext_cfg.parity             = SPI_PARITY_MODE_DISABLE;
  _spi_ext_cfg.byte_swap          = SPI_BYTE_SWAP_DISABLE;
  _spi_ext_cfg.spck_div           = { /* Actual calculated bitrate: 12000000. */ .spbr = 1, .brdv = 0 };
  _spi_ext_cfg.spck_delay         = SPI_DELAY_COUNT_1;
  _spi_ext_cfg.ssl_negation_delay = SPI_DELAY_COUNT_1;
  _spi_ext_cfg.next_access_delay  = SPI_DELAY_COUNT_1;


  /** Extended SPI configuration for SPI SCI HAL driver. */
  /* Actual calculated bitrate: 1000000. */
  _sci_spi_ext_cfg.clk_div.cks  = 0;
  _sci_spi_ext_cfg.clk_div.brr  = 11;
  _sci_spi_ext_cfg.clk_div.mddr = 0;

  configSpiSettings(DEFAULT_SPI_SETTINGS);

  /* Configure the Interrupt Controller. */
  if (_is_sci)
  {
    SciSpiMasterIrqReq_t irq_req
    {
      .ctrl = &_spi_sci_ctrl,
      .cfg = &_spi_cfg,
      .hw_channel = (uint8_t)_channel,
    };
    init_ok &= IRQManager::getInstance().addPeripheral(IRQ_SCI_SPI_MASTER, &irq_req);

    if (FSP_SUCCESS == _open(&_spi_sci_ctrl, &_spi_cfg)) {
      init_ok &= true;
    } else {
      init_ok = false;
    }
  }
  else
  {
    SpiMasterIrqReq_t irq_req
    {
      .ctrl = &_spi_ctrl,
      .cfg = &_spi_cfg,
      .hw_channel = (uint8_t)_channel,
    };
    init_ok &= IRQManager::getInstance().addPeripheral(IRQ_SPI_MASTER, &irq_req);

    if (FSP_SUCCESS == _open(&_spi_ctrl, &_spi_cfg)) {
      init_ok &= true;
    } else {
      init_ok = false;
    }
  }

  _is_initialized = init_ok;
}

void ArduinoSPI::end()
{
  if (_is_sci) {
    _close(&_spi_sci_ctrl);
  } else {
    _close(&_spi_ctrl);
  }
  _is_initialized = false;
}

uint8_t ArduinoSPI::transfer(uint8_t data)
{
  uint8_t rxbuf;
  _spi_cb_event[_cb_event_idx] = SPI_EVENT_TRANSFER_ABORTED;
  if (_is_sci) {
    _write_then_read(&_spi_sci_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_8_BITS);
  } else {
    _write_then_read(&_spi_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_8_BITS);
  }

  for (auto const start = millis();
       (SPI_EVENT_TRANSFER_COMPLETE != _spi_cb_event[_cb_event_idx]) && (millis() - start < 1000); )
  {
      __NOP();
  }
  if (SPI_EVENT_TRANSFER_ABORTED == _spi_cb_event[_cb_event_idx])
  {
      end();
      return 0;
  }
  return rxbuf;
}

uint16_t ArduinoSPI::transfer16(uint16_t data)
{
  union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } t;
  t.val = data;

  if (_settings.getBitOrder() == LSBFIRST) {
    t.lsb = transfer(t.lsb);
    t.msb = transfer(t.msb);
  } else {
    t.msb = transfer(t.msb);
    t.lsb = transfer(t.lsb);
  }
  return t.val;
}

void ArduinoSPI::transfer(void *buf, size_t count)
{
  _spi_cb_event[_cb_event_idx] = SPI_EVENT_TRANSFER_ABORTED;

  if (_is_sci) {
    _write_then_read(&_spi_sci_ctrl, buf, buf, count, SPI_BIT_WIDTH_8_BITS);
  } else {
    _write_then_read(&_spi_ctrl, buf, buf, count, SPI_BIT_WIDTH_8_BITS);
  }

  for (auto const start = millis();
       (SPI_EVENT_TRANSFER_COMPLETE != _spi_cb_event[_cb_event_idx]) && (millis() - start < 1000); )
  {
      __NOP();
  }
  if (SPI_EVENT_TRANSFER_ABORTED == _spi_cb_event[_cb_event_idx])
  {
      end();
  }
}

void ArduinoSPI::beginTransaction(arduino::SPISettings settings)
{
  if (!_is_initialized)
    begin();

  if (_settings != settings)
  {
    configSpiSettings(settings);
    _settings = settings;
    /*
    if (_is_sci) {
      _close(&_spi_sci_ctrl);
      _open(&_spi_sci_ctrl, &_spi_cfg);
    } else {
      _close(&_spi_ctrl);
      _open(&_spi_ctrl, &_spi_cfg);
    }
    */
  }
}

/**************************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 **************************************************************************************/

std::tuple<bool, int, bool> ArduinoSPI::cfg_pins(int const max_index, int const miso_pin, int const mosi_pin, int const sck_pin, uint8_t const periph_mode)
{
  /* Provide default return values. */
  int channel = 0;
  bool is_sci = false;

  /* Verify if indices are good. */
  if (miso_pin < 0 || mosi_pin < 0 || sck_pin < 0 || miso_pin >= max_index || mosi_pin >= max_index || sck_pin >= max_index) {
    return std::make_tuple(false, channel, is_sci);
  }

  /* Getting configuration from table. */
  auto cfgs_miso = getPinCfgs(miso_pin, PIN_CFG_REQ_MISO);
  auto cfgs_mosi = getPinCfgs(mosi_pin, PIN_CFG_REQ_MOSI);
  auto cfgs_sck = getPinCfgs(sck_pin, PIN_CFG_REQ_SCK);

  uint16_t cfg_miso = 0;
  uint16_t cfg_mosi = 0;
  uint16_t cfg_sck = 0;

  /* Find the best combination */
  for (size_t i = 0; i < cfgs_miso.size(); i++) {
    for (size_t j = 0; j < cfgs_mosi.size(); j++) {
      for (size_t k = 0; k < cfgs_sck.size(); k++) {
        if (cfgs_miso[i] && cfgs_mosi[i] && cfgs_sck[i] &&
            (GET_CHANNEL(cfgs_miso[i]) == GET_CHANNEL(cfgs_mosi[j])) && (GET_CHANNEL(cfgs_miso[i]) == GET_CHANNEL(cfgs_sck[k]))) {
          cfg_miso = cfgs_miso[i];
          cfg_mosi = cfgs_mosi[j];
          cfg_sck = cfgs_sck[k];
          channel = GET_CHANNEL(cfg_miso);
          if ((IS_SCI(cfg_miso) && periph_mode == MODE_SCI) || (!IS_SCI(cfg_miso) && periph_mode == MODE_SPI) || (periph_mode == MODE_DONTCARE)) {
            goto done;
          }
        }
      }
    }
  }

done:
  /* Verify if configurations are good. */
  if (cfg_miso == 0 || cfg_mosi == 0 || cfg_sck == 0) {
    return std::make_tuple(false, channel, is_sci);
  }

  /* Actually configure pin functions. */
  ioport_peripheral_t ioport_miso, ioport_mosi, ioport_sck;

  if(IS_SCI(cfg_miso)) {
    if(channel >= SPI_MAX_SCI_CHANNELS) {
      return std::make_tuple(false, channel, is_sci);
    }
    is_sci = true;
    ioport_miso = USE_SCI_EVEN_CFG(cfg_mosi) ? IOPORT_PERIPHERAL_SCI0_2_4_6_8 : IOPORT_PERIPHERAL_SCI1_3_5_7_9;
    ioport_mosi = USE_SCI_EVEN_CFG(cfg_miso) ? IOPORT_PERIPHERAL_SCI0_2_4_6_8 : IOPORT_PERIPHERAL_SCI1_3_5_7_9;
    ioport_sck  = USE_SCI_EVEN_CFG(cfg_sck ) ? IOPORT_PERIPHERAL_SCI0_2_4_6_8 : IOPORT_PERIPHERAL_SCI1_3_5_7_9;

    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[miso_pin].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | ioport_miso));
    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[mosi_pin].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | ioport_mosi));
    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[sck_pin ].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | ioport_sck));
  }
  else {
    if(channel >= SPI_MAX_SPI_CHANNELS) {
      return std::make_tuple(false, channel, is_sci);
    }
    is_sci = false;
    ioport_miso = IOPORT_PERIPHERAL_SPI;
    ioport_mosi = IOPORT_PERIPHERAL_SPI;
    ioport_sck  = IOPORT_PERIPHERAL_SPI;

    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[miso_pin].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | ioport_miso));
    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[mosi_pin].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | ioport_mosi));
    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[sck_pin ].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | ioport_sck));
  }

  return std::make_tuple(true, channel, is_sci);;
}


void ArduinoSPI::configSpiSettings(arduino::SPISettings const & settings)
{
  if (_is_sci)
    configSpiSci(settings);
  else
    configSpi(settings);
}

void ArduinoSPI::configSpi(arduino::SPISettings const & settings)
{
  auto [clk_phase, clk_polarity, bit_order] = toFspSpiConfig(settings);

  rspck_div_setting_t spck_div = _spi_ext_cfg.spck_div;
  R_SPI_CalculateBitrate(settings.getClockFreq(), &spck_div);

  uint32_t spcmd0 = _spi_ctrl.p_regs->SPCMD[0];

  /* Configure CPHA setting. */
  spcmd0 |= (uint32_t) clk_phase;

  /* Configure CPOL setting. */
  spcmd0 |= (uint32_t) clk_polarity << 1;

  /* Configure Bit Order (MSB,LSB) */
  spcmd0 |= (uint32_t) bit_order << 12;

  /* Configure the Bit Rate Division Setting */
  spcmd0 &= !(((uint32_t)0xFF) << 2);
  spcmd0 |= (uint32_t) spck_div.brdv << 2;

  /* Update settings. */
  _spi_ctrl.p_regs->SPCMD[0] = (uint16_t) spcmd0;
  _spi_ctrl.p_regs->SPBR = (uint8_t) spck_div.spbr;
}

void ArduinoSPI::configSpiSci(arduino::SPISettings const & settings)
{
  auto [clk_phase, clk_polarity, bit_order] = toFspSpiConfig(settings);

  sci_spi_div_setting_t clk_div = _sci_spi_ext_cfg.clk_div;
  R_SCI_SPI_CalculateBitrate(settings.getClockFreq(), &clk_div, false);

  uint32_t spmr = _spi_sci_ctrl.p_reg->SPMR;
  uint32_t scmr = _spi_sci_ctrl.p_reg->SCMR;
  uint32_t smr  = R_SCI0_SMR_CM_Msk;

  /* Configure CPHA setting. */
  spmr |= (uint32_t) clk_phase << 7;

  /* Configure CPOL setting. */
  spmr |= (uint32_t) clk_polarity << 6;

  /* Configure Bit Order (MSB,LSB) */
  scmr |= (uint32_t) bit_order << 3;

  /* Select the baud rate generator clock divider. */
  smr |= (uint32_t) clk_div.cks;

  /* Update settings. */
  _spi_sci_ctrl.p_reg->SMR  = (uint8_t) smr;
  _spi_sci_ctrl.p_reg->BRR  = (uint8_t) clk_div.brr;
  _spi_sci_ctrl.p_reg->SPMR = spmr;
  _spi_sci_ctrl.p_reg->SCMR = scmr;
}

std::tuple<spi_clk_phase_t, spi_clk_polarity_t, spi_bit_order_t> ArduinoSPI::toFspSpiConfig(arduino::SPISettings const & settings)
{
  spi_clk_phase_t clk_phase = SPI_CLK_PHASE_EDGE_ODD;
  spi_clk_polarity_t clk_polarity = SPI_CLK_POLARITY_LOW;
  spi_bit_order_t bit_order = SPI_BIT_ORDER_MSB_FIRST;

  switch(settings.getDataMode())
  {
    case arduino::SPI_MODE0:
      clk_polarity = SPI_CLK_POLARITY_LOW;
      clk_phase = SPI_CLK_PHASE_EDGE_ODD;
    break;
    case arduino::SPI_MODE1:
     clk_polarity = SPI_CLK_POLARITY_LOW;
     clk_phase = SPI_CLK_PHASE_EDGE_EVEN;
    break;
    case arduino::SPI_MODE2:
      clk_polarity = SPI_CLK_POLARITY_HIGH;
      clk_phase = SPI_CLK_PHASE_EDGE_ODD;
    break;
    case arduino::SPI_MODE3:
      clk_polarity = SPI_CLK_POLARITY_HIGH;
      clk_phase = SPI_CLK_PHASE_EDGE_EVEN;
    break;
  }

  if(settings.getBitOrder() == LSBFIRST)
    bit_order = SPI_BIT_ORDER_LSB_FIRST;
  else
    bit_order = SPI_BIT_ORDER_MSB_FIRST;

  return std::make_tuple(clk_phase, clk_polarity, bit_order);
}

/**************************************************************************************
 * CALLBACKS FOR FSP FRAMEWORK
 **************************************************************************************/

void spi_callback(spi_callback_args_t *p_args)
{
  if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
  {
    _spi_cb_event[p_args->channel] = SPI_EVENT_TRANSFER_COMPLETE;
  }
  else
  {
    /* Updating the flag here to capture and handle all other error events */
    _spi_cb_event[p_args->channel] = SPI_EVENT_TRANSFER_ABORTED;
  }
}

void sci_spi_callback(spi_callback_args_t *p_args)
{
  int const spi_master_offset = SPI_MAX_SPI_CHANNELS;
  if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
  {
    _spi_cb_event[p_args->channel + spi_master_offset] = SPI_EVENT_TRANSFER_COMPLETE;
  }
  else
  {
    /* Updating the flag here to capture and handle all other error events */
    _spi_cb_event[p_args->channel + spi_master_offset] = SPI_EVENT_TRANSFER_ABORTED;
  }
}

/**************************************************************************************
 * OBJECT INSTANTIATION
 **************************************************************************************/

#ifndef FORCE_SPI_MODE
#define FORCE_SPI_MODE MODE_DONTCARE
#endif

#ifndef FORCE_SPI1_MODE
#define FORCE_SPI1_MODE MODE_DONTCARE
#endif

#if SPI_HOWMANY > 0
ArduinoSPI SPI(PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SCK, FORCE_SPI_MODE);
#endif

#if SPI_HOWMANY > 1
ArduinoSPI SPI1(PIN_SPI1_MISO, PIN_SPI1_MOSI, PIN_SPI1_SCK, FORCE_SPI1_MODE);
#endif
