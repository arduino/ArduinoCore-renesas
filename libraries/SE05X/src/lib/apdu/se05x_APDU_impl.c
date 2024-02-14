/** @file se05x_APDU_impl.c
 *  @brief Se05x APDU function implementation.
 *
 * Copyright 2021,2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/* ********************** Include files ********************** */
#include "smCom.h"
#ifdef ARDUINO
#include "../platform/arduino/sm_port.h"
#else
#include "sm_port.h"
#endif
#include "se05x_types.h"
#ifdef ARDUINO
#include "../t1oi2c/phNxpEse_Api.h"
#else
#include "phNxpEse_Api.h"
#endif

/* ********************** Defines ********************** */
#define kSE05x_CLA 0x80
#define CLA_ISO7816 (0x00)   //!< ISO7816-4 defined CLA byte
#define INS_GP_SELECT (0xA4) //!< Global platform defined instruction

/* clang-format off */
#define APPLET_NAME { 0xa0, 0x00, 0x00, 0x03, 0x96, 0x54, 0x53, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00 }
#define SSD_NAME {0xD2, 0x76, 0x00, 0x00, 0x85, 0x30, 0x4A, 0x43, 0x4F, 0x90, 0x03}
/* clang-format on */

/* ********************** Function Prototypes ********************** */
#ifdef WITH_PLATFORM_SCP03
smStatus_t Se05x_API_SCP03_CreateSession(pSe05xSession_t session_ctx);
#endif //#ifdef WITH_PLATFORM_SCP03

/* ********************** Functions ********************** */

bool Se05x_IsInValidRangeOfUID(uint32_t uid)
{
    // Block required keyids
    (void)uid;
    return FALSE;
}

smStatus_t Se05x_API_SessionOpen(pSe05xSession_t session_ctx)
{
    size_t buff_len            = 0;
    size_t tx_len              = 0;
    smStatus_t ret             = SM_NOT_OK;
    unsigned char appletName[] = APPLET_NAME;
#ifdef WITH_PLATFORM_SCP03
    unsigned char ssdName[] = SSD_NAME;
#endif
    unsigned char *appSsdName = NULL;
    size_t appSsdNameLen      = 0;

    SMLOG_I("Plug and Trust nano package - version: %d.%d.%d \n", VERSION_MAJOR, VERSION_MINOR, VERSION_DEV);

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    buff_len = sizeof(session_ctx->apdu_buffer);

    ret = smComT1oI2C_Init(&session_ctx->conn_context, NULL);
    ENSURE_OR_GO_CLEANUP(SM_OK == ret);

    if (session_ctx->session_resume == 1) {
        ret = smComT1oI2C_Open(session_ctx->conn_context, ESE_MODE_RESUME, 0x00, session_ctx->apdu_buffer, &buff_len);
    }
    else {
        ret = smComT1oI2C_Open(session_ctx->conn_context, ESE_MODE_NORMAL, 0x00, session_ctx->apdu_buffer, &buff_len);
    }
    ENSURE_OR_GO_CLEANUP(SM_OK == ret);

    if (session_ctx->skip_applet_select == 1) {
#ifndef WITH_PLATFORM_SCP03
        return ret;
#else
        appSsdName    = &ssdName[0];
        appSsdNameLen = sizeof(ssdName);
#endif
    }
    else {
        appSsdName    = &appletName[0];
        appSsdNameLen = sizeof(appletName);
    }

    if (!session_ctx->session_resume) {
        /* Select applet / ssd */
        session_ctx->apdu_buffer[0] = CLA_ISO7816;
        session_ctx->apdu_buffer[1] = INS_GP_SELECT;
        session_ctx->apdu_buffer[2] = 4;
        session_ctx->apdu_buffer[3] = 0;

        tx_len = 1 /* CLA */ + 1 /* INS */ + 1 /* P1 */ + 1 /* P2 */;

        session_ctx->apdu_buffer[4] = appSsdNameLen;
        tx_len                      = tx_len + 1 /* Lc */ + appSsdNameLen /* Payload */ + 1 /* Le */;
        memcpy(&session_ctx->apdu_buffer[5], appSsdName, appSsdNameLen);
        session_ctx->apdu_buffer[tx_len - 1] = 0; /* Le */

        buff_len = sizeof(session_ctx->apdu_buffer);
        ret      = smComT1oI2C_TransceiveRaw(
            session_ctx->conn_context, session_ctx->apdu_buffer, tx_len, session_ctx->apdu_buffer, &buff_len);
        if (ret != SM_OK) {
            SMLOG_E("Se05x_API_SessionOpen failed");
            goto cleanup;
        }

        if (ret == SM_OK && buff_len >= 2) {
            ret = session_ctx->apdu_buffer[buff_len - 2];
            ret <<= 8;
            ret |= session_ctx->apdu_buffer[buff_len - 1];
        }
        ENSURE_OR_GO_CLEANUP(SM_OK == ret);
    }

#ifdef WITH_PLATFORM_SCP03
    if (session_ctx->session_resume) {
        SMLOG_I("Resuming Secure Channel to SE05x !\n");
    }
    else {
        SMLOG_I("Establish Secure Channel to SE05x !\n");
        ret = Se05x_API_SCP03_CreateSession(session_ctx);
    }
#endif //#ifdef WITH_PLATFORM_SCP03

cleanup:
    return ret;
}

