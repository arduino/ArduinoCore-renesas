#include "analogWave.h"
#include "dac.h"

void analogWave::init(pin_size_t pinNumber) {

  buffer = nullptr;
  size = 0;
  index = 0;
  _amplitude = 1.1;

  dac_bits = 0;

  if (IS_DAC(pinNumber)) {
    auto cfg_dac = getPinCfgs(pinNumber, PIN_CFG_REQ_DAC);
    pin = cfg_dac[0];
    if (IS_DAC_8BIT(pin)) {
      dac_bits = 8;
      dtc_info.transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE;
      if (GET_CHANNEL(pin) == 0) {
#ifdef DAC_ADDRESS_8_CH0
        dtc_info.p_dest = (void *)DAC_ADDRESS_8_CH0;
#endif
      } else if (GET_CHANNEL(pin) == 1) {
#ifdef DAC_ADDRESS_8_CH1
        dtc_info.p_dest = (void *)DAC_ADDRESS_8_CH1;
#endif
      }
    } else {
      dac_bits = 12;
      dtc_info.transfer_settings_word_b.size = TRANSFER_SIZE_2_BYTE;
      if (GET_CHANNEL(pin) == 0) {
#ifdef DAC_ADDRESS_12_CH0
        dtc_info.p_dest = (void *)DAC_ADDRESS_12_CH0;
#endif
      } else if (GET_CHANNEL(pin) == 1) {
#ifdef DAC_ADDRESS_12_CH1
        dtc_info.p_dest = (void *)DAC_ADDRESS_12_CH1;
#endif
      }
    }
  }
  samples8 = nullptr;
  samples16 = nullptr;

  dtc_info.transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED;
  dtc_info.transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE;
  dtc_info.transfer_settings_word_b.irq = TRANSFER_IRQ_END;
  dtc_info.transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED;
  dtc_info.transfer_settings_word_b.src_addr_mode =
      TRANSFER_ADDR_MODE_INCREMENTED;
  dtc_info.transfer_settings_word_b.mode = TRANSFER_MODE_REPEAT;
  // dtc_info.p_dest = (void*) NULL;
  dtc_info.p_src = (void const *)NULL;

  dtc_info.num_blocks = 0;
  dtc_info.length = 0;

  dtc_cfg.p_info = &dtc_info;
  dtc_cfg.p_extend = &dtc_cfg_extend;

  dtc_cfg_extend.activation_source = FSP_INVALID_VECTOR;

  dac_max_samp_rate = 1 / (DAC_MAX_CONVERSION_PERIOD * pow(10, -6));
}

analogWave::analogWave(pin_size_t pinNumber) {
  init(pinNumber);
}

analogWave::analogWave(pin_size_t pinNumber, uint16_t *_buffer, uint32_t _size,
                       uint32_t _offset) {
  init(pinNumber);
  buffer = _buffer;
  size = _size;
  index = _offset;
}

bool analogWave::freq(float freq_hz) {

  freq_max = (dac_max_samp_rate / (double)size);
  if (freq_hz >= freq_max) {
    return false;
  }

  float freq = freq_hz * size;
  float p = 1000000 / freq;

  timer.set_period_us((uint32_t)p);

  return true;
}

bool analogWave::begin(float freq_hz) {

  bool rv = false;
  freq_max = (dac_max_samp_rate / (double)size);
  if (freq_hz >= freq_max) {
    return rv;
  }

  float freq = freq_hz * size;

  /*
   * TIMER INITIALIZATION
   */
  uint8_t type;
  int8_t tindex = FspTimer::get_available_timer(type);

  if (tindex < 0) {
    tindex = FspTimer::get_available_timer(type, true);
  }

  if (tindex >= 0) {
    FspTimer::force_use_of_pwm_reserved_timer();

    // timer.begin(TIMER_MODE_PERIODIC, type, tindex, freq, 50.0, waweform_cb,
    // this);
    timer.begin(TIMER_MODE_PERIODIC, type, tindex, freq, 50.0);
    timer.setup_overflow_irq(WAVE_PRIORITY);
    dtc_cfg_extend.activation_source = timer.get_cfg()->cycle_end_irq;
    timer.open();
    timer.start();
  } else {
    return rv;
  }

  /*
   * DAC INITIALIZATION
   */
  if (IS_DAC_8BIT(pin)) {
#if DAC8_HOWMANY > 0
    if (GET_CHANNEL(pin) < DAC8_HOWMANY) {
      _dac8[GET_CHANNEL(pin)].init();
      rv = true;
    }
#endif
  } else {
    if (GET_CHANNEL(pin) < DAC12_HOWMANY) {
      _dac12[GET_CHANNEL(pin)].init();
      rv = true;
    }
  }

  /*
   *  DTC INITIALIZATION
   */

  R_DTC_Open(&dtc_ctrl, &dtc_cfg);
  R_DTC_Enable(&dtc_ctrl);

  update();

  return rv;
}

