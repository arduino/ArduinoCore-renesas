/** @file smCom.c
 *  @brief SmCom APIs.
 *
 * Copyright 2021,2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @par Description
 * This file implements the SmCom T1oI2C communication layer.
 *
 *****************************************************************************/

/* ********************** Include files ********************** */
#ifdef ARDUINO
#include "../t1oi2c/phNxpEseProto7816_3.h"
#include "../t1oi2c/phEseStatus.h"
#include "../platform/arduino/sm_port.h"
#else
#include "phNxpEseProto7816_3.h"
#include "phEseStatus.h"
#include "sm_port.h"
#endif
#include "smCom.h"
#include <limits.h>

/* ********************** Global vaiables ********************** */
SM_MUTEX_DEFINE(g_sm_mutex);

/* ********************** Functions ********************** */

smStatus_t smComT1oI2C_Close(void *conn_ctx, uint8_t mode)
{
    ESESTATUS status;
    (void)mode;

    ENSURE_OR_RETURN_ON_ERROR((conn_ctx != NULL), SM_NOT_OK);

    status = phNxpEse_EndOfApdu(conn_ctx);
    ENSURE_OR_RETURN_ON_ERROR((status == ESESTATUS_SUCCESS), SM_NOT_OK);

    status = phNxpEse_close(conn_ctx);
    ENSURE_OR_RETURN_ON_ERROR((status == ESESTATUS_SUCCESS), SM_NOT_OK);

    SM_MUTEX_DEINIT(g_sm_mutex);

    return SM_OK;
}

smStatus_t smComT1oI2C_Init(void **conn_ctx, const char *pConnString)
{
    ESESTATUS status;
    phNxpEse_initParams initParams;
    initParams.initMode = ESE_MODE_NORMAL;

    status = phNxpEse_open(conn_ctx, initParams, pConnString);
    ENSURE_OR_RETURN_ON_ERROR((status == ESESTATUS_SUCCESS), SM_NOT_OK);

    SM_MUTEX_INIT(g_sm_mutex);

    return SM_OK;
}

smStatus_t smComT1oI2C_Open(void *conn_ctx, uint8_t mode, uint8_t seqCnt, uint8_t *T1oI2Catr, size_t *T1oI2CatrLen)
{
    ESESTATUS status;
    phNxpEse_data AtrRsp;
    phNxpEse_initParams initParams;
    initParams.initMode = mode;
    AtrRsp.len          = *T1oI2CatrLen;
    AtrRsp.p_data       = T1oI2Catr;

    (void)mode;
    (void)seqCnt;

    ENSURE_OR_RETURN_ON_ERROR((conn_ctx != NULL), SM_NOT_OK);

    status = phNxpEse_init(conn_ctx, initParams, &AtrRsp);
    ENSURE_OR_RETURN_ON_ERROR((status == ESESTATUS_SUCCESS), SM_NOT_OK);

    *T1oI2CatrLen = AtrRsp.len; /*Retrive INF FIELD*/

    return SM_OK;
}

smStatus_t smComT1oI2C_TransceiveRaw(void *conn_ctx, uint8_t *pTx, size_t txLen, uint8_t *pRx, size_t *pRxLen)
{
    phNxpEse_data pCmdTrans;
    phNxpEse_data pRspTrans = {0};
    ESESTATUS status;

    ENSURE_OR_RETURN_ON_ERROR((UINT8_MAX >= txLen), SM_NOT_OK);
    ENSURE_OR_RETURN_ON_ERROR((UINT8_MAX >= (*pRxLen)), SM_NOT_OK);

    pCmdTrans.len    = txLen;
    pCmdTrans.p_data = pTx;

    pRspTrans.len    = *pRxLen;
    pRspTrans.p_data = pRx;

    ENSURE_OR_RETURN_ON_ERROR((conn_ctx != NULL), SM_NOT_OK);
    ENSURE_OR_RETURN_ON_ERROR((pTx != NULL), SM_NOT_OK);
    ENSURE_OR_RETURN_ON_ERROR((pRx != NULL), SM_NOT_OK);
    ENSURE_OR_RETURN_ON_ERROR((pRxLen != NULL), SM_NOT_OK);

    SMLOG_MAU8_D("APDU Tx>", pTx, txLen);

    SM_MUTEX_LOCK(g_sm_mutex);
    status = phNxpEse_Transceive(conn_ctx, &pCmdTrans, &pRspTrans);
    SM_MUTEX_UNLOCK(g_sm_mutex);
    ENSURE_OR_RETURN_ON_ERROR((status == ESESTATUS_SUCCESS), SM_NOT_OK);

    *pRxLen = pRspTrans.len;
    SMLOG_MAU8_D("APDU Rx<", pRx, pRspTrans.len);

    return SM_OK;
}

smStatus_t smComT1oI2C_ComReset(void *conn_ctx)
{
    ESESTATUS status = ESESTATUS_SUCCESS;
    status           = phNxpEse_deInit(conn_ctx);
    if (status != ESESTATUS_SUCCESS) {
        SMLOG_E("Failed to Reset 7816 protocol instance ");
        return SM_NOT_OK;
    }
    return SM_OK;
}
