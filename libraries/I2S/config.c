#include "r_i2s_api.h"
#include "r_ssi.h"
#include "r_dtc.h"
dtc_instance_ctrl_t g_transfer0_ctrl;

transfer_info_t g_transfer0_info = {
    .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
    .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
    .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
    .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
    .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
    .transfer_settings_word_b.size = TRANSFER_SIZE_4_BYTE,
    .transfer_settings_word_b.mode = TRANSFER_MODE_BLOCK,
    .p_dest = (void*) NULL,
    .p_src = (void const*) NULL,
    .num_blocks = 0,
    .length = 0,
};

const dtc_extended_cfg_t g_transfer0_cfg_extend ={
    .activation_source = VECTOR_NUMBER_SSI0_TXI,
};

const transfer_cfg_t g_transfer0_cfg = {
    .p_info = &g_transfer0_info,
    .p_extend = &g_transfer0_cfg_extend
};

/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 = {
    .p_ctrl = &g_transfer0_ctrl,
    .p_cfg = &g_transfer0_cfg,
    .p_api = &g_transfer_on_dtc
};
ssi_instance_ctrl_t g_i2s0_ctrl;

/** SSI instance configuration */
const ssi_extended_cfg_t g_i2s0_cfg_extend = {
    .audio_clock = SSI_AUDIO_CLOCK_INTERNAL,
    .bit_clock_div = SSI_CLOCK_DIV_1
};

/** I2S interface configuration */
const i2s_cfg_t g_i2s0_cfg = {
    .channel = 0,
    .pcm_width = I2S_PCM_WIDTH_16_BITS,
    //.pcm_width = I2S_PCM_WIDTH_32_BITS,
    .operating_mode = I2S_MODE_MASTER,
    .word_length = I2S_WORD_LENGTH_32_BITS,
    .ws_continue = I2S_WS_CONTINUE_ON,
    .p_callback = NULL,
    .p_context = NULL,
    .p_extend = &g_i2s0_cfg_extend,
    .txi_irq = FSP_INVALID_VECTOR,
    .rxi_irq = FSP_INVALID_VECTOR,
    .int_irq = FSP_INVALID_VECTOR,
    .txi_ipl = (2),
    .rxi_ipl = (2),
    .idle_err_ipl = (2),
    .p_transfer_tx = NULL,  // MAY be null
    .p_transfer_rx = NULL,
};

/* Instance structure to use this module. */
const i2s_instance_t g_i2s0 = {
    .p_ctrl = &g_i2s0_ctrl,
    .p_cfg = &g_i2s0_cfg,
    .p_api = &g_i2s_on_ssi 
};
