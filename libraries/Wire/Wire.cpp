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
*/

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include "Wire.h"

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);

static uint32_t timeout_ms = 1000;

static i2c_master_event_t _i2c_cb_event[6] = {I2C_MASTER_EVENT_ABORTED};

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire(i2c_master_ctrl_t *g_i2c_master_ctrl
                 ,const i2c_master_cfg_t *g_i2c_master_cfg, int ch):
  _g_i2c_master_ctrl(g_i2c_master_ctrl)
, _g_i2c_master_cfg(g_i2c_master_cfg)
, _channel(ch)
{
}

TwoWire::TwoWire(int ch, bool isSci):
  _channel(ch),
  _is_sci(isSci)
{
}

void TwoWire::configureI2C(bsp_io_port_pin_t sda, bsp_io_port_pin_t scl, int ch) {
  pinPeripheral(sda, (uint32_t) IOPORT_CFG_DRIVE_MID |
                     (uint32_t) IOPORT_CFG_PERIPHERAL_PIN |
                     (uint32_t) IOPORT_PERIPHERAL_IIC);
  pinPeripheral(scl, (uint32_t) IOPORT_CFG_DRIVE_MID |
                     (uint32_t) IOPORT_CFG_PERIPHERAL_PIN |
                     (uint32_t) IOPORT_PERIPHERAL_IIC);
  if (ch==0) {
    _g_i2c_master_ctrl = &g_i2c_master0_ctrl;
    _g_i2c_master_cfg = &g_i2c_master0_cfg;
  } else if (ch==1) {
    _g_i2c_master_ctrl = &g_i2c_master1_ctrl;
    _g_i2c_master_cfg = &g_i2c_master1_cfg;
  } else if (ch==2) {
    _g_i2c_master_ctrl = &g_i2c_master2_ctrl;
    _g_i2c_master_cfg = &g_i2c_master2_cfg;
  }
}

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(void)
{
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txBufferIndex = 0;
  txBufferLength = 0;

  _i2c_config = *_g_i2c_master_cfg;

  configureI2C(_sda, _scl, _channel);

  R_IIC_MASTER_Open(_g_i2c_master_ctrl, &_i2c_config);
}

void TwoWire::begin(uint8_t address)
{
  begin();
  // TO DO
  //twi_setAddress(address);
}

void TwoWire::begin(int address)
{
  begin((uint8_t)address);
}

void TwoWire::end(void)
{
  R_IIC_MASTER_Close(_g_i2c_master_ctrl);
}

void TwoWire::setPins(int sda, int scl)
{
  setPins(digitalPinToBspPin(sda), digitalPinToBspPin(scl));
}

void TwoWire::setPins(bsp_io_port_pin_t sda, bsp_io_port_pin_t scl)
{
  pinPeripheral(miso, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_IIC);
  pinPeripheral(mosi, (uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_IIC);
}

void TwoWire::setClock(uint32_t clock)
{
  // TO DO
  //twi_setFrequency(clock);
}

size_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
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
  while (isize-- > 0)
    write((uint8_t)(iaddress >> (isize*8)));
  endTransmission(false);
  }

  // clamp to buffer length
  if(quantity > BUFFER_LENGTH){
    quantity = BUFFER_LENGTH;
  }
  // perform blocking read into buffer
  uint8_t read = twi_readFrom(address, rxBuffer, quantity, sendStop);
  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = read;

  return (size_t)read;
}

size_t TwoWire::requestFrom(uint8_t address, size_t quantity, bool sendStop) {
	return requestFrom((uint8_t)address, quantity, (uint32_t)0, (uint8_t)0, sendStop);
}

size_t TwoWire::requestFrom(uint8_t address, size_t quantity)
{
  return requestFrom((uint8_t)address, quantity, true);
}

void TwoWire::beginTransmission(uint8_t address)
{
  // indicate that we are transmitting
  transmitting = 1;
  // set address of targeted slave
  txAddress = address;
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
  beginTransmission((uint8_t)address);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to 
//	perform a repeated start. 
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(bool sendStop)
{
  // transmit buffer (blocking)
  uint8_t ret = twi_writeTo(txAddress, txBuffer, txBufferLength, 1, sendStop);
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
  // indicate that we are done transmitting
  transmitting = 0;
  return ret;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
  return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
  if(transmitting){
  // in master transmitter mode
    // don't bother if buffer is full
    if(txBufferLength >= BUFFER_LENGTH){
      setWriteError();
      return 0;
    }
    // put byte in tx buffer
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    // update amount in buffer   
    txBufferLength = txBufferIndex;
  }else{
  // in slave send mode
    // reply to master
    //twi_transmit(&data, 1);
  }
  return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
  if(transmitting){
  // in master transmitter mode
    for(size_t i = 0; i < quantity; ++i){
      write(data[i]);
    }
  }else{
  // in slave send mode
    // reply to master
    //twi_transmit(data, quantity);
  }
  return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
  int value = -1;
  
  // get each successive byte on each call
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
  int value = -1;
  
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }

  return value;
}

