/** @file se05x_APDU_apis.h
 *  @brief Se05x apdu functions.
 *
 * Copyright 2021,2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SE05X_APDU_APIS_H_INC
#define SE05X_APDU_APIS_H_INC

/* ********************** Include files ********************** */
#include "se05x_types.h"
#include "se05x_tlv.h"

/** Se05x_API_SessionOpen
 *
 * Open session to SE05x.
 * Multiple sessions are not supported.
 *
 * @param[in]  session_ctx  The session context
 *
 * @return     The sm status.
 */
smStatus_t Se05x_API_SessionOpen(pSe05xSession_t session_ctx);

/** Se05x_API_SessionClose
 *
 * Close session to SE05x.
 *
 * @param[in]  session_ctx  The session context
 *
 * @return     The sm status.
 */
smStatus_t Se05x_API_SessionClose(pSe05xSession_t session_ctx);

/** Se05x_API_WriteECKey
 *
 * Write or update an EC key object.
 *
 * P1KeyPart indicates the key type to be created (if the object does not yet
 * exist).
 *
 * If P1KeyPart = P1_KEY_PAIR, Private Key Value (TLV[TAG_3]) and Public Key
 * Value (TLV[TAG_4) must both be present, or both be absent. If absent, the key
 * pair is generated in the SE05X .
 *
 * If the object already exists, P1KeyPart is ignored.
 *
 * @rst
 * +---------+------------------------+------------------------------------------------+
 * | Field   | Value                  | Description                                    |
 * +=========+========================+================================================+
 * | P1      | :cpp:type:`SE05x_P1_t` | See  :cpp:type:`SE05x_P1_t` ,  P1KeyType       |
 * |         | | P1_EC                | should only be set for new objects.            |
 * +---------+------------------------+------------------------------------------------+
 * | P2      | P2_DEFAULT             | See P2                                         |
 * +---------+------------------------+------------------------------------------------+
 * | Payload | TLV[TAG_POLICY]        | Byte array containing the object policy.       |
 * |         |                        | [Optional: default policy applies]             |
 * |         |                        | [Conditional - only when the object            |
 * |         |                        | identifier is not in use yet]                  |
 * +---------+------------------------+------------------------------------------------+
 * |         | TLV[TAG_MAX_ATTEMPTS]  | 2-byte maximum number of attempts. If 0 is     |
 * |         |                        | given, this means unlimited.   [Optional:      |
 * |         |                        | default unlimited]   [Conditional: only when   |
 * |         |                        | the object  identifier is not in use yet and   |
 * |         |                        | INS includes  INS_AUTH_OBJECT; see             |
 * |         |                        | AuthenticationObjectPolicies ]                 |
 * +---------+------------------------+------------------------------------------------+
 * |         | TLV[TAG_1]             | 4-byte object identifier                       |
 * +---------+------------------------+------------------------------------------------+
 * |         | TLV[TAG_2]             | 1-byte curve identifier, see ECCurve           |
 * |         |                        | [Conditional: only when the object  identifier |
 * |         |                        | is not in use yet; ]                           |
 * +---------+------------------------+------------------------------------------------+
 * |         | TLV[TAG_3]             | Private key value (see  :cpp:type:`ECKeyRef`   |
 * |         |                        | )   [Conditional: only when the private key is |
 * |         |                        | externally generated and P1KeyType is either   |
 * |         |                        | P1_KEY_PAIR  or P1_PRIVATE]                    |
 * +---------+------------------------+------------------------------------------------+
 * |         | TLV[TAG_4]             | Public key value (see  :cpp:type:`ECKeyRef`  ) |
 * |         |                        | [Conditional: only when the public key is      |
 * |         |                        | externally generated and P1KeyType is either   |
 * |         |                        | P1_KEY_PAIR  or P1_PUBLIC]                     |
 * +---------+------------------------+------------------------------------------------+
 * |         | TLV[TAG_11]            | 4-byte version    [Optional]                   |
 * +---------+------------------------+------------------------------------------------+
 * @endrst
 *
 * @param[in]  session_ctx  The session context
 * @param[in]  policy       The policy
 * @param[in]  maxAttempt   The maximum attempt
 * @param[in]  objectID     The object id
 * @param[in]  curveID      The curve id
 * @param[in]  privKey      The priv key
 * @param[in]  privKeyLen   The priv key length
 * @param[in]  pubKey       The pub key
 * @param[in]  pubKeyLen    The pub key length
 * @param[in]  ins_type     The insert type
 * @param[in]  key_part     The key part
 *
 * @return     The sm status.
 */
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
    const SE05x_KeyPart_t key_part);

/** Se05x_API_ReadObject
 *
 * Reads the content of a Secure Object.
 *
 *  * If the object is a key pair, the command will return the key
 *    pair's public key.
 *
 *  * If the object is a public key, the command will return the public
 *    key.
 *
 *  * If the object is a private key or a symmetric key or a userID,
 *    the command will return SW_CONDITIONS_NOT_SATISFIED.
 *
 *  * If the object is a binary file, the file content is read, giving
 *    the offset in TLV[TAG_2] and the length to read in
 *    TLV[TAG_3]. Both TLV[TAG_2] and TLV[TAG_3] are bound together;
 *    i.e.. either both tags are present, or both are absent. If both
 *    are absent, the whole file content is returned.
 *
 * Command to Applet
 *
 * @rst
 * +-------+------------+----------------------------------------------+
 * | Field | Value      | Description                                  |
 * +=======+============+==============================================+
 * | CLA   | 0x80       |                                              |
 * +-------+------------+----------------------------------------------+
 * | INS   | INS_READ   | See :cpp:type:`SE05x_INS_t`, in addition to  |
 * |       |            | INS_READ, users can set the INS_ATTEST flag. |
 * |       |            | In that case, attestation applies.           |
 * +-------+------------+----------------------------------------------+
 * | P1    | P1_DEFAULT | See :cpp:type:`SE05x_P1_t`                   |
 * +-------+------------+----------------------------------------------+
 * | P2    | P2_DEFAULT | See :cpp:type:`SE05x_P2_t`                   |
 * +-------+------------+----------------------------------------------+
 * | Lc    | #(Payload) | Payload Length.                              |
 * +-------+------------+----------------------------------------------+
 * |       | TLV[TAG_1] | 4-byte object identifier                     |
 * +-------+------------+----------------------------------------------+
 * |       | TLV[TAG_2] | 2-byte offset   [Optional: default 0]        |
 * |       |            | [Conditional: only when the object is a      |
 * |       |            | BinaryFile object]                           |
 * +-------+------------+----------------------------------------------+
 * |       | TLV[TAG_3] | 2-byte length   [Optional: default 0]        |
 * |       |            | [Conditional: only when the object is a      |
 * |       |            | BinaryFile object]                           |
 * +-------+------------+----------------------------------------------+
 * |       | TLV[TAG_4] | 1-byte :cpp:type:`SE05x_RSAKeyComponent_t`:  |
 * |       |            | either RSA_COMP_MOD or RSA_COMP_PUB_EXP.     |
 * |       |            | [Optional]   [Conditional: only for RSA key  |
 * |       |            | components]                                  |
 * +-------+------------+----------------------------------------------+
 * | Le    | 0x00       |                                              |
 * +-------+------------+----------------------------------------------+
 * @endrst
 *
 * R-APDU Body
 *
 * @rst
 * +------------+--------------------------------------------+
 * | Value      | Description                                |
 * +============+============================================+
 * | TLV[TAG_1] | Data read from the secure object.          |
 * +------------+--------------------------------------------+
 * @endrst
 *
 * R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------+
 * | SW          | Description                    |
 * +=============+================================+
 * | SW_NO_ERROR | The read is done successfully. |
 * +-------------+--------------------------------+
 * @endrst
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[in] offset offset [2:kSE05x_TAG_2]
 * @param[in] length length [3:kSE05x_TAG_3]
 * @param[out] data  [0:kSE05x_TAG_1]
 * @param[in,out] pdataLen Length for data
 */
