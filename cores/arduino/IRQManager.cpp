#include "IRQManager.h"
#include "bsp_api.h"
#include "r_timer_api.h"

#define FIXED_IRQ_NUM   16
#define PROG_IRQ_NUM    32

#define UART_SCI2_REQ_NUM   4
#define USB_REQ_NUM    4
#define AGT_REQ_NUM    1
#define UART_SCI2_PRIORITY 12
#define USB_PRIORITY  12
#define AGT_PRIORITY  12
#define RTC_PRIORITY  12

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
        
        if( (last_interrupt_index +  UART_SCI2_REQ_NUM) < PROG_IRQ_NUM ) {
            /* TX interrupt */
            p_cfg->txi_ipl = UART_SCI2_PRIORITY;
            p_cfg->txi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_txi_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TXI);
            last_interrupt_index++;
            
            /* TX-ERROR interrupt */
            p_cfg->tei_ipl = UART_SCI2_PRIORITY;
            p_cfg->tei_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_tei_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TEI);
            last_interrupt_index++;
            
            /* RX interrupt */
            p_cfg->rxi_ipl = UART_SCI2_PRIORITY;
            p_cfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_rxi_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SCI2_RXI);
            last_interrupt_index++;
            /* RX-ERROR interrupt */
            
            p_cfg->eri_ipl = UART_SCI2_PRIORITY;
            p_cfg->eri_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_eri_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SCI2_ERI);
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
    else {
        rv = false;
    }
    __enable_irq();
    return rv;
}




