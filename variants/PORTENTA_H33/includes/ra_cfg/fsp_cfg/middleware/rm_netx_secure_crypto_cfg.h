/* generated configuration header file - do not edit */
#ifndef RM_NETX_SECURE_CRYPTO_CFG_H_
#define RM_NETX_SECURE_CRYPTO_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif

#if (1)
#define NX_CRYPTO_STANDALONE_ENABLE
#endif
#define NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_SHA256_ALT   (1)
#define NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_AES_ALT      (1)
#define NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_TRNG_ALT     (1)
#define NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_ECC_ALT      (1)
#define NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_RSA_2048_ALT (1)
#define NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_RSA_3072_ALT (0)
#define NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_RSA_4096_ALT (0)
#define NETX_SECURE_CRYPTO_HW_ACCELERATE                  (NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_SHA256_ALT | \
                                                           NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_AES_ALT | \
                                                           NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_TRNG_ALT | \
                                                           NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_ECC_ALT  | \
                                                           NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_RSA_2048_ALT | \
                                                           NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_RSA_3072_ALT | \
                                                           NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_RSA_4096_ALT)
#define NX_CRYPTO_ECDH_SCRATCH_BUFFER_SIZE                 (2464)
#define NX_CRYPTO_ECDSA_SCRATCH_BUFFER_SIZE                (3016)

#if (1) /* Use RSA HW */
#if (1U == NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_RSA_4096_ALT)
 #define NX_CRYPTO_MAX_RSA_MODULUS_SIZE  (4096)
#elif (1U == NETX_SECURE_CRYPTO_NX_CRYPTO_METHODS_RSA_3072_ALT)
 #define NX_CRYPTO_MAX_RSA_MODULUS_SIZE  (3072)
#else
#define NX_CRYPTO_MAX_RSA_MODULUS_SIZE  (2048)
#endif
#else /* Use RSA SW */
 #define NX_CRYPTO_MAX_RSA_MODULUS_SIZE  (4)
#endif

#ifdef __cplusplus
}
#endif
#endif /* RM_NETX_SECURE_CRYPTO_CFG_H_ */