smStatus_t Se05x_API_ReadObject(
    pSe05xSession_t session_ctx, uint32_t objectID, uint16_t offset, uint16_t length, uint8_t *data, size_t *pdataLen);

/** Se05x_API_GetVersion
 *
 * Gets the applet version information.
 *
 * This will return 7-byte VersionInfo (including major, minor and patch version
 * of the applet, supported applet features and secure box version).
 *
 * Command to Applet
 *
 * @rst
 * +-------+------------------------------+----------------------------------------------+
 * | Field | Value                        | Description                                  |
 * +=======+==============================+==============================================+
 * | CLA   | 0x80                         |                                              |
 * +-------+------------------------------+----------------------------------------------+
 * | INS   | INS_MGMT                     | See :cpp:type:`SE05x_INS_t`                  |
 * +-------+------------------------------+----------------------------------------------+
 * | P1    | P1_DEFAULT                   | See :cpp:type:`SE05x_P1_t`                   |
 * +-------+------------------------------+----------------------------------------------+
 * | P2    | P2_VERSION or P2_VERSION_EXT | See :cpp:type:`SE05x_P2_t`                   |
 * +-------+------------------------------+----------------------------------------------+
 * | Lc    | #(Payload)                   |                                              |
 * +-------+------------------------------+----------------------------------------------+
 * | Le    | 0x00                         | Expecting TLV with 7-byte data  (when P2 =   |
 * |       |                              | P2_VERSION) or a TLV with 37 byte data (when |
 * |       |                              | P2=  P2_VERSION_EXT).                        |
 * +-------+------------------------------+----------------------------------------------+
 * @endrst
 *
 *
 * R-APDU Body
 *
 * @rst
 * +------------+------------------------------------------------+
 * | Value      | Description                                    |
 * +============+================================================+
 * | TLV[TAG_1] | 7-byte :cpp:type:`VersionInfoRef` (if P2 =     |
 * |            | P2_VERSION) or 7-byte  VersionInfo followed by |
 * |            | 30 bytes extendedFeatureBits (if P2 =          |
 * |            | P2_VERSION_EXT)                                |
 * +------------+------------------------------------------------+
 * @endrst
 *
 * R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------+
 * | SW          | Description                    |
 * +=============+================================+
 * | SW_NO_ERROR | Data is returned successfully. |
 * +-------------+--------------------------------+
 * @endrst
 *
 * @param[in]  session_ctx       The session context
 * @param      pappletVersion    The papplet version
 * @param      appletVersionLen  The applet version length
 *
 * @return     The sm status.
 */
smStatus_t Se05x_API_GetVersion(pSe05xSession_t session_ctx, uint8_t *pappletVersion, size_t *appletVersionLen);

/** Se05x_API_ECDSASign
 *
 * The ECDSASign command signs external data using the indicated key pair or
 * private key.
 *
 * The ECSignatureAlgo indicates the ECDSA algorithm that is used, but the
 * hashing of data always must be done on the host. E.g., if ECSignatureAlgo =
 * SIG_ ECDSA_SHA256, the user must have applied SHA256 on the input data
 * already. Supported SHA algorithms - SHA1, SHA224, SHA256, SHA384.
 *
 * The user must take care of providing the correct input length; i.e., the data
 * input length (TLV[TAG_3]) must match the digest indicated in the signature
 * algorithm (TLV[TAG_2]).
 *
 * In any case, the APDU payload must be smaller than MAX_APDU_PAYLOAD_LENGTH.
 *
 * This is performed according to the ECDSA algorithm as specified in [ANSI
 * X9.62]. The signature (a sequence of two integers 'r' and 's') as
 * returned in the response adheres to the ASN.1 DER encoded formatting rules for
 * integers.
 *
 * Command to Applet
 *
 * @rst
 * +-------+--------------+---------------------------------------------+
 * | Field | Value        | Description                                 |
 * +=======+==============+=============================================+
 * | CLA   | 0x80         |                                             |
 * +-------+--------------+---------------------------------------------+
 * | INS   | INS_CRYPTO   | :cpp:type:`SE05x_INS_t`                     |
 * +-------+--------------+---------------------------------------------+
 * | P1    | P1_SIGNATURE | See :cpp:type:`SE05x_P1_t`                  |
 * +-------+--------------+---------------------------------------------+
 * | P2    | P2_SIGN      | See :cpp:type:`SE05x_P2_t`                  |
 * +-------+--------------+---------------------------------------------+
 * | Lc    | #(Payload)   |                                             |
 * +-------+--------------+---------------------------------------------+
 * |       | TLV[TAG_1]   | 4-byte identifier of EC key pair or private |
 * |       |              | key.                                        |
 * +-------+--------------+---------------------------------------------+
 * |       | TLV[TAG_2]   | 1-byte ECSignatureAlgo.                     |
 * +-------+--------------+---------------------------------------------+
 * |       | TLV[TAG_3]   | Byte array containing input data.           |
 * +-------+--------------+---------------------------------------------+
 * | Le    | 0x00         | Expecting ASN.1 signature                   |
 * +-------+--------------+---------------------------------------------+
 * @endrst
 *
 * R-APDU Body
 *
 * @rst
 * +------------+----------------------------------+
 * | Value      | Description                      |
 * +============+==================================+
 * | TLV[TAG_1] | ECDSA Signature in ASN.1 format. |
 * +------------+----------------------------------+
 * @endrst
 *
 * R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------------+
 * | SW          | Description                          |
 * +=============+======================================+
 * | SW_NO_ERROR | The command is handled successfully. |
 * +-------------+--------------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] ecSignAlgo ecSignAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] signature  [0:kSE05x_TAG_1]
 * @param[in,out] psignatureLen Length for signature
 */
smStatus_t Se05x_API_ECDSASign(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_ECSignatureAlgo_t ecSignAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *signature,
    size_t *psignatureLen);

