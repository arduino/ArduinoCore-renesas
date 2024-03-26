#include "IRQManager.h"
#include "bsp_api.h"

#define FIXED_IRQ_NUM   16


#define PROG_IRQ_NUM    BSP_ICU_VECTOR_MAX_ENTRIES //32

#define ETHERNET_PRIORITY          12
#define SDCARD_ACCESS_PRIORITY     10
#define SDCARD_DMA_REQ_PRIORITY    10
#define SDCARD_CARD_PRIORITY       12
#define EXTERNAL_PIN_PRIORITY      12
#define UART_SCI_PRIORITY          12
#define USB_PRIORITY               12
#define AGT_PRIORITY               14
#define RTC_PRIORITY               12
#define I2C_MASTER_PRIORITY        12
#define I2C_SLAVE_PRIORITY         12
#define SPI_MASTER_PRIORITY        6
#define DMA_PRIORITY               12
#define TIMER_PRIORITY             8
#define ADC_PRIORITY               12
#define CAN_PRIORITY               12
#define CANFD_PRIORITY             12
#define I2S_PRIORITY               12
#define FIRST_INT_SLOT_FREE         0

IRQManager::IRQManager() : last_interrupt_index{0} {

}

IRQManager::~IRQManager() {

}

IRQManager& IRQManager::getInstance() {
    static IRQManager    instance;
    return instance;
}

