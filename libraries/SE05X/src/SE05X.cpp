/*
  SE05X.cpp
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

#include "SE05X.h"

/**
 * 26 bytes see ecc_der_header_nist256
 */
#define SE05X_EC_KEY_DER_HEADER_LENGTH   26

/**
 * 1 byte for key compression format 0x02 / 0x03 / 0x04
 */
#define SE05X_EC_KEY_FORMAT_LENGTH        1

/**
 * 64 bytes X Y uncompressed points
 */
#define SE05X_EC_KEY_RAW_LENGTH          64

/**
 * 91 bytes total key length in DER format
 */
#define SE05X_EC_KEY_DER_LENGTH          SE05X_EC_KEY_DER_HEADER_LENGTH + \
                                         SE05X_EC_KEY_FORMAT_LENGTH     + \
                                         SE05X_EC_KEY_RAW_LENGTH

/**
 * 32 bytes R values + 32 bytes S values
 */
#define SE05X_EC_SIGNATURE_RAW_LENGTH    64

/**
 * 8 bytes worst case 30 45 02 21 00 | 32 bytes R values | 02 21 00 | 32 bytes S values
 */
#define SE05X_EC_SIGNATURE_MAX_HEADER_LENGTH 8

/**
 * 6 bytes best case 30 45 02 21 | 32 bytes R values | 02 21 | 32 bytes S values
 */
#define SE05X_EC_SIGNATURE_MIN_HEADER_LENGTH 6

/**
 * 72 bytes worst case
 */
#define SE05X_EC_SIGNATURE_MAX_DER_LENGTH    SE05X_EC_SIGNATURE_MAX_HEADER_LENGTH + \
                                             SE05X_EC_SIGNATURE_RAW_LENGTH

/**
 * 70 bytes best case
 */
#define SE05X_EC_SIGNATURE_MIN_DER_LENGTH    SE05X_EC_SIGNATURE_MIN_HEADER_LENGTH + \
                                             SE05X_EC_SIGNATURE_RAW_LENGTH

#define SE05X_SHA256_LENGTH              32
#define SE05X_SN_LENGTH                  18

#define SE05X_TEMP_OBJECT                9999

#define SE05X_MAX_CHUNK_SIZE             100

static const byte ecc_der_header_nist256[SE05X_EC_KEY_DER_HEADER_LENGTH] =
{
    0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2a, 0x86,
    0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a,
    0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03,
    0x42, 0x00
};

SE05XClass::SE05XClass() { }
SE05XClass::~SE05XClass() { }

int SE05XClass::begin()
{
    smStatus_t status;

    pinMode(SE050_ENA_PIN, OUTPUT);
    digitalWrite(SE050_ENA_PIN, HIGH);

    _se05x_session = {0,0,{0,},0,0,0,0,0,0,0,0};

    status = Se05x_API_SessionOpen(&_se05x_session);
    if(status != SM_OK) {
        return 0;
    }

    return 1;
}

void SE05XClass::end()
{
    Se05x_API_SessionClose(&_se05x_session);
}

String SE05XClass::serialNumber()
{
    String result = (char*)NULL;
    byte UID[SE05X_SN_LENGTH];
    size_t uidLen = SE05X_SN_LENGTH;
    const int kSE05x_AppletResID_UNIQUE_ID = 0x7FFF0206,

    status = Se05x_API_ReadObject(&_se05x_session, kSE05x_AppletResID_UNIQUE_ID, 0, uidLen, UID, &uidLen);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_ReadObject \n");
        return "";
    }

    result.reserve(uidLen * 2);

    for (size_t i = 0; i < uidLen; i++) {
        byte b = UID[i];

        if (b < 16) {
          result += "0";
        }
        result += String(b, HEX);
    }

    result.toUpperCase();

    return result;
}

long SE05XClass::random(long max)
{
    return random(0, max);
}

long SE05XClass::random(long min, long max)
{
    if (min >= max)
    {
        return min;
    }

    long diff = max - min;

    long r;
    random((byte*)&r, sizeof(r));

    if (r < 0) {
        r = -r;
    }

    r = (r % diff);

    return (r + min);
}

