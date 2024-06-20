/*
  Serial.cpp - wrapper over mbed RawSerial
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2018-2019 Arduino SA

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "Arduino.h"
#include "Serial.h"
#include "IRQManager.h"


#ifdef Serial
#undef Serial
#endif


UART * UART::g_uarts[MAX_UARTS] = {nullptr};

void uart_callback(uart_callback_args_t __attribute((unused)) *p_args)
{
    /* This callback function is not used but it is referenced into 
       FSP configuration so that (for the moment it is necessary to keep it) */
}

/* -------------------------------------------------------------------------- */
void UART::WrapperCallback(uart_callback_args_t *p_args) {
/* -------------------------------------------------------------------------- */  

  uint32_t channel = p_args->channel;
  
  UART *uart_ptr = UART::g_uarts[channel];

  if(uart_ptr == nullptr) {
    return;
  }
  

  switch (p_args->event){
      case UART_EVENT_ERR_PARITY:
      case UART_EVENT_ERR_FRAMING:
      case UART_EVENT_ERR_OVERFLOW:
      case UART_EVENT_RX_COMPLETE: // This is called when all the "expected" data are received
      {
        //digitalWrite(DEBUG_PIN_CALLBACK_ERROR, HIGH);
        //digitalWrite(DEBUG_PIN_CALLBACK_ERROR, LOW);
          break;
      }
      case UART_EVENT_TX_DATA_EMPTY:
      {
        //digitalWrite(DEBUG_PIN_CALLBACK_TXDE, HIGH);
        if (uart_ptr->txBuffer.available() == 0) {
          uart_ptr->tx_fsi_state = TX_FSI_WAITING_TE;  // maybe...
          uart_ptr->save_tx_info(0x40); // top nibble bits 0100
        } else {
          size_t cb = 0;
          while (cb < sizeof(tx_fsi_buffer)) {
            int ch = uart_ptr->txBuffer.read_char();
            if (ch == -1) break;
            uart_ptr->tx_fsi_buffer[cb++] = ch;            
          }
          uart_ptr->tx_fsi_state = TX_FSI_ACTIVE;
          //digitalToggle(DEBUG_PIN_WRITE_TOGGLE);
          // See if we can simply stuff out the new buffer and count
          // Maybe special case if we get here and TDR is already empty
          // maybe different test for FIFO.
          if (uart_ptr->uart_ctrl.p_reg->SSR_b.TDRE) {
            uart_ptr->save_tx_info(cb | 0xA0); // top nibble bits 1010
            R_SCI_UART_Write(&(uart_ptr->uart_ctrl), uart_ptr->tx_fsi_buffer, cb);
          } else {
            uart_ptr->save_tx_info(cb | 0x60); // top nibble bits 0110

            uart_ptr->uart_ctrl.tx_src_bytes = cb;
            uart_ptr->uart_ctrl.p_tx_src     =  uart_ptr->tx_fsi_buffer;
  
            // and reenable the TIE and not TEIE
            uart_ptr->uart_ctrl.p_reg->SCR &= (uint8_t) ~(R_SCI0_SCR_TEIE_Msk); // don't wait on transer end interrupt.
            uart_ptr->uart_ctrl.p_reg->SCR |= (uint8_t)R_SCI0_SCR_TIE_Msk;
          }
        }
        //digitalWrite(DEBUG_PIN_CALLBACK_TXDE, LOW);
        break;
      }
      case UART_EVENT_TX_COMPLETE:
      {
        // 
        //digitalWrite(DEBUG_PIN_CALLBACK_TE, HIGH);
        uart_ptr->tx_fsi_state = TX_FSI_COMPLETE;
        //digitalWrite(DEBUG_PIN_CALLBACK_TE, LOW);
        uart_ptr->save_tx_info(0x80); // top nibble bits 0110
        break;
      }
      case UART_EVENT_RX_CHAR:
      {

        //digitalWrite(DEBUG_PIN_CALLBACK_RX, HIGH);
        if (uart_ptr->rxBuffer.availableForStore()) {
          uart_ptr->rxBuffer.store_char(p_args->data);
        }
        //digitalWrite(DEBUG_PIN_CALLBACK_RX, LOW);
        break;
      }
      case UART_EVENT_BREAK_DETECT:
      {
          break;
      }
  }

}

