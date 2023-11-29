/*
  TwoWire.cpp - TWI/I2C library for Wiring & Arduino
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
 
  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 2017 by Chuck Todd (ctodd@cableone.net) to correct Unconfigured Slave Mode reboot

  Version 2022 for Renesas RA4 by Daniele Aimo (d.aimo@arduino.cc)
*/

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include "Wire.h"

TwoWire *TwoWire::g_SCIWires[TWOWIRE_MAX_I2C_CHANNELS] = {nullptr};
TwoWire *TwoWire::g_I2CWires[TWOWIRE_MAX_SCI_CHANNELS] = {nullptr};

/* -------------------------------------------------------------------------- */
void TwoWire::setBusStatus(WireStatus_t ws) {
/* -------------------------------------------------------------------------- */  
  bus_status = ws;
}

/* -------------------------------------------------------------------------- */
void TwoWire::WireSCIMasterCallback(i2c_master_callback_args_t *arg) {
/* -------------------------------------------------------------------------- */
  /* +++++ MASTER I2C SCI Callback ++++++ */
  
  i2c_master_cfg_t *cfg = (i2c_master_cfg_t *)arg->p_context;

  TwoWire *ptr = nullptr; 
  if(cfg->channel < TWOWIRE_MAX_SCI_CHANNELS) {
    ptr = g_SCIWires[cfg->channel];
  }

  if(ptr == nullptr) {
    return;
  }
  if(!ptr->init_ok) {
    return;
  }

  if(ptr != nullptr) {  
    if(arg->event == I2C_MASTER_EVENT_ABORTED) {
      ptr->setBusStatus(WIRE_STATUS_TRANSACTION_ABORTED);
    }
    else if(arg->event == I2C_MASTER_EVENT_RX_COMPLETE) {
      ptr->setBusStatus(WIRE_STATUS_RX_COMPLETED);
    }
    else if(arg->event == I2C_MASTER_EVENT_TX_COMPLETE) {
      ptr->setBusStatus(WIRE_STATUS_TX_COMPLETED);
    }
  }

}

/* -------------------------------------------------------------------------- */
void TwoWire::WireMasterCallback(i2c_master_callback_args_t *arg) {
/* -------------------------------------------------------------------------- */  
  /* +++++ MASTER I2C not SCI Callback ++++++ */
  i2c_master_cfg_t *cfg = (i2c_master_cfg_t *)arg->p_context;

  TwoWire *ptr = nullptr; 
  if(cfg->channel < TWOWIRE_MAX_I2C_CHANNELS) {
    ptr = g_I2CWires[cfg->channel];
  }

  if(ptr == nullptr) {
    return;
  }
  if(!ptr->init_ok) {
    return;
  }

  if(ptr != nullptr) {  
    if(arg->event == I2C_MASTER_EVENT_ABORTED) {
      ptr->setBusStatus(WIRE_STATUS_TRANSACTION_ABORTED);
    }
    else if(arg->event == I2C_MASTER_EVENT_RX_COMPLETE) {
      ptr->setBusStatus(WIRE_STATUS_RX_COMPLETED);
    }
    else if(arg->event == I2C_MASTER_EVENT_TX_COMPLETE) {
      ptr->setBusStatus(WIRE_STATUS_TX_COMPLETED);
    }
  }
}

