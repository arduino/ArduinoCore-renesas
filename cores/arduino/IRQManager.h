#ifndef ARDUINO_IRQ_MANAGER_H
#define ARDUINO_IRQ_MANAGER_H

#include "bsp_api.h"
#include "r_uart_api.h"

typedef enum {
    RTC,
    UART_SCI2,
    I2C_MASTER,
    I2C_SLAVE
} Peripheral_t;


class IRQManager {
    public:
    bool addPeripheral(Peripheral_t p, uart_cfg_t &cfg);
    static IRQManager& getInstance();
    IRQManager(IRQManager const&)               = delete;
    void operator=(IRQManager const&)           = delete;
    ~IRQManager();

    private:
    int last_interrupt_index;
    IRQManager();

};





#endif