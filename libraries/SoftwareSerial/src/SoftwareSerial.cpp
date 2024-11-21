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
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <IRQManager.h>

#define SS_DEBUG            (0)
#define SS_DEBUG_PIN        BSP_IO_PORT_01_PIN_03

#define SS_PARITY_NONE      (0)
#define SS_PARITY_ODD       (1)
#define SS_PARITY_EVEN      (2)

#define SS_ARRAY_SIZE(a)    (sizeof(a) / sizeof(a[0]))
#define SS_PORT_ADDR(pn)    ((uint32_t) (R_PORT1 - R_PORT0) * (pn) + R_PORT0)

// NOTE: DMA channels priorities are hard-wired, channel 0 > 1 > 2 etc.
// so the RX channel is given a higher priority.
// TODO: The core doesn't support allocating DMA channels, so these are
// hard-coded for now.
#define SS_DMA_CHANNEL_RX   (0)
#define SS_DMA_CHANNEL_TX   (1)

static elc_instance_ctrl_t elc_ctrl;
static const elc_cfg_t elc_cfg = { {ELC_EVENT_NONE}, };

void dma_tx_callback(dmac_callback_args_t *args);
void dma_rx_callback(dmac_callback_args_t *args);

extern int attachIrq2Link(uint32_t pin, PinStatus mode);
typedef void (*fsp_dma_callback_t) (dmac_callback_args_t *args);

static uint32_t tx_get_sample(bsp_io_port_pin_t tx, ioport_size_t value)
{
    ioport_size_t shift = (tx & 0xFF);
    ioport_size_t mask = (1 << shift);
    value <<= shift;
    return (uint32_t) ((((uint32_t) ((~value) & mask)) << 16) | (value & mask));
}

static elc_event_t fsp_irq_to_elc_event(int channel)
{
    static elc_event_t irq_chan_to_elc_event[] = {
        ELC_EVENT_ICU_IRQ0,
        ELC_EVENT_ICU_IRQ1,
        ELC_EVENT_ICU_IRQ2,
        ELC_EVENT_ICU_IRQ3,
        ELC_EVENT_ICU_IRQ4,
        ELC_EVENT_ICU_IRQ5,
        ELC_EVENT_ICU_IRQ6,
        ELC_EVENT_ICU_IRQ7,
        ELC_EVENT_ICU_IRQ8,
        ELC_EVENT_ICU_IRQ9,
        ELC_EVENT_ICU_IRQ10,
        ELC_EVENT_ICU_IRQ11,
        ELC_EVENT_ICU_IRQ12,
        ELC_EVENT_ICU_IRQ13,
        ELC_EVENT_ICU_IRQ14,
        ELC_EVENT_ICU_IRQ15,
    };

    elc_event_t event = ELC_EVENT_NONE;
    if (channel >= 0 && channel < (int)SS_ARRAY_SIZE(irq_chan_to_elc_event)) {
        event = irq_chan_to_elc_event[channel];
    }
    return event;
}

static elc_event_t fsp_tim_to_elc_event(int channel)
{
    static elc_event_t tim_chan_to_elc_event[] = {
        ELC_EVENT_GPT0_CAPTURE_COMPARE_A,
        ELC_EVENT_GPT1_CAPTURE_COMPARE_A,
        ELC_EVENT_GPT2_CAPTURE_COMPARE_A,
        ELC_EVENT_GPT3_CAPTURE_COMPARE_A,
        ELC_EVENT_GPT4_CAPTURE_COMPARE_A,
        ELC_EVENT_GPT5_CAPTURE_COMPARE_A,
        ELC_EVENT_GPT6_CAPTURE_COMPARE_A,
        ELC_EVENT_GPT7_CAPTURE_COMPARE_A,
    };

    elc_event_t event = ELC_EVENT_NONE;
    if (channel >= 0 && channel < (int)SS_ARRAY_SIZE(tim_chan_to_elc_event)) {
        event = tim_chan_to_elc_event[channel];
    }
    return event;
}

static int fsp_tim_config(fsp_tim_t *timer, uint32_t frequency, bool elc_clrstp)
{
    uint8_t type = 0;
    int8_t channel = FspTimer::get_available_timer(type);
    if (channel >= 0 && channel < 8 && type == GPT_TIMER) {
        // Configure and enable the timer.
        timer->begin(TIMER_MODE_PERIODIC, type,
                channel, frequency, 50.0f);

        if (elc_clrstp
            && timer->set_source_start(GPT_SOURCE_GPT_A) == false) {
            return -1;
        }

        if (elc_clrstp
            && timer->set_source_clear(GPT_SOURCE_GPT_A) == false) {
            return -1;
        }
        timer->open();
        timer->stop();
        return 0;
    }
    return -1;
}

