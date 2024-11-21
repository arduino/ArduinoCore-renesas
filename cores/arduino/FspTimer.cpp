#include "FspTimer.h"


#define TIMER_16_BIT_OFFSET      GTP32_HOWMANY

#define CH32BIT_MAX         (4294967295)
#define CH16BIT_MAX         (65535)

bool FspTimer::force_pwm_reserved = false;

TimerAvail_t FspTimer::gpt_used_channel[GPT_HOWMANY] = { TIMER_FREE };
TimerAvail_t FspTimer::agt_used_channel[AGT_HOWMANY] = { TIMER_FREE };

FspTimer::FspTimer(): init_ok(false), agt_timer(nullptr), gpt_timer(nullptr), type(GPT_TIMER), _period_buffer(true) {
    // AGT0 is always used for timekeeping (millis() and micros())
    // agt_used_channel[0] = TIMER_USED;
    timer_cfg.cycle_end_irq = FSP_INVALID_VECTOR;
}

FspTimer::~FspTimer() {
    end();
}

void FspTimer::end() {
    close();
    if(agt_timer != nullptr) {
        agt_used_channel[timer_cfg.channel] = TIMER_FREE;
        delete agt_timer;
        agt_timer = nullptr; 
    }
    if(gpt_timer != nullptr) {
        gpt_used_channel[timer_cfg.channel] = TIMER_FREE;
        delete gpt_timer;
        gpt_timer = nullptr;
    }
}

/* begin PWM with standard values...*/
/* -------------------------------------------------------------------------- */
bool FspTimer::begin_pwm(uint8_t tp, uint8_t channel, TimerPWMChannel_t pwm_channel) {
/* -------------------------------------------------------------------------- */    
    init_ok = true;

    init_ok = begin(TIMER_MODE_PWM,tp,channel, STANDARD_PWM_FREQ_HZ ,STANDARD_DUTY_CYCLE_PERC);

    if(tp == GPT_TIMER) {
        add_pwm_extended_cfg();
    }

    enable_pwm_channel(pwm_channel);

    if(init_ok) {
        init_ok &= open();
        init_ok &= start();
    }

    return init_ok;
}

extern FspTimer* __get_timer_for_channel(int channel);

/* begin function RAW mode */
/* -------------------------------------------------------------------------- */
bool FspTimer::begin(timer_mode_t mode, uint8_t tp, uint8_t channel, uint32_t period_counts, uint32_t pulse_counts, timer_source_div_t sd, GPTimerCbk_f cbk /*= nullptr*/ , void *ctx /*= nullptr*/ ) {
/* -------------------------------------------------------------------------- */    
    init_ok = false;
    
    timer_cfg.mode                                  = mode;
    timer_cfg.source_div                            = sd;
    timer_cfg.period_counts                         = period_counts;
    timer_cfg.duty_cycle_counts                     = pulse_counts;
    timer_cfg.p_callback                            = cbk;
    timer_cfg.p_context                             = ctx;
    timer_cfg.p_extend                              = nullptr;
    timer_cfg.cycle_end_ipl                         = (BSP_IRQ_DISABLED);

    if(tp == GPT_TIMER) {
        type = GPT_TIMER;
        gpt_timer = new GPTimer(timer_cfg);

        if(channel < GPT_HOWMANY) {
            if(mode == TIMER_MODE_PWM || FspTimer::force_pwm_reserved) {
                if(gpt_used_channel[channel] == TIMER_PWM || gpt_used_channel[channel] == TIMER_FREE) {
                    timer_cfg.channel = channel;
                    gpt_used_channel[channel] = TIMER_USED;
                    init_ok = true;
                } else if (mode == TIMER_MODE_PWM) {
                    // check if compatible PWM on another channel
                    timer_cfg.channel = channel;
                    memcpy(&(gpt_timer->ext_cfg), __get_timer_for_channel(channel)->get_cfg()->p_extend, sizeof(gpt_extended_cfg_t));
                    timer_cfg.duty_cycle_counts = __get_timer_for_channel(channel)->get_duty_cycle();
                    init_ok = true;
                }
            }
            else {
                if(gpt_used_channel[channel] == TIMER_FREE) {
                    timer_cfg.channel = channel;
                    gpt_used_channel[channel] = TIMER_USED;
                    init_ok = true;
                }
            } 
        }
        
    }
    else if(tp == AGT_TIMER) {
        type = AGT_TIMER;
        agt_timer = new AGTimer(timer_cfg);
        
        if(channel < AGT_HOWMANY) {
            if(mode == TIMER_MODE_PWM || FspTimer::force_pwm_reserved) {
                if(agt_used_channel[channel] == TIMER_PWM || agt_used_channel[channel] == TIMER_FREE) {
                    timer_cfg.channel = channel;
                    agt_used_channel[channel] = TIMER_USED;
                    init_ok = true;
                }
            }
            else {
                if(agt_used_channel[channel] == TIMER_FREE) {
                    timer_cfg.channel = channel;
                    agt_used_channel[channel] = TIMER_USED;
                    init_ok = true;
                }
            } 
        }
    }
    FspTimer::force_pwm_reserved = false;
    return init_ok;
}



