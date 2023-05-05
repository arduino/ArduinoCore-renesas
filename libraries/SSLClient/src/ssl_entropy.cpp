/*
  ssl_entropy.cpp
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

#include "Arduino.h"
#include "ssl_entropy.h"

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
    ((void) data);
    *olen = 0;

    R_SCE_Open(&sce_ctrl, &sce_cfg);

    while(*olen < len) {
        uint8_t rnd[16];
        size_t left = (len - *olen);
        size_t chunk = left > 16 ? 16 : left;
        R_SCE_RandomNumberGenerate((uint32_t*)rnd);
        memcpy(output, rnd, chunk);

        *olen  += chunk;
        output += chunk;
    }

    return 0;
}
