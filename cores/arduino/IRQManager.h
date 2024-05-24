#ifndef ARDUINO_IRQ_MANAGER_H
#define ARDUINO_IRQ_MANAGER_H

#include "analog.h"
#include "bsp_api.h"
#include "pins_arduino.h"
#include "elc_defines.h"

#if ETHERNET_HOWMANY > 0
#include "r_ether_api.h"
#endif

#if SERIAL_HOWMANY > 0
#include "r_uart_api.h"
#define UART_INTERRUPT_COUNT 4
#endif

#if EXT_INTERRUPTS_HOWMANY > 0
#include "r_external_irq_api.h"
#endif

#if I2S_HOWMANY > 0
#include "r_ssi.h"
extern "C" {
void ssi_txi_isr(void);
void ssi_rxi_isr(void);
void ssi_int_isr(void);
}
#endif

#include "r_timer_api.h"

#ifdef ELC_EVENT_DMAC0_INT
#define HAS_DMAC    1
#include "r_dmac.h"
#endif

#include "r_gpt.h"
#include "r_agt.h"

typedef enum {
    IRQ_RTC,
    IRQ_USB,
    IRQ_USB_HS,
    IRQ_AGT,
    IRQ_SCI_UART,
    IRQ_I2C_MASTER,
    IRQ_SCI_I2C_MASTER,
    IRQ_I2C_SLAVE,
    IRQ_EXTERNAL_PIN,
    IRQ_SPI_MASTER,
    IRQ_SCI_SPI_MASTER,
    IRQ_CAN,
    IRQ_ETHERNET,
    IRQ_CANFD,
    IRQ_SDCARD,
    IRQ_I2S
} Peripheral_t;

#if SDCARD_HOWMANY > 0
#include "r_sdhi.h"
#endif


#if RTC_HOWMANY > 0
#include "r_rtc_api.h"
#include "r_rtc.h"

typedef enum {
    RTC_ALARM,
    RTC_PERIODIC,
    RTC_CARRY
} RTCIrqReq_t;

typedef struct rtc_irq {
    rtc_cfg_t *cfg;
    rtc_instance_ctrl_t *ctrl;
    RTCIrqReq_t req;
} RTCIrqCfg_t;
#endif

#if WIRE_HOWMANY > 0
#include "r_i2c_slave_api.h"
#include "r_i2c_master_api.h"
#include "r_iic_master.h"
#include "r_iic_slave.h"

typedef struct i2c_irq_req {
    i2c_master_cfg_t *mcfg;
    i2c_slave_cfg_t  *scfg;
} I2CIrqReq_t;
#define WIRE_MASTER_INTERRUPT_COUNT 4
#define WIRE_SLAVE_INTERRUPT_COUNT 4
#define WIRE_SCI_MASTER_INTERRUPT_COUNT 3
#endif

#if SPI_HOWMANY > 0
#include "r_spi.h"
#include "r_sci_spi.h"

typedef struct spi_master_irq {
    spi_instance_ctrl_t * ctrl;
    spi_cfg_t * cfg;
    uint8_t hw_channel;
} SpiMasterIrqReq_t;

typedef struct sci_spi_master_irq {
    sci_spi_instance_ctrl_t * ctrl;
    spi_cfg_t * cfg;
    uint8_t hw_channel;
} SciSpiMasterIrqReq_t;
#define SPI_INTERRUPT_COUNT 4
#endif

#if CAN_HOWMANY > 0
# include "r_can.h"
typedef struct can_irq {
  can_instance_ctrl_t * ctrl;
  can_cfg_t * cfg;
} CanIrqReq_t;
#define CAN_INTERRUPT_COUNT 3
#endif /* CAN_HOWMANY > 0 */

#if CANFD_HOWMANY > 0
# include "r_canfd.h"
typedef struct canfd_irq {
  canfd_instance_ctrl_t * ctrl;
  can_cfg_t * cfg;
} CanFdIrqReq_t;
#define CANFD_INTERRUPT_COUNT 3
#endif /* CANFD_HOWMANY > 0 */

#define SD_INTERRUPT_COUNT 2

typedef struct usb {
    uint32_t num_of_irqs_required;
    uint32_t address_of_handler;
    uint32_t first_irq_number;
} USBIrqCfg_t;

typedef struct timer {
    timer_cfg_t *base_cfg;
    gpt_extended_cfg_t *gpt_ext_cfg;
    agt_extended_cfg_t *agt_ext_cfg;
} TimerIrqCfg_t;

typedef struct genericIrq {
	IRQn_Type irq;
	uint8_t ipl;
	elc_event_t event;
} GenericIrqCfg_t;