/** Se05x_API_ECDSAVerify
 *
 * The ECDSAVerify command verifies whether the signature is correct for a given
 * (hashed) data input using an EC public key or EC key pair's public key.
 *
 * The ECSignatureAlgo indicates the ECDSA algorithm that is used, but the
 * hashing of data must always be done on the host. E.g., if ECSignatureAlgo =
 * SIG_ ECDSA_SHA256, the user must have applied SHA256 on the input data
 * already. Supported SHA algorithms - SHA1, SHA224, SHA256, SHA384.
 *
 * The key cannot be passed externally to the command directly. In case users
 * want to use the command to verify signatures using different public keys or
 * the public key value regularly changes, the user should create a transient key
 * object to which the key value is written and then the identifier of that
 * transient secure object can be used by this ECDSAVerify command.
 *
 * Command to Applet
 *
 * @rst
 * +-------+--------------+-----------------------------------------------+
 * | Field | Value        | Description                                   |
 * +=======+==============+===============================================+
 * | CLA   | 0x80         |                                               |
 * +-------+--------------+-----------------------------------------------+
 * | INS   | INS_CRYPTO   | :cpp:type:`SE05x_INS_t`                       |
 * +-------+--------------+-----------------------------------------------+
 * | P1    | P1_SIGNATURE | See :cpp:type:`SE05x_P1_t`                    |
 * +-------+--------------+-----------------------------------------------+
 * | P2    | P2_VERIFY    | See :cpp:type:`SE05x_P2_t`                    |
 * +-------+--------------+-----------------------------------------------+
 * | Lc    | #(Payload)   |                                               |
 * +-------+--------------+-----------------------------------------------+
 * |       | TLV[TAG_1]   | 4-byte identifier of the key pair or public   |
 * |       |              | key.                                          |
 * +-------+--------------+-----------------------------------------------+
 * |       | TLV[TAG_2]   | 1-byte ECSignatureAlgo.                       |
 * +-------+--------------+-----------------------------------------------+
 * |       | TLV[TAG_3]   | Byte array containing ASN.1 signature         |
 * +-------+--------------+-----------------------------------------------+
 * |       | TLV[TAG_5]   | Byte array containing hashed data to compare. |
 * +-------+--------------+-----------------------------------------------+
 * | Le    | 0x03         | Expecting TLV with :cpp:type:`SE05x_Result_t` |
 * +-------+--------------+-----------------------------------------------+
 * @endrst
 *
 * R-APDU Body
 *
 * @rst
 * +------------+--------------------------------------+
 * | Value      | Description                          |
 * +============+======================================+
 * | TLV[TAG_1] | Result of the signature verification |
 * |            | (:cpp:type:`SE05x_Result_t`).        |
 * +------------+--------------------------------------+
 * @endrst
 *
 * R-APDU Trailer
 *
 * @rst
 * +-----------------------------+--------------------------------------+
 * | SW                          | Description                          |
 * +=============================+======================================+
 * | SW_NO_ERROR                 | The command is handled successfully. |
 * +-----------------------------+--------------------------------------+
 * | SW_CONDITIONS_NOT_SATISFIED | Incorrect data                       |
 * +-----------------------------+--------------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] ecSignAlgo ecSignAlgo [2:kSE05x_TAG_2]
 * @param[in] inputData inputData [3:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[in] signature signature [4:kSE05x_TAG_5]
 * @param[in] signatureLen Length of signature
 * @param[out] presult  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_ECDSAVerify(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_ECSignatureAlgo_t ecSignAlgo,
    const uint8_t *inputData,
    size_t inputDataLen,
    const uint8_t *signature,
    size_t signatureLen,
    SE05x_Result_t *presult);

/** Se05x_API_CheckObjectExists
 *
 *
 * Check if a Secure Object with a certain identifier exists or not.
 *
 * Command to Applet
 *
 * @rst
 * +-------+------------+-------------------------------------------+
 * | Field | Value      | Description                               |
 * +=======+============+===========================================+
 * | CLA   | 0x80       |                                           |
 * +-------+------------+-------------------------------------------+
 * | INS   | INS_MGMT   | See :cpp:type:`SE05x_INS_t`               |
 * +-------+------------+-------------------------------------------+
 * | P1    | P1_DEFAULT | See :cpp:type:`SE05x_P1_t`                |
 * +-------+------------+-------------------------------------------+
 * | P2    | P2_EXIST   | See :cpp:type:`SE05x_P2_t`                |
 * +-------+------------+-------------------------------------------+
 * | Lc    | #(Payload) |                                           |
 * +-------+------------+-------------------------------------------+
 * |       | TLV[TAG_1] | 4-byte existing Secure Object identifier. |
 * +-------+------------+-------------------------------------------+
 * | Le    | 0x00       |                                           |
 * +-------+------------+-------------------------------------------+
 * @endrst
 *
 * R-APDU Body
 *
 * @rst
 * +------------+-----------------------------------+
 * | Value      | Description                       |
 * +============+===================================+
 * | TLV[TAG_1] | 1-byte :cpp:type:`SE05x_Result_t` |
 * +------------+-----------------------------------+
 * @endrst
 *
 * R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------+
 * | SW          | Description                    |
 * +=============+================================+
 * | SW_NO_ERROR | Data is returned successfully. |
 * +-------------+--------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 * @param[out] presult  [0:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_CheckObjectExists(pSe05xSession_t session_ctx, uint32_t objectID, SE05x_Result_t *presult);

/** Se05x_API_WriteBinary
 *
 * Creates or writes to a binary file object. Data are written to either the
 * start of the file or (if specified) to the offset passed to the function.
 *
 * Command to Applet
 *
 * @rst
 * +---------+-----------------+-----------------------------------------------+
 * | Field   | Value           | Description                                   |
 * +=========+=================+===============================================+
 * | P1      | P1_BINARY       | See :cpp:type:`SE05x_P1_t`                    |
 * +---------+-----------------+-----------------------------------------------+
 * | P2      | P2_DEFAULT      | See :cpp:type:`SE05x_P2_t`                    |
 * +---------+-----------------+-----------------------------------------------+
 * | Payload | TLV[TAG_POLICY] | Byte array containing the object policy.      |
 * |         |                 | [Optional: default policy applies]            |
 * |         |                 | [Conditional: only when the object identifier |
 * |         |                 | is not in use yet]                            |
 * +---------+-----------------+-----------------------------------------------+
 * |         | TLV[TAG_1]      | 4-byte object identifier                      |
 * +---------+-----------------+-----------------------------------------------+
 * |         | TLV[TAG_2]      | 2-byte file offset   [Optional: default = 0]  |
 * +---------+-----------------+-----------------------------------------------+
 * |         | TLV[TAG_3]      | 2-byte file length (up to 0x7FFF).            |
 * |         |                 | [Conditional: only when the object identifier |
 * |         |                 | is not in use yet]                            |
 * +---------+-----------------+-----------------------------------------------+
 * |         | TLV[TAG_4]      | Data to be written   [Optional: if not given, |
 * |         |                 | TAG_3 must be filled]                         |
 * +---------+-----------------+-----------------------------------------------+
 * |         | TLV[TAG_11]     | 4-byte version    [Optional]                  |
 * +---------+-----------------+-----------------------------------------------+
 * @endrst
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] policy policy [1:kSE05x_TAG_POLICY]
 * @param[in] objectID object id [2:kSE05x_TAG_1]
 * @param[in] offset offset [3:kSE05x_TAG_2]
 * @param[in] length length [4:kSE05x_TAG_3]
 * @param[in] inputData input data. (Max - 128 Bytes) [5:kSE05x_TAG_4]
 * @param[in] inputDataLen Length of inputData
 */