bool IRQManager::addGenericInterrupt(GenericIrqCfg_t &cfg, Irq_f fnc /*= nullptr*/){
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = false;
    
    if((cfg.irq == FSP_INVALID_VECTOR) && (last_interrupt_index < PROG_IRQ_NUM)) {
    	if(fnc != nullptr){
    		R_ICU->IELSR[last_interrupt_index] = cfg.event;
    		*(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
    		R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
    		R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
    		NVIC_SetPriority((IRQn_Type)last_interrupt_index, cfg.ipl);
    		R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
    		cfg.irq = (IRQn_Type)last_interrupt_index;
    		last_interrupt_index++;
    		rv = true;
    	}
    }
	return rv;
}

bool IRQManager::addADCScanEnd(ADC_Container *adc, Irq_f fnc /*= nullptr*/) {
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;
    if (last_interrupt_index >= PROG_IRQ_NUM){
        rv = false;
    }
    else if (adc->cfg.scan_end_irq == FSP_INVALID_VECTOR) {
        if(set_adc_end_link_event(last_interrupt_index, adc->cfg.unit)) {
            adc->cfg.scan_end_ipl = TIMER_PRIORITY;
            adc->cfg.scan_end_irq = (IRQn_Type)last_interrupt_index;
            
            if(fnc == nullptr) {
                *(irq_ptr + last_interrupt_index) = (uint32_t)adc_scan_end_isr;
            }
            else {
                *(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
            }
            last_interrupt_index++;
        }
    }
    else {
        if(adc->cfg.scan_end_irq == FSP_INVALID_VECTOR) {
            rv = false;
        }
        else {
            rv = true;
        }
    }
    return rv;
}



bool IRQManager::addADCScanEndB(ADC_Container *adc, Irq_f fnc /*= nullptr*/) {
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;
    if (last_interrupt_index >= PROG_IRQ_NUM){
        rv = false;
    }
    else if (adc->cfg.scan_end_b_irq == FSP_INVALID_VECTOR) {
        if(set_adc_end_b_link_event(last_interrupt_index, adc->cfg.unit)) {
            adc->cfg.scan_end_b_ipl = TIMER_PRIORITY;
            adc->cfg.scan_end_b_irq = (IRQn_Type)last_interrupt_index;
            
            if(fnc == nullptr) {
                *(irq_ptr + last_interrupt_index) = (uint32_t)adc_scan_end_b_isr;
            }
            else {
                *(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
            }
            last_interrupt_index++;
        }
    }
    return rv;
}

bool IRQManager::addADCWinCmpA(ADC_Container *adc, Irq_f fnc /*= nullptr*/) {
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;
    if (last_interrupt_index >= PROG_IRQ_NUM){
        rv = false;
    }
    else if( ((adc_extended_cfg_t *)(adc->cfg.p_extend))->window_a_irq == FSP_INVALID_VECTOR) {
        if(set_adc_win_a_link_event(last_interrupt_index, adc->cfg.unit)) {
            ((adc_extended_cfg_t *)(adc->cfg.p_extend))->window_a_ipl = TIMER_PRIORITY;
            ((adc_extended_cfg_t *)(adc->cfg.p_extend))->window_a_irq = (IRQn_Type)last_interrupt_index;
            
            if(fnc == nullptr) {
                *(irq_ptr + last_interrupt_index) = (uint32_t)adc_window_compare_isr;
            }
            else {
                *(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
            }
            last_interrupt_index++;
        }
    }
    return rv;

}

bool IRQManager::addADCWinCmpB(ADC_Container *adc, Irq_f fnc /*= nullptr*/) {
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;
    if (last_interrupt_index >= PROG_IRQ_NUM){
        rv = false;
    }
    else if (((adc_extended_cfg_t *)(adc->cfg.p_extend))->window_b_irq == FSP_INVALID_VECTOR) {
        if(set_adc_win_b_link_event(last_interrupt_index, adc->cfg.unit)) {
            ((adc_extended_cfg_t *)(adc->cfg.p_extend))->window_b_ipl = TIMER_PRIORITY;
            ((adc_extended_cfg_t *)(adc->cfg.p_extend))->window_b_irq = (IRQn_Type)last_interrupt_index;
            
            if(fnc == nullptr) {
                *(irq_ptr + last_interrupt_index) = (uint32_t)adc_window_compare_isr;
            }
            else {
                *(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
            }
            last_interrupt_index++;
        }
    }
    return rv;

}

/* -------------------------------------------------------------------------- */
bool IRQManager::addTimerOverflow(TimerIrqCfg_t &cfg, Irq_f fnc /* = nullptr */) {
/* -------------------------------------------------------------------------- */    
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;
    if (last_interrupt_index >= PROG_IRQ_NUM){
        rv = false;
    }
    else if (cfg.base_cfg->cycle_end_irq == FSP_INVALID_VECTOR) {
        if(cfg.gpt_ext_cfg != nullptr) {
            if(set_gpt_over_link_event(last_interrupt_index, cfg.base_cfg->channel)) {
                cfg.base_cfg->cycle_end_ipl = TIMER_PRIORITY;
                cfg.base_cfg->cycle_end_irq = (IRQn_Type)last_interrupt_index;
                /* that is a GPT timer */
                if(fnc == nullptr) {
                    *(irq_ptr + last_interrupt_index) = (uint32_t)gpt_counter_overflow_isr;
                }
                else {
                    *(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
                }
                last_interrupt_index++;
            }
        }
        else if(cfg.agt_ext_cfg != nullptr) {
            /* DOUBT: is EVENT_AGT0_INT Overflow?? */
            if(set_agt_link_event(last_interrupt_index, cfg.base_cfg->channel)) {
                cfg.base_cfg->cycle_end_ipl = TIMER_PRIORITY;
                cfg.base_cfg->cycle_end_irq = (IRQn_Type)last_interrupt_index;
                /* that is a AGT timer */
                if(fnc == nullptr) {
                    *(irq_ptr + last_interrupt_index) = (uint32_t)agt_int_isr;
                }
                else {
                    *(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
                }
                last_interrupt_index++;
            }
        }
        else {
            rv = false;
        }
    }
    return rv;
}

/* -------------------------------------------------------------------------- */
bool IRQManager::addTimerUnderflow(TimerIrqCfg_t &cfg, Irq_f fnc /*= nullptr*/) {
/* -------------------------------------------------------------------------- */ 
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;
    
    if((cfg.agt_ext_cfg != nullptr) || (last_interrupt_index >= PROG_IRQ_NUM)) {
        /* not supported for AGT  */
        rv = false;
    }
    else if(cfg.gpt_ext_cfg != nullptr && cfg.gpt_ext_cfg->p_pwm_cfg != nullptr) {
        if (cfg.gpt_ext_cfg->p_pwm_cfg->trough_irq == FSP_INVALID_VECTOR) {
            if(set_gpt_under_link_event(last_interrupt_index, cfg.base_cfg->channel)){
                ((gpt_extended_pwm_cfg_t *)(cfg.gpt_ext_cfg->p_pwm_cfg))->trough_ipl = TIMER_PRIORITY;
                ((gpt_extended_pwm_cfg_t *)(cfg.gpt_ext_cfg->p_pwm_cfg))->trough_irq = (IRQn_Type)last_interrupt_index;
                if(fnc == nullptr) {
                    *(irq_ptr + last_interrupt_index) = (uint32_t)gpt_counter_underflow_isr;
                }
                else {
                    *(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
                }
                last_interrupt_index++;
            }
        }
    }
    else {
        rv = false;
    }
    return rv;   
}

/* -------------------------------------------------------------------------- */
bool IRQManager::addTimerCompareCaptureA(TimerIrqCfg_t &cfg, Irq_f fnc /*= nullptr*/) {
/* -------------------------------------------------------------------------- */    
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;
    
    if((cfg.agt_ext_cfg != nullptr) || (last_interrupt_index >= PROG_IRQ_NUM)) {
        /* not supported for AGT  */
        rv = false;
    }
    else if(cfg.gpt_ext_cfg != nullptr ) {
        if (cfg.gpt_ext_cfg->capture_a_irq == FSP_INVALID_VECTOR) {
            if(set_gpt_compare_capture_A_link_event(last_interrupt_index, cfg.base_cfg->channel)){
                cfg.gpt_ext_cfg->capture_a_ipl = TIMER_PRIORITY;
                cfg.gpt_ext_cfg->capture_a_irq = (IRQn_Type)last_interrupt_index;
                if(fnc == nullptr) {
                    *(irq_ptr + last_interrupt_index) = (uint32_t)gpt_capture_a_isr;
                }
                else {
                    *(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
                }
                last_interrupt_index++;
            }
        }
    }
    else {
        rv = false;
    }
    return rv;   
}

/* -------------------------------------------------------------------------- */
bool IRQManager::addTimerCompareCaptureB(TimerIrqCfg_t &cfg, Irq_f fnc /*= nullptr*/) {
/* -------------------------------------------------------------------------- */
   /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;
    
    if((cfg.agt_ext_cfg != nullptr) || (last_interrupt_index >= PROG_IRQ_NUM)) {
        /* not supported for AGT  */
        rv = false;
    }
    else if(cfg.gpt_ext_cfg != nullptr ) {
        if (cfg.gpt_ext_cfg->capture_b_irq == FSP_INVALID_VECTOR) {
            if(set_gpt_compare_capture_B_link_event(last_interrupt_index, cfg.base_cfg->channel)){
                cfg.gpt_ext_cfg->capture_b_ipl = TIMER_PRIORITY;
                cfg.gpt_ext_cfg->capture_b_irq = (IRQn_Type)last_interrupt_index;
                if(fnc == nullptr) {
                    *(irq_ptr + last_interrupt_index) = (uint32_t)gpt_capture_b_isr;
                }
                else {
                    *(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
                }
                last_interrupt_index++;
            }
        }
    }
    else {
        rv = false;
    }
    return rv;   
}

#ifdef HAS_DMAC
/* -------------------------------------------------------------------------- */
bool IRQManager::addDMA(dmac_extended_cfg_t &cfg, Irq_f fnc /* = nullptr */) {
/* -------------------------------------------------------------------------- */    
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;
    if (last_interrupt_index >= PROG_IRQ_NUM){
        rv = false;
    }
    else if (cfg.irq == FSP_INVALID_VECTOR) {
        /* to check correctness of the channel */
        if(set_dma_link_event(last_interrupt_index, cfg.channel)) {
            cfg.ipl = DMA_PRIORITY;
            cfg.irq = (IRQn_Type)last_interrupt_index;
            if(fnc == nullptr) {
                *(irq_ptr + last_interrupt_index) = (uint32_t)dmac_int_isr;
            }
            else {
                *(irq_ptr + last_interrupt_index) = (uint32_t)fnc;
            }
            last_interrupt_index++;
        }
        else {
            rv = false;
        }
    }
    return rv;
}
#endif

/* -------------------------------------------------------------------------- */
bool IRQManager::addPeripheral(Peripheral_t p, void *cfg) {
/* -------------------------------------------------------------------------- */    
    /* getting the address of the current location of the irq vector table */
    volatile uint32_t *irq_ptr = (volatile uint32_t *)SCB->VTOR;
    /* set the displacement to the "programmable" part of the table */
    irq_ptr += FIXED_IRQ_NUM;
    bool rv = true;

    if(last_interrupt_index >= PROG_IRQ_NUM){
        rv = false;
        goto end_config;
    }

    __disable_irq();
    /* **********************************************************************
                                      USB
       ********************************************************************** */
    if(p == IRQ_USB && cfg != NULL) {
        USBIrqCfg_t *irqcfg = (USBIrqCfg_t *)cfg;

        if (irqcfg->first_irq_number != FSP_INVALID_VECTOR) {
            // already configured, return
            goto end_config;
        }

        /* configuring USB interrupts */
        /* in case of USB which does not use any FSP API the cfg contains the 
           only interrupt handler function used */
        if( (last_interrupt_index +  irqcfg->num_of_irqs_required ) < PROG_IRQ_NUM ) {

            #ifdef ELC_EVENT_USBFS_INT
            /* USBFS INT (USBFS interrupt) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBFS_INT);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            irqcfg->first_irq_number = last_interrupt_index;
            last_interrupt_index++;
            #endif

            #ifdef ELC_EVENT_USBFS_RESUME
            /* USBFS RESUME (USBFS resume interrupt) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBFS_RESUME);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
            #endif

            #ifdef ELC_EVENT_USBFS_FIFO_0
            /* USBFS FIFO 0 (DMA transfer request 0) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_0);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
            #endif

            #ifdef ELC_EVENT_USBFS_FIFO_1
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

    else if(p == IRQ_USB_HS && cfg != NULL) {
        USBIrqCfg_t *irqcfg = (USBIrqCfg_t *)cfg;

        if (irqcfg->first_irq_number != FSP_INVALID_VECTOR) {
            // already configured, return
            goto end_config;
        }

        /* configuring USB interrupts */
        /* in case of USB which does not use any FSP API the cfg contains the 
           only interrupt handler function used */
        if( (last_interrupt_index +  irqcfg->num_of_irqs_required ) < PROG_IRQ_NUM ) {

            #ifdef ELC_EVENT_USBHS_USB_INT_RESUME
            /* USBHS INT (USBFS interrupt) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBHS_USB_INT_RESUME);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            irqcfg->first_irq_number = last_interrupt_index;
            last_interrupt_index++;
            #endif

            #ifdef ELC_EVENT_USBHS_FIFO_0
            /* USBFS FIFO 0 (DMA transfer request 0) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBHS_FIFO_0);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
            #endif

            #ifdef ELC_EVENT_USBHS_FIFO_1
            /* USBFS FIFO 1 (DMA transfer request 1) */
            *(irq_ptr + last_interrupt_index) = (uint32_t)irqcfg->address_of_handler;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_USBHS_FIFO_1);
            R_BSP_IrqDisable((IRQn_Type)last_interrupt_index);
            R_BSP_IrqStatusClear((IRQn_Type)last_interrupt_index);
            NVIC_SetPriority((IRQn_Type)last_interrupt_index, USB_PRIORITY);
            R_BSP_IrqEnable ((IRQn_Type)last_interrupt_index);
            last_interrupt_index++;
            #endif
        } else {
            rv = false;
        }
    }

    /* **********************************************************************
                                      AGT
       ********************************************************************** */
    else if(p == IRQ_AGT && cfg != NULL) {
        timer_cfg_t *p_cfg = (timer_cfg_t *)cfg;
        if (p_cfg->cycle_end_irq == FSP_INVALID_VECTOR) {
            p_cfg->cycle_end_ipl = AGT_PRIORITY;
            p_cfg->cycle_end_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)agt_int_isr;
            set_agt_link_event(last_interrupt_index, p_cfg->channel);
            last_interrupt_index++;
        }
    }
    /* **********************************************************************
                                      UART
       ********************************************************************** */
    else if(p == IRQ_SCI_UART && cfg != NULL) {
        uart_cfg_t *p_cfg = (uart_cfg_t *)cfg;
        
        if (p_cfg->txi_irq == FSP_INVALID_VECTOR) {
            if (last_interrupt_index + UART_INTERRUPT_COUNT > PROG_IRQ_NUM){
                rv = false;
                goto end_config;
            }
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
            set_sci_eri_link_event(last_interrupt_index, p_cfg->channel);
            last_interrupt_index++;
        }
        R_BSP_IrqEnable (p_cfg->txi_irq);
        R_BSP_IrqEnable (p_cfg->tei_irq);
        R_BSP_IrqEnable (p_cfg->rxi_irq);
        R_BSP_IrqEnable (p_cfg->eri_irq);
    }
    /* **********************************************************************
                                      RTC
       ********************************************************************** */
#if RTC_HOWMANY > 0
    else if(p == IRQ_RTC && cfg != NULL) {
        RTCIrqCfg_t *p_cfg = (RTCIrqCfg_t *)cfg;

        /* rtc interrupts are added once a time and not all three together */

        if(p_cfg->req == RTC_ALARM) {
            if (p_cfg->cfg->alarm_irq == FSP_INVALID_VECTOR) {
                p_cfg->cfg->alarm_ipl = RTC_PRIORITY;
                p_cfg->cfg->alarm_irq = (IRQn_Type)last_interrupt_index;
                *(irq_ptr + last_interrupt_index) = (uint32_t)rtc_alarm_periodic_isr;
                R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_RTC_ALARM);
                R_BSP_IrqCfg(p_cfg->cfg->alarm_irq, p_cfg->cfg->alarm_ipl, p_cfg->ctrl);
                last_interrupt_index++;
            }
            R_BSP_IrqEnable (p_cfg->cfg->alarm_irq);

        }
        else if(p_cfg->req == RTC_PERIODIC) {
            if (p_cfg->cfg->periodic_irq == FSP_INVALID_VECTOR) {
                p_cfg->cfg->periodic_ipl = RTC_PRIORITY;
                p_cfg->cfg->periodic_irq = (IRQn_Type)last_interrupt_index;
                *(irq_ptr + last_interrupt_index) = (uint32_t)rtc_alarm_periodic_isr;
                R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_RTC_PERIOD);
                R_BSP_IrqCfg(p_cfg->cfg->periodic_irq, p_cfg->cfg->periodic_ipl, p_cfg->ctrl);
                last_interrupt_index++;
            }
            R_BSP_IrqEnable (p_cfg->cfg->periodic_irq);

        }
        else if(p_cfg->req == RTC_CARRY) {
            if (p_cfg->cfg->carry_irq == FSP_INVALID_VECTOR) {
                p_cfg->cfg->carry_ipl = RTC_PRIORITY;
                p_cfg->cfg->carry_irq = (IRQn_Type)last_interrupt_index;
                *(irq_ptr + last_interrupt_index) = (uint32_t)rtc_carry_isr;
                R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_RTC_CARRY);
                R_BSP_IrqCfg(p_cfg->cfg->carry_irq, p_cfg->cfg->carry_ipl, p_cfg->ctrl);
                last_interrupt_index++;
            }
            R_BSP_IrqEnable (p_cfg->cfg->carry_irq);
        }
        else {
            rv = false;
        }
    }
#endif

#if WIRE_HOWMANY > 0
    /* I2C true NOT SCI */
    else if(p == IRQ_I2C_MASTER && cfg != NULL) {       

        I2CIrqReq_t *p_cfg = (I2CIrqReq_t *)cfg;
        i2c_master_cfg_t *mcfg = (i2c_master_cfg_t *)p_cfg->mcfg;
        i2c_slave_cfg_t *scfg = (i2c_slave_cfg_t *)p_cfg->scfg;
        mcfg->ipl = I2C_MASTER_PRIORITY;
        
        if (mcfg->txi_irq  == FSP_INVALID_VECTOR) {
            if (last_interrupt_index + WIRE_MASTER_INTERRUPT_COUNT > PROG_IRQ_NUM){
                rv = false;
                goto end_config;
            } 
            /* TX interrupt */
            mcfg->txi_irq = (IRQn_Type)last_interrupt_index;
            set_iic_tx_link_event(last_interrupt_index, p_cfg->mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            last_interrupt_index++;

            /* RX interrupt */
            mcfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            set_iic_rx_link_event(last_interrupt_index, p_cfg->mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            last_interrupt_index++;

            /* TX ERROR interrupt */
            mcfg->tei_irq = (IRQn_Type)last_interrupt_index;
            set_iic_tei_link_event(last_interrupt_index, p_cfg->mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            last_interrupt_index++;

            /* RX ERROR interrupt */
            mcfg->eri_irq = (IRQn_Type)last_interrupt_index;
            set_iic_eri_link_event(last_interrupt_index, p_cfg->mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            last_interrupt_index++;
        }
        
        *(irq_ptr + mcfg->txi_irq) = (uint32_t)iic_master_txi_isr;
        *(irq_ptr + mcfg->rxi_irq) = (uint32_t)iic_master_rxi_isr;
        *(irq_ptr + mcfg->tei_irq) = (uint32_t)iic_master_tei_isr;
        *(irq_ptr + mcfg->eri_irq) = (uint32_t)iic_master_eri_isr;

        R_BSP_IrqEnable (mcfg->txi_irq);
        R_BSP_IrqEnable (mcfg->rxi_irq);
        R_BSP_IrqEnable (mcfg->tei_irq);
        R_BSP_IrqEnable (mcfg->eri_irq);
    }
    /* I2C SCI MASTER (only) */
    else if(p == IRQ_SCI_I2C_MASTER && cfg != NULL) {      
        I2CIrqReq_t *p_cfg = (I2CIrqReq_t *)cfg;
        i2c_master_cfg_t *mcfg = (i2c_master_cfg_t *)p_cfg->mcfg;
        mcfg->ipl = I2C_MASTER_PRIORITY;
        if (mcfg->txi_irq == FSP_INVALID_VECTOR) {
            if (last_interrupt_index + WIRE_SCI_MASTER_INTERRUPT_COUNT > PROG_IRQ_NUM) {
                rv = false;
                goto end_config;
            }
            /* TX interrupt */
            mcfg->txi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_txi_isr;
            set_sci_tx_link_event(last_interrupt_index, p_cfg->mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            last_interrupt_index++;

            /* RX interrupt */
            mcfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_rxi_isr;
            set_sci_rx_link_event(last_interrupt_index, p_cfg->mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            last_interrupt_index++;

            /* TX ERROR interrupt */
            mcfg->tei_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_tei_isr;
            set_sci_tei_link_event(last_interrupt_index, p_cfg->mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            last_interrupt_index++;

            /* RX ERROR interrupt */
            #if 0
            mcfg->eri_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_i2c_eri_isr;
            set_sci_eri_link_event(last_interrupt_index, p_cfg->mcfg->channel);
            R_BSP_IrqCfg((IRQn_Type)last_interrupt_index, I2C_MASTER_PRIORITY, mcfg);
            last_interrupt_index++;
            #endif
        }
        R_BSP_IrqEnable (mcfg->txi_irq);
        R_BSP_IrqEnable (mcfg->rxi_irq);
        R_BSP_IrqEnable (mcfg->tei_irq);
        #if 0
        R_BSP_IrqEnable (mcfg->eri_irq);
        #endif
    }
    else if(p == IRQ_I2C_SLAVE && cfg != NULL) {
        I2CIrqReq_t *p_cfg = (I2CIrqReq_t *)cfg;
        i2c_master_cfg_t *mcfg = (i2c_master_cfg_t *)p_cfg->mcfg;
        i2c_slave_cfg_t *scfg = (i2c_slave_cfg_t *)p_cfg->scfg;
        scfg->ipl = I2C_SLAVE_PRIORITY;
        scfg->eri_ipl = I2C_SLAVE_PRIORITY;

        if (scfg->txi_irq == FSP_INVALID_VECTOR) {
            if (last_interrupt_index + WIRE_SLAVE_INTERRUPT_COUNT > PROG_IRQ_NUM) {
                rv = false;
                goto end_config;
            }
            /* TX interrupt */
            scfg->txi_irq = (IRQn_Type)last_interrupt_index;
            set_iic_tx_link_event(last_interrupt_index, scfg->channel);
            last_interrupt_index++;

            /* RX interrupt */
            scfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            set_iic_rx_link_event(last_interrupt_index, scfg->channel);
            last_interrupt_index++;

            /* TEI interrupt */
            scfg->tei_irq = (IRQn_Type)last_interrupt_index;
            set_iic_tei_link_event(last_interrupt_index, scfg->channel);
            last_interrupt_index++;

            /* ERI interrupt */
            scfg->eri_irq = (IRQn_Type)last_interrupt_index;
            set_iic_eri_link_event(last_interrupt_index, scfg->channel);
            last_interrupt_index++;
        }
        *(irq_ptr + scfg->txi_irq) = (uint32_t)iic_slave_txi_isr;
        *(irq_ptr + scfg->rxi_irq) = (uint32_t)iic_slave_rxi_isr;
        *(irq_ptr + scfg->tei_irq) = (uint32_t)iic_slave_tei_isr;
        *(irq_ptr + scfg->eri_irq) = (uint32_t)iic_slave_eri_isr;
        R_BSP_IrqEnable (scfg->txi_irq);
        R_BSP_IrqEnable (scfg->rxi_irq);
        R_BSP_IrqEnable (scfg->tei_irq);
        R_BSP_IrqEnable (scfg->eri_irq);

    }
#endif

#if EXT_INTERRUPTS_HOWMANY > 0
    /* **********************************************************************
                             PIN EXTERNAL INTERRUPT
       ********************************************************************** */
    else if(p == IRQ_EXTERNAL_PIN && cfg != NULL) {
        external_irq_cfg_t *p_cfg = (external_irq_cfg_t *)cfg;
        if (p_cfg->irq == FSP_INVALID_VECTOR) {
            p_cfg->ipl = EXTERNAL_PIN_PRIORITY;
            p_cfg->irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)r_icu_isr;
            set_ext_link_event(last_interrupt_index, p_cfg->channel);
            last_interrupt_index++;
        }
    }
#endif

#if SPI_HOWMANY > 0
    /* **********************************************************************
                                      SPI MASTER
       ********************************************************************** */
    else if(p == IRQ_SPI_MASTER && cfg != NULL) {      
        spi_instance_ctrl_t * p_ctrl = reinterpret_cast<SpiMasterIrqReq_t *>(cfg)->ctrl;
        spi_cfg_t  * p_cfg  = reinterpret_cast<SpiMasterIrqReq_t *>(cfg)->cfg;
        uint8_t const hw_channel = reinterpret_cast<SpiMasterIrqReq_t *>(cfg)->hw_channel;

        if (p_cfg->txi_irq == FSP_INVALID_VECTOR) {
            if (last_interrupt_index + SPI_INTERRUPT_COUNT > PROG_IRQ_NUM) {
                rv = false;
                goto end_config;
            } 
            /* TX interrupt */
            p_cfg->txi_irq = (IRQn_Type)last_interrupt_index;
            p_cfg->txi_ipl = SPI_MASTER_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)spi_txi_isr;
            set_spi_tx_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg(p_cfg->txi_irq, p_cfg->txi_ipl, p_ctrl);
            last_interrupt_index++;

            /* RX interrupt */
            p_cfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            p_cfg->rxi_ipl = SPI_MASTER_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)spi_rxi_isr;
            set_spi_rx_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg(p_cfg->rxi_irq, p_cfg->rxi_ipl, p_ctrl);
            last_interrupt_index++;

            /* Error interrupt */
            p_cfg->eri_irq = (IRQn_Type)last_interrupt_index;
            p_cfg->eri_ipl = SPI_MASTER_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)spi_eri_isr;
            set_spi_eri_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg(p_cfg->eri_irq, p_cfg->eri_ipl, p_ctrl);
            last_interrupt_index++;

            /* TX register empty interrupt */
            p_cfg->tei_irq = (IRQn_Type)last_interrupt_index;
            p_cfg->tei_ipl = SPI_MASTER_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)spi_tei_isr;
            set_spi_tei_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg(p_cfg->tei_irq, p_cfg->tei_ipl, p_ctrl);
            last_interrupt_index++;
        }
        R_BSP_IrqEnable (p_cfg->txi_irq);
        R_BSP_IrqEnable (p_cfg->rxi_irq);
        R_BSP_IrqEnable (p_cfg->eri_irq);
        /* Note tei_irq is not enabled until the last data frame is transfered. */
        // R_BSP_IrqEnable (p_cfg->tei_irq);
    }

    /* **********************************************************************
                                    SCI SPI MASTER
       ********************************************************************** */
    else if(p == IRQ_SCI_SPI_MASTER && cfg != NULL) {  
        sci_spi_instance_ctrl_t * p_ctrl = reinterpret_cast<SciSpiMasterIrqReq_t *>(cfg)->ctrl;
        spi_cfg_t  * p_cfg  = reinterpret_cast<SciSpiMasterIrqReq_t *>(cfg)->cfg;
        uint8_t const hw_channel = reinterpret_cast<SciSpiMasterIrqReq_t *>(cfg)->hw_channel;

        if (p_cfg->txi_irq == FSP_INVALID_VECTOR) {
            if (last_interrupt_index + SPI_INTERRUPT_COUNT > PROG_IRQ_NUM) {
                rv = false;
                goto end_config;
            } 
            /* TX interrupt */
            p_cfg->txi_irq = (IRQn_Type)last_interrupt_index;
            p_cfg->txi_ipl = SPI_MASTER_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_spi_txi_isr;
            set_sci_tx_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg(p_cfg->txi_irq, p_cfg->txi_ipl, p_ctrl);
            last_interrupt_index++;

            /* RX interrupt */
            p_cfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            p_cfg->rxi_ipl = SPI_MASTER_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_spi_rxi_isr;
            set_sci_rx_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg(p_cfg->rxi_irq, p_cfg->rxi_ipl, p_ctrl);
            last_interrupt_index++;

            /* Error interrupt */
            p_cfg->eri_irq = (IRQn_Type)last_interrupt_index;
            p_cfg->eri_ipl = SPI_MASTER_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_spi_eri_isr;
            set_sci_eri_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg(p_cfg->eri_irq, p_cfg->eri_ipl, p_ctrl);
            last_interrupt_index++;

            /* TX register empty interrupt */
            p_cfg->tei_irq = (IRQn_Type)last_interrupt_index;
            p_cfg->tei_ipl = SPI_MASTER_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sci_spi_tei_isr;
            set_sci_tei_link_event(last_interrupt_index, hw_channel);
            R_BSP_IrqCfg(p_cfg->tei_irq, p_cfg->tei_ipl, p_ctrl);
            last_interrupt_index++;
        }
        R_BSP_IrqEnable (p_cfg->txi_irq);
        R_BSP_IrqEnable (p_cfg->rxi_irq);
        R_BSP_IrqEnable (p_cfg->eri_irq);
        R_BSP_IrqEnable (p_cfg->tei_irq);
    }
#endif

#if CAN_HOWMANY > 0
    /* **********************************************************************
                                      CAN
       ********************************************************************** */
    else if(p == IRQ_CAN && cfg != NULL) {  
        can_instance_ctrl_t * p_ctrl = reinterpret_cast<CanIrqReq_t *>(cfg)->ctrl;
        can_cfg_t * p_cfg  = reinterpret_cast<CanIrqReq_t *>(cfg)->cfg;
        p_cfg->ipl = CAN_PRIORITY; /* All interrupts share the same priority. */

        if (p_cfg->error_irq == FSP_INVALID_VECTOR) {
            if (last_interrupt_index + CAN_INTERRUPT_COUNT > PROG_IRQ_NUM) {
                rv = false;
                goto end_config;
            } 
            /* Error interrupt */
            p_cfg->error_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)can_error_isr;
            set_can_error_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqCfg(p_cfg->error_irq, p_cfg->ipl, p_ctrl);
            last_interrupt_index++;

            /* Receive interrupt */
            p_cfg->rx_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)can_rx_isr;
            set_can_rx_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqCfg(p_cfg->rx_irq, p_cfg->ipl, p_ctrl);
            last_interrupt_index++;

            /* Transmit interrupt */
            p_cfg->tx_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)can_tx_isr;
            set_can_tx_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqCfg(p_cfg->tx_irq, p_cfg->ipl, p_ctrl);
            last_interrupt_index++;
        }
        R_BSP_IrqEnable (p_cfg->error_irq);
        R_BSP_IrqEnable (p_cfg->rx_irq);
        R_BSP_IrqEnable (p_cfg->tx_irq);
    }
#endif /* CAN_HOWMANY > 0 */


#if ETHERNET_HOWMANY > 0
    /* **********************************************************************
                                    ETHERNET
       ********************************************************************** */
    else if(p == IRQ_ETHERNET && cfg != NULL) {
        ether_cfg_t *eth = (ether_cfg_t *)cfg;
        if (eth->irq == FSP_INVALID_VECTOR) {
            eth->irq = (IRQn_Type)last_interrupt_index;
            eth->interrupt_priority = ETHERNET_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)ether_eint_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_EDMAC0_EINT);
            last_interrupt_index++;
        }
    }
#endif

#if CANFD_HOWMANY > 0
    /* **********************************************************************
                                      CANFD
       ********************************************************************** */
    else if(p == IRQ_CANFD && cfg != NULL) {
        canfd_instance_ctrl_t * p_ctrl = reinterpret_cast<CanFdIrqReq_t *>(cfg)->ctrl;
        can_cfg_t * p_cfg  = reinterpret_cast<CanFdIrqReq_t *>(cfg)->cfg;
        p_cfg->ipl = CAN_PRIORITY; /* All interrupts share the same priority. */

        if (p_cfg->error_irq == FSP_INVALID_VECTOR) {
            if (last_interrupt_index + CANFD_INTERRUPT_COUNT > PROG_IRQ_NUM) {
                rv = false;
                goto end_config;
            }   
            /* Error interrupt */
            p_cfg->error_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)canfd_error_isr;
            set_canfd_error_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqCfg(p_cfg->error_irq, p_cfg->ipl, p_ctrl);
            last_interrupt_index++;

            /* Receive interrupt */
            p_cfg->rx_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)canfd_rx_fifo_isr;
            set_canfd_rx_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqCfg(p_cfg->rx_irq, p_cfg->ipl, p_ctrl);
            last_interrupt_index++;

            /* Transmit interrupt */
            p_cfg->tx_irq = (IRQn_Type)last_interrupt_index;
            *(irq_ptr + last_interrupt_index) = (uint32_t)canfd_channel_tx_isr;
            set_canfd_tx_link_event(last_interrupt_index, p_cfg->channel);
            R_BSP_IrqCfg(p_cfg->tx_irq, p_cfg->ipl, p_ctrl);
            last_interrupt_index++;
        }
        R_BSP_IrqEnable (p_cfg->error_irq);
        R_BSP_IrqEnable (p_cfg->rx_irq);
        R_BSP_IrqEnable (p_cfg->tx_irq);
    }
#endif /* CANFD_HOWMANY > 0 */

#if SDCARD_HOWMANY > 0
    /* **********************************************************************
                                    SDCARD
       ********************************************************************** */
    else if(p == IRQ_SDCARD && cfg != NULL) {
        sdmmc_cfg_t *sd_cfg = (sdmmc_cfg_t *)cfg;
        /* SDCARD_ACCESS */
        if(sd_cfg->access_irq == FSP_INVALID_VECTOR) {
            if (last_interrupt_index + SD_INTERRUPT_COUNT > PROG_IRQ_NUM){
                rv = false;
                goto end_config;
            }   
            sd_cfg->access_irq = (IRQn_Type)last_interrupt_index;
            sd_cfg->access_ipl = SDCARD_ACCESS_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sdhimmc_accs_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SDHIMMC0_ACCS);
            last_interrupt_index++;
        }
        /*
        this interrupt is neeed if DTC is used but it must not be used if
        DMA is used
        -----------
        if(sd_cfg->dma_req_irq == FSP_INVALID_VECTOR) {
            sd_cfg->dma_req_irq = (IRQn_Type)last_interrupt_index;
            sd_cfg->dma_req_ipl = SDCARD_DMA_REQ_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sdhimmc_dma_req_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SDHIMMC0_DMA_REQ);
            last_interrupt_index++;
        }
        */
        if(sd_cfg->card_irq == FSP_INVALID_VECTOR) {
            sd_cfg->card_irq = (IRQn_Type)last_interrupt_index;
            sd_cfg->card_ipl = SDCARD_CARD_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)sdhimmc_card_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SDHIMMC0_CARD);
            last_interrupt_index++;
        }
    }
