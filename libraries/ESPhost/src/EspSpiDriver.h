/* ########################################################################## */
/* - File: esp_host_spi_driver.h
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

#ifndef ESP_HOSTED_SPI_DRIVER_H
#define ESP_HOSTED_SPI_DRIVER_H

//#define ESP_HOST_DEBUG_ENABLED

#include "Arduino.h"
#include "IRQManager.h"
#include "CEspCommunication.h"
//#include "esp_host_protocol.h"

#include "r_spi.h"
#include "r_sci_spi.h"

using CbkFuncRx_f = void(*)();

#define ESP_HOSTED_SPI_DRIVER_OK   0
#define ESP_HOSTED_SPI_NOTHING_TO_TX_OR_RX    3
#define ESP_HOSTED_SPI_MESSAGE_RECEIVED       8


#define ESP_HOSTED_SPI_DRIVER_INIT_IRQ_FAILED 1
#define ESP_HOSTED_SPI_DRIVER_SPI_FAIL_OPEN   2
#define ESP_HOSTED_SPI_SPI_TRANSACTION_ERR    4
#define ESP_HOSTED_SPI_ESP_NOT_READY          5
#define ESP_HOSTED_SPI_TX_RX_ABORTED          6
#define ESP_HOSTED_SPI_TIMEOUT                7
#define ESP_HOSTED_MSG_PREPARATION_ERROR      9

/* init the SPI driver */
int esp_host_spi_init(void);
/* it performs the spi communication untill there are message to be tx or rx
   (it stops if an error occurred) 
   it returns the error or 0 if all is ok */
int esp_host_perform_spi_communication(bool wait_for_valid_msg);

bool isEspSpiInitialized() ;
bool arePendingRxMsg();

bool isSpiTransactionInProgress();
void setSpiTransactionInPRogress();




#endif