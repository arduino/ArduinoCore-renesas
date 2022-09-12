#include "bsp_api.h"

#ifdef OLD_INITIALIZATION
BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
{
    [0] = iic_master_rxi_isr, /* IIC1 RXI (Receive data full) */
    [1] = iic_master_txi_isr, /* IIC1 TXI (Transmit data empty) */
    [2] = iic_master_tei_isr, /* IIC1 TEI (Transmit end) */
    [3] = iic_master_eri_isr, /* IIC1 ERI (Transfer error) */
    [4] = spi_rxi_isr, /* SPI1 RXI (Receive buffer full) */
    [5] = spi_txi_isr, /* SPI1 TXI (Transmit buffer empty) */
    [6] = spi_tei_isr, /* SPI1 TEI (Transmission complete event) */
    [7] = spi_eri_isr, /* SPI1 ERI (Error) */
    [8] = r_icu_isr, /* ICU IRQ0 (External pin interrupt 0) */
    [9] = r_icu_isr, /* ICU IRQ1 (External pin interrupt 1) */
    [10] = usbfs_interrupt_handler, /* USBFS INT (USBFS interrupt) */
    [11] = usbfs_resume_handler, /* USBFS RESUME (USBFS resume interrupt) */
    [12] = usbfs_d0fifo_handler, /* USBFS FIFO 0 (DMA transfer request 0) */
    [13] = usbfs_d1fifo_handler, /* USBFS FIFO 1 (DMA transfer request 1) */
    [14] = rtc_alarm_periodic_isr, /* RTC ALARM (Alarm interrupt) */
    [15] = rtc_alarm_periodic_isr, /* RTC PERIOD (Periodic interrupt) */
    [16] = rtc_carry_isr, /* RTC CARRY (Carry interrupt) */
    [17] = agt_int_isr, /* AGT0 INT (AGT interrupt) */
    [18] = sci_spi_rxi_isr, /* SCI0 RXI (Receive data full) */
    [19] = sci_spi_txi_isr, /* SCI0 TXI (Transmit data empty) */
    [20] = sci_spi_tei_isr, /* SCI0 TEI (Transmit end) */
    [21] = sci_spi_eri_isr, /* SCI0 ERI (Receive error) */
    [22] = sci_i2c_rxi_isr, /* SCI1 RXI (Received data full) */
    [23] = sci_i2c_txi_isr, /* SCI1 TXI (Transmit data empty) */
    [24] = sci_i2c_tei_isr, /* SCI1 TEI (Transmit end) */
    [25] = sci_spi_eri_isr, /* SCI1 ERI (Receive error) */
    [26] = defaltHandler, /* SCI2 TXI (Transmit data empty) */
    [27] = defaltHandler, /* SCI2 TEI (Transmit end) */
    [28] = defaltHandler, /* SCI2 RXI (Received data full) */
    [29] = defaltHandler, /* SCI2 ERI (Receive error) */
    [30] = iic_slave_rxi_isr, /* IIC0 RXI (Receive data full) */
    [31] = iic_slave_txi_isr, /* IIC0 TXI (Transmit data empty) */
};

const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
{
    [0] = BSP_PRV_IELS_ENUM(EVENT_IIC1_RXI), /* IIC1 RXI (Receive data full) */
    [1] = BSP_PRV_IELS_ENUM(EVENT_IIC1_TXI), /* IIC1 TXI (Transmit data empty) */
    [2] = BSP_PRV_IELS_ENUM(EVENT_IIC1_TEI), /* IIC1 TEI (Transmit end) */
    [3] = BSP_PRV_IELS_ENUM(EVENT_IIC1_ERI), /* IIC1 ERI (Transfer error) */
    [4] = BSP_PRV_IELS_ENUM(EVENT_SPI1_RXI), /* SPI1 RXI (Receive buffer full) */
    [5] = BSP_PRV_IELS_ENUM(EVENT_SPI1_TXI), /* SPI1 TXI (Transmit buffer empty) */
    [6] = BSP_PRV_IELS_ENUM(EVENT_SPI1_TEI), /* SPI1 TEI (Transmission complete event) */
    [7] = BSP_PRV_IELS_ENUM(EVENT_SPI1_ERI), /* SPI1 ERI (Error) */
    [8] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ0), /* ICU IRQ0 (External pin interrupt 0) */
    [9] = BSP_PRV_IELS_ENUM(EVENT_ICU_IRQ1), /* ICU IRQ1 (External pin interrupt 1) */
    [10] = BSP_PRV_IELS_ENUM(EVENT_USBFS_INT), /* USBFS INT (USBFS interrupt) */
    [11] = BSP_PRV_IELS_ENUM(EVENT_USBFS_RESUME), /* USBFS RESUME (USBFS resume interrupt) */
    [12] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_0), /* USBFS FIFO 0 (DMA transfer request 0) */
    [13] = BSP_PRV_IELS_ENUM(EVENT_USBFS_FIFO_1), /* USBFS FIFO 1 (DMA transfer request 1) */
    [14] = BSP_PRV_IELS_ENUM(EVENT_RTC_ALARM), /* RTC ALARM (Alarm interrupt) */
    [15] = BSP_PRV_IELS_ENUM(EVENT_RTC_PERIOD), /* RTC PERIOD (Periodic interrupt) */
    [16] = BSP_PRV_IELS_ENUM(EVENT_RTC_CARRY), /* RTC CARRY (Carry interrupt) */
    [17] = BSP_PRV_IELS_ENUM(EVENT_AGT0_INT), /* AGT0 INT (AGT interrupt) */
    [18] = BSP_PRV_IELS_ENUM(EVENT_SCI0_RXI), /* SCI0 RXI (Receive data full) */
    [19] = BSP_PRV_IELS_ENUM(EVENT_SCI0_TXI), /* SCI0 TXI (Transmit data empty) */
    [20] = BSP_PRV_IELS_ENUM(EVENT_SCI0_TEI), /* SCI0 TEI (Transmit end) */
    [21] = BSP_PRV_IELS_ENUM(EVENT_SCI0_ERI), /* SCI0 ERI (Receive error) */
    [22] = BSP_PRV_IELS_ENUM(EVENT_SCI1_RXI), /* SCI1 RXI (Received data full) */
    [23] = BSP_PRV_IELS_ENUM(EVENT_SCI1_TXI), /* SCI1 TXI (Transmit data empty) */
    [24] = BSP_PRV_IELS_ENUM(EVENT_SCI1_TEI), /* SCI1 TEI (Transmit end) */
    [25] = BSP_PRV_IELS_ENUM(EVENT_SCI1_ERI), /* SCI1 ERI (Receive error) */
    [26] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TXI), /* SCI2 TXI (Transmit data empty) */
    [27] = BSP_PRV_IELS_ENUM(EVENT_SCI2_TEI), /* SCI2 TEI (Transmit end) */
    [28] = BSP_PRV_IELS_ENUM(EVENT_SCI2_RXI), /* SCI2 RXI (Received data full) */
    [29] = BSP_PRV_IELS_ENUM(EVENT_SCI2_ERI), /* SCI2 ERI (Receive error) */
    [30] = BSP_PRV_IELS_ENUM(EVENT_IIC0_RXI), /* IIC0 RXI (Receive data full) */
    [31] = BSP_PRV_IELS_ENUM(EVENT_IIC0_TXI), /* IIC0 TXI (Transmit data empty) */
};

#else
/* default EMPTY interrupt table - it in FLASH at the beginning of the space allowed for the sketch 
   The bootloader is responsible to copy it in RAM and setting the Interrupt vector table to that */
BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) = {NULL};
const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] = {0};
#endif

