#include "IRQManager.h"
#include "bsp_api.h"

#define FIXED_IRQ_NUM   16
#define PROG_IRQ_NUM    32

#define UART_SCI_REQ_NUM   4
#define I2C_MASTER_REQ_NUM 4
#define I2C_SLAVE_REQ_NUM  2
#define USB_REQ_NUM    4
#define AGT_REQ_NUM    1
#define EXTERNAL_PIN_NUM 1
#define EXTERNAL_PIN_PRIORITY 12
#define UART_SCI_PRIORITY 12
#define USB_PRIORITY  12
#define AGT_PRIORITY  12
#define RTC_PRIORITY  12
#define I2C_MASTER_PRIORITY 12
#define I2C_SLAVE_PRIORITY 12

#define FIRST_INT_SLOT_FREE 0

IRQManager::IRQManager() : last_interrupt_index{0} {

}

IRQManager::~IRQManager() {

}

IRQManager& IRQManager::getInstance() {
    static IRQManager    instance;
    return instance;
}

 

bool IRQManager::addPeripheral(Peripheral_t p, void *cfg) {
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;

    __disable_irq();
    /* **********************************************************************
                                      USB
       ********************************************************************** */
    if(p == IRQ_USB && cfg != NULL) {
        USBIrqCfg_t *irqcfg = (USBIrqCfg_t *)cfg;

        /* configuring USB interrupts */
        /* in case of USB which does not use any FSP API the cfg contains the 
           only interrupt handler function used */
        if( (last_interrupt_index +  irqcfg->num_of_irqs_required ) < PROG_IRQ_NUM ) {

            if(irqcfg->num_of_irqs_required == 4) {
                /* USBFS INT (USBFS interrupt) */
                *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
                R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBFS_INT);
                R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
                R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
                NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
                R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
                irqcfg->first_irq_number = last_interrupt_index;
                last_interrupt_index++;
            }

            /* USBFS RESUME (USBFS resume interrupt) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBFS_RESUME);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            #ifdef EVENT_USBFS_FIFO_0
            /* USBFS FIFO 0 (DMA transfer request 0) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_0);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
            #endif

            #ifdef EVENT_USBFS_FIFO_1
            /* USBFS FIFO 1 (DMA transfer request 1) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_1);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
            #endif
        }
        else {
            rv = false;
        }
    }
    /* **********************************************************************
                                      AGT
       ********************************************************************** */
    else if(p == IRQ_AGT && cfg != NULL) {
        if( (last_interrupt_index +  AGT_REQ_NUM) < PROG_IRQ_NUM ) {
            timer_cfg_t *p_cfg = (timer_cfg_t *)cfg;
            p_cfg->cycle_end_ipl = AGT_PRIORITY;
            p_cfg->cycle_end_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)agt_int_isr;
            set_agt_link_event(last_interrupt_index, p_cfg->channel);
            last_interrupt_index++;
        }
        else {
            rv = false;
        }
    }
    /* **********************************************************************
                                      UART
       ********************************************************************** */
    else if(p == IRQ_SCI_UART && cfg != NULL) {
        uart_cfg_t *p_cfg = (uart_cfg_t *)cfg;
        if( (last_interrupt_index +  UART_SCI_REQ_NUM) < PROG_IRQ_NUM ) {

            if (p_cfg->txi_irq == FSP_INVALID_VECTOR) {
                /* TX interrupt */
                p_cfg->txi_ipl = UART_SCI_PRIORITY;
                p_cfg->txi_irq = (IRQn_Type)last_interrupt_index;
                *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_txi_isr;
                set_sci_tx_link_event(last_interrupt_index, p_cfg->channel);
                last_interrupt_index++;

                /* TX-ERROR interrupt */
                p_cfg->tei_ipl = UART_SCI_PRIORITY;
                p_cfg->tei_irq = (IRQn_Type)last_interrupt_index;
                *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_tei_isr;
                set_sci_tei_link_event(last_interrupt_index, p_cfg->channel);
                last_interrupt_index++;

                /* RX interrupt */
                p_cfg->rxi_ipl = UART_SCI_PRIORITY;
                p_cfg->rxi_irq = (IRQn_Type)last_interrupt_index;
                *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_rxi_isr;
                set_sci_rx_link_event(last_interrupt_index, p_cfg->channel);
                last_interrupt_index++;

                /* RX-ERROR interrupt */
                p_cfg->eri_ipl = UART_SCI_PRIORITY;
                p_cfg->eri_irq = (IRQn_Type)last_interrupt_index;
                *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_eri_isr;
                set_sci_rei_link_event(last_interrupt_index, p_cfg->channel);
                last_interrupt_index++;
            }
            R_BSP_IrqEnable (p_cfg->txi_irq);
            R_BSP_IrqEnable (p_cfg->tei_irq);
            R_BSP_IrqEnable (p_cfg->rxi_irq);
            R_BSP_IrqEnable (p_cfg->eri_irq);
        }
        else {
            rv = false;
        }
    }
    /* **********************************************************************
                                      RTC
       ********************************************************************** */
#if RTC_HOWMANY > 0
    else if(p == IRQ_RTC && cfg != NULL) {
        RTCIrqCfg_t *p_cfg = (RTCIrqCfg_t *)cfg;
        /* rtc interrupts are added once a time and not all three together */
        if( (last_interrupt_index) < PROG_IRQ_NUM ) {
            
            if(p_cfg->req == RTC_ALARM) {
                p_cfg->cfg->alarm_ipl = RTC_PRIORITY;
                p_cfg->cfg->alarm_irq = (IRQn_Type)last_interrupt_index;
                *(irq_ptr + last_interrupt_index) = (uint32_t)rtc_alarm_periodic_isr;
                R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_RTC_ALARM);
                R_BSP_IrqCfg(p_cfg->cfg->alarm_irq, p_cfg->cfg->alarm_ipl, p_cfg->ctrl);
                R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
                last_interrupt_index++;
            }
            else if(p_cfg->req == RTC_PERIODIC) {
                p_cfg->cfg->periodic_ipl = RTC_PRIORITY;
                p_cfg->cfg->periodic_irq = (IRQn_Type)last_interrupt_index;
                *(irq_ptr + last_interrupt_index) = (uint32_t)rtc_alarm_periodic_isr;
                R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_RTC_PERIOD);
                R_BSP_IrqCfg(p_cfg->cfg->periodic_irq, p_cfg->cfg->periodic_ipl, p_cfg->ctrl);
                R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
                last_interrupt_index++;
            }
            else if(p_cfg->req == RTC_CARRY) {
                p_cfg->cfg->carry_ipl = RTC_PRIORITY;
                p_cfg->cfg->carry_irq = (IRQn_Type)last_interrupt_index;
                *(irq_ptr + last_interrupt_index) = (uint32_t)rtc_carry_isr;
                R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_RTC_CARRY);
                R_BSP_IrqCfg(p_cfg->cfg->carry_irq, p_cfg->cfg->carry_ipl, p_cfg->ctrl);
                R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
                last_interrupt_index++;
            }
        }
        else {
            rv = false;
        }
    }
