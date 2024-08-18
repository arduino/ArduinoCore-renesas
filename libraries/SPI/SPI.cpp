/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * Copyright (c) 2024 by Mitchell C. Nelson, PhD <drmcnelsonlab@gmai.com>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 *
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
  int const max_index = PINS_COUNT;
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
    _spi_cfg.p_callback = nullptr;
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

  /* not using FSP for SPI anymore and no interrupts */

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

    if (_is_sci) {
        _spi_cb_event[_cb_event_idx] = SPI_EVENT_TRANSFER_ABORTED;

        _write_then_read(&_spi_sci_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_8_BITS);

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
    }
    else
    {
        _spi_ctrl.p_regs->SPDR_BY = data;
        while (0U == _spi_ctrl.p_regs->SPSR_b.SPRF) {}
        rxbuf = _spi_ctrl.p_regs->SPDR_BY;
    }

  return rxbuf;
}

/* This provides true 16 bit transfers
 */
uint16_t ArduinoSPI::transfer16(uint16_t data)
{
    uint16_t rxbuf;

    if (_is_sci) {
        _spi_cb_event[_cb_event_idx] = SPI_EVENT_TRANSFER_ABORTED;

        _write_then_read(&_spi_sci_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_16_BITS);

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
    }
    else
    {
      _spi_ctrl.p_regs->SPCR_b.SPE = 0; /* disable SPI unit */
      _spi_ctrl.p_regs->SPDCR = R_SPI0_SPDCR_SPLW_Msk; /* SPI word access */
      _spi_ctrl.p_regs->SPCMD_b[0].SPB = 0x0F; /* spi bit width = 16 */
      _spi_ctrl.p_regs->SPCR_b.SPE = 1; /* enable SPI unit */

      while (!_spi_ctrl.p_regs->SPSR_b.SPTEF){}

      _spi_ctrl.p_regs->SPDR = data;

      while (!_spi_ctrl.p_regs->SPSR_b.SPRF){}
      
      rxbuf = _spi_ctrl.p_regs->SPDR;


      _spi_ctrl.p_regs->SPCR_b.SPE = 0; /* disable SPI unit */
      _spi_ctrl.p_regs->SPDCR = R_SPI0_SPDCR_SPBYT_Msk; /* SPI byte access */
      _spi_ctrl.p_regs->SPCMD_b[0].SPB = 7; /* spi bit width = 8 */
      _spi_ctrl.p_regs->SPCR_b.SPE = 1; /* enable SPI unit */
    }

  return rxbuf;
}

/* Use these three functions when you need to loop over a device
   such as an ADC or DAC where you need to pulse a pin between
   each access.  Call transfer16_setup() before the loop, then
   call tansfer16_transfer() inside the loop, and then when you
   are done you may choose to call transfer16_cleanup() to restore
   the default 8 bit setup.
*/

uint16_t ArduinoSPI::transfer16_setup()
{
  if (!_is_sci) {
    _spi_ctrl.p_regs->SPCR_b.SPE = 0; /* disable SPI unit */
    _spi_ctrl.p_regs->SPDCR = R_SPI0_SPDCR_SPLW_Msk; /* SPI word access */
    _spi_ctrl.p_regs->SPCMD_b[0].SPB = 0x0F; /* spi bit width = 16 */
    _spi_ctrl.p_regs->SPCR_b.SPE = 1; /* enable SPI unit */
  }
}

uint16_t ArduinoSPI::transfer16_transfer(uint16_t data)
{
  uint16_t rxbuf;
  if (_is_sci) {
    return transfer16(data);
  }
  else {
    while (!_spi_ctrl.p_regs->SPSR_b.SPTEF){}

    _spi_ctrl.p_regs->SPDR = data;
      
    while (!_spi_ctrl.p_regs->SPSR_b.SPRF){}
      
    rxbuf = _spi_ctrl.p_regs->SPDR;
  }
}

uint16_t ArduinoSPI::transfer16_cleanup()
{
  _spi_ctrl.p_regs->SPCR_b.SPE = 0; /* disable SPI unit */
  _spi_ctrl.p_regs->SPDCR = R_SPI0_SPDCR_SPBYT_Msk; /* SPI byte access */
  _spi_ctrl.p_regs->SPCMD_b[0].SPB = 7; /* spi bit width = 8 */
  _spi_ctrl.p_regs->SPCR_b.SPE = 1; /* enable SPI unit */
}


/* This is the original 16 bit transfer as two bytes, with an
   extra 1.4 usecs between the two bytes.
 */