/* -------------------------------------------------------------------------- */
void UART::WrapperCallbackFIFO(uart_callback_args_t *p_args) {
/* -------------------------------------------------------------------------- */  

  uint32_t channel = p_args->channel;
  
  UART *uart_ptr = UART::g_uarts[channel];

  if(uart_ptr == nullptr) {
    return;
  }
  

  switch (p_args->event){
      case UART_EVENT_ERR_PARITY:
      case UART_EVENT_ERR_FRAMING:
      case UART_EVENT_ERR_OVERFLOW:
      case UART_EVENT_RX_COMPLETE: // This is called when all the "expected" data are received
      {
        //digitalWrite(DEBUG_PIN_CALLBACK_ERROR, HIGH);
        //digitalWrite(DEBUG_PIN_CALLBACK_ERROR, LOW);
          break;
      }
      case UART_EVENT_TX_DATA_EMPTY:
      {
        //digitalWrite(DEBUG_PIN_CALLBACK_TXDE, HIGH);
        if (uart_ptr->txBuffer.available() == 0) {
          uart_ptr->tx_fsi_state = TX_FSI_WAITING_TE;  // maybe...
          uart_ptr->save_tx_info(0x40); // top nibble bits 0100
        } else {
          size_t cb = 0;
          while (cb < sizeof(tx_fsi_buffer)) {
            int ch = uart_ptr->txBuffer.read_char();
            if (ch == -1) break;
            uart_ptr->tx_fsi_buffer[cb++] = ch;            
          }
          uart_ptr->tx_fsi_state = TX_FSI_ACTIVE;
          //digitalToggle(DEBUG_PIN_WRITE_TOGGLE);
          // we are losing chars.  See if the UART is full, before and see if waiting helps
          // two cases with or without fifo.
          // but don't see any way we can get here without...
          // See if we can simply stuff out the new buffer and count
          // Maybe special case if we get here and TDR is already empty
          // maybe different test for FIFO.
          if (((uart_ptr->uart_ctrl.fifo_depth == 0) && (uart_ptr->uart_ctrl.p_reg->SSR_b.TDRE)) 
              /*|| ((uart_ptr->uart_ctrl.fifo_depth > 0) && (uart_ptr->uart_ctrl.p_reg->FDR_b.T ==0))*/) {
            uart_ptr->save_tx_info(cb | 0xA0); // top nibble bits 1010
            R_SCI_UART_Write(&(uart_ptr->uart_ctrl), uart_ptr->tx_fsi_buffer, cb);
          } else {
            uart_ptr->save_tx_info(cb | 0x60); // top nibble bits 0110

            uart_ptr->uart_ctrl.tx_src_bytes = cb;
            uart_ptr->uart_ctrl.p_tx_src     =  uart_ptr->tx_fsi_buffer;
  
            // and reenable the TIE and not TEIE
            uart_ptr->uart_ctrl.p_reg->SCR &= (uint8_t) ~(R_SCI0_SCR_TEIE_Msk); // don't wait on transfer end interrupt.
            uart_ptr->uart_ctrl.p_reg->SCR |= (uint8_t)R_SCI0_SCR_TIE_Msk;
          }

        }
        //digitalWrite(DEBUG_PIN_CALLBACK_TXDE, LOW);
        break;
      }
      case UART_EVENT_TX_COMPLETE:
      {
        // 
        //digitalWrite(DEBUG_PIN_CALLBACK_TE, HIGH);
        uart_ptr->tx_fsi_state = TX_FSI_COMPLETE;
        //digitalWrite(DEBUG_PIN_CALLBACK_TE, LOW);
        uart_ptr->save_tx_info(0x80); // top nibble bits 0110
        break;
      }
      case UART_EVENT_RX_CHAR:
      {
        // See if we can minimize the callbacks, by checking ourself if there is
        // more data in the FIFO...
        // Don't look to see if there is room, as the store_char does the same
        // and in either case we toss it anyway
        //digitalWrite(DEBUG_PIN_CALLBACK_RX, HIGH);
        uart_ptr->rxBuffer.store_char(p_args->data);
        //digitalWrite(DEBUG_PIN_CALLBACK_RX, LOW);

        while (uart_ptr->uart_ctrl.p_reg->FDR_b.R > 0U) {
          //digitalWrite(DEBUG_PIN_CALLBACK_RX, HIGH);
          uart_ptr->rxBuffer.store_char( uart_ptr->uart_ctrl.p_reg->FRDRHL & 0xff);
          //digitalWrite(DEBUG_PIN_CALLBACK_RX, LOW);
        }

        break;
      }
      case UART_EVENT_BREAK_DETECT:
      {
          break;
      }
  }

}


