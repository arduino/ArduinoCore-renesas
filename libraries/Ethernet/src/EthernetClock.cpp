#include "EthernetClock.h"
#include "pins_arduino.h"

#if defined(ETHERNET_CLK_PIN)

EthernetClock::EthernetClock() {
  pinPeripheral(ETHERNET_CLK_PIN, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_AGT));

  this->TIMER_ETHERNET_extend.count_source = AGT_CLOCK_PCLKB;
  this->TIMER_ETHERNET_extend.agto = AGT_PIN_CFG_START_LEVEL_LOW;
  this->TIMER_ETHERNET_extend.agtoab_settings_b.agtoa = AGT_PIN_CFG_DISABLED;
  this->TIMER_ETHERNET_extend.agtoab_settings_b.agtob = AGT_PIN_CFG_DISABLED;
  this->TIMER_ETHERNET_extend.measurement_mode = AGT_MEASURE_DISABLED;
  this->TIMER_ETHERNET_extend.agtio_filter = AGT_AGTIO_FILTER_NONE;
  this->TIMER_ETHERNET_extend.enable_pin = AGT_ENABLE_PIN_NOT_USED;
  this->TIMER_ETHERNET_extend.trigger_edge = AGT_TRIGGER_EDGE_RISING;

  this->TIMER_ETHERNET_cfg.mode = TIMER_MODE_PERIODIC;
  this->TIMER_ETHERNET_cfg.period_counts = (uint32_t) 0x1;
  this->TIMER_ETHERNET_cfg.duty_cycle_counts = 0x00;
  this->TIMER_ETHERNET_cfg.source_div = (timer_source_div_t) 0;
  this->TIMER_ETHERNET_cfg.channel = ETHERNET_AGT_TIMER_CHANNEL;
  this->TIMER_ETHERNET_cfg.p_callback = NULL;
  this->TIMER_ETHERNET_cfg.p_context  = NULL;
  this->TIMER_ETHERNET_cfg.p_extend = &TIMER_ETHERNET_extend;
  this->TIMER_ETHERNET_cfg.cycle_end_ipl = (BSP_IRQ_DISABLED);
  this->TIMER_ETHERNET_cfg.cycle_end_irq = FSP_INVALID_VECTOR;
}

fsp_err_t EthernetClock::start() {
  fsp_err_t err = R_AGT_Open(&this->TIMER_ETHERNET_ctrl,&this->TIMER_ETHERNET_cfg);
  if (err != FSP_SUCCESS) {
    return err;
  }
  err = R_AGT_Enable(&this->TIMER_ETHERNET_ctrl);
  if (err != FSP_SUCCESS) {
    return err;
  }
  err = R_AGT_Start(&this->TIMER_ETHERNET_ctrl);
  if (err != FSP_SUCCESS) {
    return err;
  }

  FspTimer::set_timer_is_used(AGT_TIMER, ETHERNET_AGT_TIMER_CHANNEL);
  return err;
}

fsp_err_t EthernetClock::stop() {
  fsp_err_t err = R_AGT_Stop(&this->TIMER_ETHERNET_ctrl);
  if (err != FSP_SUCCESS) {
    return err;
  } else {
    err = R_AGT_Close(&this->TIMER_ETHERNET_ctrl);
    if (err != FSP_SUCCESS) {
      return err;
    } else {
      err = R_AGT_Disable(&this->TIMER_ETHERNET_ctrl);
      if (err != FSP_SUCCESS) {
        return err;
      }
    }
  }
}

#else

EthernetClock::EthernetClock() {
}

fsp_err_t EthernetClock::start() {
  return FSP_SUCCESS;
}

fsp_err_t EthernetClock::stop() {
  return FSP_SUCCESS;
}

#endif