#endif

#if I2S_HOWMANY > 0
    /* **********************************************************************
                                    I2S
       ********************************************************************** */
    else if(p == IRQ_I2S && cfg != NULL) {
        i2s_cfg_t *i2s_cfg = (i2s_cfg_t *)cfg;

        if(i2s_cfg->txi_irq == FSP_INVALID_VECTOR) {
            i2s_cfg->txi_irq = (IRQn_Type)last_interrupt_index;
            i2s_cfg->txi_ipl = I2S_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)ssi_txi_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SSI0_TXI);
            R_FSP_IsrContextSet(i2s_cfg->txi_irq, (void*)i2s_cfg->p_context);
            last_interrupt_index++;
        }
        if(i2s_cfg->rxi_irq == FSP_INVALID_VECTOR) {
            i2s_cfg->rxi_irq = (IRQn_Type)last_interrupt_index;
            i2s_cfg->rxi_ipl = I2S_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)ssi_rxi_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SSI0_RXI);
            R_FSP_IsrContextSet(i2s_cfg->rxi_irq, (void*)i2s_cfg->p_context);
            last_interrupt_index++;
        }
        if(i2s_cfg->int_irq == FSP_INVALID_VECTOR) {
            i2s_cfg->int_irq = (IRQn_Type)last_interrupt_index;
            i2s_cfg->idle_err_ipl = I2S_PRIORITY;
            *(irq_ptr + last_interrupt_index) = (uint32_t)ssi_int_isr;
            R_ICU->IELSR[last_interrupt_index] = BSP_PRV_IELS_ENUM(EVENT_SSI0_INT);
            R_FSP_IsrContextSet(i2s_cfg->int_irq, (void*)i2s_cfg->p_context);
            last_interrupt_index++;
        }
    }