/* -------------------------------------------------------------------------- */
void TwoWire::WireSlaveCallback(i2c_slave_callback_args_t *arg) {
/* -------------------------------------------------------------------------- */
  /* +++++ SLAVE Callback ++++++ */
  volatile uint32_t bytes = arg->bytes;
  volatile i2c_slave_cfg_t *cfg = (i2c_slave_cfg_t *)arg->p_context;
  
  TwoWire *ptr = nullptr;
  if(cfg->channel < TWOWIRE_MAX_I2C_CHANNELS) {
    ptr = g_I2CWires[cfg->channel];
  }

  if(ptr == nullptr) {
    return;
  }
  if(!ptr->init_ok) {
    return;
  }

  if(arg->event == I2C_SLAVE_EVENT_ABORTED) {
    ptr->setBusStatus(WIRE_STATUS_TRANSACTION_ABORTED);
  }
  else if(arg->event == I2C_SLAVE_EVENT_RX_COMPLETE) {
    ptr->setBusStatus(WIRE_STATUS_RX_COMPLETED);
    
    ptr->cpy_rx_buffer(bytes);
    if(ptr->rx_callback != nullptr) {
      ptr->rx_callback(bytes);
    }
  }
  else if(arg->event == I2C_SLAVE_EVENT_TX_COMPLETE) {
    ptr->setBusStatus(WIRE_STATUS_TX_COMPLETED);
  }
  else if(arg->event == I2C_SLAVE_EVENT_RX_REQUEST) {
    ptr->setBusStatus(WIRE_STATUS_RX_REQUEST);
    if(ptr->tmp_i + bytes < I2C_BUFFER_LENGTH) {
      volatile fsp_err_t err;
      err = ptr->slave_read(1);
      if(err == FSP_SUCCESS) {
        ptr->tmp_i += 1;
      }
    }
    else {
      ptr->slave_read(0);
    }
  }
  else if(arg->event == I2C_SLAVE_EVENT_TX_REQUEST) {
    ptr->setBusStatus(WIRE_STATUS_TX_REQUEST);
    
    if(ptr->tx_callback != nullptr) {
      ptr->tx_callback();
    }
  }
  else if(arg->event == I2C_SLAVE_EVENT_RX_MORE_REQUEST) {
    ptr->setBusStatus(WIRE_STATUS_RX_REQUEST);
    
    if(ptr->tmp_i + bytes < I2C_BUFFER_LENGTH) {
      volatile fsp_err_t err;
      err = ptr->slave_read(bytes);
      if(err == FSP_SUCCESS) {
        ptr->tmp_i += bytes;
      }
    }
    else {
      ptr->slave_read(0);
    }
  }
  else if(arg->event == I2C_SLAVE_EVENT_TX_MORE_REQUEST) {
    ptr->setBusStatus(WIRE_STATUS_TX_REQUEST);
    if(ptr->tx_callback != nullptr) {
      ptr->tx_callback();
    }
  }
  else if(arg->event == I2C_SLAVE_EVENT_GENERAL_CALL) {
    ptr->setBusStatus(WIRE_STATUS_GENERAL_CALL);
  }
  
}


/* -------------------------------------------------------------------------- */
TwoWire::TwoWire(int scl, int sda, WireAddressMode_t am /*= ADDRESS_MODE_7_BITS*/, bool prefer_sci /*= false*/) :
  scl_pin(scl),
  sda_pin(sda), 
  init_ok(false),
  is_master(true),
  is_sci(false),
  address_mode(am),
  timeout(1000),
  transmission_begun(false),
  data_too_long(false),
  rx_index(0),
  tx_index(0),
  require_sci(prefer_sci) {
/* -------------------------------------------------------------------------- */    
      m_i2c_cfg.rxi_irq = FSP_INVALID_VECTOR;
      m_i2c_cfg.txi_irq = FSP_INVALID_VECTOR;
      m_i2c_cfg.tei_irq = FSP_INVALID_VECTOR;
      m_i2c_cfg.eri_irq = FSP_INVALID_VECTOR;

      s_i2c_cfg.rxi_irq = FSP_INVALID_VECTOR;
      s_i2c_cfg.txi_irq = FSP_INVALID_VECTOR;
      s_i2c_cfg.tei_irq = FSP_INVALID_VECTOR;
      s_i2c_cfg.tei_irq = FSP_INVALID_VECTOR;
}