smStatus_t Se05x_API_WriteBinary(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    uint32_t objectID,
    uint16_t offset,
    uint16_t length,
    const uint8_t *inputData,
    size_t inputDataLen);

/** Se05x_API_ECDHGenerateSharedSecret
 *
 * The ECDHGenerateSharedSecret command generates a shared secret ECC point on
 * the curve using an EC private key on SE05X and an external public key provided
 * by the caller. The output shared secret is returned to the caller.
 *
 * Command to Applet
 *
 * @rst
 * +------------+------------------------------+----------------------------------------------+
 * | Field      | Value                        | Description                                  |
 * +============+==============================+==============================================+
 * | CLA        | 0x80                         |                                              |
 * +------------+------------------------------+----------------------------------------------+
 * | INS        | INS_CRYPTO                   | :cpp:type:`SE05x_INS_t`                      |
 * +------------+------------------------------+----------------------------------------------+
 * | P1         | P1_EC                        | See :cpp:type:`SE05x_P1_t`                   |
 * +------------+------------------------------+----------------------------------------------+
 * | P2         | P2_DH                        | See :cpp:type:`SE05x_P2_t`                   |
 * +------------+------------------------------+----------------------------------------------+
 * | Lc         | #(Payload)                   |                                              |
 * +------------+------------------------------+----------------------------------------------+
 * | Payload    | TLV[TAG_1]                   | 4-byte identifier of the key pair or private |
 * |            |                              | key.                                         |
 * +------------+------------------------------+----------------------------------------------+
 * | TLV[TAG_2] | External public key (see     |                                              |
 * |            | :cpp:type:`ECKeyRef`).       |                                              |
 * +------------+------------------------------+----------------------------------------------+
 * | TLV[TAG_7] | 4-byte HMACKey identifier to |                                              |
 * |            | store output.    [Optional]  |                                              |
 * +------------+------------------------------+----------------------------------------------+
 * | Le         | 0x00                         | Expected shared secret length.               |
 * +------------+------------------------------+----------------------------------------------+
 * @endrst
 *
 * R-APDU Body
 *
 * @rst
 * +------------+----------------------------------------------+
 * | Value      | Description                                  |
 * +============+==============================================+
 * | TLV[TAG_1] | The returned shared secret.    [Conditional: |
 * |            | only when the input does not contain         |
 * |            | TLV[TAG_7].}                                 |
 * +------------+----------------------------------------------+
 * @endrst
 *
 * R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------------+
 * | SW          | Description                          |
 * +=============+======================================+
 * | SW_NO_ERROR | The command is handled successfully. |
 * +-------------+--------------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID objectID [1:kSE05x_TAG_1]
 * @param[in] pubKey pubKey [2:kSE05x_TAG_2]
 * @param[in] pubKeyLen Length of pubKey
 * @param[out] sharedSecret  [0:kSE05x_TAG_1]
 * @param[in,out] psharedSecretLen Length for sharedSecret
 */
smStatus_t Se05x_API_ECDHGenerateSharedSecret(pSe05xSession_t session_ctx,
    uint32_t objectID,
    const uint8_t *pubKey,
    size_t pubKeyLen,
    uint8_t *sharedSecret,
    size_t *psharedSecretLen);

/**
 * @brief      Se05x_API_CipherOneShot
 *
 * Encrypt or decrypt data in one shot mode.
 *
 * The key object must be either an AES key or DES key.
 *
 * Command to Applet
 *
 * @rst
 * +---------+-----------------------+------------------------------------------------+
 * | Field   | Value                 | Description                                    |
 * +=========+=======================+================================================+
 * | CLA     | 0x80                  |                                                |
 * +---------+-----------------------+------------------------------------------------+
 * | INS     | INS_CRYPTO            | :cpp:type:`SE05x_INS_t`                        |
 * +---------+-----------------------+------------------------------------------------+
 * | P1      | P1_CIPHER             | See :cpp:type:`SE05x_P1_t`                     |
 * +---------+-----------------------+------------------------------------------------+
 * | P2      | P2_ENCRYPT_ONESHOT or | See :cpp:type:`SE05x_P2_t`                     |
 * |         | P2_DECRYPT_ONESHOT    |                                                |
 * +---------+-----------------------+------------------------------------------------+
 * | Lc      | #(Payload)            |                                                |
 * +---------+-----------------------+------------------------------------------------+
 * | Payload | TLV[TAG_1]            | 4-byte identifier of the key object.           |
 * +---------+-----------------------+------------------------------------------------+
 * |         | TLV[TAG_2]            | 1-byte CipherMode                              |
 * +---------+-----------------------+------------------------------------------------+
 * |         | TLV[TAG_3]            | Byte array containing input data.              |
 * +---------+-----------------------+------------------------------------------------+
 * |         | TLV[TAG_4]            | Byte array containing an initialization        |
 * |         |                       | vector.   [Optional]   [Conditional: only when |
 * |         |                       | the Crypto Object type equals CC_CIPHER and    |
 * |         |                       | subtype is not including ECB]                  |
 * +---------+-----------------------+------------------------------------------------+
 * | Le      | 0x00                  | Expecting return data.                         |
 * +---------+-----------------------+------------------------------------------------+
 * @endrst
 *
 * R-APDU Body
 *
 * @rst
 * +------------+-------------+
 * | Value      | Description |
 * +============+=============+
 * | TLV[TAG_1] | Output data |
 * +------------+-------------+
 * @endrst
 *
 * R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------------+
 * | SW          | Description                          |
 * +=============+======================================+
 * | SW_NO_ERROR | The command is handled successfully. |
 * +-------------+--------------------------------------+
 * @endrst
 *
 *
 * @param[in]     session_ctx     The session context
 * @param[in]     objectID        The object id (AES key object with key length = 128 or 192 or 256 bits)
 * @param[in]     cipherMode      The cipher mode
 * @param[in]     inputData       The input data (16 Bytes aligned data. Max - 112 Bytes)
 * @param[in]     inputDataLen    The input data length
 * @param[in]     IV              Initial vector (16 Bytes)
 * @param[in]     IVLen           The iv length
 * @param[in,out] outputData      The output data
 * @param[in,out] poutputDataLen  The poutput data length
 * @param[in]     operation       The operation
 *
 * @return     The sm status.
 */
