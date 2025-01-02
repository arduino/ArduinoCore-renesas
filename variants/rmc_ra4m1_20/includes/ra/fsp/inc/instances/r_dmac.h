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
 * @addtogroup DMAC
 * @{
 **********************************************************************************************************************/

#ifndef R_DMAC_H
#define R_DMAC_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_transfer_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Max configurable number of transfers in TRANSFER_MODE_NORMAL. */
#define DMAC_MAX_NORMAL_TRANSFER_LENGTH    (0xFFFF)

/** Max number of transfers per repeat for TRANSFER_MODE_REPEAT. */
#define DMAC_MAX_REPEAT_TRANSFER_LENGTH    (0x400)

/** Max number of transfers per block in TRANSFER_MODE_BLOCK */
#define DMAC_MAX_BLOCK_TRANSFER_LENGTH     (0x400)

/** Max configurable number of repeats to trasnfer in TRANSFER_MODE_REPEAT */
#define DMAC_MAX_REPEAT_COUNT              (0x10000)

/** Max configurable number of blocks to transfer in TRANSFER_MODE_BLOCK */
#define DMAC_MAX_BLOCK_COUNT               (0x10000)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Control block used by driver. DO NOT INITIALIZE - this structure will be initialized in @ref transfer_api_t::open. */
typedef struct st_dmac_instance_ctrl
{
    uint32_t open;                     // Driver ID

    transfer_cfg_t const * p_cfg;

    /* Pointer to base register. */
    R_DMAC0_Type * p_reg;
} dmac_instance_ctrl_t;

/** Callback function parameter data. */
typedef struct st_dmac_callback_args_t
{
    void const * p_context;            ///< Placeholder for user data.  Set in r_transfer_t::open function in ::transfer_cfg_t.
} dmac_callback_args_t;

/** DMAC transfer configuration extension. This extension is required. */
typedef struct st_dmac_extended_cfg
{
    uint8_t   channel;                 ///< Channel number, does not apply to all HAL drivers.
    IRQn_Type irq;                     ///< DMAC interrupt number
    uint8_t   ipl;                     ///< DMAC interrupt priority
    int32_t   offset;                  ///< Offset value used with transfer_addr_mode_t::TRANSFER_ADDR_MODE_OFFSET.

    /** Source ring buffer size for @ref TRANSFER_MODE_REPEAT_BLOCK. */
    uint16_t src_buffer_size;

    /** Select which event will trigger the transfer.
     *  @note Select ELC_EVENT_NONE for software activation in order to use softwareStart and softwareStart to trigger
     * transfers. */
    elc_event_t activation_source;

    /** Callback for transfer end interrupt. */
    void (* p_callback)(dmac_callback_args_t * cb_data);

    /** Placeholder for user data.  Passed to the user p_callback in ::dmac_callback_args_t. */
    void const * p_context;
} dmac_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const transfer_api_t g_transfer_on_dmac;

/** @endcond */

/***********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_DMAC_Open(transfer_ctrl_t * const p_api_ctrl, transfer_cfg_t const * const p_cfg);
fsp_err_t R_DMAC_Reconfigure(transfer_ctrl_t * const p_api_ctrl, transfer_info_t * p_info);
fsp_err_t R_DMAC_Reset(transfer_ctrl_t * const p_api_ctrl,
                       void const * volatile   p_src,
                       void * volatile         p_dest,
                       uint16_t const          num_transfers);
fsp_err_t R_DMAC_SoftwareStart(transfer_ctrl_t * const p_api_ctrl, transfer_start_mode_t mode);
fsp_err_t R_DMAC_SoftwareStop(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DMAC_Enable(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DMAC_Disable(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DMAC_InfoGet(transfer_ctrl_t * const p_api_ctrl, transfer_properties_t * const p_info);
fsp_err_t R_DMAC_Close(transfer_ctrl_t * const p_api_ctrl);
fsp_err_t R_DMAC_BlockReset(transfer_ctrl_t * const p_ctrl,
                            void const * volatile   p_src,
                            void * volatile         p_dest,
                            uint16_t const          length,
                            transfer_size_t         size,
                            uint16_t const          num_transfers);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/*******************************************************************************************************************//**
 * @} (end defgroup DMAC)
 **********************************************************************************************************************/