/* -------------------------------------------------------------------------- */ 
bool TwoWire::cfg_pins(int max_index) {
/* -------------------------------------------------------------------------- */   
  /* verify index are good */
  if(scl_pin < 0 || sda_pin < 0 || scl_pin >= max_index || sda_pin >= max_index) {
    return false;
  }
  /* getting configuration from table */
  auto cfgs_scl = getPinCfgs(scl_pin, PIN_CFG_REQ_SCL);
  auto cfgs_sda = getPinCfgs(sda_pin, PIN_CFG_REQ_SDA);

  uint16_t cfg_scl = 0;
  uint16_t cfg_sda = 0;

  /* Find the best combination */
  for (size_t i = 0; i < cfgs_scl.size(); i++) {
    for (size_t j = 0; j < cfgs_sda.size(); j++) {
      if (cfgs_scl[i] && cfgs_sda[j] && (GET_CHANNEL(cfgs_scl[i]) == GET_CHANNEL(cfgs_sda[j])) && (IS_SCI(cfgs_scl[i]) == IS_SCI(cfgs_sda[j]))) {
        cfg_scl = cfgs_scl[i];
        cfg_sda = cfgs_sda[j];
        channel = GET_CHANNEL(cfg_scl);
        goto done;
      }
    }
  }

done:
  if (cfg_sda == 0 || cfg_scl == 0) {
    return false;
  }

  /* actually configuring PIN function */
  ioport_peripheral_t ioport_sda;
  ioport_peripheral_t ioport_scl;
  
  if(IS_SCI(cfg_sda)) {
    if(channel >= TWOWIRE_MAX_SCI_CHANNELS) { // channels are 0 index based
      return false;
    }
    is_sci = true;
    ioport_sda = USE_SCI_EVEN_CFG(cfg_sda) ? IOPORT_PERIPHERAL_SCI0_2_4_6_8 : IOPORT_PERIPHERAL_SCI1_3_5_7_9;
    ioport_scl = USE_SCI_EVEN_CFG(cfg_scl) ? IOPORT_PERIPHERAL_SCI0_2_4_6_8 : IOPORT_PERIPHERAL_SCI1_3_5_7_9;
  
    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[sda_pin].pin, (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_PERIPHERAL_PIN | ioport_sda));
    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[scl_pin].pin, (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_PERIPHERAL_PIN | ioport_scl));
  
  }
  else {
    if(channel >= TWOWIRE_MAX_I2C_CHANNELS) { // channels are 0 index based
      return false;
    }
    is_sci = false;
    ioport_sda = IOPORT_PERIPHERAL_IIC;
    ioport_scl = IOPORT_PERIPHERAL_IIC;

    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[sda_pin].pin, (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PERIPHERAL_PIN | ioport_sda));
    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[scl_pin].pin, (uint32_t) (IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_DRIVE_MID | IOPORT_CFG_PERIPHERAL_PIN | ioport_scl));
  }

  return true;
}

/* -------------------------------------------------------------------------- */
void TwoWire::begin(void) {
/* -------------------------------------------------------------------------- */  
  end();
  is_master = true;
  _begin();

}


