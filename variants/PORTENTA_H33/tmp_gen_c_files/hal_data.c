/* generated HAL source file - do not edit */
#include "hal_data.h"
/* Macros to tie dynamic ELC links to ADC_TRIGGER_SYNC_ELC option in adc_trigger_t. */
#define ADC_TRIGGER_ADC0        ADC_TRIGGER_SYNC_ELC
#define ADC_TRIGGER_ADC0_B      ADC_TRIGGER_SYNC_ELC
#define ADC_TRIGGER_ADC1        ADC_TRIGGER_SYNC_ELC
#define ADC_TRIGGER_ADC1_B      ADC_TRIGGER_SYNC_ELC

dtc_instance_ctrl_t g_transfer74_ctrl;

transfer_info_t g_transfer74_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer74_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI9_RXI, };
const transfer_cfg_t g_transfer74_cfg =
{ .p_info = &g_transfer74_info, .p_extend = &g_transfer74_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer74 =
{ .p_ctrl = &g_transfer74_ctrl, .p_cfg = &g_transfer74_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer73_ctrl;

transfer_info_t g_transfer73_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer73_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI9_TXI, };
const transfer_cfg_t g_transfer73_cfg =
{ .p_info = &g_transfer73_info, .p_extend = &g_transfer73_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer73 =
{ .p_ctrl = &g_transfer73_ctrl, .p_cfg = &g_transfer73_cfg, .p_api = &g_transfer_on_dtc };
sci_spi_instance_ctrl_t g_spi11_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi11_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 1000000. */.cks = 0,
  .brr = 24, .mddr = 0, } };

const spi_cfg_t g_spi11_cfg =
{ .channel = 9, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer73)
    .p_transfer_tx   = NULL,
#else
  .p_transfer_tx = &g_transfer73,
#endif
#if (RA_NOT_DEFINED == g_transfer74)
    .p_transfer_rx   = NULL,
#else
  .p_transfer_rx = &g_transfer74,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI9_RXI, .txi_irq = VECTOR_NUMBER_SCI9_TXI, .tei_irq =
          VECTOR_NUMBER_SCI9_TEI,
  .eri_irq = VECTOR_NUMBER_SCI9_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi11_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi11 =
{ .p_ctrl = &g_spi11_ctrl, .p_cfg = &g_spi11_cfg, .p_api = &g_spi_on_sci };
dtc_instance_ctrl_t g_transfer72_ctrl;

transfer_info_t g_transfer72_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer72_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI8_RXI, };
const transfer_cfg_t g_transfer72_cfg =
{ .p_info = &g_transfer72_info, .p_extend = &g_transfer72_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer72 =
{ .p_ctrl = &g_transfer72_ctrl, .p_cfg = &g_transfer72_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer71_ctrl;

transfer_info_t g_transfer71_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer71_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI8_TXI, };
const transfer_cfg_t g_transfer71_cfg =
{ .p_info = &g_transfer71_info, .p_extend = &g_transfer71_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer71 =
{ .p_ctrl = &g_transfer71_ctrl, .p_cfg = &g_transfer71_cfg, .p_api = &g_transfer_on_dtc };
sci_spi_instance_ctrl_t g_spi10_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi10_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 1000000. */.cks = 0,
  .brr = 24, .mddr = 0, } };

const spi_cfg_t g_spi10_cfg =
{ .channel = 8, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer71)
    .p_transfer_tx   = NULL,
#else
  .p_transfer_tx = &g_transfer71,
#endif
#if (RA_NOT_DEFINED == g_transfer72)
    .p_transfer_rx   = NULL,
#else
  .p_transfer_rx = &g_transfer72,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI8_RXI, .txi_irq = VECTOR_NUMBER_SCI8_TXI, .tei_irq =
          VECTOR_NUMBER_SCI8_TEI,
  .eri_irq = VECTOR_NUMBER_SCI8_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi10_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi10 =
{ .p_ctrl = &g_spi10_ctrl, .p_cfg = &g_spi10_cfg, .p_api = &g_spi_on_sci };
dtc_instance_ctrl_t g_transfer70_ctrl;

transfer_info_t g_transfer70_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer70_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI7_RXI, };
const transfer_cfg_t g_transfer70_cfg =
{ .p_info = &g_transfer70_info, .p_extend = &g_transfer70_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer70 =
{ .p_ctrl = &g_transfer70_ctrl, .p_cfg = &g_transfer70_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer69_ctrl;

transfer_info_t g_transfer69_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer69_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI7_TXI, };
const transfer_cfg_t g_transfer69_cfg =
{ .p_info = &g_transfer69_info, .p_extend = &g_transfer69_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer69 =
{ .p_ctrl = &g_transfer69_ctrl, .p_cfg = &g_transfer69_cfg, .p_api = &g_transfer_on_dtc };
sci_spi_instance_ctrl_t g_spi9_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi9_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 1000000. */.cks = 0,
  .brr = 24, .mddr = 0, } };

const spi_cfg_t g_spi9_cfg =
{ .channel = 7, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer69)
    .p_transfer_tx   = NULL,
#else
  .p_transfer_tx = &g_transfer69,
#endif
#if (RA_NOT_DEFINED == g_transfer70)
    .p_transfer_rx   = NULL,
#else
  .p_transfer_rx = &g_transfer70,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI7_RXI, .txi_irq = VECTOR_NUMBER_SCI7_TXI, .tei_irq =
          VECTOR_NUMBER_SCI7_TEI,
  .eri_irq = VECTOR_NUMBER_SCI7_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi9_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi9 =
{ .p_ctrl = &g_spi9_ctrl, .p_cfg = &g_spi9_cfg, .p_api = &g_spi_on_sci };
dtc_instance_ctrl_t g_transfer68_ctrl;

transfer_info_t g_transfer68_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer68_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI6_RXI, };
const transfer_cfg_t g_transfer68_cfg =
{ .p_info = &g_transfer68_info, .p_extend = &g_transfer68_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer68 =
{ .p_ctrl = &g_transfer68_ctrl, .p_cfg = &g_transfer68_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer67_ctrl;

transfer_info_t g_transfer67_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer67_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI6_TXI, };
const transfer_cfg_t g_transfer67_cfg =
{ .p_info = &g_transfer67_info, .p_extend = &g_transfer67_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer67 =
{ .p_ctrl = &g_transfer67_ctrl, .p_cfg = &g_transfer67_cfg, .p_api = &g_transfer_on_dtc };
sci_spi_instance_ctrl_t g_spi8_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi8_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 1000000. */.cks = 0,
  .brr = 24, .mddr = 0, } };

const spi_cfg_t g_spi8_cfg =
{ .channel = 6, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer67)
    .p_transfer_tx   = NULL,
#else
  .p_transfer_tx = &g_transfer67,
#endif
#if (RA_NOT_DEFINED == g_transfer68)
    .p_transfer_rx   = NULL,
#else
  .p_transfer_rx = &g_transfer68,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI6_RXI, .txi_irq = VECTOR_NUMBER_SCI6_TXI, .tei_irq =
          VECTOR_NUMBER_SCI6_TEI,
  .eri_irq = VECTOR_NUMBER_SCI6_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi8_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi8 =
{ .p_ctrl = &g_spi8_ctrl, .p_cfg = &g_spi8_cfg, .p_api = &g_spi_on_sci };
dtc_instance_ctrl_t g_transfer66_ctrl;

transfer_info_t g_transfer66_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer66_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI5_RXI, };
const transfer_cfg_t g_transfer66_cfg =
{ .p_info = &g_transfer66_info, .p_extend = &g_transfer66_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer66 =
{ .p_ctrl = &g_transfer66_ctrl, .p_cfg = &g_transfer66_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer65_ctrl;

transfer_info_t g_transfer65_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer65_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI5_TXI, };
const transfer_cfg_t g_transfer65_cfg =
{ .p_info = &g_transfer65_info, .p_extend = &g_transfer65_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer65 =
{ .p_ctrl = &g_transfer65_ctrl, .p_cfg = &g_transfer65_cfg, .p_api = &g_transfer_on_dtc };
sci_spi_instance_ctrl_t g_spi7_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi7_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 1000000. */.cks = 0,
  .brr = 24, .mddr = 0, } };

const spi_cfg_t g_spi7_cfg =
{ .channel = 5, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer65)
    .p_transfer_tx   = NULL,
#else
  .p_transfer_tx = &g_transfer65,
#endif
#if (RA_NOT_DEFINED == g_transfer66)
    .p_transfer_rx   = NULL,
#else
  .p_transfer_rx = &g_transfer66,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI5_RXI, .txi_irq = VECTOR_NUMBER_SCI5_TXI, .tei_irq =
          VECTOR_NUMBER_SCI5_TEI,
  .eri_irq = VECTOR_NUMBER_SCI5_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi7_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi7 =
{ .p_ctrl = &g_spi7_ctrl, .p_cfg = &g_spi7_cfg, .p_api = &g_spi_on_sci };
dtc_instance_ctrl_t g_transfer64_ctrl;

transfer_info_t g_transfer64_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer64_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI3_RXI, };
const transfer_cfg_t g_transfer64_cfg =
{ .p_info = &g_transfer64_info, .p_extend = &g_transfer64_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer64 =
{ .p_ctrl = &g_transfer64_ctrl, .p_cfg = &g_transfer64_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer63_ctrl;

transfer_info_t g_transfer63_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer63_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI3_TXI, };
const transfer_cfg_t g_transfer63_cfg =
{ .p_info = &g_transfer63_info, .p_extend = &g_transfer63_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer63 =
{ .p_ctrl = &g_transfer63_ctrl, .p_cfg = &g_transfer63_cfg, .p_api = &g_transfer_on_dtc };
sci_spi_instance_ctrl_t g_spi6_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi6_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 1000000. */.cks = 0,
  .brr = 24, .mddr = 0, } };

const spi_cfg_t g_spi6_cfg =
{ .channel = 3, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer63)
    .p_transfer_tx   = NULL,
#else
  .p_transfer_tx = &g_transfer63,
#endif
#if (RA_NOT_DEFINED == g_transfer64)
    .p_transfer_rx   = NULL,
#else
  .p_transfer_rx = &g_transfer64,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI3_RXI, .txi_irq = VECTOR_NUMBER_SCI3_TXI, .tei_irq =
          VECTOR_NUMBER_SCI3_TEI,
  .eri_irq = VECTOR_NUMBER_SCI3_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi6_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi6 =
{ .p_ctrl = &g_spi6_ctrl, .p_cfg = &g_spi6_cfg, .p_api = &g_spi_on_sci };
dtc_instance_ctrl_t g_transfer62_ctrl;

transfer_info_t g_transfer62_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer62_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI2_RXI, };
const transfer_cfg_t g_transfer62_cfg =
{ .p_info = &g_transfer62_info, .p_extend = &g_transfer62_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer62 =
{ .p_ctrl = &g_transfer62_ctrl, .p_cfg = &g_transfer62_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer61_ctrl;

transfer_info_t g_transfer61_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer61_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI2_TXI, };
const transfer_cfg_t g_transfer61_cfg =
{ .p_info = &g_transfer61_info, .p_extend = &g_transfer61_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer61 =
{ .p_ctrl = &g_transfer61_ctrl, .p_cfg = &g_transfer61_cfg, .p_api = &g_transfer_on_dtc };
sci_spi_instance_ctrl_t g_spi5_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi5_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 1000000. */.cks = 0,
  .brr = 24, .mddr = 0, } };

const spi_cfg_t g_spi5_cfg =
{ .channel = 2, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer61)
    .p_transfer_tx   = NULL,
#else
  .p_transfer_tx = &g_transfer61,
#endif
#if (RA_NOT_DEFINED == g_transfer62)
    .p_transfer_rx   = NULL,
#else
  .p_transfer_rx = &g_transfer62,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI2_RXI, .txi_irq = VECTOR_NUMBER_SCI2_TXI, .tei_irq =
          VECTOR_NUMBER_SCI2_TEI,
  .eri_irq = VECTOR_NUMBER_SCI2_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi5_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi5 =
{ .p_ctrl = &g_spi5_ctrl, .p_cfg = &g_spi5_cfg, .p_api = &g_spi_on_sci };
dtc_instance_ctrl_t g_transfer60_ctrl;

transfer_info_t g_transfer60_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer60_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI1_RXI, };
const transfer_cfg_t g_transfer60_cfg =
{ .p_info = &g_transfer60_info, .p_extend = &g_transfer60_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer60 =
{ .p_ctrl = &g_transfer60_ctrl, .p_cfg = &g_transfer60_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer59_ctrl;

transfer_info_t g_transfer59_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer59_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI1_TXI, };
const transfer_cfg_t g_transfer59_cfg =
{ .p_info = &g_transfer59_info, .p_extend = &g_transfer59_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer59 =
{ .p_ctrl = &g_transfer59_ctrl, .p_cfg = &g_transfer59_cfg, .p_api = &g_transfer_on_dtc };
sci_spi_instance_ctrl_t g_spi4_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi4_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 1000000. */.cks = 0,
  .brr = 24, .mddr = 0, } };

