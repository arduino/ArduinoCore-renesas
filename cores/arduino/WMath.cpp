/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Wiring project - http://wiring.org.co
  Copyright (c) 2004-06 Hernando Barragan
  Modified 13 August 2006, David A. Mellis for Arduino - http://www.arduino.cc/
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

extern "C" {
  #include <stdlib.h>
  #include "common_data.h"
}

#if defined (ARDUINO_PORTENTA_C33)
#define SCE_TRNG_SUPPORT 1
static long trng()
{
  uint32_t value[4];
  if (R_SCE_Open(&sce_ctrl, &sce_cfg) != FSP_SUCCESS)
    return -1;
  R_SCE_RandomNumberGenerate(value);
  R_SCE_Close(&sce_ctrl);
  return (long)value[0] >= 0 ? value[0] : -value[0];
}
#endif

#if defined(ARDUINO_UNOR4_WIFI) || defined(ARDUINO_UNOR4_MINIMA)
#define SCE_TRNG_SUPPORT 1
extern "C" {
  fsp_err_t HW_SCE_McuSpecificInit(void);
  fsp_err_t HW_SCE_RNG_Read(uint32_t * OutData_Text);
}
static long trng()
{
  uint32_t value[4];
  if (HW_SCE_McuSpecificInit() != FSP_SUCCESS)
    return -1;
  HW_SCE_RNG_Read(value);
  return (long)value[0] >= 0 ? value[0] : -value[0];
}
#endif

#if (SCE_TRNG_SUPPORT == 1)
static bool useTRNG = true;
#endif

void randomSeed(unsigned long seed)
{
#if (SCE_TRNG_SUPPORT == 1)
  useTRNG = false;
#endif
  if (seed != 0) {
    srand(seed);
  }
}

long random(long howbig)
{
  if (howbig == 0) {
    return 0;
  }
#if (SCE_TRNG_SUPPORT == 1)
  if (useTRNG == true) {
    return trng() % howbig;
  }
#endif
  return rand() % howbig;
}

long random(long howsmall, long howbig)
{
  if (howsmall >= howbig) {
    return howsmall;
  }
  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}

#undef SCE_TRNG_SUPPORT