/* -------------------------------------------------------------------------- */
void TwoWire::_begin(void) {
/* -------------------------------------------------------------------------- */  
  init_ok = true;
  int max_index = PINS_COUNT;

  init_ok &= cfg_pins(max_index);

  if(init_ok) {

    /* -----------------------------------
        ->>>>>  MASTER initialization
     * ----------------------------------- */
    if(is_master) {

      setClock(I2C_MASTER_RATE_STANDARD);

      if(is_sci) {
        TwoWire::g_SCIWires[channel]          = this;
  
        m_open                                = R_SCI_I2C_Open;
        m_read                                = R_SCI_I2C_Read;
        m_write                               = R_SCI_I2C_Write;
        m_abort                               = R_SCI_I2C_Abort;
        m_setSlaveAdd                         = R_SCI_I2C_SlaveAddressSet;
        m_setCallback                         = R_SCI_I2C_CallbackSet;
        m_getStatus                           = R_SCI_I2C_StatusGet;
        m_close                               = R_SCI_I2C_Close;

        m_i2c_cfg.p_extend                    = &m_sci_i2c_extend;
        m_i2c_cfg.p_callback                  = WireSCIMasterCallback;
      }
      else {
        TwoWire::g_I2CWires[channel]          = this; 

        m_open                                = R_IIC_MASTER_Open;
        m_read                                = R_IIC_MASTER_Read;
        m_write                               = R_IIC_MASTER_Write;
        m_abort                               = R_IIC_MASTER_Abort;
        m_setSlaveAdd                         = R_IIC_MASTER_SlaveAddressSet;
        m_setCallback                         = R_IIC_MASTER_CallbackSet;
        m_getStatus                           = R_IIC_MASTER_StatusGet;
        m_close                               = R_IIC_MASTER_Close;

        m_i2c_cfg.p_extend                    = &m_i2c_extend;
        m_i2c_cfg.p_callback                  = WireMasterCallback;

        m_i2c_extend.timeout_mode             = IIC_MASTER_TIMEOUT_MODE_SHORT;
        m_i2c_extend.timeout_scl_low          = IIC_MASTER_TIMEOUT_SCL_LOW_DISABLED;
      }

      m_i2c_cfg.channel                     = channel;
      m_i2c_cfg.rate                        = I2C_MASTER_RATE_STANDARD;
      m_i2c_cfg.slave                       = 0x00;
      m_i2c_cfg.addr_mode                   = (address_mode == ADDRESS_MODE_7_BITS) ? I2C_MASTER_ADDR_MODE_7BIT : I2C_MASTER_ADDR_MODE_10BIT;
      m_i2c_cfg.p_transfer_tx               = NULL;
      m_i2c_cfg.p_transfer_rx               = NULL;

      m_i2c_cfg.p_context                   = &m_i2c_cfg;
      m_i2c_cfg.ipl                         = (12);

    } // if(is_master) {
    /* -----------------------------------
        ->>>>>  SLAVE initialization
     * ----------------------------------- */
    else {
      /* a slave device cannot be instatiated on SCI peripheral */
      if(is_sci) {
        init_ok = false;
        return;
      }
      TwoWire::g_I2CWires[channel]      = this;

      s_open                            = R_IIC_SLAVE_Open;
      s_read                            = R_IIC_SLAVE_Read;
      s_write                           = R_IIC_SLAVE_Write;
      s_setCallback                     = R_IIC_SLAVE_CallbackSet;
      s_close                           = R_IIC_SLAVE_Close;
      
      s_i2c_cfg.channel                 = channel;
      s_i2c_cfg.rate                    = I2C_SLAVE_RATE_STANDARD;
      s_i2c_cfg.slave                   = slave_address;
      s_i2c_cfg.addr_mode               = (address_mode == ADDRESS_MODE_7_BITS) ? I2C_SLAVE_ADDR_MODE_7BIT : I2C_SLAVE_ADDR_MODE_10BIT;
      s_i2c_cfg.general_call_enable     = false;
      s_i2c_cfg.ipl                     = (12);
      s_i2c_cfg.eri_ipl                 = (12);
      s_i2c_cfg.clock_stretching_enable = false;
      s_i2c_cfg.p_callback              = WireSlaveCallback;
      s_i2c_cfg.p_context               = &s_i2c_cfg;
      s_i2c_cfg.p_extend                = NULL;
    }
  }
  else {
    init_ok = false;
    return;
  }

  I2CIrqReq_t irq_req;
  irq_req.mcfg = &m_i2c_cfg; 
  irq_req.scfg = &s_i2c_cfg;
  
  if(is_master) {    
      if(is_sci) {
        init_ok &= IRQManager::getInstance().addPeripheral(IRQ_SCI_I2C_MASTER,&irq_req);
      }
      else {
        init_ok &= IRQManager::getInstance().addPeripheral(IRQ_I2C_MASTER,&irq_req);
      }
      if(FSP_SUCCESS == m_open(&m_i2c_ctrl,&m_i2c_cfg)) {
         init_ok &= true;
      }
      else {
         init_ok = false;
      }
  }
  else {
      init_ok &= IRQManager::getInstance().addPeripheral(IRQ_I2C_SLAVE,&irq_req);
      if(FSP_SUCCESS == s_open(&s_i2c_ctrl,&s_i2c_cfg)) {
         init_ok &= true;
      }
      else {
         init_ok = false;
      }
  }
}

/* -------------------------------------------------------------------------- */
void TwoWire::begin(uint16_t address) {
/* -------------------------------------------------------------------------- */  
  end();
  is_master = false;
  slave_address = address;
  /* Address is set inside begin() using slave_address member variable */
  _begin();
  
}

/* -------------------------------------------------------------------------- */
void TwoWire::begin(int address) {
/* -------------------------------------------------------------------------- */  
  begin((uint16_t)address);
}

/* -------------------------------------------------------------------------- */
void TwoWire::begin(uint8_t address) {
/* -------------------------------------------------------------------------- */  
  begin((uint16_t)address);
}

