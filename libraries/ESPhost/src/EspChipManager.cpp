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

#include "EspChipManager.h"

#define ESP_RESET       BSP_IO_PORT_08_PIN_04
#define ESP_DOWNLOAD    DATA_READY // The DATA_READY pin is shared with the ESP_DOWNLOAD pin

CEspChipManager& CEspChipManager::getInstance() {
    static CEspChipManager instance;
    return instance;
}

void CEspChipManager::initialize() {
    if (!isInitialized) {
        forceReset();
        isInitialized = true;
    }
}

void CEspChipManager::forceReset() {
    R_IOPORT_PinCfg(NULL, ESP_RESET, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
    // Set the ESP_DOWNLOAD pin to high to ensure the ESP doesn't enter in download mode when resetting
    R_IOPORT_PinCfg(NULL, ESP_DOWNLOAD, (uint32_t) (IOPORT_CFG_PORT_DIRECTION_OUTPUT));
    R_IOPORT_PinWrite(NULL, ESP_DOWNLOAD, BSP_IO_LEVEL_HIGH);
    delay(100);
    R_IOPORT_PinWrite(NULL, ESP_RESET, BSP_IO_LEVEL_HIGH);
    R_IOPORT_PinWrite(NULL, ESP_RESET, BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(NULL, ESP_RESET, BSP_IO_LEVEL_HIGH);
    R_IOPORT_PinWrite(NULL, ESP_DOWNLOAD, BSP_IO_LEVEL_LOW);
    R_IOPORT_PinCfg(NULL, ESP_DOWNLOAD, (uint32_t) (IOPORT_CFG_IRQ_ENABLE | IOPORT_CFG_PORT_DIRECTION_INPUT));
}