int SE05XClass::random(byte data[], size_t length)
{
    smStatus_t status;
    uint16_t   offset = 0;
    uint16_t   left = length;
    
    while (left > 0) {
        uint16_t chunk     = (left > SE05X_MAX_CHUNK_SIZE) ? SE05X_MAX_CHUNK_SIZE : left;
        size_t max_buffer  = chunk;
        
        status = Se05x_API_GetRandom(&_se05x_session, chunk, (data + offset), &max_buffer);
        if (status != SM_OK) {
            SMLOG_E("Error in Se05x_API_GetRandom \n");
            return 0;
        }
        left   = left - chunk;
        offset = offset + chunk;
    }

    return 1;
}

int SE05XClass::generatePrivateKey(int keyID, byte keyBuf[], size_t keyBufMaxLen, size_t* keyLen)
{
    if (keyBufMaxLen < SE05X_EC_KEY_DER_LENGTH ) {
        SMLOG_E("Error in generatePrivateKey buffer length \n");
        return 0;
    }

    *keyLen = SE05X_EC_KEY_DER_LENGTH;

    /* Copy header byte from 0 to 25 */
    memcpy(keyBuf, ecc_der_header_nist256, sizeof(ecc_der_header_nist256));

    /* Add format byte */
    keyBuf[SE05X_EC_KEY_DER_HEADER_LENGTH] = 0x04;

    /* Add X Y points */
    return generatePrivateKey(keyID, &keyBuf[SE05X_EC_KEY_DER_HEADER_LENGTH + SE05X_EC_KEY_FORMAT_LENGTH]);
}

int SE05XClass::generatePrivateKey(int keyID, byte publicKey[])
{
    smStatus_t      status;
    SE05x_ECCurve_t curveID = kSE05x_ECCurve_NIST_P256;
    SE05x_Result_t  result;

    /* SE050 fills a buffer with 1 byte key format + 64 bytes of X Y points */
    uint8_t         keyBuf[SE05X_EC_KEY_FORMAT_LENGTH + SE05X_EC_KEY_RAW_LENGTH];
    size_t          keylen = sizeof(keyBuf);

    status = Se05x_API_CheckObjectExists(&_se05x_session, keyID, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        return 0;
    }

    if (result == kSE05x_Result_SUCCESS) {
        SMLOG_I("Object already exists \n");
        curveID = kSE05x_ECCurve_NA;
    }

    SMLOG_I("Generate ec key \n");
    status = Se05x_API_WriteECKey(&_se05x_session, NULL, 0, keyID, curveID, NULL, 0, NULL, 0, kSE05x_INS_NA, kSE05x_KeyPart_Pair);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_WriteECKey \n");
        return 0;
    }

    status = Se05x_API_ReadObject(&_se05x_session, keyID, 0, 0, keyBuf, &keylen);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_ReadObject \n");
        return 0;
    }

    /* To User: copy only 64 bytes of X Y points */
    memcpy(publicKey, &keyBuf[SE05X_EC_KEY_FORMAT_LENGTH], SE05X_EC_KEY_RAW_LENGTH);

    return 1;
}

int SE05XClass::generatePublicKey(int keyID, byte keyBuf[], size_t keyBufMaxLen, size_t* keyLen)
{
    if (keyBufMaxLen < SE05X_EC_KEY_DER_LENGTH ) {
        SMLOG_E("Error in generatePublicKey buffer too short \n");
        return 0;
    }

    *keyLen = SE05X_EC_KEY_DER_LENGTH;

    /* Copy header byte from 0 to 25 */
    memcpy(keyBuf, ecc_der_header_nist256, SE05X_EC_KEY_DER_HEADER_LENGTH);

    /* Add format byte */
    keyBuf[SE05X_EC_KEY_DER_HEADER_LENGTH] = 0x04;

    /* Add X Y points */
    return generatePublicKey(keyID, &keyBuf[SE05X_EC_KEY_DER_HEADER_LENGTH + SE05X_EC_KEY_FORMAT_LENGTH]);
}

