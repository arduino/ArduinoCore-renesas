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

#ifndef R_I2C_MASTER_API_H
#define R_I2C_MASTER_API_H

/*******************************************************************************************************************//**
 * @ingroup RENESAS_INTERFACES
 * @defgroup I2C_MASTER_API I2C Master Interface
 * @brief Interface for I2C master communication.
 *
 * @section I2C_MASTER_API_SUMMARY Summary
 * The I2C master interface provides a common API for I2C HAL drivers. The I2C master interface supports:
 *        - Interrupt driven transmit/receive processing
 *        - Callback function support which can return an event code
 *
 * Implemented by:
 * - @ref IIC_MASTER
 * - @ref SCI_I2C
 * - @ref IIC_B_MASTER
 * - @ref SCI_B_I2C
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Register definitions, common services and error codes. */
#include "bsp_api.h"
#include "r_transfer_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Communication speed options */
typedef enum e_i2c_master_rate
{
    I2C_MASTER_RATE_STANDARD = 100000, ///< 100 kHz
    I2C_MASTER_RATE_FAST     = 400000, ///< 400 kHz
    I2C_MASTER_RATE_FASTPLUS = 1000000 ///< 1 MHz
} i2c_master_rate_t;

/** Addressing mode options */
typedef enum e_i2c_master_addr_mode
{
    I2C_MASTER_ADDR_MODE_7BIT  = 1,    ///< Use 7-bit addressing mode
    I2C_MASTER_ADDR_MODE_10BIT = 2,    ///< Use 10-bit addressing mode
} i2c_master_addr_mode_t;

/** Callback events */
typedef enum e_i2c_master_event
{
    I2C_MASTER_EVENT_ABORTED     = 1,  ///< A transfer was aborted
    I2C_MASTER_EVENT_RX_COMPLETE = 2,  ///< A receive operation was completed successfully
    I2C_MASTER_EVENT_TX_COMPLETE = 3   ///< A transmit operation was completed successfully
} i2c_master_event_t;

/** I2C callback parameter definition */
typedef struct st_i2c_master_callback_args
{
    void const       * p_context;      ///< Pointer to user-provided context
    i2c_master_event_t event;          ///< Event code
} i2c_master_callback_args_t;

/** I2C status indicators */
typedef struct st_i2c_master_status
{
    bool open;                         ///< True if driver is open
} i2c_master_status_t;

/** I2C configuration block */
typedef struct st_i2c_master_cfg
{
    /** Generic configuration */
    uint8_t                channel;                           ///< Identifier recognizable by implementation
    i2c_master_rate_t      rate;                              ///< Device's maximum clock rate from enum i2c_rate_t
    uint32_t               slave;                             ///< The address of the slave device
    i2c_master_addr_mode_t addr_mode;                         ///< Indicates how slave fields should be interpreted
    uint8_t                ipl;                               ///< Interrupt priority level. Same for RXI, TXI, TEI and ERI.
    IRQn_Type              rxi_irq;                           ///< Receive IRQ number
    IRQn_Type              txi_irq;                           ///< Transmit IRQ number
    IRQn_Type              tei_irq;                           ///< Transmit end IRQ number
    IRQn_Type              eri_irq;                           ///< Error IRQ number

    /** DTC support */
    transfer_instance_t const * p_transfer_tx;                ///< DTC instance for I2C transmit.Set to NULL if unused.
    transfer_instance_t const * p_transfer_rx;                ///< DTC instance for I2C receive. Set to NULL if unused.

    /** Parameters to control software behavior */
    void (* p_callback)(i2c_master_callback_args_t * p_args); ///< Pointer to callback function
    void const * p_context;                                   ///< Pointer to the user-provided context

    /** Implementation-specific configuration */
    void const * p_extend;                                    ///< Any configuration data needed by the hardware
} i2c_master_cfg_t;

/** I2C control block.  Allocate an instance specific control block to pass into the I2C API calls.
 * @par Implemented as
 * - iic_master_instance_ctrl_t
 */
typedef void i2c_master_ctrl_t;

