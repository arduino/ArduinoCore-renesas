/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2015-2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : hw_sce_ra_private.h
 * Version      : 1.09
 * Description  : SCE function private header file.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_sce_if.h"
#include "SCE_ProcCommon.h"

#ifndef HW_SCE_RA_PRIVATE_HEADER_FILE
#define HW_SCE_RA_PRIVATE_HEADER_FILE

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if SCE_SECURE_BOOT != 0
/* Required for each variable definition with no initial value to be placed in the SECURE_BOOT section. */
#define SCE_PRV_SEC_B_SECURE_BOOT  R_BSP_ATTRIB_SECTION_CHANGE(B, SECURE_BOOT, 4)
/* Required for each function definition to be placed in the SECURE_BOOT section. */
#define SCE_PRV_SEC_P_SECURE_BOOT  R_BSP_ATTRIB_SECTION_CHANGE(P, SECURE_BOOT)
/* Revert to default section. */
#define SCE_PRV_SEC_DEFAULT        R_BSP_ATTRIB_SECTION_CHANGE_END
#else
/* Required for each variable definition with no initial value to be placed in the SECURE_BOOT section.(dummy) */
#define SCE_PRV_SEC_B_SECURE_BOOT
/* Required for each function definition to be placed in the SECURE_BOOT section.(dummy) */
#define SCE_PRV_SEC_P_SECURE_BOOT
/* Revert to default section.(dummy) */
#define SCE_PRV_SEC_DEFAULT
#endif  /* SCE_SECURE_BOOT != 0 */

 #define SIZE_AES_128BIT_KEYLEN_BITS_WRAPPED     (384)
 #define SIZE_AES_128BIT_KEYLEN_BYTES_WRAPPED    ((SIZE_AES_128BIT_KEYLEN_BITS_WRAPPED) / 8)
 #define SIZE_AES_128BIT_KEYLEN_WORDS_WRAPPED    ((SIZE_AES_128BIT_KEYLEN_BITS_WRAPPED) / 32)

 #define SIZE_AES_192BIT_KEYLEN_BITS_WRAPPED     (544) /* Added 32 bits here to differentiate 192 wrapped key from 256 wrapped key in the psa_crypto stack. */
 #define SIZE_AES_192BIT_KEYLEN_BYTES_WRAPPED    ((SIZE_AES_192BIT_KEYLEN_BITS_WRAPPED) / 8)
 #define SIZE_AES_192BIT_KEYLEN_WORDS_WRAPPED    ((SIZE_AES_192BIT_KEYLEN_BITS_WRAPPED) / 32)

 #define SIZE_AES_256BIT_KEYLEN_BITS_WRAPPED     (512)
 #define SIZE_AES_256BIT_KEYLEN_BYTES_WRAPPED    ((SIZE_AES_256BIT_KEYLEN_BITS_WRAPPED) / 8)
 #define SIZE_AES_256BIT_KEYLEN_WORDS_WRAPPED    ((SIZE_AES_256BIT_KEYLEN_BITS_WRAPPED) / 32)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
extern uint32_t const S_FLASH[];

extern uint32_t S_RAM[HW_SCE_SRAM_WORD_SIZE];
extern uint32_t S_HEAP[HW_SCE_SHEAP_WORD_SIZE];
extern uint32_t S_INST[HW_SCE_SINST_WORD_SIZE];
extern uint32_t S_INST2[HW_SCE_SINST2_WORD_SIZE];

extern SCE_GEN_MAC_CB_FUNC_T SCE_GEN_MAC_CB_FUNC;

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
/* --------------------- SCE driver wrapper layer ---------------------- */

fsp_err_t HW_SCE_Aes128EcbEncryptInitPrivate(sce_aes_key_index_t *key_index);
fsp_err_t HW_SCE_Aes128EcbEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128EcbEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
fsp_err_t HW_SCE_Aes128EcbDecryptInitPrivate(sce_aes_key_index_t *key_index);
fsp_err_t HW_SCE_Aes128EcbDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128EcbDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
fsp_err_t HW_SCE_Aes128CbcEncryptInitPrivate(sce_aes_key_index_t *key_index, uint32_t *InData_IV);
fsp_err_t HW_SCE_Aes128CbcEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128CbcEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
fsp_err_t HW_SCE_Aes128CbcDecryptInitPrivate(sce_aes_key_index_t *key_index, uint32_t *InData_IV);
fsp_err_t HW_SCE_Aes128CbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128CbcDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);