static int fsp_dma_config(fsp_dma_t *dma, uint32_t channel, elc_event_t elc_event,
        void *dmabuf, void *ioport, uint32_t size, fsp_dma_callback_t callback, void *ctx)
{
    // Configure DMA transfer info.
    dma->info.length = size;
    dma->info.num_blocks = 0;
    dma->info.p_src  = (channel == SS_DMA_CHANNEL_RX) ? ioport : dmabuf;
    dma->info.p_dest = (channel == SS_DMA_CHANNEL_TX) ? ioport : dmabuf;
    dma->info.transfer_settings_word_b.irq            = TRANSFER_IRQ_END;
    dma->info.transfer_settings_word_b.mode           = TRANSFER_MODE_NORMAL;
    dma->info.transfer_settings_word_b.size           = TRANSFER_SIZE_4_BYTE;
    dma->info.transfer_settings_word_b.chain_mode     = TRANSFER_CHAIN_MODE_DISABLED;
    dma->info.transfer_settings_word_b.repeat_area    = (channel == SS_DMA_CHANNEL_TX) ?
                TRANSFER_REPEAT_AREA_SOURCE : TRANSFER_REPEAT_AREA_DESTINATION;
    dma->info.transfer_settings_word_b.src_addr_mode  = (channel == SS_DMA_CHANNEL_RX) ?
                TRANSFER_ADDR_MODE_FIXED    : TRANSFER_ADDR_MODE_INCREMENTED;
    dma->info.transfer_settings_word_b.dest_addr_mode = (channel == SS_DMA_CHANNEL_TX) ?
                TRANSFER_ADDR_MODE_FIXED    : TRANSFER_ADDR_MODE_INCREMENTED;

    // Configure DMA extended configuration.
    dma->extend.offset = 1;
    dma->extend.src_buffer_size = 1;
    dma->extend.channel = channel;
    dma->extend.ipl = 12;
    dma->extend.irq = FSP_INVALID_VECTOR;
    dma->extend.p_context = ctx;
    dma->extend.p_callback = callback;
    dma->extend.activation_source = elc_event;

    // Link transfer info and extended config to DMA config.
    dma->cfg.p_info = &dma->info;
    dma->cfg.p_extend = &dma->extend;

    // Link DMA config and control structs to DMA instance.
    dma->inst.p_cfg = &dma->cfg;
    dma->inst.p_ctrl = &dma->ctrl;
    dma->inst.p_api = &g_transfer_on_dmac;

    // Configure and enable the DMA.
    // Configure DMA IRQ from the extended config. This will set the
    // IRQ channel and priority in the config, and vector table entry.
    if (IRQManager::getInstance().addDMA(dma->extend) != true
     || R_DMAC_Open(&dma->ctrl, &dma->cfg) != FSP_SUCCESS
     || R_DMAC_Reconfigure(&dma->ctrl, dma->cfg.p_info) != FSP_SUCCESS) {
        return -1;
    }

    #if (SS_DEBUG > 0)
    printf ("dma %p chan %lu activation %d buf %p port %p size %d cb %p ctx %p\n",
        dma, channel, elc_event, dmabuf, ioport, size, callback, ctx);
    #endif
    return 0;
}

SoftwareSerial::SoftwareSerial(uint8_t rx_pin, uint8_t tx_pin, size_t bufsize):
    config{}, tx_descr(0), rx_descr(bufsize)
{
    assert(tx_pin < NUM_DIGITAL_PINS);
    assert(rx_pin < NUM_DIGITAL_PINS);
    _tx_pin = tx_pin;
    _rx_pin = rx_pin;
}

SoftwareSerial::~SoftwareSerial()
{

}

size_t SoftwareSerial::printTo(Print& p) const
{
    size_t n = 0;
    const char ptostr[] = {'N', 'O', 'E'};

    n += p.print(config.baudrate);
    n += p.print(",");
    n += p.print(config.databits);
    n += p.print(ptostr[config.parity]);
    n += p.print(config.stopbits);
    return n;
}

