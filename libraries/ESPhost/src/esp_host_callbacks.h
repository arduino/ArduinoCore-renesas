#ifndef EPS_HOST_CALLBACK_H
#define EPS_HOST_CALLBACK_H

#include "CCtrlWrapper.h"



/* resp callback */
typedef int (*ctrl_resp_cb_t) (ctrl_cmd_t * resp);

/* event callback */
typedef int (*ctrl_event_cb_t) (ctrl_cmd_t * event);



/* 
 * FUNCTIONs for RESPONSE CALLBACKs management
 */

/* SET response callback */
int esp_host_set_response_cb(int id, ctrl_resp_cb_t resp_cb);
/* IS response callback set */
int esp_host_is_response_cb_set(int id);
/* GET response callback (can be nullptr if not set) */
ctrl_resp_cb_t esp_host_get_response_cb(int id);
/* CALL response callback */
int esp_host_call_response_cb(int id, ctrl_cmd_t *app_resp);
/* RESET response callback (i.e. set to nullptr) */
int esp_host_reset_response_cb(int id);

/*
 * FUNCTION for EVENT CALLBACKs management
 */

/* SET event callback */
int esp_host_set_event_cb(int id, ctrl_event_cb_t event_cb);
/* IS event callback set */
int esp_host_is_event_cb_set(int id);
/* GET event callback (can be nullptr if not set) */
ctrl_event_cb_t esp_host_get_event_cb(int id);
/* CALL event callback */
int esp_host_call_event_cb(int id, ctrl_cmd_t *app_resp);
/* RESET event callback (i.e. set to nullptr) */
int esp_host_reset_event_cb(int id);



#endif