/** Interface definition for I2C access as master */
typedef struct st_i2c_master_api
{
    /** Opens the I2C Master driver and initializes the hardware.
     * @par Implemented as
     * - @ref R_IIC_MASTER_Open()
     * - @ref R_SCI_I2C_Open()
     * - @ref R_IIC_B_MASTER_Open()
     * - @ref R_SCI_B_I2C_Open()
     *
     * @param[in] p_ctrl    Pointer to control block. Must be declared by user. Elements are set here.
     * @param[in] p_cfg     Pointer to configuration structure.
     */
    fsp_err_t (* open)(i2c_master_ctrl_t * const p_ctrl, i2c_master_cfg_t const * const p_cfg);

    /** Performs a read operation on an I2C Master device.
     * @par Implemented as
     * - @ref R_SCI_I2C_Read()
     * - @ref R_IIC_MASTER_Read()
     * - @ref R_IIC_B_MASTER_Read()
     * - @ref R_SCI_B_I2C_Read()
     *
     * @param[in] p_ctrl    Pointer to control block set in i2c_api_master_t::open call.
     * @param[in] p_dest    Pointer to the location to store read data.
     * @param[in] bytes     Number of bytes to read.
     * @param[in] restart   Specify if the restart condition should be issued after reading.
     */
    fsp_err_t (* read)(i2c_master_ctrl_t * const p_ctrl, uint8_t * const p_dest, uint32_t const bytes,
                       bool const restart);

    /** Performs a write operation on an I2C Master device.
     * @par Implemented as
     * - @ref R_IIC_MASTER_Write()
     * - @ref R_SCI_I2C_Write()
     * - @ref R_IIC_B_MASTER_Write()
     * - @ref R_SCI_B_I2C_Write()
     *
     * @param[in] p_ctrl    Pointer to control block set in i2c_api_master_t::open call.
     * @param[in] p_src     Pointer to the location to get write data from.
     * @param[in] bytes     Number of bytes to write.
     * @param[in] restart   Specify if the restart condition should be issued after writing.
     */
    fsp_err_t (* write)(i2c_master_ctrl_t * const p_ctrl, uint8_t * const p_src, uint32_t const bytes,
                        bool const restart);

    /** Performs a reset of the peripheral.
     * @par Implemented as
     * - @ref R_IIC_MASTER_Abort()
     * - @ref R_SCI_I2C_Abort()
     * - @ref R_IIC_B_MASTER_Abort()
     * - @ref R_SCI_B_I2C_Abort()
     *
     * @param[in] p_ctrl    Pointer to control block set in i2c_api_master_t::open call.
     */
    fsp_err_t (* abort)(i2c_master_ctrl_t * const p_ctrl);

    /** Sets address of the slave device without reconfiguring the bus.
     * @par Implemented as
     * - @ref R_IIC_MASTER_SlaveAddressSet()
     * - @ref R_SCI_I2C_SlaveAddressSet()
     * - @ref R_IIC_B_MASTER_SlaveAddressSet()
     * - @ref R_SCI_B_I2C_SlaveAddressSet()
     *
     * @param[in] p_ctrl            Pointer to control block set in i2c_api_master_t::open call.
     * @param[in] slave_address     Address of the slave device.
     * @param[in] address_mode      Addressing mode.
     */
    fsp_err_t (* slaveAddressSet)(i2c_master_ctrl_t * const p_ctrl, uint32_t const slave,
                                  i2c_master_addr_mode_t const addr_mode);

    /**
     * Specify callback function and optional context pointer and working memory pointer.
     * @par Implemented as
     * - @ref R_IIC_MASTER_CallbackSet()
     * - @ref R_SCI_I2C_CallbackSet()
     * - @ref R_IIC_B_MASTER_CallbackSet()
     * - @ref R_SCI_B_I2C_CallbackSet()
     *
     * @param[in]   p_ctrl                   Pointer to the IIC Master control block.
     * @param[in]   p_callback               Callback function
     * @param[in]   p_context                Pointer to send to callback function
     * @param[in]   p_working_memory         Pointer to volatile memory where callback structure can be allocated.
     *                                       Callback arguments allocated here are only valid during the callback.
     */
    fsp_err_t (* callbackSet)(i2c_master_ctrl_t * const p_api_ctrl, void (* p_callback)(i2c_master_callback_args_t *),
                              void const * const p_context, i2c_master_callback_args_t * const p_callback_memory);

    /** Gets the status of the configured I2C device.
     * @par Implemented as
     * - @ref R_IIC_MASTER_StatusGet()
     * - @ref R_SCI_I2C_StatusGet()
     * - @ref R_IIC_B_MASTER_StatusGet()
     * - @ref R_SCI_B_I2C_StatusGet()
     *
     * @param[in]   p_ctrl             Pointer to the IIC Master control block.
     * @param[out]  p_status           Pointer to store current status.
     */
    fsp_err_t (* statusGet)(i2c_master_ctrl_t * const p_api_ctrl, i2c_master_status_t * p_status);

    /** Closes the driver and releases the I2C Master device.
     * @par Implemented as
     * - @ref R_IIC_MASTER_Close()
     * - @ref R_SCI_I2C_Close()
     * - @ref R_IIC_B_MASTER_Close()
     * - @ref R_SCI_B_I2C_Close()
     *
     * @param[in] p_ctrl    Pointer to control block set in i2c_api_master_t::open call.
     */
    fsp_err_t (* close)(i2c_master_ctrl_t * const p_ctrl);
} i2c_master_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_i2c_master_instance
{
    i2c_master_ctrl_t      * p_ctrl;   ///< Pointer to the control structure for this instance
    i2c_master_cfg_t const * p_cfg;    ///< Pointer to the configuration structure for this instance
    i2c_master_api_t const * p_api;    ///< Pointer to the API structure for this instance
} i2c_master_instance_t;

/******************************************************************************************************************//**
 * @} (end addtogroup I2C_MASTER_API)
 *********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
