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

#include "EspSpiDriver.h"

/* #####################
 * Configuration defines 
 * ##################### */



/* FSP SPI Channel */
#ifdef USE_SCI_SPI_FSP_API
#define SPI_CHANNEL  (0) 
#else
#define SPI_CHANNEL  (1) 
#endif

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
#ifdef USE_SCI_SPI_FSP_API
static sci_spi_extended_cfg_t _sci_spi_ext_cfg;
#else
static spi_extended_cfg_t   _esp_host_spi_ext_cfg;
#endif

static int spi_rx_waiting = 0;

bool arePendingRxMsg() {
   bool rv = spi_rx_waiting > 0 ? true : false;
   spi_rx_waiting--;
   return rv;
}


static bool spi_transaction_in_progress = false;

bool isSpiTransactionInProgress() { 
   bool rv = spi_transaction_in_progress;
   //Serial.println("TEST");
   if(spi_transaction_in_progress == false) {
      //Serial.println("IN PROGRESS");
      spi_transaction_in_progress = true;
   } 
   return rv;
}
void setSpiTransactionInPRogress() { spi_transaction_in_progress  = true; }

//static icu_instance_ctrl_t  _esp_host_icu_ctrl;
//static external_irq_cfg_t   _esp_host_icu_cfg;



static spi_event_t _spi_cb_status = SPI_EVENT_TRANSFER_ABORTED;


//static CbkFuncRx_f esp_host_cb_rx_msg = nullptr;

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

static bool spi_driver_initialized = false;

bool isEspSpiInitialized() {
   return spi_driver_initialized;
}

/* -------------------------------------------------------------------------- */
/* INIT THE SPI DRIVER (to always called at first)                            
 * SPI driver also use 2 PIN (handshake and data ready) to synchronize SPI 
 * configuration                                                              */
/* -------------------------------------------------------------------------- */
int esp_host_spi_init(void) {
   
   if(spi_driver_initialized) {
      return ESP_HOSTED_SPI_DRIVER_OK;
   }

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
   #ifdef USE_SCI_SPI_FSP_API
   R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_MISO, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8));
   R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_MOSI, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8));
   R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_CK,   (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8));
   #else
   R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_MISO, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SPI));
   R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_MOSI, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SPI));
   R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_CK,   (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SPI));
   #endif
   //R_IOPORT_PinCfg(&g_ioport_ctrl, ESP_CS,   (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SCI0_2_4_6_8));

   /* SPI configuration */
   #ifdef USE_SCI_SPI_FSP_API
   _esp_host_spi_cfg.p_extend       = &_sci_spi_ext_cfg;//_esp_host_spi_ext_cfg;
   #else
   _esp_host_spi_cfg.p_extend       = &_esp_host_spi_ext_cfg;
   #endif

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

   #ifdef USE_SCI_SPI_FSP_API
   _sci_spi_ext_cfg.clk_div.cks  = 0;
   _sci_spi_ext_cfg.clk_div.brr  = 11;
   _sci_spi_ext_cfg.clk_div.mddr = 0;

   R_SCI_SPI_CalculateBitrate (24000000, &(_sci_spi_ext_cfg.clk_div), false);
   #else
   R_SPI_CalculateBitrate (24000000, &(_esp_host_spi_ext_cfg.spck_div));
   #endif

  /* +++++++++++
   * INTERRUPTS
   * +++++++++++ */

   SpiMasterIrqReq_t irq_req {
     .ctrl = &_esp_host_spi_ctrl,
     .cfg = &_esp_host_spi_cfg,
     .hw_channel = (uint8_t)SPI_CHANNEL,
   };
    
   #ifdef USE_SCI_SPI_FSP_API 
   if(!IRQManager::getInstance().addPeripheral(IRQ_SCI_SPI_MASTER, &irq_req)) {
   #else 
   if(!IRQManager::getInstance().addPeripheral(IRQ_SPI_MASTER, &irq_req)) {
   #endif   
     return ESP_HOSTED_SPI_DRIVER_INIT_IRQ_FAILED;
   }

   

   /* +++++++++
    * OPEN SPI
    * +++++++++ */
   #ifdef USE_SCI_SPI_FSP_API 
   /* Configure the SPI using the FSP HAL functionlity. */
   if (FSP_SUCCESS != R_SCI_SPI_Open(&_esp_host_spi_ctrl, &_esp_host_spi_cfg)) {
   #else 
   if (FSP_SUCCESS != R_SPI_Open(&_esp_host_spi_ctrl, &_esp_host_spi_cfg)) {
   #endif
      return ESP_HOSTED_SPI_DRIVER_SPI_FAIL_OPEN;
   }
   
   R_IOPORT_PinWrite(NULL, ESP_RESET, BSP_IO_LEVEL_HIGH);
   spi_driver_initialized = true;
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

   #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
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
   __disable_irq();
   bool rv = CEspCom::get_msg_from_app((uint8_t*)esp32_spi_tx_buffer, MAX_SPI_BUFFER_SIZE);
   if(!rv) {
      CEspCom::clearToEspQueue();
   }
   __enable_irq();
   #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
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

/* this function calls esp_host_spi_transaction() to perfom spi communication
   esp_host_spi_transaction() checks if there is somenthing to send (by checking
   if something is on the tx queue) or something to receive if the DATA_READY
   pin is high 
   Only in this case a communication with the SPI is actually performed 
   Since a single SPI communcation always transmit and receive if something 
   valid is received this is put into the RX queue
   At the end esp_host_spi_transaction() can return:
   - ESP_HOSTED_SPI_NOTHING_TO_TX_OR_RX if there is nothing to transmit or receive
   - ESP_HOSTED_SPI_MESSAGE_RECEIVED if a valid message has ben received and put into the rx_queue 
   - ESP_HOSTED_SPI_DRIVER_OK if the SPI communication went well but no valid message has beed received 
   - something different indicating an error into the SPI communication */
int esp_host_perform_spi_communication(bool wait_for_valid_msg) {
   int rv = ESP_HOSTED_SPI_DRIVER_OK;
   bool continue_spi_communication = true;

   do {
      rv = esp_host_spi_transaction();
      if(rv == ESP_HOSTED_SPI_NOTHING_TO_TX_OR_RX) {
         /* exit because it makes no sense to execute SPI communication if there is nothing to do */
         continue_spi_communication = false;
      }
      
      /* if this flag is true we are waiting for an answer from ESP32, this is related to control
         messages */
      else if(wait_for_valid_msg && rv == ESP_HOSTED_SPI_MESSAGE_RECEIVED) {
         
            /* we received a valid message, we exit to see if the message received is actually
               the answer we were waiting, although this is not guarantee at this point because
               we could also have received some network trafic */
            continue_spi_communication = false;
      }
      else if(wait_for_valid_msg && rv == ESP_HOSTED_SPI_DRIVER_OK) {
         /* in this case we wait to see if new message are arriving */
         continue_spi_communication = true;
      }
      else {
         /* either, or we do no care to wait or something went wrong during the
            SPI communication, so we exit */
         continue_spi_communication = false;
      }
      

   } while(continue_spi_communication);
   
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
         #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
         Serial.print("  [SPI]: MESSAGE RECEIVED");
         #endif
         /* SPI transaction went OK */
         __disable_irq();
         bool res = CEspCom::send_msg_to_app((const uint8_t*)esp32_spi_rx_buffer, MAX_SPI_BUFFER_SIZE);
         __enable_irq();
         if(res) {
            
            rv = ESP_HOSTED_SPI_MESSAGE_RECEIVED;
            #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
            Serial.println("AND QUEUED to APP");
            #endif
         }
         else {
            #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
            Serial.println("BUT NOT QUEUED to APP");
            #endif
         }

      }
   }
   else {
      
      spi_transaction_in_progress = false;
   }
   return rv; 
}

