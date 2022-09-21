#include "FspTimer.h"

#define TIMER_16_BIT_OFFSET 2

FspTimer::FspTimer(): init_ok(false) {
    /* extended cfg */
    timer_extended_cfg.gtioca.output_enabled        = false;
    timer_extended_cfg.gtioca.stop_level            = GPT_PIN_LEVEL_LOW;
    timer_extended_cfg.gtiocb.output_enabled        = false;
    timer_extended_cfg.gtiocb.stop_level            = GPT_PIN_LEVEL_LOW;
    timer_extended_cfg.start_source                 = (gpt_source_t) (GPT_SOURCE_NONE); 
    timer_extended_cfg.stop_source                  = (gpt_source_t) (GPT_SOURCE_NONE);
    timer_extended_cfg.clear_source                 = (gpt_source_t) (GPT_SOURCE_NONE);
    timer_extended_cfg.count_up_source              = (gpt_source_t) (GPT_SOURCE_NONE); 
    timer_extended_cfg.count_down_source            = (gpt_source_t) (GPT_SOURCE_NONE); 
    timer_extended_cfg.capture_a_source             = (gpt_source_t) (GPT_SOURCE_NONE);
    timer_extended_cfg.capture_b_source             = (gpt_source_t) (GPT_SOURCE_NONE); 
    timer_extended_cfg.capture_a_ipl                = (BSP_IRQ_DISABLED);
    timer_extended_cfg.capture_b_ipl                = (BSP_IRQ_DISABLED);
    timer_extended_cfg.capture_a_irq                = FSP_INVALID_VECTOR;
    timer_extended_cfg.capture_b_irq                = FSP_INVALID_VECTOR;
    timer_extended_cfg.capture_filter_gtioca        = GPT_CAPTURE_FILTER_NONE;
    timer_extended_cfg.capture_filter_gtiocb        = GPT_CAPTURE_FILTER_NONE;
    timer_extended_cfg.p_pwm_cfg                    = NULL;
    timer_extended_cfg.gtior_setting.gtior          = 0U;
}

/* begin PWM with standard values...*/
/* -------------------------------------------------------------------------- */
bool FspTimer::begin_pwm(uint8_t type, uint8_t channel, gpt_extended_pwm_cfg_t *pwm_cfg, TimerPWMChannel_t pwm_channel) {
/* -------------------------------------------------------------------------- */    
    init_ok = true;
    /* at the present the type GPT / AGT is not used ---- !!!!!! TODO !!!!!! */
    timer_cfg.mode = TIMER_MODE_PWM;

    if(channel > 7)       { init_ok &= false; }
    else                  { timer_cfg.channel = channel; }

    if(init_ok) {
        if(timer_cfg.channel < TIMER_16_BIT_OFFSET) {
            /* timer a 32 BIT */
            timer_cfg.source_div = (timer_source_div_t) 0;
            timer_cfg.period_counts = (uint32_t) 0x17ea7;
            timer_cfg.duty_cycle_counts = 0xbf53;
        }
        else {
            /* timer a 16 BIT */
            timer_cfg.source_div = (timer_source_div_t) 2;
            timer_cfg.period_counts = (uint32_t) 0x5fa9;
            timer_cfg.duty_cycle_counts = 0x2fd4;
        }  
    }
    
    timer_extended_cfg.p_pwm_cfg                    = pwm_cfg;

    timer_cfg.p_callback                            = NULL;
    timer_cfg.p_context                             = NULL;
    timer_cfg.p_extend                              = &timer_extended_cfg;
    timer_cfg.cycle_end_ipl                         = (BSP_IRQ_DISABLED);
    timer_cfg.cycle_end_irq                         = FSP_INVALID_VECTOR;

    if(pwm_channel == CHANNEL_A) {
      timer_extended_cfg.gtioca.output_enabled      = true;
    } 
    else {
      timer_extended_cfg.gtiocb.output_enabled      = true;
    }

    if(init_ok) {
        fsp_err_t err = R_GPT_Open(&timer_ctrl,&timer_cfg);
        if ((err != FSP_ERR_ALREADY_OPEN) && (err != FSP_SUCCESS)) {
            return false;
        }
    }
  
    if (R_GPT_Start(&timer_ctrl) != FSP_SUCCESS) {
        return false;
    }



    return init_ok;

}



