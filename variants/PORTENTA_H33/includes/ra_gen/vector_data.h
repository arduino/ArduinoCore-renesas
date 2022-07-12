/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (78)
#endif
/* ISR prototypes */
void rtc_alarm_periodic_isr(void);
void rtc_carry_isr(void);
void r_icu_isr(void);
void iic_master_rxi_isr(void);
void iic_master_txi_isr(void);
void iic_master_tei_isr(void);
void iic_master_eri_isr(void);
void sci_uart_rxi_isr(void);
void sci_uart_txi_isr(void);
void sci_uart_tei_isr(void);
void sci_uart_eri_isr(void);
void fcu_frdyi_isr(void);
void fcu_fiferr_isr(void);
void canfd_error_isr(void);
void canfd_channel_tx_isr(void);
void canfd_rx_fifo_isr(void);
void spi_rxi_isr(void);
void spi_txi_isr(void);
void spi_tei_isr(void);
void spi_eri_isr(void);
void ssi_txi_isr(void);
void ssi_rxi_isr(void);
void ssi_int_isr(void);
void sci_spi_rxi_isr(void);
void sci_spi_txi_isr(void);
void sci_spi_tei_isr(void);
void sci_spi_eri_isr(void);
void agt_int_isr(void);
void ether_eint_isr(void);
void usbfs_interrupt_handler(void);
void usbfs_resume_handler(void);
void usbfs_d0fifo_handler(void);
void usbfs_d1fifo_handler(void);
void usbhs_interrupt_handler(void);
void usbhs_d0fifo_handler(void);
void usbhs_d1fifo_handler(void);
void dmac_int_isr(void);
void sdhimmc_accs_isr(void);
void sdhimmc_card_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_RTC_ALARM ((IRQn_Type) 0) /* RTC ALARM (Alarm interrupt) */
#define RTC_ALARM_IRQn          ((IRQn_Type) 0) /* RTC ALARM (Alarm interrupt) */
#define VECTOR_NUMBER_RTC_PERIOD ((IRQn_Type) 1) /* RTC PERIOD (Periodic interrupt) */
#define RTC_PERIOD_IRQn          ((IRQn_Type) 1) /* RTC PERIOD (Periodic interrupt) */
#define VECTOR_NUMBER_RTC_CARRY ((IRQn_Type) 2) /* RTC CARRY (Carry interrupt) */
#define RTC_CARRY_IRQn          ((IRQn_Type) 2) /* RTC CARRY (Carry interrupt) */
#define VECTOR_NUMBER_ICU_IRQ0 ((IRQn_Type) 3) /* ICU IRQ0 (External pin interrupt 0) */
#define ICU_IRQ0_IRQn          ((IRQn_Type) 3) /* ICU IRQ0 (External pin interrupt 0) */
#define VECTOR_NUMBER_IIC0_RXI ((IRQn_Type) 4) /* IIC0 RXI (Receive data full) */
#define IIC0_RXI_IRQn          ((IRQn_Type) 4) /* IIC0 RXI (Receive data full) */
#define VECTOR_NUMBER_IIC0_TXI ((IRQn_Type) 5) /* IIC0 TXI (Transmit data empty) */
#define IIC0_TXI_IRQn          ((IRQn_Type) 5) /* IIC0 TXI (Transmit data empty) */
#define VECTOR_NUMBER_IIC0_TEI ((IRQn_Type) 6) /* IIC0 TEI (Transmit end) */
#define IIC0_TEI_IRQn          ((IRQn_Type) 6) /* IIC0 TEI (Transmit end) */
#define VECTOR_NUMBER_IIC0_ERI ((IRQn_Type) 7) /* IIC0 ERI (Transfer error) */
#define IIC0_ERI_IRQn          ((IRQn_Type) 7) /* IIC0 ERI (Transfer error) */
#define VECTOR_NUMBER_SCI5_RXI ((IRQn_Type) 8) /* SCI5 RXI (Received data full) */
#define SCI5_RXI_IRQn          ((IRQn_Type) 8) /* SCI5 RXI (Received data full) */
#define VECTOR_NUMBER_SCI5_TXI ((IRQn_Type) 9) /* SCI5 TXI (Transmit data empty) */
#define SCI5_TXI_IRQn          ((IRQn_Type) 9) /* SCI5 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI5_TEI ((IRQn_Type) 10) /* SCI5 TEI (Transmit end) */
#define SCI5_TEI_IRQn          ((IRQn_Type) 10) /* SCI5 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI5_ERI ((IRQn_Type) 11) /* SCI5 ERI (Receive error) */
#define SCI5_ERI_IRQn          ((IRQn_Type) 11) /* SCI5 ERI (Receive error) */
#define VECTOR_NUMBER_ICU_IRQ1 ((IRQn_Type) 12) /* ICU IRQ1 (External pin interrupt 1) */
#define ICU_IRQ1_IRQn          ((IRQn_Type) 12) /* ICU IRQ1 (External pin interrupt 1) */
#define VECTOR_NUMBER_IIC1_RXI ((IRQn_Type) 13) /* IIC1 RXI (Receive data full) */
#define IIC1_RXI_IRQn          ((IRQn_Type) 13) /* IIC1 RXI (Receive data full) */
#define VECTOR_NUMBER_IIC1_TXI ((IRQn_Type) 14) /* IIC1 TXI (Transmit data empty) */
#define IIC1_TXI_IRQn          ((IRQn_Type) 14) /* IIC1 TXI (Transmit data empty) */
#define VECTOR_NUMBER_IIC1_TEI ((IRQn_Type) 15) /* IIC1 TEI (Transmit end) */
#define IIC1_TEI_IRQn          ((IRQn_Type) 15) /* IIC1 TEI (Transmit end) */
#define VECTOR_NUMBER_IIC1_ERI ((IRQn_Type) 16) /* IIC1 ERI (Transfer error) */
#define IIC1_ERI_IRQn          ((IRQn_Type) 16) /* IIC1 ERI (Transfer error) */
#define VECTOR_NUMBER_FCU_FRDYI ((IRQn_Type) 17) /* FCU FRDYI (Flash ready interrupt) */
#define FCU_FRDYI_IRQn          ((IRQn_Type) 17) /* FCU FRDYI (Flash ready interrupt) */
#define VECTOR_NUMBER_FCU_FIFERR ((IRQn_Type) 18) /* FCU FIFERR (Flash access error interrupt) */
#define FCU_FIFERR_IRQn          ((IRQn_Type) 18) /* FCU FIFERR (Flash access error interrupt) */
#define VECTOR_NUMBER_ICU_IRQ2 ((IRQn_Type) 19) /* ICU IRQ2 (External pin interrupt 2) */
#define ICU_IRQ2_IRQn          ((IRQn_Type) 19) /* ICU IRQ2 (External pin interrupt 2) */
#define VECTOR_NUMBER_ICU_IRQ3 ((IRQn_Type) 20) /* ICU IRQ3 (External pin interrupt 3) */
#define ICU_IRQ3_IRQn          ((IRQn_Type) 20) /* ICU IRQ3 (External pin interrupt 3) */
#define VECTOR_NUMBER_ICU_IRQ4 ((IRQn_Type) 21) /* ICU IRQ4 (External pin interrupt 4) */
#define ICU_IRQ4_IRQn          ((IRQn_Type) 21) /* ICU IRQ4 (External pin interrupt 4) */
#define VECTOR_NUMBER_ICU_IRQ6 ((IRQn_Type) 22) /* ICU IRQ6 (External pin interrupt 6) */
#define ICU_IRQ6_IRQn          ((IRQn_Type) 22) /* ICU IRQ6 (External pin interrupt 6) */
#define VECTOR_NUMBER_ICU_IRQ8 ((IRQn_Type) 23) /* ICU IRQ8 (External pin interrupt 8) */
#define ICU_IRQ8_IRQn          ((IRQn_Type) 23) /* ICU IRQ8 (External pin interrupt 8) */
#define VECTOR_NUMBER_ICU_IRQ11 ((IRQn_Type) 24) /* ICU IRQ11 (External pin interrupt 11) */
#define ICU_IRQ11_IRQn          ((IRQn_Type) 24) /* ICU IRQ11 (External pin interrupt 11) */
#define VECTOR_NUMBER_ICU_IRQ13 ((IRQn_Type) 25) /* ICU IRQ13 (External pin interrupt 13) */
#define ICU_IRQ13_IRQn          ((IRQn_Type) 25) /* ICU IRQ13 (External pin interrupt 13) */
#define VECTOR_NUMBER_ICU_IRQ14 ((IRQn_Type) 26) /* ICU IRQ14 (External pin interrupt 14) */
#define ICU_IRQ14_IRQn          ((IRQn_Type) 26) /* ICU IRQ14 (External pin interrupt 14) */
#define VECTOR_NUMBER_CAN1_CHERR ((IRQn_Type) 27) /* CAN1 CHERR (Channel error) */
#define CAN1_CHERR_IRQn          ((IRQn_Type) 27) /* CAN1 CHERR (Channel error) */
#define VECTOR_NUMBER_CAN1_TX ((IRQn_Type) 28) /* CAN1 TX (Transmit interrupt) */
#define CAN1_TX_IRQn          ((IRQn_Type) 28) /* CAN1 TX (Transmit interrupt) */
#define VECTOR_NUMBER_CAN_GLERR ((IRQn_Type) 29) /* CAN GLERR (Global error) */
#define CAN_GLERR_IRQn          ((IRQn_Type) 29) /* CAN GLERR (Global error) */
#define VECTOR_NUMBER_CAN_RXF ((IRQn_Type) 30) /* CAN RXF (Global recieve FIFO interrupt) */
#define CAN_RXF_IRQn          ((IRQn_Type) 30) /* CAN RXF (Global recieve FIFO interrupt) */
#define VECTOR_NUMBER_IIC2_RXI ((IRQn_Type) 31) /* IIC2 RXI (Receive data full) */
#define IIC2_RXI_IRQn          ((IRQn_Type) 31) /* IIC2 RXI (Receive data full) */
#define VECTOR_NUMBER_IIC2_TXI ((IRQn_Type) 32) /* IIC2 TXI (Transmit data empty) */
#define IIC2_TXI_IRQn          ((IRQn_Type) 32) /* IIC2 TXI (Transmit data empty) */
#define VECTOR_NUMBER_IIC2_TEI ((IRQn_Type) 33) /* IIC2 TEI (Transmit end) */
#define IIC2_TEI_IRQn          ((IRQn_Type) 33) /* IIC2 TEI (Transmit end) */
#define VECTOR_NUMBER_IIC2_ERI ((IRQn_Type) 34) /* IIC2 ERI (Transfer error) */
#define IIC2_ERI_IRQn          ((IRQn_Type) 34) /* IIC2 ERI (Transfer error) */
#define VECTOR_NUMBER_SPI1_RXI ((IRQn_Type) 35) /* SPI1 RXI (Receive buffer full) */
#define SPI1_RXI_IRQn          ((IRQn_Type) 35) /* SPI1 RXI (Receive buffer full) */
#define VECTOR_NUMBER_SPI1_TXI ((IRQn_Type) 36) /* SPI1 TXI (Transmit buffer empty) */
#define SPI1_TXI_IRQn          ((IRQn_Type) 36) /* SPI1 TXI (Transmit buffer empty) */
#define VECTOR_NUMBER_SPI1_TEI ((IRQn_Type) 37) /* SPI1 TEI (Transmission complete event) */
#define SPI1_TEI_IRQn          ((IRQn_Type) 37) /* SPI1 TEI (Transmission complete event) */
#define VECTOR_NUMBER_SPI1_ERI ((IRQn_Type) 38) /* SPI1 ERI (Error) */
#define SPI1_ERI_IRQn          ((IRQn_Type) 38) /* SPI1 ERI (Error) */
#define VECTOR_NUMBER_SSI0_TXI ((IRQn_Type) 39) /* SSI0 TXI (Transmit data empty) */
#define SSI0_TXI_IRQn          ((IRQn_Type) 39) /* SSI0 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SSI0_RXI ((IRQn_Type) 40) /* SSI0 RXI (Receive data full) */
#define SSI0_RXI_IRQn          ((IRQn_Type) 40) /* SSI0 RXI (Receive data full) */
#define VECTOR_NUMBER_SSI0_INT ((IRQn_Type) 41) /* SSI0 INT (Error interrupt) */
#define SSI0_INT_IRQn          ((IRQn_Type) 41) /* SSI0 INT (Error interrupt) */
#define VECTOR_NUMBER_SCI4_RXI ((IRQn_Type) 42) /* SCI4 RXI (Received data full) */
#define SCI4_RXI_IRQn          ((IRQn_Type) 42) /* SCI4 RXI (Received data full) */
#define VECTOR_NUMBER_SCI4_TXI ((IRQn_Type) 43) /* SCI4 TXI (Transmit data empty) */
#define SCI4_TXI_IRQn          ((IRQn_Type) 43) /* SCI4 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI4_TEI ((IRQn_Type) 44) /* SCI4 TEI (Transmit end) */
#define SCI4_TEI_IRQn          ((IRQn_Type) 44) /* SCI4 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI4_ERI ((IRQn_Type) 45) /* SCI4 ERI (Receive error) */
#define SCI4_ERI_IRQn          ((IRQn_Type) 45) /* SCI4 ERI (Receive error) */
#define VECTOR_NUMBER_SCI6_RXI ((IRQn_Type) 46) /* SCI6 RXI (Received data full) */
#define SCI6_RXI_IRQn          ((IRQn_Type) 46) /* SCI6 RXI (Received data full) */
#define VECTOR_NUMBER_SCI6_TXI ((IRQn_Type) 47) /* SCI6 TXI (Transmit data empty) */
#define SCI6_TXI_IRQn          ((IRQn_Type) 47) /* SCI6 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI6_TEI ((IRQn_Type) 48) /* SCI6 TEI (Transmit end) */
#define SCI6_TEI_IRQn          ((IRQn_Type) 48) /* SCI6 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI6_ERI ((IRQn_Type) 49) /* SCI6 ERI (Receive error) */
#define SCI6_ERI_IRQn          ((IRQn_Type) 49) /* SCI6 ERI (Receive error) */
#define VECTOR_NUMBER_SCI7_RXI ((IRQn_Type) 50) /* SCI7 RXI (Received data full) */
#define SCI7_RXI_IRQn          ((IRQn_Type) 50) /* SCI7 RXI (Received data full) */
#define VECTOR_NUMBER_SCI7_TXI ((IRQn_Type) 51) /* SCI7 TXI (Transmit data empty) */
#define SCI7_TXI_IRQn          ((IRQn_Type) 51) /* SCI7 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI7_TEI ((IRQn_Type) 52) /* SCI7 TEI (Transmit end) */
#define SCI7_TEI_IRQn          ((IRQn_Type) 52) /* SCI7 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI7_ERI ((IRQn_Type) 53) /* SCI7 ERI (Receive error) */
#define SCI7_ERI_IRQn          ((IRQn_Type) 53) /* SCI7 ERI (Receive error) */
#define VECTOR_NUMBER_SCI8_RXI ((IRQn_Type) 54) /* SCI8 RXI (Received data full) */
#define SCI8_RXI_IRQn          ((IRQn_Type) 54) /* SCI8 RXI (Received data full) */
#define VECTOR_NUMBER_SCI8_TXI ((IRQn_Type) 55) /* SCI8 TXI (Transmit data empty) */
#define SCI8_TXI_IRQn          ((IRQn_Type) 55) /* SCI8 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI8_TEI ((IRQn_Type) 56) /* SCI8 TEI (Transmit end) */
#define SCI8_TEI_IRQn          ((IRQn_Type) 56) /* SCI8 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI8_ERI ((IRQn_Type) 57) /* SCI8 ERI (Receive error) */
#define SCI8_ERI_IRQn          ((IRQn_Type) 57) /* SCI8 ERI (Receive error) */
#define VECTOR_NUMBER_SCI9_RXI ((IRQn_Type) 58) /* SCI9 RXI (Received data full) */
#define SCI9_RXI_IRQn          ((IRQn_Type) 58) /* SCI9 RXI (Received data full) */
#define VECTOR_NUMBER_SCI9_TXI ((IRQn_Type) 59) /* SCI9 TXI (Transmit data empty) */
#define SCI9_TXI_IRQn          ((IRQn_Type) 59) /* SCI9 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI9_TEI ((IRQn_Type) 60) /* SCI9 TEI (Transmit end) */
#define SCI9_TEI_IRQn          ((IRQn_Type) 60) /* SCI9 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI9_ERI ((IRQn_Type) 61) /* SCI9 ERI (Receive error) */
#define SCI9_ERI_IRQn          ((IRQn_Type) 61) /* SCI9 ERI (Receive error) */
#define VECTOR_NUMBER_AGT1_INT ((IRQn_Type) 62) /* AGT1 INT (AGT interrupt) */
#define AGT1_INT_IRQn          ((IRQn_Type) 62) /* AGT1 INT (AGT interrupt) */
#define VECTOR_NUMBER_CAN0_CHERR ((IRQn_Type) 63) /* CAN0 CHERR (Channel error) */
#define CAN0_CHERR_IRQn          ((IRQn_Type) 63) /* CAN0 CHERR (Channel error) */
#define VECTOR_NUMBER_CAN0_TX ((IRQn_Type) 64) /* CAN0 TX (Transmit interrupt) */
#define CAN0_TX_IRQn          ((IRQn_Type) 64) /* CAN0 TX (Transmit interrupt) */
#define VECTOR_NUMBER_EDMAC0_EINT ((IRQn_Type) 65) /* EDMAC0 EINT (EDMAC 0 interrupt) */
#define EDMAC0_EINT_IRQn          ((IRQn_Type) 65) /* EDMAC0 EINT (EDMAC 0 interrupt) */
#define VECTOR_NUMBER_USBFS_INT ((IRQn_Type) 66) /* USBFS INT (USBFS interrupt) */
#define USBFS_INT_IRQn          ((IRQn_Type) 66) /* USBFS INT (USBFS interrupt) */
#define VECTOR_NUMBER_USBFS_RESUME ((IRQn_Type) 67) /* USBFS RESUME (USBFS resume interrupt) */
#define USBFS_RESUME_IRQn          ((IRQn_Type) 67) /* USBFS RESUME (USBFS resume interrupt) */
#define VECTOR_NUMBER_USBFS_FIFO_0 ((IRQn_Type) 68) /* USBFS FIFO 0 (DMA transfer request 0) */
#define USBFS_FIFO_0_IRQn          ((IRQn_Type) 68) /* USBFS FIFO 0 (DMA transfer request 0) */
#define VECTOR_NUMBER_USBFS_FIFO_1 ((IRQn_Type) 69) /* USBFS FIFO 1 (DMA transfer request 1) */
#define USBFS_FIFO_1_IRQn          ((IRQn_Type) 69) /* USBFS FIFO 1 (DMA transfer request 1) */
#define VECTOR_NUMBER_USBHS_USB_INT_RESUME ((IRQn_Type) 70) /* USBHS USB INT RESUME (USBHS interrupt) */
#define USBHS_USB_INT_RESUME_IRQn          ((IRQn_Type) 70) /* USBHS USB INT RESUME (USBHS interrupt) */
#define VECTOR_NUMBER_USBHS_FIFO_0 ((IRQn_Type) 71) /* USBHS FIFO 0 (DMA transfer request 0) */
#define USBHS_FIFO_0_IRQn          ((IRQn_Type) 71) /* USBHS FIFO 0 (DMA transfer request 0) */
#define VECTOR_NUMBER_USBHS_FIFO_1 ((IRQn_Type) 72) /* USBHS FIFO 1 (DMA transfer request 1) */
#define USBHS_FIFO_1_IRQn          ((IRQn_Type) 72) /* USBHS FIFO 1 (DMA transfer request 1) */
#define VECTOR_NUMBER_DMAC0_INT ((IRQn_Type) 73) /* DMAC0 INT (DMAC transfer end 0) */
#define DMAC0_INT_IRQn          ((IRQn_Type) 73) /* DMAC0 INT (DMAC transfer end 0) */
#define VECTOR_NUMBER_DMAC1_INT ((IRQn_Type) 74) /* DMAC1 INT (DMAC transfer end 1) */
#define DMAC1_INT_IRQn          ((IRQn_Type) 74) /* DMAC1 INT (DMAC transfer end 1) */
#define VECTOR_NUMBER_SDHIMMC0_ACCS ((IRQn_Type) 75) /* SDHIMMC0 ACCS (Card access) */
#define SDHIMMC0_ACCS_IRQn          ((IRQn_Type) 75) /* SDHIMMC0 ACCS (Card access) */
#define VECTOR_NUMBER_SDHIMMC0_CARD ((IRQn_Type) 76) /* SDHIMMC0 CARD (Card detect) */
#define SDHIMMC0_CARD_IRQn          ((IRQn_Type) 76) /* SDHIMMC0 CARD (Card detect) */
#define VECTOR_NUMBER_DMAC2_INT ((IRQn_Type) 77) /* DMAC2 INT (DMAC transfer end 2) */
#define DMAC2_INT_IRQn          ((IRQn_Type) 77) /* DMAC2 INT (DMAC transfer end 2) */
#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