smStatus_t Se05x_API_CipherOneShot(pSe05xSession_t session_ctx,
    uint32_t objectID,
    SE05x_CipherMode_t cipherMode,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *IV,
    size_t IVLen,
    uint8_t *outputData,
    size_t *poutputDataLen,
    const SE05x_Cipher_Oper_OneShot_t operation);

/** Se05x_API_WriteSymmKey
 *
 * Creates or writes an AES key, DES key or HMAC key, indicated by P1:
 *
 *   * P1_AES
 *
 *   * P1_DES
 *
 *   * P1_HMAC
 *
 * Users can pass RFC3394 wrapped keys by indicating the KEK in TLV[TAG_2]. Note
 * that RFC3394 required 8-byte aligned input, so this can only be used when the
 * key has an 8-byte aligned length.
 *
 * Command to Applet
 *
 * @rst
 * +---------+-----------------------+-----------------------------------------------+
 * | Field   | Value                 | Description                                   |
 * +=========+=======================+===============================================+
 * | P1      | See above             | See :cpp:type:`SE05x_P1_t`                    |
 * +---------+-----------------------+-----------------------------------------------+
 * | P2      | P2_DEFAULT            | See :cpp:type:`SE05x_P2_t`                    |
 * +---------+-----------------------+-----------------------------------------------+
 * | Payload | TLV[TAG_POLICY]       | Byte array containing the object policy.      |
 * |         |                       | [Optional: default policy applies]            |
 * |         |                       | [Conditional: only when the object identifier |
 * |         |                       | is not in use yet]                            |
 * +---------+-----------------------+-----------------------------------------------+
 * |         | TLV[TAG_MAX_ATTEMPTS] | 2-byte maximum number of attempts. If 0 is    |
 * |         |                       | given, this means unlimited.   [Optional:     |
 * |         |                       | default unlimited]   [Conditional: only when  |
 * |         |                       | the object identifier is not in use yet and   |
 * |         |                       | INS includes  INS_AUTH_OBJECT; see            |
 * |         |                       | AuthenticationObjectPolicies]                 |
 * +---------+-----------------------+-----------------------------------------------+
 * |         | TLV[TAG_1]            | 4-byte object identifier                      |
 * +---------+-----------------------+-----------------------------------------------+
 * |         | TLV[TAG_2]            | 4-byte KEK identifier   [Conditional: only    |
 * |         |                       | when the key value is RFC3394 wrapped]        |
 * +---------+-----------------------+-----------------------------------------------+
 * |         | TLV[TAG_3]            | Key value, either plain or RFC3394 wrapped.   |
 * +---------+-----------------------+-----------------------------------------------+
 * |         | TLV[TAG_4]            | Tag length for GCM/GMAC. Will only be used if |
 * |         |                       | the object is an  AESKey.   [Optional]        |
 * +---------+-----------------------+-----------------------------------------------+
 * |         | TLV[TAG_11]           | 4-byte version    [Optional]                  |
 * +---------+-----------------------+-----------------------------------------------+
 * @endrst
 *
 * @param[in]  session_ctx  The session context
 * @param[in]  policy       The policy
 * @param[in]  maxAttempt   The maximum attempt
 * @param[in]  objectID     The object id
 * @param[in]  kekID        The kek id
 * @param[in]  keyValue     The key value (Supported lengths - 128, 192 or 256 bits)
 * @param[in]  keyValueLen  The key value length
 * @param[in]  ins_type     The insert type
 * @param[in]  type         The type
 *
 * @return     The sm status.
 */
smStatus_t Se05x_API_WriteSymmKey(pSe05xSession_t session_ctx,
    pSe05xPolicy_t policy,
    SE05x_MaxAttemps_t maxAttempt,
    uint32_t objectID,
    SE05x_KeyID_t kekID,
    const uint8_t *keyValue,
    size_t keyValueLen,
    const SE05x_INS_t ins_type,
    const SE05x_SymmKeyType_t type);

/** Se05x_API_DeleteSecureObject
 *
 * Deletes a Secure Object.
 *
 * If the object origin = ORIGIN_PROVISIONED, an error will be returned and the
 * object is not deleted.
 *
 *
 * Command to Applet
 *
 * @rst
 * +-------+------------------+-------------------------------------------+
 * | Field | Value            | Description                               |
 * +=======+==================+===========================================+
 * | CLA   | 0x80             |                                           |
 * +-------+------------------+-------------------------------------------+
 * | INS   | INS_MGMT         | See :cpp:type:`SE05x_INS_t`               |
 * +-------+------------------+-------------------------------------------+
 * | P1    | P1_DEFAULT       | See :cpp:type:`SE05x_P1_t`                |
 * +-------+------------------+-------------------------------------------+
 * | P2    | P2_DELETE_OBJECT | See :cpp:type:`SE05x_P2_t`                |
 * +-------+------------------+-------------------------------------------+
 * | Lc    | #(Payload)       |                                           |
 * +-------+------------------+-------------------------------------------+
 * |       | TLV[TAG_1]       | 4-byte existing Secure Object identifier. |
 * +-------+------------------+-------------------------------------------+
 * | Le    | -                |                                           |
 * +-------+------------------+-------------------------------------------+
 * @endrst
 *
 * R-APDU Body
 *
 * NA
 *
 * R-APDU Trailer
 *
 * @rst
 * +-------------+----------------------------------------------+
 * | SW          | Description                                  |
 * +=============+==============================================+
 * | SW_NO_ERROR | The file is created or updated successfully. |
 * +-------------+----------------------------------------------+
 * @endrst
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] objectID object id [1:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_DeleteSecureObject(pSe05xSession_t session_ctx, uint32_t objectID);

/** Se05x_API_GetRandom
 *
 * Gets random data from the SE05X .
 *
 *
 * # Command to Applet
 *
 * @rst
 * +-------+------------+-----------------------------+
 * | Field | Value      | Description                 |
 * +=======+============+=============================+
 * | CLA   | 0x80       |                             |
 * +-------+------------+-----------------------------+
 * | INS   | INS_MGMT   | See :cpp:type:`SE05x_INS_t` |
 * +-------+------------+-----------------------------+
 * | P1    | P1_DEFAULT | See :cpp:type:`SE05x_P1_t`  |
 * +-------+------------+-----------------------------+
 * | P2    | P2_RANDOM  | See :cpp:type:`SE05x_P2_t`  |
 * +-------+------------+-----------------------------+
 * | Lc    | #(Payload) |                             |
 * +-------+------------+-----------------------------+
 * |       | TLV[TAG_1] | 2-byte requested size.      |
 * +-------+------------+-----------------------------+
 * | Le    | 0x00       | Expecting random data       |
 * +-------+------------+-----------------------------+
 * @endrst
 *
 * # R-APDU Body
 *
 * @rst
 * +------------+--------------+
 * | Value      | Description  |
 * +============+==============+
 * | TLV[TAG_1] | Random data. |
 * +------------+--------------+
 * @endrst
 *
 * # R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------+
 * | SW          | Description                    |
 * +=============+================================+
 * | SW_NO_ERROR | Data is returned successfully. |
 * +-------------+--------------------------------+
 * @endrst
 *
 *
 *
 * @param[in]  session_ctx     The session context
 * @param[in]  size            The size
 * @param      randomData      The random data
 * @param      prandomDataLen  The prandom data length
 *
 * @return     The sm status.
 */