template <typename T>
void _update(uint16_t *src, T *dts, float _amp, uint32_t _off, uint32_t _size,
             uint8_t _res) {
  uint16_t src_index = _off;

  for (uint16_t i = 0; i < _size; i++) {
    src_index = src_index % _size;
    uint16_t sample = *(src + src_index);
    src_index++;

    if (_amp < 1) {
      sample = (uint16_t)((float)sample * _amp);
    }
    T value = (T)map(sample, 0, (1 << 16), 0, (1 << _res));
    *(dts + i) = value;
  }
}

bool analogWave::update() {

  uint8_t *local_samples8 = nullptr;
  uint16_t *local_samples16 = nullptr;

  /*
   * calculate updated values on a new allocated buffer
   */

  if (dac_bits == 8) {
    local_samples8 = new uint8_t[size];
    if (local_samples8 == nullptr) {
      return false;
    }
    _update<uint8_t>(buffer, local_samples8, _amplitude, _offset, size, 8);
  } else if (dac_bits == 12) {

    local_samples16 = new uint16_t[size];
    if (local_samples16 == nullptr) {
      return false;
    }
    _update<uint16_t>(buffer, local_samples16, _amplitude, _offset, size, 12);
  }

  stop();

  if (samples8 != local_samples8) {
    if (samples8 != nullptr)
      delete[] samples8;
    samples8 = local_samples8;
  }

  if (samples16 != local_samples16) {
    if (samples16 != nullptr)
      delete[] samples16;
    samples16 = local_samples16;
  }
  _pre_sync();

  start();
  return true;
}

uint32_t analogWave::num_of_sample() {
  return size;
}

void analogWave::amplitude(float a) {
  if (a >= 0 && a <= 1) {
    _amplitude = a;
  }
  update();
}

void analogWave::offset(uint32_t o) {
  if (o < size) {
    _offset = o;
  }
  update();
}

void analogWave::_pre_sync() {
  if (dac_bits == 8 && samples8 != nullptr) {
    dtc_cfg.p_info->p_src = samples8;
  } else if (dac_bits == 12 && samples16 != nullptr) {
    dtc_cfg.p_info->p_src = samples16;
  }
  dtc_cfg.p_info->length = size;

  R_DTC_Reconfigure(&dtc_ctrl, dtc_cfg.p_info);
}

void analogWave::sync(analogWave &w) {
  w.stop();
  stop();

  w._pre_sync();
  _pre_sync();

  synchronized {
    w.start();
    start();
  }
}

void analogWave::start() {
  timer.start();
}

void analogWave::stop() {
  timer.stop();
}

static constexpr std::array<uint16_t, SAMPLES_FOR_PREDEFINED_WAVE> getSine() {
  std::array<uint16_t, SAMPLES_FOR_PREDEFINED_WAVE> a{};

  float step = (2 * 3.1415) / SAMPLES_FOR_PREDEFINED_WAVE;

  for (size_t i = 0; i < a.size(); ++i) {
    float sample = (1 + sin((float)i * step)) * (float)65535 / 2.0;
    a[i] = (uint16_t)sample;
  }
  return a;
}

static const std::array<uint16_t, SAMPLES_FOR_PREDEFINED_WAVE> _sinewave =
    getSine();

static constexpr std::array<uint16_t, SAMPLES_FOR_PREDEFINED_WAVE> getSaw() {
  std::array<uint16_t, SAMPLES_FOR_PREDEFINED_WAVE> a{};

  float step = 65535 / (SAMPLES_FOR_PREDEFINED_WAVE - 1);

  for (size_t i = 0; i < a.size(); ++i) {
    float sample = (float)i * step;
    a[i] = (uint16_t)sample;
  }
  return a;
}

static const std::array<uint16_t, SAMPLES_FOR_PREDEFINED_WAVE> _sawwave =
    getSaw();

static constexpr auto getSquare() {

  std::array<uint16_t, SAMPLES_FOR_PREDEFINED_WAVE> a{};

  for (size_t i = 0; i < a.size(); ++i) {
    if (i < SAMPLES_FOR_PREDEFINED_WAVE / 2) {
      a[i] = (uint16_t)65535;
    } else {
      a[i] = 0;
    }
  }
  return a;
}

static const std::array<uint16_t, SAMPLES_FOR_PREDEFINED_WAVE> _squarewave =
    getSquare();

void analogWave::sine(float freq_hz) {

  buffer = (uint16_t *)_sinewave.data();
  size = _sinewave.size();
  index = 0;
  begin(freq_hz);
}

void analogWave::saw(float freq_hz) {

  buffer = (uint16_t *)_sawwave.data();
  size = _sawwave.size();
  index = 0;
  begin(freq_hz);
}

void analogWave::square(float freq_hz) {

  buffer = (uint16_t *)_squarewave.data();
  size = _squarewave.size();
  index = 0;
  begin(freq_hz);
}