UART::UART(int _pin_tx, int _pin_rx, int _pin_rts, int _pin_cts):
  tx_pin(_pin_tx),
  rx_pin(_pin_rx),
  rts_pin(_pin_rts),
  cts_pin(_pin_cts),
  init_ok(false) {
/* -------------------------------------------------------------------------- */    
  uart_cfg.txi_irq = FSP_INVALID_VECTOR;
  uart_cfg.tei_irq = FSP_INVALID_VECTOR;
  uart_cfg.rxi_irq = FSP_INVALID_VECTOR;
  uart_cfg.eri_irq = FSP_INVALID_VECTOR;
}

/* -------------------------------------------------------------------------- */
bool UART::setUpUartIrqs(uart_cfg_t &cfg) {
/* -------------------------------------------------------------------------- */  
  bool rv = false;

  rv = IRQManager::getInstance().addPeripheral(IRQ_SCI_UART,&cfg);
  
  return rv;
} 

/* -------------------------------------------------------------------------- */
size_t UART::write(uint8_t c) {
/* -------------------------------------------------------------------------- */  
  return write(&c, 1);
}

size_t  UART::write(const uint8_t* c, size_t len) {
  if(init_ok && (len > 0)) {
    //digitalWrite(DEBUG_PIN_WRITE_CALL, HIGH);
    size_t cb_left = len;

    // If there is no transfer active, try to bypass putting stuff into
    // the txBuffer, just to pull it out again, for up to the size
    // of our temporary buffer.
    if (tx_fsi_state == TX_FSI_COMPLETE) {
      size_t cb_copy = len;
      if (cb_copy > sizeof(tx_fsi_buffer)) cb_copy = sizeof(tx_fsi_buffer);
      memcpy(tx_fsi_buffer, c, cb_copy);
      cb_left -= cb_copy;
      c += cb_copy;
      tx_fsi_state = TX_FSI_ACTIVE;
      //digitalToggle(DEBUG_PIN_WRITE_TOGGLE);
      save_tx_info(cb_copy); // top nibble bits 0010
      R_SCI_UART_Write(&(uart_ctrl), tx_fsi_buffer, cb_copy);
    }

    while (cb_left) {
      // Put as much of data into txBuffer as will fit
      while(cb_left && !txBuffer.isFull()) {
        txBuffer.store_char(*c++);
        cb_left--;
      }

      // was active, see if we can always make the callback handle setting up the next transfer
#if 1
      if (tx_fsi_state != TX_FSI_ACTIVE) {
        // Does not have an active buffer so we need to send a new one.
        // need to setup the next buffer... Unsure yet how much we should do.
        uint16_t cb_copy = txBuffer.available();
        if (cb_copy > sizeof(tx_fsi_buffer)) cb_copy = sizeof(tx_fsi_buffer);
        for (uint16_t i = 0; i < cb_copy; i++) tx_fsi_buffer[i] = txBuffer.read_char();
        tx_fsi_state = TX_FSI_ACTIVE;
        //digitalToggle(DEBUG_PIN_WRITE_TOGGLE);

        // we were losing chars.  See if the UART is full, before and see if waiting helps
        // two cases with or without fifo.
        // but don't see any way we can get here without...
        uint32_t start_time = micros();
        if (uart_ctrl.fifo_depth) {
          // Hopefully they don't completely fill the FIFO before calling us.
        } else {
          // Uart does not have FIFO so check TDRE..
          while ((((uint32_t)micros() - start_time) < 250) && !uart_ctrl.p_reg->SSR_b.TDRE) {
            //digitalToggle(DEBUG_PIN_WRITE_TOGGLE);
          }
        }
        save_tx_info(cb_copy | 0x20); // top nibble bits 0010
        R_SCI_UART_Write(&(uart_ctrl), tx_fsi_buffer, cb_copy);
      }
#else
      synchronized {
        do_R_SCI_UART_Write = txBuffer.available() && (tx_fsi_state != TX_FSI_ACTIVE);
        if (txBuffer.available()) {
          tx_fsi_state = TX_FSI_ACTIVE;

          // if the TIE is enabled already, nothing to do.
          if ((uart_ctrl.p_reg->SCR & R_SCI0_SCR_TIE_Msk) == 0) {
            // probably in the case of either not active or waiting for TEIE so reset to
            // looking for TIE
            uart_ctrl.p_reg->SCR &= (uint8_t) ~(R_SCI0_SCR_TEIE_Msk); // don't wait on transer end interrupt.
            uart_ctrl.p_reg->SCR |= (uint8_t)R_SCI0_SCR_TIE_Msk;
          }            
        }
      }
#endif    
      //digitalToggle(DEBUG_PIN_WRITE_TOGGLE);
    }
    //digitalWrite(DEBUG_PIN_WRITE_CALL, LOW);
    return len;
  }
  else {
    return 0;
  }
}

