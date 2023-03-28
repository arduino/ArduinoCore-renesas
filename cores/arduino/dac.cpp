#include "dac.h"
#include "Arduino.h"
#include "analog.h"
#include "bsp_api.h"
#include "hal_data.h"

CDac::CDac(pin_size_t _dac) {

  auto req = getPinCfgs(_dac, PIN_CFG_REQ_DAC);
  dac = req[0];
  pin = _dac;

#if DAC8_HOWMANY > 0
  /* DAC 8 configuration */
  ext_cfg_dac8.enable_charge_pump = true;
  ext_cfg_dac8.dac_mode = DAC8_MODE_NORMAL;
#endif
  /* DAC 12 configuration */
  ext_cfg_dac.enable_charge_pump = 0;
  ext_cfg_dac.data_format = DAC_DATA_FORMAT_FLUSH_RIGHT;
  ext_cfg_dac.output_amplifier_enabled = 0;
  ext_cfg_dac.internal_output_enabled = false;

  cfg.ad_da_synchronized = false;

  cfg.channel = GET_CHANNEL(dac);

  if (IS_DAC_8BIT(dac)) {
#if DAC8_HOWMANY > 0
    cfg.p_extend = &ext_cfg_dac8;
    open = R_DAC8_Open;
    close = R_DAC8_Close;
    write = R_DAC8_Write;
    start = R_DAC8_Start;
    stop = R_DAC8_Stop;
#endif
  } else {
    cfg.p_extend = &ext_cfg_dac;
    open = R_DAC_Open;
    close = R_DAC_Close;
    write = R_DAC_Write;
    start = R_DAC_Start;
    stop = R_DAC_Stop;
  }
}

void CDac::set(uint16_t value) {
  if (IS_DAC_8BIT(dac)) {
#if DAC8_HOWMANY > 0
    value = map(value, 0, (1 << 16), 0, (1 << 8));
    write(&ctrl_dac8, value);
#endif
  } else {
    value = map(value, 0, (1 << 16), 0, (1 << 12));
    write(&ctrl_dac, value);
  }
}

void CDac::init() {
  R_IOPORT_PinCfg(NULL, g_pin_cfg[pin].pin,
                  (uint32_t)((uint32_t)IOPORT_CFG_ANALOG_ENABLE |
                             (uint32_t)IOPORT_CFG_PERIPHERAL_PIN |
                             (uint32_t)IOPORT_PERIPHERAL_CAC_AD));

  if (IS_DAC_8BIT(dac)) {
#if DAC8_HOWMANY > 0
    open(&ctrl_dac8, &cfg);
    start(&ctrl_dac8);
#endif
  } else {
    open(&ctrl_dac, &cfg);
    start(&ctrl_dac);
  }
}

void CDac::analogWrite(int value) {

  if (value > (1 << analogWriteResolution())) {
    value = (1 << analogWriteResolution());
  }

  if (!pin_init) {
    pin_init = true;
    R_IOPORT_PinCfg(NULL, g_pin_cfg[pin].pin,
                    (uint32_t)((uint32_t)IOPORT_CFG_ANALOG_ENABLE |
                               (uint32_t)IOPORT_CFG_PERIPHERAL_PIN |
                               (uint32_t)IOPORT_PERIPHERAL_CAC_AD));
  }

  if (IS_DAC_8BIT(dac)) {
#if DAC8_HOWMANY > 0
    value = map(value, 0, (1 << analogWriteResolution()), 0, (1 << 8));
    open(&ctrl_dac8, &cfg);
    write(&ctrl_dac8, value);
    start(&ctrl_dac8);
#endif
  } else {
    value = map(value, 0, (1 << analogWriteResolution()), 0, (1 << 12));
    open(&ctrl_dac, &cfg);
    write(&ctrl_dac, value);
    start(&ctrl_dac);
  }
}

#if DAC12_HOWMANY > 0
CDac _dac12[] = {
#ifdef DAC
    CDac(DAC),
#endif

#ifdef DAC1
    CDac(DAC1)
#endif
};
#endif

#if DAC8_HOWMANY > 0
CDac _dac8[] = {
#ifdef DAC2
    CDac(DAC2),
#endif

#ifdef DAC3
    CDac(DAC3)
#endif
};
#endif