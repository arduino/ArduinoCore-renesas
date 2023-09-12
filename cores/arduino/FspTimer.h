#ifndef ARDUINO_FSP_TIMER_H
#define ARDUINO_FSP_TIMER_H
#include "Arduino.h"
#include "IRQManager.h"


#define GPT_TIMER           (0)
#define AGT_TIMER           (1)

#define STANDARD_PWM_FREQ_HZ       (490.0)
#define STANDARD_DUTY_CYCLE_PERC   (50.0)

using GPTimerCbk_f          = void(*)(timer_callback_args_t *);

typedef enum {
  TIMER_FREE = 0,
  TIMER_PWM,
  TIMER_USED
} TimerAvail_t;


typedef enum {
  CHANNEL_A,
  CHANNEL_B
} TimerPWMChannel_t;



class GPTimer {
  public:
    ~GPTimer() {
      if(ext_cfg.p_pwm_cfg != nullptr) {
        delete ext_cfg.p_pwm_cfg;
      }
    }
    GPTimer(timer_cfg_t &cfg) {
      ext_cfg.gtioca.output_enabled        = false;
      ext_cfg.gtioca.stop_level            = GPT_PIN_LEVEL_LOW;
      ext_cfg.gtiocb.output_enabled        = false;
      ext_cfg.gtiocb.stop_level            = GPT_PIN_LEVEL_LOW;
      ext_cfg.start_source                 = (gpt_source_t) (GPT_SOURCE_NONE); 
      ext_cfg.stop_source                  = (gpt_source_t) (GPT_SOURCE_NONE);
      ext_cfg.clear_source                 = (gpt_source_t) (GPT_SOURCE_NONE);
      ext_cfg.count_up_source              = (gpt_source_t) (GPT_SOURCE_NONE); 
      ext_cfg.count_down_source            = (gpt_source_t) (GPT_SOURCE_NONE); 
      ext_cfg.capture_a_source             = (gpt_source_t) (GPT_SOURCE_NONE);
      ext_cfg.capture_b_source             = (gpt_source_t) (GPT_SOURCE_NONE); 
      ext_cfg.capture_a_ipl                = (BSP_IRQ_DISABLED);
      ext_cfg.capture_b_ipl                = (BSP_IRQ_DISABLED);
      ext_cfg.capture_a_irq                = FSP_INVALID_VECTOR;
      ext_cfg.capture_b_irq                = FSP_INVALID_VECTOR;
      ext_cfg.capture_filter_gtioca        = GPT_CAPTURE_FILTER_NONE;
      ext_cfg.capture_filter_gtiocb        = GPT_CAPTURE_FILTER_NONE;
      ext_cfg.p_pwm_cfg                    = nullptr;
      ext_cfg.gtior_setting.gtior          = 0U;

      cfg.p_extend                         = &ext_cfg;

      memset(&ctrl, 0, sizeof(gpt_instance_ctrl_t));
    }

    gpt_instance_ctrl_t ctrl;
    gpt_extended_cfg_t ext_cfg;
};

class AGTimer {
  public:
  AGTimer(timer_cfg_t &cfg) {
      ext_cfg.count_source                  = AGT_CLOCK_PCLKB;
	    ext_cfg.agto                          = AGT_PIN_CFG_DISABLED;
	    ext_cfg.agtoab_settings_b.agtoa       = AGT_PIN_CFG_DISABLED;
	    ext_cfg.agtoab_settings_b.agtob       = AGT_PIN_CFG_DISABLED;
	    ext_cfg.measurement_mode              = AGT_MEASURE_DISABLED;
	    ext_cfg.agtio_filter                  = AGT_AGTIO_FILTER_NONE;
	    ext_cfg.enable_pin                    = AGT_ENABLE_PIN_NOT_USED;
	    ext_cfg.trigger_edge                  = AGT_TRIGGER_EDGE_RISING;

      cfg.p_extend                         = &ext_cfg;

      memset(&ctrl, 0, sizeof(agt_instance_ctrl_t));
  }
  agt_extended_cfg_t ext_cfg;
  agt_instance_ctrl_t ctrl;
};