/* -------------------------------------------------------------------------- */
int8_t FspTimer::get_available_timer(uint8_t &type, bool force /*= false*/) {
/* -------------------------------------------------------------------------- */    
    int8_t rv = -1;
    for(uint8_t i = 0; i < GPT_HOWMANY; i++) {
        if(gpt_used_channel[i] == TIMER_FREE) {
            rv = i;
            type = GPT_TIMER;
            break;
        }
    }

    if(rv == -1) {
        for(uint8_t i = 0; i < AGT_HOWMANY; i++) {
            if(agt_used_channel[i] == TIMER_FREE) {
                rv = i;
                type = AGT_TIMER;
                break;
            }
        }
    }

    if(force && rv == -1) {
        for(int8_t i = GPT_HOWMANY-1; i >= 0; i--) {
            if(gpt_used_channel[i] != TIMER_USED) {
                rv = i;
                type = GPT_TIMER;
                break;
            }
        }

        if(rv == -1) {
            for(int8_t i = AGT_HOWMANY - 1; i >= 0; i--) {
                if(agt_used_channel[i] != TIMER_USED) {
                    rv = i;
                    type = AGT_TIMER;
                    break;
                }
            }
        }
        if(rv != -1) {
            FspTimer::force_pwm_reserved = true;
        }
    }

    return rv;

}


/* -------------------------------------------------------------------------- */
bool FspTimer::begin(timer_mode_t mode, uint8_t tp, uint8_t channel, float freq_hz, float duty_perc, GPTimerCbk_f cbk /*= nullptr*/ , void *ctx /*= nullptr*/  ) {
/* -------------------------------------------------------------------------- */    
    
    init_ok = true;
    /* AGT timer is always 16 bit */
    if(channel < TIMER_16_BIT_OFFSET && tp == GPT_TIMER) {
        /* timer a 32 BIT */
        set_period_counts(tp, 1.0 / freq_hz, CH32BIT_MAX);
        }
    else {
        /* timer a 16 BIT */
        set_period_counts(tp, 1.0 / freq_hz, CH16BIT_MAX);
    }

    if(duty_perc >= 0 && duty_perc <= 100) {
        _duty_cycle_counts = (uint32_t)(((float)_period_counts *  duty_perc) / 100.0);
    }
    else {
        init_ok = false;
    }
    
    if(init_ok) {
        init_ok = begin(mode, tp, channel, _period_counts, _duty_cycle_counts, _sd, cbk, ctx);
    }
    return init_ok;
}

/* -------------------------------------------------------------------------- */
void FspTimer::set_irq_callback(GPTimerCbk_f cbk , void *ctx /*= nullptr*/ ) {
/* -------------------------------------------------------------------------- */    
    timer_cfg.p_callback                            = cbk;
    timer_cfg.p_context                             = ctx;
}

