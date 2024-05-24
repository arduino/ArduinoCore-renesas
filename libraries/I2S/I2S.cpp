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

#include "I2S.h"

I2SClass I2S;

extern "C" i2s_cfg_t g_i2s0_cfg;
extern "C" ssi_instance_ctrl_t g_i2s0_ctrl;
extern "C" void i2s_callback(i2s_callback_args_t *p_args);

int I2SClass::start_transfer() {
    if (!rx_buf && (i2s_mode & I2S_MODE_IN)) {
        rx_buf = rx_pool->alloc(DMA_BUFFER_WRITE);
    }

    if (!tx_buf && (i2s_mode & I2S_MODE_OUT)) {
        tx_buf = tx_pool->alloc(DMA_BUFFER_READ);
    }

    // Start I2S DMA.
    if (i2s_mode == I2S_MODE_IN) {
        if (R_SSI_Read(&g_i2s0_ctrl, (void*) rx_buf->data(), rx_buf->bytes()) != FSP_SUCCESS) {
            return 0;
        }
    } else if (i2s_mode == I2S_MODE_OUT) {
        if (R_SSI_Write(&g_i2s0_ctrl, (void*) tx_buf->data(), tx_buf->bytes()) != FSP_SUCCESS) {
            return 0;
        }
    } else {
        if (R_SSI_WriteRead(&g_i2s0_ctrl, (void*) tx_buf->data(),
                    (void*) rx_buf->data(), tx_buf->bytes()) != FSP_SUCCESS) {
            return 0;
        }
    }
    return 1;
}

int I2SClass::begin(uint32_t i2s_mode, uint32_t sample_rate, size_t n_samples, size_t n_buffers) {
    this->i2s_mode = i2s_mode;

    IRQManager::getInstance().addPeripheral(IRQ_I2S, &g_i2s0_cfg);
    if (R_SSI_Open(&g_i2s0_ctrl, &g_i2s0_cfg) != FSP_SUCCESS) {
        return false;
    }
    
    if (R_SSI_CallbackSet(&g_i2s0_ctrl, i2s_callback, this, NULL) != FSP_SUCCESS) {
        return false;
    }

    // Internal AUDIO_CLK from GPT channel.
    // Need to find the timer connected to GPT_TIMER 2A for internal clock
    auto pwm = new PwmOut(64);
    pwm->begin(50, 25, true);

    // Configure I/Os.
    pinPeripheral(BSP_IO_PORT_01_PIN_12, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SSI));
    pinPeripheral(BSP_IO_PORT_01_PIN_13, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SSI));
    pinPeripheral(BSP_IO_PORT_01_PIN_14, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SSI));
    pinPeripheral(BSP_IO_PORT_01_PIN_15, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SSI));

    if (i2s_mode & I2S_MODE_IN) {
        // Allocate RX buffer pool.
        if (!(rx_pool = new DMAPool<Sample>(n_samples, 2, n_buffers))) {
            return 0;
        }
    }

    if (i2s_mode & I2S_MODE_OUT) {
        // Allocate TX buffer pool.
        if (!(tx_pool = new DMAPool<Sample>(n_samples, 2, n_buffers))) {
            return 0;
        }
    }

    if (i2s_mode == I2S_MODE_IN) {
        return start_transfer();
    }

    if (i2s_mode == I2S_MODE_INOUT) {
        // The transmit pool has to be primed with a few buffers first,
        // before the transfer can be started in full-duplex mode.
        for (int i=0; i<3; i++) {
            SampleBuffer outbuf = dequeue();
            memset(outbuf.data(), 0, outbuf.bytes());
            write(outbuf);
        }
    }

    return 1;
}

bool I2SClass::available() {
    if (rx_pool && i2s_mode == I2S_MODE_IN) {
        return rx_pool->readable();
    } else if (tx_pool && i2s_mode == I2S_MODE_OUT) {
        return tx_pool->writable();
    } else if (tx_pool && rx_pool) {
        return rx_pool->readable() && tx_pool->writable();
    }
    return false;
}

SampleBuffer I2SClass::read() {
    while (!rx_pool->readable()) {
        __WFI();
    }
    return *rx_pool->alloc(DMA_BUFFER_READ);
}

SampleBuffer I2SClass::dequeue() {
    while (!tx_pool->writable()) {
        __WFI();
    }
    return *tx_pool->alloc(DMA_BUFFER_WRITE);
}

void I2SClass::write(SampleBuffer buf) {
    static uint32_t buf_count = 0;
    if (tx_pool == nullptr) {
        return;
    }

    buf.flush();
    buf.release();

    if (tx_buf == nullptr && (++buf_count % 3) == 0) {
        start_transfer();
    }
}

extern "C" void i2s_callback(i2s_callback_args_t *p_args) {
    if (p_args == NULL) {
        return;
    }

    i2s_event_t i2s_event = p_args->event;
    I2SClass *i2s = (I2SClass *) p_args->p_context;

    if (i2s_event == I2S_EVENT_TX_EMPTY) {
        // Release the current buffer and get the next one.
        if (i2s->tx_pool->readable()) {
            i2s->tx_buf->release();
            i2s->tx_buf = i2s->tx_pool->alloc(DMA_BUFFER_READ);
        } else {
            // TODO stop/restart
        }
        R_SSI_Write(&g_i2s0_ctrl, (void*) i2s->tx_buf->data(), i2s->tx_buf->bytes());
    }

    if (i2s_event == I2S_EVENT_RX_FULL) {
        // Update the buffer's timestamp.
        //i2s->rx_buf->timestamp(us_ticker_read());
        if (i2s->rx_pool->writable()) {
            // Move current DMA buffer to ready queue.
            i2s->rx_buf->release();
            // Allocate a new free buffer.
            i2s->rx_buf = i2s->rx_pool->alloc(DMA_BUFFER_WRITE);
            // Currently, all multi-channel buffers are interleaved.
            if (i2s->rx_buf->channels() > 1) {
                i2s->rx_buf->set_flags(DMA_BUFFER_INTRLVD);
            }
        } else {
            i2s->rx_buf->set_flags(DMA_BUFFER_DISCONT);
        }
        R_SSI_Read(&g_i2s0_ctrl, (void*) i2s->rx_buf->data(), i2s->rx_buf->bytes());
    }
}
