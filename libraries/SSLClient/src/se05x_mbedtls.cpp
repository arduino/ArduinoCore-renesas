/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common.h"

#include "se05x_mbedtls.h"
//#include <sm_port.h>
//#include "se05x_APDU_apis.h"
#include <SE05X.h>

Se05xSession_t *pSession;

smStatus_t se05x_open_session(void)
{
    pSession = SE05X.getSession();

    if (pSession->conn_context != NULL) {
        return SM_OK;
    }

    SE05X.begin();
    pSession = SE05X.getSession();

    return SM_OK;
}