/* -------------------------------------------------------------------------- */
void FspTimer::set_period_counts(uint8_t tp, float period, uint32_t _max) {
/* -------------------------------------------------------------------------- */    
    
    uint32_t freq_hz = 0;
    if(tp == GPT_TIMER) {
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
            _period_counts = (uint32_t) (period * (float) freq_hz / 1024.0 );
            _sd = TIMER_SOURCE_DIV_1024;
        }
        else {
            init_ok = false;
        }
    }
    else if(tp == AGT_TIMER) {
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
uint32_t FspTimer::get_channel() {
/* -------------------------------------------------------------------------- */
    return get_cfg()->channel;
}

/* -------------------------------------------------------------------------- */
void FspTimer::add_pwm_extended_cfg() {
/* -------------------------------------------------------------------------- */
    if(gpt_timer != nullptr) {
        gpt_timer->ext_cfg.p_pwm_cfg = new gpt_extended_pwm_cfg_t;

        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->trough_ipl               = (BSP_IRQ_DISABLED);
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->trough_irq               = FSP_INVALID_VECTOR;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->poeg_link                = GPT_POEG_LINK_POEG0;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->output_disable           = GPT_OUTPUT_DISABLE_NONE;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->adc_trigger              = GPT_ADC_TRIGGER_NONE;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->dead_time_count_up       = 0;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->dead_time_count_down     = 0;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->adc_a_compare_match      = 0;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->adc_b_compare_match      = 0;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->interrupt_skip_source    = GPT_INTERRUPT_SKIP_SOURCE_NONE;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->interrupt_skip_count     = GPT_INTERRUPT_SKIP_COUNT_0;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->interrupt_skip_adc       = GPT_INTERRUPT_SKIP_ADC_NONE;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->gtioca_disable_setting   = GPT_GTIOC_DISABLE_PROHIBITED;
        ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->gtiocb_disable_setting   = GPT_GTIOC_DISABLE_PROHIBITED;
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
    
    uint32_t pulse_counts_ms = (uint32_t) ((uint64_t) (freq_hz * ms)/1000);
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

    uint32_t pulse_counts_us = (uint32_t) ((uint64_t) (freq_hz * us)/1000000 );
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
        set_period_counts(type, period_sec, CH32BIT_MAX);
    }
    else {
        /* timer a 16 BIT */
        set_period_counts(type, period_sec, CH16BIT_MAX);
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

bool FspTimer::set_frequency(float hz) {
    return set_period_ms(1000/hz);
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_period(uint32_t p) {
/* -------------------------------------------------------------------------- */    

    if(type == GPT_TIMER && gpt_timer != nullptr) {
        if (_period_buffer) {
        if (R_GPT_PeriodSet(&(gpt_timer->ctrl), p) != FSP_SUCCESS) {
            return false;
        }
    }
        else {
            // Not buffered set it directly
            gpt_timer->ctrl.p_reg->GTPR = p;
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
bool FspTimer::set_period_buffer(bool period_buffer) {
/* -------------------------------------------------------------------------- */    

    if (_period_buffer == (uint8_t)period_buffer) {
        return true;
    }

    _period_buffer = (uint8_t)period_buffer;
    if(type == GPT_TIMER && gpt_timer != nullptr) {

        if (period_buffer) {
            gpt_timer->ctrl.p_reg->GTBER_b.PR = 1;
            gpt_timer->ctrl.p_reg->GTBER_b.BD1 = 0;
        }
        else {
            gpt_timer->ctrl.p_reg->GTBER_b.PR = 0;
            gpt_timer->ctrl.p_reg->GTBER_b.BD1 = 1;
        } 
    }
    else if(type == AGT_TIMER && agt_timer != nullptr) {
        // not buffered..
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
        R_GPT_Enable(&gpt_timer->ctrl);
    }
    else if(type == AGT_TIMER && agt_timer != nullptr) {
        err = R_AGT_Open(&(agt_timer->ctrl),&timer_cfg);
        R_AGT_Enable(&(agt_timer->ctrl));
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
bool FspTimer::reset() {
/* -------------------------------------------------------------------------- */
    if(type == GPT_TIMER && gpt_timer != nullptr) {
        if (R_GPT_Reset(&(gpt_timer->ctrl)) != FSP_SUCCESS) {
            return false;
        }
    }
    else if(type == AGT_TIMER && agt_timer != nullptr) {
        if (R_AGT_Reset(&(agt_timer->ctrl)) != FSP_SUCCESS) {
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
    _duty_cycle_counts = duty_cycle_counts;
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

/* -------------------------------------------------------------------------- */
bool FspTimer::set_source_start(gpt_source_t src) { 
/* -------------------------------------------------------------------------- */    
    if(gpt_timer != nullptr) {
        gpt_timer->ext_cfg.start_source = src;
        return true;
    }
    return false;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_source_stop(gpt_source_t src) {
/* -------------------------------------------------------------------------- */    
    if(gpt_timer != nullptr) {
        gpt_timer->ext_cfg.stop_source = src;
        return true;
    }
    return false;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_source_clear(gpt_source_t src) {
/* -------------------------------------------------------------------------- */
    if(gpt_timer != nullptr) {
        gpt_timer->ext_cfg.clear_source = src;
        return true;
    }
    return false;

}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_source_count_up(gpt_source_t src) {
/* -------------------------------------------------------------------------- */    
    if(gpt_timer != nullptr) {
        gpt_timer->ext_cfg.count_up_source = src;
        return true;
    }
    return false;
}


/* -------------------------------------------------------------------------- */
bool FspTimer::set_source_count_down(gpt_source_t src) {
/* -------------------------------------------------------------------------- */    
    if(gpt_timer != nullptr) {
        gpt_timer->ext_cfg.count_down_source = src;
        return true;
    }
    return false;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_source_capture_a(gpt_source_t src) {
/* -------------------------------------------------------------------------- */    
    if(gpt_timer != nullptr) {
        gpt_timer->ext_cfg.capture_a_source = src;
        return true;
    }
    return false;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::set_source_capture_b(gpt_source_t src) {
/* -------------------------------------------------------------------------- */    
    if(gpt_timer != nullptr) {
        gpt_timer->ext_cfg.capture_b_source = src;
        return true;
    }
    return false;
}

/* -------------------------------------------------------------------------- */
TimerIrqCfg_t FspTimer::get_cfg_for_irq() {
/* -------------------------------------------------------------------------- */    
    TimerIrqCfg_t cfg;
    cfg.base_cfg = &timer_cfg;
    if(gpt_timer != nullptr) {
       cfg.gpt_ext_cfg = &(gpt_timer->ext_cfg);
    }
    else {
       cfg.gpt_ext_cfg = nullptr; 
    }

    if(agt_timer != nullptr) {
       cfg.agt_ext_cfg = &(agt_timer->ext_cfg);
    }
    else {
       cfg.agt_ext_cfg = nullptr; 
    }
    return cfg;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::is_opened() {
/* -------------------------------------------------------------------------- */
    bool rv = false;
    
    if(gpt_timer != nullptr) {
       rv = (gpt_timer->ctrl.open) ? true : false;
    }
    else if(agt_timer != nullptr) {
       rv = (agt_timer->ctrl.open) ? true : false;
    }
   
    return rv;
}

extern void R_BSP_IrqEnable (IRQn_Type const irq);
extern void R_BSP_IrqDisable (IRQn_Type const irq);

/* -------------------------------------------------------------------------- */
void FspTimer::disable_overflow_irq() {
/* -------------------------------------------------------------------------- */    
    synchronized {
        //TimerIrqCfg_t cfg = get_cfg_for_irq();
        R_BSP_IrqDisable ((IRQn_Type)timer_cfg.cycle_end_irq);
    }
}

/* -------------------------------------------------------------------------- */
void FspTimer::enable_overflow_irq() {
/* -------------------------------------------------------------------------- */    
    synchronized {
        //TimerIrqCfg_t cfg = get_cfg_for_irq();
        R_BSP_IrqEnable ((IRQn_Type)timer_cfg.cycle_end_irq);
    }
}

/* -------------------------------------------------------------------------- */
bool FspTimer::setup_overflow_irq(uint8_t priority /*= 12*/,  Irq_f isr_fnc /*= nullptr*/ ) {
/* -------------------------------------------------------------------------- */
    
    TimerIrqCfg_t cfg = get_cfg_for_irq();
    /* that is due to the fact that when the timer is opened the configuration
       of the interrupt is ignore by FSP, this is usually not a problem
       if first you set up the timer and then open and start it 
       in some case, for example you want to set up an interrupt via a PWM timer
       you need to call begin in advance (otherwise timer is not correctly populated) 
       but begin open and start the timer*/
    bool was_opened = is_opened();

    if(was_opened) {
        close();
    }


    bool rv = IRQManager::getInstance().addTimerOverflow(cfg,isr_fnc);

    if(rv) {
        timer_cfg.cycle_end_ipl = priority;
    }
    else {
        timer_cfg.cycle_end_irq = FSP_INVALID_VECTOR;    
    }

    if(was_opened) {
        open();
        start();
    }
    return rv;
}

/* -------------------------------------------------------------------------- */
bool FspTimer::setup_underflow_irq(uint8_t priority /*= 12*/, Irq_f isr_fnc /*= nullptr */) {
/* -------------------------------------------------------------------------- */
    TimerIrqCfg_t cfg = get_cfg_for_irq();
    
    bool was_opened = is_opened();

    if(was_opened) {
        close();
    }
    
    /* underflow is handled using extended pwm, if not available -> make it*/
    if(gpt_timer != nullptr) {
        if(gpt_timer->ext_cfg.p_pwm_cfg == nullptr) {
            add_pwm_extended_cfg();
        }
    }


    bool rv = IRQManager::getInstance().addTimerUnderflow(cfg, isr_fnc);
    
    if(gpt_timer != nullptr) {
        if(gpt_timer->ext_cfg.p_pwm_cfg != nullptr ) {
            if(rv) {
                ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->trough_ipl = priority;
            }
            else {
                ((gpt_extended_pwm_cfg_t *)(gpt_timer->ext_cfg.p_pwm_cfg))->trough_irq = FSP_INVALID_VECTOR;
            }
        }
    }
    
    if(was_opened) {
        open();
        start();
    }

    return rv;
}


/* -------------------------------------------------------------------------- */
bool FspTimer::setup_capture_a_irq(uint8_t priority /*= 12*/, Irq_f isr_fnc /*= nullptr*/ ) {
/* -------------------------------------------------------------------------- */
    TimerIrqCfg_t cfg = get_cfg_for_irq();
    bool was_opened = is_opened();

    if(was_opened) {
        close();
    }
    bool rv = IRQManager::getInstance().addTimerCompareCaptureA(cfg, isr_fnc);
    if(gpt_timer != nullptr) {
        if(rv) {
            cfg.gpt_ext_cfg->capture_a_ipl = priority;
        }
        else {
            cfg.gpt_ext_cfg->capture_a_irq = FSP_INVALID_VECTOR;
        }    
    }
    
    if(was_opened) {
        open();
        start();
    }
    return rv;
}


/* -------------------------------------------------------------------------- */
bool FspTimer::setup_capture_b_irq(uint8_t priority /*= 12*/, Irq_f isr_fnc /*= nullptr */) {
/* -------------------------------------------------------------------------- */
    TimerIrqCfg_t cfg = get_cfg_for_irq();
    bool was_opened = is_opened();

    if(was_opened) {
        close();
    }
    bool rv = IRQManager::getInstance().addTimerCompareCaptureB(cfg, isr_fnc);
    if(gpt_timer != nullptr) {
        if(rv) {
            cfg.gpt_ext_cfg->capture_b_ipl = priority;
        }
        else {
            cfg.gpt_ext_cfg->capture_b_irq = FSP_INVALID_VECTOR;
        }    
    }
    if(was_opened) {
        open();
        start();
    }
    return rv;
}
