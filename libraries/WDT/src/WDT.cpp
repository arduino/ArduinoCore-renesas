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

WDTimer::WDTimer()
: _is_initialized {false}
, _p_ctrl {0}
, _timeout {0}
{

}

WDTimer::~WDTimer() { }

int WDTimer::begin(wdt_cfg_t config)
{
    if (_is_initialized) {
        return 1;
    }

    fsp_err_t err = R_WDT_Open(&_p_ctrl, &config);
    if (err != FSP_SUCCESS) {
        return 0;
    }
    R_WDT_Refresh(&_p_ctrl);
    _is_initialized = true;

    return 1;
}

int WDTimer::begin(uint32_t timeout_ms)
{
    if (_is_initialized) {
        return 1;
    }

    const uint8_t pr = getPrescaler(timeout_ms);
    if (pr == WDT_INVALID_TIMEOUT) {
        return 0;
    }

    const uint8_t rl = getReload(pr, timeout_ms);

    wdt_cfg_t p_cfg;
    p_cfg.timeout = (wdt_timeout_t)rl;
    p_cfg.clock_division = (wdt_clock_division_t)pr;
    p_cfg.window_start = WDT_WINDOW_START_100;
    p_cfg.window_end = WDT_WINDOW_END_0;
    p_cfg.reset_control = WDT_RESET_CONTROL_RESET;
    p_cfg.stop_control = WDT_STOP_CONTROL_ENABLE;

    fsp_err_t err = R_WDT_Open(&_p_ctrl, &p_cfg);
    if (err != FSP_SUCCESS) {
        return 0;
    }

    R_WDT_Refresh(&_p_ctrl);
    _is_initialized = true;

    return 1;
}

void WDTimer::refresh()
{
    if (_is_initialized) {
        R_WDT_Refresh(&_p_ctrl);
    }
}

uint32_t WDTimer::getTimeout()
{
    if (_is_initialized) {
        return _timeout;
    }
    return 0;
}

uint32_t WDTimer::getCounter()
{
    uint32_t count = 0;

    if (_is_initialized) {
        R_WDT_CounterGet (&_p_ctrl, &count);
    }
    return count;
}

uint32_t WDTimer::getTicksMs()
{
    return (R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB) / 1000);
}

uint32_t WDTimer::getMaxTimeout()
{
    return ((WDT_RL_VALUES::RL_16384 * WDT_PR_VALUES::PR_8192) / getTicksMs());
}

uint32_t WDTimer::getPrescalerMaxTimeout(WDT_PR_VALUES pr)
{
    return ((WDT_RL_VALUES::RL_16384 * pr) / getTicksMs());
}

uint8_t WDTimer::getPrescaler(uint32_t timeout_ms)
{
    const uint32_t max_timeout_ms = getMaxTimeout();

    if (timeout_ms > max_timeout_ms) {
        return WDT_INVALID_TIMEOUT;
    }

    uint32_t pr;
    for (pr = 0; pr < WDT_PR_ARRAY_SIZE; pr++) {
        if (!prescaler_values[pr]) {
            break;
        }

        const uint32_t pr_max_timeout_ms = getPrescalerMaxTimeout(prescaler_values[pr]);
        if (timeout_ms <= pr_max_timeout_ms) {
            break;
        }
    }

    const uint32_t pr_max_timeout_ms = getPrescalerMaxTimeout(WDT_PR_VALUES::PR_128);
    if (pr > WDT_CLOCK_DIVISION_64 && timeout_ms <= pr_max_timeout_ms) {
        return WDT_CLOCK_DIVISION_128;
    }
    return pr;
}

uint8_t WDTimer::getReload(uint8_t pr, uint32_t timeout_ms)
{
    uint32_t rl;
    for (rl = 0; rl < WDT_RL_ARRAY_SIZE; rl++) {
        const uint32_t pr_max_timeout_ms = (reload_values[rl] * prescaler_values[pr]) / (getTicksMs());
        if (timeout_ms <= pr_max_timeout_ms) {
            _timeout = pr_max_timeout_ms;
            return rl;
        }
    }
    return  rl;
}

WDTimer WDT;