/* begin function RAW mode */
/* -------------------------------------------------------------------------- */
bool FspTimer::begin(timer_mode_t mode, uint8_t type, uint8_t channel, uint32_t period_counts, uint32_t pulse_counts, timer_source_div_t sd ) {
/* -------------------------------------------------------------------------- */    
    init_ok = true;
    /* at the present the type GPT / AGT is not used ---- !!!!!! TODO !!!!!! */
    timer_cfg.mode                                  = mode;

    if(channel > 7)       { init_ok &= false; }
    else                  { timer_cfg.channel = channel; }

    if(init_ok) {
        timer_cfg.source_div = sd;
        timer_cfg.period_counts = period_counts;
        timer_cfg.duty_cycle_counts = pulse_counts;
    }

    timer_cfg.p_callback                            = NULL;
    timer_cfg.p_context                             = NULL;
    timer_cfg.p_extend                              = &timer_extended_cfg;
    timer_cfg.cycle_end_ipl                         = (BSP_IRQ_DISABLED);
    timer_cfg.cycle_end_irq                         = FSP_INVALID_VECTOR;

    return init_ok;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::begin(timer_mode_t mode, uint8_t type, uint8_t channel, float freq_hz, float duty_perc) {
/* -------------------------------------------------------------------------- */    
    init_ok = true;
    float period = 1.0 / freq_hz;
    
    /* at the present the type GPT / AGT is not used ---- !!!!!! TODO !!!!!! */
    
    /* timer cfg */
    timer_cfg.mode                                  = mode;

    if(channel > 7)       { init_ok &= false; }
    else                  { timer_cfg.channel = channel; }

    if(init_ok) {
        if(timer_cfg.channel < TIMER_16_BIT_OFFSET) {
            /* timer a 32 BIT */
            set_period_counts(period, CH32BIT_MAX);
        }
        else {
            /* timer a 16 BIT */
            set_period_counts(period, CH16BIT_MAX);
        }
    }

    if(init_ok) {
        if(duty_perc >= 0 && duty_perc <= 100) {
            timer_cfg.duty_cycle_counts = (uint32_t)(((float)timer_cfg.period_counts *  duty_perc) / 100.0);
        }
        else {
            init_ok = false;
        }
    }
    
    timer_cfg.p_callback                            = NULL;
    timer_cfg.p_context                             = NULL;
    timer_cfg.p_extend                              = &timer_extended_cfg;
    timer_cfg.cycle_end_ipl                         = (BSP_IRQ_DISABLED);
    timer_cfg.cycle_end_irq                         = FSP_INVALID_VECTOR;

    return init_ok;

}

/* -------------------------------------------------------------------------- */
void FspTimer::set_period_counts(float period, uint32_t _max) {
/* -------------------------------------------------------------------------- */    
    if(period / TICK_0 < _max) {
        timer_cfg.period_counts = (uint32_t) (period / TICK_0);
        timer_cfg.source_div = TIMER_SOURCE_DIV_1;
    }
    else if(period / TICK_1 < _max) {
        timer_cfg.period_counts = (uint32_t) (period / TICK_1);
        timer_cfg.source_div = TIMER_SOURCE_DIV_4;
    }
    else if(period / TICK_2 < _max) {
        timer_cfg.period_counts = (uint32_t) (period / TICK_2);
        timer_cfg.source_div = TIMER_SOURCE_DIV_16;
    }
    else if(period / TICK_3 < _max) {
        timer_cfg.period_counts = (uint32_t) (period / TICK_3);
        timer_cfg.source_div = TIMER_SOURCE_DIV_64;
    }
    else if(period / TICK_4 < _max) {
        timer_cfg.period_counts = (uint32_t) (period / TICK_4);
        timer_cfg.source_div = TIMER_SOURCE_DIV_256;
    }
    else if(period / TICK_5 < _max) {
        timer_cfg.period_counts = (uint32_t) (period / TICK_5);
        timer_cfg.source_div = TIMER_SOURCE_DIV_1024;
    }
    else {
        init_ok = false;
    }
}

/* -------------------------------------------------------------------------- */
void FspTimer::enable_gtioc_CH_A() {
/* -------------------------------------------------------------------------- */    
    timer_extended_cfg.gtioca.output_enabled = true;
}

/* -------------------------------------------------------------------------- */
void FspTimer::enable_gtioc_CH_B() {
/* -------------------------------------------------------------------------- */    
    timer_extended_cfg.gtiocb.output_enabled = true;
}

/* -------------------------------------------------------------------------- */
void FspTimer::set_pwm_extended_cfg(gpt_extended_pwm_cfg_t *cfg) {
/* -------------------------------------------------------------------------- */
    timer_extended_cfg.p_pwm_cfg = cfg;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_pulse_ms(double ms,uint32_t pin) {
/* -------------------------------------------------------------------------- */ 
    uint32_t pclkd_freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKD) >> timer_cfg.source_div;
    uint32_t pulse_counts_ms = (uint32_t) ((uint64_t) pclkd_freq_hz/1000 * ms);
    if (R_GPT_DutyCycleSet(&timer_ctrl, pulse_counts_ms, pin) != FSP_SUCCESS) {
        return false;
    }
    return true;   
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_pulse_us(double us,uint32_t pin) {
/* -------------------------------------------------------------------------- */    
    uint32_t pclkd_freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKD) >> timer_cfg.source_div;
    uint32_t pulse_counts_us = (uint32_t) ((uint64_t) pclkd_freq_hz/1000000 * us);
    if (R_GPT_DutyCycleSet(&timer_ctrl, pulse_counts_us, pin) != FSP_SUCCESS) {
        return false;
    }
    return true;  
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_period_ms(double ms) {
/* -------------------------------------------------------------------------- */    
    
    double period_sec = (double)ms / 1000.0; 
    
    R_GPT_Close(&timer_ctrl);
    if(timer_cfg.channel < TIMER_16_BIT_OFFSET) {
        /* timer a 32 BIT */
        set_period_counts(period_sec, CH32BIT_MAX);
    }
    else {
        /* timer a 16 BIT */
        set_period_counts(period_sec, CH16BIT_MAX);
    }
    
    
    if (R_GPT_Open(&timer_ctrl,&timer_cfg) != FSP_SUCCESS) {
        return false;
    }
    

    if (R_GPT_Start(&timer_ctrl) != FSP_SUCCESS) {
        return false;
    }
    
    return true;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_period_us(double us) {
    return set_period_ms(us/1000.0);
}