smStatus_t Se05x_API_GetRandom(pSe05xSession_t session_ctx, uint16_t size, uint8_t *randomData, size_t *prandomDataLen);

/** Se05x_API_DigestInit
 *
 * Open a digest operation. The state of the digest operation is kept in the
 * Crypto Object until the Crypto Object is finalized or deleted.
 *
 *
 * # Command to Applet
 *
 * @rst
 * +-------+------------+---------------------------------+
 * | Field | Value      | Description                     |
 * +=======+============+=================================+
 * | CLA   | 0x80       |                                 |
 * +-------+------------+---------------------------------+
 * | INS   | INS_CRYPTO | See :cpp:type:`SE05x_INS_t`     |
 * +-------+------------+---------------------------------+
 * | P1    | P1_DEFAULT | See :cpp:type:`SE05x_P1_t`      |
 * +-------+------------+---------------------------------+
 * | P2    | P2_INIT    | See :cpp:type:`SE05x_P2_t`      |
 * +-------+------------+---------------------------------+
 * | Lc    | #(Payload) |                                 |
 * +-------+------------+---------------------------------+
 * |       | TLV[TAG_2] | 2-byte Crypto Object identifier |
 * +-------+------------+---------------------------------+
 * @endrst
 *
 * # R-APDU Body
 *
 * NA
 *
 * # R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------------+
 * | SW          | Description                          |
 * +=============+======================================+
 * | SW_NO_ERROR | The command is handled successfully. |
 * +-------------+--------------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_2]
 */
smStatus_t Se05x_API_DigestInit(pSe05xSession_t session_ctx, SE05x_CryptoObjectID_t cryptoObjectID);

/** Se05x_API_DigestUpdate
 *
 *
 * # Command to Applet
 *
 * @rst
 * +-------+------------+---------------------------------+
 * | Field | Value      | Description                     |
 * +=======+============+=================================+
 * | CLA   | 0x80       |                                 |
 * +-------+------------+---------------------------------+
 * | INS   | INS_CRYPTO | See :cpp:type:`SE05x_INS_t`     |
 * +-------+------------+---------------------------------+
 * | P1    | P1_DEFAULT | See :cpp:type:`SE05x_P1_t`      |
 * +-------+------------+---------------------------------+
 * | P2    | P2_UPDATE  | See :cpp:type:`SE05x_P2_t`      |
 * +-------+------------+---------------------------------+
 * | Lc    | #(Payload) |                                 |
 * +-------+------------+---------------------------------+
 * |       | TLV[TAG_2] | 2-byte Crypto Object identifier |
 * +-------+------------+---------------------------------+
 * |       | TLV[TAG_3] | Data to be hashed.              |
 * +-------+------------+---------------------------------+
 * | Le    |            |                                 |
 * +-------+------------+---------------------------------+
 * @endrst
 *
 * # R-APDU Body
 *
 * NA
 *
 * # R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------------+
 * | SW          | Description                          |
 * +=============+======================================+
 * | SW_NO_ERROR | The command is handled successfully. |
 * +-------------+--------------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_2]
 * @param[in] inputData inputData [2:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 */
smStatus_t Se05x_API_DigestUpdate(
    pSe05xSession_t session_ctx, SE05x_CryptoObjectID_t cryptoObjectID, const uint8_t *inputData, size_t inputDataLen);

/** Se05x_API_DigestFinal
 *
 *
 * # Command to Applet
 *
 * @rst
 * +-------+------------+------------------------------------+
 * | Field | Value      | Description                        |
 * +=======+============+====================================+
 * | CLA   | 0x80       |                                    |
 * +-------+------------+------------------------------------+
 * | INS   | INS_CRYPTO | See :cpp:type:`SE05x_INS_t`        |
 * +-------+------------+------------------------------------+
 * | P1    | P1_DEFAULT | See :cpp:type:`SE05x_P1_t`         |
 * +-------+------------+------------------------------------+
 * | P2    | P2_FINAL   | See :cpp:type:`SE05x_P2_t`         |
 * +-------+------------+------------------------------------+
 * | Lc    | #(Payload) |                                    |
 * +-------+------------+------------------------------------+
 * |       | TLV[TAG_2] | 2-byte Crypto Object identifier    |
 * +-------+------------+------------------------------------+
 * |       | TLV[TAG_3] | Data to be encrypted or decrypted. |
 * +-------+------------+------------------------------------+
 * | Le    | 0x00       | Expecting TLV with hash value.     |
 * +-------+------------+------------------------------------+
 * @endrst
 *
 * # R-APDU Body
 *
 * @rst
 * +------------+-------------+
 * | Value      | Description |
 * +============+=============+
 * | TLV[TAG_1] | CMAC value  |
 * +------------+-------------+
 * @endrst
 *
 * # R-APDU Trailer
 *
 * @rst
 * +-------------+-----------------------------------+
 * | SW          | Description                       |
 * +=============+===================================+
 * | SW_NO_ERROR | The hash is created successfully. |
 * +-------------+-----------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_2]
 * @param[in] inputData inputData [2:kSE05x_TAG_3]
 * @param[in] inputDataLen Length of inputData
 * @param[out] cmacValue  [0:kSE05x_TAG_1]
 * @param[in,out] pcmacValueLen Length for cmacValue
 */
smStatus_t Se05x_API_DigestFinal(pSe05xSession_t session_ctx,
    SE05x_CryptoObjectID_t cryptoObjectID,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *cmacValue,
    size_t *pcmacValueLen);

