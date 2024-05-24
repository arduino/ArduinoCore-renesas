/*
  This file is part of the Arduino_AdvancedAnalog library.
  Copyright (c) 2024 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef __I2S_H__
#define __I2S_H__

#include "r_i2s_api.h"
#include "r_ssi.h"
#include "pwm.h"
#include "api/DMAPool.h"

typedef uint16_t Sample;
typedef DMABuffer<Sample> &SampleBuffer;

enum {
    I2S_MODE_IN      = (1U << 0U),
    I2S_MODE_OUT     = (1U << 1U),
    I2S_MODE_INOUT   = (I2S_MODE_IN | I2S_MODE_OUT),
};

class I2SClass {
    private:
        uint32_t i2s_mode;
        int start_transfer();

    public:
        DMABuffer<Sample> *tx_buf;
        DMABuffer<Sample> *rx_buf;
        DMAPool<Sample> *tx_pool;
        DMAPool<Sample> *rx_pool;

        I2SClass(): tx_buf(nullptr), rx_buf(nullptr), tx_pool(nullptr), rx_pool(nullptr) {
        }
        bool available();
        SampleBuffer read();
        SampleBuffer dequeue();
        void write(SampleBuffer buf);
        int begin(uint32_t i2s_mode, uint32_t sample_rate, size_t n_samples, size_t n_buffers);
        int stop();
};

extern I2SClass I2S;
#endif // __I2S_H__
