#include "r_agt.h"

static agt_instance_ctrl_t g_timer_ctrl;
static const agt_extended_cfg_t g_timer_extend =
{ .count_source = AGT_CLOCK_PCLKB,
  .agto = AGT_PIN_CFG_DISABLED,
  .agtoab_settings_b.agtoa = AGT_PIN_CFG_DISABLED,
  .agtoab_settings_b.agtob = AGT_PIN_CFG_DISABLED,
  .measurement_mode = AGT_MEASURE_DISABLED,
  .agtio_filter = AGT_AGTIO_FILTER_NONE,
  .enable_pin = AGT_ENABLE_PIN_NOT_USED,
  .trigger_edge = AGT_TRIGGER_EDGE_RISING,
};

static const timer_cfg_t g_timer_cfg =
{ .mode = TIMER_MODE_PWM,
  .period_counts = (uint32_t) 0x1,
  .duty_cycle_counts = 0x0,
  .source_div = (timer_source_div_t) 0,
  .channel = AGT_TIMER_FOR_25MHZ,
  .p_callback = NULL,
  .p_context = NULL,
  .p_extend = &g_timer_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
  .cycle_end_irq = FSP_INVALID_VECTOR,
};

/* Instance structure to use this module. */
const timer_instance_t g_timer_eth =
{ .p_ctrl = &g_timer_ctrl, .p_cfg = &g_timer_cfg, .p_api = &g_timer_on_agt };