/* -------------------------------------------------------------------------- */
void TwoWire::end(void) {
/* -------------------------------------------------------------------------- */  

  if(init_ok) {
    if(is_master) {
      if(m_close != nullptr) {
        m_close(&m_i2c_ctrl);
        R_BSP_IrqDisable (m_i2c_cfg.txi_irq);
        R_BSP_IrqDisable (m_i2c_cfg.rxi_irq);
        R_BSP_IrqDisable (m_i2c_cfg.tei_irq);
        R_BSP_IrqDisable (m_i2c_cfg.eri_irq);
          
      }
    }
    else {
      if(s_close != nullptr) {
        s_close(&s_i2c_ctrl);
        R_BSP_IrqDisable (s_i2c_cfg.txi_irq);
        R_BSP_IrqDisable (s_i2c_cfg.rxi_irq);
        R_BSP_IrqDisable (s_i2c_cfg.tei_irq);
        R_BSP_IrqDisable (s_i2c_cfg.eri_irq);
        
      }
    }
  }
  init_ok = false;
}



/* -------------------------------------------------------------------------- */
uint8_t TwoWire::read_from(uint8_t address, uint8_t* data, uint8_t length, unsigned int timeout_ms, bool sendStop) {
/* -------------------------------------------------------------------------- */  
  /* ??? does this function make sense only for MASTER ???? */
  
  fsp_err_t err = FSP_ERR_ASSERTION;
  if(init_ok) {
    if(m_setSlaveAdd != nullptr) {
      err = m_setSlaveAdd(&m_i2c_ctrl, address, m_i2c_cfg.addr_mode);
    }
    if(err == FSP_SUCCESS) {
      if(m_read != nullptr) {
        bus_status = WIRE_STATUS_UNSET;
        err = m_read(&m_i2c_ctrl,data,length,!sendStop);
      }
    }
    uint32_t const start = millis();
    while(((millis() - start) < timeout_ms) && bus_status == WIRE_STATUS_UNSET && err == FSP_SUCCESS) {

    }
  }

  if(bus_status == WIRE_STATUS_RX_COMPLETED) {
    return length;
  }
  
  return 0; /* ???????? return value ??????? */
}

/* -------------------------------------------------------------------------- */    
uint8_t TwoWire::write_to(uint8_t address, uint8_t* data, uint8_t length, unsigned int timeout_ms, bool sendStop) {
/* -------------------------------------------------------------------------- */  
  uint8_t rv = END_TX_OK;
  fsp_err_t err = FSP_ERR_ASSERTION;
  if(init_ok) {
    if(m_setSlaveAdd != nullptr) {
      err = m_setSlaveAdd(&m_i2c_ctrl, address, m_i2c_cfg.addr_mode);
    }
    if(err == FSP_SUCCESS) {
      if(m_write != nullptr) {
        bus_status = WIRE_STATUS_UNSET;
        err = m_write(&m_i2c_ctrl,data,length,!sendStop);
      }
    }
    uint32_t const start = millis();
    while(((millis() - start) < timeout_ms) && bus_status == WIRE_STATUS_UNSET && err == FSP_SUCCESS) {

    }

    if(err != FSP_SUCCESS) {
      rv = END_TX_ERR_FSP;
    }
    else if(data_too_long) {
      rv = END_TX_DATA_TOO_LONG;
    }
    else if(bus_status == WIRE_STATUS_UNSET) {
      rv = END_TX_TIMEOUT;
    }
    /* as far as I know is impossible to distinguish between NACK on ADDRESS and
      NACK on DATA */
    else if(bus_status == WIRE_STATUS_TRANSACTION_ABORTED) {
      rv = END_TX_NACK_ON_ADD;
    }
  }
  else {
    rv = END_TX_NOT_INIT;
  }

  
  return rv;
}

