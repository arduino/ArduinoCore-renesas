#include "FspTimer.h"

#define TIMER_16_BIT_OFFSET 2

FspTimer::FspTimer(): init_ok(false), agt_timer(nullptr), gpt_timer(nullptr), type(GPT_TIMER) {
    
}

FspTimer::~FspTimer() {
    if(agt_timer != nullptr) {
        delete agt_timer;
        agt_timer = nullptr; 
    }
    if(gpt_timer != nullptr) {
        delete gpt_timer;
        gpt_timer = nullptr;
    }
}

/* begin PWM with standard values...*/
/* -------------------------------------------------------------------------- */
bool FspTimer::begin_pwm(uint8_t tp, uint8_t channel, gpt_extended_pwm_cfg_t *pwm_cfg, TimerPWMChannel_t pwm_channel) {
/* -------------------------------------------------------------------------- */    
    init_ok = true;
    
    init_ok = begin(TIMER_MODE_PWM,tp,channel, STANDARD_PWM_FREQ_HZ ,STANDARD_DUTY_CYCLE_PERC);
    if(pwm_cfg != nullptr) {
        set_pwm_extended_cfg(pwm_cfg);
    }

    enable_pwm_channel(pwm_channel);

    if(init_ok) {
        init_ok &= open();
        init_ok &= start();
    }
       
    return init_ok;
}

/* begin function RAW mode */
/* -------------------------------------------------------------------------- */
bool FspTimer::begin(timer_mode_t mode, uint8_t tp, uint8_t channel, uint32_t period_counts, uint32_t pulse_counts, timer_source_div_t sd, void(*callback)(timer_callback_args_t *) /*= nullptr*/  ) {
/* -------------------------------------------------------------------------- */    
    init_ok = true;
    
    timer_cfg.mode                                  = mode;
    timer_cfg.source_div                            = sd;
    timer_cfg.period_counts                         = period_counts;
    timer_cfg.duty_cycle_counts                     = pulse_counts;
    timer_cfg.p_callback                            = callback;
    timer_cfg.p_context                             = NULL;
    timer_cfg.p_extend                              = NULL;
    timer_cfg.cycle_end_ipl                         = (BSP_IRQ_DISABLED);
    timer_cfg.cycle_end_irq                         = FSP_INVALID_VECTOR;


    if(tp == GPT_TIMER) {
        type = GPT_TIMER;
        gpt_timer = new GPTimer(timer_cfg);
        
    }
    else if(tp == AGT_TIMER) {
        type = AGT_TIMER;
        agt_timer = new AGTimer(timer_cfg);
        
    }
    else {
        init_ok = false;
    }
   
    if(channel > 7)       { init_ok &= false; }
    else                  { timer_cfg.channel = channel; }
    
    

    return init_ok;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::begin(timer_mode_t mode, uint8_t tp, uint8_t channel, float freq_hz, float duty_perc, void(*callback)(timer_callback_args_t *) /*= nullptr*/  ) {
/* -------------------------------------------------------------------------- */    
    
    init_ok = true;
    /* AGT timer is always 16 bit */
    if(channel < TIMER_16_BIT_OFFSET && type == GPT_TIMER) {
        /* timer a 32 BIT */
        set_period_counts(1.0 / freq_hz, CH32BIT_MAX);
        }
    else {
        /* timer a 16 BIT */
        set_period_counts(1.0 / freq_hz, CH16BIT_MAX);
    }

    if(duty_perc >= 0 && duty_perc <= 100) {
        _duty_cicle_counts = (uint32_t)(((float)_period_counts *  duty_perc) / 100.0);
    }
    else {
        init_ok = false;
    }
    
    if(init_ok) {
        init_ok = begin(mode, tp, channel, _period_counts, _duty_cicle_counts, _sd, callback );
    }
    return init_ok;
}

/* -------------------------------------------------------------------------- */
void FspTimer::set_period_counts(float period, uint32_t _max) {
/* -------------------------------------------------------------------------- */    
    
    uint32_t freq_hz = 0;
    if(type == GPT_TIMER) {
        freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKD);
        if(period * (float) freq_hz / 1.0 < _max) {
            _period_counts = (uint32_t) (period * (float) freq_hz / 1.0);
            _sd = TIMER_SOURCE_DIV_1;
        }
        else if(period * (float) freq_hz / 4.0 < _max) {
            _period_counts = (uint32_t) (period * (float) freq_hz / 4.0);
            _sd = TIMER_SOURCE_DIV_4;
        }
        else if(period * (float) freq_hz / 16.0 < _max) {
            _period_counts = (uint32_t) (period * (float) freq_hz / 16.0 );
            _sd = TIMER_SOURCE_DIV_16;
        }
        else if(period * (float) freq_hz / 64.0 < _max) {
            _period_counts = (uint32_t) (period * (float) freq_hz / 64.0 );
            _sd = TIMER_SOURCE_DIV_64;
        }
        else if(period * (float) freq_hz / 256.0 < _max) {
            _period_counts = (uint32_t) (period * (float) freq_hz / 256.0 );
            _sd = TIMER_SOURCE_DIV_256;
        }
        else if(period * (float) freq_hz / 1024.0 < _max) {
            _period_counts = (uint32_t) (period * (float) freq_hz / 16.0 );
            _sd = TIMER_SOURCE_DIV_1024;
        }
        else {
            init_ok = false;
        }
    }
    else if(type == AGT_TIMER) {
        freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB);
        if(period * (float) freq_hz / 1.0 < _max) {
            _period_counts = (uint32_t) (period * (float) freq_hz / 1.0);
            _sd = TIMER_SOURCE_DIV_1;
        }
        else if(period * (float) freq_hz / 2.0 < _max) {
            _period_counts = (uint32_t) (period * (float) freq_hz / 2.0);
            _sd = TIMER_SOURCE_DIV_2;
        }
        else if(period * (float) freq_hz / 8.0 < _max) {
            _period_counts = (uint32_t) (period * (float) freq_hz / 8.0 );
            _sd = TIMER_SOURCE_DIV_8;
        }
        else {
            init_ok = false;
        }
    }
}