int SE05XClass::generatePublicKey(int keyID, byte publicKey[])
{
    smStatus_t      status;
    SE05x_Result_t  result;

    /* SE050 fills a buffer with 1 byte key format + 64 bytes of X Y points */
    uint8_t         keyBuf[SE05X_EC_KEY_FORMAT_LENGTH + SE05X_EC_KEY_RAW_LENGTH];
    size_t          keyLen = sizeof(keyBuf);

    status = Se05x_API_CheckObjectExists(&_se05x_session, keyID, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        return 0;
    }

    status = Se05x_API_ReadObject(&_se05x_session, keyID, 0, 0, keyBuf, &keyLen);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_ReadObject \n");
        return 0;
    }

    /* To User: copy only 64 bytes of X Y points */
    memcpy(publicKey, &keyBuf[SE05X_EC_KEY_FORMAT_LENGTH], SE05X_EC_KEY_RAW_LENGTH);

    return 1;
}

int SE05XClass::importPublicKey(int keyID, const byte publicKey[], size_t keyLen)
{
    smStatus_t      status;
    SE05x_ECCurve_t curveID = kSE05x_ECCurve_NIST_P256;
    SE05x_Result_t  result;

    /* SE050 fills a buffer with 1 byte key format + 64 bytes of X Y points */
    uint8_t         keyBuf[SE05X_EC_KEY_FORMAT_LENGTH + SE05X_EC_KEY_RAW_LENGTH];

    if (keyLen < SE05X_EC_KEY_DER_LENGTH) {
        SMLOG_E("Error in importPublicKey invalid key length \n");
        return 0;
    }

    if (memcmp(ecc_der_header_nist256, publicKey, SE05X_EC_KEY_DER_HEADER_LENGTH)) {
        SMLOG_E("Error in importPublicKey invalid key format \n");
        return 0;
    }

    status = Se05x_API_CheckObjectExists(&_se05x_session, keyID, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        return 0;
    }

    if (result == kSE05x_Result_SUCCESS) {
        SMLOG_I("Object already exists \n");
        curveID = kSE05x_ECCurve_NA;
    }

    /* To SE050: copy 65 bytes Key format + 64 bytes of X Y points */
    memcpy(keyBuf, &publicKey[SE05X_EC_KEY_DER_HEADER_LENGTH], sizeof(keyBuf));

    SMLOG_I("Import ec key \n");
    status = Se05x_API_WriteECKey(&_se05x_session, NULL, 0, keyID, curveID, NULL, 0, keyBuf, sizeof(keyBuf), kSE05x_INS_WRITE, kSE05x_KeyPart_Public);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_WriteECKey \n");
        return 0;
    }

    return 1;
}

int SE05XClass::beginSHA256()
{
    smStatus_t      status;
    SE05x_CryptoModeSubType_t subtype;
    
    subtype.digest = kSE05x_DigestMode_SHA256;

    status = Se05x_API_CreateCryptoObject(&_se05x_session, kSE05x_CryptoObject_DIGEST_SHA256, kSE05x_CryptoContext_DIGEST, subtype);
    if (status != SM_OK) {
        SMLOG_W("Se05x_API_CreateCryptoObject failed. Object already exists? \n");
    }

    status = Se05x_API_DigestInit(&_se05x_session, kSE05x_CryptoObject_DIGEST_SHA256);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_DigestInit \n");
        return 0;
    }

    return 1;
}

int SE05XClass::updateSHA256(const byte in[], size_t inLen)
{
    smStatus_t      status;
    
    status = Se05x_API_DigestUpdate(&_se05x_session, kSE05x_CryptoObject_DIGEST_SHA256, in, inLen);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_DigestUpdate \n");
        return 0;
    }

    return 1;
}

int SE05XClass::endSHA256(byte out[], size_t* outLen)
{
    smStatus_t      status;

    if (*outLen < SE05X_SHA256_LENGTH) {
        SMLOG_E("Error in endSHA256 \n");
        *outLen = 0;
        return 0;  
    }

    status = Se05x_API_DigestFinal(&_se05x_session, kSE05x_CryptoObject_DIGEST_SHA256, NULL, 0, out, outLen);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_DigestFinal \n");
        *outLen = 0;
        return 0;
    }

    status = Se05x_API_DeleteCryptoObject(&_se05x_session, kSE05x_CryptoObject_DIGEST_SHA256);

    return 1;
}

int SE05XClass::SHA256(const byte in[], size_t inLen, byte out[], size_t outMaxLen, size_t* outLen)
{
    smStatus_t      status;

    *outLen = outMaxLen;
    status = Se05x_API_DigestOneShot(&_se05x_session,  kSE05x_DigestMode_SHA256, in, inLen, out, outLen);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_DigestOneShot \n");
        *outLen = 0;
        return 0;
    }

    return 1;
}

