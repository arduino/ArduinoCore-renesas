/* generated common source file - do not edit */
#include "common_data.h"
dmac_instance_ctrl_t g_transfer0_ctrl;
transfer_info_t g_transfer0_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_4_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 128, };
const dmac_extended_cfg_t g_transfer0_extend =
        { .offset = 1, .src_buffer_size = 1,
#if defined(VECTOR_NUMBER_DMAC2_INT)
    .irq                 = VECTOR_NUMBER_DMAC2_INT,
#else
          .irq = FSP_INVALID_VECTOR,
#endif
          .ipl = (2),
          .channel = 2, .p_callback = g_sdmmc0_dmac_callback, .p_context = NULL, .activation_source =
                  ELC_EVENT_SDHIMMC0_DMA_REQ, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info, .p_extend = &g_transfer0_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dmac };
#define RA_NOT_DEFINED (UINT32_MAX)
#if (RA_NOT_DEFINED) != (1)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
#include "r_dmac.h"
extern void r_sdhi_transfer_callback(sdhi_instance_ctrl_t *p_ctrl);

void g_sdmmc0_dmac_callback(dmac_callback_args_t *p_args)
{
    r_sdhi_transfer_callback ((sdhi_instance_ctrl_t*) p_args->p_context);
}
#endif
#undef RA_NOT_DEFINED

sdhi_instance_ctrl_t g_sdmmc0_ctrl;
sdmmc_cfg_t g_sdmmc0_cfg =
{ .bus_width = SDMMC_BUS_WIDTH_4_BITS, .channel = 0, .p_callback = rm_block_media_sdmmc_callback, .p_context =
          &g_rm_block_media0_ctrl,
  .block_size = 512, .card_detect = SDMMC_CARD_DETECT_NONE, .write_protect = SDMMC_WRITE_PROTECT_NONE,

  .p_extend = NULL,
  .p_lower_lvl_transfer = &g_transfer0,

  .access_ipl = (2),
  .sdio_ipl = BSP_IRQ_DISABLED, .card_ipl = (2), .dma_req_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_SDHIMMC0_ACCS)
    .access_irq             = VECTOR_NUMBER_SDHIMMC0_ACCS,
#else
  .access_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SDHIMMC0_CARD)
    .card_irq               = VECTOR_NUMBER_SDHIMMC0_CARD,
#else
  .card_irq = FSP_INVALID_VECTOR,
#endif
  .sdio_irq = FSP_INVALID_VECTOR,
#if defined(VECTOR_NUMBER_SDHIMMC0_DMA_REQ)
    .dma_req_irq            = VECTOR_NUMBER_SDHIMMC0_DMA_REQ,
#else
  .dma_req_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const sdmmc_instance_t g_sdmmc0 =
{ .p_ctrl = &g_sdmmc0_ctrl, .p_cfg = &g_sdmmc0_cfg, .p_api = &g_sdmmc_on_sdhi };
const rm_block_media_sdmmc_extended_cfg_t g_rm_block_media0_cfg_extend =
{ .p_sdmmc = &g_sdmmc0, };
const rm_block_media_cfg_t g_rm_block_media0_cfg =
{ .p_extend = &g_rm_block_media0_cfg_extend, .p_callback = rm_filex_block_media_memory_callback, .p_context =
          &g_rm_filex_block_media_0_ctrl, };
rm_block_media_sdmmc_instance_ctrl_t g_rm_block_media0_ctrl;
const rm_block_media_instance_t g_rm_block_media0 =
{ .p_api = &g_rm_block_media_on_sdmmc, .p_ctrl = &g_rm_block_media0_ctrl, .p_cfg = &g_rm_block_media0_cfg, };
rm_filex_block_media_instance_ctrl_t g_rm_filex_block_media_0_ctrl;

const rm_filex_block_media_cfg_t g_rm_filex_block_media_0_cfg =
{ .p_lower_lvl_block_media = (rm_block_media_instance_t*) &g_rm_block_media0, .partition =
          RM_FILEX_BLOCK_MEDIA_PARTITION0,
  .p_callback = g_rm_filex_block_media_0_callback };

const rm_filex_block_media_instance_t g_rm_filex_block_media_0_instance =
{ .p_ctrl = &g_rm_filex_block_media_0_ctrl, .p_cfg = &g_rm_filex_block_media_0_cfg, .p_api = &g_filex_on_block_media };
const cgc_cfg_t g_cgc0_cfg =
{ .p_callback = NULL, };
cgc_instance_ctrl_t g_cgc0_ctrl;
const cgc_instance_t g_cgc0 =
{ .p_api = &g_cgc_on_cgc, .p_ctrl = &g_cgc0_ctrl, .p_cfg = &g_cgc0_cfg, };
ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_ctrl = &g_ioport_ctrl, .p_cfg = &g_bsp_pin_cfg, };
TX_EVENT_FLAGS_GROUP g_cdcacm_event_flags0;
void tx_startup_err_callback(void *p_instance, void *p_data);
void g_common_init(void)
{
    UINT err_g_cdcacm_event_flags0;
    err_g_cdcacm_event_flags0 = tx_event_flags_create (&g_cdcacm_event_flags0, (CHAR*) "New Event Flags");
    if (TX_SUCCESS != err_g_cdcacm_event_flags0)
    {
        tx_startup_err_callback (&g_cdcacm_event_flags0, 0);
    }
}