/* -------------------------------------------------------------------------- */
UART::operator bool() {
/* -------------------------------------------------------------------------- */  
	return true;
}

/* -------------------------------------------------------------------------- */
bool  UART::cfg_pins(int max_index) {
/* -------------------------------------------------------------------------- */  
  /* verify index are good */
  if(tx_pin < 0 || rx_pin < 0 || tx_pin >= max_index || rx_pin >= max_index) {
    return false;
  }
  /* getting configuration from table */
  auto cfgs_tx = getPinCfgs(tx_pin, PIN_CFG_REQ_UART_TX);
  auto cfgs_rx = getPinCfgs(rx_pin, PIN_CFG_REQ_UART_RX);

  uint16_t cfg_tx = 0;
  uint16_t cfg_rx = 0;

  /* Find the best combination */
  for (size_t i = 0; i < cfgs_tx.size(); i++) {
    for (size_t j = 0; j < cfgs_rx.size(); j++) {
      if (cfgs_tx[i] && cfgs_rx[i] && GET_CHANNEL(cfgs_tx[i]) == GET_CHANNEL(cfgs_rx[j])) {
        cfg_tx = cfgs_tx[i];
        cfg_rx = cfgs_rx[j];
        channel = GET_CHANNEL(cfg_tx);
        goto done;
      }
    }
  }

done:
  /* verify configuration are good */
  if(cfg_tx == 0 || cfg_rx == 0 ) {
    return false;
  }

  /* verify channel does not exceed max possible uart channels */
  if(channel >= MAX_UARTS) {
    return false;
  }
  /* setting channel */
  channel = GET_CHANNEL(cfg_tx);
  
  /* actually configuring PIN function */
  ioport_peripheral_t ioport_tx = USE_SCI_EVEN_CFG(cfg_tx) ? IOPORT_PERIPHERAL_SCI0_2_4_6_8 : IOPORT_PERIPHERAL_SCI1_3_5_7_9;
  ioport_peripheral_t ioport_rx = USE_SCI_EVEN_CFG(cfg_rx) ? IOPORT_PERIPHERAL_SCI0_2_4_6_8 : IOPORT_PERIPHERAL_SCI1_3_5_7_9;

  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[tx_pin].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | ioport_tx));
  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[rx_pin].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | ioport_rx));
  if (rts_pin != -1 && cts_pin != -1) {
    // hopefully people using flow control have read the datasheet so let's avoid the double check
    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[rts_pin].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | ioport_rx));
    R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[cts_pin].pin, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | ioport_rx));
  }

  return true;
}

