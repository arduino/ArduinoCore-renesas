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

#include "Arduino.h"
#include "IRQManager.h"
#include "esp_host_protocol.h"

#include "r_spi.h"
#include "r_spi_api.h"

#define ESP_HOSTED_SPI_DRIVER_OK   0
#define ESP_HOSTED_SPI_DRIVER_INIT_IRQ_FAILED 1
#define ESP_HOSTED_SPI_DRIVER_SPI_FAIL_OPEN   2
#define ESP_HOSTED_SPI_NOTHING_TO_TX_OR_RX    3
#define ESP_HOSTED_SPI_SPI_TRANSACTION_ERR    4

/* init the SPI driver */
int esp_host_spi_init(void);
/* check if something has to be sent to or received from ESP32 and, if necessary
   performs an actual spi transaction, to send and receive*/
int esp_host_spi_transaction(void);




#endif