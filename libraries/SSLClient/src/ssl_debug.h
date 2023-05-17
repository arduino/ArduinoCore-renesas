/*
  ssl_debug.h
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

#ifndef SSL_DEBUG_H_INC
#define SSL_DEBUG_H_INC
#include "Arduino.h"

/*
 * 0: NONE
 * 1: ERROR
 * 2: WARNING
 * 3: INFO
 * 4: DEBUG
 * 5: VERBOSE
 */
#define DEBUG_LEVEL 1

#if DEBUG_LEVEL > 0
    #define log_e ssl_debug_println
#else
    #define log_e ssl_debug_none
#endif

#if DEBUG_LEVEL > 1
    #define log_w ssl_debug_println
#else
    #define log_w ssl_debug_none
#endif

#if DEBUG_LEVEL > 2
    #define log_i ssl_debug_println
#else
    #define log_i ssl_debug_none
#endif

#if DEBUG_LEVEL > 3
    #define log_d ssl_debug_println
#else
    #define log_d ssl_debug_none
#endif
    
#if DEBUG_LEVEL > 4
    #define log_v ssl_debug_println
#else
    #define log_v ssl_debug_none
#endif

#ifdef __cplusplus
extern "C" {
#endif
void ssl_debug_print(const char *format, ...);
void ssl_debug_println(const char *format, ...);
void ssl_debug_none(const char *format, ...);
void mbedtls_debug_print(void *ctx, int level, const char *file, int line, const char *str);
#ifdef __cplusplus
}
#endif

#endif // #ifndef SSL_DEBUG_H_INC