fsp_err_t HW_SCE_Aes256EcbEncryptInitPrivate(sce_aes_key_index_t *key_index);
fsp_err_t HW_SCE_Aes256EcbEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256EcbEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
fsp_err_t HW_SCE_Aes256EcbDecryptInitPrivate(sce_aes_key_index_t *key_index);
fsp_err_t HW_SCE_Aes256EcbDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256EcbDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
fsp_err_t HW_SCE_Aes256CbcEncryptInitPrivate(sce_aes_key_index_t *key_index, uint32_t *InData_IV);
fsp_err_t HW_SCE_Aes256CbcEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256CbcEncryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);
fsp_err_t HW_SCE_Aes256CbcDecryptInitPrivate(sce_aes_key_index_t *key_index, uint32_t *InData_IV);
fsp_err_t HW_SCE_Aes256CbcDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256CbcDecryptFinalPrivate(uint32_t *OutData_Text, uint32_t *OutData_length);

fsp_err_t HW_SCE_Aes128GcmEncryptInitPrivate(sce_aes_key_index_t *key_index, uint32_t *InData_IV);
fsp_err_t HW_SCE_Aes128GcmEncryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128GcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
fsp_err_t HW_SCE_Aes128GcmEncryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
fsp_err_t HW_SCE_Aes128GcmDecryptInitPrivate(sce_aes_key_index_t *key_index, uint32_t *InData_IV);
fsp_err_t HW_SCE_Aes128GcmDecryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128GcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
fsp_err_t HW_SCE_Aes128GcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
fsp_err_t HW_SCE_Aes256GcmEncryptInitPrivate(sce_aes_key_index_t *key_index, uint32_t *InData_IV);
fsp_err_t HW_SCE_Aes256GcmEncryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256GcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
fsp_err_t HW_SCE_Aes256GcmEncryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
fsp_err_t HW_SCE_Aes256GcmDecryptInitPrivate(sce_aes_key_index_t *key_index, uint32_t *InData_IV);
fsp_err_t HW_SCE_Aes256GcmDecryptUpdateAadPrivate(uint32_t *InData_DataA, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256GcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT, uint32_t *OutData_Text);
fsp_err_t HW_SCE_Aes256GcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
void         HW_SCE_AesGcmEncryptUpdateTransitionPrivate(void);
void         HW_SCE_AesGcmDecryptUpdateTransitionPrivate(void);

fsp_err_t HW_SCE_Aes128CcmEncryptInitPrivate(sce_aes_key_index_t *KeyIndex, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
fsp_err_t HW_SCE_Aes128CcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128CcmEncryptFinalPrivate(uint32_t *InData_TextLen, uint32_t *InData_Text,
        uint32_t *OutData_Text, uint32_t *OutData_MAC);
fsp_err_t HW_SCE_Aes128CcmDecryptInitPrivate(sce_aes_key_index_t *KeyIndex, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
fsp_err_t HW_SCE_Aes128CcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128CcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text);
fsp_err_t HW_SCE_Aes256CcmEncryptInitPrivate(sce_aes_key_index_t *KeyIndex, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
fsp_err_t HW_SCE_Aes256CcmEncryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256CcmEncryptFinalPrivate(uint32_t *InData_TextLen, uint32_t *InData_Text,
        uint32_t *OutData_Text, uint32_t *OutData_MAC);
fsp_err_t HW_SCE_Aes256CcmDecryptInitPrivate(sce_aes_key_index_t *KeyIndex, uint32_t *InData_IV,
        uint32_t *InData_Header, uint32_t Header_Len);
fsp_err_t HW_SCE_Aes256CcmDecryptUpdatePrivate(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256CcmDecryptFinalPrivate(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text);

fsp_err_t HW_SCE_Aes128CmacGenerateInitPrivate(sce_aes_key_index_t *key_index);
fsp_err_t HW_SCE_Aes128CmacGenerateUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128CmacGenerateFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text,
        uint32_t *OutData_DataT);
fsp_err_t HW_SCE_Aes128CmacVerifyInitPrivate(sce_aes_key_index_t *key_index);
fsp_err_t HW_SCE_Aes128CmacVerifyUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128CmacVerifyFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen);

fsp_err_t HW_SCE_Aes256CmacGenerateInitPrivate(sce_aes_key_index_t *key_index);
fsp_err_t HW_SCE_Aes256CmacGenerateUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256CmacGenerateFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text,
        uint32_t *OutData_DataT);
