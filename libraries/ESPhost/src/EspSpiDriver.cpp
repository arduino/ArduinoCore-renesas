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

#include <SPI.h>
#include "EspSpiDriver.h"

/* #####################
 * Configuration defines 
 * ##################### */

#ifdef USE_ESP32_C3_DEVKIT_RUST_1
/* GPIOs */
#define HANDSHAKE         60
#define DATA_READY        33

/* SPI PIN definition */
#define SPIWIFI SPI1
#define ESP_CS     48
#else
/* GPIOs */
#define ESP_RESET         101
#define HANDSHAKE         102
#define DATA_READY        100

/* SPI PIN definition */
#define SPIWIFI SPI1
#define ESP_CS     103
#endif

/* #################
 * PRIVATE Variables
 * ################# */


static volatile uint8_t esp32_spi_rx_buffer[MAX_SPI_BUFFER_SIZE];
static volatile uint8_t esp32_spi_tx_buffer[MAX_SPI_BUFFER_SIZE];

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


/* #############################
 * PRIVATE Functions declaration
 * ############################# */

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
   pinMode(HANDSHAKE, INPUT);
   pinMode(DATA_READY, INPUT);
   pinMode(ESP_CS, OUTPUT);
   pinMode(ESP_RESET, OUTPUT);

   /* +++++
    *  SPI
    * +++++ */
    
   digitalWrite(ESP_CS, HIGH);
   SPIWIFI.begin();
   
   digitalWrite(ESP_RESET, HIGH);
   spi_driver_initialized = true;
   return ESP_HOSTED_SPI_DRIVER_OK; 
}

/*
 This function checks the DATA READY pin drive by ESP. It returns:
 - true if ESP has data to be transmitted (received by host)
 - false if ESP has no data to be transmitted
 */
bool esp_host_there_are_data_to_be_rx() {
   bool rv = digitalRead(DATA_READY);

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
   do {
     esp_ready = (digitalRead(HANDSHAKE) == HIGH);
      if(esp_ready) {
         #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
         Serial.print("ESP ready? ");
         Serial.println(esp_ready);
         #endif
         break;
      }
      delayMicroseconds(100);
      time_num++;
   } while(time_num < 5000);

   if(esp_ready) {
      SPIWIFI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE2));
      /* Put CS LOW */
      digitalWrite(ESP_CS, LOW);
      delayMicroseconds(100);
      
      /* memset RX buffer */
      memset((void *)esp32_spi_rx_buffer,0x00, MAX_SPI_BUFFER_SIZE);
      
      #ifdef ESP_HOST_DEBUG_ENABLED_AVOID
      Serial.println("  [SPI] Execute single SPI transaction:");
      Serial.print("  [SPI] TX DATA: ");
      for(int i = 0; i < 200; i++) {
         Serial.print(esp32_spi_tx_buffer[i], HEX);
         Serial.print(" ");
      }
      Serial.println();
      #endif


      for (int i = 0; i < MAX_SPI_BUFFER_SIZE; i++) {
        esp32_spi_rx_buffer[i] = SPIWIFI.transfer(esp32_spi_tx_buffer[i]);
      }
      
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
         spi_transaction_in_progress = false;
   }
   else {
      rv = ESP_HOSTED_SPI_ESP_NOT_READY;
   }

   /* in any case de-select ESP32 */
   digitalWrite(ESP_CS, HIGH);
   SPIWIFI.endTransaction();

   
   return rv;
}
