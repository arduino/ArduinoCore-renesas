/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef __RM_TX_PORT_H
 #define __RM_TX_PORT_H

 #ifdef __cplusplus
extern "C"
{
 #endif

 #ifndef NX_INCLUDE_USER_DEFINE_FILE
  #define NX_INCLUDE_USER_DEFINE_FILE
 #endif

 #if   defined(__GNUC__)               /* GCC compiler */
  #include "gnu/inc/nx_port.h"
 #elif defined(__ICCARM__)             /* IAR compiler */
  #include "iar/inc/nx_port.h"
 #elif defined(__ARMCC_VERSION)        /* AC6 compiler */
  #include "ac5/inc/nx_port.h"
 #endif

 #ifdef __cplusplus
}
 #endif

#endif
