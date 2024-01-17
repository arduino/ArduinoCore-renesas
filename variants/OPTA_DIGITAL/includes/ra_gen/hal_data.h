/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_flash_lp.h"
#include "r_flash_api.h"
#include "rm_vee_flash.h"
#include "r_can.h"
#include "r_can_api.h"
#include "r_doc.h"
#include "r_doc_api.h"
#include "r_kint.h"
#include "r_keymatrix_api.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_iic_slave.h"
#include "r_i2c_slave_api.h"
#include "r_dmac.h"
#include "r_transfer_api.h"
#include "r_sci_i2c.h"
#include "r_i2c_master_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "r_sci_spi.h"
#include "r_spi_api.h"
#include "r_agt.h"
#include "r_timer_api.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_rtc.h"
#include "r_rtc_api.h"
#include "r_dac.h"
#include "r_dac_api.h"
#include "r_usb_basic.h"
#include "r_usb_basic_api.h"
#include "r_usb_pcdc_api.h"
#include "r_lpm.h"
#include "r_lpm_api.h"
#include "r_wdt.h"
#include "r_wdt_api.h"
#include "r_spi.h"
#include "r_iic_master.h"
#include "r_i2c_master_api.h"
#include "r_adc.h"
#include "r_adc_api.h"
FSP_HEADER
/* Flash on Flash LP Instance. */
extern const flash_instance_t g_flash0;

/** Access the Flash LP instance using these structures when calling API functions directly (::p_api is not used). */
extern flash_lp_instance_ctrl_t g_flash0_ctrl;
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
/** CAN on CAN Instance. */
extern const can_instance_t g_can0;
/** Access the CAN instance using these structures when calling API functions directly (::p_api is not used). */
extern can_instance_ctrl_t g_can0_ctrl;
extern const can_cfg_t g_can0_cfg;
extern const can_extended_cfg_t g_can0_extended_cfg;

#ifndef can_callback
void can_callback(can_callback_args_t *p_args);
#endif
#define CAN_NO_OF_MAILBOXES_g_can0 (32)
extern const doc_instance_t g_doc0;

/** Access the DOC instance using these structures when calling API functions directly (::p_api is not used). */
extern doc_instance_ctrl_t g_doc0_ctrl;
extern const doc_cfg_t g_doc0_cfg;

#ifndef NULL
void NULL(doc_callback_args_t *p_args);
#endif
/** Key Matrix on KINT Instance. */
extern const keymatrix_instance_t g_kint0;

/** Access the KINT instance using these structures when calling API functions directly (::p_api is not used). */
extern kint_instance_ctrl_t g_kint0_ctrl;
extern const keymatrix_cfg_t g_kint0_cfg;

