/*
  sm_port.cpp
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

#include "../../platform/arduino/sm_port.h"

#if defined __has_include
  #if __has_include (<Arduino_DebugUtils.h>)
    #include <Arduino_DebugUtils.h>
    #define SE05X_DEBUG_ENABLE 1
  #else
    #define SE05X_DEBUG_ENABLE 0
  #endif
#else
  #define SE05X_DEBUG_ENABLE 0
#endif

void smlog_print(int const lvl, const char *fmt, ...) {
#if SE05X_DEBUG_ENABLE
    va_list args;
    va_start(args, fmt);
    Debug.newlineOff();
    Debug.print(lvl, fmt, args);
    Debug.newlineOn();
    va_end(args);
#else
    (void)lvl;
    (void)fmt;
#endif
}
