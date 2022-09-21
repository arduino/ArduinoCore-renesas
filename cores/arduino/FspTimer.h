#include "Arduino.h"

#ifndef ARDUINO_FSP_TIMER_H
#define ARDUINO_FSP_TIMER_H


#define FREQUENCY_0_PCLKD     (48000000)
#define FREQUENCY_1_PCLKD     (FREQUENCY_0_PCLKD / 4)
#define FREQUENCY_2_PCLKD     (FREQUENCY_1_PCLKD / 4)
#define FREQUENCY_3_PCLKD     (FREQUENCY_2_PCLKD / 4)
#define FREQUENCY_4_PCLKD     (FREQUENCY_3_PCLKD / 4)
#define FREQUENCY_5_PCLKD     (FREQUENCY_4_PCLKD / 4)

#define TICK_0              (float)(1.0/(float)FREQUENCY_0_PCLKD)
#define TICK_1              (float)(1.0/(float)FREQUENCY_1_PCLKD)
#define TICK_2              (float)(1.0/(float)FREQUENCY_2_PCLKD)
#define TICK_3              (float)(1.0/(float)FREQUENCY_3_PCLKD)
#define TICK_4              (float)(1.0/(float)FREQUENCY_4_PCLKD)
#define TICK_5              (float)(1.0/(float)FREQUENCY_5_PCLKD)

#define CH32BIT_MAX         (4294967295)
#define CH16BIT_MAX         (65535)

#define GPT_TIMER           (0)
#define AGT_TIMER           (1)

#define TIMER_DIVIDER_AUTO       (-1)
#define TIMER_DIVEDER_4          (2)
#define TIMER_DIVIDER_16         (4)
#define TIMER_DIVIDER_64         (6)
#define TIMER_DIVIDER_256        (8)
#define TIMER_DIVIDER_1024       (10)

typedef enum {
  CHANNEL_A,
  CHANNEL_B
} TimerPWMChannel_t;


class FspTimer {
    private:
    gpt_extended_cfg_t timer_extended_cfg;
    timer_cfg_t timer_cfg;
    gpt_instance_ctrl_t timer_ctrl;
    bool init_ok;
    void set_period_counts(float period, uint32_t max);
    public:
    FspTimer();
    bool begin_pwm(uint8_t type, uint8_t channel, gpt_extended_pwm_cfg_t *pwm_cfg, TimerPWMChannel_t pwm_channel);
    bool begin(timer_mode_t mode, uint8_t type, uint8_t channel,  float freq_hz, float duty_perc );
    bool begin(timer_mode_t mode, uint8_t type, uint8_t channel,  uint32_t period, uint32_t pulse, timer_source_div_t sd );
    
    gpt_instance_ctrl_t *get_ctrl_ptr() { return &timer_ctrl; }
    timer_cfg_t *get_cfg_ptr() { return &timer_cfg; }
    void enable_gtioc_CH_A();
    void enable_gtioc_CH_B();
    void set_pwm_extended_cfg(gpt_extended_pwm_cfg_t *cfg);
    bool set_period_ms(double ms);
    bool set_period_us(double us);
    bool set_pulse_ms(double ms,uint32_t pin);
    bool set_pulse_us(double us,uint32_t pin);
};


#endif