#ifndef ARDUINO_IRQ_MANAGER_H
#define ARDUINO_IRQ_MANAGER_H

#include "bsp_api.h"
#include "r_uart_api.h"
#include "r_rtc_api.h"
#include "r_rtc.h"
#include "r_i2c_slave_api.h"
#include "r_i2c_master_api.h"
#include "r_iic_master.h"
#include "r_iic_slave.h"

typedef enum {
    IRQ_RTC,
    IRQ_USB,
    IRQ_AGT,
    IRQ_UART_SCI2,
    IRQ_I2C_MASTER,
    IRQ_SCI_I2C_MASTER,
    IRQ_I2C_SLAVE
} Peripheral_t;

typedef enum {
    RTC_ALARM,
    RTC_PERIODIC,
    RTC_CARRY
} RTCIrqReq_t;

typedef struct rtc_irq {
    rtc_cfg_t *cfg;
    rtc_instance_ctrl_t *ctrl;
    RTCIrqReq_t req;
} RTCIrqCfg_t;

typedef struct i2c_master_irq {
    iic_master_instance_ctrl_t *ctrl;
    i2c_master_cfg_t *cfg;

} I2CIrqMasterReq_t;

typedef struct i2c_slave_irq {
    iic_slave_instance_ctrl_t *ctrl;
    i2c_slave_cfg_t  *cfg;

} I2CIrqSlaveReq_t;


typedef struct usb {
    uint32_t num_of_irqs_required;
    uint32_t address_of_handler;
    uint32_t first_irq_number;
} USBIrqCfg_t;


class IRQManager {
    public:
    bool addPeripheral(Peripheral_t p, void *cfg);
    static IRQManager& getInstance();
    IRQManager(IRQManager const&)               = delete;
    void operator=(IRQManager const&)           = delete;
    ~IRQManager();

    private:
    int last_interrupt_index;
    void set_sci_tx_link_event(int li, int ch);
    void set_sci_rx_link_event(int li, int ch);
    void set_sci_tei_link_event(int li, int ch);
    void set_sci_rei_link_event(int li, int ch);

    void set_iic_tx_link_event(int li, int ch);
    void set_iic_rx_link_event(int li, int ch);
    void set_iic_tei_link_event(int li, int ch);
    void set_iic_eri_link_event(int li, int ch);
    IRQManager();

};





#endif