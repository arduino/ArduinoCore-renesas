/* The MIT License (MIT)
 *
 * Copyright (c) 2022 Arduino SA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef __SOFTWARE_SERIAL_H__
#define __SOFTWARE_SERIAL_H__
#include "Printable.h"
#include "FspTimer.h"
#include "RingBuffer.h"

#define SS_MAX_DATA_BITS    (8)
// Max bits per frame :      start + data[0........n] + <parity> + stop + <stop>
#define SS_MAX_FRAME_SIZE   (  1   + SS_MAX_DATA_BITS +    1     +  1   +   1  )
#define SS_ARRAY_SIZE(a)    (sizeof(a) / sizeof(a[0]))

typedef FspTimer fsp_tim_t;
typedef bsp_io_port_pin_t fsp_pin_t;

typedef struct {
    transfer_info_t info;
    transfer_cfg_t cfg;
    transfer_instance_t inst;
    dmac_instance_ctrl_t ctrl;
    dmac_extended_cfg_t extend;
} fsp_dma_t;

typedef struct {
    uint8_t  parity;
    uint8_t  polarity;
    uint8_t  bitshift;
    uint8_t  databits;
    uint8_t  stopbits;
    uint32_t baudrate;
    uint32_t nsamples;
} ss_config_t;

template <size_t N> struct ss_descr_t {
    fsp_tim_t tim;
    fsp_dma_t dma;
    fsp_pin_t pin;
    int irq_chan;
    ::RingBuffer<char> ringbuf;
    uint32_t dmabuf[N][SS_MAX_FRAME_SIZE] __attribute__((aligned(4)));
    ss_descr_t(size_t bufsize): irq_chan(-1), ringbuf(bufsize) {
    }
};

class SoftwareSerial : public Stream, public arduino::Printable {
    private:
        ss_config_t config;
        ss_descr_t<1> tx_descr;
        ss_descr_t<2> rx_descr;
        uint8_t _tx_pin, _rx_pin;
        void rx_process();

    public:
        using Print::write;
        friend void dma_tx_callback(dmac_callback_args_t *args);
        friend void dma_rx_callback(dmac_callback_args_t *args);
        SoftwareSerial(uint8_t rx_pin, uint8_t tx_pin, size_t bufsize=1024);
        virtual ~SoftwareSerial();
        virtual size_t printTo(Print& p) const;
        int begin(uint32_t baudrate, uint32_t sconfig=SERIAL_8N1, bool inverted=false);
        virtual int read();
        int peek();
        virtual size_t write(uint8_t byte);
        virtual int available();
};

#endif  //__SOFTWARE_SERIAL_H__
