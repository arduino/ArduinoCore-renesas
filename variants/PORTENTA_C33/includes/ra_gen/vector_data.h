/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (30)
#endif
/* ISR prototypes */
void canfd_error_isr(void);
void canfd_channel_tx_isr(void);
void canfd_rx_fifo_isr(void);
void iic_slave_rxi_isr(void);
void iic_slave_txi_isr(void);
void iic_slave_tei_isr(void);
void iic_slave_eri_isr(void);
void sci_uart_txi_isr(void);
void sci_uart_tei_isr(void);
void ssi_txi_isr(void);
void ssi_int_isr(void);
void sci_uart_rxi_isr(void);
void sci_uart_eri_isr(void);
void spi_rxi_isr(void);
void spi_txi_isr(void);
void spi_tei_isr(void);
void spi_eri_isr(void);
void usbfs_interrupt_handler(void);
void usbfs_resume_handler(void);
void usbfs_d0fifo_handler(void);
void usbfs_d1fifo_handler(void);
void usbhs_interrupt_handler(void);
void usbhs_d0fifo_handler(void);
void usbhs_d1fifo_handler(void);
void r_icu_isr(void);
void rtc_carry_isr(void);
void ether_eint_isr(void);
void sdhimmc_accs_isr(void);
void sdhimmc_dma_req_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_CAN0_CHERR ((IRQn_Type) 0) /* CAN0 CHERR (Channel error) */
#define CAN0_CHERR_IRQn          ((IRQn_Type) 0) /* CAN0 CHERR (Channel error) */
#define VECTOR_NUMBER_CAN0_TX ((IRQn_Type) 1) /* CAN0 TX (Transmit interrupt) */
#define CAN0_TX_IRQn          ((IRQn_Type) 1) /* CAN0 TX (Transmit interrupt) */
#define VECTOR_NUMBER_CAN_GLERR ((IRQn_Type) 2) /* CAN GLERR (Global error) */
#define CAN_GLERR_IRQn          ((IRQn_Type) 2) /* CAN GLERR (Global error) */
#define VECTOR_NUMBER_CAN_RXF ((IRQn_Type) 3) /* CAN RXF (Global recieve FIFO interrupt) */
#define CAN_RXF_IRQn          ((IRQn_Type) 3) /* CAN RXF (Global recieve FIFO interrupt) */
#define VECTOR_NUMBER_IIC0_RXI ((IRQn_Type) 4) /* IIC0 RXI (Receive data full) */
#define IIC0_RXI_IRQn          ((IRQn_Type) 4) /* IIC0 RXI (Receive data full) */
#define VECTOR_NUMBER_IIC0_TXI ((IRQn_Type) 5) /* IIC0 TXI (Transmit data empty) */
#define IIC0_TXI_IRQn          ((IRQn_Type) 5) /* IIC0 TXI (Transmit data empty) */
#define VECTOR_NUMBER_IIC0_TEI ((IRQn_Type) 6) /* IIC0 TEI (Transmit end) */
#define IIC0_TEI_IRQn          ((IRQn_Type) 6) /* IIC0 TEI (Transmit end) */
#define VECTOR_NUMBER_IIC0_ERI ((IRQn_Type) 7) /* IIC0 ERI (Transfer error) */
#define IIC0_ERI_IRQn          ((IRQn_Type) 7) /* IIC0 ERI (Transfer error) */
#define VECTOR_NUMBER_SCI0_TXI ((IRQn_Type) 8) /* SCI0 TXI (Transmit data empty) */
#define SCI0_TXI_IRQn          ((IRQn_Type) 8) /* SCI0 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI0_TEI ((IRQn_Type) 9) /* SCI0 TEI (Transmit end) */
#define SCI0_TEI_IRQn          ((IRQn_Type) 9) /* SCI0 TEI (Transmit end) */
#define VECTOR_NUMBER_SSI0_TXI ((IRQn_Type) 10) /* SSI0 TXI (Transmit data empty) */
#define SSI0_TXI_IRQn          ((IRQn_Type) 10) /* SSI0 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SSI0_INT ((IRQn_Type) 11) /* SSI0 INT (Error interrupt) */
#define SSI0_INT_IRQn          ((IRQn_Type) 11) /* SSI0 INT (Error interrupt) */
#define VECTOR_NUMBER_SCI0_RXI ((IRQn_Type) 12) /* SCI0 RXI (Receive data full) */
#define SCI0_RXI_IRQn          ((IRQn_Type) 12) /* SCI0 RXI (Receive data full) */
#define VECTOR_NUMBER_SCI0_ERI ((IRQn_Type) 13) /* SCI0 ERI (Receive error) */
#define SCI0_ERI_IRQn          ((IRQn_Type) 13) /* SCI0 ERI (Receive error) */
#define VECTOR_NUMBER_SPI0_RXI ((IRQn_Type) 14) /* SPI0 RXI (Receive buffer full) */
#define SPI0_RXI_IRQn          ((IRQn_Type) 14) /* SPI0 RXI (Receive buffer full) */
#define VECTOR_NUMBER_SPI0_TXI ((IRQn_Type) 15) /* SPI0 TXI (Transmit buffer empty) */
#define SPI0_TXI_IRQn          ((IRQn_Type) 15) /* SPI0 TXI (Transmit buffer empty) */
#define VECTOR_NUMBER_SPI0_TEI ((IRQn_Type) 16) /* SPI0 TEI (Transmission complete event) */
#define SPI0_TEI_IRQn          ((IRQn_Type) 16) /* SPI0 TEI (Transmission complete event) */
#define VECTOR_NUMBER_SPI0_ERI ((IRQn_Type) 17) /* SPI0 ERI (Error) */
#define SPI0_ERI_IRQn          ((IRQn_Type) 17) /* SPI0 ERI (Error) */
#define VECTOR_NUMBER_USBFS_INT ((IRQn_Type) 18) /* USBFS INT (USBFS interrupt) */
#define USBFS_INT_IRQn          ((IRQn_Type) 18) /* USBFS INT (USBFS interrupt) */
#define VECTOR_NUMBER_USBFS_RESUME ((IRQn_Type) 19) /* USBFS RESUME (USBFS resume interrupt) */
#define USBFS_RESUME_IRQn          ((IRQn_Type) 19) /* USBFS RESUME (USBFS resume interrupt) */
#define VECTOR_NUMBER_USBFS_FIFO_0 ((IRQn_Type) 20) /* USBFS FIFO 0 (DMA transfer request 0) */
#define USBFS_FIFO_0_IRQn          ((IRQn_Type) 20) /* USBFS FIFO 0 (DMA transfer request 0) */
#define VECTOR_NUMBER_USBFS_FIFO_1 ((IRQn_Type) 21) /* USBFS FIFO 1 (DMA transfer request 1) */
#define USBFS_FIFO_1_IRQn          ((IRQn_Type) 21) /* USBFS FIFO 1 (DMA transfer request 1) */
#define VECTOR_NUMBER_USBHS_USB_INT_RESUME ((IRQn_Type) 22) /* USBHS USB INT RESUME (USBHS interrupt) */
#define USBHS_USB_INT_RESUME_IRQn          ((IRQn_Type) 22) /* USBHS USB INT RESUME (USBHS interrupt) */
#define VECTOR_NUMBER_USBHS_FIFO_0 ((IRQn_Type) 23) /* USBHS FIFO 0 (DMA transfer request 0) */
#define USBHS_FIFO_0_IRQn          ((IRQn_Type) 23) /* USBHS FIFO 0 (DMA transfer request 0) */
#define VECTOR_NUMBER_USBHS_FIFO_1 ((IRQn_Type) 24) /* USBHS FIFO 1 (DMA transfer request 1) */
#define USBHS_FIFO_1_IRQn          ((IRQn_Type) 24) /* USBHS FIFO 1 (DMA transfer request 1) */
#define VECTOR_NUMBER_ICU_IRQ0 ((IRQn_Type) 25) /* ICU IRQ0 (External pin interrupt 0) */
#define ICU_IRQ0_IRQn          ((IRQn_Type) 25) /* ICU IRQ0 (External pin interrupt 0) */
#define VECTOR_NUMBER_RTC_CARRY ((IRQn_Type) 26) /* RTC CARRY (Carry interrupt) */
#define RTC_CARRY_IRQn          ((IRQn_Type) 26) /* RTC CARRY (Carry interrupt) */
#define VECTOR_NUMBER_EDMAC0_EINT ((IRQn_Type) 27) /* EDMAC0 EINT (EDMAC 0 interrupt) */
#define EDMAC0_EINT_IRQn          ((IRQn_Type) 27) /* EDMAC0 EINT (EDMAC 0 interrupt) */
#define VECTOR_NUMBER_SDHIMMC0_ACCS ((IRQn_Type) 28) /* SDHIMMC0 ACCS (Card access) */
#define SDHIMMC0_ACCS_IRQn          ((IRQn_Type) 28) /* SDHIMMC0 ACCS (Card access) */
#define VECTOR_NUMBER_SDHIMMC0_DMA_REQ ((IRQn_Type) 29) /* SDHIMMC0 DMA REQ (DMA transfer request) */
#define SDHIMMC0_DMA_REQ_IRQn          ((IRQn_Type) 29) /* SDHIMMC0 DMA REQ (DMA transfer request) */
#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
