/*
  lzss.h
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

#ifndef SSU_LZSS_H_
#define SSU_LZSS_H_

/**************************************************************************************
   INCLUDE
 **************************************************************************************/

#include <stdint.h>
#include <stdio.h>

typedef void(*SFUWatchdogResetFuncPointer)(void);

/**************************************************************************************
   FUNCTION DEFINITION
 **************************************************************************************/

void lzss_init(FILE * update_file_ptr, FILE * target_file_ptr, uint32_t const lzss_file_size, SFUWatchdogResetFuncPointer wdog_feed_func_ptr);
void lzss_decode();
void lzss_flush();

#endif /* SSU_LZSS_H_ */