#endif

    else {
        rv = false;
    }

end_config:
    __enable_irq();
    return rv;
}

bool IRQManager::set_adc_end_link_event(int li, int ch){
    bool rv = false;
    if (0) {}
#ifdef ELC_EVENT_ADC0_SCAN_END
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ADC0_SCAN_END);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_ADC1_SCAN_END1
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ADC1_SCAN_END);
        rv = true;
    }
#endif
    return rv;
}

bool IRQManager::set_adc_end_b_link_event(int li, int ch) {
    bool rv = false;
    if (0) {}
#ifdef ELC_EVENT_ADC0_SCAN_END_B
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ADC0_SCAN_END_B);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_ADC1_SCAN_END_B
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ADC1_SCAN_END_B);
        rv = true;
    }
#endif
    return rv;    

}

bool IRQManager::set_adc_win_a_link_event(int li, int ch) {
    bool rv = false;
    if (0) {}
#ifdef ELC_EVENT_ADC0_WINDOW_A
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ADC0_WINDOW_A);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_ADC1_WINDOW_A
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ADC1_WINDOW_A);
        rv = true;
    }
#endif
    return rv; 

}

bool IRQManager::set_adc_win_b_link_event(int li, int ch) {
      bool rv = false;
    if (0) {}
#ifdef ELC_EVENT_ADC0_WINDOW_B
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ADC0_WINDOW_B);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_ADC1_WINDOW_B
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ADC1_WINDOW_B);
        rv = true;
    }
