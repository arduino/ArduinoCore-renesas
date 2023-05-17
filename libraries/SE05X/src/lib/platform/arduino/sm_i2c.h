/*
  sm_i2c.h
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

#ifndef SM_I2C_H_INC
#define SM_I2C_H_INC

#define T1oI2C_UM11225

#define I2C_IDLE 0
#define I2C_STARTED 1
#define I2C_RESTARTED 2
#define I2C_REPEATED_START 3
#define DATA_ACK 4
#define DATA_NACK 5
#define I2C_BUSY 6
#define I2C_NO_DATA 7
#define I2C_NACK_ON_ADDRESS 8
#define I2C_NACK_ON_DATA 9
#define I2C_ARBITRATION_LOST 10
#define I2C_TIME_OUT 11
#define I2C_OK 12
#define I2C_FAILED 13
#define I2C_BUS_0 (0)

typedef unsigned int i2c_error_t;

#if defined(__cplusplus)
extern "C" {
#endif

i2c_error_t axI2CInit(void **conn_ctx, const char *pDevName);
void axI2CTerm(void *conn_ctx, int mode);
i2c_error_t axI2CWrite(void *conn_ctx, unsigned char bus, unsigned char addr, unsigned char *pTx, unsigned short txLen);
i2c_error_t axI2CRead(void *conn_ctx, unsigned char bus, unsigned char addr, unsigned char *pRx, unsigned short rxLen);

#if defined(__cplusplus)
}
#endif

#endif // #ifndef SM_I2C_H_INC
