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

static spi_event_t _spi_cb_event[13] = {SPI_EVENT_TRANSFER_ABORTED};
static uint32_t timeout_ms = 1000;

ArduinoSPI::ArduinoSPI(spi_ctrl_t *g_spi_ctrl
                      ,const spi_cfg_t *g_spi_cfg
                      ,const spi_extended_cfg_t *g_spi_ext_cfg, int ch):
  _g_spi_ctrl(g_spi_ctrl)
, _g_spi_cfg(g_spi_cfg)
, _g_spi_ext_cfg(g_spi_ext_cfg)
, _clk_phase(SPI_CLK_PHASE_EDGE_ODD)
, _clk_polarity(SPI_CLK_POLARITY_LOW)
, _bit_order(SPI_BIT_ORDER_MSB_FIRST)
, _channel(ch)
, _is_sci(false)
{
}

ArduinoSPI::ArduinoSPI(spi_ctrl_t *g_spi_ctrl
                      ,const spi_cfg_t *g_spi_cfg
                      ,const sci_spi_extended_cfg_t *g_spi_ext_cfg, int ch):
  _g_spi_ctrl(g_spi_ctrl)
, _g_spi_cfg(g_spi_cfg)
, _g_sci_spi_ext_cfg(g_spi_ext_cfg)
, _clk_phase(SPI_CLK_PHASE_EDGE_ODD)
, _clk_polarity(SPI_CLK_POLARITY_LOW)
, _bit_order(SPI_BIT_ORDER_MSB_FIRST)
, _channel(ch)
, _is_sci(true)
{
}

ArduinoSPI::ArduinoSPI(int ch, bool isSci):
  _channel(ch),
  _is_sci(isSci)
{
}

void ArduinoSPI::begin()
{
  bool isSPIObject = false;

  EPeripheralBus periphBusCfg = NOT_A_BUS;

#if SPI_HOWMANY > 0
  if (_channel == SPI_CHANNEL) {
    isSPIObject = true;
    periphBusCfg = SPI_BUS;
  }
#endif
#if SERIAL_HOWMANY > 1
  if (_channel == SPI1_CHANNEL) {
    isSPIObject = true;
    periphBusCfg = SPI1_BUS;
  }
#endif
#if SERIAL_HOWMANY > 2
  if (_channel == SPI2_CHANNEL) {
    isSPIObject = true;
    periphBusCfg = SPI2_BUS;
  }
#endif

  if (isSPIObject) {
    int pin_count = 0;
    bsp_io_port_pin_t spi_pins[3];
    for (int i=0; i<PINCOUNT_fn(); i++) {
      if (g_APinDescription[i].PeripheralConfig == periphBusCfg) {
        spi_pins[pin_count] = g_APinDescription[i].name;
        pin_count++;
      }
      if (pin_count == 3) break;
    }
    setPins(spi_pins[0], spi_pins[1], spi_pins[2]);
  }
  _cb_event_idx = _channel;

  if (_is_sci) {
    //Enable Isr from vector table for this SCI SPI channel
    enableSciSpiIrqs();
    _g_spi_ctrl = (spi_ctrl_t*)(SciTable[_channel].spi_instance->p_ctrl);
    _g_spi_cfg = (const spi_cfg_t *)(SciTable[_channel].spi_instance->p_cfg);
    for (int i=0; i<SPI_COUNT; i++) {
      //Adjust callback event index to be referred to I2CMasterTable table
      if ((spi_instance_t*)SciTable[_channel].spi_instance == &SpiTable[i]) {
        _cb_event_idx = i;
        break;
      }
    }
  } else {
    _g_spi_ctrl = (spi_ctrl_t*)(SpiTable[_channel].p_ctrl);
    _g_spi_cfg = (const spi_cfg_t *)(SpiTable[_channel].p_cfg);
  }

  _g_spi_ext_cfg = (const spi_extended_cfg_t *)(_g_spi_cfg->p_extend);

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
  if (_is_sci) {
    R_SCI_SPI_Close(_g_spi_ctrl);
  } else {
    R_SPI_Close(_g_spi_ctrl);
  }
}

void ArduinoSPI::setPins(int miso, int mosi, int sck, int cs) {
  setPins(digitalPinToBspPin(miso), digitalPinToBspPin(mosi), digitalPinToBspPin(sck), digitalPinToBspPin(cs));
}

