/**
 * @file ecp_alt_se05x.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 *
 * Copyright 2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * @par Description
 * Implementation of ECP Sign ALT between NXP Secure Element and mbedTLS.
 *
 *****************************************************************************/

#include "common.h"
//#include <kernel.h>
//#include <logging/log.h>
//#include <stdio.h>
//#include <sm_port.h>

#if defined(MBEDTLS_ECP_C)

#include "mbedtls/ecp.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include "bn_mul.h"
#include "ecp_invasive.h"

#include <string.h>

#if defined(MBEDTLS_ECDSA_SIGN_ALT)

#include "se05x_mbedtls.h"
//#include "se05x_APDU_apis.h"
#include <SE05X.h>

extern int mbedtls_ecdsa_sign_o(mbedtls_ecp_group *grp,
    mbedtls_mpi *r,
    mbedtls_mpi *s,
    const mbedtls_mpi *d,
    const unsigned char *buf,
    size_t blen,
    int (*f_rng)(void *, unsigned char *, size_t),
    void *p_rng);

/** @brief Ec Signature To RandS.
 * This function generates RandS from signature.
 *
 * @param signature - Buffer containing the signature to read and verify.
 * @param sigLen - Size of signature in bytes.
 *
 * @returns Status of the operation
 * @retval 0 The operation has completed successfully.
 * @retval -1 The requested function could not be performed.
 */
static int EcSignatureToRandS(uint8_t *signature, size_t *sigLen)
{
    int result         = -1;
    uint8_t rands[128] = {0};
    int index          = 0;
    size_t i           = 0;
    size_t len         = 0;
    if (signature[index++] != 0x30) {
        goto exit;
    }
    if (signature[index++] != (*sigLen - 2)) {
        goto exit;
    }
    if (signature[index++] != 0x02) {
        goto exit;
    }

    len = signature[index++];
    if (len & 0x80) {
        /* Signature length will never exceed 0x7F */
        len = signature[index++];
    }
    if (len & 0x01) {
        len--;
        index++;
    }

    for (i = 0; i < len; i++) {
        rands[i] = signature[index++];
    }

    if (signature[index++] != 0x02) {
        goto exit;
    }

    len = signature[index++];
    if (len & 0x80) {
        /* Signature length will never exceed 0x7F */
        len = signature[index++];
    }
    if (len & 0x01) {
        len--;
        index++;
    }

    len = len + i;
    for (; i < len; i++) {
        rands[i] = signature[index++];
    }

    memcpy(&signature[0], &rands[0], i);
    *sigLen = i;

    result = 0;

exit:
    return result;
}

int mbedtls_ecdsa_sign(mbedtls_ecp_group *grp,
    mbedtls_mpi *r,
    mbedtls_mpi *s,
    const mbedtls_mpi *d,
    const unsigned char *buf,
    size_t blen,
    int (*f_rng)(void *, unsigned char *, size_t),
    void *p_rng)
{
    SMLOG_D("%s", __FUNCTION__);

    smStatus_t status = SM_NOT_OK;
    SE05x_Result_t result;
    SE05x_ECSignatureAlgo_t ecSignAlgo;
    uint32_t keyID          = 0;
    uint8_t magic_bytes[]   = ALT_KEYS_MAGIC;
    uint8_t buffer[150]     = {0};
    uint8_t signature[128]  = {0};
    size_t signature_len    = sizeof(signature);
    size_t rawPrivatekeylen = mbedtls_mpi_size(d);
    int ret                 = mbedtls_mpi_write_binary(d, buffer, rawPrivatekeylen);

    if (ret != 0) {
        SMLOG_E("Error %d\r\n", ret);
        return -1;
    }

    if (0 != memcmp(&buffer[rawPrivatekeylen - sizeof(magic_bytes) - 2], magic_bytes, sizeof(magic_bytes))) {
        SMLOG_I("Other key found !! Fallback on mbedtls\r\n");
        return mbedtls_ecdsa_sign_o(grp, r, s, d, buf, blen, f_rng, p_rng);
    }

    keyID = (uint32_t)((buffer[rawPrivatekeylen - sizeof(magic_bytes) - 2 - 4] << (8 * 3)) +
                       (buffer[rawPrivatekeylen - sizeof(magic_bytes) - 2 - 3] << (8 * 2)) +
                       (buffer[rawPrivatekeylen - sizeof(magic_bytes) - 2 - 2] << (8 * 1)) +
                       (buffer[rawPrivatekeylen - sizeof(magic_bytes) - 2 - 1] << (8 * 0)));

    status = se05x_open_session();
    if (status != SM_OK) {
        SMLOG_E("Failed to initialize SE05x session\r\n");
        return -1;
    }

    status = Se05x_API_CheckObjectExists(pSession, keyID, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists\r\n");
        return -1;
    }

    if (result != kSE05x_Result_SUCCESS) {
        SMLOG_E("Object not provisioned\r\n");
        return -1;
    }

    // decide on the algo based on the input size
    // (input being the hash) 
    switch(blen) {
        case 20:
            ecSignAlgo = kSE05x_ECSignatureAlgo_SHA;
            break;
        case 28:
            ecSignAlgo = kSE05x_ECSignatureAlgo_SHA_224;
            break;
        case 32:
            ecSignAlgo = kSE05x_ECSignatureAlgo_SHA_256;
            break;
        case 48:
            ecSignAlgo = kSE05x_ECSignatureAlgo_SHA_384;
            break;
        case 64:
            ecSignAlgo = kSE05x_ECSignatureAlgo_SHA_512;
            break;
        default:
            SMLOG_E("Unsupported hash length: %d\r\n",  blen);
            return -1;
    }

    SMLOG_I("Using SE05x for ecdsa sign. blen: %d\r\n", blen);
    status = Se05x_API_ECDSASign(
        pSession, keyID, ecSignAlgo, (uint8_t *)buf, blen, signature, &signature_len);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_ECDSASign\r\n");
        return -1;
    }

    ret = EcSignatureToRandS(signature, &signature_len);
    if (ret != 0) {
        SMLOG_E("EcSignatureToRandS failed\r\n");
        return -1;
    }

    if ((signature_len % 2) != 0) {
        SMLOG_E("Invalid R and S values\r\n");
        return -1;
    }

    ret = mbedtls_mpi_read_binary(r, (const unsigned char *)&signature[0], (signature_len / 2));
    if (ret != 0) {
        SMLOG_E("mbedtls_mpi_read_binary failed\r\n");
        return -1;
    }

    ret = mbedtls_mpi_read_binary(s, (const unsigned char *)&signature[signature_len / 2], (signature_len / 2));
    if (ret != 0) {
        SMLOG_E("mbedtls_mpi_read_binary failed\r\n");
        return -1;
    }

    return 0;
}

#endif /* MBEDTLS_ECDSA_SIGN_ALT */

#endif /* MBEDTLS_ECP_C */