/* -------------------------------------------------------------------------- */
void UART::begin(unsigned long baudrate, uint16_t config) {
/* -------------------------------------------------------------------------- */  
  int max_index = PINS_COUNT;

  init_ok = cfg_pins(max_index);
  
  if(init_ok) {
    UART::g_uarts[channel]        = this;

    uart_baud.semr_baudrate_bits_b.abcse          = 0;
    uart_baud.semr_baudrate_bits_b.abcs           = 0;
    uart_baud.semr_baudrate_bits_b.bgdm           = 1;
    uart_baud.cks                                 = 0;
    uart_baud.brr                                 = 25;
    uart_baud.mddr                                = (uint8_t) 256;
    uart_baud.semr_baudrate_bits_b.brme           = false;

    uart_cfg_extend.clock                         = SCI_UART_CLOCK_INT;
    uart_cfg_extend.rx_edge_start                 = SCI_UART_START_BIT_FALLING_EDGE;
    uart_cfg_extend.noise_cancel                  = SCI_UART_NOISE_CANCELLATION_DISABLE;
    //uart_cfg_extend.rx_fifo_trigger               = SCI_UART_RX_FIFO_TRIGGER_MAX;
    uart_cfg_extend.rx_fifo_trigger               = SCI_UART_RX_FIFO_TRIGGER_DEFAULT;
    uart_cfg_extend.p_baud_setting                = &uart_baud;
    uart_cfg_extend.flow_control                  = SCI_UART_FLOW_CONTROL_RTS;
    uart_cfg_extend.flow_control_pin              = (bsp_io_port_pin_t) UINT16_MAX;
    if (rts_pin != -1 && cts_pin != -1) {
      uart_cfg_extend.flow_control                  = SCI_UART_FLOW_CONTROL_HARDWARE_CTSRTS;
    }
    uart_cfg_extend.rs485_setting.enable          = SCI_UART_RS485_DISABLE;
    uart_cfg_extend.rs485_setting.polarity        = SCI_UART_RS485_DE_POLARITY_HIGH;
    uart_cfg_extend.rs485_setting.de_control_pin  = (bsp_io_port_pin_t) UINT16_MAX;
    
    uart_cfg.channel                              = channel; 
    uart_cfg.p_context                            = NULL;
    uart_cfg.p_extend                             = &uart_cfg_extend;
    uart_cfg.p_transfer_tx                        = NULL;
    uart_cfg.p_transfer_rx                        = NULL;
  
    switch(config){
      case SERIAL_8N1:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_OFF;
          uart_cfg.stop_bits = UART_STOP_BITS_1;
          break;
      case SERIAL_8N2:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_OFF;
          uart_cfg.stop_bits = UART_STOP_BITS_2;
          break;
      case SERIAL_8E1:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_EVEN;
          uart_cfg.stop_bits = UART_STOP_BITS_1;
          break;
      case SERIAL_8E2:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_EVEN;
          uart_cfg.stop_bits = UART_STOP_BITS_2;
          break;
      case SERIAL_8O1:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_ODD;
          uart_cfg.stop_bits = UART_STOP_BITS_1;
          break;
      case SERIAL_8O2:
          uart_cfg.data_bits = UART_DATA_BITS_8;
          uart_cfg.parity = UART_PARITY_ODD;
          uart_cfg.stop_bits = UART_STOP_BITS_2;
          break;
    }
    
    uart_cfg.p_callback = UART::WrapperCallback;
    tx_fsi_state = TX_FSI_COMPLETE;
  }
  else {
    return;
  }

  init_ok &= setUpUartIrqs(uart_cfg);
  
  fsp_err_t err;
  const bool bit_mod = true;
  const uint32_t err_rate = 3000; //means 3%

  err = R_SCI_UART_BaudCalculate(baudrate, bit_mod, err_rate, &uart_baud);
  if (uart_baud.mddr == 0) {
    err = R_SCI_UART_BaudCalculate(baudrate, false, err_rate, &uart_baud);
  }

  // TODO: Do you really want to hang the processor if it fails to open the Serial port?
  // or set the Baud rate?
  err = R_SCI_UART_Open (&uart_ctrl, &uart_cfg);
  if(err != FSP_SUCCESS) while(1);
  err = R_SCI_UART_BaudSet(&uart_ctrl, (void *) &uart_baud);
  if(err != FSP_SUCCESS) while(1);

  // Set a different callback if the uart has a FIFO so we don't have to keep
  // checking it
  if (uart_ctrl.fifo_depth > 0) {
    R_SCI_UART_CallbackSet (&uart_ctrl, UART::WrapperCallbackFIFO, nullptr, nullptr);
  }

  rxBuffer.clear();
  txBuffer.clear();
}

/* -------------------------------------------------------------------------- */
void UART::begin(unsigned long baudrate) {
/* -------------------------------------------------------------------------- */  
  begin(baudrate, SERIAL_8N1);
}

/* -------------------------------------------------------------------------- */
void UART::end() {
/* -------------------------------------------------------------------------- */  
  rxBuffer.clear();
  txBuffer.clear();
  R_SCI_UART_Close (&uart_ctrl);
}

/* -------------------------------------------------------------------------- */
int UART::available() {
/* -------------------------------------------------------------------------- */  
  return rxBuffer.available();
}

/* -------------------------------------------------------------------------- */
int UART::peek() {
/* -------------------------------------------------------------------------- */  
  return rxBuffer.peek();
}

/* -------------------------------------------------------------------------- */
int UART::read() {
/* -------------------------------------------------------------------------- */  
  return  rxBuffer.read_char();
}

/* -------------------------------------------------------------------------- */
void UART::flush() {
/* -------------------------------------------------------------------------- */  
  // wait until our software queue is not empty and we are not at TEND.
//  while(txBuffer.available());
//  while ((uart_ctrl.p_reg->SSR_b.TEND == 0) || (uart_ctrl.p_reg->SSR_b.TDRE == 0)) {}
  while(tx_fsi_state != TX_FSI_COMPLETE) ;

}

/* -------------------------------------------------------------------------- */
int UART::availableForWrite() {
/* -------------------------------------------------------------------------- */  
  return txBuffer.availableForStore();
}


/* -------------------------------------------------------------------------- */
size_t UART::write_raw(uint8_t* c, size_t len) {
/* -------------------------------------------------------------------------- */
  size_t i = 0;
  while (i < len) {
    uart_ctrl.p_reg->TDR = *(c+i);
    while (uart_ctrl.p_reg->SSR_b.TEND == 0) {}
    i++;
  }
  return len;
}
