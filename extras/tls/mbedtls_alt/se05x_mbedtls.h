/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __SE05X_MBEDTLS_H__
#define __SE05X_MBEDTLS_H__

//#include "se05x_APDU_apis.h"
#include <SE05X.h>

extern Se05xSession_t *pSession;

#include "mbedtls/pk.h"
#include "mbedtls/ssl.h"

/* Magic bytes to identify ALT keys */
#define ALT_KEYS_MAGIC                                 \
    {                                                  \
        0xA5, 0xA6, 0xB5, 0xB6, 0xA5, 0xA6, 0xB5, 0xB6 \
    }

smStatus_t se05x_open_session(void);

#endif /* __SE05X_MBEDTLS_H__ */