#endif

#if WIRE_HOWMANY > 0
    else if(p == IRQ_I2C_MASTER && cfg != NULL) {
        I2CIrqMasterReq_t *p_cfg = (I2CIrqMasterReq_t *)cfg;
        iic_master_instance_ctrl_t *ctrl = (iic_master_instance_ctrl_t *)p_cfg->ctrl;
        i2c_master_cfg_t *mcfg = (i2c_master_cfg_t *)p_cfg->cfg;
        uint8_t hw_channel = p_cfg->hw_channel;
        mcfg->ipl = I2C_MASTER_PRIORITY;
        if( (last_interrupt_index +  I2C_MASTER_REQ_NUM) < PROG_IRQ_NUM ) {
            /* TX interrupt */
            mcfg->txi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_master_txi_isr;
            set_iic_tx_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* RX interrupt */
            mcfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_master_rxi_isr;
            set_iic_rx_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* TX ERROR interrupt */
            mcfg->tei_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_master_tei_isr;
            set_iic_tei_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* RX ERROR interrupt */
            mcfg->eri_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_master_eri_isr;
            set_iic_eri_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
        }
        else {
            rv = false;
        }
    }
    else if(p == IRQ_SCI_I2C_MASTER && cfg != NULL) {
        I2CIrqMasterReq_t *p_cfg = (I2CIrqMasterReq_t *)cfg;
        iic_master_instance_ctrl_t *ctrl = (iic_master_instance_ctrl_t *)p_cfg->ctrl;
        i2c_master_cfg_t *mcfg = (i2c_master_cfg_t *)p_cfg->cfg;
        uint8_t hw_channel = p_cfg->hw_channel;
        mcfg->ipl = I2C_MASTER_PRIORITY;
        if( (last_interrupt_index +  I2C_MASTER_REQ_NUM) < PROG_IRQ_NUM ) {
            /* TX interrupt */
            mcfg->txi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_txi_isr;
            set_sci_tx_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* RX interrupt */
            mcfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_rxi_isr;
            set_sci_rx_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* TX ERROR interrupt */
            mcfg->tei_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_tei_isr;
            set_sci_tei_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* RX ERROR interrupt */
            #ifdef DO_NOT_USE
            mcfg->eri_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_rei_isr;
            set_sci_tei_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
            #endif
        }
        else {
            rv = false;
        }

    }
    else if(p == IRQ_I2C_SLAVE && cfg != NULL) {
        i2c_slave_cfg_t *p_cfg = (i2c_slave_cfg_t *)cfg;
        p_cfg->ipl = I2C_SLAVE_PRIORITY;
        p_cfg->eri_ipl = I2C_SLAVE_PRIORITY;
        if( (last_interrupt_index +  I2C_SLAVE_REQ_NUM) < PROG_IRQ_NUM ) {
            /* TX interrupt */
            p_cfg->txi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_slave_txi_isr;
            set_iic_tx_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* RX interrupt */
            p_cfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_slave_rxi_isr;
            set_iic_rx_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* TEI interrupt */
            p_cfg->tei_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_slave_tei_isr;
            set_iic_tei_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* ERI interrupt */
            p_cfg->eri_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_slave_eri_isr;
            set_iic_eri_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
        }
        else {
            rv = false;
        }
    }
