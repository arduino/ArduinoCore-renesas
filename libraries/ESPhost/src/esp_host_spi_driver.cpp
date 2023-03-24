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

#ifdef USE_ESP32_C3_DEVKIT_RUST_1
/* GPIOs */
#define HANDSHAKE         BSP_IO_PORT_05_PIN_05  
#define DATA_READY        BSP_IO_PORT_08_PIN_02  
#define DATA_READY_PIN    33                     

/* SPI PIN definition */
#define ESP_MISO   BSP_IO_PORT_01_PIN_00
#define ESP_MOSI   BSP_IO_PORT_01_PIN_01
#define ESP_CK     BSP_IO_PORT_01_PIN_02
#define ESP_CS     BSP_IO_PORT_01_PIN_03 
#else
/* GPIOs */
#define ESP_RESET         BSP_IO_PORT_08_PIN_04
#define HANDSHAKE         BSP_IO_PORT_08_PIN_06  
#define DATA_READY        BSP_IO_PORT_08_PIN_03
#define DATA_READY_PIN    100

/* SPI PIN definition */
#define ESP_MISO   BSP_IO_PORT_01_PIN_00
#define ESP_MOSI   BSP_IO_PORT_01_PIN_01
#define ESP_CK     BSP_IO_PORT_01_PIN_02
#define ESP_CS     BSP_IO_PORT_01_PIN_04 
#endif

/* #################
 * PRIVATE Variables
 * ################# */
static volatile unsigned int pi_msg_to_get_from_esp32 = 0;


static volatile uint8_t esp32_spi_rx_buffer[MAX_SPI_BUFFER_SIZE];
static volatile uint8_t esp32_spi_tx_buffer[MAX_SPI_BUFFER_SIZE];

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
   R_IOPORT_PinCfg(NULL, ESP_RESET, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
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
   _esp_host_spi_cfg.clk_polarity   = SPI_CLK_POLARITY_HIGH;
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

   R_SCI_SPI_CalculateBitrate (100000, &(_sci_spi_ext_cfg.clk_div), false);

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
   
   R_IOPORT_PinWrite(NULL, ESP_RESET, BSP_IO_LEVEL_HIGH);

   return ESP_HOSTED_SPI_DRIVER_OK; 
}

/*
 This function checks the DATA READY pin drive by ESP. It returns:
 - true if ESP has data to be transmitted (received by host)
 - false if ESP has no data to be transmitted
 */
bool esp_host_there_are_data_to_be_rx() {
   bsp_io_level_t data_ready;
   R_IOPORT_PinRead(NULL, DATA_READY, &data_ready);
   bool rv = (data_ready == BSP_IO_LEVEL_HIGH);

   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.print("**** RX data? ");
   if(rv) {
      Serial.println("YES!!!!!");
   }
   else {
      Serial.println("no");
   }
   #endif
   return rv;
}

bool esp_host_there_are_data_to_be_tx() {
   /* the function esp32_receive_msg_to_be_sent_on_SPI memset the tx buffer to 0 if there are no data to be tx*/
   bool rv = CEspCom::get_msg_from_app((uint8_t*)esp32_spi_tx_buffer, MAX_SPI_BUFFER_SIZE);

   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.print("**** TX data? ");
   if(rv) {
      Serial.println("YES!!!!!");
   }
   else {
      Serial.println("no");
   }
   #endif
   return rv;
}


int esp_host_perform_spi_communication() {
   int rv = ESP_HOSTED_SPI_DRIVER_OK;
   int res = 0;
   /* it exits if there is some transaction error or if there is no more
      data to be tx ox rx*/
   do {
      res = esp_host_spi_transaction();
      R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
      #ifdef ESP_HOST_DEBUG_ENABLED
      Serial.print("Execute single SPI transaction ");
      Serial.println(res);
      #endif


      if(res == ESP_HOSTED_SPI_DRIVER_OK || 
         res == ESP_HOSTED_SPI_MESSAGE_RECEIVED ||
         res == ESP_HOSTED_SPI_NOTHING_TO_TX_OR_RX) {
         /* those are good possible results */
      }
      else {
         /* exit for ERROR */
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.println("!!!!! SPI TRANSACTION ERROR ");
         #endif
         rv = res;
         break;
      }
   }
   while(res != ESP_HOSTED_SPI_NOTHING_TO_TX_OR_RX);
   
   #ifdef ESP_HOST_DEBUG_ENABLED
   Serial.print("esp_host_perform_spi_communication ");
   Serial.println(rv);
   #endif

   return rv;
}


/* ################################
 * PRIVATE Functions implementation
 * ################################ */