/* -------------------------------------------------------------------------- */    
void TwoWire::setClock(uint32_t freq) {
/* -------------------------------------------------------------------------- */      
  if(init_ok && is_master) {
    if(m_close != nullptr) {
      m_close(&m_i2c_ctrl); 
    }
  }

  if(is_master) {
    m_i2c_cfg.rate = (i2c_master_rate_t)freq;

    int clock_divisor = (R_FSP_SystemClockHzGet(BSP_FEATURE_SCI_CLOCK) / 48000000u) - 1;

    if (is_sci) {
      m_sci_i2c_extend.clock_settings.clk_divisor_value = 0;
      m_sci_i2c_extend.clock_settings.cycles_value = 15;
      m_sci_i2c_extend.clock_settings.snfr_value = (1);
      switch (m_i2c_cfg.rate) {
        case I2C_MASTER_RATE_STANDARD:
          m_sci_i2c_extend.clock_settings.brr_value = 14;
          m_sci_i2c_extend.clock_settings.mddr_value = 255;
          m_sci_i2c_extend.clock_settings.bitrate_modulation = false;
          break;
        case I2C_MASTER_RATE_FAST:
        default:
          m_sci_i2c_extend.clock_settings.brr_value = 2;
          m_sci_i2c_extend.clock_settings.mddr_value = 204;
          m_sci_i2c_extend.clock_settings.bitrate_modulation = true;
          break;
        }
    } else {
      switch (m_i2c_cfg.rate) {
        case I2C_MASTER_RATE_STANDARD:
          m_i2c_extend.clock_settings.brl_value = 27;
          m_i2c_extend.clock_settings.brh_value = 26;
          m_i2c_extend.clock_settings.cks_value = 2 + clock_divisor;
          break;
        case I2C_MASTER_RATE_FAST:
          m_i2c_extend.clock_settings.brl_value = 16;
          m_i2c_extend.clock_settings.brh_value = 15;
          m_i2c_extend.clock_settings.cks_value = 0 + clock_divisor;
          break;
#if BSP_FEATURE_IIC_FAST_MODE_PLUS
        case I2C_MASTER_RATE_FASTPLUS:
          m_i2c_extend.clock_settings.brl_value = 6;
          m_i2c_extend.clock_settings.brh_value = 5;
          m_i2c_extend.clock_settings.cks_value = 0;
          break;
#endif
      }
    }
  }

  if(init_ok) {
    if(m_open != nullptr) {
      if(FSP_SUCCESS == m_open(&m_i2c_ctrl,&m_i2c_cfg)) {
          init_ok &= true;
      }
      else {
          init_ok = false;
      }
    }
  }
}

/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *                           TRANSMISSION BEGIN
 *  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* -------------------------------------------------------------------------- */
void TwoWire::beginTransmission(uint32_t address) {
/* -------------------------------------------------------------------------- */  
  if (init_ok) {
    data_too_long = false;
    master_tx_address = address;
    transmission_begun = true;
    tx_index = 0;
  }
}

/* -------------------------------------------------------------------------- */
void TwoWire::beginTransmission(uint16_t address) {
/* -------------------------------------------------------------------------- */
  beginTransmission((uint32_t)address);
}

/* -------------------------------------------------------------------------- */
void TwoWire::beginTransmission(uint8_t address){
/* -------------------------------------------------------------------------- */  
  beginTransmission((uint32_t)address);
}

/* -------------------------------------------------------------------------- */
void TwoWire::beginTransmission(int address) {
/* -------------------------------------------------------------------------- */  
  beginTransmission((uint32_t)address);
}

/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *                            TRANSMISSION END
 *  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* -------------------------------------------------------------------------- */
uint8_t TwoWire::endTransmission(bool sendStop) {
/* -------------------------------------------------------------------------- */  
  uint8_t ret = write_to(master_tx_address, tx_buffer, tx_index, timeout, sendStop);
  transmission_begun = false;
  return ret;
}

/* -------------------------------------------------------------------------- */
uint8_t TwoWire::endTransmission(void) {
/* -------------------------------------------------------------------------- */  
  return endTransmission(true);
}

/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *                             REQUEST FROM
 *  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* -------------------------------------------------------------------------- */
size_t TwoWire::requestFrom(uint8_t address, size_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop) {
/* -------------------------------------------------------------------------- */  
  if(init_ok) {
  
    if (isize > 0) {
      // send internal address; this mode allows sending a repeated start to access
      // some devices' internal registers. This function is executed by the hardware
      // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

      beginTransmission(address);

      // the maximum size of internal address is 3 bytes
      if (isize > 3){
        isize = 3;
      }

      // write internal register address - most significant byte first
      while (isize-- > 0) {
        write((uint8_t)(iaddress >> (isize*8)));
      }

      endTransmission(false);
    }

    // clamp to buffer length
    if(quantity > I2C_BUFFER_LENGTH){
      quantity = I2C_BUFFER_LENGTH;
    }
    // perform blocking read into buffer
    uint8_t read = read_from(address, rx_buffer, quantity, timeout, sendStop);
    // set rx buffer iterator vars
    rx_index = read;
    rx_extract_index = 0;

    return (size_t)read;
  }
  else {
    return 0;
  }
}

