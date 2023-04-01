/** @file smCom.h
 *  @brief SmCom APIs.
 *
 * Copyright 2021,2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @par Description
 * Interface of installable communication layer to exchange APDU's between Host and Secure Module.
 */

#ifndef SMCOM_H_INC
#define SMCOM_H_INC

/* ********************** Include files ********************** */
#include "se05x_tlv.h"

/* ********************** Function Prototypes ********************** */
#ifdef __cplusplus
extern "C" {
#endif

smStatus_t smComT1oI2C_Close(void *conn_ctx, uint8_t mode);
smStatus_t smComT1oI2C_Init(void **conn_ctx, const char *pConnString);
smStatus_t smComT1oI2C_Open(void *conn_ctx, uint8_t mode, uint8_t seqCnt, uint8_t *T1oI2Catr, size_t *T1oI2CatrLen);
smStatus_t smComT1oI2C_TransceiveRaw(void *conn_ctx, uint8_t *pTx, size_t txLen, uint8_t *pRx, size_t *pRxLen);
smStatus_t smComT1oI2C_ComReset(void *conn_ctx);

#ifdef __cplusplus
}
#endif

#endif //#ifndef SMCOM_H_INC