/** Se05x_API_DigestOneShot
 *
 * Performs a hash operation in one shot (without context).
 *
 *
 * # Command to Applet
 *
 * @rst
 * +-------+------------+-------------------------------------------+
 * | Field | Value      | Description                               |
 * +=======+============+===========================================+
 * | CLA   | 0x80       |                                           |
 * +-------+------------+-------------------------------------------+
 * | INS   | INS_CRYPTO | See :cpp:type:`SE05x_INS_t`               |
 * +-------+------------+-------------------------------------------+
 * | P1    | P1_DEFAULT | See :cpp:type:`SE05x_P1_t`                |
 * +-------+------------+-------------------------------------------+
 * | P2    | P2_ONESHOT | See :cpp:type:`SE05x_P2_t`                |
 * +-------+------------+-------------------------------------------+
 * | Lc    | #(Payload) |                                           |
 * +-------+------------+-------------------------------------------+
 * |       | TLV[TAG_1] | 1-byte DigestMode (except DIGEST_NO_HASH) |
 * +-------+------------+-------------------------------------------+
 * |       | TLV[TAG_2] | Data to hash.                             |
 * +-------+------------+-------------------------------------------+
 * | Le    | 0x00       | TLV expecting hash value                  |
 * +-------+------------+-------------------------------------------+
 * @endrst
 *
 * # R-APDU Body
 *
 * @rst
 * +------------+-------------+
 * | Value      | Description |
 * +============+=============+
 * | TLV[TAG_1] | Hash value. |
 * +------------+-------------+
 * @endrst
 *
 * # R-APDU Trailer
 *
 * @rst
 * +-------------+-----------------------------------+
 * | SW          | Description                       |
 * +=============+===================================+
 * | SW_NO_ERROR | The hash is created successfully. |
 * +-------------+-----------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] digestMode digestMode [1:kSE05x_TAG_1]
 * @param[in] inputData inputData [2:kSE05x_TAG_2]
 * @param[in] inputDataLen Length of inputData
 * @param[out] hashValue  [0:kSE05x_TAG_1]
 * @param[in,out] phashValueLen Length for hashValue
 */
smStatus_t Se05x_API_DigestOneShot(pSe05xSession_t session_ctx,
    uint8_t digestMode,
    const uint8_t *inputData,
    size_t inputDataLen,
    uint8_t *hashValue,
    size_t *phashValueLen);

/** Se05x_API_CreateCryptoObject
 *
 * Creates a Crypto Object on the SE05X . Once the Crypto Object is created, it
 * is bound to the user who created the Crypto Object.
 *
 * A CryptoObject is a 2-byte value consisting of a CryptoContext in MSB and one
 * of the following in LSB:
 *
 *   * DigestMode in case CryptoContext = CC_DIGEST
 *
 *   * CipherMode in case CryptoContext = CC_CIPHER
 *
 *   * MACAlgo in case CryptoContext = CC_SIGNATURE
 *
 *   * AEADMode in case CryptoContext = CC_AEAD
 *
 * # Command to Applet
 *
 * @rst
 * +---------+---------------+-------------------------------------------+
 * | Field   | Value         | Description                               |
 * +=========+===============+===========================================+
 * | CLA     | 0x80          |                                           |
 * +---------+---------------+-------------------------------------------+
 * | INS     | INS_WRITE     | See :cpp:type:`SE05x_INS_t`               |
 * +---------+---------------+-------------------------------------------+
 * | P1      | P1_CRYPTO_OBJ | See :cpp:type:`SE05x_P1_t`                |
 * +---------+---------------+-------------------------------------------+
 * | P2      | P2_DEFAULT    | See :cpp:type:`SE05x_P2_t`                |
 * +---------+---------------+-------------------------------------------+
 * | Lc      | #(Payload)    | Payload length                            |
 * +---------+---------------+-------------------------------------------+
 * | Payload | TLV[TAG_1]    | 2-byte Crypto Object identifier           |
 * +---------+---------------+-------------------------------------------+
 * |         | TLV[TAG_2]    | 1-byte :cpp:type:`SE05x_CryptoObject_t`   |
 * +---------+---------------+-------------------------------------------+
 * |         | TLV[TAG_3]    | 1-byte Crypto Object subtype, either from |
 * |         |               | :cpp:type:`DigestModeRef`, CipherMode,    |
 * |         |               | MACAlgo (depending on TAG_2) or AEADMode. |
 * +---------+---------------+-------------------------------------------+
 * @endrst
 *
 * # R-APDU Body
 *
 * NA
 *
 * # R-APDU Trailer
 *
 * @rst
 * +-------------+----------------------------------------------+
 * | SW          | Description                                  |
 * +=============+==============================================+
 * | SW_NO_ERROR | The file is created or updated successfully. |
 * +-------------+----------------------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_1]
 * @param[in] cryptoContext cryptoContext [2:kSE05x_TAG_2]
 *
 * @param[in] subtype 1-byte Crypto Object subtype, either from
 *            DigestMode, CipherMode or MACAlgo (depending on
 *            TAG_2). [3:kSE05x_TAG_3]
 */
smStatus_t Se05x_API_CreateCryptoObject(pSe05xSession_t session_ctx,
    SE05x_CryptoObjectID_t cryptoObjectID,
    SE05x_CryptoContext_t cryptoContext,
    SE05x_CryptoModeSubType_t subtype);

/** Se05x_API_DeleteCryptoObject
 *
 * Deletes a Crypto Object on the SE05X .
 *
 * Note: when a Crypto Object is deleted, the memory (as mentioned in ) is de-
 * allocated, but the transient memory is only freed when de-selecting the
 * applet!
 *
 * # Command to Applet
 *
 * @rst
 * +---------+------------------+---------------------------------+
 * | Field   | Value            | Description                     |
 * +=========+==================+=================================+
 * | CLA     | 0x80             |                                 |
 * +---------+------------------+---------------------------------+
 * | INS     | INS_MGMT         | See :cpp:type:`SE05x_INS_t`     |
 * +---------+------------------+---------------------------------+
 * | P1      | P1_CRYPTO_OBJ    | See :cpp:type:`SE05x_P1_t`      |
 * +---------+------------------+---------------------------------+
 * | P2      | P2_DELETE_OBJECT | See :cpp:type:`SE05x_P2_t`      |
 * +---------+------------------+---------------------------------+
 * | Lc      | #(Payload)       | Payload length                  |
 * +---------+------------------+---------------------------------+
 * | Payload | TLV[TAG_1]       | 2-byte Crypto Object identifier |
 * +---------+------------------+---------------------------------+
 * @endrst
 *
 * # R-APDU Body
 *
 * NA
 *
 * # R-APDU Trailer
 *
 * @rst
 * +-------------+----------------------------------------------+
 * | SW          | Description                                  |
 * +=============+==============================================+
 * | SW_NO_ERROR | The file is created or updated successfully. |
 * +-------------+----------------------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] cryptoObjectID cryptoObjectID [1:kSE05x_TAG_1]
 */
smStatus_t Se05x_API_DeleteCryptoObject(pSe05xSession_t session_ctx, SE05x_CryptoObjectID_t cryptoObjectID);