class FspTimer {
  private:
    bool init_ok;
    AGTimer *agt_timer;
    GPTimer *gpt_timer;
    timer_cfg_t timer_cfg;
    uint32_t _period_counts;
    uint32_t _duty_cycle_counts;
    timer_source_div_t _sd;
    uint8_t type;
    uint8_t _period_buffer;
    void set_period_counts(uint8_t tp, float period, uint32_t max);
    TimerIrqCfg_t get_cfg_for_irq();
    static bool force_pwm_reserved;
    static TimerAvail_t gpt_used_channel[GPT_HOWMANY];
    static TimerAvail_t agt_used_channel[AGT_HOWMANY];

  public:
    FspTimer();
    ~FspTimer();
    bool is_opened();
    bool open();
    bool start();
    bool stop();
    bool reset();
    bool set_duty_cycle(uint32_t const duty_cycle_counts, TimerPWMChannel_t pwm_ch);
    bool set_period(uint32_t p);
    bool set_period_buffer(bool period_buffer);
    bool close();
    void enable_pwm_channel(TimerPWMChannel_t pwm_channel);
    uint32_t get_counter();
    uint32_t get_freq_hz();
    uint32_t get_duty_cycle() { return _duty_cycle_counts; };
    uint32_t get_channel();

    static void set_timer_is_used(uint8_t type, int index) {
      if (type == GPT_TIMER) {
        gpt_used_channel[index] = TIMER_USED;
      }
      if (type == AGT_TIMER) {
        agt_used_channel[index] = TIMER_USED;
      }
    }

    static void set_initial_timer_channel_as_pwm(uint8_t type, int index) {
      if (type == GPT_TIMER) {
        gpt_used_channel[index] = TIMER_PWM;
      }
      if (type == AGT_TIMER) {
        agt_used_channel[index] = TIMER_PWM;
      }
    }

    timer_cfg_t *get_cfg() { return &timer_cfg; }
    
    bool begin_pwm(uint8_t type, uint8_t channel, TimerPWMChannel_t pwm_channel);
    
    bool begin(timer_mode_t mode, uint8_t type, uint8_t channel,  float freq_hz, float duty_perc, GPTimerCbk_f cbk = nullptr , void *ctx = nullptr );
    bool begin(timer_mode_t mode, uint8_t type, uint8_t channel,  uint32_t period, uint32_t pulse, timer_source_div_t sd, GPTimerCbk_f cbk = nullptr , void *ctx = nullptr);
    
    void end();

    void set_irq_callback(GPTimerCbk_f cbk , void *ctx = nullptr );

    bool setup_overflow_irq(uint8_t priority = 12,  Irq_f isr_fnc = nullptr );
    bool setup_underflow_irq(uint8_t priority = 12, Irq_f isr_fnc = nullptr );
    bool setup_capture_a_irq(uint8_t priority = 12, Irq_f isr_fnc = nullptr ); 
    bool setup_capture_b_irq(uint8_t priority = 12, Irq_f isr_fnc = nullptr );
    
    void disable_overflow_irq();
    void enable_overflow_irq();

    bool set_source_start(gpt_source_t src);
    bool set_source_stop(gpt_source_t src);
    bool set_source_clear(gpt_source_t scr);
    bool set_source_count_up(gpt_source_t scr);
    bool set_source_count_down(gpt_source_t scr);
    bool set_source_capture_a(gpt_source_t scr);
    bool set_source_capture_b(gpt_source_t scr);
    
    uint32_t get_period_raw();
    void add_pwm_extended_cfg();
    bool set_frequency(float hz);
    bool set_period_ms(double ms);
    bool set_period_us(double us);
    bool set_pulse_ms(double ms,TimerPWMChannel_t pwm_ch);
    bool set_pulse_us(double us,TimerPWMChannel_t pwm_ch);

    static int8_t get_available_timer(uint8_t &type, bool force = false); 
    static void force_use_of_pwm_reserved_timer() {FspTimer::force_pwm_reserved = true; }
};


#endif