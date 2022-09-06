#include "IRQManager.h"
#include "bsp_api.h"

#define FIXED_IRQ_NUM   16
#define PROG_IRQ_NUM    32

#define UART_SCI2_REQ_NUM   4
#define UART_SCI2_PRIORITY 12

IRQManager::IRQManager() : last_interrupt_index{-1} {

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
    

    if(p == UART_SCI2) {
        uart_cfg_t *p_cfg = (uart_cfg_t *)cfg;
        if( (last_interrupt_index +  UART_SCI2_REQ_NUM) < PROG_IRQ_NUM ) {
            /* TX interrupt */
            last_interrupt_index = 26;
            p_cfg->txi_ipl = UART_SCI2_PRIORITY;
            p_cfg->txi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_txi_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TXI);
            
            /* TX-ERROR interrupt */
            last_interrupt_index++;
            p_cfg->tei_ipl = UART_SCI2_PRIORITY;
            p_cfg->tei_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_tei_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TEI);
            
            /* RX interrupt */
            last_interrupt_index++;
            p_cfg->rxi_ipl = UART_SCI2_PRIORITY;
            p_cfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_rxi_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SCI2_RXI);
            
            /* RX-ERROR interrupt */
            last_interrupt_index++;
            p_cfg->eri_ipl = UART_SCI2_PRIORITY;
            p_cfg->eri_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_uart_eri_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SCI2_ERI);
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */


