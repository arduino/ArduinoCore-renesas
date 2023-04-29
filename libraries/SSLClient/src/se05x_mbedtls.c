/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "common.h"

#include "se05x_mbedtls.h"
#include <sm_port.h>
#include "se05x_APDU_apis.h"

Se05xSession_t pSession;

smStatus_t se05x_open_session(void)
{
    if (pSession.conn_context != NULL) {
        return SM_OK;
    }

    smStatus_t status = Se05x_API_SessionOpen(&pSession);
    if (status != SM_OK) {
        SMLOG_E("Error in Se05x_API_SessionOpen \n");
    }
    return status;
}
