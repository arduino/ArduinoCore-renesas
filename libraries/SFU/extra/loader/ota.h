/*
  ota.h
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

#ifndef SFU_OTA_H_
#define SFU_OTA_H_

#include <CodeFlashBlockDevice.h>
#include "lzss.h"

#define POST_APPLICATION_ADDR                         (0x20000)

int verify_header(FILE* update_file);
int decompress(FILE* update_file, FILE* target_file, SFUWatchdogResetFuncPointer wdog_feed_func_ptr);
int verify_sketch(FILE* file);
int flash(FILE *file, CodeFlashBlockDevice* flash, uint32_t address);

#endif