#endif
    return rv; 

}

void IRQManager::set_ext_link_event(int li, int ch) {
    if (0) {}
#ifdef ELC_EVENT_ICU_IRQ0
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ0);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ1
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ1);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ2
    else if(ch == 2) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ2);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ3
    else if(ch == 3) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ3);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ4
    else if(ch == 4) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ4);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ5
    else if(ch == 5) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ5);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ6
    else if(ch == 6) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ6);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ7
    else if(ch == 7) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ7);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ8
    else if(ch == 8) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ8);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ9
    else if(ch == 9) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ9);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ10
    else if(ch == 10) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ10);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ11
    else if(ch == 11) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ11);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ12
    else if(ch == 12) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ12);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ13
    else if(ch == 13) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ13);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ14
    else if(ch == 14) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ14);
    }
#endif
#ifdef ELC_EVENT_ICU_IRQ15
    else if(ch == 15) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ15);
    }
#endif
}




bool IRQManager::set_gpt_under_link_event(int li, int ch) {
    bool rv = false; 
    if (0) {}
#ifdef ELC_EVENT_GPT0_COUNTER_UNDERFLOW 
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT0_COUNTER_UNDERFLOW);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_GPT1_COUNTER_UNDERFLOW
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT1_COUNTER_UNDERFLOW);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_GPT2_COUNTER_UNDERFLOW
    else if(ch == 2) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT2_COUNTER_UNDERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT3_COUNTER_UNDERFLOW
    else if(ch == 3) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT3_COUNTER_UNDERFLOW);
        rv = true;
    }