int SE05XClass::Sign(int keyID, const byte hash[], size_t hashLen, byte sig[], size_t sigMaxLen, size_t* sigLen)
{
    smStatus_t      status;
    SE05x_Result_t  result;

    if (hashLen != SE05X_SHA256_LENGTH) {
        SMLOG_E("Error in Sign invalid input SHA256 buffer \n");
        *sigLen = 0;
        return 0;
    }

    if (sigMaxLen < SE05X_EC_SIGNATURE_MAX_DER_LENGTH) {
        SMLOG_E("Error in Sign signature buffer too small \n");
        *sigLen = 0;
        return 0;
    }

    status = Se05x_API_CheckObjectExists(&_se05x_session, keyID, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        *sigLen = 0;
        return 0;
    }

    if (result != kSE05x_Result_SUCCESS) {
        SMLOG_E("Object not exists \n");
        *sigLen = 0;
        return 0;
    }

    *sigLen = sigMaxLen;
    status = Se05x_API_ECDSASign(&_se05x_session, keyID, kSE05x_ECSignatureAlgo_SHA_256, hash, hashLen, sig, sigLen);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_ECDSASign \n");
        return 0;
    }
    return 1;
}

int SE05XClass::ecSign(int slot, const byte message[], byte signature[])
{
    byte signatureDer[SE05X_EC_SIGNATURE_MAX_DER_LENGTH];
    size_t signatureDerLen;
    size_t size = SE05X_EC_SIGNATURE_RAW_LENGTH;

    if (!Sign(slot, message, SE05X_SHA256_LENGTH, signatureDer, sizeof(signatureDer), &signatureDerLen)) {
        SMLOG_E("Error in ecSign \n");
        return 0;
    }

    /* Get r s values from DER buffer */
    if (!getECSignatureRsValuesFromDER(signatureDer, signatureDerLen, signature, &size)) {
        SMLOG_E("Error in ecSign cannot get R S values\n");
        return 0;
    }

    return 1;
}

int SE05XClass::Verify(int keyID, const byte hash[], size_t hashLen, const byte sig[], size_t sigLen)
{
    smStatus_t      status;
    SE05x_Result_t  result;

    if (hashLen != SE05X_SHA256_LENGTH) {
        SMLOG_E("Error in Verify invalid input SHA256 buffer \n");
        return 0;
    }

    if ((sigLen < SE05X_EC_SIGNATURE_MIN_DER_LENGTH) || (sigLen > SE05X_EC_SIGNATURE_MAX_DER_LENGTH)) {
        SMLOG_E("Error in Verify invalid signature \n");
        return 0;
    }

    status = Se05x_API_CheckObjectExists(&_se05x_session, keyID, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        return 0;
    }

    if (result != kSE05x_Result_SUCCESS) {
        SMLOG_E("Object not exists \n");
        return 0;
    }

    status = Se05x_API_ECDSAVerify(&_se05x_session, keyID, kSE05x_ECSignatureAlgo_SHA_256, hash, hashLen, sig, sigLen, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_ECDSASign \n");
        return 0;
    }

    if (result != kSE05x_Result_SUCCESS) {
        SMLOG_E("Verify failure \n");
        return 0;
    }
    return 1;
}

int SE05XClass::ecdsaVerify(const byte message[], const byte signature[], const byte pubkey[])
{
    byte pubKeyDER[SE05X_EC_KEY_DER_LENGTH];
    size_t pubKeyDERLen = sizeof(pubKeyDER);
    byte signatureDER[SE05X_EC_SIGNATURE_MAX_DER_LENGTH];
    size_t signatureDERLen = sizeof(signatureDER);

    if (!setECKeyXyVauesInDER(pubkey, SE05X_EC_KEY_RAW_LENGTH, pubKeyDER, &pubKeyDERLen)) {
        SMLOG_E("ecdsaVerify failure creating key DER\n");
        return 0;
    }

    if (!importPublicKey(SE05X_TEMP_OBJECT, pubKeyDER, pubKeyDERLen)) {
        SMLOG_E("ecdsaVerify failure importing temp key\n");
        return 0;
    }

    if (!setECSignatureRsValuesInDER(signature, SE05X_EC_SIGNATURE_RAW_LENGTH, signatureDER, &signatureDERLen)) {
        SMLOG_E("ecdsaVerify failure creating signature DER\n");
        return 0;
    }

    if (!Verify(SE05X_TEMP_OBJECT, message, SE05X_SHA256_LENGTH, signatureDER, SE05X_EC_SIGNATURE_MAX_DER_LENGTH)) {
        SMLOG_E("ecdsaVerify failure\n");
        return 0;
    }

    if (!deleteBinaryObject(SE05X_TEMP_OBJECT)) {
        SMLOG_E("ecdsaVerify failure deleting temporary object\n");     
        return 0;
    }

    return 1;
}