smStatus_t Se05x_API_SessionClose(pSe05xSession_t session_ctx)
{
    SMLOG_D("APDU - Se05x_API_SessionClose [] \n");
    return smComT1oI2C_Close(session_ctx->conn_context, 0);
}

smStatus_t Se05x_API_WriteECKey(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    SE05x_MaxAttemps_t maxAttempt,
    uint32_t objectID,
    SE05x_ECCurve_t curveID,
    const uint8_t *privKey,
    size_t privKeyLen,
    const uint8_t *pubKey,
    size_t pubKeyLen,
    const SE05x_INS_t ins_type,
    const SE05x_KeyPart_t key_part)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_WRITE | ins_type, kSE05x_P1_EC | key_part, kSE05x_P2_DEFAULT}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    pCmdbuf = &session_ctx->apdu_buffer[0];

    if (Se05x_IsInValidRangeOfUID(objectID)) {
        return SM_NOT_OK;
    }

    SMLOG_D("APDU - WriteECKey [] \n");

    tlvRet = TLVSET_Se05xPolicy("policy", &pCmdbuf, &cmdbufLen, kSE05x_TAG_POLICY, policy);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_MaxAttemps("maxAttempt", &pCmdbuf, &cmdbufLen, kSE05x_TAG_MAX_ATTEMPTS, maxAttempt);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_ECCurve("curveID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, curveID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("privKey", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, privKey, privKeyLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("pubKey", &pCmdbuf, &cmdbufLen, kSE05x_TAG_4, pubKey, pubKeyLen);
    if (0 != tlvRet) {
        goto cleanup;
    }

    retStatus = DoAPDUTx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, 0);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ReadObject(
    pSe05xSession_t session_ctx, uint32_t objectID, uint16_t offset, uint16_t length, uint8_t *data, size_t *pdataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_READ, kSE05x_P1_DEFAULT, kSE05x_P2_DEFAULT}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;
    uint8_t *pRspbuf     = NULL;
    size_t rspbufLen     = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    pCmdbuf   = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    SMLOG_D("APDU - ReadObject [] \n");

    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_U16Optional("offset", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, offset);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_U16Optional("length", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, length);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 1);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, data, pdataLen); /*  */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]);
        }
    }

    if (retStatus == SM_ERR_ACCESS_DENIED_BASED_ON_POLICY) {
        SMLOG_I("Denied to read object %08X bases on policy.", objectID);
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_GetVersion(pSe05xSession_t session_ctx, uint8_t *pappletVersion, size_t *appletVersionLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_VERSION}};
    int tlvRet           = 0;
    uint8_t *pRspbuf     = NULL;
    size_t rspbufLen     = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    SMLOG_D("APDU - GetVersion [] \n");

    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, 0, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, pappletVersion, appletVersionLen);
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]);
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ECDSASign(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_ECSignatureAlgo_t ecSignAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *signature,
    size_t *psignatureLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_SIGNATURE, kSE05x_P2_SIGN}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;
    uint8_t *pRspbuf     = NULL;
    size_t rspbufLen     = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    pCmdbuf   = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    SMLOG_D("APDU - ECDSASign [] \n");

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_ECSignatureAlgo("ecSignAlgo", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, ecSignAlgo);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, signature, psignatureLen); /*  */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]);
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ECDSAVerify(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_ECSignatureAlgo_t ecSignAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    const uint8_t *signature,
    size_t signatureLen,
    SE05x_Result_t *presult)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_SIGNATURE, kSE05x_P2_VERIFY}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;
    uint8_t *pRspbuf     = NULL;
    size_t rspbufLen     = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    pCmdbuf   = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    SMLOG_D("APDU - ECDSAVerify [] \n");

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_ECSignatureAlgo("ecSignAlgo", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, ecSignAlgo);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("signature", &pCmdbuf, &cmdbufLen, kSE05x_TAG_5, signature, signatureLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_Result(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, presult); /* - */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]);
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_CheckObjectExists(pSe05xSession_t session_ctx, uint32_t objectID, SE05x_Result_t *presult)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_EXIST}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;
    uint8_t *pRspbuf     = NULL;
    size_t rspbufLen     = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    pCmdbuf   = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    SMLOG_D("APDU - CheckObjectExists [] \n");

    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_Result(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, presult); /* - */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]);
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_WriteBinary(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    uint32_t objectID,
    uint16_t offset,
    uint16_t length,
    const uint8_t *inputData,
    size_t inputDataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_WRITE, kSE05x_P1_BINARY, kSE05x_P2_DEFAULT}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    pCmdbuf = &session_ctx->apdu_buffer[0];

    if (Se05x_IsInValidRangeOfUID(objectID)) {
        return SM_NOT_OK;
    }

    SMLOG_D("APDU - WriteBinary [] \n");

    tlvRet = TLVSET_Se05xPolicy("policy", &pCmdbuf, &cmdbufLen, kSE05x_TAG_POLICY, policy);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_U16Optional("offset", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, offset);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_U16Optional("length", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, length);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("input data", &pCmdbuf, &cmdbufLen, kSE05x_TAG_4, inputData, inputDataLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, 0);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ECDHGenerateSharedSecret(pSe05xSession_t session_ctx,
    uint32_t objectID,
    const uint8_t *pubKey,
    size_t pubKeyLen,
    uint8_t *sharedSecret,
    size_t *psharedSecretLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_EC, kSE05x_P2_DH}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;
    uint8_t *pRspbuf     = NULL;
    size_t rspbufLen     = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    pCmdbuf   = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    SMLOG_D("APDU -ECDHGenerateSharedSecret [] \n");

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("pubKey", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, pubKey, pubKeyLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, sharedSecret, psharedSecretLen); /*  */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]);
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_CipherOneShot(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_CipherMode_t cipherMode,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *IV,
    size_t IVLen,
    uint8_t *outputData,
    size_t *poutputDataLen,
    const SE05x_Cipher_Oper_OneShot_t operation)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_CIPHER, operation}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;
    uint8_t *pRspbuf     = NULL;
    size_t rspbufLen     = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    pCmdbuf   = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    SMLOG_D("APDU - CipherOneShot [] \n");

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_CipherMode("cipherMode", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cipherMode);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("IV", &pCmdbuf, &cmdbufLen, kSE05x_TAG_4, IV, IVLen);
    if (0 != tlvRet) {
        goto cleanup;
    }

    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, outputData, poutputDataLen); /*  */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]);
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_WriteSymmKey(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    SE05x_MaxAttemps_t maxAttempt,
    uint32_t objectID,
    SE05x_KeyID_t kekID,
    const uint8_t *keyValue,
    size_t keyValueLen,
    const SE05x_INS_t ins_type,
    const SE05x_SymmKeyType_t type)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_WRITE | ins_type, type, kSE05x_P2_DEFAULT}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    pCmdbuf = &session_ctx->apdu_buffer[0];

    if (Se05x_IsInValidRangeOfUID(objectID)) {
        return SM_NOT_OK;
    }

    SMLOG_D("APDU - WriteSymmKey [] \n");

    tlvRet = TLVSET_Se05xPolicy("policy", &pCmdbuf, &cmdbufLen, kSE05x_TAG_POLICY, policy);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_MaxAttemps("maxAttempt", &pCmdbuf, &cmdbufLen, kSE05x_TAG_MAX_ATTEMPTS, maxAttempt);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_KeyID("KEK id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, kekID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("key value", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, keyValue, keyValueLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, 0);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DeleteSecureObject(pSe05xSession_t session_ctx, uint32_t objectID)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_DELETE_OBJECT}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_DeleteSecureObject [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];

    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, 0);

