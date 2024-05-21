
#ifndef ETHERNET_CLOCK_H
#define ETHERNET_CLOCK_H

#include "FspTimer.h"

#define AGT_TIMER_CHANNEL 3
#define ETHERNET_CLK_PIN  BSP_IO_PORT_06_PIN_00


class EthernetClock {
    public:
        EthernetClock();
        fsp_err_t start();
        fsp_err_t stop();

    private:
        agt_instance_ctrl_t TIMER_ETHERNET_ctrl;
        agt_extended_cfg_t TIMER_ETHERNET_extend;
        timer_cfg_t TIMER_ETHERNET_cfg;
};

#endif