void TwoWire::flush(void)
{
  // XXX: to be implemented.
}

// behind the scenes function that is called when data is received
void TwoWire::onReceiveService(uint8_t* inBytes, int numBytes)
{
  // don't bother if user hasn't registered a callback
  if(!user_onReceive){
    return;
  }
  // don't bother if rx buffer is in use by a master requestFrom() op
  // i know this drops data, but it allows for slight stupidity
  // meaning, they may not have read all the master requestFrom() data yet
  if(rxBufferIndex < rxBufferLength){
    return;
  }
  // copy twi rx buffer into local read buffer
  // this enables new reads to happen in parallel
  for(uint8_t i = 0; i < numBytes; ++i){
    rxBuffer[i] = inBytes[i];    
  }
  // set rx iterator vars
  rxBufferIndex = 0;
  rxBufferLength = numBytes;
  // alert user program
  user_onReceive(numBytes);
}

// behind the scenes function that is called when data is requested
void TwoWire::onRequestService(void)
{
  // don't bother if user hasn't registered a callback
  if(!user_onRequest){
    return;
  }
  // reset tx buffer iterator vars
  // !!! this will kill any pending pre-master sendTo() activity
  txBufferIndex = 0;
  txBufferLength = 0;
  // alert user program
  user_onRequest();
}

// sets function called on slave write
void TwoWire::onReceive( void (*function)(int) )
{
  user_onReceive = function;
}

// sets function called on slave read
void TwoWire::onRequest( void (*function)(void) )
{
  user_onRequest = function;
}
/*
void TwoWire::setCallbackEvent(i2c_master_event_t cb_event, int ch)
{
  _g_i2c_callback_event = cb_event;
}
*/

/* 
 * Function twi_readFrom
 * Desc     attempts to become twi bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes to read into array
 *          sendStop: Boolean indicating whether to send a stop at the end
 * Output   number of bytes read
 */
uint8_t TwoWire::twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop)
{
  R_IIC_MASTER_SlaveAddressSet (_g_i2c_master_ctrl, address, I2C_MASTER_ADDR_MODE_7BIT);
  R_IIC_MASTER_Read(_g_i2c_master_ctrl, data, length, !sendStop);
  while ((I2C_MASTER_EVENT_RX_COMPLETE != _i2c_cb_event[_channel]) && timeout_ms)
  {
      timeout_ms--;
      delay(1);
  }
  return length;
}

/* 
 * Function twi_writeTo
 * Desc     attempts to become twi bus master and write a
 *          series of bytes to a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes in array
 *          wait: boolean indicating to wait for write or not
 *          sendStop: boolean indicating whether or not to send a stop at the end
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 */
uint8_t TwoWire::twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
{
  fsp_err_t err;
  _i2c_cb_event[_channel] = I2C_MASTER_EVENT_ABORTED;
  R_IIC_MASTER_SlaveAddressSet (_g_i2c_master_ctrl, address, I2C_MASTER_ADDR_MODE_7BIT);
  err = R_IIC_MASTER_Write(_g_i2c_master_ctrl, data, length, !sendStop);
  while ((I2C_MASTER_EVENT_TX_COMPLETE != _i2c_cb_event[_channel]) && timeout_ms)
  {
      timeout_ms--;
      delay(1);
  }
  if (err == FSP_ERR_IN_USE){
      return 2;
  } else if (I2C_MASTER_EVENT_ABORTED == _i2c_cb_event[_channel])
  {
      return 4;
  }
  return 0;
}

#if WIRE_HOWMANY > 0
TwoWire Wire(&g_i2c_master0_ctrl, &g_i2c_master0_cfg, 0);
#endif
void isr_i2c0 (i2c_master_callback_args_t * p_args)
{
  _i2c_cb_event[0] = p_args->event;
}
#if WIRE_HOWMANY > 1
TwoWire Wire1(&g_i2c_master1_ctrl, &g_i2c_master1_cfg, 1);
#endif
void isr_i2c1 (i2c_master_callback_args_t * p_args)
{
  _i2c_cb_event[1] = p_args->event;
}
#if WIRE_HOWMANY > 2
TwoWire Wire2(&g_i2c_master2_ctrl, &g_i2c_master2_cfg, 2);
#endif
void isr_i2c2 (i2c_master_callback_args_t * p_args)
{
  _i2c_cb_event[2] = p_args->event;
}

void isr_i2c3 (i2c_master_callback_args_t * p_args)
{
  _i2c_cb_event[3] = p_args->event;
}

void isr_i2c4 (i2c_master_callback_args_t * p_args)
{
  _i2c_cb_event[4] = p_args->event;
}

void isr_i2c5 (i2c_master_callback_args_t * p_args)
{
  _i2c_cb_event[5] = p_args->event;
}