int SoftwareSerial::begin(uint32_t baudrate, uint32_t sconfig, bool inverted)
{

    // NOTE: The IO port must be opened before calling any R_IOPORT_xxx functions.
    // This sets the following: p_instance_ctrl->open = IOPORT_OPEN; (0x504F5254U)
    // which is "PORT" in ASCII. Uncomment the following line if R_IOPORT_Open is
    // Not called from anywhere else.
    //R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);
    tx_descr.pin = g_pin_cfg[_tx_pin].pin;
    rx_descr.pin = g_pin_cfg[_rx_pin].pin;

    #if (SS_DEBUG > 1)
    R_IOPORT_PinCfg(&g_ioport_ctrl, SS_DEBUG_PIN,
            IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_HIGH);
    #endif

    R_IOPORT_PinCfg(&g_ioport_ctrl, tx_descr.pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT
            | IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_PORT_OUTPUT_HIGH);

    // Enable RX pin IRQ.
    rx_descr.irq_chan = attachIrq2Link(_rx_pin, CHANGE);
    if (rx_descr.irq_chan != -1) {
        // TODO: workaround for the core not setting pull-ups.
        R_IOPORT_PinCfg(&g_ioport_ctrl, rx_descr.pin, IOPORT_CFG_PORT_DIRECTION_INPUT
                | IOPORT_CFG_PULLUP_ENABLE | IOPORT_CFG_IRQ_ENABLE);
    }

    // Set serial configuration.
    config.bitshift = (rx_descr.pin & 0xFF);
    config.polarity = (inverted) ? 1 : 0;
    config.baudrate = baudrate;

	switch (sconfig & SERIAL_DATA_MASK) {
		case SERIAL_DATA_7:
            config.databits = 7;
			break;
        default:
            config.databits = 8;
			break;
	}

	switch (sconfig & SERIAL_STOP_BIT_MASK) {
		case SERIAL_STOP_BIT_2:
            config.stopbits = 2;
			break;
        default:
            config.stopbits = 1;
			break;
	}

	switch (sconfig & SERIAL_PARITY_MASK) {
		case SERIAL_PARITY_ODD:
			config.parity = SS_PARITY_ODD;
			break;
		case SERIAL_PARITY_EVEN:
			config.parity = SS_PARITY_EVEN;
			break;
        default:
			config.parity = SS_PARITY_NONE;
			break;
	}

    // The start and stop samples are always the same, only the data bits samples change.
    // Once the configuration parameters are known, the exact number of TX/RX samples can
    // be calculated, and the start/stop samples can be prepopulated in the DMA TX buffer.
    config.nsamples = (1 + config.databits + (!!config.parity) + config.stopbits);

    // Prepopulate start and stop bits samples.
    tx_descr.dmabuf[0][0] = tx_get_sample(tx_descr.pin, 0);
    for (size_t i=0; i<config.stopbits; i++) {
        tx_descr.dmabuf[0][config.nsamples-1-i] = tx_get_sample(tx_descr.pin, 1);
    }

    // Configure the TX DMA and its tigger timer.
    R_PORT0_Type *tx_port = SS_PORT_ADDR(((tx_descr.pin >> 8) & 0xFF));
    if (fsp_tim_config(&tx_descr.tim, config.baudrate, false) != 0) {
        return 0;
    }

    if (fsp_dma_config(&tx_descr.dma, SS_DMA_CHANNEL_TX,
        fsp_tim_to_elc_event(tx_descr.tim.get_channel()), tx_descr.dmabuf[0],
        (void *) &tx_port->PCNTR3, config.nsamples, dma_tx_callback, this) != 0) {
        return 0;
    }

    // Configure the RX DMA and its trigger timer.
    R_PORT0_Type *rx_port = SS_PORT_ADDR(((rx_descr.pin >> 8) & 0xFF));
    if (fsp_tim_config(&rx_descr.tim, config.baudrate, true) != 0) {
        return 0;
    }

    if (fsp_dma_config(&rx_descr.dma, SS_DMA_CHANNEL_RX,
        fsp_tim_to_elc_event(rx_descr.tim.get_channel()), rx_descr.dmabuf[0],
        (void *) &rx_port->PCNTR2, config.nsamples, dma_rx_callback, this) != 0) {
        return 0;
    }

    // Configure and enable the ELC.
    elc_event_t irq_evt = fsp_irq_to_elc_event(rx_descr.irq_chan);
    if (irq_evt == ELC_EVENT_NONE
        || R_ELC_Open(&elc_ctrl, &elc_cfg) != FSP_SUCCESS
        || R_ELC_LinkSet(&elc_ctrl, ELC_PERIPHERAL_GPT_A, irq_evt) != FSP_SUCCESS
        || R_ELC_Enable(&elc_ctrl) != FSP_SUCCESS) {
        return 0;
    }

    #if (SS_DEBUG > 0)
    const char parity_tostr[] = {'N', 'O', 'E'};
    printf("tx: 0x%x rx: 0x%x shift: %d polarity: %s samples: %d config:%d,%d%c%d\n",
            tx_descr.pin, rx_descr.pin, config.bitshift, (config.polarity) ? "Inverted" : "Normal",
            config.nsamples, config.baudrate, config.databits, parity_tostr[config.parity], config.stopbits);
    #endif
    return 1;
}

int SoftwareSerial::read()
{
    int chr = -1;
    if (!rx_descr.ringbuf.empty()) {
        chr = rx_descr.ringbuf.get();
    }
    return chr;
}

