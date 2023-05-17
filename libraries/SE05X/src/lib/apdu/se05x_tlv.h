/** @file se05x_tlv.h
 *  @brief TLV utils functions.
 *
 * Copyright 2021,2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SE05X_TLV_H_INC
#define SE05X_TLV_H_INC

/* ********************** Include files ********************** */
#include "se05x_types.h"

/* ********************** Constants ********************** */
typedef enum
{
    SM_NOT_OK                              = 0xFFFF,
    SM_OK                                  = 0x9000,
    SM_ERR_CONDITIONS_OF_USE_NOT_SATISFIED = 0x6985,
    SM_ERR_ACCESS_DENIED_BASED_ON_POLICY   = 0x6986,
} smStatus_t;

/* ********************** Function Prototypes ********************** */

int tlvSet_U8(uint8_t **buf, size_t *bufLen, SE05x_TAG_t tag, uint8_t value);
int tlvSet_U16(uint8_t **buf, size_t *bufLen, SE05x_TAG_t tag, uint16_t value);
int tlvSet_U32(uint8_t **buf, size_t *bufLen, SE05x_TAG_t tag, uint32_t value);
int tlvSet_u8buf(uint8_t **buf, size_t *bufLen, SE05x_TAG_t tag, const uint8_t *cmd, size_t cmdLen);
int tlvSet_u8bufOptional(uint8_t **buf, size_t *bufLen, SE05x_TAG_t tag, const uint8_t *cmd, size_t cmdLen);
int tlvSet_U16Optional(uint8_t **buf, size_t *bufLen, SE05x_TAG_t tag, uint16_t value);
int tlvSet_Se05xPolicy(const char *description, uint8_t **buf, size_t *bufLen, SE05x_TAG_t tag, Se05xPolicy_t *policy);
int tlvSet_MaxAttemps(uint8_t **buf, size_t *bufLen, SE05x_TAG_t tag, uint16_t maxAttemps);
int tlvSet_ECCurve(uint8_t **buf, size_t *bufLen, SE05x_TAG_t tag, SE05x_ECCurve_t value);
int tlvSet_KeyID(uint8_t **buf, size_t *bufLen, SE05x_TAG_t tag, uint32_t keyID);
int tlvSet_header(uint8_t **buf, size_t *bufLen, tlvHeader_t *hdr);
int tlvGet_U8(uint8_t *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, uint8_t *pRsp);
int tlvGet_U16(uint8_t *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, uint16_t *pRsp);
int tlvGet_u8buf(uint8_t *buf, size_t *pBufIndex, const size_t bufLen, SE05x_TAG_t tag, uint8_t *rsp, size_t *pRspLen);
int tlvGet_Result(uint8_t *buf, size_t *pBufIndex, size_t bufLen, SE05x_TAG_t tag, SE05x_Result_t *presult);
smStatus_t DoAPDUTx(
    pSe05xSession_t session_ctx, const tlvHeader_t *hdr, uint8_t *cmdBuf, size_t cmdBufLen, uint8_t hasle);
smStatus_t DoAPDUTxRx(pSe05xSession_t session_ctx,
    const tlvHeader_t *hdr,
    uint8_t *cmdBuf,
    size_t cmdBufLen,
    uint8_t *rspBuf,
    size_t *pRspBufLen,
    uint8_t hasle);

/* ********************** Defines ********************** */

#define DO_LOG_V(TAG, DESCRIPTION, VALUE)         \
    SMLOG_D("APDU  :DEBUG:" #TAG "[" #DESCRIPTION \
            "]"                                   \
            " = 0x%X \n",                         \
        VALUE);
#define DO_LOG_A(TAG, DESCRIPTION, ARRAY, ARRAY_LEN) \
    SMLOG_MAU8_D("APDU  :DEBUG:" #TAG "[" #DESCRIPTION "]", ARRAY, ARRAY_LEN);

#define TLVSET_U8(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_U8(PBUF, PBUFLEN, TAG, VALUE);                 \
    DO_LOG_V(TAG, DESCRIPTION, VALUE)

#define TLVSET_U16(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_U16(PBUF, PBUFLEN, TAG, VALUE);                 \
    DO_LOG_V(TAG, DESCRIPTION, VALUE)

#define TLVSET_U32(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_U32(PBUF, PBUFLEN, TAG, VALUE);                 \
    DO_LOG_V(TAG, DESCRIPTION, VALUE)

#define TLVSET_u8buf(DESCRIPTION, PBUF, PBUFLEN, TAG, CMD, CMDLEN) \
    tlvSet_u8buf(PBUF, PBUFLEN, TAG, CMD, CMDLEN);                 \
    DO_LOG_A(TAG, DESCRIPTION, CMD, CMDLEN)

#define TLVSET_u8bufOptional(DESCRIPTION, PBUF, PBUFLEN, TAG, CMD, CMDLEN) \
    tlvSet_u8bufOptional(PBUF, PBUFLEN, TAG, CMD, CMDLEN);                 \
    DO_LOG_A(TAG, DESCRIPTION, CMD, CMDLEN)

#define TLVSET_U16Optional(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_U16Optional(PBUF, PBUFLEN, TAG, VALUE);                 \
    DO_LOG_V(TAG, DESCRIPTION, VALUE)

#define TLVSET_KeyID(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_KeyID(PBUF, PBUFLEN, TAG, VALUE);                 \
    DO_LOG_V(TAG, DESCRIPTION, VALUE)

#define TLVSET_Se05xPolicy(DESCRIPTION, PBUF, PBUFLEN, TAG, POLICY) \
    tlvSet_Se05xPolicy(DESCRIPTION, PBUF, PBUFLEN, TAG, POLICY)

#define TLVSET_MaxAttemps(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_MaxAttemps(PBUF, PBUFLEN, TAG, VALUE);                 \
    DO_LOG_V(TAG, DESCRIPTION, VALUE)

#define TLVSET_ECCurve(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    tlvSet_ECCurve(PBUF, PBUFLEN, TAG, VALUE);                 \
    DO_LOG_V(TAG, DESCRIPTION, VALUE)

#define TLVSET_Header(PBUF, PBUFLEN, HDR) tlvSet_header(PBUF, PBUFLEN, HDR);

#define TLVSET_ECSignatureAlgo TLVSET_U8
#define TLVSET_CipherMode TLVSET_U8

#define TLVSET_CryptoModeSubType(DESCRIPTION, PBUF, PBUFLEN, TAG, VALUE) \
    TLVSET_U8(DESCRIPTION, PBUF, PBUFLEN, TAG, ((VALUE).union_8bit))

#endif // #ifndef SE05X_TLV_H_INC