int SE05XClass::readBinaryObject(int objectId, byte data[], size_t dataMaxLen, size_t* length)
{
    smStatus_t      status;
    SE05x_Result_t  result;
    uint16_t        offset = 0;
    uint16_t        size;

    status = Se05x_API_CheckObjectExists(&_se05x_session, objectId, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        *length = 0;
        return 0;
    }

    if (result != kSE05x_Result_SUCCESS) {
        SMLOG_E("Object not exists \n");
        *length = 0;
        return 0;
    }

    status = Se05x_API_ReadSize(&_se05x_session, objectId, &size);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        *length = 0;
        return 0;
    }

    if (dataMaxLen < size) {
        SMLOG_E("Error in readBinaryObject buffer too small \n");
        *length = 0;
        return 0;
    }

    uint16_t left = size;
    while (left > 0) {
        uint16_t chunk     = (left > SE05X_MAX_CHUNK_SIZE) ? SE05X_MAX_CHUNK_SIZE : left;
        size_t max_buffer  = chunk;
        
        status = Se05x_API_ReadObject(&_se05x_session, objectId, offset, chunk, (data + offset), &max_buffer);
        if (status != SM_OK) {
            SMLOG_E("Error in Se05x_API_ReadObject \n");
            *length = 0;
            return 0;
        }
        left   = left - chunk;
        offset = offset + chunk;
    }

    *length = size;
    return 1;
}

int SE05XClass::readSlot(int slot, byte data[], int length)
{
    size_t  size;
    return readBinaryObject(slot, data, length, &size);
}

int SE05XClass::AES_ECB_encrypt(int objectId, const byte data[], size_t data_length, byte output[], size_t *output_len)
{
    smStatus_t status;
    status = Se05x_API_CipherOneShot(&_se05x_session, objectId, kSE05x_CipherMode_AES_ECB_NOPAD, data, data_length, 0, 0, output, output_len, kSE05x_Cipher_Oper_OneShot_Encrypt);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CipherOneShot \n");
        return 0;
    }
    return 1;
}

int SE05XClass::AES_ECB_decrypt(int objectId, const byte data[], size_t data_length, byte output[], size_t *output_len)
{
    smStatus_t status;
    status = Se05x_API_CipherOneShot(&_se05x_session, objectId, kSE05x_CipherMode_AES_ECB_NOPAD, data, data_length, 0, 0, output, output_len, kSE05x_Cipher_Oper_OneShot_Decrypt);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CipherOneShot \n");
        return 0;
    }
    return 1;
}

int SE05XClass::writeAESKey(int objectId, const byte data[], size_t length)
{
    smStatus_t      status;
    SE05x_Result_t  result;
    uint16_t        offset = 0;
    uint16_t        size;

    status = Se05x_API_CheckObjectExists(&_se05x_session, objectId, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        return 0;
    }

    if (result == kSE05x_Result_SUCCESS) {
        SMLOG_E("Object exists \n");
        return 0;
    }

    uint16_t left = length;

    status = Se05x_API_WriteSymmKey(&_se05x_session, NULL, 3, objectId, NULL, data, length, kSE05x_INS_NA, kSE05x_SymmKeyType_AES);

    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_WriteSymmKey \n");
        return 0;
    }
    return 1;
}