extern void getESPDebugLog();
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
         #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
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
      
      #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
      Serial.println("  [SPI] Execute single SPI transaction:");
      Serial.print("  [SPI] TX DATA: ");
      for(int i = 0; i < 200; i++) {
         Serial.print(esp32_spi_tx_buffer[i], HEX);
         Serial.print(" ");
      }
      Serial.println();
      #endif


      #ifdef USE_SCI_SPI_FSP_API
      fsp_err_t err = R_SCI_SPI_WriteRead (&_esp_host_spi_ctrl, (void *)esp32_spi_tx_buffer, (void *)esp32_spi_rx_buffer, MAX_SPI_BUFFER_SIZE, SPI_BIT_WIDTH_8_BITS);
      #else
      fsp_err_t err = R_SPI_WriteRead (&_esp_host_spi_ctrl, (void *)esp32_spi_tx_buffer, (void *)esp32_spi_rx_buffer, MAX_SPI_BUFFER_SIZE, SPI_BIT_WIDTH_8_BITS);
      #endif
      
      if(err == FSP_SUCCESS) {
         bool exited = false;
         /* wait for SPI transaction to finish */
         time_num = 0;
         do {
            
            if(_spi_cb_status == SPI_EVENT_TRANSFER_COMPLETE) { 
               rv = ESP_HOSTED_SPI_DRIVER_OK;
               
               //Serial.println("NO MORE IN PROGRESS");
               #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
               Serial.print("  [SPI] RX DATA: ");
               for(int i = 0; i < 50; i++) {
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
         spi_transaction_in_progress = false;
         if(!exited) {
            //Serial.print("SPI TIMEOUT");
            rv = ESP_HOSTED_SPI_TIMEOUT;
         }         
      }
      else {
         #ifdef ESP_HOST_DEBUG_ENABLED
         Serial.print("[ERROR]: SPI TRANSACTION FAILED! ");
         Serial.println(err);
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
  
}