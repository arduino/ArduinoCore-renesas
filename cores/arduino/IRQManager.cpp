#include "IRQManager.h"
#include "bsp_api.h"
#include "r_timer_api.h"

#define FIXED_IRQ_NUM   16
#define PROG_IRQ_NUM    32

#define UART_SCI2_REQ_NUM   4
#define I2C_MASTER_REQ_NUM 4
#define I2C_SLAVE_REQ_NUM  2
#define USB_REQ_NUM    4
#define AGT_REQ_NUM    1
#define UART_SCI2_PRIORITY 12
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

            /* USBFS FIFO 0 (DMA transfer request 0) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_0);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* USBFS FIFO 1 (DMA transfer request 1) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_1);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
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
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_AGT0_INT);
            last_interrupt_index++;
        }
        else {
            rv = false;
        }
    }
    /* **********************************************************************
                                      UART
       ********************************************************************** */
    else if(p == IRQ_UART_SCI2 && cfg != NULL) {
        uart_cfg_t *p_cfg = (uart_cfg_t *)cfg;
        
        /* !!!!! TODO: to be generic the EVENT_SCI2_TXI and so on must depend
           on the channel, to be evaluated the use of the same interrupt routine
           for different peripherals */


        if( (last_interrupt_index +  UART_SCI2_REQ_NUM) < PROG_IRQ_NUM ) {
            /* TX interrupt */
            p_cfg->txi_ipl = UART_SCI2_PRIORITY;
            p_cfg->txi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_txi_isr;
            set_sci_tx_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
            
            /* TX-ERROR interrupt */
            p_cfg->tei_ipl = UART_SCI2_PRIORITY;
            p_cfg->tei_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_tei_isr;
            set_sci_tei_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
            
            /* RX interrupt */
            p_cfg->rxi_ipl = UART_SCI2_PRIORITY;
            p_cfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_rxi_isr;
            set_sci_rx_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
            /* RX-ERROR interrupt */
            
            p_cfg->eri_ipl = UART_SCI2_PRIORITY;
            p_cfg->eri_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_eri_isr;
            set_sci_rei_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
        }
        else {
            rv = false;
        }
    }
    /* **********************************************************************
                                      RTC
       ********************************************************************** */
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
    
    else if(p == IRQ_I2C_MASTER && cfg != NULL) {
        I2CIrqMasterReq_t *p_cfg = (I2CIrqMasterReq_t *)cfg;
        iic_master_instance_ctrl_t *ctrl = (iic_master_instance_ctrl_t *)p_cfg->ctrl;
        i2c_master_cfg_t *mcfg = (i2c_master_cfg_t *)p_cfg->cfg;
        
        mcfg->ipl = I2C_MASTER_PRIORITY;
        if( (last_interrupt_index +  I2C_MASTER_REQ_NUM) < PROG_IRQ_NUM ) {
            /* TX interrupt */
            mcfg->txi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_master_txi_isr;
            set_iic_tx_link_event(last_interrupt_index, mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* RX interrupt */
            mcfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_master_rxi_isr;
            set_iic_rx_link_event(last_interrupt_index, mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* TX ERROR interrupt */
            mcfg->tei_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_master_tei_isr;
            set_iic_tei_link_event(last_interrupt_index, mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* RX ERROR interrupt */
            mcfg->eri_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_master_eri_isr;
            set_iic_rei_link_event(last_interrupt_index, mcfg->channel);
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
        mcfg->ipl = I2C_MASTER_PRIORITY;
        if( (last_interrupt_index +  I2C_MASTER_REQ_NUM) < PROG_IRQ_NUM ) {
            /* TX interrupt */
            mcfg->txi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_txi_isr;
            set_sci_tx_link_event(last_interrupt_index, mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* RX interrupt */
            mcfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_rxi_isr;
            set_sci_rx_link_event(last_interrupt_index, mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* TX ERROR interrupt */
            mcfg->tei_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_tei_isr;
            set_sci_tei_link_event(last_interrupt_index, mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;

            /* RX ERROR interrupt */
            #ifdef DO_NOT_USE
            mcfg->eri_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_rei_isr;
            set_sci_tei_link_event(last_interrupt_index, mcfg->channel);
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

            /* TX interrupt */
            p_cfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)iic_slave_rxi_isr;
            set_iic_rx_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
        }
        else {
            rv = false;
        }
    }
    else {
        rv = false;
    }
    __enable_irq();
    return rv;
}

void IRQManager::set_iic_tx_link_event(int li, int ch) {
    if(ch == 0) {      R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC0_TXI);}
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC1_TXI);}
}

void IRQManager::set_iic_rx_link_event(int li, int ch) {
    if(ch == 0) {      R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC0_RXI);}
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC1_RXI);}
}

void IRQManager::set_iic_tei_link_event(int li, int ch) {
    if(ch == 0) {      R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC0_TEI);}
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC1_TEI);}

}

void IRQManager::set_iic_rei_link_event(int li, int ch) {
    if(ch == 0) {      R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC0_ERI);}
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC1_ERI);}
}

void IRQManager::set_sci_tx_link_event(int li, int ch) {
    if(ch == 0) {      R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI0_TXI);}
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI1_TXI);}
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TXI);}
    #ifdef DO_NOT_USE
    else if(ch == 3) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI3_TXI);}
    else if(ch == 4) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI4_TXI);}
    else if(ch == 5) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI5_TXI);}
    else if(ch == 6) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI6_TXI);}
    else if(ch == 7) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI7_TXI);}
    else if(ch == 8) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI8_TXI);}
    #endif
    else if(ch == 9) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI9_TXI);}
}

void IRQManager::set_sci_rx_link_event(int li, int ch) {
    if(ch == 0) {      R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI0_RXI);}
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI1_RXI);}
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI2_RXI);}
    #ifdef DO_NOT_USE
    else if(ch == 3) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI3_RXI);}
    else if(ch == 4) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI4_RXI);}
    else if(ch == 5) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI5_RXI);}
    else if(ch == 6) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI6_RXI);}
    else if(ch == 7) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI7_RXI);}
    else if(ch == 8) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI8_RXI);}
    #endif
    else if(ch == 9) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI9_RXI);}
}
    
void IRQManager::set_sci_tei_link_event(int li, int ch){
    if(ch == 0) {      R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI0_TEI);}
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI1_TEI);}
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TEI);}
    #ifdef DO_NOT_USE
    else if(ch == 3) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI3_TEI);}
    else if(ch == 4) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI4_TEI);}
    else if(ch == 5) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI5_TEI);}
    else if(ch == 6) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI6_TEI);}
    else if(ch == 7) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI7_TEI);}
    else if(ch == 8) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI8_TEI);}
    #endif
    else if(ch == 9) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI9_TEI);}
}
    
void IRQManager::set_sci_rei_link_event(int li, int ch){
    if(ch == 0) {      R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI0_ERI);}
    else if(ch == 1) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI1_ERI);}
    else if(ch == 2) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI2_ERI);}
    #ifdef DO_NOT_USE
    else if(ch == 3) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI3_ERI);}
    else if(ch == 4) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI4_ERI);}
    else if(ch == 5) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI5_ERI);}
    else if(ch == 6) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI6_ERI);}
    else if(ch == 7) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI7_ERI);}
    else if(ch == 8) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI8_ERI);}
    #endif
    else if(ch == 9) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SCI9_ERI);}
}




