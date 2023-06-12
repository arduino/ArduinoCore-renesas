/*
  WDT.cpp
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

#include "WDT.h"

WDTimer::WDTimer() { }
WDTimer::~WDTimer() { }

int WDTimer::begin(wdt_cfg_t config)
{
    if(_is_initialized) {
        return FSP_SUCCESS;
    }

    fsp_err_t err = R_WDT_Open(&_p_ctrl, &config);
    if(FSP_SUCCESS == err ) {
        R_WDT_Refresh(&_p_ctrl);
        _is_initialized = true;
    }

    return (int)err;
}

void WDTimer::refresh()
{
    R_WDT_Refresh(&_p_ctrl);
}

WDTimer WDT;