#endif    
#ifdef ELC_EVENT_GPT4_COUNTER_UNDERFLOW
    else if(ch == 4) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT4_COUNTER_UNDERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT5_COUNTER_UNDERFLOW
    else if(ch == 5) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT5_COUNTER_UNDERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT6_COUNTER_UNDERFLOW
    else if(ch == 6) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT6_COUNTER_UNDERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT7_COUNTER_UNDERFLOW
    else if(ch == 7) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT7_COUNTER_UNDERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT8_COUNTER_UNDERFLOW
    else if(ch == 8) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT8_COUNTER_UNDERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT9_COUNTER_UNDERFLOW
    else if(ch == 9) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT9_COUNTER_UNDERFLOW);
        rv = true;
    }
#endif
    return rv;     
}

bool IRQManager::set_gpt_compare_capture_A_link_event(int li, int ch) {
    bool rv = false; 
    if (0) {}
#ifdef ELC_EVENT_GPT0_CAPTURE_COMPARE_A 
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT0_CAPTURE_COMPARE_A);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_GPT1_CAPTURE_COMPARE_A
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT1_CAPTURE_COMPARE_A);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_GPT2_CAPTURE_COMPARE_A
    else if(ch == 2) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT2_CAPTURE_COMPARE_A);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT3_CAPTURE_COMPARE_A
    else if(ch == 3) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT3_CAPTURE_COMPARE_A);
        rv = true;
    }