cleanup:
    return retStatus;
}

/* API added from PlugNTrust full library*/

smStatus_t Se05x_API_GetRandom(pSe05xSession_t session_ctx, uint16_t size, uint8_t *randomData, size_t *prandomDataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_DEFAULT, kSE05x_P2_RANDOM}};
    size_t cmdbufLen                       = 0;
    uint8_t *pCmdbuf                       = NULL;
    int tlvRet                             = 0;
    uint8_t *pRspbuf                       = NULL;
    size_t rspbufLen                       = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_GetRandom [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);
    
    tlvRet = TLVSET_U16("size", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, size);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, randomData, prandomDataLen); /*  */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DigestInit(pSe05xSession_t session_ctx, SE05x_CryptoObjectID_t cryptoObjectID)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_DEFAULT, kSE05x_P2_INIT}};
    size_t cmdbufLen     = 0;
    uint8_t *pCmdbuf     = NULL;
    int tlvRet           = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_DigestInit [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];

    tlvRet = TLVSET_U16("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, 0);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DigestUpdate(
    pSe05xSession_t session_ctx, SE05x_CryptoObjectID_t cryptoObjectID, const uint8_t *inputData, size_t inputDataLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_DEFAULT, kSE05x_P2_UPDATE}};
    size_t cmdbufLen = 0;
    uint8_t *pCmdbuf = NULL;
    int tlvRet       = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_DigestUpdate [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];

    tlvRet = TLVSET_U16("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet) {
        SMLOG_D("cryptoObjectID err \n");
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, 0);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DigestFinal(pSe05xSession_t session_ctx,
    SE05x_CryptoObjectID_t cryptoObjectID,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *cmacValue,
    size_t *pcmacValueLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_DEFAULT, kSE05x_P2_FINAL}};
    size_t cmdbufLen                       = 0;
    uint8_t *pCmdbuf                       = NULL;
    int tlvRet                             = 0;
    uint8_t *pRspbuf                       = NULL;
    size_t rspbufLen                       = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_DigestFinal [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    tlvRet = TLVSET_U16("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoObjectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8buf("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, cmacValue, pcmacValueLen); /*  */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DigestOneShot(pSe05xSession_t session_ctx,
    uint8_t digestMode,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *hashValue,
    size_t *phashValueLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_DEFAULT, kSE05x_P2_ONESHOT}};
    size_t cmdbufLen                       = 0;
    uint8_t *pCmdbuf                       = NULL;
    int tlvRet                             = 0;
    uint8_t *pRspbuf                       = NULL;
    size_t rspbufLen                       = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_DigestOneShot [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    tlvRet = TLVSET_U8("digestMode", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, digestMode);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, inputData, inputDataLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, hashValue, phashValueLen); /*  */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}


