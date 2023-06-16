/*
  WDT.h
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

#ifndef _WDTIMER_H_
#define _WDTIMER_H_

#include "r_wdt.h"

#define WDT_INVALID_TIMEOUT  255
#define WDT_PR_ARRAY_SIZE     16
#define WDT_RL_ARRAY_SIZE      7

class WDTimer {
    public:
    WDTimer();
    ~WDTimer();

    int begin(wdt_cfg_t config);
    int begin(uint32_t timeout_ms);
    void refresh();
    uint32_t getTimeout();
    uint32_t getCounter();

    private:
    bool _is_initialized;
    wdt_instance_ctrl_t _p_ctrl;
    uint32_t _timeout;

    /* WDT prescaler values */
    enum WDT_PR_VALUES
    {
        PR_INVALID =     0,
        PR_1       =     1,
        PR_4       =     4,
        PR_16      =    16,
        PR_32      =    32,
        PR_64      =    64,
        PR_128     =   128,
        PR_256     =   256,
        PR_512     =   512,
        PR_2048    =  2048,
        PR_8192    =  8192,
    };

    /* WDT reload values */
    enum WDT_RL_VALUES
    {
        RL_128     =   128,
        RL_512     =   512,
        RL_1024    =  1024,
        RL_2048    =  2048,
        RL_4096    =  4096,
        RL_8192    =  8192,
        RL_16384   = 16384,
    };

    /* Maps to WDT clock division ratio. e_wdt_clock_division */
    const WDT_PR_VALUES prescaler_values[WDT_PR_ARRAY_SIZE] =
    {
        WDT_PR_VALUES::PR_1,
        WDT_PR_VALUES::PR_4,
        WDT_PR_VALUES::PR_16,
        WDT_PR_VALUES::PR_32,
        WDT_PR_VALUES::PR_64,
        WDT_PR_VALUES::PR_256,
        WDT_PR_VALUES::PR_512,
        WDT_PR_VALUES::PR_2048,
        WDT_PR_VALUES::PR_8192,
        WDT_PR_VALUES::PR_INVALID,
        WDT_PR_VALUES::PR_INVALID,
        WDT_PR_VALUES::PR_INVALID,
        WDT_PR_VALUES::PR_INVALID,
        WDT_PR_VALUES::PR_INVALID,
        WDT_PR_VALUES::PR_INVALID,
        WDT_PR_VALUES::PR_128
    };

    /* Maps to WDT time-out periods. e_wdt_timeout */
    const WDT_RL_VALUES reload_values[WDT_RL_ARRAY_SIZE] =
    {
        WDT_RL_VALUES::RL_128,
        WDT_RL_VALUES::RL_512,
        WDT_RL_VALUES::RL_1024,
        WDT_RL_VALUES::RL_2048,
        WDT_RL_VALUES::RL_4096,
        WDT_RL_VALUES::RL_8192,
        WDT_RL_VALUES::RL_16384
    };

    uint32_t getTicksMs();
    uint32_t getMaxTimeout();
    uint32_t getPrescalerMaxTimeout(WDT_PR_VALUES pr);
    uint8_t getPrescaler(uint32_t timeout_ms);
    uint8_t getReload(uint8_t pr, uint32_t timeout_ms);

};

extern WDTimer WDT;

#endif