/* -------------------------------------------------------------------------- */
void FspTimer::enable_pwm_channel(TimerPWMChannel_t pwm_channel) {
/* -------------------------------------------------------------------------- */
    if(type == GPT_TIMER && gpt_timer != nullptr) {
        if(pwm_channel == CHANNEL_A) {
            gpt_timer->ext_cfg.gtioca.output_enabled = true;
        } 
        else {
            gpt_timer->ext_cfg.gtiocb.output_enabled = true;
        }
    }
    else if(type == AGT_TIMER && agt_timer != nullptr) {
        if(pwm_channel == CHANNEL_A) {
            agt_timer->ext_cfg.agtoab_settings_b.agtoa = AGT_PIN_CFG_START_LEVEL_LOW;
        } 
        else {
            agt_timer->ext_cfg.agtoab_settings_b.agtob = AGT_PIN_CFG_START_LEVEL_LOW;
        }
    }
}

/* -------------------------------------------------------------------------- */
uint32_t FspTimer::get_period_raw() {
/* -------------------------------------------------------------------------- */    
    timer_info_t info;
    if(type == GPT_TIMER) {
        R_GPT_InfoGet(&(gpt_timer->ctrl), &info);
    }
    else if(type == AGT_TIMER){
        R_AGT_InfoGet(&(agt_timer->ctrl), &info);
    }
    else {
        return 0;
    }
    return info.period_counts;
}

	
/* -------------------------------------------------------------------------- */
 uint32_t FspTimer::get_counter() {
/* -------------------------------------------------------------------------- */ 
    timer_status_t status;
    if(type == GPT_TIMER) {
        R_GPT_StatusGet(&(gpt_timer->ctrl), &status);
    }
    else if(type == AGT_TIMER){
        R_AGT_StatusGet(&(agt_timer->ctrl), &status);
    }
    else {
        return 0;
    }
    return status.counter;
 }

/* -------------------------------------------------------------------------- */    
uint32_t FspTimer::get_freq_hz() {
/* -------------------------------------------------------------------------- */
    timer_info_t info;
    if(type == GPT_TIMER) {
        R_GPT_InfoGet(&(gpt_timer->ctrl), &info);
    }
    else if(type == AGT_TIMER){
        R_AGT_InfoGet(&(agt_timer->ctrl), &info);
    }
    else {
        return 0;
    }
    return info.clock_frequency;
}

/* -------------------------------------------------------------------------- */
void FspTimer::set_pwm_extended_cfg(gpt_extended_pwm_cfg_t *cfg) {
/* -------------------------------------------------------------------------- */
    if(gpt_timer != nullptr) {
        gpt_timer->ext_cfg.p_pwm_cfg = cfg;
    }
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_pulse_ms(double ms,TimerPWMChannel_t pwm_ch) {
/* -------------------------------------------------------------------------- */ 
    
    uint32_t freq_hz = 0;
    if(type == GPT_TIMER) {
        freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKD) >> timer_cfg.source_div;
    }
    else if(type == AGT_TIMER){
        freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB) >> timer_cfg.source_div;
    }
    
    uint32_t pulse_counts_ms = (uint32_t) ((uint64_t) freq_hz/1000 * ms);
    if(!set_duty_cycle(pulse_counts_ms, pwm_ch)) {
        return false;
    }
    
    return true;  
      
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_pulse_us(double us,TimerPWMChannel_t pwm_ch) {
/* -------------------------------------------------------------------------- */    
    uint32_t freq_hz = 0;
    if(type == GPT_TIMER) {
        freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKD) >> timer_cfg.source_div;
    }
    else if(type == AGT_TIMER){
        freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKB) >> timer_cfg.source_div;
    }
    
    uint32_t pulse_counts_us = (uint32_t) ((uint64_t) freq_hz/1000000 * us);
    if(!set_duty_cycle(pulse_counts_us, pwm_ch)) {
        return false;
    }
    
    return true;  
}