int SE05XClass::writeHMACKey(int objectId, const byte data[], size_t length)
{
    smStatus_t      status;
    SE05x_Result_t  result;
    uint8_t         exists = 0;
    uint16_t        offset = 0;
    uint16_t        size;

    status = Se05x_API_CheckObjectExists(&_se05x_session, objectId, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        return 0;
    }

    if (result == kSE05x_Result_SUCCESS) {
        SMLOG_E("Object exists \n");
        exists = 1;
    }

    status = Se05x_API_WriteSymmKey(&_se05x_session, NULL, 0, objectId, SE05x_KeyID_KEK_NONE, data, length, kSE05x_INS_NA, kSE05x_SymmKeyType_HMAC);

    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_WriteSymmKey \n");
        return 0;
    }
    return 1;
}

int SE05XClass::HMAC_Generate(int objectId, uint8_t mac_operation, const byte data[], size_t data_length, byte output[], size_t *output_len)
{
    smStatus_t status;
    status = Se05x_API_MACOneShot_G(&_se05x_session, objectId, mac_operation, data, data_length, output, output_len);

    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CipherOneShot \n");
        return status;
    }
    return 1;
}

int SE05XClass::writeBinaryObject(int objectId, const byte data[], size_t length)
{
    smStatus_t      status;
    SE05x_Result_t  result;
    uint8_t         exists = 0;
    uint16_t        offset = 0;
    uint16_t        size;

    status = Se05x_API_CheckObjectExists(&_se05x_session, objectId, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        return 0;
    }

    if (result == kSE05x_Result_SUCCESS) {
        SMLOG_E("Object exists \n");
        exists = 1;
    }

    uint16_t left = length;

    while (left > 0) {
        uint16_t chunk = (left > SE05X_MAX_CHUNK_SIZE) ? SE05X_MAX_CHUNK_SIZE : left;
        left           = left - chunk;
        size           = exists ? 0 : length;

        status = Se05x_API_WriteBinary(&_se05x_session, NULL, objectId, offset, size, (data + offset), chunk);
        if (status != SM_OK) {
            SMLOG_E("Error in Se05x_API_WriteBinary \n");
            return 0;
        }
        exists = 1;
        offset = offset + chunk;
    }

    return 1;
}

int SE05XClass::writeSlot(int slot, const byte data[], int length)
{
    if (existsBinaryObject(slot)) {
        if (!deleteBinaryObject(slot)) {
            return 0;
        }
    }
    return writeBinaryObject(slot, data, length);
}

int SE05XClass::existsBinaryObject(int objectId)
{
    smStatus_t      status;
    SE05x_Result_t  result;

    status = Se05x_API_CheckObjectExists(&_se05x_session, objectId, &result);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        return 0;
    }

    if (result != kSE05x_Result_SUCCESS) {
        SMLOG_E("Object not exists \n");
        return 0;
    }

    return 1;
}

int SE05XClass::deleteBinaryObject(int objectId)
{
    smStatus_t      status;

    status = Se05x_API_DeleteSecureObject(&_se05x_session, objectId);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_DeleteSecureObject \n");
        return 0;
    }

    return 1;
}

int SE05XClass::deleteAllObjects(void)
{
    smStatus_t      status;

    status = Se05x_API_DeleteAll_Iterative(&_se05x_session);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_CheckObjectExists \n");
        return 0;
    }

    return 1;
}

int SE05XClass::getECKeyXyValuesFromDER(byte* derKey, size_t derLen, byte* rawKey, size_t* rawLen)
{
    if(*rawLen < SE05X_EC_KEY_RAW_LENGTH) {
        SMLOG_E("Error in getECKeyXyValuesFromDER \n");
        *rawLen = 0;
        return 0;    
    }
    
    /* XY values are stored in the last 64 bytes of DER buffer */
    *rawLen = SE05X_EC_KEY_RAW_LENGTH;
    memcpy(rawKey, &derKey[derLen - SE05X_EC_KEY_RAW_LENGTH], SE05X_EC_KEY_RAW_LENGTH);

    return 1;
}

