/***********************************************************************************************************************
 * File Name    : qspi_ep.h
 * Description  : Contains data structures and functions used in qspi_ep.h.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

#ifndef QSPI_EP_H_
#define QSPI_EP_H_


#define EP_INFO                         "\r\nThis Example Project demonstrates the functionality of\r\n"               \
                                        "QSPI module in QPI and Extended SPI modes. The SPI Protocol\r\n" \
                                        "is selected from RA configurator.\r\n"                        \
                                        "Fixed data is written to QSPI Flash device and read back.\r\n"         \
                                        "Read and Write data is compared and the result is displayed\r\non RTTViewer.\r\n"


/* QSPI flash page number to be written */
#define PAGE_FIRST                      (0U)

/* QSPI flash page Size */
#define PAGE_WRITE_SIZE                 (256U)

/* QSPI flash address through page*/
#define QSPI_FLASH_ADDRESS(page_no)     (uint8_t *) (QSPI_DEVICE_START_ADDRESS + (page_no * PAGE_WRITE_SIZE))

/* Status register pay-load */
#define STATUS_REG_PAYLOAD              {0x01,0x40,0x00}

/* data written to status register */
#define SET_SREG_VALUE                  (0x40)

/* sector size of QSPI flash device */
#define SECTOR_SIZE                     (4096U)

/* one byte data transfer */
#define ONE_BYTE                        (0x01)

/* SREG pay-load size */
#define SREG_SIZE                       (0x03)

/* default memory value */
#define DEFAULT_MEM_VAL                 (0xFF)

/* QPI mode exit command */
#define QSPI_MX25L_CMD_EXIT_QPI_MODE    (0xF5)

/* QPI mode entry command */
#define QSPI_MX25L_CMD_ENTER_QPI_MODE   (0x35)

#endif /* QSPI_EP_H_ */