uint16_t ArduinoSPI::transfer16_asbytes(uint16_t data)
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
    if (NULL == buf) {
        return;
    }

    if (_is_sci) {
        _spi_cb_event[_cb_event_idx] = SPI_EVENT_TRANSFER_ABORTED;

        _write_then_read(&_spi_sci_ctrl, buf, buf, count, SPI_BIT_WIDTH_8_BITS);

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
    else {
        uint32_t *buffer32 = (uint32_t *) buf;
        size_t index_rx = 0;
        size_t index_tx = 0;
        size_t const n32 = count / 4U;
        uint8_t const bytes_remaining = (uint8_t) (count & 3U);

        if (n32 != 0U) {
            _spi_ctrl.p_regs->SPCR_b.SPE = 0; /* disable SPI unit */
            _spi_ctrl.p_regs->SPDCR = R_SPI0_SPDCR_SPLW_Msk; /* SPI word access */
            _spi_ctrl.p_regs->SPCMD_b[0].SPB = 2; /* spi bit width = 32 */
            _spi_ctrl.p_regs->SPCR_b.SPE = 1; /* enable SPI unit */

            while ((index_tx < 2U) && (index_tx < n32)) {
                if (_spi_ctrl.p_regs->SPSR_b.SPTEF) {
                    _spi_ctrl.p_regs->SPDR = buffer32[index_tx];
                    index_tx++;
                }
            }

            while (index_tx < n32) {
                if (_spi_ctrl.p_regs->SPSR_b.SPRF) {
                    uint32_t tmp = _spi_ctrl.p_regs->SPDR;
                    _spi_ctrl.p_regs->SPDR = buffer32[index_tx];
                    buffer32[index_rx] = tmp;
                    index_rx++;
                    index_tx++;
                }
            }

            while (index_rx < n32) { /* collect the last word received */
                if (_spi_ctrl.p_regs->SPSR_b.SPRF) {
                    uint32_t tmp = _spi_ctrl.p_regs->SPDR;
                    buffer32[index_rx] = tmp;
                    index_rx++;
                }
            }

            _spi_ctrl.p_regs->SPCR_b.SPE = 0; /* disable SPI unit */
            _spi_ctrl.p_regs->SPDCR = R_SPI0_SPDCR_SPBYT_Msk; /* SPI byte access */
            _spi_ctrl.p_regs->SPCMD_b[0].SPB = 7; /* spi bit width = 8 */
            _spi_ctrl.p_regs->SPCR_b.SPE = 1; /* enable SPI unit */
        }

        /* send the remaining bytes with 8-bit transfers */
        uint8_t *buffer = (uint8_t *) &buffer32[index_rx];

        for (uint8_t index = 0; index < bytes_remaining; index++) {
            _spi_ctrl.p_regs->SPDR_BY = buffer[index];
            while (0U == _spi_ctrl.p_regs->SPSR_b.SPRF) {}
            buffer[index] = _spi_ctrl.p_regs->SPDR_BY;
        }
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
/** SPI base register access macro.  */
#define SPI_REG(channel)  ((R_SPI0_Type *) ((uint32_t) R_SPI0 + \
                           ((uint32_t) R_SPI1 - (uint32_t) R_SPI0) * (channel)))

    _spi_ctrl.p_cfg             = &_spi_cfg;
    _spi_ctrl.p_callback        = _spi_cfg.p_callback;
    _spi_ctrl.p_context         = _spi_cfg.p_context;
    _spi_ctrl.p_callback_memory = NULL;
    _spi_ctrl.p_regs = SPI_REG(_spi_ctrl.p_cfg->channel);

    auto [clk_phase, clk_polarity, bit_order] = toFspSpiConfig(settings);
    rspck_div_setting_t spck_div = _spi_ext_cfg.spck_div;
    R_SPI_CalculateBitrate(settings.getClockFreq(), &spck_div);

    _spi_ctrl.p_regs->SPCR = 0; /* disable SPI unit */

    /* Power up the SPI module. */
    R_BSP_MODULE_START(FSP_IP_SPI, _spi_cfg.channel);

    /* configure SSLn polarity setting. */
    uint32_t sslp = 0;
    sslp |= (uint32_t) _spi_ext_cfg.ssl_polarity << _spi_ext_cfg.ssl_select;
    _spi_ctrl.p_regs->SSLP = (uint8_t) sslp;

    uint32_t sppcr  = 0;
    /* set MOSI idle value to low */
    sppcr |= R_SPI0_SPPCR_MOIFE_Msk;
    _spi_ctrl.p_regs->SPPCR = (uint8_t) sppcr;

    /* configure bit rate */
    _spi_ctrl.p_regs->SPBR = (uint8_t) spck_div.spbr;

    /* the SPBYT bit in SPDCR is documented only by "Technical Update" */
    _spi_ctrl.p_regs->SPDCR_b.SPBYT = 1; /* SPI byte access */

    /* register undocumented for the RA4M1 but found to be working and necessary */
    /* BYSW - Byte Swap Operating Mode Select - 1 = Byte Swap ON - essential for 32 bit transfers */
    _spi_ctrl.p_regs->SPDCR2_b.BYSW = ~bit_order;

    _spi_ctrl.p_regs->SPCKD = 0;

    _spi_ctrl.p_regs->SSLND = 0;

    _spi_ctrl.p_regs->SPND = 0;

    _spi_ctrl.p_regs->SPCR2 = R_SPI0_SPCR2_SCKASE_Msk;

    /* SPMS = 0 -> SPI operation, TXMD = 0 -> full-duplex, SPxIE = 0 -> no interrupts */
    if (SPI_MODE_MASTER == _spi_cfg.operating_mode) {
        _spi_ctrl.p_regs->SPCR_b.MSTR = 1;
    }

    _spi_ctrl.p_regs->SPCMD[0] = 0;
    _spi_ctrl.p_regs->SPCMD_b[0].CPHA = clk_phase;
    _spi_ctrl.p_regs->SPCMD_b[0].CPOL = clk_polarity;
    _spi_ctrl.p_regs->SPCMD_b[0].BRDV = spck_div.brdv; /* set bit rate division */
    _spi_ctrl.p_regs->SPCMD_b[0].SPB = 7; /* spi bit width = 8 */
    _spi_ctrl.p_regs->SPCMD_b[0].LSBF = bit_order;

    _spi_ctrl.p_regs->SPSR; /* read to clear OVRF */
    _spi_ctrl.p_regs->SPSR = 0; /* clear status register */

    _spi_ctrl.p_regs->SPCR_b.SPE = 1; /* enable SPI unit */

    _spi_ctrl.open = (0x52535049ULL); /* "SPI" in ASCII, used to determine if channel is open. */
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