#endif

#if EXT_INTERRUPTS_HOWMANY > 0
    /* **********************************************************************
                             PIN EXTERNAL INTERRUPT
       ********************************************************************** */
    else if(p == IRQ_EXTERNAL_PIN && cfg != NULL) {
        external_irq_cfg_t *p_cfg = (external_irq_cfg_t *)cfg;
        if( (p_cfg->irq == FSP_INVALID_VECTOR) && (last_interrupt_index + EXTERNAL_PIN_NUM) < PROG_IRQ_NUM ) {
            p_cfg->ipl = EXTERNAL_PIN_PRIORITY;
            p_cfg->irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)r_icu_isr;
            set_ext_link_event(last_interrupt_index, p_cfg->channel);
            last_interrupt_index++;
        }
        else {
            if(p_cfg->irq == FSP_INVALID_VECTOR) {
                rv = false;
            }
            else {
                rv = true;
            }
        }
    }
#endif

    else {
        rv = false;
    }
    __enable_irq();
    return rv;
}

void IRQManager::set_ext_link_event(int li, int ch) {
    if (0) {}
#ifdef EVENT_ICU_IRQ0
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ0);
    }
#endif
#ifdef EVENT_ICU_IRQ1
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ1);
    }
#endif
#ifdef EVENT_ICU_IRQ2
    else if(ch == 2) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ2);
    }
#endif
#ifdef EVENT_ICU_IRQ3
    else if(ch == 3) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ3);
    }
#endif
#ifdef EVENT_ICU_IRQ4
    else if(ch == 4) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ4);
    }
#endif
#ifdef EVENT_ICU_IRQ5
    else if(ch == 5) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ5);
    }
#endif
#ifdef EVENT_ICU_IRQ6
    else if(ch == 6) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ6);
    }
#endif
#ifdef EVENT_ICU_IRQ7
    else if(ch == 7) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ7);
    }
