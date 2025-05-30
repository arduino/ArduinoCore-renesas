/* ########################################################################## */
/* - File: EspChipManager.h
   - Copyright (c): 2025 Arduino srl.
   - Author: Fabio Massimo Centonze (f.centonze@arduino.cc)

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

#ifndef ESP_CHIP_MANAGER_H
#define ESP_CHIP_MANAGER_H
#include <Arduino.h>

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
#define HANDSHAKE         BSP_IO_PORT_08_PIN_06  
#define DATA_READY        BSP_IO_PORT_08_PIN_03
#define DATA_READY_PIN    100

/* SPI PIN definition */
#define ESP_MISO   BSP_IO_PORT_01_PIN_00
#define ESP_MOSI   BSP_IO_PORT_01_PIN_01
#define ESP_CK     BSP_IO_PORT_01_PIN_02
#define ESP_CS     BSP_IO_PORT_01_PIN_04 
#endif


class CEspChipManager {
   public:
      static CEspChipManager& getInstance();

      // Delete copy constructor and assignment operator to enforce singleton
      CEspChipManager(const CEspChipManager&) = delete;
      CEspChipManager& operator=(const CEspChipManager&) = delete;
      // General initialization
      void initialize();
      // Force a reset of the ESP chip, don't use for initialization
      void forceReset();
   private:
      CEspChipManager() = default;
      ~CEspChipManager() = default;
      bool isInitialized = false;
};
#endif // ESP_CHIP_MANAGER_H