void ArduinoSPI::setPins(bsp_io_port_pin_t miso, bsp_io_port_pin_t mosi,
                         bsp_io_port_pin_t sck, bsp_io_port_pin_t cs) {
  uint32_t peripheralCfg = 0;
  if (_is_sci) {
    if (_channel%2 == 0) {
      peripheralCfg = (uint32_t) IOPORT_PERIPHERAL_SCI0_2_4_6_8;
    } else {
      peripheralCfg = (uint32_t) IOPORT_PERIPHERAL_SCI1_3_5_7_9;
    }
  } else {
    peripheralCfg = (uint32_t) IOPORT_PERIPHERAL_SPI;
  }
  pinPeripheral(miso, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
  pinPeripheral(mosi, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
  pinPeripheral(sck, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | peripheralCfg);
}

void ArduinoSPI::usingInterrupt(int interruptNumber)
{
}

void ArduinoSPI::notUsingInterrupt(int interruptNumber)
{
}

uint8_t ArduinoSPI::transfer(uint8_t data) {
  uint8_t rxbuf;
  _spi_cb_event[_cb_event_idx] = SPI_EVENT_TRANSFER_ABORTED;
  if (_is_sci) {
    R_SCI_SPI_WriteRead(_g_spi_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_8_BITS);
  } else {
    R_SPI_WriteRead(_g_spi_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_8_BITS);
  }
  while ((SPI_EVENT_TRANSFER_COMPLETE != _spi_cb_event[_cb_event_idx]) && timeout_ms)
  {
      timeout_ms--;
      delay(1);
  }
  if (SPI_EVENT_TRANSFER_ABORTED == _spi_cb_event[_cb_event_idx])
  {
      end();
      return 0;
  }
  return rxbuf;
}

uint16_t ArduinoSPI::transfer16(uint16_t data) {
  uint16_t rxbuf;
  _spi_cb_event[_cb_event_idx] = SPI_EVENT_TRANSFER_ABORTED;
  if (_is_sci) {
    R_SCI_SPI_WriteRead(_g_spi_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_16_BITS);
  } else {
    R_SPI_WriteRead(_g_spi_ctrl, &data, &rxbuf, 1, SPI_BIT_WIDTH_16_BITS);
  }
  while ((SPI_EVENT_TRANSFER_COMPLETE != _spi_cb_event[_cb_event_idx]) && timeout_ms)
  {
      timeout_ms--;
      delay(1);
  }
  if (SPI_EVENT_TRANSFER_ABORTED == _spi_cb_event[_cb_event_idx])
  {
      end();
      return 0;
  }
  return rxbuf;
}

void ArduinoSPI::transfer(void *buf, size_t count) {
  _spi_cb_event[_cb_event_idx] = SPI_EVENT_TRANSFER_ABORTED;
  if (_is_sci) {
    R_SCI_SPI_WriteRead(_g_spi_ctrl, buf, buf, count, SPI_BIT_WIDTH_8_BITS);
  } else {
    R_SPI_WriteRead(_g_spi_ctrl, buf, buf, count, SPI_BIT_WIDTH_8_BITS);
  }
  while ((SPI_EVENT_TRANSFER_COMPLETE != _spi_cb_event[_cb_event_idx]) && timeout_ms)
  {
      timeout_ms--;
      delay(1);
  }
  if (SPI_EVENT_TRANSFER_ABORTED == _spi_cb_event[_cb_event_idx])
  {
      end();
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

void ArduinoSPI::enableSciSpiIrqs() {

  switch (_channel)
  {
  case 0:
#ifdef SCI0_RXI_IRQn
    __NVIC_SetVector(SCI0_RXI_IRQn, (uint32_t)sci_spi_rxi_isr);
    __NVIC_SetVector(SCI0_TXI_IRQn, (uint32_t)sci_spi_txi_isr);
    __NVIC_SetVector(SCI0_TEI_IRQn, (uint32_t)sci_spi_tei_isr);
#endif
    break;
  case 1:
#ifdef SCI1_RXI_IRQn
    __NVIC_SetVector(SCI1_RXI_IRQn, (uint32_t)sci_spi_rxi_isr);
    __NVIC_SetVector(SCI1_TXI_IRQn, (uint32_t)sci_spi_txi_isr);
    __NVIC_SetVector(SCI1_TEI_IRQn, (uint32_t)sci_spi_tei_isr);
#endif
    break;
  case 2:
#ifdef SCI2_RXI_IRQn
    __NVIC_SetVector(SCI2_RXI_IRQn, (uint32_t)sci_spi_rxi_isr);
    __NVIC_SetVector(SCI2_TXI_IRQn, (uint32_t)sci_spi_txi_isr);
    __NVIC_SetVector(SCI2_TEI_IRQn, (uint32_t)sci_spi_tei_isr);
#endif
    break;
  case 3:
#ifdef SCI3_RXI_IRQn
    __NVIC_SetVector(SCI3_RXI_IRQn, (uint32_t)sci_spi_rxi_isr);
    __NVIC_SetVector(SCI3_TXI_IRQn, (uint32_t)sci_spi_txi_isr);
    __NVIC_SetVector(SCI3_TEI_IRQn, (uint32_t)sci_spi_tei_isr);
#endif
    break;
  case 4:
#ifdef SCI4_RXI_IRQn
    __NVIC_SetVector(SCI4_RXI_IRQn, (uint32_t)sci_spi_rxi_isr);
    __NVIC_SetVector(SCI4_TXI_IRQn, (uint32_t)sci_spi_txi_isr);
    __NVIC_SetVector(SCI4_TEI_IRQn, (uint32_t)sci_spi_tei_isr);
#endif
    break;
  case 5:
#ifdef SCI5_RXI_IRQn
    __NVIC_SetVector(SCI5_RXI_IRQn, (uint32_t)sci_spi_rxi_isr);
    __NVIC_SetVector(SCI5_TXI_IRQn, (uint32_t)sci_spi_txi_isr);
    __NVIC_SetVector(SCI5_TEI_IRQn, (uint32_t)sci_spi_tei_isr);
#endif
    break;
  case 6:
#ifdef SCI6_RXI_IRQn
    __NVIC_SetVector(SCI6_RXI_IRQn, (uint32_t)sci_spi_rxi_isr);
    __NVIC_SetVector(SCI6_TXI_IRQn, (uint32_t)sci_spi_txi_isr);
    __NVIC_SetVector(SCI6_TEI_IRQn, (uint32_t)sci_spi_tei_isr);
#endif
    break;
  case 7:
#ifdef SCI7_RXI_IRQn
    __NVIC_SetVector(SCI7_RXI_IRQn, (uint32_t)sci_spi_rxi_isr);
    __NVIC_SetVector(SCI7_TXI_IRQn, (uint32_t)sci_spi_txi_isr);
    __NVIC_SetVector(SCI7_TEI_IRQn, (uint32_t)sci_spi_tei_isr);
#endif
    break;
  case 8:
#ifdef SCI8_RXI_IRQn
    __NVIC_SetVector(SCI8_RXI_IRQn, (uint32_t)sci_spi_rxi_isr);
    __NVIC_SetVector(SCI8_TXI_IRQn, (uint32_t)sci_spi_txi_isr);
    __NVIC_SetVector(SCI8_TEI_IRQn, (uint32_t)sci_spi_tei_isr);
#endif
    break;
  case 9:
#ifdef SCI9_RXI_IRQn
    __NVIC_SetVector(SCI9_RXI_IRQn, (uint32_t)sci_spi_rxi_isr);
    __NVIC_SetVector(SCI9_TXI_IRQn, (uint32_t)sci_spi_txi_isr);
    __NVIC_SetVector(SCI9_TEI_IRQn, (uint32_t)sci_spi_tei_isr);
#endif
    break;
  
  default:
    break;
  }

}

void spi_callback(spi_callback_args_t *p_args) {
  if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event) {
    _spi_cb_event[p_args->channel] = SPI_EVENT_TRANSFER_COMPLETE;
  }else
  {
    /* Updating the flag here to capture and handle all other error events */
    _spi_cb_event[p_args->channel] = SPI_EVENT_TRANSFER_ABORTED;
  }
}

void sci_spi_callback(spi_callback_args_t *p_args) {
  int spi_master_offset = SPI_COUNT - SCI_COUNT;
  if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event) {
    _spi_cb_event[p_args->channel + spi_master_offset] = SPI_EVENT_TRANSFER_COMPLETE;
  }else
  {
    /* Updating the flag here to capture and handle all other error events */
    _spi_cb_event[p_args->channel + spi_master_offset] = SPI_EVENT_TRANSFER_ABORTED;
  }
}

#if SPI_HOWMANY > 0
ArduinoSPI SPI(SPI_CHANNEL, (bool)IS_SPI_SCI);
#endif

#if SPI_HOWMANY > 1
ArduinoSPI SPI1(SPI1_CHANNEL, (bool)IS_SPI1_SCI);
#endif