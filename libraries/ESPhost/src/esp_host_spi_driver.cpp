/* ########################################################################## */
/* - File: esp_host_spi_driver.cpp
   - Copyright (c): 2022 Arduino srl.
   - Author: Daniele Aimo (d.aimo@arduino.cc)

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
     Foundation, Inc.,51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA */
/* ########################################################################## */


/* SHORT DESCRITION:
   This file contains the implementation of the functions used to communicate
   via SPI with the ESP32C3 device */


/* ########
 * Includes 
 * ######## */

#include "esp_host_spi_driver.h"

/* #####################
 * Configuration defines 
 * ##################### */



/* FSP SPI Channel */
#define SPI_CHANNEL  (0) 

#define EXT_IRQ_CHANNEL (0)

/* GPIOs */
#define HANDSHAKE         BSP_IO_PORT_05_PIN_05  //BSP_IO_PORT_08_PIN_06  
#define DATA_READY        BSP_IO_PORT_08_PIN_02  //BSP_IO_PORT_08_PIN_03
#define DATA_READY_PIN    33                     //100

/* SPI PIN definition */
#define ESP_MISO   BSP_IO_PORT_01_PIN_00
#define ESP_MOSI   BSP_IO_PORT_01_PIN_01
#define ESP_CK     BSP_IO_PORT_01_PIN_02
#define ESP_CS     BSP_IO_PORT_01_PIN_03 // TODO: il vero CS è P104

/* #################
 * PRIVATE Variables
 * ################# */
static uint8_t rx_buffer[MAX_SPI_BUFFER_SIZE];
static uint8_t tx_buffer[MAX_SPI_BUFFER_SIZE];

static spi_instance_ctrl_t  _esp_host_spi_ctrl;
static spi_cfg_t            _esp_host_spi_cfg;
static spi_extended_cfg_t   _esp_host_spi_ext_cfg;

static icu_instance_ctrl_t  _esp_host_icu_ctrl;
static external_irq_cfg_t   _esp_host_icu_cfg;

static sci_spi_extended_cfg_t _sci_spi_ext_cfg;

static spi_event_t _spi_cb_status = SPI_EVENT_TRANSFER_ABORTED;


static CbkFuncRx_f esp_host_cb_rx_msg = nullptr;

/* #############################
 * PRIVATE Functions declaration
 * ############################# */

static void spi_callback(spi_callback_args_t *p_args);
static void ext_irq_callback(void);


/* execute SPI communication, send the content of tx_buffer to ESP32, put the
   message received from ESP32 in rx_buffer */
static int esp_host_send_and_receive(void);

/* check if something has to be sent to or received from ESP32 and, if necessary
   performs an actual spi transaction, to send and receive*/
int esp_host_spi_transaction(void);


/* ################
 * PUBLIC Functions
 * ################ */

/* -------------------------------------------------------------------------- */
/* INIT THE SPI DRIVER (to always called at first)                            
 * SPI driver also use 2 PIN (handshake and data ready) to synchronize SPI 
 * configuration                                                              */