#endif
#ifdef EVENT_ICU_IRQ8
    else if(ch == 8) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ8);
    }
#endif
#ifdef EVENT_ICU_IRQ9
    else if(ch == 9) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ9);
    }
#endif
#ifdef EVENT_ICU_IRQ10
    else if(ch == 10) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ10);
    }
#endif
#ifdef EVENT_ICU_IRQ11
    else if(ch == 11) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ11);
    }
#endif
#ifdef EVENT_ICU_IRQ12
    else if(ch == 12) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ12);
    }
#endif
#ifdef EVENT_ICU_IRQ13
    else if(ch == 13) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ13);
    }
#endif
#ifdef EVENT_ICU_IRQ14
    else if(ch == 14) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ14);
    }
#endif
#ifdef EVENT_ICU_IRQ15
    else if(ch == 15) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ15);
    }
#endif
}

void IRQManager::set_agt_link_event(int li, int ch) {
    if (0) {}
#ifdef ELC_EVENT_AGT0_INT
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT0_INT);
    }
#endif
#ifdef ELC_EVENT_AGT1_INT
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT1_INT);
    }
#endif
#ifdef ELC_EVENT_AGT2_INT
    else if(ch == 2) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT2_INT);
    }
#endif  
#ifdef ELC_EVENT_AGT3_INT
    else if(ch == 3) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT3_INT);
    }
#endif    
#ifdef ELC_EVENT_AGT4_INT
    else if(ch == 4) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT4_INT);
    }
#endif  
#ifdef ELC_EVENT_AGT5_INT
    else if(ch == 5) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT5_INT);
    }
#endif  

}

void IRQManager::set_iic_tx_link_event(int li, int ch) {
    if (0) {}
#ifdef ELC_EVENT_IIC2_TXI
    else if(ch == 0) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC0_TXI);}
#endif
#ifdef ELC_EVENT_IIC2_TXI
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC1_TXI);}
#endif
#ifdef ELC_EVENT_IIC2_TXI
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC2_TXI);}
#endif
}

void IRQManager::set_iic_rx_link_event(int li, int ch) {
    if (0) {}
#ifdef ELC_EVENT_IIC0_RXI
    else if(ch == 0) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC0_RXI);}
#endif
#ifdef ELC_EVENT_IIC1_RXI
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC1_RXI);}
#endif
#ifdef ELC_EVENT_IIC2_RXI
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC2_RXI);}
#endif    
}

void IRQManager::set_iic_tei_link_event(int li, int ch) {
    if (0) {}
#ifdef ELC_EVENT_IIC0_TEI
    else if(ch == 0) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC0_TEI);}
#endif
#ifdef ELC_EVENT_IIC1_TEI
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC1_TEI);}
#endif
#ifdef ELC_EVENT_IIC2_TEI
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC2_TEI);}
#endif    

}

void IRQManager::set_iic_eri_link_event(int li, int ch) {
    if (0) {}
#ifdef ELC_EVENT_IIC0_ERI
    else if(ch == 0) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC0_ERI);}
#endif
#ifdef ELC_EVENT_IIC1_ERI
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC1_ERI);}
#endif
#ifdef ELC_EVENT_IIC2_ERI
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC2_ERI);}
#endif    
}

void IRQManager::set_sci_tx_link_event(int li, int ch) {
    if (0) {}
#ifdef ELC_EVENT_SCI0_TXI
    else if(ch == 0) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI0_TXI);}
#endif
#ifdef ELC_EVENT_SCI1_TXI
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI1_TXI);}
#endif
#ifdef ELC_EVENT_SCI2_TXI
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TXI);}
#endif
#ifdef ELC_EVENT_SCI3_TXI
    else if(ch == 3) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI3_TXI);}
#endif
#ifdef ELC_EVENT_SCI4_TXI
    else if(ch == 4) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI4_TXI);}
#endif
#ifdef ELC_EVENT_SCI5_TXI
    else if(ch == 5) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI5_TXI);}
#endif
#ifdef ELC_EVENT_SCI6_TXI
    else if(ch == 6) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI6_TXI);}
