/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup RENESAS_INTERFACES
 * @defgroup SPI_FLASH_API SPI Flash Interface
 * @brief Interface for accessing external SPI flash devices.
 *
 * @section SPI_FLASH_API_SUMMARY Summary
 * The SPI flash API provides an interface that configures, writes, and erases sectors in SPI flash devices.
 *
 * Implemented by:
 * - @ref OSPI
 * - @ref QSPI
 * @{
 **********************************************************************************************************************/

#ifndef R_SPI_FLASH_API_H
#define R_SPI_FLASH_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Register definitions, common services and error codes. */
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define SPI_FLASH_ERASE_SIZE_CHIP_ERASE    (UINT32_MAX)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Read mode. */
typedef enum e_spi_flash_read_mode
{
    SPI_FLASH_READ_MODE_STANDARD              = 0, ///< Standard Read Mode (no dummy cycles)
    SPI_FLASH_READ_MODE_FAST_READ             = 1, ///< Fast Read Mode (dummy cycles between address and data)
    SPI_FLASH_READ_MODE_FAST_READ_DUAL_OUTPUT = 2, ///< Fast Read Dual Output Mode (data on 2 lines)
    SPI_FLASH_READ_MODE_FAST_READ_DUAL_IO     = 3, ///< Fast Read Dual I/O Mode (address and data on 2 lines)
    SPI_FLASH_READ_MODE_FAST_READ_QUAD_OUTPUT = 4, ///< Fast Read Quad Output Mode (data on 4 lines)
    SPI_FLASH_READ_MODE_FAST_READ_QUAD_IO     = 5, ///< Fast Read Quad I/O Mode (address and data on 4 lines)
} spi_flash_read_mode_t;

/** SPI protocol. */
typedef enum e_spi_flash_protocol
{
    SPI_FLASH_PROTOCOL_EXTENDED_SPI = 0, ///< Extended SPI mode (commands on 1 line)

    /** QPI mode (commands on 4 lines). Note that the application must ensure the device is in QPI mode. */
    SPI_FLASH_PROTOCOL_QPI = 2,

    /** SOPI mode (command and data on 8 lines). Note that the application must ensure the device is in SOPI mode. */
    SPI_FLASH_PROTOCOL_SOPI = 3,

    /** DOPI mode (command and data on 8 lines, dual data rate). Note that the application must ensure the device is in DOPI mode. */
    SPI_FLASH_PROTOCOL_DOPI = 4,
} spi_flash_protocol_t;

/** Number of bytes in the address. */
typedef enum e_spi_flash_address_bytes
{
    SPI_FLASH_ADDRESS_BYTES_3 = 2,     ///< 3 address bytes

    /** 4 address bytes with standard commands. If this option is selected, the application must issue the EN4B
     * command using @ref spi_flash_api_t::directWrite() if required by the device. */
    SPI_FLASH_ADDRESS_BYTES_4 = 3,

    /** 4 address bytes using standard 4-byte command set. */
    SPI_FLASH_ADDRESS_BYTES_4_4BYTE_READ_CODE = 0x13,
} spi_flash_address_bytes_t;

/** Number of data lines used. */
typedef enum e_spi_flash_data_lines
{
    SPI_FLASH_DATA_LINES_1 = 0,        ///< 1 data line
    SPI_FLASH_DATA_LINES_2 = 1,        ///< 2 data lines
    SPI_FLASH_DATA_LINES_4 = 2,        ///< 4 data lines
} spi_flash_data_lines_t;

/** Number of dummy cycles for fast read operations. */
typedef enum e_spi_flash_dummy_clocks
{
    /** Default is 6 clocks for Fast Read Quad I/O, 4 clocks for Fast Read Dual I/O, and 8 clocks for other
     * fast read instructions including Fast Read Quad Output, Fast Read Dual Output, and Fast Read. */
    SPI_FLASH_DUMMY_CLOCKS_DEFAULT,
    SPI_FLASH_DUMMY_CLOCKS_3,          ///< 3 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_4,          ///< 4 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_5,          ///< 5 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_6,          ///< 6 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_7,          ///< 7 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_8,          ///< 8 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_9,          ///< 9 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_10,         ///< 10 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_11,         ///< 11 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_12,         ///< 12 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_13,         ///< 13 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_14,         ///< 14 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_15,         ///< 15 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_16,         ///< 16 dummy clocks
    SPI_FLASH_DUMMY_CLOCKS_17,         ///< 17 dummy clocks
} spi_flash_dummy_clocks_t;

/** Direct Read and Write direction */
typedef enum e_spi_flash_direct_transfer_dir_option
{
    SPI_FLASH_DIRECT_TRANSFER_DIR_READ  = 0x0,
    SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE = 0x1
} spi_flash_direct_transfer_dir_t;

/** Structure to define an erase command and associated erase size. */
typedef struct st_spi_flash_erase_command
{
    uint16_t command;                  ///< Erase command
    uint32_t size;                     ///< Size of erase for associated command, set to SPI_FLASH_ERASE_SIZE_CHIP_ERASE for chip erase
} spi_flash_erase_command_t;

/** Structure to define a direct transfer. */
typedef struct st_spi_flash_direct_transfer
{
    uint32_t address;                  ///< Starting address
    uint32_t data;                     ///< Data
    uint16_t command;                  ///< Transfer command
    uint8_t  dummy_cycles;             ///< Number of dummy cycles
    uint8_t  command_length;           ///< Command length
    uint8_t  address_length;           ///< Address lengrh
    uint8_t  data_length;              ///< Data length
} spi_flash_direct_transfer_t;

/** User configuration structure used by the open function */
typedef struct st_spi_flash_cfg
{
    spi_flash_protocol_t      spi_protocol;                      ///< Initial SPI protocol.  SPI protocol can be changed in @ref spi_flash_api_t::spiProtocolSet.
    spi_flash_read_mode_t     read_mode;                         ///< Read mode
    spi_flash_address_bytes_t address_bytes;                     ///< Number of bytes used to represent the address
    spi_flash_dummy_clocks_t  dummy_clocks;                      ///< Number of dummy clocks to use for fast read operations

    /** Number of lines used to send address for page program command. This should either be 1 or match the number of lines used in
     * the selected read mode. */
    spi_flash_data_lines_t            page_program_address_lines;
    uint8_t                           write_status_bit;          ///< Which bit determines write status
    uint8_t                           write_enable_bit;          ///< Which bit determines write status
    uint32_t                          page_size_bytes;           ///< Page size in bytes (maximum number of bytes for page program). Used to specify single continuous write size (bytes) in case of OSPI RAM.
    uint8_t                           page_program_command;      ///< Page program command
    uint8_t                           write_enable_command;      ///< Command to enable write or erase, typically 0x06
    uint8_t                           status_command;            ///< Command to read the write status
    uint8_t                           read_command;              ///< Read command - OSPI SPI mode only
    uint8_t                           xip_enter_command;         ///< Command to enter XIP mode
    uint8_t                           xip_exit_command;          ///< Command to exit XIP mode
    uint8_t                           erase_command_list_length; ///< Length of erase command list
    spi_flash_erase_command_t const * p_erase_command_list;      ///< List of all erase commands and associated sizes
    void const                      * p_extend;                  ///< Pointer to implementation specific extended configurations
} spi_flash_cfg_t;

/** SPI flash control block.  Allocate an instance specific control block to pass into the SPI flash API calls.
 * @par Implemented as
 * - qspi_instance_ctrl_t
 * - ospi_instance_ctrl_t
 */
typedef void spi_flash_ctrl_t;

/** Status. */
typedef struct st_spi_flash_status
{
    /** Whether or not a write is in progress.  This is determined by reading the @ref spi_flash_cfg_t::write_status_bit
     * from the @ref spi_flash_cfg_t::status_command. */
    bool write_in_progress;
} spi_flash_status_t;

/** SPI flash implementations follow this API. */
typedef struct st_spi_flash_api
{
    /** Open the SPI flash driver module.
     * @par Implemented as
     * - @ref R_OSPI_Open()
     * - @ref R_QSPI_Open()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_cfg                Pointer to a configuration structure
     **/
    fsp_err_t (* open)(spi_flash_ctrl_t * p_ctrl, spi_flash_cfg_t const * const p_cfg);

    /** Write raw data to the SPI flash.
     * @par Implemented as
     * - @ref R_OSPI_DirectWrite()
     * - @ref R_QSPI_DirectWrite()
     *
     * @param[in] p_ctrl                Pointer to a driver handle
     * @param[in] p_src                 Pointer to raw data to write, must include any required command/address
     * @param[in] bytes                 Number of bytes to write
     * @param[in] read_after_write      If true, the slave select remains asserted and the peripheral does not return
     *                                  to direct communications mode. If false, the slave select is deasserted and
     *                                  memory mapped access is possible after this function returns if the device
     *                                  is not busy.
     **/
    fsp_err_t (* directWrite)(spi_flash_ctrl_t * p_ctrl, uint8_t const * const p_src, uint32_t const bytes,
                              bool const read_after_write);

    /** Read raw data from the SPI flash. Must follow a call to @ref spi_flash_api_t::directWrite.
     * @par Implemented as
     * - @ref R_OSPI_DirectRead()
     * - @ref R_QSPI_DirectRead()
     *
     * @param[in]  p_ctrl               Pointer to a driver handle
     * @param[out] p_dest               Pointer to read raw data into
     * @param[in]  bytes                Number of bytes to read
     **/
    fsp_err_t (* directRead)(spi_flash_ctrl_t * p_ctrl, uint8_t * const p_dest, uint32_t const bytes);

    /** Direct Read/Write raw data to the SPI flash.
     * @par Implemented as
     * - @ref R_OSPI_DirectTransfer()
     * - @ref R_QSPI_DirectTransfer()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_data               Pointer to command, address and data values and lengths
     * @param[in] direction            Direct Read/Write
     **/
    fsp_err_t (* directTransfer)(spi_flash_ctrl_t * p_ctrl, spi_flash_direct_transfer_t * const p_transfer,
                                 spi_flash_direct_transfer_dir_t direction);

    /** Change the SPI protocol in the driver. The application must change the SPI protocol on the device.
     * @par Implemented as
     * - @ref R_OSPI_SpiProtocolSet()
     * - @ref R_QSPI_SpiProtocolSet()
     *
     * @param[in] p_ctrl                Pointer to a driver handle
     * @param[in] spi_protocol          Desired SPI protocol
     **/
    fsp_err_t (* spiProtocolSet)(spi_flash_ctrl_t * p_ctrl, spi_flash_protocol_t spi_protocol);

    /** Program a page of data to the flash.
     * @par Implemented as
     * - @ref R_OSPI_Write()
     * - @ref R_QSPI_Write()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_src                The memory address of the data to write to the flash device
     * @param[in] p_dest               The location in the flash device address space to write the data to
     * @param[in] byte_count           The number of bytes to write
     **/
    fsp_err_t (* write)(spi_flash_ctrl_t * p_ctrl, uint8_t const * const p_src, uint8_t * const p_dest,
                        uint32_t byte_count);

    /** Erase a certain number of bytes of the flash.
     * @par Implemented as
     * - @ref R_OSPI_Erase()
     * - @ref R_QSPI_Erase()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_device_address     The location in the flash device address space to start the erase from
     * @param[in] byte_count           The number of bytes to erase. Set to SPI_FLASH_ERASE_SIZE_CHIP_ERASE to erase entire
     *                                 chip.
     **/
    fsp_err_t (* erase)(spi_flash_ctrl_t * p_ctrl, uint8_t * const p_device_address, uint32_t byte_count);

    /** Get the write or erase status of the flash.
     * @par Implemented as
     * - @ref R_OSPI_StatusGet()
     * - @ref R_QSPI_StatusGet()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[out] p_status            Current status of the SPI flash device stored here.
     **/
    fsp_err_t (* statusGet)(spi_flash_ctrl_t * p_ctrl, spi_flash_status_t * const p_status);

    /** Enter XIP mode.
     * @par Implemented as
     * - @ref R_OSPI_XipEnter()
     * - @ref R_QSPI_XipEnter()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     **/
    fsp_err_t (* xipEnter)(spi_flash_ctrl_t * p_ctrl);

    /** Exit XIP mode.
     * @par Implemented as
     * - @ref R_OSPI_XipExit()
     * - @ref R_QSPI_XipExit()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     **/
    fsp_err_t (* xipExit)(spi_flash_ctrl_t * p_ctrl);

    /** Select the bank to access.  See implementation for details.
     * @par Implemented as
     * - @ref R_OSPI_BankSet()
     * - @ref R_QSPI_BankSet()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] bank                 The bank number
     **/
    fsp_err_t (* bankSet)(spi_flash_ctrl_t * p_ctrl, uint32_t bank);

    /** AutoCalibrate the SPI flash driver module. Expected to be used when auto-calibrating OSPI RAM device.
     * @par Implemented as
     * - @ref R_OSPI_AutoCalibrate()
     * - @ref R_QSPI_AutoCalibrate()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     **/
    fsp_err_t (* autoCalibrate)(spi_flash_ctrl_t * p_ctrl);

    /** Close the SPI flash driver module.
     * @par Implemented as
     * - @ref R_OSPI_Close()
     * - @ref R_QSPI_Close()
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     **/
    fsp_err_t (* close)(spi_flash_ctrl_t * p_ctrl);
} spi_flash_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_spi_flash_instance
{
    spi_flash_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    spi_flash_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    spi_flash_api_t const * p_api;     ///< Pointer to the API structure for this instance
} spi_flash_instance_t;

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/*******************************************************************************************************************//**
 * @} (end addtogroup SPI_FLASH_API)
 **********************************************************************************************************************/