/* -------------------------------------------------------------------------- */
int esp_host_spi_init(void) {
   
   

   /* ++++++++++++++++++++++++++++++++++
    *  GPIOs (HANDSHAKE and DATA_READY)
    * ++++++++++++++++++++++++++++++++++ */
   R_IOPORT_PinCfg(NULL, HANDSHAKE, IOPORT_CFG_PORT_DIRECTION_INPUT);
   /* DATA READY is configure in attach interrupt function below */
   //#ifdef EXPLICIT_PIN_CONFIGURATION
   R_IOPORT_PinCfg(NULL, DATA_READY, (uint32_t) (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT ));

   R_IOPORT_PinCfg(NULL, ESP_CS, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
   //#endif

   /* +++++
    *  SPI
    * +++++ */

   /*  INIT SPI peripheral PINs */
   R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_MISO, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8));
   R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_MOSI, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8));
   R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_CK,   (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8));
   //R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_CS,   (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8));

   /* SPI configuration */

   _esp_host_spi_cfg.p_extend       = &_sci_spi_ext_cfg;//_esp_host_spi_ext_cfg;
   _esp_host_spi_cfg.p_callback     = spi_callback; 
   _esp_host_spi_cfg.channel        = SPI_CHANNEL;
   _esp_host_spi_cfg.rxi_irq        = FSP_INVALID_VECTOR;
   _esp_host_spi_cfg.txi_irq        = FSP_INVALID_VECTOR;
   _esp_host_spi_cfg.tei_irq        = FSP_INVALID_VECTOR;
   _esp_host_spi_cfg.eri_irq        = FSP_INVALID_VECTOR;
   _esp_host_spi_cfg.rxi_ipl        = (12);
   _esp_host_spi_cfg.txi_ipl        = (12);
   _esp_host_spi_cfg.tei_ipl        = (12);
   _esp_host_spi_cfg.eri_ipl        = (12);
   _esp_host_spi_cfg.operating_mode = SPI_MODE_MASTER;
   _esp_host_spi_cfg.clk_phase      = SPI_CLK_PHASE_EDGE_ODD;
   _esp_host_spi_cfg.clk_polarity   = SPI_CLK_POLARITY_LOW;
   _esp_host_spi_cfg.mode_fault     = SPI_MODE_FAULT_ERROR_DISABLE;
   _esp_host_spi_cfg.bit_order      = SPI_BIT_ORDER_MSB_FIRST;
   _esp_host_spi_cfg.p_transfer_tx  = NULL;
   _esp_host_spi_cfg.p_transfer_rx  = NULL;
   _esp_host_spi_cfg.p_context      = NULL;

   /* ?????????????????????????
    * TODO: Use of DMA? or DTC?
    * ????????????????????????? */

   /* SPI extended configuration */

   _esp_host_spi_ext_cfg.spi_clksyn         = SPI_SSL_MODE_CLK_SYN;
   _esp_host_spi_ext_cfg.spi_comm           = SPI_COMMUNICATION_FULL_DUPLEX;
   _esp_host_spi_ext_cfg.ssl_polarity       = SPI_SSLP_HIGH;
   _esp_host_spi_ext_cfg.ssl_select         = SPI_SSL_SELECT_SSL0;
   _esp_host_spi_ext_cfg.mosi_idle          = SPI_MOSI_IDLE_VALUE_FIXING_DISABLE;
   _esp_host_spi_ext_cfg.parity             = SPI_PARITY_MODE_DISABLE;
   _esp_host_spi_ext_cfg.byte_swap          = SPI_BYTE_SWAP_DISABLE;
   _esp_host_spi_ext_cfg.spck_div           = { /* Actual calculated bitrate: 12000000. */ .spbr = 1, .brdv = 0 };
   _esp_host_spi_ext_cfg.spck_delay         = SPI_DELAY_COUNT_1;
   _esp_host_spi_ext_cfg.ssl_negation_delay = SPI_DELAY_COUNT_1;
   _esp_host_spi_ext_cfg.next_access_delay  = SPI_DELAY_COUNT_1;

   _sci_spi_ext_cfg.clk_div.cks  = 0;
   _sci_spi_ext_cfg.clk_div.brr  = 11;
   _sci_spi_ext_cfg.clk_div.mddr = 0;


   R_SCI_SPI_CalculateBitrate (1000000, &(_sci_spi_ext_cfg.clk_div), false);


   
  


  /* +++++++++++
   * INTERRUPTS
   * +++++++++++ */

   SpiMasterIrqReq_t irq_req {
     .ctrl = &_esp_host_spi_ctrl,
     .cfg = &_esp_host_spi_cfg,
     .hw_channel = (uint8_t)SPI_CHANNEL,
   };
    
   if(!IRQManager::getInstance().addPeripheral(IRQ_SCI_SPI_MASTER, &irq_req)) {
     return ESP_HOSTED_SPI_DRIVER_INIT_IRQ_FAILED;
   }

   attachInterrupt(DATA_READY_PIN, ext_irq_callback, RISING);

   /* +++++++++
    * OPEN SPI
    * +++++++++ */

   /* Configure the SPI using the FSP HAL functionlity. */
   if (FSP_SUCCESS != R_SCI_SPI_Open(&_esp_host_spi_ctrl, &_esp_host_spi_cfg)) {
      return ESP_HOSTED_SPI_DRIVER_SPI_FAIL_OPEN;
   }
   
   return ESP_HOSTED_SPI_DRIVER_OK; 
}