/** Key Matrix on KINT user callback. */
#ifndef kint_callback
void kint_callback(keymatrix_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer23;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer23_ctrl;
extern const transfer_cfg_t g_transfer23_cfg;
/** I2C Slave on IIC Instance. */
extern const i2c_slave_instance_t g_i2c_slave0;

/** Access the I2C Slave instance using these structures when calling API functions directly (::p_api is not used). */
extern iic_slave_instance_ctrl_t g_i2c_slave0_ctrl;
extern const i2c_slave_cfg_t g_i2c_slave0_cfg;

#ifndef NULL
void NULL(i2c_slave_callback_args_t *p_args);
#endif
/* Transfer on DMAC Instance. */
extern const transfer_instance_t g_transfer22;

/** Access the DMAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dmac_instance_ctrl_t g_transfer22_ctrl;
extern const transfer_cfg_t g_transfer22_cfg;

#ifndef pippo
void pippo(dmac_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer8;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer8_ctrl;
extern const transfer_cfg_t g_transfer8_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer7;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer7_ctrl;
extern const transfer_cfg_t g_transfer7_cfg;
extern const i2c_master_cfg_t g_i2c2_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c2;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c2_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c2_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer21;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer21_ctrl;
extern const transfer_cfg_t g_transfer21_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer4;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer4_ctrl;
extern const transfer_cfg_t g_transfer4_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart0;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart0_ctrl;
extern const uart_cfg_t g_uart0_cfg;
extern const sci_uart_extended_cfg_t g_uart0_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer20;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer20_ctrl;
extern const transfer_cfg_t g_transfer20_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer19;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer19_ctrl;
extern const transfer_cfg_t g_transfer19_cfg;
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
extern const transfer_instance_t g_transfer18;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer18_ctrl;
extern const transfer_cfg_t g_transfer18_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer17;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer17_ctrl;
extern const transfer_cfg_t g_transfer17_cfg;
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
extern const transfer_instance_t g_transfer16;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer16_ctrl;
extern const transfer_cfg_t g_transfer16_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer15;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer15_ctrl;
extern const transfer_cfg_t g_transfer15_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_spi1;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_spi1_ctrl;
extern const spi_cfg_t g_spi1_cfg;

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
extern const i2c_master_cfg_t g_i2c3_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c3;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c3_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c3_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer6;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer6_ctrl;
extern const transfer_cfg_t g_transfer6_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer5;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer5_ctrl;
extern const transfer_cfg_t g_transfer5_cfg;
extern const i2c_master_cfg_t g_i2c1_cfg;
/* I2C on SCI Instance. */
extern const i2c_master_instance_t g_i2c1;
#ifndef sci_i2c_callback
void sci_i2c_callback(i2c_master_callback_args_t *p_args);
#endif

extern const sci_i2c_extended_cfg_t g_i2c1_cfg_extend;
extern sci_i2c_instance_ctrl_t g_i2c1_ctrl;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer3;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer3_ctrl;
extern const transfer_cfg_t g_transfer3_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer2;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer2_ctrl;
extern const transfer_cfg_t g_transfer2_cfg;
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
extern const transfer_instance_t g_transfer1;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer1_ctrl;
extern const transfer_cfg_t g_transfer1_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer0_ctrl;
extern const transfer_cfg_t g_transfer0_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart1;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart1_ctrl;
extern const uart_cfg_t g_uart1_cfg;
extern const sci_uart_extended_cfg_t g_uart1_cfg_extend;

#ifndef uart_callback
void uart_callback(uart_callback_args_t *p_args);
#endif
/** AGT Timer Instance */
extern const timer_instance_t g_timer10;

/** Access the AGT instance using these structures when calling API functions directly (::p_api is not used). */
extern agt_instance_ctrl_t g_timer10_ctrl;
extern const timer_cfg_t g_timer10_cfg;

#ifndef timer10_callback
void timer10_callback(timer_callback_args_t *p_args);
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
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer1;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer1_ctrl;
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
/* RTC Instance. */
extern const rtc_instance_t g_rtc0;

/** Access the RTC instance using these structures when calling API functions directly (::p_api is not used). */
extern rtc_instance_ctrl_t g_rtc0_ctrl;
extern const rtc_cfg_t g_rtc0_cfg;

#ifndef rtc_callback
void rtc_callback(rtc_callback_args_t *p_args);
#endif
/** DAC on DAC Instance. */
extern const dac_instance_t g_dac0;

/** Access the DAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dac_instance_ctrl_t g_dac0_ctrl;
extern const dac_cfg_t g_dac0_cfg;
/* Basic on USB Instance. */
extern const usb_instance_t g_basic0;

/** Access the USB instance using these structures when calling API functions directly (::p_api is not used). */
extern usb_instance_ctrl_t g_basic0_ctrl;
extern const usb_cfg_t g_basic0_cfg;

#ifndef NULL
void NULL(void*);
#endif

#if 2 == BSP_CFG_RTOS
#ifndef NULL
void NULL(usb_event_info_t *, usb_hdl_t, usb_onoff_t);
#endif
#endif
/** CDC Driver on USB Instance. */
/** lpm Instance */
extern const lpm_instance_t g_lpm0;

/** Access the LPM instance using these structures when calling API functions directly (::p_api is not used). */
extern lpm_instance_ctrl_t g_lpm0_ctrl;
extern const lpm_cfg_t g_lpm0_cfg;
/** WDT on WDT Instance. */
extern const wdt_instance_t g_wdt0;

/** Access the WDT instance using these structures when calling API functions directly (::p_api is not used). */
extern wdt_instance_ctrl_t g_wdt0_ctrl;
extern const wdt_cfg_t g_wdt0_cfg;

#ifndef NULL
void NULL(wdt_callback_args_t *p_args);
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
extern const spi_instance_t g_spi0;

/** Access the SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern spi_instance_ctrl_t g_spi0_ctrl;
extern const spi_cfg_t g_spi0_cfg;

/** Callback used by SPI Instance. */
#ifndef spi_callback
void spi_callback(spi_callback_args_t *p_args);
#endif

#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer9)
    #define g_spi0_P_TRANSFER_TX (NULL)
#else
#define g_spi0_P_TRANSFER_TX (&g_transfer9)
#endif
#if (RA_NOT_DEFINED == g_transfer10)
    #define g_spi0_P_TRANSFER_RX (NULL)
#else
#define g_spi0_P_TRANSFER_RX (&g_transfer10)
#endif
#undef RA_NOT_DEFINED
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer14;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer14_ctrl;
extern const transfer_cfg_t g_transfer14_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer13;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer13_ctrl;
extern const transfer_cfg_t g_transfer13_cfg;
/* I2C Master on IIC Instance. */
extern const i2c_master_instance_t g_i2c_master0;

/** Access the I2C Master instance using these structures when calling API functions directly (::p_api is not used). */
extern iic_master_instance_ctrl_t g_i2c_master0_ctrl;
extern const i2c_master_cfg_t g_i2c_master0_cfg;

#ifndef i2c_callback
void i2c_callback(i2c_master_callback_args_t *p_args);
#endif
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
#define ADC_DMAC_CHANNELS_PER_BLOCK_NULL  6
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
