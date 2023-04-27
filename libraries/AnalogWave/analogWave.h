#ifndef ARDUINO_ANALOG_WAVE
#define ARDUINO_ANALOG_WAVE

#include "FspTimer.h"
#include "r_dtc.h"

#define SAMPLES_FOR_PREDEFINED_WAVE 24
#define WAVE_PRIORITY  1

class analogWave {
private:
  FspTimer timer;
  volatile uint32_t pin;
  uint16_t *buffer;

  uint8_t dac_bits;
  uint8_t *samples8;
  uint16_t *samples16;
  uint32_t size;
  volatile uint32_t index;
  volatile float _amplitude;
  volatile uint32_t _offset;

  double freq_max;
  double dac_max_samp_rate;

  dtc_instance_ctrl_t dtc_ctrl;
  transfer_info_t dtc_info;
  dtc_extended_cfg_t dtc_cfg_extend;
  transfer_cfg_t dtc_cfg;

  void init(pin_size_t p);
  /* update the output with the next sample value */
  bool update();

public:
  /* constructor 1 with only DAC pin (to be used when predefined waveform are used) */
  analogWave(pin_size_t pinNumber);
  /* constructor 2 with buffer to be used (sample in buffer are output ciclycally */
  analogWave(pin_size_t pinNumber, uint16_t *_buffer, uint32_t _size, uint32_t _offset);
  
  /* begin the output of the waweform the frequency is the one the complete
     set on sample are output (all samples will be output in 1 period */
  bool begin(float freq_hz);
  /* updated the frequency used */
  bool freq(float freq_hz);
  /* make the current index in the buffer to "jump" of the specified quantity */
  void offset(uint32_t o);
  /* start the generation of sample */
  void start();
  /* stop the genration of sample */
  void stop();
  /* every sample is multiplied for this factor must be a value between 0 and 1 */
  void amplitude(float amplitude);
  /* get the number of sample used */
  uint32_t num_of_sample();

  void _pre_sync();

  void sync(analogWave &w);

  void sine(float freq_hz);
  void square(float freq_hz);
  void saw(float freq_hz);
};

#endif