/* -------------------------------------------------------------------------- */
size_t TwoWire::requestFrom(uint8_t address, size_t quantity, bool sendStop) {
/* -------------------------------------------------------------------------- */  
	return requestFrom((uint8_t)address, quantity, (uint32_t)0, (uint8_t)0, sendStop);
}

/* -------------------------------------------------------------------------- */
size_t TwoWire::requestFrom(uint8_t address, size_t quantity) {
/* -------------------------------------------------------------------------- */  
  return requestFrom((uint8_t)address, quantity, true);
}

/*  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *                                 WRITE
 *  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
/* -------------------------------------------------------------------------- */
size_t TwoWire::write(uint8_t data) {
/* -------------------------------------------------------------------------- */  
  if(init_ok) {
    if(is_master) {
      if(transmission_begun) {
        if(tx_index >= I2C_BUFFER_LENGTH) {
          data_too_long = true;
          setWriteError();
          return 0;
        }
        tx_buffer[tx_index] = data;
        tx_index++;
      }
    }
    else {
      if(s_write != nullptr) {
        s_write(&s_i2c_ctrl,(uint8_t *)&data,1);
      }
    }
    return 1;
  }
  return 0;
  
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
/* -------------------------------------------------------------------------- */
size_t TwoWire::write(const uint8_t *data, size_t quantity) {
/* -------------------------------------------------------------------------- */  
  if(init_ok) {
    if(is_master) {
    // in master transmitter mode
      for(size_t i = 0; i < quantity; ++i){
        write(data[i]);
      }
    }
    else{
      if(s_write != nullptr) {
        s_write(&s_i2c_ctrl,(uint8_t *)data,quantity);
      }
    }
    return quantity;
  }
  else {
    return 0;
  }
}


// sets function called on slave write
/* -------------------------------------------------------------------------- */
void TwoWire::onReceive( I2C_onRxCallback_f f ) {
/* -------------------------------------------------------------------------- */  
  rx_callback = f;
}

// sets function called on slave read
/* -------------------------------------------------------------------------- */
void TwoWire::onRequest( I2C_onTxCallback_f f ) {
/* -------------------------------------------------------------------------- */  
  tx_callback = f;
}



// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)

/* -------------------------------------------------------------------------- */
int TwoWire::available(void) {
/* -------------------------------------------------------------------------- */  
  return rx_index - rx_extract_index;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
/* -------------------------------------------------------------------------- */
int TwoWire::read(void) {
/* -------------------------------------------------------------------------- */  
  int rv = -1;
  
  // get each successive byte on each call
  if(rx_extract_index < rx_index){
    rv = rx_buffer[rx_extract_index];
    rx_extract_index++;
  }

  return rv;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
/* -------------------------------------------------------------------------- */
int TwoWire::peek(void) {
/* -------------------------------------------------------------------------- */  
  int rv = -1;
  
  // get each successive byte on each call
  if(rx_extract_index < rx_index){
    rv = rx_buffer[rx_extract_index];
  }

  return rv;
}

/* -------------------------------------------------------------------------- */
void TwoWire::flush(void) {
/* -------------------------------------------------------------------------- */  
  while(bus_status != WIRE_STATUS_TX_COMPLETED && bus_status != WIRE_STATUS_TRANSACTION_ABORTED) {}
}




#if WIRE_HOWMANY > 0
TwoWire Wire(WIRE_SCL_PIN, WIRE_SDA_PIN);
#endif

#if WIRE_HOWMANY > 1
TwoWire Wire1(WIRE1_SCL_PIN, WIRE1_SDA_PIN);
#endif

#if WIRE_HOWMANY > 2
TwoWire Wire2(WIRE2_SCL_PIN, WIRE2_SDA_PIN);
#endif

#if WIRE_HOWMANY > 3
TwoWire Wire3(WIRE3_SCL_PIN, WIRE3_SDA_PIN);
#endif


