/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_spi.h"
#include "r_spi_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "r_sci_i2c.h"
#include "r_i2c_master_api.h"
#include "r_spi.h"
#include "r_canfd.h"
#include "r_can_api.h"
#include "r_agt.h"
#include "r_timer_api.h"
#include "r_dac.h"
#include "r_dac_api.h"
#include "r_i2s_api.h"
#include "r_ssi.h"
#include "r_iic_master.h"
#include "r_i2c_master_api.h"
#include "r_flash_hp.h"
#include "r_flash_api.h"
#include "rm_vee_flash.h"
#include "r_qspi.h"
#include "r_spi_flash_api.h"
#include "r_adc.h"
#include "r_adc_api.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_lpm.h"
#include "r_lpm_api.h"
#include "rm_adpcm_decoder.h"
#include "r_rtc.h"
#include "r_rtc_api.h"
FSP_HEADER
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer74;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer74_ctrl;
extern const transfer_cfg_t g_transfer74_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer73;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer73_ctrl;
extern const transfer_cfg_t g_transfer73_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi11;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi11_ctrl;
extern const spi_cfg_t g_spi11_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sci_spi_callback
void sci_spi_callback(spi_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer72;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer72_ctrl;
extern const transfer_cfg_t g_transfer72_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer71;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer71_ctrl;
extern const transfer_cfg_t g_transfer71_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi10;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi10_ctrl;
extern const spi_cfg_t g_spi10_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sci_spi_callback
void sci_spi_callback(spi_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer70;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer70_ctrl;
extern const transfer_cfg_t g_transfer70_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer69;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer69_ctrl;
extern const transfer_cfg_t g_transfer69_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi9;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi9_ctrl;
extern const spi_cfg_t g_spi9_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sci_spi_callback
void sci_spi_callback(spi_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer68;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer68_ctrl;
extern const transfer_cfg_t g_transfer68_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer67;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer67_ctrl;
extern const transfer_cfg_t g_transfer67_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi8;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi8_ctrl;
extern const spi_cfg_t g_spi8_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sci_spi_callback
void sci_spi_callback(spi_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer66;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer66_ctrl;
extern const transfer_cfg_t g_transfer66_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer65;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer65_ctrl;
extern const transfer_cfg_t g_transfer65_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi7;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi7_ctrl;
extern const spi_cfg_t g_spi7_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sci_spi_callback
void sci_spi_callback(spi_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer64;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer64_ctrl;
extern const transfer_cfg_t g_transfer64_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer63;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer63_ctrl;
extern const transfer_cfg_t g_transfer63_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi6;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi6_ctrl;
extern const spi_cfg_t g_spi6_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sci_spi_callback
void sci_spi_callback(spi_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer62;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer62_ctrl;
extern const transfer_cfg_t g_transfer62_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer61;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer61_ctrl;
extern const transfer_cfg_t g_transfer61_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi5;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi5_ctrl;
extern const spi_cfg_t g_spi5_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sci_spi_callback
void sci_spi_callback(spi_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer60;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer60_ctrl;
extern const transfer_cfg_t g_transfer60_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer59;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer59_ctrl;
extern const transfer_cfg_t g_transfer59_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi4;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi4_ctrl;
extern const spi_cfg_t g_spi4_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sci_spi_callback
void sci_spi_callback(spi_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer58;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer58_ctrl;
extern const transfer_cfg_t g_transfer58_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer57;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer57_ctrl;
extern const transfer_cfg_t g_transfer57_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart9;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart9_ctrl;
extern const uart_cfg_t g_uart9_cfg;
extern const sci_uart_extended_cfg_t g_uart9_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer56;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer56_ctrl;
extern const transfer_cfg_t g_transfer56_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer55;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer55_ctrl;
extern const transfer_cfg_t g_transfer55_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart8;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart8_ctrl;
extern const uart_cfg_t g_uart8_cfg;
extern const sci_uart_extended_cfg_t g_uart8_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer54;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer54_ctrl;
extern const transfer_cfg_t g_transfer54_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer53;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer53_ctrl;
extern const transfer_cfg_t g_transfer53_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart7;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart7_ctrl;
extern const uart_cfg_t g_uart7_cfg;
extern const sci_uart_extended_cfg_t g_uart7_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer52;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer52_ctrl;
extern const transfer_cfg_t g_transfer52_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer51;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer51_ctrl;
extern const transfer_cfg_t g_transfer51_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart6;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart6_ctrl;
extern const uart_cfg_t g_uart6_cfg;
extern const sci_uart_extended_cfg_t g_uart6_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer50;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer50_ctrl;
extern const transfer_cfg_t g_transfer50_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer49;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer49_ctrl;
extern const transfer_cfg_t g_transfer49_cfg;
extern const i2c_master_cfg_t g_i2c12_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c12;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c12_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c12_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer48;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer48_ctrl;
extern const transfer_cfg_t g_transfer48_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer47;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer47_ctrl;
extern const transfer_cfg_t g_transfer47_cfg;
extern const i2c_master_cfg_t g_i2c11_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c11;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c11_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c11_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer46;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer46_ctrl;
extern const transfer_cfg_t g_transfer46_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer45;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer45_ctrl;
extern const transfer_cfg_t g_transfer45_cfg;
extern const i2c_master_cfg_t g_i2c10_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c10;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c10_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c10_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer44;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer44_ctrl;
extern const transfer_cfg_t g_transfer44_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer43;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer43_ctrl;
extern const transfer_cfg_t g_transfer43_cfg;
extern const i2c_master_cfg_t g_i2c9_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c9;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c9_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c9_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer42;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer42_ctrl;
extern const transfer_cfg_t g_transfer42_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer41;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer41_ctrl;
extern const transfer_cfg_t g_transfer41_cfg;
extern const i2c_master_cfg_t g_i2c8_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c8;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c8_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c8_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer40;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer40_ctrl;
extern const transfer_cfg_t g_transfer40_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer39;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer39_ctrl;
extern const transfer_cfg_t g_transfer39_cfg;
extern const i2c_master_cfg_t g_i2c7_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c7;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c7_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c7_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer38;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer38_ctrl;
extern const transfer_cfg_t g_transfer38_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer37;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer37_ctrl;
extern const transfer_cfg_t g_transfer37_cfg;
extern const i2c_master_cfg_t g_i2c6_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c6;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c6_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c6_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer36;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer36_ctrl;
extern const transfer_cfg_t g_transfer36_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer35;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer35_ctrl;
extern const transfer_cfg_t g_transfer35_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart5;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart5_ctrl;
extern const uart_cfg_t g_uart5_cfg;
extern const sci_uart_extended_cfg_t g_uart5_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer34;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer34_ctrl;
extern const transfer_cfg_t g_transfer34_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer33;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer33_ctrl;
extern const transfer_cfg_t g_transfer33_cfg;
extern const i2c_master_cfg_t g_i2c5_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c5;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c5_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c5_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer32;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer32_ctrl;
extern const transfer_cfg_t g_transfer32_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer31;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer31_ctrl;
extern const transfer_cfg_t g_transfer31_cfg;
extern const i2c_master_cfg_t g_i2c4_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c4;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c4_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c4_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer30;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer30_ctrl;
extern const transfer_cfg_t g_transfer30_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer29;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer29_ctrl;
extern const transfer_cfg_t g_transfer29_cfg;
extern const i2c_master_cfg_t g_i2c3_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c3;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c3_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c3_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer28;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer28_ctrl;
extern const transfer_cfg_t g_transfer28_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer27;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer27_ctrl;
extern const transfer_cfg_t g_transfer27_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi3;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi3_ctrl;
extern const spi_cfg_t g_spi3_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sci_spi_callback
void sci_spi_callback(spi_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer26;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer26_ctrl;
extern const transfer_cfg_t g_transfer26_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer14;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer14_ctrl;
extern const transfer_cfg_t g_transfer14_cfg;
/** SPI on SPI Instance. */
extern const spi_instance_t g_spi0;

/** Access the SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern spi_instance_ctrl_t g_spi0_ctrl;
extern const spi_cfg_t g_spi0_cfg;

/** Callback used by SPI Instance. */
#ifndef spi_callback
void spi_callback(spi_callback_args_t *p_args);
#endif

#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer14)
    #define g_spi0_P_TRANSFER_TX (NULL)
#else
#define g_spi0_P_TRANSFER_TX (&g_transfer14)
#endif
#if (RA_NOT_DEFINED == g_transfer26)
    #define g_spi0_P_TRANSFER_RX (NULL)
#else
#define g_spi0_P_TRANSFER_RX (&g_transfer26)
#endif
#undef RA_NOT_DEFINED
/** CANFD on CANFD Instance. */
extern const can_instance_t g_canfd0;
/** Access the CANFD instance using these structures when calling API functions directly (::p_api is not used). */
extern canfd_instance_ctrl_t g_canfd0_ctrl;
extern const can_cfg_t g_canfd0_cfg;
extern const canfd_extended_cfg_t g_canfd0_cfg_extend;

#ifndef canfd0_callback
void canfd0_callback(can_callback_args_t *p_args);
#endif

/* Global configuration (referenced by all instances) */
extern canfd_global_cfg_t g_canfd_global_cfg;
/** AGT Timer Instance */
extern const timer_instance_t g_timer10;

/** Access the AGT instance using these structures when calling API functions directly (::p_api is not used). */
extern agt_instance_ctrl_t g_timer10_ctrl;
extern const timer_cfg_t g_timer10_cfg;

#ifndef timer10_callback
void timer10_callback(timer_callback_args_t *p_args);
#endif
/** DAC on DAC Instance. */
extern const dac_instance_t g_dac1;

/** Access the DAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dac_instance_ctrl_t g_dac1_ctrl;
extern const dac_cfg_t g_dac1_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer25;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer25_ctrl;
extern const transfer_cfg_t g_transfer25_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer18;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer18_ctrl;
extern const transfer_cfg_t g_transfer18_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart4;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart4_ctrl;
extern const uart_cfg_t g_uart4_cfg;
extern const sci_uart_extended_cfg_t g_uart4_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer24;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer24_ctrl;
extern const transfer_cfg_t g_transfer24_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer16;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer16_ctrl;
extern const transfer_cfg_t g_transfer16_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart3;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart3_ctrl;
extern const uart_cfg_t g_uart3_cfg;
extern const sci_uart_extended_cfg_t g_uart3_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer23;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer23_ctrl;
extern const transfer_cfg_t g_transfer23_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer17;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer17_ctrl;
extern const transfer_cfg_t g_transfer17_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart2;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart2_ctrl;
extern const uart_cfg_t g_uart2_cfg;
extern const sci_uart_extended_cfg_t g_uart2_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer22;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer22_ctrl;
extern const transfer_cfg_t g_transfer22_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer15;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer15_ctrl;
extern const transfer_cfg_t g_transfer15_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart1;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart1_ctrl;
extern const uart_cfg_t g_uart1_cfg;
extern const sci_uart_extended_cfg_t g_uart1_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer76;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer76_ctrl;
extern const transfer_cfg_t g_transfer76_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer75;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer75_ctrl;
extern const transfer_cfg_t g_transfer75_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi2;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi2_ctrl;
extern const spi_cfg_t g_spi2_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sci_spi_callback
void sci_spi_callback(spi_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer12;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer12_ctrl;
extern const transfer_cfg_t g_transfer12_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer11;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer11_ctrl;
extern const transfer_cfg_t g_transfer11_cfg;
/** SSI Instance. */
extern const i2s_instance_t g_i2s0;

/** Access the SSI instance using these structures when calling API functions directly (::p_api is not used). */
extern ssi_instance_ctrl_t g_i2s0_ctrl;
extern const i2s_cfg_t g_i2s0_cfg;

#ifndef NULL
void NULL(i2s_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer10;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer10_ctrl;
extern const transfer_cfg_t g_transfer10_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer9;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer9_ctrl;
extern const transfer_cfg_t g_transfer9_cfg;
/** SPI on SPI Instance. */
extern const spi_instance_t g_spi1;

/** Access the SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern spi_instance_ctrl_t g_spi1_ctrl;
extern const spi_cfg_t g_spi1_cfg;

/** Callback used by SPI Instance. */
#ifndef spi_callback
void spi_callback(spi_callback_args_t *p_args);
#endif

#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer9)
    #define g_spi1_P_TRANSFER_TX (NULL)
#else
#define g_spi1_P_TRANSFER_TX (&g_transfer9)
#endif
#if (RA_NOT_DEFINED == g_transfer10)
    #define g_spi1_P_TRANSFER_RX (NULL)
#else
#define g_spi1_P_TRANSFER_RX (&g_transfer10)
#endif
#undef RA_NOT_DEFINED
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer6;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer6_ctrl;
extern const transfer_cfg_t g_transfer6_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer4;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer4_ctrl;
extern const transfer_cfg_t g_transfer4_cfg;
/* I2C Master on IIC Instance. */
extern const i2c_master_instance_t g_i2c_master2;

/** Access the I2C Master instance using these structures when calling API functions directly (::p_api is not used). */
extern iic_master_instance_ctrl_t g_i2c_master2_ctrl;
extern const i2c_master_cfg_t g_i2c_master2_cfg;

#ifndef i2c_callback
void i2c_callback(i2c_master_callback_args_t *p_args);
#endif
/** CANFD on CANFD Instance. */
extern const can_instance_t g_canfd1;
/** Access the CANFD instance using these structures when calling API functions directly (::p_api is not used). */
extern canfd_instance_ctrl_t g_canfd1_ctrl;
extern const can_cfg_t g_canfd1_cfg;
extern const canfd_extended_cfg_t g_canfd1_cfg_extend;

#ifndef canfd1_callback
void canfd1_callback(can_callback_args_t *p_args);
#endif

/* Global configuration (referenced by all instances) */
extern canfd_global_cfg_t g_canfd_global_cfg;
/* Flash on Flash HP Instance */
extern const flash_instance_t g_flash0;

/** Access the Flash HP instance using these structures when calling API functions directly (::p_api is not used). */
extern flash_hp_instance_ctrl_t g_flash0_ctrl;
extern const flash_cfg_t g_flash0_cfg;

#ifndef rm_vee_flash_callback
void rm_vee_flash_callback(flash_callback_args_t *p_args);
#endif
extern const rm_vee_instance_t g_vee0;
extern rm_vee_flash_instance_ctrl_t g_vee0_ctrl;
extern const rm_vee_cfg_t g_vee0_cfg;

/** Callback used by VEE Instance. */
#ifndef vee_callback
void vee_callback(rm_vee_callback_args_t *p_args);
#endif
extern const spi_flash_instance_t g_qspi0;
extern qspi_instance_ctrl_t g_qspi0_ctrl;
extern const spi_flash_cfg_t g_qspi0_cfg;
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc1;

/** Access the ADC instance using these structures when calling API functions directly (::p_api is not used). */
extern adc_instance_ctrl_t g_adc1_ctrl;
extern const adc_cfg_t g_adc1_cfg;
extern const adc_channel_cfg_t g_adc1_channel_cfg;

#ifndef NULL
void NULL(adc_callback_args_t *p_args);
#endif

#ifndef NULL
#define ADC_DMAC_CHANNELS_PER_BLOCK_NULL  0
#endif
/** AGT Timer Instance */
extern const timer_instance_t g_timer9;

/** Access the AGT instance using these structures when calling API functions directly (::p_api is not used). */
extern agt_instance_ctrl_t g_timer9_ctrl;
extern const timer_cfg_t g_timer9_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer6;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer6_ctrl;
extern const timer_cfg_t g_timer6_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer5;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer5_ctrl;
extern const timer_cfg_t g_timer5_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer4;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer4_ctrl;
extern const timer_cfg_t g_timer4_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer3;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer3_ctrl;
extern const timer_cfg_t g_timer3_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer2;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer2_ctrl;
extern const timer_cfg_t g_timer2_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer13;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer13_ctrl;
extern const transfer_cfg_t g_transfer13_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer8;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer8_ctrl;
extern const transfer_cfg_t g_transfer8_cfg;
/* I2C Master on IIC Instance. */
extern const i2c_master_instance_t g_i2c_master1;

/** Access the I2C Master instance using these structures when calling API functions directly (::p_api is not used). */
extern iic_master_instance_ctrl_t g_i2c_master1_ctrl;
extern const i2c_master_cfg_t g_i2c_master1_cfg;

#ifndef i2c_callback
void i2c_callback(i2c_master_callback_args_t *p_args);
#endif
/** lpm Instance */
extern const lpm_instance_t g_lpm0;

/** Access the LPM instance using these structures when calling API functions directly (::p_api is not used). */
extern lpm_instance_ctrl_t g_lpm0_ctrl;
extern const lpm_cfg_t g_lpm0_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer21;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer21_ctrl;
extern const transfer_cfg_t g_transfer21_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer3;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer3_ctrl;
extern const transfer_cfg_t g_transfer3_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart0;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart0_ctrl;
extern const uart_cfg_t g_uart0_cfg;
extern const sci_uart_extended_cfg_t g_uart0_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/** ADPCM Audio Decoder Instance */
extern const adpcm_decoder_instance_t g_adpcm_decoder0;

/** Access the ADPCM Audio Decoder instance using these structures
 when calling API functions directly (::p_api is not used). */
extern adpcm_decoder_instance_ctrl_t g_adpcm_decoder0_ctrl;

extern const adpcm_decoder_cfg_t g_adpcm_decoder0_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer7;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer7_ctrl;
extern const transfer_cfg_t g_transfer7_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer5;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer5_ctrl;
extern const transfer_cfg_t g_transfer5_cfg;
/* I2C Master on IIC Instance. */
extern const i2c_master_instance_t g_i2c_master0;

/** Access the I2C Master instance using these structures when calling API functions directly (::p_api is not used). */
extern iic_master_instance_ctrl_t g_i2c_master0_ctrl;
extern const i2c_master_cfg_t g_i2c_master0_cfg;

#ifndef i2c_callback
void i2c_callback(i2c_master_callback_args_t *p_args);
#endif
/* RTC Instance. */
extern const rtc_instance_t g_rtc0;

/** Access the RTC instance using these structures when calling API functions directly (::p_api is not used). */
extern rtc_instance_ctrl_t g_rtc0_ctrl;
extern const rtc_cfg_t g_rtc0_cfg;

#ifndef rtc_callback
void rtc_callback(rtc_callback_args_t *p_args);
#endif
/** AGT Timer Instance */
extern const timer_instance_t g_timer1;

/** Access the AGT instance using these structures when calling API functions directly (::p_api is not used). */
extern agt_instance_ctrl_t g_timer1_ctrl;
extern const timer_cfg_t g_timer1_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer0;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer0_ctrl;
extern const timer_cfg_t g_timer0_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** DAC on DAC Instance. */
extern const dac_instance_t g_dac0;

/** Access the DAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dac_instance_ctrl_t g_dac0_ctrl;
extern const dac_cfg_t g_dac0_cfg;
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc0;

/** Access the ADC instance using these structures when calling API functions directly (::p_api is not used). */
extern adc_instance_ctrl_t g_adc0_ctrl;
extern const adc_cfg_t g_adc0_cfg;
extern const adc_channel_cfg_t g_adc0_channel_cfg;

#ifndef NULL
void NULL(adc_callback_args_t *p_args);
#endif

#ifndef NULL
#define ADC_DMAC_CHANNELS_PER_BLOCK_NULL  8
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
