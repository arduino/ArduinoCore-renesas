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
 * -1: NONE
 *  0: ERROR
 *  1: WARNING
 *  2: INFO
 *  3: DEBUG
 *  4: VERBOSE/APDU
 */

#define SMLOG_E(fmt, ...) smlog_print(0, fmt, ## __VA_ARGS__)
#define SMLOG_W(fmt, ...) smlog_print(1, fmt, ## __VA_ARGS__)
#define SMLOG_I(fmt, ...) smlog_print(2, fmt, ## __VA_ARGS__)
#define SMLOG_D(fmt, ...) smlog_print(3, fmt, ## __VA_ARGS__)

#define SMLOG_AU8_D(BUF, LEN)                                       \
    smlog_print(4, ":");                                            \
    for (size_t bufIndex = 0; bufIndex < (size_t)LEN; bufIndex++) { \
        smlog_print(4, "%02x ", BUF[bufIndex]);                     \
    }                                                               \
    smlog_print(4, "\n")

#define SMLOG_MAU8_D(MSG, BUF, LEN)                                 \
    smlog_print(4, "%s:", MSG);                                     \
    for (size_t bufIndex = 0; bufIndex < (size_t)LEN; bufIndex++) { \
        smlog_print(4, "%02x ", BUF[bufIndex]);                     \
    }                                                               \
    smlog_print(4, "\n")


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
void smlog_print(int const lvl, const char *fmt, ...);
#ifdef __cplusplus
}
#endif

#endif // #ifndef SM_PORT_H_INC
