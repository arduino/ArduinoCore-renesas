#ifndef _CAN_H_INCLUDED
#define _CAN_H_INCLUDED

#include <Arduino.h>

#include "bsp_api.h"
#include "hal_data.h"

typedef struct can_msg
{
    uint32_t         id;             // CAN ID.
    uint8_t          data_length;    // CAN Data Length Code (DLC).
    uint8_t*         data;           // CAN data.
} CanMessage;

class ArduinoCAN
{
public:
    ArduinoCAN(can_ctrl_t *g_can_ctrl,
               const can_cfg_t *g_can_cfg);
    bool begin();
    void end();
    uint8_t write(CanMessage msg);
    uint8_t read(CanMessage &msg);

    bool tx_complete;
    bool rx_complete;
    bool err_status;

private:
    can_ctrl_t *g_can_ctrl;
    const can_cfg_t *g_can_cfg;

    uint32_t time_out;
    can_info_t rx_info;
};

#if CAN_HOWMANY > 0
extern ArduinoCAN CAN;
#endif
#if SPI_HOWMANY > 1
extern ArduinoCAN CAN1;
#endif

#endif    //_CAN_H_INCLUDED