int SE05XClass::setECKeyXyVauesInDER(const byte* rawKey, size_t rawLen, byte* derKey, size_t* derLen)
{
    if(rawLen != SE05X_EC_KEY_RAW_LENGTH) {
        SMLOG_E("Error in setECKeyXyVauesInDER invalid raw key\n");
        *derLen = 0;
        return 0;    
    }

    if(*derLen < SE05X_EC_KEY_DER_LENGTH) {
        SMLOG_E("Error in setECKeyXyVauesInDER buffer too small\n");
        *derLen = 0;
        return 0;    
    }
    
    /* Copy header byte from 0 to 25 */
    memcpy(&derKey[0], &ecc_der_header_nist256[0], SE05X_EC_KEY_DER_HEADER_LENGTH);
    /* Add format byte */
    derKey[SE05X_EC_KEY_DER_HEADER_LENGTH] = 0x04;
    /* Add X Y points */
    memcpy(&derKey[SE05X_EC_KEY_DER_HEADER_LENGTH + SE05X_EC_KEY_FORMAT_LENGTH], &rawKey[0], SE05X_EC_KEY_RAW_LENGTH);

    *derLen = SE05X_EC_KEY_DER_LENGTH;
    return 1;
}

int SE05XClass::getECSignatureRsValuesFromDER(byte* derSignature, size_t derLen, byte* rawSignature, size_t *rawLen)
{
    byte rLen;
    byte sLen;

    if ((derLen < SE05X_EC_SIGNATURE_MIN_DER_LENGTH) || (derLen > SE05X_EC_SIGNATURE_MAX_DER_LENGTH)) {
        SMLOG_E("Error in getECSignatureRsValuesFromDER invalid signature\n");
        *rawLen = 0;
        return 0;  
    }

    if (*rawLen < SE05X_EC_SIGNATURE_RAW_LENGTH) {
        SMLOG_E("Error in getECSignatureRsValuesFromDER buffer too small\n");
        *rawLen = 0;
        return 0;  
    }

    rLen = derSignature[3];
    sLen = derSignature[3 + rLen + 2];

    byte * out = rawSignature;

    if(rLen == (SE05X_EC_SIGNATURE_RAW_LENGTH / 2))
    {
        memcpy(out, &derSignature[4], (SE05X_EC_SIGNATURE_RAW_LENGTH / 2));
    }
    else if ((rLen == ((SE05X_EC_SIGNATURE_RAW_LENGTH / 2) + 1)) && (derSignature[4] == 0))
    {
        memcpy(out, &derSignature[5], (SE05X_EC_SIGNATURE_RAW_LENGTH / 2));
    }

    out += (SE05X_EC_SIGNATURE_RAW_LENGTH / 2);

    if(sLen == (SE05X_EC_SIGNATURE_RAW_LENGTH / 2))
    {
        memcpy(out, &derSignature[3 + rLen + 3], (SE05X_EC_SIGNATURE_RAW_LENGTH / 2));
    }
    else if ((sLen == ((SE05X_EC_SIGNATURE_RAW_LENGTH / 2) + 1)) && (derSignature[3 + rLen + 3] == 0))
    {
        memcpy(out, &derSignature[3 + rLen + 4], (SE05X_EC_SIGNATURE_RAW_LENGTH / 2));
    }

    return 1;
}

int SE05XClass::setECSignatureRsValuesInDER(const byte* rawSignature, size_t rawLen, byte* signature, size_t* derLen)
{
    /**
     * Always consider worst case with padding
     * 
     * | 0x30 0x46 0x02 0x21 0x00 | R values 32 bytes | 0x02 0x21 0x00 | S values 32 bytes |
     *
     */
    const int halfSigLen = SE05X_EC_SIGNATURE_RAW_LENGTH / 2;

    if (rawLen != SE05X_EC_SIGNATURE_RAW_LENGTH) {
        SMLOG_E("Error in setECSignatureRsValuesInDER invalid signature\n");
        *derLen = 0;
        return 0;
    }

    if (*derLen < SE05X_EC_SIGNATURE_MAX_DER_LENGTH) {
        SMLOG_E("Error in setECSignatureRsValuesInDER buffer too small\n");
        *derLen = 0;
        return 0;
    }

    signature[0] = 0x30;
    signature[1] = 0x46; /* 3 + 32 + 3 + 32*/
    signature[2] = 0x02;
    signature[3] = 0x21;
    signature[4] = 0x00;
    memcpy(&signature[5], &rawSignature[0], halfSigLen);
    signature[37] = 0x02;
    signature[38] = 0x21;
    signature[39] = 0x00;
    memcpy(&signature[40], &rawSignature[halfSigLen], halfSigLen);

    return 1;
}

SE05XClass SE05X;