fsp_err_t HW_SCE_Aes256CmacVerifyInitPrivate(sce_aes_key_index_t *key_index);
fsp_err_t HW_SCE_Aes256CmacVerifyUpdatePrivate(uint32_t *InData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256CmacVerifyFinalPrivate(uint32_t All_Msg_Len, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen);

fsp_err_t HW_SCE_Aes192EncryptDecryptInitSub(const uint32_t *InData_Cmd, const uint32_t *InData_KeyIndex, const uint32_t *InData_IV);
void      HW_SCE_Aes192EncryptDecryptUpdateSub(const uint32_t *InData_Text, uint32_t *OutData_Text, const uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes192EncryptDecryptFinalSub(void);
/* --------------------- SCE control procedure related ---------------------- */

void      HW_SCE_SoftwareResetSub(void);
fsp_err_t HW_SCE_SelfCheck1Sub(void);
fsp_err_t HW_SCE_SelfCheck2Sub(void);

fsp_err_t HW_SCE_GenerateAes128KeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
fsp_err_t HW_SCE_GenerateAes256KeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
fsp_err_t HW_SCE_GenerateAes128PlainKeyIndexSub(uint32_t *InData_KeyType, uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
fsp_err_t HW_SCE_GenerateAes256PlainKeyIndexSub(uint32_t *InData_KeyType, uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
fsp_err_t HW_SCE_GenerateAes128RandomKeyIndexSub(uint32_t *OutData_KeyIndex);
fsp_err_t HW_SCE_GenerateAes256RandomKeyIndexSub(uint32_t *OutData_KeyIndex);
fsp_err_t HW_SCE_GenerateRandomNumberSub(uint32_t *OutData_Text);
fsp_err_t HW_SCE_GenerateUpdateKeyRingKeyIndexSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_InstData);
fsp_err_t HW_SCE_GenerateUpdatePlainKeyRingKeyIndexSub(uint32_t *InData_KeyType, uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_InstData);

fsp_err_t HW_SCE_UpdateAes128KeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);
fsp_err_t HW_SCE_UpdateAes256KeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex);

fsp_err_t HW_SCE_StartUpdateFirmwareSub(void);
fsp_err_t HW_SCE_GenerateFirmwareMacSub(uint32_t *InData_KeyIndex, uint32_t *InData_SessionKey,
        uint32_t *InData_UpProgram, uint32_t *InData_IV, uint32_t *OutData_Program, uint32_t MAX_CNT/*,
        sce_firmware_generate_mac_resume_handle_t *sce_firmware_generate_mac_resume_handle*/);
fsp_err_t HW_SCE_VerifyFirmwareMacSub(uint32_t *InData_Program, uint32_t MAX_CNT);

fsp_err_t HW_SCE_Aes128EncryptDecryptInitSub(const uint32_t *InData_KeyType, const uint32_t *InData_Cmd, const uint32_t *InData_KeyIndex, const uint32_t *InData_IV);
void         HW_SCE_Aes128EncryptDecryptUpdateSub(const uint32_t *InData_Text, uint32_t *OutData_Text, const uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128EncryptDecryptFinalSub(void);
fsp_err_t HW_SCE_Aes256EncryptDecryptInitSub(const uint32_t *InData_KeyType, const uint32_t *InData_Cmd, const uint32_t *InData_KeyIndex, const uint32_t *InData_IV);
void         HW_SCE_Aes256EncryptDecryptUpdateSub(const uint32_t *InData_Text, uint32_t *OutData_Text, const uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256EncryptDecryptFinalSub(void);

fsp_err_t HW_SCE_Ghash(uint32_t *InData_HV, uint32_t *InData_IV, uint32_t *InData_Text, uint32_t *OutData_DataT,
        uint32_t MAX_CNT);

fsp_err_t HW_SCE_Aes128GcmEncryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         HW_SCE_Aes128GcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128GcmEncryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
fsp_err_t HW_SCE_Aes128GcmDecryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         HW_SCE_Aes128GcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128GcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataT, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
void         HW_SCE_AesGcmEncryptDecryptUpdateTransitionSub(void);
void         HW_SCE_Aes128GcmEncryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);
void         HW_SCE_Aes128GcmDecryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256GcmEncryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         HW_SCE_Aes256GcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256GcmEncryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
fsp_err_t HW_SCE_Aes256GcmDecryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         HW_SCE_Aes256GcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256GcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
void         HW_SCE_Aes256GcmEncryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);
void         HW_SCE_Aes256GcmDecryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);