const spi_cfg_t g_spi4_cfg =
{ .channel = 1, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer59)
    .p_transfer_tx   = NULL,
#else
  .p_transfer_tx = &g_transfer59,
#endif
#if (RA_NOT_DEFINED == g_transfer60)
    .p_transfer_rx   = NULL,
#else
  .p_transfer_rx = &g_transfer60,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI1_RXI, .txi_irq = VECTOR_NUMBER_SCI1_TXI, .tei_irq =
          VECTOR_NUMBER_SCI1_TEI,
  .eri_irq = VECTOR_NUMBER_SCI1_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi4_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi4 =
{ .p_ctrl = &g_spi4_ctrl, .p_cfg = &g_spi4_cfg, .p_api = &g_spi_on_sci };
dtc_instance_ctrl_t g_transfer58_ctrl;

transfer_info_t g_transfer58_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer58_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI4_RXI, };
const transfer_cfg_t g_transfer58_cfg =
{ .p_info = &g_transfer58_info, .p_extend = &g_transfer58_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer58 =
{ .p_ctrl = &g_transfer58_ctrl, .p_cfg = &g_transfer58_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer57_ctrl;

transfer_info_t g_transfer57_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer57_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI4_TXI, };
const transfer_cfg_t g_transfer57_cfg =
{ .p_info = &g_transfer57_info, .p_extend = &g_transfer57_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer57 =
{ .p_ctrl = &g_transfer57_ctrl, .p_cfg = &g_transfer57_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart9_ctrl;

baud_setting_t g_uart9_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart9_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart9_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart9_cfg =
{ .channel = 4, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          uart_callback,
  .p_context = NULL, .p_extend = &g_uart9_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer57)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer57,
#endif
#if (RA_NOT_DEFINED == g_transfer58)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer58,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI4_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI4_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI4_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI4_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI4_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart9 =
{ .p_ctrl = &g_uart9_ctrl, .p_cfg = &g_uart9_cfg, .p_api = &g_uart_on_sci };
dtc_instance_ctrl_t g_transfer56_ctrl;

transfer_info_t g_transfer56_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer56_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI3_RXI, };
const transfer_cfg_t g_transfer56_cfg =
{ .p_info = &g_transfer56_info, .p_extend = &g_transfer56_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer56 =
{ .p_ctrl = &g_transfer56_ctrl, .p_cfg = &g_transfer56_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer55_ctrl;

transfer_info_t g_transfer55_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer55_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI3_TXI, };
const transfer_cfg_t g_transfer55_cfg =
{ .p_info = &g_transfer55_info, .p_extend = &g_transfer55_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer55 =
{ .p_ctrl = &g_transfer55_ctrl, .p_cfg = &g_transfer55_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart8_ctrl;

baud_setting_t g_uart8_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart8_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart8_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart8_cfg =
{ .channel = 3, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          uart_callback,
  .p_context = NULL, .p_extend = &g_uart8_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer55)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer55,
#endif
#if (RA_NOT_DEFINED == g_transfer56)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer56,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI3_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI3_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI3_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI3_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI3_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart8 =
{ .p_ctrl = &g_uart8_ctrl, .p_cfg = &g_uart8_cfg, .p_api = &g_uart_on_sci };
dtc_instance_ctrl_t g_transfer54_ctrl;

transfer_info_t g_transfer54_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer54_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI2_RXI, };
const transfer_cfg_t g_transfer54_cfg =
{ .p_info = &g_transfer54_info, .p_extend = &g_transfer54_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer54 =
{ .p_ctrl = &g_transfer54_ctrl, .p_cfg = &g_transfer54_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer53_ctrl;

transfer_info_t g_transfer53_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer53_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI2_TXI, };
const transfer_cfg_t g_transfer53_cfg =
{ .p_info = &g_transfer53_info, .p_extend = &g_transfer53_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer53 =
{ .p_ctrl = &g_transfer53_ctrl, .p_cfg = &g_transfer53_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart7_ctrl;

baud_setting_t g_uart7_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart7_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart7_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart7_cfg =
{ .channel = 2, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          uart_callback,
  .p_context = NULL, .p_extend = &g_uart7_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer53)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer53,
#endif
#if (RA_NOT_DEFINED == g_transfer54)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer54,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI2_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI2_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI2_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI2_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI2_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart7 =
{ .p_ctrl = &g_uart7_ctrl, .p_cfg = &g_uart7_cfg, .p_api = &g_uart_on_sci };
dtc_instance_ctrl_t g_transfer52_ctrl;

transfer_info_t g_transfer52_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer52_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI1_RXI, };
const transfer_cfg_t g_transfer52_cfg =
{ .p_info = &g_transfer52_info, .p_extend = &g_transfer52_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer52 =
{ .p_ctrl = &g_transfer52_ctrl, .p_cfg = &g_transfer52_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer51_ctrl;

transfer_info_t g_transfer51_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer51_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI1_TXI, };
const transfer_cfg_t g_transfer51_cfg =
{ .p_info = &g_transfer51_info, .p_extend = &g_transfer51_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer51 =
{ .p_ctrl = &g_transfer51_ctrl, .p_cfg = &g_transfer51_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart6_ctrl;

baud_setting_t g_uart6_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart6_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart6_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart6_cfg =
{ .channel = 1, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          uart_callback,
  .p_context = NULL, .p_extend = &g_uart6_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer51)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer51,
#endif
#if (RA_NOT_DEFINED == g_transfer52)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer52,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI1_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI1_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI1_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI1_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI1_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart6 =
{ .p_ctrl = &g_uart6_ctrl, .p_cfg = &g_uart6_cfg, .p_api = &g_uart_on_sci };
dtc_instance_ctrl_t g_transfer50_ctrl;

transfer_info_t g_transfer50_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer50_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI9_RXI, };
const transfer_cfg_t g_transfer50_cfg =
{ .p_info = &g_transfer50_info, .p_extend = &g_transfer50_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer50 =
{ .p_ctrl = &g_transfer50_ctrl, .p_cfg = &g_transfer50_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer49_ctrl;

transfer_info_t g_transfer49_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer49_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI9_TXI, };
const transfer_cfg_t g_transfer49_cfg =
{ .p_info = &g_transfer49_info, .p_extend = &g_transfer49_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer49 =
{ .p_ctrl = &g_transfer49_ctrl, .p_cfg = &g_transfer49_cfg, .p_api = &g_transfer_on_dtc };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c12_ctrl;
const sci_i2c_extended_cfg_t g_i2c12_cfg_extend =
        {
        /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value = 0,
          .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value =
                  30,
          .clock_settings.snfr_value = (1), };

const i2c_master_cfg_t g_i2c12_cfg =
{ .channel = 9, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer49)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer49,
#endif
#if (RA_NOT_DEFINED == g_transfer50)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer50,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI9_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI9_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI9_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI9_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12), /* (12) is unused */
  .p_extend = &g_i2c12_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c12 =
{ .p_ctrl = &g_i2c12_ctrl, .p_cfg = &g_i2c12_cfg, .p_api = &g_i2c_master_on_sci };
dtc_instance_ctrl_t g_transfer48_ctrl;

transfer_info_t g_transfer48_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer48_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI8_RXI, };
const transfer_cfg_t g_transfer48_cfg =
{ .p_info = &g_transfer48_info, .p_extend = &g_transfer48_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer48 =
{ .p_ctrl = &g_transfer48_ctrl, .p_cfg = &g_transfer48_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer47_ctrl;

transfer_info_t g_transfer47_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer47_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI8_TXI, };
const transfer_cfg_t g_transfer47_cfg =
{ .p_info = &g_transfer47_info, .p_extend = &g_transfer47_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer47 =
{ .p_ctrl = &g_transfer47_ctrl, .p_cfg = &g_transfer47_cfg, .p_api = &g_transfer_on_dtc };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c11_ctrl;
const sci_i2c_extended_cfg_t g_i2c11_cfg_extend =
        {
        /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value = 0,
          .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value =
                  30,
          .clock_settings.snfr_value = (1), };

const i2c_master_cfg_t g_i2c11_cfg =
{ .channel = 8, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer47)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer47,
#endif
#if (RA_NOT_DEFINED == g_transfer48)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer48,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI8_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI8_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI8_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI8_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI8_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI8_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12), /* (12) is unused */
  .p_extend = &g_i2c11_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c11 =
{ .p_ctrl = &g_i2c11_ctrl, .p_cfg = &g_i2c11_cfg, .p_api = &g_i2c_master_on_sci };
dtc_instance_ctrl_t g_transfer46_ctrl;

transfer_info_t g_transfer46_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer46_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI7_RXI, };
const transfer_cfg_t g_transfer46_cfg =
{ .p_info = &g_transfer46_info, .p_extend = &g_transfer46_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer46 =
{ .p_ctrl = &g_transfer46_ctrl, .p_cfg = &g_transfer46_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer45_ctrl;

transfer_info_t g_transfer45_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer45_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI7_TXI, };
const transfer_cfg_t g_transfer45_cfg =
{ .p_info = &g_transfer45_info, .p_extend = &g_transfer45_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer45 =
{ .p_ctrl = &g_transfer45_ctrl, .p_cfg = &g_transfer45_cfg, .p_api = &g_transfer_on_dtc };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c10_ctrl;
const sci_i2c_extended_cfg_t g_i2c10_cfg_extend =
        {
        /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value = 0,
          .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value =
                  30,
          .clock_settings.snfr_value = (1), };

const i2c_master_cfg_t g_i2c10_cfg =
{ .channel = 7, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer45)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer45,
#endif
#if (RA_NOT_DEFINED == g_transfer46)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer46,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI7_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI7_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI7_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI7_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12), /* (12) is unused */
  .p_extend = &g_i2c10_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c10 =
{ .p_ctrl = &g_i2c10_ctrl, .p_cfg = &g_i2c10_cfg, .p_api = &g_i2c_master_on_sci };
dtc_instance_ctrl_t g_transfer44_ctrl;

transfer_info_t g_transfer44_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer44_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI6_RXI, };
const transfer_cfg_t g_transfer44_cfg =
{ .p_info = &g_transfer44_info, .p_extend = &g_transfer44_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer44 =
{ .p_ctrl = &g_transfer44_ctrl, .p_cfg = &g_transfer44_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer43_ctrl;

transfer_info_t g_transfer43_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer43_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI6_TXI, };
const transfer_cfg_t g_transfer43_cfg =
{ .p_info = &g_transfer43_info, .p_extend = &g_transfer43_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer43 =
{ .p_ctrl = &g_transfer43_ctrl, .p_cfg = &g_transfer43_cfg, .p_api = &g_transfer_on_dtc };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c9_ctrl;
const sci_i2c_extended_cfg_t g_i2c9_cfg_extend =
        {
        /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value = 0,
          .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value =
                  30,
          .clock_settings.snfr_value = (1), };

const i2c_master_cfg_t g_i2c9_cfg =
{ .channel = 6, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer43)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer43,
#endif
#if (RA_NOT_DEFINED == g_transfer44)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer44,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI6_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI6_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI6_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI6_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12), /* (12) is unused */
  .p_extend = &g_i2c9_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c9 =
{ .p_ctrl = &g_i2c9_ctrl, .p_cfg = &g_i2c9_cfg, .p_api = &g_i2c_master_on_sci };
dtc_instance_ctrl_t g_transfer42_ctrl;

transfer_info_t g_transfer42_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer42_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI5_RXI, };
const transfer_cfg_t g_transfer42_cfg =
{ .p_info = &g_transfer42_info, .p_extend = &g_transfer42_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer42 =
{ .p_ctrl = &g_transfer42_ctrl, .p_cfg = &g_transfer42_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer41_ctrl;

transfer_info_t g_transfer41_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer41_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI5_TXI, };
const transfer_cfg_t g_transfer41_cfg =
{ .p_info = &g_transfer41_info, .p_extend = &g_transfer41_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer41 =
{ .p_ctrl = &g_transfer41_ctrl, .p_cfg = &g_transfer41_cfg, .p_api = &g_transfer_on_dtc };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c8_ctrl;
const sci_i2c_extended_cfg_t g_i2c8_cfg_extend =
        {
        /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value = 0,
          .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value =
                  30,
          .clock_settings.snfr_value = (1), };

const i2c_master_cfg_t g_i2c8_cfg =
{ .channel = 5, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer41)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer41,
#endif
#if (RA_NOT_DEFINED == g_transfer42)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer42,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI5_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI5_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI5_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI5_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI5_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI5_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12), /* (12) is unused */
  .p_extend = &g_i2c8_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c8 =
{ .p_ctrl = &g_i2c8_ctrl, .p_cfg = &g_i2c8_cfg, .p_api = &g_i2c_master_on_sci };
dtc_instance_ctrl_t g_transfer40_ctrl;

transfer_info_t g_transfer40_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer40_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI4_RXI, };
const transfer_cfg_t g_transfer40_cfg =
{ .p_info = &g_transfer40_info, .p_extend = &g_transfer40_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer40 =
{ .p_ctrl = &g_transfer40_ctrl, .p_cfg = &g_transfer40_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer39_ctrl;

transfer_info_t g_transfer39_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer39_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI4_TXI, };
const transfer_cfg_t g_transfer39_cfg =
{ .p_info = &g_transfer39_info, .p_extend = &g_transfer39_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer39 =
{ .p_ctrl = &g_transfer39_ctrl, .p_cfg = &g_transfer39_cfg, .p_api = &g_transfer_on_dtc };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c7_ctrl;
const sci_i2c_extended_cfg_t g_i2c7_cfg_extend =
        {
        /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value = 0,
          .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value =
                  30,
          .clock_settings.snfr_value = (1), };

const i2c_master_cfg_t g_i2c7_cfg =
{ .channel = 4, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer39)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer39,
#endif
#if (RA_NOT_DEFINED == g_transfer40)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer40,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI4_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI4_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI4_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI4_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI4_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12), /* (12) is unused */
  .p_extend = &g_i2c7_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c7 =
{ .p_ctrl = &g_i2c7_ctrl, .p_cfg = &g_i2c7_cfg, .p_api = &g_i2c_master_on_sci };
dtc_instance_ctrl_t g_transfer38_ctrl;

transfer_info_t g_transfer38_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer38_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI0_RXI, };
const transfer_cfg_t g_transfer38_cfg =
{ .p_info = &g_transfer38_info, .p_extend = &g_transfer38_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer38 =
{ .p_ctrl = &g_transfer38_ctrl, .p_cfg = &g_transfer38_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer37_ctrl;

transfer_info_t g_transfer37_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer37_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI0_TXI, };
const transfer_cfg_t g_transfer37_cfg =
{ .p_info = &g_transfer37_info, .p_extend = &g_transfer37_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer37 =
{ .p_ctrl = &g_transfer37_ctrl, .p_cfg = &g_transfer37_cfg, .p_api = &g_transfer_on_dtc };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c6_ctrl;
const sci_i2c_extended_cfg_t g_i2c6_cfg_extend =
        {
        /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value = 0,
          .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value =
                  30,
          .clock_settings.snfr_value = (1), };

const i2c_master_cfg_t g_i2c6_cfg =
{ .channel = 0, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer37)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer37,
#endif
#if (RA_NOT_DEFINED == g_transfer38)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer38,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI0_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI0_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12), /* (12) is unused */
  .p_extend = &g_i2c6_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c6 =
{ .p_ctrl = &g_i2c6_ctrl, .p_cfg = &g_i2c6_cfg, .p_api = &g_i2c_master_on_sci };
dtc_instance_ctrl_t g_transfer36_ctrl;

transfer_info_t g_transfer36_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer36_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI0_RXI, };
const transfer_cfg_t g_transfer36_cfg =
{ .p_info = &g_transfer36_info, .p_extend = &g_transfer36_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer36 =
{ .p_ctrl = &g_transfer36_ctrl, .p_cfg = &g_transfer36_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer35_ctrl;

transfer_info_t g_transfer35_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer35_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI0_TXI, };
const transfer_cfg_t g_transfer35_cfg =
{ .p_info = &g_transfer35_info, .p_extend = &g_transfer35_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer35 =
{ .p_ctrl = &g_transfer35_ctrl, .p_cfg = &g_transfer35_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart5_ctrl;

baud_setting_t g_uart5_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart5_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart5_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart5_cfg =
{ .channel = 0, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          uart_callback,
  .p_context = NULL, .p_extend = &g_uart5_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer35)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer35,
#endif
#if (RA_NOT_DEFINED == g_transfer36)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer36,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI0_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI0_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI0_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart5 =
{ .p_ctrl = &g_uart5_ctrl, .p_cfg = &g_uart5_cfg, .p_api = &g_uart_on_sci };
dtc_instance_ctrl_t g_transfer34_ctrl;

transfer_info_t g_transfer34_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer34_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI3_RXI, };
const transfer_cfg_t g_transfer34_cfg =
{ .p_info = &g_transfer34_info, .p_extend = &g_transfer34_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer34 =
{ .p_ctrl = &g_transfer34_ctrl, .p_cfg = &g_transfer34_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer33_ctrl;

transfer_info_t g_transfer33_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer33_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI3_TXI, };
const transfer_cfg_t g_transfer33_cfg =
{ .p_info = &g_transfer33_info, .p_extend = &g_transfer33_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer33 =
{ .p_ctrl = &g_transfer33_ctrl, .p_cfg = &g_transfer33_cfg, .p_api = &g_transfer_on_dtc };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c5_ctrl;
const sci_i2c_extended_cfg_t g_i2c5_cfg_extend =
        {
        /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value = 0,
          .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value =
                  30,
          .clock_settings.snfr_value = (1), };

const i2c_master_cfg_t g_i2c5_cfg =
{ .channel = 3, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer33)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer33,
#endif
#if (RA_NOT_DEFINED == g_transfer34)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer34,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI3_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI3_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI3_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI3_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI3_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12), /* (12) is unused */
  .p_extend = &g_i2c5_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c5 =
{ .p_ctrl = &g_i2c5_ctrl, .p_cfg = &g_i2c5_cfg, .p_api = &g_i2c_master_on_sci };
dtc_instance_ctrl_t g_transfer32_ctrl;

transfer_info_t g_transfer32_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer32_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI2_RXI, };
const transfer_cfg_t g_transfer32_cfg =
{ .p_info = &g_transfer32_info, .p_extend = &g_transfer32_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer32 =
{ .p_ctrl = &g_transfer32_ctrl, .p_cfg = &g_transfer32_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer31_ctrl;

transfer_info_t g_transfer31_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer31_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI2_TXI, };
const transfer_cfg_t g_transfer31_cfg =
{ .p_info = &g_transfer31_info, .p_extend = &g_transfer31_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer31 =
{ .p_ctrl = &g_transfer31_ctrl, .p_cfg = &g_transfer31_cfg, .p_api = &g_transfer_on_dtc };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c4_ctrl;
const sci_i2c_extended_cfg_t g_i2c4_cfg_extend =
        {
        /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value = 0,
          .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value =
                  30,
          .clock_settings.snfr_value = (1), };

const i2c_master_cfg_t g_i2c4_cfg =
{ .channel = 2, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer31)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer31,
#endif
#if (RA_NOT_DEFINED == g_transfer32)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer32,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI2_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI2_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI2_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI2_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI2_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12), /* (12) is unused */
  .p_extend = &g_i2c4_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c4 =
{ .p_ctrl = &g_i2c4_ctrl, .p_cfg = &g_i2c4_cfg, .p_api = &g_i2c_master_on_sci };
dtc_instance_ctrl_t g_transfer30_ctrl;

transfer_info_t g_transfer30_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer30_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI1_RXI, };
const transfer_cfg_t g_transfer30_cfg =
{ .p_info = &g_transfer30_info, .p_extend = &g_transfer30_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer30 =
{ .p_ctrl = &g_transfer30_ctrl, .p_cfg = &g_transfer30_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer29_ctrl;

transfer_info_t g_transfer29_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer29_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI1_TXI, };
const transfer_cfg_t g_transfer29_cfg =
{ .p_info = &g_transfer29_info, .p_extend = &g_transfer29_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer29 =
{ .p_ctrl = &g_transfer29_ctrl, .p_cfg = &g_transfer29_cfg, .p_api = &g_transfer_on_dtc };
#include "r_sci_i2c_cfg.h"
sci_i2c_instance_ctrl_t g_i2c3_ctrl;
const sci_i2c_extended_cfg_t g_i2c3_cfg_extend =
        {
        /* Actual calculated bitrate: 99981. Actual SDA delay: 300 ns. */.clock_settings.clk_divisor_value = 0,
          .clock_settings.brr_value = 20, .clock_settings.mddr_value = 172, .clock_settings.bitrate_modulation = true, .clock_settings.cycles_value =
                  30,
          .clock_settings.snfr_value = (1), };

const i2c_master_cfg_t g_i2c3_cfg =
{ .channel = 1, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x00, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer29)
    .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer29,
#endif
#if (RA_NOT_DEFINED == g_transfer30)
    .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer30,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_SCI1_RXI) && SCI_I2C_CFG_DTC_ENABLE
    .rxi_irq             = VECTOR_NUMBER_SCI1_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_TXI)
    .txi_irq             = VECTOR_NUMBER_SCI1_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI1_TEI)
    .tei_irq             = VECTOR_NUMBER_SCI1_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12), /* (12) is unused */
  .p_extend = &g_i2c3_cfg_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c3 =
{ .p_ctrl = &g_i2c3_ctrl, .p_cfg = &g_i2c3_cfg, .p_api = &g_i2c_master_on_sci };
dtc_instance_ctrl_t g_transfer28_ctrl;

transfer_info_t g_transfer28_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer28_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI0_RXI, };
const transfer_cfg_t g_transfer28_cfg =
{ .p_info = &g_transfer28_info, .p_extend = &g_transfer28_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer28 =
{ .p_ctrl = &g_transfer28_ctrl, .p_cfg = &g_transfer28_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer27_ctrl;

transfer_info_t g_transfer27_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer27_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI0_TXI, };
const transfer_cfg_t g_transfer27_cfg =
{ .p_info = &g_transfer27_info, .p_extend = &g_transfer27_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer27 =
{ .p_ctrl = &g_transfer27_ctrl, .p_cfg = &g_transfer27_cfg, .p_api = &g_transfer_on_dtc };
sci_spi_instance_ctrl_t g_spi3_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi3_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 1000000. */.cks = 0,
  .brr = 24, .mddr = 0, } };

const spi_cfg_t g_spi3_cfg =
{ .channel = 0, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer27)
    .p_transfer_tx   = NULL,
#else
  .p_transfer_tx = &g_transfer27,
#endif
#if (RA_NOT_DEFINED == g_transfer28)
    .p_transfer_rx   = NULL,
#else
  .p_transfer_rx = &g_transfer28,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI0_RXI, .txi_irq = VECTOR_NUMBER_SCI0_TXI, .tei_irq =
          VECTOR_NUMBER_SCI0_TEI,
  .eri_irq = VECTOR_NUMBER_SCI0_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi3_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi3 =
{ .p_ctrl = &g_spi3_ctrl, .p_cfg = &g_spi3_cfg, .p_api = &g_spi_on_sci };
dtc_instance_ctrl_t g_transfer26_ctrl;

transfer_info_t g_transfer26_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_2_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer26_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SPI0_RXI, };
const transfer_cfg_t g_transfer26_cfg =
{ .p_info = &g_transfer26_info, .p_extend = &g_transfer26_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer26 =
{ .p_ctrl = &g_transfer26_ctrl, .p_cfg = &g_transfer26_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer14_ctrl;

transfer_info_t g_transfer14_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_2_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer14_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SPI0_TXI, };
const transfer_cfg_t g_transfer14_cfg =
{ .p_info = &g_transfer14_info, .p_extend = &g_transfer14_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer14 =
{ .p_ctrl = &g_transfer14_ctrl, .p_cfg = &g_transfer14_cfg, .p_api = &g_transfer_on_dtc };
spi_instance_ctrl_t g_spi0_ctrl;

/** SPI extended configuration for SPI HAL driver */
const spi_extended_cfg_t g_spi0_ext_cfg =
{ .spi_clksyn = SPI_SSL_MODE_CLK_SYN,
  .spi_comm = SPI_COMMUNICATION_FULL_DUPLEX,
  .ssl_polarity = SPI_SSLP_LOW,
  .ssl_select = SPI_SSL_SELECT_SSL0,
  .mosi_idle = SPI_MOSI_IDLE_VALUE_FIXING_DISABLE,
  .parity = SPI_PARITY_MODE_DISABLE,
  .byte_swap = SPI_BYTE_SWAP_DISABLE,
  .spck_div =
  {
  /* Actual calculated bitrate: 1000000. */.spbr = 49,
    .brdv = 0 },
  .spck_delay = SPI_DELAY_COUNT_1,
  .ssl_negation_delay = SPI_DELAY_COUNT_1,
  .next_access_delay = SPI_DELAY_COUNT_1 };

/** SPI configuration for SPI HAL driver */
const spi_cfg_t g_spi0_cfg =
{ .channel = 0,

#if defined(VECTOR_NUMBER_SPI0_RXI)
    .rxi_irq             = VECTOR_NUMBER_SPI0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI0_TXI)
    .txi_irq             = VECTOR_NUMBER_SPI0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI0_TEI)
    .tei_irq             = VECTOR_NUMBER_SPI0_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI0_ERI)
    .eri_irq             = VECTOR_NUMBER_SPI0_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif

  .rxi_ipl = (12),
  .txi_ipl = (12),
  .tei_ipl = (12),
  .eri_ipl = (12),

  .operating_mode = SPI_MODE_MASTER,

  .clk_phase = SPI_CLK_PHASE_EDGE_ODD,
  .clk_polarity = SPI_CLK_POLARITY_LOW,

  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE,
  .bit_order = SPI_BIT_ORDER_MSB_FIRST,
  .p_transfer_tx = g_spi0_P_TRANSFER_TX,
  .p_transfer_rx = g_spi0_P_TRANSFER_RX,
  .p_callback = spi_callback,

  .p_context = NULL,
  .p_extend = (void*) &g_spi0_ext_cfg, };

/* Instance structure to use this module. */
const spi_instance_t g_spi0 =
{ .p_ctrl = &g_spi0_ctrl, .p_cfg = &g_spi0_cfg, .p_api = &g_spi_on_spi };
/* Nominal and Data bit timing configuration */

can_bit_timing_cfg_t g_canfd0_bit_timing_cfg =
{
/* Actual bitrate: 500000 Hz. Actual sample point: 75 %. */
.baud_rate_prescaler = 1,
  .time_segment_1 = 59, .time_segment_2 = 20, .synchronization_jump_width = 4 };

can_bit_timing_cfg_t g_canfd0_data_timing_cfg =
{
/* Actual bitrate: 2000000 Hz. Actual sample point: 75 %. */
.baud_rate_prescaler = 1,
  .time_segment_1 = 14, .time_segment_2 = 5, .synchronization_jump_width = 1 };

extern const canfd_afl_entry_t p_canfd0_afl[CANFD_CFG_AFL_CH0_RULE_NUM];

#ifndef CANFD_PRV_GLOBAL_CFG
#define CANFD_PRV_GLOBAL_CFG
canfd_global_cfg_t g_canfd_global_cfg =
{ .global_interrupts = CANFD_CFG_GLOBAL_ERR_SOURCES, .global_config = (CANFD_CFG_TX_PRIORITY | CANFD_CFG_DLC_CHECK
        | (BSP_CFG_CANFDCLK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC ? R_CANFD_CFDGCFG_DCS_Msk : 0U)
        | CANFD_CFG_FD_OVERFLOW),
  .rx_mb_config = (CANFD_CFG_RXMB_NUMBER | (CANFD_CFG_RXMB_SIZE << R_CANFD_CFDRMNB_RMPLS_Pos)), .global_err_ipl =
          CANFD_CFG_GLOBAL_ERR_IPL,
  .rx_fifo_ipl = CANFD_CFG_RX_FIFO_IPL, .rx_fifo_config =
  { ((CANFD_CFG_RXFIFO0_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO0_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO0_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO0_INT_MODE)
            | (CANFD_CFG_RXFIFO0_ENABLE)),
    ((CANFD_CFG_RXFIFO1_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO1_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO1_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO1_INT_MODE)
            | (CANFD_CFG_RXFIFO1_ENABLE)),
#if !BSP_FEATURE_CANFD_LITE
    ((CANFD_CFG_RXFIFO2_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO2_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO2_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO2_INT_MODE)
            | (CANFD_CFG_RXFIFO2_ENABLE)),
    ((CANFD_CFG_RXFIFO3_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO3_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO3_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO3_INT_MODE)
            | (CANFD_CFG_RXFIFO3_ENABLE)),
    ((CANFD_CFG_RXFIFO4_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO4_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO4_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO4_INT_MODE)
            | (CANFD_CFG_RXFIFO4_ENABLE)),
    ((CANFD_CFG_RXFIFO5_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO5_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO5_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO5_INT_MODE)
            | (CANFD_CFG_RXFIFO5_ENABLE)),
    ((CANFD_CFG_RXFIFO6_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO6_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO6_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO6_INT_MODE)
            | (CANFD_CFG_RXFIFO6_ENABLE)),
    ((CANFD_CFG_RXFIFO7_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos)
            | (CANFD_CFG_RXFIFO7_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos)
            | (CANFD_CFG_RXFIFO7_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO7_INT_MODE)
            | (CANFD_CFG_RXFIFO7_ENABLE)),
#endif
          }, };
#endif

canfd_extended_cfg_t g_canfd0_extended_cfg =
{ .p_afl = p_canfd0_afl, .txmb_txi_enable = ((1ULL << 0) | 0ULL), .error_interrupts = (R_CANFD_CFDC_CTR_EWIE_Msk
        | R_CANFD_CFDC_CTR_EPIE_Msk | R_CANFD_CFDC_CTR_BOEIE_Msk | R_CANFD_CFDC_CTR_BORIE_Msk
        | R_CANFD_CFDC_CTR_OLIE_Msk | 0U),
  .p_data_timing = &g_canfd0_data_timing_cfg, .delay_compensation = (1), .p_global_cfg = &g_canfd_global_cfg, };

canfd_instance_ctrl_t g_canfd0_ctrl;
const can_cfg_t g_canfd0_cfg =
{ .channel = 0, .p_bit_timing = &g_canfd0_bit_timing_cfg, .p_callback = canfd0_callback, .p_extend =
          &g_canfd0_extended_cfg,
  .p_context = NULL, .ipl = (12),
#if defined(VECTOR_NUMBER_CAN0_TX)
    .tx_irq             = VECTOR_NUMBER_CAN0_TX,
#else
  .tx_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN0_CHERR)
    .error_irq             = VECTOR_NUMBER_CAN0_CHERR,
#else
  .error_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const can_instance_t g_canfd0 =
{ .p_ctrl = &g_canfd0_ctrl, .p_cfg = &g_canfd0_cfg, .p_api = &g_canfd_on_canfd };
agt_instance_ctrl_t g_timer10_ctrl;
const agt_extended_cfg_t g_timer10_extend =
{ .count_source = AGT_CLOCK_PCLKB,
  .agto = AGT_PIN_CFG_DISABLED,
  .agtoab_settings_b.agtoa = AGT_PIN_CFG_DISABLED,
  .agtoab_settings_b.agtob = AGT_PIN_CFG_DISABLED,
  .measurement_mode = AGT_MEASURE_DISABLED,
  .agtio_filter = AGT_AGTIO_FILTER_NONE,
  .enable_pin = AGT_ENABLE_PIN_NOT_USED,
  .trigger_edge = AGT_TRIGGER_EDGE_RISING, };
const timer_cfg_t g_timer10_cfg =
{ .mode = TIMER_MODE_PERIODIC,
/* Actual period: 0.01 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0xf424,
  .duty_cycle_counts = 0x7a12, .source_div = (timer_source_div_t) 3, .channel = 1, .p_callback = timer10_callback,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer10_extend,
  .cycle_end_ipl = (12),
#if defined(VECTOR_NUMBER_AGT1_INT)
    .cycle_end_irq       = VECTOR_NUMBER_AGT1_INT,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer10 =
{ .p_ctrl = &g_timer10_ctrl, .p_cfg = &g_timer10_cfg, .p_api = &g_timer_on_agt };
dac_instance_ctrl_t g_dac1_ctrl;
const dac_extended_cfg_t g_dac1_ext_cfg =
{ .enable_charge_pump = 0,
  .data_format = DAC_DATA_FORMAT_FLUSH_RIGHT,
  .output_amplifier_enabled = 0,
  .internal_output_enabled = false, };
const dac_cfg_t g_dac1_cfg =
{ .channel = 1, .ad_da_synchronized = false, .p_extend = &g_dac1_ext_cfg };
/* Instance structure to use this module. */
const dac_instance_t g_dac1 =
{ .p_ctrl = &g_dac1_ctrl, .p_cfg = &g_dac1_cfg, .p_api = &g_dac_on_dac };
dtc_instance_ctrl_t g_transfer25_ctrl;

transfer_info_t g_transfer25_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer25_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI9_RXI, };
const transfer_cfg_t g_transfer25_cfg =
{ .p_info = &g_transfer25_info, .p_extend = &g_transfer25_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer25 =
{ .p_ctrl = &g_transfer25_ctrl, .p_cfg = &g_transfer25_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer18_ctrl;

transfer_info_t g_transfer18_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer18_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI9_TXI, };
const transfer_cfg_t g_transfer18_cfg =
{ .p_info = &g_transfer18_info, .p_extend = &g_transfer18_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer18 =
{ .p_ctrl = &g_transfer18_ctrl, .p_cfg = &g_transfer18_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart4_ctrl;

baud_setting_t g_uart4_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart4_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart4_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart4_cfg =
{ .channel = 9, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          uart_callback,
  .p_context = NULL, .p_extend = &g_uart4_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer18)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer18,
#endif
#if (RA_NOT_DEFINED == g_transfer25)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer25,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI9_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI9_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI9_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI9_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI9_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart4 =
{ .p_ctrl = &g_uart4_ctrl, .p_cfg = &g_uart4_cfg, .p_api = &g_uart_on_sci };
dtc_instance_ctrl_t g_transfer24_ctrl;

transfer_info_t g_transfer24_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer24_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI8_RXI, };
const transfer_cfg_t g_transfer24_cfg =
{ .p_info = &g_transfer24_info, .p_extend = &g_transfer24_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer24 =
{ .p_ctrl = &g_transfer24_ctrl, .p_cfg = &g_transfer24_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer16_ctrl;

transfer_info_t g_transfer16_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer16_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI8_TXI, };
const transfer_cfg_t g_transfer16_cfg =
{ .p_info = &g_transfer16_info, .p_extend = &g_transfer16_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer16 =
{ .p_ctrl = &g_transfer16_ctrl, .p_cfg = &g_transfer16_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart3_ctrl;

baud_setting_t g_uart3_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart3_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart3_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart3_cfg =
{ .channel = 8, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          uart_callback,
  .p_context = NULL, .p_extend = &g_uart3_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer16)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer16,
#endif
#if (RA_NOT_DEFINED == g_transfer24)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer24,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI8_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI8_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI8_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI8_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI8_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI8_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI8_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI8_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart3 =
{ .p_ctrl = &g_uart3_ctrl, .p_cfg = &g_uart3_cfg, .p_api = &g_uart_on_sci };
dtc_instance_ctrl_t g_transfer23_ctrl;

transfer_info_t g_transfer23_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer23_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI7_RXI, };
const transfer_cfg_t g_transfer23_cfg =
{ .p_info = &g_transfer23_info, .p_extend = &g_transfer23_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer23 =
{ .p_ctrl = &g_transfer23_ctrl, .p_cfg = &g_transfer23_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer17_ctrl;

transfer_info_t g_transfer17_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer17_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI7_TXI, };
const transfer_cfg_t g_transfer17_cfg =
{ .p_info = &g_transfer17_info, .p_extend = &g_transfer17_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer17 =
{ .p_ctrl = &g_transfer17_ctrl, .p_cfg = &g_transfer17_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart2_ctrl;

baud_setting_t g_uart2_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart2_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart2_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart2_cfg =
{ .channel = 7, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          uart_callback,
  .p_context = NULL, .p_extend = &g_uart2_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer17)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer17,
#endif
#if (RA_NOT_DEFINED == g_transfer23)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer23,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI7_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI7_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI7_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI7_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI7_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI7_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart2 =
{ .p_ctrl = &g_uart2_ctrl, .p_cfg = &g_uart2_cfg, .p_api = &g_uart_on_sci };
dtc_instance_ctrl_t g_transfer22_ctrl;

transfer_info_t g_transfer22_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer22_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI6_RXI, };
const transfer_cfg_t g_transfer22_cfg =
{ .p_info = &g_transfer22_info, .p_extend = &g_transfer22_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer22 =
{ .p_ctrl = &g_transfer22_ctrl, .p_cfg = &g_transfer22_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer15_ctrl;

transfer_info_t g_transfer15_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer15_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI6_TXI, };
const transfer_cfg_t g_transfer15_cfg =
{ .p_info = &g_transfer15_info, .p_extend = &g_transfer15_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer15 =
{ .p_ctrl = &g_transfer15_ctrl, .p_cfg = &g_transfer15_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart1_ctrl;

baud_setting_t g_uart1_baud_setting =
        {
        /* Baud rate calculated with 0.014% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 38, .mddr = (uint8_t) 184, .semr_baudrate_bits_b.brme =
                  true };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart1_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart1_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart1_cfg =
{ .channel = 6, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          uart_callback,
  .p_context = NULL, .p_extend = &g_uart1_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer15)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer15,
#endif
#if (RA_NOT_DEFINED == g_transfer22)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer22,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI6_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI6_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI6_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI6_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI6_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI6_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart1 =
{ .p_ctrl = &g_uart1_ctrl, .p_cfg = &g_uart1_cfg, .p_api = &g_uart_on_sci };
dtc_instance_ctrl_t g_transfer76_ctrl;

transfer_info_t g_transfer76_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer76_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI4_RXI, };
const transfer_cfg_t g_transfer76_cfg =
{ .p_info = &g_transfer76_info, .p_extend = &g_transfer76_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer76 =
{ .p_ctrl = &g_transfer76_ctrl, .p_cfg = &g_transfer76_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer75_ctrl;

transfer_info_t g_transfer75_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer75_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI4_TXI, };
const transfer_cfg_t g_transfer75_cfg =
{ .p_info = &g_transfer75_info, .p_extend = &g_transfer75_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer75 =
{ .p_ctrl = &g_transfer75_ctrl, .p_cfg = &g_transfer75_cfg, .p_api = &g_transfer_on_dtc };
sci_spi_instance_ctrl_t g_spi2_ctrl;

/** SPI extended configuration */
const sci_spi_extended_cfg_t g_spi2_cfg_extend =
{ .clk_div =
{
/* Actual calculated bitrate: 1000000. */.cks = 0,
  .brr = 24, .mddr = 0, } };

const spi_cfg_t g_spi2_cfg =
{ .channel = 4, .operating_mode = SPI_MODE_MASTER, .clk_phase = SPI_CLK_PHASE_EDGE_ODD, .clk_polarity =
          SPI_CLK_POLARITY_LOW,
  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE, .bit_order = SPI_BIT_ORDER_MSB_FIRST,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer75)
    .p_transfer_tx   = NULL,
#else
  .p_transfer_tx = &g_transfer75,
#endif
#if (RA_NOT_DEFINED == g_transfer76)
    .p_transfer_rx   = NULL,
#else
  .p_transfer_rx = &g_transfer76,
#endif
#undef RA_NOT_DEFINED
  .p_callback = sci_spi_callback,
  .p_context = NULL, .rxi_irq = VECTOR_NUMBER_SCI4_RXI, .txi_irq = VECTOR_NUMBER_SCI4_TXI, .tei_irq =
          VECTOR_NUMBER_SCI4_TEI,
  .eri_irq = VECTOR_NUMBER_SCI4_ERI, .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12), .p_extend =
          &g_spi2_cfg_extend, };
/* Instance structure to use this module. */
const spi_instance_t g_spi2 =
{ .p_ctrl = &g_spi2_ctrl, .p_cfg = &g_spi2_cfg, .p_api = &g_spi_on_sci };
dtc_instance_ctrl_t g_transfer12_ctrl;

transfer_info_t g_transfer12_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_4_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_BLOCK,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer12_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SSI0_RXI, };
const transfer_cfg_t g_transfer12_cfg =
{ .p_info = &g_transfer12_info, .p_extend = &g_transfer12_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer12 =
{ .p_ctrl = &g_transfer12_ctrl, .p_cfg = &g_transfer12_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer11_ctrl;

transfer_info_t g_transfer11_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_4_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_BLOCK,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer11_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SSI0_TXI, };
const transfer_cfg_t g_transfer11_cfg =
{ .p_info = &g_transfer11_info, .p_extend = &g_transfer11_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer11 =
{ .p_ctrl = &g_transfer11_ctrl, .p_cfg = &g_transfer11_cfg, .p_api = &g_transfer_on_dtc };
ssi_instance_ctrl_t g_i2s0_ctrl;

/** SSI instance configuration */
const ssi_extended_cfg_t g_i2s0_cfg_extend =
{ .audio_clock = SSI_AUDIO_CLOCK_EXTERNAL, .bit_clock_div = SSI_CLOCK_DIV_1, };

/** I2S interface configuration */
const i2s_cfg_t g_i2s0_cfg =
{ .channel = 0, .pcm_width = I2S_PCM_WIDTH_16_BITS, .operating_mode = I2S_MODE_MASTER, .word_length =
          I2S_WORD_LENGTH_16_BITS,
  .ws_continue = I2S_WS_CONTINUE_OFF, .p_callback = NULL, .p_context = NULL, .p_extend = &g_i2s0_cfg_extend,
#if (2) != BSP_IRQ_DISABLED
#if 0 == 0
  .txi_irq = VECTOR_NUMBER_SSI0_TXI,
#else
                .txi_irq                 = VECTOR_NUMBER_SSI0_TXI_RXI,
  #endif
#else
                .txi_irq                 = FSP_INVALID_VECTOR,
#endif
#if (2) != BSP_IRQ_DISABLED
#if 0 == 0
  .rxi_irq = VECTOR_NUMBER_SSI0_RXI,
#else
                .rxi_irq                 = VECTOR_NUMBER_SSI0_TXI_RXI,
  #endif
#else
                .rxi_irq                 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SSI0_INT)
                .int_irq                 = VECTOR_NUMBER_SSI0_INT,
#else
  .int_irq = FSP_INVALID_VECTOR,
#endif
  .txi_ipl = (2),
  .rxi_ipl = (2), .idle_err_ipl = (2),
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer11)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer11,
#endif
#if (RA_NOT_DEFINED == g_transfer12)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer12,
#endif
#undef RA_NOT_DEFINED
        };

/* Instance structure to use this module. */
const i2s_instance_t g_i2s0 =
{ .p_ctrl = &g_i2s0_ctrl, .p_cfg = &g_i2s0_cfg, .p_api = &g_i2s_on_ssi };
dtc_instance_ctrl_t g_transfer10_ctrl;

transfer_info_t g_transfer10_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_2_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer10_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SPI1_RXI, };
const transfer_cfg_t g_transfer10_cfg =
{ .p_info = &g_transfer10_info, .p_extend = &g_transfer10_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer10 =
{ .p_ctrl = &g_transfer10_ctrl, .p_cfg = &g_transfer10_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer9_ctrl;

transfer_info_t g_transfer9_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_2_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer9_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SPI1_TXI, };
const transfer_cfg_t g_transfer9_cfg =
{ .p_info = &g_transfer9_info, .p_extend = &g_transfer9_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer9 =
{ .p_ctrl = &g_transfer9_ctrl, .p_cfg = &g_transfer9_cfg, .p_api = &g_transfer_on_dtc };
spi_instance_ctrl_t g_spi1_ctrl;

/** SPI extended configuration for SPI HAL driver */
const spi_extended_cfg_t g_spi1_ext_cfg =
{ .spi_clksyn = SPI_SSL_MODE_CLK_SYN,
  .spi_comm = SPI_COMMUNICATION_FULL_DUPLEX,
  .ssl_polarity = SPI_SSLP_LOW,
  .ssl_select = SPI_SSL_SELECT_SSL0,
  .mosi_idle = SPI_MOSI_IDLE_VALUE_FIXING_DISABLE,
  .parity = SPI_PARITY_MODE_DISABLE,
  .byte_swap = SPI_BYTE_SWAP_DISABLE,
  .spck_div =
  {
  /* Actual calculated bitrate: 1000000. */.spbr = 49,
    .brdv = 0 },
  .spck_delay = SPI_DELAY_COUNT_1,
  .ssl_negation_delay = SPI_DELAY_COUNT_1,
  .next_access_delay = SPI_DELAY_COUNT_1 };

/** SPI configuration for SPI HAL driver */
const spi_cfg_t g_spi1_cfg =
{ .channel = 1,

#if defined(VECTOR_NUMBER_SPI1_RXI)
    .rxi_irq             = VECTOR_NUMBER_SPI1_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_TXI)
    .txi_irq             = VECTOR_NUMBER_SPI1_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_TEI)
    .tei_irq             = VECTOR_NUMBER_SPI1_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SPI1_ERI)
    .eri_irq             = VECTOR_NUMBER_SPI1_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif

  .rxi_ipl = (12),
  .txi_ipl = (12),
  .tei_ipl = (12),
  .eri_ipl = (12),

  .operating_mode = SPI_MODE_MASTER,

  .clk_phase = SPI_CLK_PHASE_EDGE_ODD,
  .clk_polarity = SPI_CLK_POLARITY_LOW,

  .mode_fault = SPI_MODE_FAULT_ERROR_DISABLE,
  .bit_order = SPI_BIT_ORDER_MSB_FIRST,
  .p_transfer_tx = g_spi1_P_TRANSFER_TX,
  .p_transfer_rx = g_spi1_P_TRANSFER_RX,
  .p_callback = spi_callback,

  .p_context = NULL,
  .p_extend = (void*) &g_spi1_ext_cfg, };

/* Instance structure to use this module. */
const spi_instance_t g_spi1 =
{ .p_ctrl = &g_spi1_ctrl, .p_cfg = &g_spi1_cfg, .p_api = &g_spi_on_spi };
dtc_instance_ctrl_t g_transfer6_ctrl;

transfer_info_t g_transfer6_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer6_cfg_extend =
{ .activation_source = VECTOR_NUMBER_IIC2_RXI, };
const transfer_cfg_t g_transfer6_cfg =
{ .p_info = &g_transfer6_info, .p_extend = &g_transfer6_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer6 =
{ .p_ctrl = &g_transfer6_ctrl, .p_cfg = &g_transfer6_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer4_ctrl;

transfer_info_t g_transfer4_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer4_cfg_extend =
{ .activation_source = VECTOR_NUMBER_IIC2_TXI, };
const transfer_cfg_t g_transfer4_cfg =
{ .p_info = &g_transfer4_info, .p_extend = &g_transfer4_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer4 =
{ .p_ctrl = &g_transfer4_ctrl, .p_cfg = &g_transfer4_cfg, .p_api = &g_transfer_on_dtc };
iic_master_instance_ctrl_t g_i2c_master2_ctrl;
const iic_master_extended_cfg_t g_i2c_master2_extend =
{ .timeout_mode = IIC_MASTER_TIMEOUT_MODE_SHORT, .timeout_scl_low = IIC_MASTER_TIMEOUT_SCL_LOW_ENABLED,
/* Actual calculated bitrate: 98425. Actual calculated duty cycle: 50%. */.clock_settings.brl_value = 28,
  .clock_settings.brh_value = 28, .clock_settings.cks_value = 3, };
const i2c_master_cfg_t g_i2c_master2_cfg =
{ .channel = 2, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x54, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer4)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer4,
#endif
#if (RA_NOT_DEFINED == g_transfer6)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer6,
#endif
#undef RA_NOT_DEFINED
  .p_callback = i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_IIC2_RXI)
    .rxi_irq             = VECTOR_NUMBER_IIC2_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC2_TXI)
    .txi_irq             = VECTOR_NUMBER_IIC2_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC2_TEI)
    .tei_irq             = VECTOR_NUMBER_IIC2_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC2_ERI)
    .eri_irq             = VECTOR_NUMBER_IIC2_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12),
  .p_extend = &g_i2c_master2_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_master2 =
{ .p_ctrl = &g_i2c_master2_ctrl, .p_cfg = &g_i2c_master2_cfg, .p_api = &g_i2c_master_on_iic };
/* Nominal and Data bit timing configuration */

can_bit_timing_cfg_t g_canfd1_bit_timing_cfg =
{
/* Actual bitrate: 500000 Hz. Actual sample point: 75 %. */
.baud_rate_prescaler = 2,
  .time_segment_1 = 29, .time_segment_2 = 10, .synchronization_jump_width = 4 };

can_bit_timing_cfg_t g_canfd1_data_timing_cfg =
{
/* Actual bitrate: 500000 Hz. Actual sample point: 75 %. */
.baud_rate_prescaler = 2,
  .time_segment_1 = 29, .time_segment_2 = 10, .synchronization_jump_width = 4 };

extern const canfd_afl_entry_t p_canfd1_afl[CANFD_CFG_AFL_CH1_RULE_NUM];

#ifndef CANFD_PRV_GLOBAL_CFG
#define CANFD_PRV_GLOBAL_CFG
canfd_global_cfg_t g_canfd_global_cfg =
{
    .global_interrupts = CANFD_CFG_GLOBAL_ERR_SOURCES,
    .global_config     = (CANFD_CFG_TX_PRIORITY | CANFD_CFG_DLC_CHECK | (BSP_CFG_CANFDCLK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC ? R_CANFD_CFDGCFG_DCS_Msk : 0U) | CANFD_CFG_FD_OVERFLOW),
    .rx_mb_config      = (CANFD_CFG_RXMB_NUMBER | (CANFD_CFG_RXMB_SIZE << R_CANFD_CFDRMNB_RMPLS_Pos)),
    .global_err_ipl = CANFD_CFG_GLOBAL_ERR_IPL,
    .rx_fifo_ipl    = CANFD_CFG_RX_FIFO_IPL,
    .rx_fifo_config    =
    {
        ((CANFD_CFG_RXFIFO0_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO0_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO0_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO0_INT_MODE) | (CANFD_CFG_RXFIFO0_ENABLE)),
        ((CANFD_CFG_RXFIFO1_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO1_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO1_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO1_INT_MODE) | (CANFD_CFG_RXFIFO1_ENABLE)),
#if !BSP_FEATURE_CANFD_LITE
        ((CANFD_CFG_RXFIFO2_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO2_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO2_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO2_INT_MODE) | (CANFD_CFG_RXFIFO2_ENABLE)),
        ((CANFD_CFG_RXFIFO3_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO3_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO3_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO3_INT_MODE) | (CANFD_CFG_RXFIFO3_ENABLE)),
        ((CANFD_CFG_RXFIFO4_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO4_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO4_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO4_INT_MODE) | (CANFD_CFG_RXFIFO4_ENABLE)),
        ((CANFD_CFG_RXFIFO5_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO5_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO5_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO5_INT_MODE) | (CANFD_CFG_RXFIFO5_ENABLE)),
        ((CANFD_CFG_RXFIFO6_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO6_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO6_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO6_INT_MODE) | (CANFD_CFG_RXFIFO6_ENABLE)),
        ((CANFD_CFG_RXFIFO7_INT_THRESHOLD << R_CANFD_CFDRFCC_RFIGCV_Pos) | (CANFD_CFG_RXFIFO7_DEPTH << R_CANFD_CFDRFCC_RFDC_Pos) | (CANFD_CFG_RXFIFO7_PAYLOAD << R_CANFD_CFDRFCC_RFPLS_Pos) | (CANFD_CFG_RXFIFO7_INT_MODE) | (CANFD_CFG_RXFIFO7_ENABLE)),
#endif
    },
};
#endif

canfd_extended_cfg_t g_canfd1_extended_cfg =
{ .p_afl = p_canfd1_afl, .txmb_txi_enable = ((1ULL << 0) | 0ULL), .error_interrupts = (R_CANFD_CFDC_CTR_EWIE_Msk
        | R_CANFD_CFDC_CTR_EPIE_Msk | R_CANFD_CFDC_CTR_BOEIE_Msk | R_CANFD_CFDC_CTR_BORIE_Msk
        | R_CANFD_CFDC_CTR_OLIE_Msk | 0U),
  .p_data_timing = &g_canfd1_data_timing_cfg, .delay_compensation = (1), .p_global_cfg = &g_canfd_global_cfg, };

canfd_instance_ctrl_t g_canfd1_ctrl;
const can_cfg_t g_canfd1_cfg =
{ .channel = 1, .p_bit_timing = &g_canfd1_bit_timing_cfg, .p_callback = canfd1_callback, .p_extend =
          &g_canfd1_extended_cfg,
  .p_context = NULL, .ipl = (12),
#if defined(VECTOR_NUMBER_CAN1_TX)
    .tx_irq             = VECTOR_NUMBER_CAN1_TX,
#else
  .tx_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_CAN1_CHERR)
    .error_irq             = VECTOR_NUMBER_CAN1_CHERR,
#else
  .error_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const can_instance_t g_canfd1 =
{ .p_ctrl = &g_canfd1_ctrl, .p_cfg = &g_canfd1_cfg, .p_api = &g_canfd_on_canfd };
icu_instance_ctrl_t g_external_irq9_ctrl;
const external_irq_cfg_t g_external_irq9_cfg =
{ .channel = 14,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = isr_irq9,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ14)
    .irq                 = VECTOR_NUMBER_ICU_IRQ14,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq9 =
{ .p_ctrl = &g_external_irq9_ctrl, .p_cfg = &g_external_irq9_cfg, .p_api = &g_external_irq_on_icu };
icu_instance_ctrl_t g_external_irq8_ctrl;
const external_irq_cfg_t g_external_irq8_cfg =
{ .channel = 13,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = isr_irq8,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ13)
    .irq                 = VECTOR_NUMBER_ICU_IRQ13,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq8 =
{ .p_ctrl = &g_external_irq8_ctrl, .p_cfg = &g_external_irq8_cfg, .p_api = &g_external_irq_on_icu };
icu_instance_ctrl_t g_external_irq7_ctrl;
const external_irq_cfg_t g_external_irq7_cfg =
{ .channel = 11,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = isr_irq7,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ11)
    .irq                 = VECTOR_NUMBER_ICU_IRQ11,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq7 =
{ .p_ctrl = &g_external_irq7_ctrl, .p_cfg = &g_external_irq7_cfg, .p_api = &g_external_irq_on_icu };
icu_instance_ctrl_t g_external_irq6_ctrl;
const external_irq_cfg_t g_external_irq6_cfg =
{ .channel = 8,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = isr_irq6,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ8)
    .irq                 = VECTOR_NUMBER_ICU_IRQ8,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq6 =
{ .p_ctrl = &g_external_irq6_ctrl, .p_cfg = &g_external_irq6_cfg, .p_api = &g_external_irq_on_icu };
icu_instance_ctrl_t g_external_irq5_ctrl;
const external_irq_cfg_t g_external_irq5_cfg =
{ .channel = 6,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = isr_irq5,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ6)
    .irq                 = VECTOR_NUMBER_ICU_IRQ6,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq5 =
{ .p_ctrl = &g_external_irq5_ctrl, .p_cfg = &g_external_irq5_cfg, .p_api = &g_external_irq_on_icu };
icu_instance_ctrl_t g_external_irq4_ctrl;
const external_irq_cfg_t g_external_irq4_cfg =
{ .channel = 4,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = isr_irq4,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ4)
    .irq                 = VECTOR_NUMBER_ICU_IRQ4,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq4 =
{ .p_ctrl = &g_external_irq4_ctrl, .p_cfg = &g_external_irq4_cfg, .p_api = &g_external_irq_on_icu };
icu_instance_ctrl_t g_external_irq3_ctrl;
const external_irq_cfg_t g_external_irq3_cfg =
{ .channel = 3,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = isr_irq3,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ3)
    .irq                 = VECTOR_NUMBER_ICU_IRQ3,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq3 =
{ .p_ctrl = &g_external_irq3_ctrl, .p_cfg = &g_external_irq3_cfg, .p_api = &g_external_irq_on_icu };
icu_instance_ctrl_t g_external_irq2_ctrl;
const external_irq_cfg_t g_external_irq2_cfg =
{ .channel = 2,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = isr_irq2,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ2)
    .irq                 = VECTOR_NUMBER_ICU_IRQ2,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq2 =
{ .p_ctrl = &g_external_irq2_ctrl, .p_cfg = &g_external_irq2_cfg, .p_api = &g_external_irq_on_icu };
flash_hp_instance_ctrl_t g_flash0_ctrl;
const flash_cfg_t g_flash0_cfg =
{ .data_flash_bgo = true, .p_callback = rm_vee_flash_callback, .p_context = &g_vee0_ctrl,
#if defined(VECTOR_NUMBER_FCU_FRDYI)
    .irq                 = VECTOR_NUMBER_FCU_FRDYI,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_FCU_FIFERR)
    .err_irq             = VECTOR_NUMBER_FCU_FIFERR,
#else
  .err_irq = FSP_INVALID_VECTOR,
#endif
  .err_ipl = (10),
  .ipl = (10), };
/* Instance structure to use this module. */
const flash_instance_t g_flash0 =
{ .p_ctrl = &g_flash0_ctrl, .p_cfg = &g_flash0_cfg, .p_api = &g_flash_on_flash_hp };
rm_vee_flash_instance_ctrl_t g_vee0_ctrl;

const rm_vee_flash_cfg_t g_vee0_cfg_ext =
{ .p_flash = &g_flash0 };

static uint16_t g_vee0_record_offset[16 + 1] =
{ 0 };

const rm_vee_cfg_t g_vee0_cfg =
{ .start_addr = BSP_FEATURE_FLASH_DATA_FLASH_START,
  .num_segments = 2,
  .total_size = BSP_DATA_FLASH_SIZE_BYTES,
  .ref_data_size = 0,
  .record_max_id = 16,
  .rec_offset = &g_vee0_record_offset[0],
  .p_callback = vee_callback,
  .p_context = NULL,
  .p_extend = &g_vee0_cfg_ext };

/* Instance structure to use this module. */
const rm_vee_instance_t g_vee0 =
{ .p_ctrl = &g_vee0_ctrl, .p_cfg = &g_vee0_cfg, .p_api = &g_rm_vee_on_flash };
qspi_instance_ctrl_t g_qspi0_ctrl;

static const spi_flash_erase_command_t g_qspi0_erase_command_list[] =
{
#if 4096 > 0
  { .command = 0x20, .size = 4096 },
#endif
#if 32768 > 0
  { .command = 0x52, .size = 32768 },
#endif
#if 65536 > 0
  { .command = 0xD8, .size = 65536 },
#endif
#if 0xC7 > 0
  { .command = 0xC7, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE },
#endif
        };
static const qspi_extended_cfg_t g_qspi0_extended_cfg =
{ .min_qssl_deselect_cycles = QSPI_QSSL_MIN_HIGH_LEVEL_4_QSPCLK, .qspclk_div = QSPI_QSPCLK_DIV_2, };
const spi_flash_cfg_t g_qspi0_cfg =
{ .spi_protocol = SPI_FLASH_PROTOCOL_EXTENDED_SPI,
  .read_mode = SPI_FLASH_READ_MODE_FAST_READ_QUAD_IO,
  .address_bytes = SPI_FLASH_ADDRESS_BYTES_3,
  .dummy_clocks = SPI_FLASH_DUMMY_CLOCKS_DEFAULT,
  .page_program_address_lines = SPI_FLASH_DATA_LINES_1,
  .page_size_bytes = 256,
  .page_program_command = 0x02,
  .write_enable_command = 0x06,
  .status_command = 0x05,
  .write_status_bit = 0,
  .xip_enter_command = 0x20,
  .xip_exit_command = 0xFF,
  .p_erase_command_list = &g_qspi0_erase_command_list[0],
  .erase_command_list_length = sizeof(g_qspi0_erase_command_list) / sizeof(g_qspi0_erase_command_list[0]),
  .p_extend = &g_qspi0_extended_cfg, };
/** This structure encompasses everything that is needed to use an instance of this interface. */
const spi_flash_instance_t g_qspi0 =
{ .p_ctrl = &g_qspi0_ctrl, .p_cfg = &g_qspi0_cfg, .p_api = &g_qspi_on_spi_flash, };
adc_instance_ctrl_t g_adc1_ctrl;
const adc_extended_cfg_t g_adc1_cfg_extend =
{ .add_average_count = ADC_ADD_OFF,
  .clearing = ADC_CLEAR_AFTER_READ_ON,
  .trigger_group_b = ADC_TRIGGER_SYNC_ELC,
  .double_trigger_mode = ADC_DOUBLE_TRIGGER_DISABLED,
  .adc_vref_control = ADC_VREF_CONTROL_VREFH,
  .enable_adbuf = 0,
#if defined(VECTOR_NUMBER_ADC1_WINDOW_A)
    .window_a_irq        = VECTOR_NUMBER_ADC1_WINDOW_A,
#else
  .window_a_irq = FSP_INVALID_VECTOR,
#endif
  .window_a_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC1_WINDOW_B)
    .window_b_irq      = VECTOR_NUMBER_ADC1_WINDOW_B,
#else
  .window_b_irq = FSP_INVALID_VECTOR,
#endif
  .window_b_ipl = (BSP_IRQ_DISABLED), };
const adc_cfg_t g_adc1_cfg =
{ .unit = 1, .mode = ADC_MODE_SINGLE_SCAN, .resolution = ADC_RESOLUTION_12_BIT, .alignment =
          (adc_alignment_t) ADC_ALIGNMENT_RIGHT,
  .trigger = ADC_TRIGGER_SOFTWARE, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_adc1_cfg_extend,
#if defined(VECTOR_NUMBER_ADC1_SCAN_END)
    .scan_end_irq        = VECTOR_NUMBER_ADC1_SCAN_END,
#else
  .scan_end_irq = FSP_INVALID_VECTOR,
#endif
  .scan_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC1_SCAN_END_B)
    .scan_end_b_irq      = VECTOR_NUMBER_ADC1_SCAN_END_B,
#else
  .scan_end_b_irq = FSP_INVALID_VECTOR,
#endif
  .scan_end_b_ipl = (BSP_IRQ_DISABLED), };
#if ((0) | (0))
const adc_window_cfg_t g_adc1_window_cfg =
{
    .compare_mask        =  0,
    .compare_mode_mask   =  0,
    .compare_cfg         = (0) | (0) | (0) | (ADC_COMPARE_CFG_EVENT_OUTPUT_OR),
    .compare_ref_low     = 0,
    .compare_ref_high    = 0,
    .compare_b_channel   = (ADC_WINDOW_B_CHANNEL_0),
    .compare_b_mode      = (ADC_WINDOW_B_MODE_LESS_THAN_OR_OUTSIDE),
    .compare_b_ref_low   = 0,
    .compare_b_ref_high  = 0,
};
#endif
const adc_channel_cfg_t g_adc1_channel_cfg =
{ .scan_mask = 0,
  .scan_mask_group_b = 0,
  .priority_group_a = ADC_GROUP_A_PRIORITY_OFF,
  .add_mask = 0,
  .sample_hold_mask = 0,
  .sample_hold_states = 24,
#if ((0) | (0))
    .p_window_cfg        = (adc_window_cfg_t *) &g_adc1_window_cfg,
#else
  .p_window_cfg = NULL,
#endif
        };
/* Instance structure to use this module. */
const adc_instance_t g_adc1 =
{ .p_ctrl = &g_adc1_ctrl, .p_cfg = &g_adc1_cfg, .p_channel_cfg = &g_adc1_channel_cfg, .p_api = &g_adc_on_adc };
agt_instance_ctrl_t g_timer9_ctrl;
const agt_extended_cfg_t g_timer9_extend =
{ .count_source = AGT_CLOCK_PCLKB,
  .agto = AGT_PIN_CFG_DISABLED,
  .agtoab_settings_b.agtoa = AGT_PIN_CFG_DISABLED,
  .agtoab_settings_b.agtob = AGT_PIN_CFG_DISABLED,
  .measurement_mode = AGT_MEASURE_DISABLED,
  .agtio_filter = AGT_AGTIO_FILTER_NONE,
  .enable_pin = AGT_ENABLE_PIN_NOT_USED,
  .trigger_edge = AGT_TRIGGER_EDGE_RISING, };
const timer_cfg_t g_timer9_cfg =
{ .mode = TIMER_MODE_PERIODIC,
/* Actual period: 0.00131072 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0x10000,
  .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t) 0, .channel = 2, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer9_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_AGT2_INT)
    .cycle_end_irq       = VECTOR_NUMBER_AGT2_INT,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer9 =
{ .p_ctrl = &g_timer9_ctrl, .p_cfg = &g_timer9_cfg, .p_api = &g_timer_on_agt };
gpt_instance_ctrl_t g_timer6_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer6_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT8_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT8_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer6_extend =
        { .gtioca =
        { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT8_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT8_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT8_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT8_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer6_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
          .gtior_setting.gtior = 0U,
#endif
        };
const timer_cfg_t g_timer6_cfg =
{ .mode = TIMER_MODE_PWM,
/* Actual period: 0.0020408 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0xc74c,
  .duty_cycle_counts = 0x63a6, .source_div = (timer_source_div_t) 2, .channel = 8, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer6_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT8_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT8_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer6 =
{ .p_ctrl = &g_timer6_ctrl, .p_cfg = &g_timer6_cfg, .p_api = &g_timer_on_gpt };
gpt_instance_ctrl_t g_timer5_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer5_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT7_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT7_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer5_extend =
        { .gtioca =
        { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT7_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer5_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
          .gtior_setting.gtior = 0U,
#endif
        };
const timer_cfg_t g_timer5_cfg =
{ .mode = TIMER_MODE_PWM,
/* Actual period: 0.0020408 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0xc74c,
  .duty_cycle_counts = 0x63a6, .source_div = (timer_source_div_t) 2, .channel = 7, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer5_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT7_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT7_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer5 =
{ .p_ctrl = &g_timer5_ctrl, .p_cfg = &g_timer5_cfg, .p_api = &g_timer_on_gpt };
gpt_instance_ctrl_t g_timer4_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer4_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT6_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT6_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer4_extend =
        { .gtioca =
        { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT6_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer4_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
          .gtior_setting.gtior = 0U,
#endif
        };
const timer_cfg_t g_timer4_cfg =
{ .mode = TIMER_MODE_PWM,
/* Actual period: 0.0020408 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0xc74c,
  .duty_cycle_counts = 0x63a6, .source_div = (timer_source_div_t) 2, .channel = 6, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer4_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT6_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT6_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer4 =
{ .p_ctrl = &g_timer4_ctrl, .p_cfg = &g_timer4_cfg, .p_api = &g_timer_on_gpt };
gpt_instance_ctrl_t g_timer3_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer3_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT4_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT4_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer3_extend =
        { .gtioca =
        { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT4_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT4_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT4_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT4_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer3_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) false,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) true,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
          .gtior_setting.gtior = 0U,
#endif
        };
const timer_cfg_t g_timer3_cfg =
{ .mode = TIMER_MODE_PWM,
/* Actual period: 0.0020408 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0xc74c,
  .duty_cycle_counts = 0x63a6, .source_div = (timer_source_div_t) 2, .channel = 4, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer3_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT4_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT4_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer3 =
{ .p_ctrl = &g_timer3_ctrl, .p_cfg = &g_timer3_cfg, .p_api = &g_timer_on_gpt };
gpt_instance_ctrl_t g_timer2_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer2_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT3_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT3_COUNTER_UNDERFLOW,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG2,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
};
#endif
const gpt_extended_cfg_t g_timer2_extend =
        { .gtioca =
        { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT3_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT3_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT3_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT3_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer2_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (1U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
          .gtior_setting.gtior = 0U,
#endif
        };
const timer_cfg_t g_timer2_cfg =
{ .mode = TIMER_MODE_PWM,
/* Actual period: 0.00204081 seconds. Actual duty: 49.999754999240494%. */.period_counts = (uint32_t) 0x31d31,
  .duty_cycle_counts = 0x18e98, .source_div = (timer_source_div_t) 0, .channel = 3, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer2_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT3_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT3_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer2 =
{ .p_ctrl = &g_timer2_ctrl, .p_cfg = &g_timer2_cfg, .p_api = &g_timer_on_gpt };
dtc_instance_ctrl_t g_transfer13_ctrl;

transfer_info_t g_transfer13_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer13_cfg_extend =
{ .activation_source = VECTOR_NUMBER_IIC1_RXI, };
const transfer_cfg_t g_transfer13_cfg =
{ .p_info = &g_transfer13_info, .p_extend = &g_transfer13_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer13 =
{ .p_ctrl = &g_transfer13_ctrl, .p_cfg = &g_transfer13_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer8_ctrl;

transfer_info_t g_transfer8_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer8_cfg_extend =
{ .activation_source = VECTOR_NUMBER_IIC1_TXI, };
const transfer_cfg_t g_transfer8_cfg =
{ .p_info = &g_transfer8_info, .p_extend = &g_transfer8_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer8 =
{ .p_ctrl = &g_transfer8_ctrl, .p_cfg = &g_transfer8_cfg, .p_api = &g_transfer_on_dtc };
iic_master_instance_ctrl_t g_i2c_master1_ctrl;
const iic_master_extended_cfg_t g_i2c_master1_extend =
{ .timeout_mode = IIC_MASTER_TIMEOUT_MODE_SHORT, .timeout_scl_low = IIC_MASTER_TIMEOUT_SCL_LOW_ENABLED,
/* Actual calculated bitrate: 98425. Actual calculated duty cycle: 50%. */.clock_settings.brl_value = 28,
  .clock_settings.brh_value = 28, .clock_settings.cks_value = 3, };
const i2c_master_cfg_t g_i2c_master1_cfg =
{ .channel = 1, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x54, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer8)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer8,
#endif
#if (RA_NOT_DEFINED == g_transfer13)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer13,
#endif
#undef RA_NOT_DEFINED
  .p_callback = i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_IIC1_RXI)
    .rxi_irq             = VECTOR_NUMBER_IIC1_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC1_TXI)
    .txi_irq             = VECTOR_NUMBER_IIC1_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC1_TEI)
    .tei_irq             = VECTOR_NUMBER_IIC1_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC1_ERI)
    .eri_irq             = VECTOR_NUMBER_IIC1_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12),
  .p_extend = &g_i2c_master1_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_master1 =
{ .p_ctrl = &g_i2c_master1_ctrl, .p_cfg = &g_i2c_master1_cfg, .p_api = &g_i2c_master_on_iic };
icu_instance_ctrl_t g_external_irq1_ctrl;
const external_irq_cfg_t g_external_irq1_cfg =
{ .channel = 3,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = isr_irq1,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ3)
    .irq                 = VECTOR_NUMBER_ICU_IRQ3,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq1 =
{ .p_ctrl = &g_external_irq1_ctrl, .p_cfg = &g_external_irq1_cfg, .p_api = &g_external_irq_on_icu };
lpm_instance_ctrl_t g_lpm0_ctrl;

const lpm_cfg_t g_lpm0_cfg =
{ .low_power_mode = LPM_MODE_SLEEP,
  .snooze_cancel_sources = LPM_SNOOZE_CANCEL_SOURCE_NONE,
  .standby_wake_sources = LPM_STANDBY_WAKE_SOURCE_IRQ0 | LPM_STANDBY_WAKE_SOURCE_IRQ1 | LPM_STANDBY_WAKE_SOURCE_IRQ2
          | LPM_STANDBY_WAKE_SOURCE_IRQ3 | LPM_STANDBY_WAKE_SOURCE_IRQ4 | LPM_STANDBY_WAKE_SOURCE_IRQ5
          | LPM_STANDBY_WAKE_SOURCE_IRQ6 | LPM_STANDBY_WAKE_SOURCE_IRQ7 | LPM_STANDBY_WAKE_SOURCE_IRQ8
          | LPM_STANDBY_WAKE_SOURCE_IRQ9 | LPM_STANDBY_WAKE_SOURCE_IRQ10 | LPM_STANDBY_WAKE_SOURCE_IRQ11
          | LPM_STANDBY_WAKE_SOURCE_IRQ12 | LPM_STANDBY_WAKE_SOURCE_IRQ13 | LPM_STANDBY_WAKE_SOURCE_IRQ14
          | LPM_STANDBY_WAKE_SOURCE_IRQ15 | LPM_STANDBY_WAKE_SOURCE_IWDT | LPM_STANDBY_WAKE_SOURCE_LVD1
          | LPM_STANDBY_WAKE_SOURCE_LVD2 | LPM_STANDBY_WAKE_SOURCE_RTCALM | LPM_STANDBY_WAKE_SOURCE_RTCPRD
          | LPM_STANDBY_WAKE_SOURCE_USBHS | LPM_STANDBY_WAKE_SOURCE_USBFS | LPM_STANDBY_WAKE_SOURCE_AGT1UD
          | LPM_STANDBY_WAKE_SOURCE_AGT1CA | LPM_STANDBY_WAKE_SOURCE_AGT1CB | LPM_STANDBY_WAKE_SOURCE_IIC0
          | LPM_STANDBY_WAKE_SOURCE_AGT3UD | LPM_STANDBY_WAKE_SOURCE_AGT3CA | LPM_STANDBY_WAKE_SOURCE_AGT3CB
          | (lpm_standby_wake_source_t) 0,
  .snooze_request_source = LPM_SNOOZE_REQUEST_RXD0_FALLING,
  .snooze_end_sources = (lpm_snooze_end_t) 0,
  .dtc_state_in_snooze = LPM_SNOOZE_DTC_DISABLE,
#if BSP_FEATURE_LPM_HAS_SBYCR_OPE
    .output_port_enable         = LPM_OUTPUT_PORT_ENABLE_RETAIN,
#endif
#if BSP_FEATURE_LPM_HAS_DEEP_STANDBY
    .io_port_state              = LPM_IO_PORT_NO_CHANGE,
    .power_supply_state         = LPM_POWER_SUPPLY_DEEPCUT0,
    .deep_standby_cancel_source =  (lpm_deep_standby_cancel_source_t) 0,
    .deep_standby_cancel_edge   =  (lpm_deep_standby_cancel_edge_t) 0,
#endif
  .p_extend = NULL, };

const lpm_instance_t g_lpm0 =
{ .p_api = &g_lpm_on_lpm, .p_ctrl = &g_lpm0_ctrl, .p_cfg = &g_lpm0_cfg };
dtc_instance_ctrl_t g_transfer21_ctrl;

transfer_info_t g_transfer21_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer21_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI5_RXI, };
const transfer_cfg_t g_transfer21_cfg =
{ .p_info = &g_transfer21_info, .p_extend = &g_transfer21_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer21 =
{ .p_ctrl = &g_transfer21_ctrl, .p_cfg = &g_transfer21_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer3_ctrl;

transfer_info_t g_transfer3_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer3_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI5_TXI, };
const transfer_cfg_t g_transfer3_cfg =
{ .p_info = &g_transfer3_info, .p_extend = &g_transfer3_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer3 =
{ .p_ctrl = &g_transfer3_ctrl, .p_cfg = &g_transfer3_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart0_ctrl;

baud_setting_t g_uart0_baud_setting =
        {
        /* Baud rate calculated with 0.469% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart0_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart0_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_HARDWARE_CTSRTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 5, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          uart_callback,
  .p_context = NULL, .p_extend = &g_uart0_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer3)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer3,
#endif
#if (RA_NOT_DEFINED == g_transfer21)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer21,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI5_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI5_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI5_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI5_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI5_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI5_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI5_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI5_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_sci };
adpcm_decoder_instance_ctrl_t g_adpcm_decoder0_ctrl;

const adpcm_decoder_cfg_t g_adpcm_decoder0_cfg =
{ .p_extend = NULL, };

/* Instance structure to use ADPCM Audio decoder module. */
const adpcm_decoder_instance_t g_adpcm_decoder0 =
{ .p_ctrl = &g_adpcm_decoder0_ctrl, .p_cfg = &g_adpcm_decoder0_cfg, .p_api = &g_adpcm_decoder_on_adpcm_decoder };
dtc_instance_ctrl_t g_transfer7_ctrl;

transfer_info_t g_transfer7_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_DESTINATION,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer7_cfg_extend =
{ .activation_source = VECTOR_NUMBER_IIC0_RXI, };
const transfer_cfg_t g_transfer7_cfg =
{ .p_info = &g_transfer7_info, .p_extend = &g_transfer7_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer7 =
{ .p_ctrl = &g_transfer7_ctrl, .p_cfg = &g_transfer7_cfg, .p_api = &g_transfer_on_dtc };
dtc_instance_ctrl_t g_transfer5_ctrl;

transfer_info_t g_transfer5_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

const dtc_extended_cfg_t g_transfer5_cfg_extend =
{ .activation_source = VECTOR_NUMBER_IIC0_TXI, };
const transfer_cfg_t g_transfer5_cfg =
{ .p_info = &g_transfer5_info, .p_extend = &g_transfer5_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer5 =
{ .p_ctrl = &g_transfer5_ctrl, .p_cfg = &g_transfer5_cfg, .p_api = &g_transfer_on_dtc };
iic_master_instance_ctrl_t g_i2c_master0_ctrl;
const iic_master_extended_cfg_t g_i2c_master0_extend =
{ .timeout_mode = IIC_MASTER_TIMEOUT_MODE_SHORT, .timeout_scl_low = IIC_MASTER_TIMEOUT_SCL_LOW_ENABLED,
/* Actual calculated bitrate: 98425. Actual calculated duty cycle: 50%. */.clock_settings.brl_value = 28,
  .clock_settings.brh_value = 28, .clock_settings.cks_value = 3, };
const i2c_master_cfg_t g_i2c_master0_cfg =
{ .channel = 0, .rate = I2C_MASTER_RATE_STANDARD, .slave = 0x54, .addr_mode = I2C_MASTER_ADDR_MODE_7BIT,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer5)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer5,
#endif
#if (RA_NOT_DEFINED == g_transfer7)
                .p_transfer_rx       = NULL,
#else
  .p_transfer_rx = &g_transfer7,
#endif
#undef RA_NOT_DEFINED
  .p_callback = i2c_callback,
  .p_context = NULL,
#if defined(VECTOR_NUMBER_IIC0_RXI)
    .rxi_irq             = VECTOR_NUMBER_IIC0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC0_TXI)
    .txi_irq             = VECTOR_NUMBER_IIC0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC0_TEI)
    .tei_irq             = VECTOR_NUMBER_IIC0_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_IIC0_ERI)
    .eri_irq             = VECTOR_NUMBER_IIC0_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
  .ipl = (12),
  .p_extend = &g_i2c_master0_extend, };
/* Instance structure to use this module. */
const i2c_master_instance_t g_i2c_master0 =
{ .p_ctrl = &g_i2c_master0_ctrl, .p_cfg = &g_i2c_master0_cfg, .p_api = &g_i2c_master_on_iic };
icu_instance_ctrl_t g_external_irq0_ctrl;
const external_irq_cfg_t g_external_irq0_cfg =
{ .channel = 2,
  .trigger = EXTERNAL_IRQ_TRIG_RISING,
  .filter_enable = false,
  .pclk_div = EXTERNAL_IRQ_PCLK_DIV_BY_64,
  .p_callback = isr_irq0,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ2)
    .irq                 = VECTOR_NUMBER_ICU_IRQ2,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq0 =
{ .p_ctrl = &g_external_irq0_ctrl, .p_cfg = &g_external_irq0_cfg, .p_api = &g_external_irq_on_icu };
rtc_instance_ctrl_t g_rtc0_ctrl;
const rtc_error_adjustment_cfg_t g_rtc0_err_cfg =
{ .adjustment_mode = RTC_ERROR_ADJUSTMENT_MODE_AUTOMATIC,
  .adjustment_period = RTC_ERROR_ADJUSTMENT_PERIOD_10_SECOND,
  .adjustment_type = RTC_ERROR_ADJUSTMENT_NONE,
  .adjustment_value = 0, };
const rtc_cfg_t g_rtc0_cfg =
{ .clock_source = RTC_CLOCK_SOURCE_LOCO, .freq_compare_value_loco = 255, .p_err_cfg = &g_rtc0_err_cfg, .p_callback =
          rtc_callback,
  .p_context = NULL, .alarm_ipl = (12), .periodic_ipl = (12), .carry_ipl = (12),
#if defined(VECTOR_NUMBER_RTC_ALARM)
    .alarm_irq               = VECTOR_NUMBER_RTC_ALARM,
#else
  .alarm_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_RTC_PERIOD)
    .periodic_irq            = VECTOR_NUMBER_RTC_PERIOD,
#else
  .periodic_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_RTC_CARRY)
    .carry_irq               = VECTOR_NUMBER_RTC_CARRY,
#else
  .carry_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const rtc_instance_t g_rtc0 =
{ .p_ctrl = &g_rtc0_ctrl, .p_cfg = &g_rtc0_cfg, .p_api = &g_rtc_on_rtc };
agt_instance_ctrl_t g_timer1_ctrl;
const agt_extended_cfg_t g_timer1_extend =
{ .count_source = AGT_CLOCK_PCLKB,
  .agto = AGT_PIN_CFG_DISABLED,
  .agtoab_settings_b.agtoa = AGT_PIN_CFG_DISABLED,
  .agtoab_settings_b.agtob = AGT_PIN_CFG_DISABLED,
  .measurement_mode = AGT_MEASURE_DISABLED,
  .agtio_filter = AGT_AGTIO_FILTER_NONE,
  .enable_pin = AGT_ENABLE_PIN_NOT_USED,
  .trigger_edge = AGT_TRIGGER_EDGE_RISING, };
const timer_cfg_t g_timer1_cfg =
{ .mode = TIMER_MODE_PWM,
/* Actual period: 0.01048576 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0x10000,
  .duty_cycle_counts = 0x8000, .source_div = (timer_source_div_t) 3, .channel = 0, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer1_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_AGT0_INT)
    .cycle_end_irq       = VECTOR_NUMBER_AGT0_INT,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer1 =
{ .p_ctrl = &g_timer1_ctrl, .p_cfg = &g_timer1_cfg, .p_api = &g_timer_on_agt };
gpt_instance_ctrl_t g_timer0_ctrl;
#if 1
const gpt_extended_pwm_cfg_t g_timer0_pwm_extend =
{ .trough_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_COUNTER_UNDERFLOW)
    .trough_irq          = VECTOR_NUMBER_GPT1_COUNTER_UNDERFLOW,
#else
  .trough_irq = FSP_INVALID_VECTOR,
#endif
  .poeg_link = GPT_POEG_LINK_POEG0,
  .output_disable = GPT_OUTPUT_DISABLE_NONE,
  .adc_trigger = GPT_ADC_TRIGGER_NONE,
  .dead_time_count_up = 0,
  .dead_time_count_down = 0,
  .adc_a_compare_match = 0,
  .adc_b_compare_match = 0,
  .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
  .interrupt_skip_count = GPT_INTERRUPT_SKIP_COUNT_0,
  .interrupt_skip_adc = GPT_INTERRUPT_SKIP_ADC_NONE,
  .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
  .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED, };
#endif
const gpt_extended_cfg_t g_timer0_extend =
        { .gtioca =
        { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_HIGH },
          .gtiocb =
          { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (BSP_IRQ_DISABLED), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_A)
    .capture_a_irq       = VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_A,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_B)
    .capture_b_irq       = VECTOR_NUMBER_GPT1_CAPTURE_COMPARE_B,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 1
          .p_pwm_cfg = &g_timer0_pwm_extend,
#else
    .p_pwm_cfg                   = NULL,
#endif
#if 0
    .gtior_setting.gtior_b.gtioa  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.oadflt = (uint32_t) GPT_PIN_LEVEL_HIGH,
    .gtior_setting.gtior_b.oahld  = 0U,
    .gtior_setting.gtior_b.oae    = (uint32_t) true,
    .gtior_setting.gtior_b.oadf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfaen  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsa  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
    .gtior_setting.gtior_b.gtiob  = (0U << 4U) | (0U << 2U) | (0U << 0U),
    .gtior_setting.gtior_b.obdflt = (uint32_t) GPT_PIN_LEVEL_LOW,
    .gtior_setting.gtior_b.obhld  = 0U,
    .gtior_setting.gtior_b.obe    = (uint32_t) false,
    .gtior_setting.gtior_b.obdf   = (uint32_t) GPT_GTIOC_DISABLE_PROHIBITED,
    .gtior_setting.gtior_b.nfben  = ((uint32_t) GPT_CAPTURE_FILTER_NONE & 1U),
    .gtior_setting.gtior_b.nfcsb  = ((uint32_t) GPT_CAPTURE_FILTER_NONE >> 1U),
#else
          .gtior_setting.gtior = 0U,
#endif
        };
const timer_cfg_t g_timer0_cfg =
{ .mode = TIMER_MODE_PWM,
/* Actual period: 0.00204081 seconds. Actual duty: 49.999754999240494%. */.period_counts = (uint32_t) 0x31d31,
  .duty_cycle_counts = 0x18e98, .source_div = (timer_source_div_t) 0, .channel = 1, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer0_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer0 =
{ .p_ctrl = &g_timer0_ctrl, .p_cfg = &g_timer0_cfg, .p_api = &g_timer_on_gpt };
dac_instance_ctrl_t g_dac0_ctrl;
const dac_extended_cfg_t g_dac0_ext_cfg =
{ .enable_charge_pump = 0,
  .data_format = DAC_DATA_FORMAT_FLUSH_RIGHT,
  .output_amplifier_enabled = 1,
  .internal_output_enabled = false, };
const dac_cfg_t g_dac0_cfg =
{ .channel = 0, .ad_da_synchronized = false, .p_extend = &g_dac0_ext_cfg };
/* Instance structure to use this module. */
const dac_instance_t g_dac0 =
{ .p_ctrl = &g_dac0_ctrl, .p_cfg = &g_dac0_cfg, .p_api = &g_dac_on_dac };
adc_instance_ctrl_t g_adc0_ctrl;
const adc_extended_cfg_t g_adc0_cfg_extend =
{ .add_average_count = ADC_ADD_OFF,
  .clearing = ADC_CLEAR_AFTER_READ_ON,
  .trigger_group_b = ADC_TRIGGER_SYNC_ELC,
  .double_trigger_mode = ADC_DOUBLE_TRIGGER_DISABLED,
  .adc_vref_control = ADC_VREF_CONTROL_VREFH,
  .enable_adbuf = 0,
#if defined(VECTOR_NUMBER_ADC0_WINDOW_A)
    .window_a_irq        = VECTOR_NUMBER_ADC0_WINDOW_A,
#else
  .window_a_irq = FSP_INVALID_VECTOR,
#endif
  .window_a_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC0_WINDOW_B)
    .window_b_irq      = VECTOR_NUMBER_ADC0_WINDOW_B,
#else
  .window_b_irq = FSP_INVALID_VECTOR,
#endif
  .window_b_ipl = (BSP_IRQ_DISABLED), };
const adc_cfg_t g_adc0_cfg =
{ .unit = 0, .mode = ADC_MODE_SINGLE_SCAN, .resolution = ADC_RESOLUTION_12_BIT, .alignment =
          (adc_alignment_t) ADC_ALIGNMENT_RIGHT,
  .trigger = ADC_TRIGGER_SOFTWARE, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_adc0_cfg_extend,
#if defined(VECTOR_NUMBER_ADC0_SCAN_END)
    .scan_end_irq        = VECTOR_NUMBER_ADC0_SCAN_END,
#else
  .scan_end_irq = FSP_INVALID_VECTOR,
#endif
  .scan_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC0_SCAN_END_B)
    .scan_end_b_irq      = VECTOR_NUMBER_ADC0_SCAN_END_B,
#else
  .scan_end_b_irq = FSP_INVALID_VECTOR,
#endif
  .scan_end_b_ipl = (BSP_IRQ_DISABLED), };
#if ((0) | (0))
const adc_window_cfg_t g_adc0_window_cfg =
{
    .compare_mask        =  0,
    .compare_mode_mask   =  0,
    .compare_cfg         = (0) | (0) | (0) | (ADC_COMPARE_CFG_EVENT_OUTPUT_OR),
    .compare_ref_low     = 0,
    .compare_ref_high    = 0,
    .compare_b_channel   = (ADC_WINDOW_B_CHANNEL_0),
    .compare_b_mode      = (ADC_WINDOW_B_MODE_LESS_THAN_OR_OUTSIDE),
    .compare_b_ref_low   = 0,
    .compare_b_ref_high  = 0,
};
#endif
const adc_channel_cfg_t g_adc0_channel_cfg =
        { .scan_mask = ADC_MASK_CHANNEL_0 | ADC_MASK_CHANNEL_1 | ADC_MASK_CHANNEL_2 | ADC_MASK_CHANNEL_4
                | ADC_MASK_CHANNEL_5 | ADC_MASK_CHANNEL_6 | ADC_MASK_CHANNEL_14 | ADC_MASK_CHANNEL_15 | 0,
          .scan_mask_group_b = 0, .priority_group_a = ADC_GROUP_A_PRIORITY_OFF, .add_mask = 0, .sample_hold_mask = 0, .sample_hold_states =
                  24,
#if ((0) | (0))
    .p_window_cfg        = (adc_window_cfg_t *) &g_adc0_window_cfg,
#else
          .p_window_cfg = NULL,
#endif
        };
/* Instance structure to use this module. */
const adc_instance_t g_adc0 =
{ .p_ctrl = &g_adc0_ctrl, .p_cfg = &g_adc0_cfg, .p_channel_cfg = &g_adc0_channel_cfg, .p_api = &g_adc_on_adc };
void g_hal_init(void)
{
    g_common_init ();
}
