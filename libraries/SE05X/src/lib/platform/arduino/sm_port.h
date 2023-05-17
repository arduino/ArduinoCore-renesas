/*
  sm_port.h
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

#ifndef SM_PORT_H_INC
#define SM_PORT_H_INC
#include "Arduino.h"

/*
 * 0: NONE
 * 1: ERROR
 * 2: WARNING
 * 3: INFO
 * 4: DEBUG
 * 5: APDU
 */
#define DEBUG_LEVEL 0

#if DEBUG_LEVEL > 0
    #define SMLOG_E smlog_print
#else
    #define SMLOG_E smlog_none
#endif

#if DEBUG_LEVEL > 1
    #define SMLOG_W smlog_print
#else
    #define SMLOG_W smlog_none
#endif

#if DEBUG_LEVEL > 2
    #define SMLOG_I smlog_print
#else
    #define SMLOG_I smlog_none
#endif

#if DEBUG_LEVEL > 3
    #define SMLOG_D smlog_print
#else
    #define SMLOG_D smlog_none
#endif
    
#if DEBUG_LEVEL > 4
    #define SMLOG_AU8_D(BUF, LEN)                                       \
        smlog_print("%s", ":");                                         \
        for (size_t bufIndex = 0; bufIndex < (size_t)LEN; bufIndex++) { \
            smlog_print("%02x ", BUF[bufIndex]);                        \
        }                                                               \
        smlog_print("%s","\n")

    #define SMLOG_MAU8_D(MSG, BUF, LEN)                                 \
        smlog_print("%s", MSG);                                         \
        smlog_print("%s", ":");                                         \
        for (size_t bufIndex = 0; bufIndex < (size_t)LEN; bufIndex++) { \
            smlog_print("%02x ", BUF[bufIndex]);                        \
        }                                                               \
        smlog_print("%s","\n")

#else
    #define SMLOG_AU8_D(BUF, LEN)
    #define SMLOG_MAU8_D(MSG, BUF, LEN)
#endif

#define SM_MUTEX_DEFINE(x)
#define SM_MUTEX_INIT(x)
#define SM_MUTEX_DEINIT(x)
#define SM_MUTEX_LOCK(x)
#define SM_MUTEX_UNLOCK(x)

#define sm_malloc malloc
#define sm_free free

#ifndef FALSE
#define FALSE false
#endif

#ifndef TRUE
#define TRUE true
#endif

#define SE050_ENA_PIN 98

#ifdef __cplusplus
extern "C" {
#endif
void smlog_print(const char *format, ...);
void smlog_none(const char *format, ...);
#ifdef __cplusplus
}
#endif

#endif // #ifndef SM_PORT_H_INC