fsp_err_t HW_SCE_Aes192GcmEncryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         HW_SCE_Aes192GcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes192GcmEncryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *OutData_Text, uint32_t *OutData_DataT);
fsp_err_t HW_SCE_Aes192GcmDecryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV);
void         HW_SCE_Aes192GcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes192GcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataT, uint32_t *InData_DataALen,
        uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text);
void         HW_SCE_Aes192GcmDecryptUpdateTransitionSub(void);
void         HW_SCE_Aes192GcmEncryptUpdateTransitionSub(void);
void         HW_SCE_Aes192GcmEncryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);
void         HW_SCE_Aes192GcmDecryptUpdateAADSub(uint32_t *InData_DataA, uint32_t MAX_CNT);

fsp_err_t HW_SCE_Aes128CcmEncryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_Header,
        uint32_t Header_Len);
void HW_SCE_Aes128CcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128CcmEncryptFinalSub(uint32_t *InData_TextLen, uint32_t *InData_Text, uint32_t *OutData_Text,
        uint32_t *OutData_MAC);
fsp_err_t HW_SCE_Aes128CcmDecryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_Header,
        uint32_t Header_Len);
void HW_SCE_Aes128CcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128CcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text);
fsp_err_t HW_SCE_Aes256CcmEncryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_Header,
        uint32_t Header_Len);
void HW_SCE_Aes256CcmEncryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256CcmEncryptFinalSub(uint32_t *InData_TextLen, uint32_t *InData_Text, uint32_t *OutData_Text,
        uint32_t *OutData_MAC);
fsp_err_t HW_SCE_Aes256CcmDecryptInitSub(uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_Header,
        uint32_t Header_Len);
void HW_SCE_Aes256CcmDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256CcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_TextLen, uint32_t *InData_MAC,
        uint32_t *InData_MACLength, uint32_t *OutData_Text);

fsp_err_t HW_SCE_Aes128CmacInitSub(uint32_t *InData_KeyIndex);
void         HW_SCE_Aes128CmacUpdateSub(uint32_t *InData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes128CmacFinalSub(uint32_t *InData_Cmd, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen, uint32_t *OutData_DataT);
fsp_err_t HW_SCE_Aes256CmacInitSub(uint32_t *InData_KeyIndex);
void         HW_SCE_Aes256CmacUpdateSub(uint32_t *InData_Text, uint32_t MAX_CNT);
fsp_err_t HW_SCE_Aes256CmacFinalSub(uint32_t *InData_Cmd, uint32_t *InData_Text, uint32_t *InData_DataT,
        uint32_t *InData_DataTLen, uint32_t *OutData_DataT);

void HW_SCE_SelfCheck1SubSub(void);
void HW_SCE_SelfCheck2SubSub(void);

void HW_SCE_func001(uint32_t ARG1, uint32_t ARG2, uint32_t ARG3, uint32_t ARG4);
void HW_SCE_func002(uint32_t ARG1, uint32_t ARG2, uint32_t ARG3, uint32_t ARG4);
void HW_SCE_func003(uint32_t ARG1, uint32_t ARG2, uint32_t ARG3, uint32_t ARG4);
void HW_SCE_func205(void);
void HW_SCE_func206(void);
void HW_SCE_func207(void);

void firm_mac_read(uint32_t *InData_Program);

uint32_t change_endian_long(uint32_t data);

#endif /* HW_SCE_RA_PRIVATE_HEADER_FILE */