#ifdef __cplusplus
extern "C" {
#endif
/* ISR prototypes */
void iic_master_rxi_isr(void);
void iic_master_txi_isr(void);
void iic_master_tei_isr(void);
void iic_master_eri_isr(void);
void spi_rxi_isr(void);
void spi_txi_isr(void);
void spi_tei_isr(void);
void spi_eri_isr(void);
void r_icu_isr(void);
void usbfs_interrupt_handler(void);
void usbfs_resume_handler(void);
void usbfs_d0fifo_handler(void);
void usbfs_d1fifo_handler(void);
void rtc_alarm_periodic_isr(void);
void rtc_carry_isr(void);
void agt_int_isr(void);
void sci_spi_rxi_isr(void);
void sci_spi_txi_isr(void);
void sci_spi_tei_isr(void);
void sci_spi_eri_isr(void);
void sci_i2c_rxi_isr(void);
void sci_i2c_txi_isr(void);
void sci_i2c_tei_isr(void);
void sci_uart_txi_isr(void);
void sci_uart_tei_isr(void);
void sci_uart_rxi_isr(void);
void sci_uart_eri_isr(void);
void iic_slave_rxi_isr(void);
void iic_slave_txi_isr(void);
void iic_slave_tei_isr(void);
void iic_slave_eri_isr(void);
void gpt_counter_overflow_isr(void);
void gpt_capture_a_isr(void);
void gpt_capture_b_isr(void);
void gpt_counter_underflow_isr(void);
void agt_int_isr(void);
void adc_scan_end_isr (void);
void adc_scan_end_b_isr (void);
void adc_window_compare_isr (void);
void dmac_int_isr(void);
void can_error_isr(void);
void can_rx_isr(void);
void can_tx_isr(void);
void ether_eint_isr (void);
void canfd_error_isr(void);
void canfd_rx_fifo_isr(void);
void canfd_channel_tx_isr(void);
void sdhimmc_dma_req_isr(void);
void sdhimmc_accs_isr(void);
void sdhimmc_card_isr(void);
#ifdef __cplusplus
}
#endif

using Irq_f          = void (*)(void);

class IRQManager {
    public:
    bool addPeripheral(Peripheral_t p, void *cfg);
    static IRQManager& getInstance();
    
#ifdef HAS_DMAC
    /* add DMA interrupt. Channels from 0 to 4 for R4, from 0 to 7 on R6 
       if fnc is nullptr the standard dmac_int_isr is added
       otherwise fnc is the interrupt handler function 
       it returns true if the interrupt is correctly added */
    bool addDMA(dmac_extended_cfg_t &cfg, Irq_f fnc = nullptr);
#endif
    
    bool addGenericInterrupt(GenericIrqCfg_t &cfg, Irq_f fnc = nullptr);
    bool addTimerOverflow(TimerIrqCfg_t &cfg, Irq_f fnc = nullptr);
    bool addTimerUnderflow(TimerIrqCfg_t &cfg, Irq_f fnc = nullptr);
    bool addTimerCompareCaptureA(TimerIrqCfg_t &cfg, Irq_f fnc = nullptr);
    bool addTimerCompareCaptureB(TimerIrqCfg_t &cfg, Irq_f fnc = nullptr);

    bool addADCScanEnd(ADC_Container *adc, Irq_f fnc = nullptr);
    bool addADCScanEndB(ADC_Container *adc, Irq_f fnc = nullptr);
    bool addADCWinCmpA(ADC_Container *adc,  Irq_f fnc = nullptr);
    bool addADCWinCmpB(ADC_Container *adc, Irq_f fnc = nullptr);

    IRQManager(IRQManager const&)               = delete;
    void operator=(IRQManager const&)           = delete;
    ~IRQManager();

    private:
    size_t last_interrupt_index;
    bool set_adc_end_link_event(int li, int ch);
    bool set_adc_end_b_link_event(int li, int ch);
    bool set_adc_win_a_link_event(int li, int ch);
    bool set_adc_win_b_link_event(int li, int ch);

    void set_sci_tx_link_event(int li, int ch);
    void set_sci_rx_link_event(int li, int ch);
    void set_sci_tei_link_event(int li, int ch);
    void set_sci_eri_link_event(int li, int ch);

    void set_iic_tx_link_event(int li, int ch);
    void set_iic_rx_link_event(int li, int ch);
    void set_iic_tei_link_event(int li, int ch);
    void set_iic_eri_link_event(int li, int ch);

    void set_ext_link_event(int li, int ch);
    bool set_agt_link_event(int li, int ch);
    bool set_gpt_over_link_event(int li, int ch);
    bool set_gpt_under_link_event(int li, int ch);
    bool set_gpt_compare_capture_A_link_event(int li, int ch);
    bool set_gpt_compare_capture_B_link_event(int li, int ch);


    void set_spi_tx_link_event(int li, int ch);
    void set_spi_rx_link_event(int li, int ch);
    void set_spi_tei_link_event(int li, int ch);
    void set_spi_eri_link_event(int li, int ch);

    void set_can_error_link_event(int li, int ch);
    void set_can_rx_link_event(int li, int ch);
    void set_can_tx_link_event(int li, int ch);

    void set_canfd_error_link_event(int li, int ch);
    void set_canfd_rx_link_event(int li, int ch);
    void set_canfd_tx_link_event(int li, int ch);

    bool set_dma_link_event(int li, int ch);
    IRQManager();
};

#endif