int SoftwareSerial::peek()
{
    int chr = -1;
    if (!rx_descr.ringbuf.empty()) {
        chr = rx_descr.ringbuf.get(true);
    }
    return chr;
}

size_t __attribute__((optimize("O2"))) SoftwareSerial::write(uint8_t byte)
{
    uint8_t parity = (config.parity == SS_PARITY_ODD) ? 1 : 0;

    // Update data bits samples in the TX buffer.
    for (size_t i=0; i<config.databits; i++) {
        uint8_t bit = ((byte >> i) & 1) ^ config.polarity;
        parity ^= bit;
        tx_descr.dmabuf[0][i + 1] = tx_get_sample(tx_descr.pin, bit);
    }

    // Set parity bit;
    if (config.parity != SS_PARITY_NONE) {
       tx_descr.dmabuf[0][config.databits + 1] = parity;
    }

    // Restart DMA transfer.
    R_DMAC_Reset(&tx_descr.dma.ctrl, tx_descr.dma.cfg.p_info->p_src,
            tx_descr.dma.cfg.p_info->p_dest, tx_descr.dma.cfg.p_info->length);

    // Start the DMA transfer.
    tx_descr.tim.reset();
    tx_descr.tim.start();

    // Wait for the DMA transfer to finish.
    while (tx_descr.dma.ctrl.p_reg->DMCNT) {
    }

    // Stop the trigger timer.
    tx_descr.tim.stop();
    return 1;
}

int SoftwareSerial::available()
{
    return rx_descr.ringbuf.size();
}

void __attribute__((optimize("O2"))) SoftwareSerial::rx_process()
{
    static uint32_t bufidx = 0;

    // Restart DMA transfer.
    rx_descr.dma.cfg.p_info->p_dest =  rx_descr.dmabuf[bufidx ^ 1];
    R_DMAC_Reset(&rx_descr.dma.ctrl, rx_descr.dma.cfg.p_info->p_src,
            rx_descr.dma.cfg.p_info->p_dest, rx_descr.dma.cfg.p_info->length);

    // Process the current DMA buffer.
    uint8_t data = 0;
    uint32_t offset = 1; // skip start bit.
    uint32_t *dmabuf = rx_descr.dmabuf[bufidx];

    // Initialize parity so that at the end of the reception we always have a 0.
    uint8_t parity = (config.parity == SS_PARITY_ODD) ? 1 : 0;

    for (size_t i=0; i<config.databits; i++) {
        // Extract bit from sampled port data.
        uint8_t bit = (dmabuf[offset++] >> config.bitshift) & 1;

        // Accumulate parity.
        parity ^= bit;

        // Shift data bits
        data |= ((bit ^ config.polarity) << i);
    }

    // Check parity error.
    if (config.parity != SS_PARITY_NONE) {
        if (parity ^ ((dmabuf[offset++] >> config.bitshift) & 1)) {
            // TODO: Handle parity error.
            #if (SS_DEBUG > 0)
            printf("Parity error!\n");
            #endif
        }
    }

    // Check frame errors.
    for (size_t i=0; i<config.stopbits; i++) {
        if (((dmabuf[offset++] >> config.bitshift) & 1) == 0) {
            // TODO: Handle frame error.
            #if (SS_DEBUG > 0)
            printf("Frame error!\n");
            #endif
            break;
        }
    }

    rx_descr.ringbuf.put(data);

    // Swap DMA buffer index.
    bufidx ^= 1;
}

void dma_tx_callback(dmac_callback_args_t *args)
{
    // Wait for the cycle to finish to keep the GPIO high enough for the last stop bit.
    fsp_tim_t *tim = &(((SoftwareSerial *) args->p_context)->tx_descr.tim);
    volatile R_GPT0_Type *regs = R_GPT0 + (tim->get_channel() * (R_GPT1 - R_GPT0));
    regs->GTST_b.TCFPO = 0;
    while (!regs->GTST_b.TCFPO) {
    }
}

void dma_rx_callback(dmac_callback_args_t *args)
{
    // Disable the DMA trigger timer as soon as the transfer is complete.
    // Note the timer will be cleared and restarted via ELC/software.
    ((SoftwareSerial *) args->p_context)->rx_descr.tim.stop();

    #if (SS_DEBUG > 1)
    R_IOPORT_PinWrite(&g_ioport_ctrl, SS_DEBUG_PIN, BSP_IO_LEVEL_LOW);
    #endif

    // Process the captured DMA buffer.
    ((SoftwareSerial *) args->p_context)->rx_process();

    #if (SS_DEBUG > 1)
    R_IOPORT_PinWrite(&g_ioport_ctrl, SS_DEBUG_PIN, BSP_IO_LEVEL_HIGH);
    #endif
}
