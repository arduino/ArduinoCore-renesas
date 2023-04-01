/*
  sm_i2c.cpp
  Copyright (c) 2023 Arduino SA.  All right reserved.

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
*/

#include "../../platform/arduino/sm_i2c.h"
#include <Wire.h>

i2c_error_t axI2CInit(void **conn_ctx, const char *pDevName) {
    (void)conn_ctx;
    (void)pDevName;

    Wire3.begin();
    Wire3.setClock(1000000);
    return I2C_OK;
}

void axI2CTerm(void *conn_ctx, int mode) {
    (void)conn_ctx;
    (void)mode;

    Wire3.end();
}

i2c_error_t axI2CWrite(void *conn_ctx, unsigned char bus, unsigned char addr, unsigned char *pTx, unsigned short txLen) {
    (void)conn_ctx;
    (void)bus;

    addr = addr >> 1;
    Wire3.beginTransmission(addr);
    Wire3.write(pTx, txLen);
    if (Wire3.endTransmission() != 0) {
        return I2C_FAILED;
    }

    return I2C_OK;
}

i2c_error_t axI2CRead(void *conn_ctx, unsigned char bus, unsigned char addr, unsigned char *pRx, unsigned short rxLen) {
    (void)conn_ctx;
    (void)bus;
    addr = addr >> 1;
    int retries = 20;

    while (Wire3.requestFrom((uint8_t)addr, (size_t)rxLen, (bool)true) != rxLen && retries--);
    
    for (size_t i = 0; Wire3.available(); i++) {
        pRx[i] = Wire3.read();
    }

    return I2C_OK;
}