/* -------------------------------------------------------------------------- */
bool FspTimer::set_period_ms(double ms) {
/* -------------------------------------------------------------------------- */    
    
    double period_sec = (double)ms / 1000.0; 
    
    close();
    if(timer_cfg.channel < TIMER_16_BIT_OFFSET && type == GPT_TIMER) {
        /* timer a 32 BIT */
        set_period_counts(period_sec, CH32BIT_MAX);
    }
    else {
        /* timer a 16 BIT */
        set_period_counts(period_sec, CH16BIT_MAX);
    }
    
    timer_cfg.period_counts = _period_counts;
    timer_cfg.source_div = _sd;

    if (!open()) {
        return false;
    }

    if (!start()) {
        return false;
    }
    
    return true;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_period_us(double us) {
    return set_period_ms(us/1000.0);
}


/* -------------------------------------------------------------------------- */
bool FspTimer::set_period(uint32_t p) {
/* -------------------------------------------------------------------------- */    
    fsp_err_t err;
    if(type == GPT_TIMER && gpt_timer != nullptr) {
        if (R_GPT_PeriodSet(&(gpt_timer->ctrl), p) != FSP_SUCCESS) {
            return false;
        }
    }
    else if(type == AGT_TIMER && agt_timer != nullptr) {
        if (R_AGT_PeriodSet(&(agt_timer->ctrl), p) != FSP_SUCCESS) {
            return false;
        }
    }
    else {
        return false;
    }
    return true;
}



/* -------------------------------------------------------------------------- */
bool FspTimer::open() {
/* -------------------------------------------------------------------------- */    
    fsp_err_t err;
    if(type == GPT_TIMER && gpt_timer != nullptr) {
        err = R_GPT_Open(&(gpt_timer->ctrl),&timer_cfg);
    }
    else if(type == AGT_TIMER && agt_timer != nullptr) {
        err = R_AGT_Open(&(agt_timer->ctrl),&timer_cfg);
    }
    else {
        return false;
    }

    if ((err != FSP_ERR_ALREADY_OPEN) && (err != FSP_SUCCESS)) {
        return false;
    }
    
    return true;

}

/* -------------------------------------------------------------------------- */
bool FspTimer::stop() {
/* -------------------------------------------------------------------------- */    

    if(type == GPT_TIMER && gpt_timer != nullptr) {
        if (R_GPT_Stop(&(gpt_timer->ctrl)) != FSP_SUCCESS) {
            return false;
        }
    }
    else if(type == AGT_TIMER && agt_timer != nullptr) {
        if (R_AGT_Stop(&(agt_timer->ctrl)) != FSP_SUCCESS) {
            return false;
        }
    }
    else {
        return false;
    }

    return true; 
} 


/* -------------------------------------------------------------------------- */
bool FspTimer::start() {
/* -------------------------------------------------------------------------- */    

    if(type == GPT_TIMER && gpt_timer != nullptr) {
        if (R_GPT_Start(&(gpt_timer->ctrl)) != FSP_SUCCESS) {
            return false;
        }
    }
    else if(type == AGT_TIMER && agt_timer != nullptr) {
        if (R_AGT_Start(&(agt_timer->ctrl)) != FSP_SUCCESS) {
            return false;
        }
    }
    else {
        return false;
    }

    return true; 
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_duty_cycle(uint32_t const duty_cycle_counts, TimerPWMChannel_t pwm_ch) {
/* -------------------------------------------------------------------------- */    
    if(type == GPT_TIMER && gpt_timer != nullptr) {
        uint32_t pin = (pwm_ch == CHANNEL_A) ?  GPT_IO_PIN_GTIOCA : GPT_IO_PIN_GTIOCB;   
        if (R_GPT_DutyCycleSet(&(gpt_timer->ctrl), duty_cycle_counts, pin) != FSP_SUCCESS) {
            return false;
        }
    }
    else if(type == AGT_TIMER && agt_timer != nullptr) {
        uint32_t pin = (pwm_ch == CHANNEL_A) ?  AGT_OUTPUT_PIN_AGTOA : AGT_OUTPUT_PIN_AGTOB;
        if (R_AGT_DutyCycleSet(&(agt_timer->ctrl), duty_cycle_counts, pin) != FSP_SUCCESS) {
            return false;
        }
    }
    else {
        return false;
    }

    return true; 
}

/* -------------------------------------------------------------------------- */
bool FspTimer::close() {
/* -------------------------------------------------------------------------- */    
     if(type == GPT_TIMER && gpt_timer != nullptr) {
        if (R_GPT_Close(&(gpt_timer->ctrl)) != FSP_SUCCESS) {
            return false;
        }
    }
    else if(type == AGT_TIMER && agt_timer != nullptr) {
        if (R_AGT_Close(&(agt_timer->ctrl)) != FSP_SUCCESS) {
            return false;
        }
    }
    else {
        return false;
    }

    return true; 

}