/** Se05x_API_ReadIDList
 *
 * Get a list of present Secure Object identifiers.
 *
 * The offset in TAG_1 is an 0-based offset in the list of object. As the user
 * does not know how many objects would be returned, the offset needs to be based
 * on the return values from the previous ReadIDList. If the applet only returns
 * a part of the result, it will indicate that more identifiers are available (by
 * setting TLV[TAG_1] in the response to 0x01). The user can then retrieve the
 * next chunk of identifiers by calling ReadIDList with an offset that equals the
 * amount of identifiers listed in the previous response.
 *
 * _Example 1:_ first ReadIDList command TAG_1=0, response TAG_1=0,
 * TAG_2=complete list
 *
 * _Example 2:_ first ReadIDList command TAG_1=0, response TAG_1=1, TAG_2=first
 * chunk (m entries) second ReadIDList command TAG_1=m, response TAG_1=1,
 * TAG_2=second chunk (n entries) thirst ReadIDList command TAG_1=(m+n), response
 * TAG_1=0, TAG_2=third last chunk
 *
 * # Command to Applet
 *
 * @rst
 * +-------+------------+-----------------------------------------------+
 * | Field | Value      | Description                                   |
 * +=======+============+===============================================+
 * | CLA   | 0x80       |                                               |
 * +-------+------------+-----------------------------------------------+
 * | INS   | INS_READ   | See :cpp:type:`SE05x_INS_t`                   |
 * +-------+------------+-----------------------------------------------+
 * | P1    | P1_DEFAULT | See :cpp:type:`SE05x_P1_t`                    |
 * +-------+------------+-----------------------------------------------+
 * | P2    | P2_LIST    | See :cpp:type:`SE05x_P2_t`                    |
 * +-------+------------+-----------------------------------------------+
 * | Lc    | #(Payload) |                                               |
 * +-------+------------+-----------------------------------------------+
 * |       | TLV[TAG_1] | 2-byte offset                                 |
 * +-------+------------+-----------------------------------------------+
 * |       | TLV[TAG_2] | 1-byte type filter: 1 byte from               |
 * |       |            | :cpp:type:`SE05x_SecObjTyp_t` or 0xFF for all |
 * |       |            | types.                                        |
 * +-------+------------+-----------------------------------------------+
 * | Le    | 0x00       |                                               |
 * +-------+------------+-----------------------------------------------+
 * @endrst
 *
 * # R-APDU Body
 *
 * @rst
 * +------------+-------------------------------------------+
 * | Value      | Description                               |
 * +============+===========================================+
 * | TLV[TAG_1] | 1-byte :cpp:type:`MoreIndicatorRef`       |
 * +------------+-------------------------------------------+
 * | TLV[TAG_2] | Byte array containing 4-byte identifiers. |
 * +------------+-------------------------------------------+
 * @endrst
 *
 * # R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------+
 * | SW          | Description                    |
 * +=============+================================+
 * | SW_NO_ERROR | Data is returned successfully. |
 * +-------------+--------------------------------+
 * @endrst
 *
 *
 *
 * @param[in] session_ctx Session Context [0:kSE05x_pSession]
 * @param[in] outputOffset output offset [1:kSE05x_TAG_1]
 * @param[in] filter filter [2:kSE05x_TAG_2]
 * @param[out] pmore If more ids are present [0:kSE05x_TAG_1]
 * @param[out] idlist Byte array containing 4-byte identifiers [1:kSE05x_TAG_2]
 * @param[in,out] pidlistLen Length for idlist
 */
smStatus_t Se05x_API_ReadIDList(pSe05xSession_t session_ctx,
    uint16_t outputOffset,
    uint8_t filter,
    uint8_t *pmore,
    uint8_t *idlist,
    size_t *pidlistLen);

/** Se05x_API_DeleteAll_Iterative
 *
 * Go through each object and delete it individually.
 *
 * This API does not use the Applet API @ref Se05x_API_DeleteAll. It
 * does not delete ALL objects and purposefully skips few objects.
 *
 * Instead, this API uses @ref Se05x_API_ReadIDList and @ref
 * Se05x_API_ReadCryptoObjectList to first fetch list of objects to host, and
 * **selectitvely** deletes.
 *
 * For e.g. It does not kill objects from:
 *  - The range SE05X_OBJID_SE05X_APPLET_RES_START to
 *    SE05X_OBJID_SE05X_APPLET_RES_END.  This range is used by applet.
 *  - The range EX_SSS_OBJID_DEMO_AUTH_START to EX_SSS_OBJID_DEMO_AUTH_END,
 *    which is used by middleware DEMOS for authentication.
 *  - And others.
 *
 * Kindly see the Implementation of is API Se05x_API_DeleteAll_Iterative to see
 * the list of ranges that are skipped.
 *
 * @param[in]  session_ctx  Session Context
 *
 * @return     The status of API.
 */
smStatus_t Se05x_API_DeleteAll_Iterative(pSe05xSession_t session_ctx);

/** Se05x_API_ReadSize
 *
 * ReadSize
 *
 * Get the size of a Secure Object (in bytes):
 *
 *   * For EC keys: the size of the curve is returned.
 *
 *   * For RSA keys: the key size is returned.
 *
 *   * For AES/DES/HMAC keys, the key size is returned.
 *
 *   * For binary files: the file size is returned
 *
 *   * For userIDs: nothing is returned (SW_CONDITIONS_NOT_SATISFIED).
 *
 *   * For counters: the counter length is returned.
 *
 *   * For PCR: the PCR length is returned.
 *
 * # Command to Applet
 *
 * @rst
 * +-------+------------+-----------------------------+
 * | Field | Value      | Description                 |
 * +=======+============+=============================+
 * | CLA   | 0x80       |                             |
 * +-------+------------+-----------------------------+
 * | INS   | INS_READ   | See :cpp:type:`SE05x_INS_t` |
 * +-------+------------+-----------------------------+
 * | P1    | P1_DEFAULT | See :cpp:type:`SE05x_P1_t`  |
 * +-------+------------+-----------------------------+
 * | P2    | P2_SIZE    | See :cpp:type:`SE05x_P2_t`  |
 * +-------+------------+-----------------------------+
 * | Lc    | #(Payload) |                             |
 * +-------+------------+-----------------------------+
 * |       | TLV[TAG_1] | 4-byte object identifier.   |
 * +-------+------------+-----------------------------+
 * | Le    | 0x00       |                             |
 * +-------+------------+-----------------------------+
 * @endrst
 *
 * # R-APDU Body
 *
 * @rst
 * +------------+-----------------------------+
 * | Value      | Description                 |
 * +============+=============================+
 * | TLV[TAG_1] | Byte array containing size. |
 * +------------+-----------------------------+
 * @endrst
 *
 * # R-APDU Trailer
 *
 * @rst
 * +-------------+--------------------------------+
 * | SW          | Description                    |
 * +=============+================================+
 * | SW_NO_ERROR | Data is returned successfully. |
 * +-------------+--------------------------------+
 * @endrst
 *
 *
 * @param[in]  session_ctx  The session context
 * @param[in]  objectID     The object id
 * @param      psize        The psize
 *
 * @return     The sm status.
 */
smStatus_t Se05x_API_ReadSize(pSe05xSession_t session_ctx, uint32_t objectID, uint16_t *psize);

#endif //#ifndef SE05X_APDU_APIS_H_INC