/* esp_host_spi_transaction:
   1. verify if there is something to receive or to transmit 
   2. if there is something to receive or to transmit calls esp_host_send_and_receive() 
   3. if esp_host_send_and_receive() actually receive a valid message */

/* -------------------------------------------------------------------------- */
int esp_host_spi_transaction(void) {
/* -------------------------------------------------------------------------- */
   int rv = ESP_HOSTED_SPI_NOTHING_TO_TX_OR_RX;
   
   bool data_to_be_rx = esp_host_there_are_data_to_be_rx();
   bool data_to_be_tx = esp_host_there_are_data_to_be_tx();
   
   if(data_to_be_rx || data_to_be_tx) {
      rv = esp_host_send_and_receive();
         /* there is something to send or to receive */
      if(rv == ESP_HOSTED_SPI_DRIVER_OK) {
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.println("MESSAGE RECEIVED ");
         #endif
         /* SPI transaction went OK */
         if(CEspCom::send_msg_to_app((const uint8_t*)esp32_spi_rx_buffer, MAX_SPI_BUFFER_SIZE)) {
            
            rv = ESP_HOSTED_SPI_MESSAGE_RECEIVED;
            #ifdef ESP_HOST_DEBUG_ENABLED
            Serial.print("RECEIVED MESSAGE QUEUED ");
            Serial.println(rv);
            #endif
         }    
      }
   }
   return rv; 
}


/* -------------------------------------------------------------------------- */
int esp_host_send_and_receive(void) {
/* -------------------------------------------------------------------------- */   
   int rv = ESP_HOSTED_SPI_SPI_TRANSACTION_ERR;
   int time_num = 0;
   
   /* VERIFY IF ESP32 is ready to accept a SPI transaction */
   bool esp_ready = false;
   bsp_io_level_t handshake;
   do {
      R_IOPORT_PinRead(NULL, HANDSHAKE, &handshake);
      esp_ready = (handshake == BSP_IO_LEVEL_HIGH);
      if(esp_ready) {
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.print("ESP ready? ");
         Serial.println(esp_ready);
         #endif
         break;
      }
      R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
      time_num++;
   } while(time_num < 5000);

   if(esp_ready) {
      /* Put CS LOW */
      R_IOPORT_PinWrite(NULL, ESP_CS, BSP_IO_LEVEL_LOW);
      R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
      
      /* memset RX buffer */
      memset((void *)esp32_spi_rx_buffer,0x00, MAX_SPI_BUFFER_SIZE);
      
      _spi_cb_status = SPI_EVENT_ERR_MODE_FAULT;
      
      #ifdef ESP_HOST_DEBUG_ENABLED
      Serial.println("Execute SPI tx/rx ");
      Serial.print("TX DATA: ");
      for(int i = 0; i < MAX_SPI_BUFFER_SIZE; i++) {
         Serial.print(esp32_spi_tx_buffer[i], HEX);
         Serial.print(" ");
      }
      Serial.println();
      #endif

      if(FSP_SUCCESS == R_SCI_SPI_WriteRead (&_esp_host_spi_ctrl, (void *)esp32_spi_tx_buffer, (void *)esp32_spi_rx_buffer, MAX_SPI_BUFFER_SIZE, SPI_BIT_WIDTH_8_BITS)) {
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.println("OK");
         #endif

         bool exited = false;
         /* wait for SPI transaction to finish */
         time_num = 0;
         do {
            if(_spi_cb_status == SPI_EVENT_TRANSFER_COMPLETE) { 
               rv = ESP_HOSTED_SPI_DRIVER_OK;
               #ifdef ESP_HOST_DEBUG_ENABLED
               Serial.print("RX DATA: ");
               for(int i = 0; i < MAX_SPI_BUFFER_SIZE; i++) {
                  Serial.print(esp32_spi_rx_buffer[i], HEX);
                  Serial.print(" ");
               }
               Serial.println();
               #endif
               exited = true;
            }
            else if(_spi_cb_status == SPI_EVENT_TRANSFER_ABORTED) {
               rv = ESP_HOSTED_SPI_TX_RX_ABORTED;
               exited = true;
            }
            R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
            time_num++;
         } while(time_num < 5000 && !exited);

         if(!exited) {
            rv = ESP_HOSTED_SPI_TIMEOUT;
         }         
      }
      else {
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.println("FAILED");
         #endif
      }
   }
   else {
      rv = ESP_HOSTED_SPI_ESP_NOT_READY;
   }

   /* in any case de-select ESP32 */
   R_IOPORT_PinWrite(NULL, ESP_CS, BSP_IO_LEVEL_HIGH);
   return rv;
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
   //pi_msg_to_get_from_esp32++;
   //esp_host_send_and_receive();
}