#endif    
#ifdef ELC_EVENT_GPT4_CAPTURE_COMPARE_A
    else if(ch == 4) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT4_CAPTURE_COMPARE_A);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT5_CAPTURE_COMPARE_A
    else if(ch == 5) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT5_CAPTURE_COMPARE_A);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT6_CAPTURE_COMPARE_A
    else if(ch == 6) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT6_CAPTURE_COMPARE_A);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT7_CAPTURE_COMPARE_A
    else if(ch == 7) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT7_CAPTURE_COMPARE_A);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT8_CAPTURE_COMPARE_A
    else if(ch == 8) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT8_CAPTURE_COMPARE_A);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT9_CAPTURE_COMPARE_A
    else if(ch == 9) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT9_CAPTURE_COMPARE_A);
        rv = true;
    }
#endif
    return rv;     
}

bool IRQManager::set_gpt_compare_capture_B_link_event(int li, int ch) {
    bool rv = false; 
    if (0) {}
#ifdef ELC_EVENT_GPT0_CAPTURE_COMPARE_B 
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT0_CAPTURE_COMPARE_B);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_GPT1_CAPTURE_COMPARE_B
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT1_CAPTURE_COMPARE_B);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_GPT2_CAPTURE_COMPARE_B
    else if(ch == 2) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT2_CAPTURE_COMPARE_B);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT3_CAPTURE_COMPARE_B
    else if(ch == 3) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT3_CAPTURE_COMPARE_B);
        rv = true;
    }
#endif    
#ifdef ELC_EVENT_GPT4_CAPTURE_COMPARE_B
    else if(ch == 4) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT4_CAPTURE_COMPARE_B);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT5_CAPTURE_COMPARE_B
    else if(ch == 5) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT5_CAPTURE_COMPARE_B);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT6_CAPTURE_COMPARE_B
    else if(ch == 6) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT6_CAPTURE_COMPARE_B);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT7_CAPTURE_COMPARE_B
    else if(ch == 7) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT7_CAPTURE_COMPARE_B);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT8_CAPTURE_COMPARE_B
    else if(ch == 8) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT8_CAPTURE_COMPARE_B);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT9_CAPTURE_COMPARE_B
    else if(ch == 9) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT9_CAPTURE_COMPARE_B);
        rv = true;
    }
#endif
    return rv;     
}