#endif
#ifdef ELC_EVENT_SCI7_TXI
    else if(ch == 7) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI7_TXI);}
#endif
#ifdef ELC_EVENT_SCI8_TXI
    else if(ch == 8) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI8_TXI);}
#endif
#ifdef ELC_EVENT_SCI9_TXI
    else if(ch == 9) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI9_TXI);}
#endif
}

void IRQManager::set_sci_rx_link_event(int li, int ch) {
    if (0) {}
#ifdef ELC_EVENT_SCI0_RXI
    else if(ch == 0) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI0_RXI);}
#endif
#ifdef ELC_EVENT_SCI1_RXI
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI1_RXI);}
#endif
#ifdef ELC_EVENT_SCI2_RXI
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI2_RXI);}
#endif
#ifdef ELC_EVENT_SCI3_RXI
    else if(ch == 3) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI3_RXI);}
#endif
#ifdef ELC_EVENT_SCI4_RXI
    else if(ch == 4) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI4_RXI);}
#endif
#ifdef ELC_EVENT_SCI5_RXI
    else if(ch == 5) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI5_RXI);}
#endif
#ifdef ELC_EVENT_SCI6_RXI
    else if(ch == 6) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI6_RXI);}
#endif
#ifdef ELC_EVENT_SCI7_RXI
    else if(ch == 7) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI7_RXI);}
#endif
#ifdef ELC_EVENT_SCI8_RXI
    else if(ch == 8) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI8_RXI);}
#endif
#ifdef ELC_EVENT_SCI9_RXI
    else if(ch == 9) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI9_RXI);}
#endif
}
    
void IRQManager::set_sci_tei_link_event(int li, int ch){
    if (0) {}
#ifdef ELC_EVENT_SCI0_TEI
    else if(ch == 0) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI0_TEI);}
#endif
#ifdef ELC_EVENT_SCI1_TEI
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI1_TEI);}
#endif
#ifdef ELC_EVENT_SCI2_TEI
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TEI);}
#endif
#ifdef ELC_EVENT_SCI3_TEI
    else if(ch == 3) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI3_TEI);}
#endif
#ifdef ELC_EVENT_SCI4_TEI
    else if(ch == 4) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI4_TEI);}
#endif
#ifdef ELC_EVENT_SCI5_TEI
    else if(ch == 5) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI5_TEI);}
#endif
#ifdef ELC_EVENT_SCI6_TEI
    else if(ch == 6) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI6_TEI);}
#endif
#ifdef ELC_EVENT_SCI7_TEI
    else if(ch == 7) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI7_TEI);}
#endif
#ifdef ELC_EVENT_SCI8_TEI
    else if(ch == 8) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI8_TEI);}
#endif
#ifdef ELC_EVENT_SCI9_TEI
    else if(ch == 9) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI9_TEI);}
#endif
}
    
void IRQManager::set_sci_rei_link_event(int li, int ch){
    if (0) {}
#ifdef ELC_EVENT_SCI0_ERI
    else if(ch == 0) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI0_ERI);}
#endif
#ifdef ELC_EVENT_SCI1_ERI
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI1_ERI);}
#endif
#ifdef ELC_EVENT_SCI2_ERI
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI2_ERI);}
#endif
#ifdef ELC_EVENT_SCI3_ERI
    else if(ch == 3) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI3_ERI);}
#endif
#ifdef ELC_EVENT_SCI4_ERI
    else if(ch == 4) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI4_ERI);}
#endif
#ifdef ELC_EVENT_SCI5_ERI
    else if(ch == 5) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI5_ERI);}
#endif
#ifdef ELC_EVENT_SCI6_ERI
    else if(ch == 6) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI6_ERI);}
#endif
#ifdef ELC_EVENT_SCI7_ERI
    else if(ch == 7) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI7_ERI);}
#endif
#ifdef ELC_EVENT_SCI8_ERI
    else if(ch == 8) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI8_ERI);}
#endif
#ifdef ELC_EVENT_SCI9_ERI
    else if(ch == 9) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI9_ERI);}
#endif
}




