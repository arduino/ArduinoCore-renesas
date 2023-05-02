/*
  ssl_debug.cpp
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

#include "ssl_debug.h"

void ssl_debug_print(const char *format, ...) {
    char debug_buf[1024];   
    va_list argptr;
    va_start(argptr, format);
    vsprintf(debug_buf, format, argptr);
    va_end(argptr);
    Serial.print(debug_buf);
}

void ssl_debug_println(const char *format, ...) {
    char debug_buf[1024];   
    va_list argptr;
    va_start(argptr, format);
    vsprintf(debug_buf, format, argptr);
    va_end(argptr);
    Serial.println(debug_buf);
}

void ssl_debug_none(const char *format, ...) {
    (void)format;
}

void mbedtls_debug_print(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void) level);
    ssl_debug_print("%s:%04d: %s", file, line, str);
}