bool IRQManager::set_gpt_over_link_event(int li, int ch) {
    bool rv = false; 
    if (0) {}
#ifdef ELC_EVENT_GPT0_COUNTER_OVERFLOW 
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT0_COUNTER_OVERFLOW);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_GPT1_COUNTER_OVERFLOW
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT1_COUNTER_OVERFLOW);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_GPT2_COUNTER_OVERFLOW
    else if(ch == 2) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT2_COUNTER_OVERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT3_COUNTER_OVERFLOW
    else if(ch == 3) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT3_COUNTER_OVERFLOW);
        rv = true;
    }
#endif    
#ifdef ELC_EVENT_GPT4_COUNTER_OVERFLOW
    else if(ch == 4) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT4_COUNTER_OVERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT5_COUNTER_OVERFLOW
    else if(ch == 5) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT5_COUNTER_OVERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT6_COUNTER_OVERFLOW
    else if(ch == 6) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT6_COUNTER_OVERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT7_COUNTER_OVERFLOW
    else if(ch == 7) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT7_COUNTER_OVERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT8_COUNTER_OVERFLOW
    else if(ch == 8) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT8_COUNTER_OVERFLOW);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_GPT9_COUNTER_OVERFLOW
    else if(ch == 9) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_GPT9_COUNTER_OVERFLOW);
        rv = true;
    }
#endif
    return rv;     
}


bool IRQManager::set_agt_link_event(int li, int ch) {
    bool rv = false;
    if (0) {}
#ifdef ELC_EVENT_AGT0_INT
    else if(ch == 0) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT0_INT);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_AGT1_INT
    else if(ch == 1) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT1_INT);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_AGT2_INT
    else if(ch == 2) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT2_INT);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_AGT3_INT
    else if(ch == 3) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT3_INT);
        rv = true;
    }
#endif    
#ifdef ELC_EVENT_AGT4_INT
    else if(ch == 4) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT4_INT);
        rv = true;
    }
#endif  
#ifdef ELC_EVENT_AGT5_INT
    else if(ch == 5) {
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_AGT5_INT);
        rv = true;
    }
#endif  
    return rv;
}

void IRQManager::set_iic_tx_link_event(int li, int ch) {
    if (0) {}
#ifdef ELC_EVENT_IIC0_TXI
    else if(ch == 0) {  R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_IIC0_TXI);}
#endif
#ifdef ELC_EVENT_IIC1_TXI
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
    
void IRQManager::set_sci_eri_link_event(int li, int ch){
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

void IRQManager::set_spi_tx_link_event(int li, int ch)
{
    if (0) {}
#ifdef ELC_EVENT_SPI0_TXI
    else if(ch == 0) { R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SPI0_TXI);}
#endif
#ifdef ELC_EVENT_SPI1_TXI
    else if(ch == 1) { R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SPI1_TXI);}
#endif
}

void IRQManager::set_spi_rx_link_event(int li, int ch)
{
    if (0) {}
#ifdef ELC_EVENT_SPI0_RXI
    else if(ch == 0) { R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SPI0_RXI);}
#endif
#ifdef ELC_EVENT_SPI1_RXI
    else if(ch == 1) { R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SPI1_RXI);}
#endif
}

void IRQManager::set_spi_tei_link_event(int li, int ch)
{
    if (0) {}
#ifdef ELC_EVENT_SPI0_TEI
    else if(ch == 0) { R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SPI0_TEI);}
#endif
#ifdef ELC_EVENT_SPI1_TEI
    else if(ch == 1) { R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SPI1_TEI);}
#endif
}

void IRQManager::set_spi_eri_link_event(int li, int ch)
{
    if (0) {}
#ifdef ELC_EVENT_SPI0_ERI
    else if(ch == 0) { R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SPI0_ERI);}
#endif
#ifdef ELC_EVENT_SPI1_ERI
    else if(ch == 1) { R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_SPI1_ERI);}
#endif
}


void IRQManager::set_can_error_link_event(int li, int ch)
{
  if (0) {}
#ifdef ELC_EVENT_CAN0_ERROR
  else if(ch == 0) {
    R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_CAN0_ERROR);
  }
#endif
}

void IRQManager::set_can_rx_link_event(int li, int ch)
{
  if (0) {}
#ifdef ELC_EVENT_CAN0_MAILBOX_RX
  else if(ch == 0) {
    R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_CAN0_MAILBOX_RX);
  }
#endif
}

void IRQManager::set_can_tx_link_event(int li, int ch)
{
  if (0) {}
#ifdef ELC_EVENT_CAN0_MAILBOX_TX
  else if(ch == 0) {
    R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_CAN0_MAILBOX_TX);
  }
#endif
}

void IRQManager::set_canfd_error_link_event(int li, int ch)
{
  if (0) {}
#ifdef ELC_EVENT_CAN0_CHERR
  else if(ch == 0) {
    R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_CAN0_CHERR);
  }
#endif
#ifdef ELC_EVENT_CAN1_CHERR
  else if(ch == 1) {
    R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_CAN1_CHERR);
  }
#endif
}

void IRQManager::set_canfd_rx_link_event(int li, int ch)
{
  if (0) {}
#ifdef ELC_EVENT_CAN0_COMFRX
  else if(ch == 0) {
    R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_CAN0_COMFRX);
  }
#endif
#ifdef ELC_EVENT_CAN1_COMFRX
  else if(ch == 1) {
    R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_CAN1_COMFRX);
  }
#endif
}

void IRQManager::set_canfd_tx_link_event(int li, int ch)
{
  if (0) {}
#ifdef ELC_EVENT_CAN0_TX
  else if(ch == 0) {
    R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_CAN0_TX);
  }
#endif
#ifdef ELC_EVENT_CAN1_TX
  else if(ch == 1) {
    R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_CAN1_TX);
  }
#endif
}

bool IRQManager::set_dma_link_event(int li, int ch) {
    bool rv = false;
    if (0) {}
#ifdef ELC_EVENT_DMAC0_INT   
    else if(ch == 0) { 
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_DMAC0_INT);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_DMAC1_INT   
    else if(ch == 1) { 
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_DMAC1_INT);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_DMAC2_INT   
    else if(ch == 2) { 
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_DMAC2_INT);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_DMAC3_INT   
    else if(ch == 3) { 
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_DMAC3_INT);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_DMAC4_INT   
    else if(ch == 4) { 
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_DMAC4_INT);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_DMAC5_INT   
    else if(ch == 5) { 
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_DMAC5_INT);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_DMAC6_INT   
    else if(ch == 6) { 
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_DMAC6_INT);
        rv = true;
    }
#endif
#ifdef ELC_EVENT_DMAC7_INT   
    else if(ch == 7) { 
        R_ICU->IELSR[li] = BSP_PRV_IELS_ENUM(EVENT_DMAC7_INT);
        rv = true;
    }
#endif
    return rv;
}