smStatus_t Se05x_API_MACOneShot_G(pSe05xSession_t session_ctx,
    uint32_t objectID,
    uint8_t macOperation,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *macValue,
    size_t *pmacValueLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_CRYPTO, kSE05x_P1_MAC, kSE05x_P2_GENERATE_ONESHOT}};
    size_t cmdbufLen                       = 0;
    uint8_t *pCmdbuf                       = NULL;
    int tlvRet                             = 0;
    uint8_t *pRspbuf                       = NULL;
    size_t rspbufLen                       = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_MACOneShot_G [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    tlvRet = TLVSET_U32("objectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_U8("macOperation", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, macOperation);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_u8bufOptional("inputData", &pCmdbuf, &cmdbufLen, kSE05x_TAG_3, inputData, inputDataLen);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);

    if (retStatus == SM_OK) {
        retStatus = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet    = tlvGet_u8buf(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, macValue, pmacValueLen);
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}


smStatus_t Se05x_API_CreateCryptoObject(pSe05xSession_t session_ctx,
    SE05x_CryptoObjectID_t cryptoObjectID,
    SE05x_CryptoContext_t cryptoContext,
    SE05x_CryptoModeSubType_t subtype)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_WRITE, kSE05x_P1_CRYPTO_OBJ, kSE05x_P2_DEFAULT}};
    size_t cmdbufLen = 0;
    uint8_t *pCmdbuf = NULL;
    int tlvRet       = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_CreateCryptoObject [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];

    tlvRet = TLVSET_U16("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, cryptoObjectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_U8("cryptoContext", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, cryptoContext);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_CryptoModeSubType(
        "1-byte Crypto Object subtype, either from DigestMode, CipherMode or MACAlgo (depending on TAG_2).",
        &pCmdbuf,
        &cmdbufLen,
        kSE05x_TAG_3,
        subtype);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, 0);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DeleteCryptoObject(pSe05xSession_t session_ctx, SE05x_CryptoObjectID_t cryptoObjectID)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_MGMT, kSE05x_P1_CRYPTO_OBJ, kSE05x_P2_DELETE_OBJECT}};
    size_t cmdbufLen = 0;
    uint8_t *pCmdbuf = NULL;
    int tlvRet       = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_DeleteCryptoObject [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];

    tlvRet = TLVSET_U16("cryptoObjectID", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, cryptoObjectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, 0);

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_ReadIDList(pSe05xSession_t session_ctx,
    uint16_t outputOffset,
    uint8_t filter,
    uint8_t *pmore,
    uint8_t *idlist,
    size_t *pidlistLen)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_READ, kSE05x_P1_DEFAULT, kSE05x_P2_LIST}};
    size_t cmdbufLen                       = 0;
    uint8_t *pCmdbuf                       = NULL;
    int tlvRet                             = 0;
    uint8_t *pRspbuf                       = NULL;
    size_t rspbufLen                       = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_DigestOneShot [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    tlvRet = TLVSET_U16("output offset", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, outputOffset);
    if (0 != tlvRet) {
        goto cleanup;
    }
    tlvRet = TLVSET_U8("filter", &pCmdbuf, &cmdbufLen, kSE05x_TAG_2, filter);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_U8(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, pmore); /* - */
        if (0 != tlvRet) {
            goto cleanup;
        }
        tlvRet = tlvGet_u8buf(pRspbuf,
            &rspIndex,
            rspbufLen,
            kSE05x_TAG_2,
            idlist,
            pidlistLen); /* Byte array containing 4-byte identifiers */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}