static bool tx_buffer_ready = false;

void esp_host_notify_spi_driver_to_tx(void) {

   /* if there is something to send esp32_receive_msg_to_be_sent_on_SPI will 
      return true and put the message to be sent on the tx_buffer */
   while(esp32_receive_msg_to_be_sent_on_SPI(tx_buffer, MAX_SPI_BUFFER_SIZE)) {
      tx_buffer_ready = true;
      esp_host_spi_transaction();
   }

}


void esp_host_set_cb_rx(CbkFuncRx_f fnc) {
   esp_host_cb_rx_msg = fnc;
}



/* ################################
 * PRIVATE Functions implementation
 * ################################ */
bsp_io_level_t handshake;
   bsp_io_level_t data_ready;

/* -------------------------------------------------------------------------- */
int esp_host_spi_transaction(void) {
/* -------------------------------------------------------------------------- */
   

   R_IOPORT_PinRead(NULL, HANDSHAKE, &handshake);
   R_IOPORT_PinRead(NULL, DATA_READY, &data_ready);

   
   /* ESP is ready to accept a new transaction */
   while(data_ready == BSP_IO_LEVEL_HIGH || tx_buffer_ready) {

      if(handshake == BSP_IO_LEVEL_HIGH) {
         /* there is something to send or to receive */
         if(esp_host_send_and_receive() == ESP_HOSTED_SPI_DRIVER_OK) {
            tx_buffer_ready = false;
            /* SPI transaction went OK */
            esp32_send_msg_to_application(rx_buffer, MAX_SPI_BUFFER_SIZE);
            if(esp_host_cb_rx_msg != nullptr) {
               esp_host_cb_rx_msg();
            }
         }
      }
      R_IOPORT_PinRead(NULL, HANDSHAKE, &handshake);
      R_IOPORT_PinRead(NULL, DATA_READY, &data_ready);
   }
   
}



int esp_host_send_and_receive(void) {
   _spi_cb_status = SPI_EVENT_ERR_MODE_FAULT;
   R_IOPORT_PinWrite(NULL, ESP_CS, BSP_IO_LEVEL_LOW);
   fsp_err_t err = R_SCI_SPI_WriteRead (&_esp_host_spi_ctrl, tx_buffer, rx_buffer, MAX_SPI_BUFFER_SIZE, SPI_BIT_WIDTH_8_BITS);
   if(err == FSP_SUCCESS) {
      for (auto const start = millis(); (SPI_EVENT_TRANSFER_COMPLETE != _spi_cb_status) && (millis() - start < 1000); ) {
        __NOP();
      }
   }
   if(_spi_cb_status == SPI_EVENT_TRANSFER_COMPLETE) {
      R_IOPORT_PinWrite(NULL, ESP_CS, BSP_IO_LEVEL_HIGH);
      return ESP_HOSTED_SPI_DRIVER_OK;
   }
   R_IOPORT_PinWrite(NULL, ESP_CS, BSP_IO_LEVEL_HIGH);
   return ESP_HOSTED_SPI_SPI_TRANSACTION_ERR;
}




/* -------------------------------------------------------------------------- */
static void spi_callback(spi_callback_args_t *p_args) {
/* -------------------------------------------------------------------------- */   
   if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event) {
      _spi_cb_status = SPI_EVENT_TRANSFER_COMPLETE;
   }
   else {
     /* Updating the flag here to capture and handle all other error events */
     _spi_cb_status = SPI_EVENT_TRANSFER_ABORTED;
   }
}

/* -------------------------------------------------------------------------- */
static void ext_irq_callback(void) {
/* -------------------------------------------------------------------------- */   
   esp_host_spi_transaction();
}