smStatus_t Se05x_API_DeleteAll_Iterative(pSe05xSession_t session_ctx)
{
    uint8_t pmore = kSE05x_MoreIndicator_NA;
    uint8_t list[1024];
    size_t listlen = sizeof(list);
    size_t i;
    smStatus_t retStatus  = SM_NOT_OK;
    uint16_t outputOffset = 0;
    do {
        retStatus = Se05x_API_ReadIDList(session_ctx, outputOffset, 0xFF, &pmore, list, &listlen);
        if (retStatus != SM_OK) {
            return retStatus;
        }
        outputOffset = (uint16_t)listlen;
        for (i = 0; i < listlen; i += 4) {
            uint32_t id = 0 | (list[i + 0] << (3 * 8)) | (list[i + 1] << (2 * 8)) | (list[i + 2] << (1 * 8)) |
                          (list[i + 3] << (0 * 8));
            if (SE05X_OBJID_SE05X_APPLET_RES_START == SE05X_OBJID_SE05X_APPLET_RES_MASK(id)) {
                SMLOG_D("Not erasing ObjId=0x%08X (Reserved)", id);
                /* In Reserved space */
            }
            else if (EX_SSS_OBJID_DEMO_AUTH_START == EX_SSS_OBJID_DEMO_AUTH_MASK(id)) {
                SMLOG_D("Not erasing ObjId=0x%08X (Demo Auth)", id);
                /* Not reasing default authentication object */
            }
            else if (EX_SSS_OBJID_IOT_HUB_A_START == EX_SSS_OBJID_IOT_HUB_A_MASK(id)) {
                SMLOG_D("Not erasing ObjId=0x%08X (IoT Hub)", id);
                /* Not reasing IoT Hub object */
            }
            else if (!SE05X_OBJID_TP_MASK(id) && id) {
                SMLOG_D("Not erasing Trust Provisioned objects");
            }
            else {
                retStatus = Se05x_API_DeleteSecureObject(session_ctx, id);
                if (retStatus != SM_OK) {
                    SMLOG_W("Error in erasing ObjId=0x%08X (Others)", id);
                }
            }
        }
    } while (pmore == kSE05x_MoreIndicator_MORE);
#if SSSFTR_SE05X_CREATE_DELETE_CRYPTOOBJ
    retStatus = Se05x_API_ReadCryptoObjectList(session_ctx, list, &listlen);
    if (retStatus != SM_OK) {
        goto cleanup;
    }
    for (i = 0; i < listlen; i += 4) {
        uint16_t cryptoObjectId                = list[i + 1] | (list[i + 0] << 8);
        SE05x_CryptoObjectID_t ecryptoObjectId = (SE05x_CryptoObjectID_t)cryptoObjectId;
        retStatus                              = Se05x_API_DeleteCryptoObject(session_ctx, ecryptoObjectId);
        if (retStatus != SM_OK) {
            LOG_W("Error in erasing CryptoObject=%04X", cryptoObjectId);
        }
    }
cleanup:
#endif
    return retStatus;
}

smStatus_t Se05x_API_ReadSize(pSe05xSession_t session_ctx, uint32_t objectID, uint16_t *psize)
{
    smStatus_t retStatus = SM_NOT_OK;
    tlvHeader_t hdr      = {{kSE05x_CLA, kSE05x_INS_READ, kSE05x_P1_DEFAULT, kSE05x_P2_SIZE}};
    size_t cmdbufLen                       = 0;
    uint8_t *pCmdbuf                       = NULL;
    int tlvRet                             = 0;
    uint8_t *pRspbuf                       = NULL;
    size_t rspbufLen                       = 0;

    ENSURE_OR_GO_CLEANUP(session_ctx != NULL);

    SMLOG_D("APDU - Se05x_API_DeleteCryptoObject [] \n");

    pCmdbuf = &session_ctx->apdu_buffer[0];
    pRspbuf   = &session_ctx->apdu_buffer[0];
    rspbufLen = sizeof(session_ctx->apdu_buffer);

    tlvRet = TLVSET_U32("object id", &pCmdbuf, &cmdbufLen, kSE05x_TAG_1, objectID);
    if (0 != tlvRet) {
        goto cleanup;
    }
    retStatus = DoAPDUTxRx(session_ctx, &hdr, session_ctx->apdu_buffer, cmdbufLen, pRspbuf, &rspbufLen, 0);
    if (retStatus == SM_OK) {
        retStatus       = SM_NOT_OK;
        size_t rspIndex = 0;
        tlvRet          = tlvGet_U16(pRspbuf, &rspIndex, rspbufLen, kSE05x_TAG_1, psize); /* - */
        if (0 != tlvRet) {
            goto cleanup;
        }
        if ((rspIndex + 2) == rspbufLen) {
            retStatus = (smStatus_t)((pRspbuf[rspIndex] << 8) | (pRspbuf[rspIndex + 1]));
        }
    }

cleanup:
    return retStatus;
}
