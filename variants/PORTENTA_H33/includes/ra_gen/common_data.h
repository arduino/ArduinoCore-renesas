/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "tx_api.h"
#include "r_dmac.h"
#include "r_transfer_api.h"
#include "r_sdhi.h"
#include "r_sdmmc_api.h"
#include "rm_block_media_sdmmc.h"
#include "rm_block_media_api.h"
#include "rm_filex_block_media.h"
#include "fx_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
#include "r_ioport.h"
#include "bsp_pin_cfg.h"
FSP_HEADER
/* Transfer on DMAC Instance. */
extern const transfer_instance_t g_transfer0;

/** Access the DMAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dmac_instance_ctrl_t g_transfer0_ctrl;
extern const transfer_cfg_t g_transfer0_cfg;

#ifndef g_sdmmc0_dmac_callback
void g_sdmmc0_dmac_callback(dmac_callback_args_t *p_args);
#endif
/** SDMMC on SDMMC Instance. */
extern const sdmmc_instance_t g_sdmmc0;

/** Access the SDMMC instance using these structures when calling API functions directly (::p_api is not used). */
extern sdhi_instance_ctrl_t g_sdmmc0_ctrl;
extern sdmmc_cfg_t g_sdmmc0_cfg;

#ifndef rm_block_media_sdmmc_callback
void rm_block_media_sdmmc_callback(sdmmc_callback_args_t *p_args);
#endif
extern const rm_block_media_instance_t g_rm_block_media0;

/** Access the SDMMC instance using these structures when calling API functions directly (::p_api is not used). */
extern rm_block_media_sdmmc_instance_ctrl_t g_rm_block_media0_ctrl;
extern const rm_block_media_cfg_t g_rm_block_media0_cfg;

#ifndef rm_filex_block_media_memory_callback
void rm_filex_block_media_memory_callback(rm_block_media_callback_args_t *p_args);
#endif
extern const rm_filex_block_media_instance_t g_rm_filex_block_media_0_instance;

/** Access the FileX Block Media instance using these structures when calling API functions directly (::p_api is not used). */
extern rm_filex_block_media_instance_ctrl_t g_rm_filex_block_media_0_ctrl;
extern const rm_filex_block_media_cfg_t g_rm_filex_block_media_0_cfg;

#ifndef g_rm_filex_block_media_0_callback
void g_rm_filex_block_media_0_callback(rm_filex_block_media_callback_args_t *p_args);
#endif
#define G_FX_MEDIA0_MEDIA_MEMORY_SIZE (10240)
#define G_FX_MEDIA0_VOLUME_NAME ("Volume 1")
#define G_FX_MEDIA0_NUMBER_OF_FATS (1)
#define G_FX_MEDIA0_DIRECTORY_ENTRIES (128)
#define G_FX_MEDIA0_HIDDEN_SECTORS (0)
#define G_FX_MEDIA0_TOTAL_SECTORS (15558144)
#define G_FX_MEDIA0_BYTES_PER_SECTOR (512)
#define G_FX_MEDIA0_SECTORS_PER_CLUSTER (1)
#define G_FX_MEDIA0_VOLUME_SERIAL_NUMBER (000000001621)
#define G_FX_MEDIA0_BOUNDARY_UNIT (8192)
/** CGC Instance */
extern const cgc_instance_t g_cgc0;

/** Access the CGC instance using these structures when calling API functions directly (::p_api is not used). */
extern cgc_instance_ctrl_t g_cgc0_ctrl;
extern const cgc_cfg_t g_cgc0_cfg;

#ifndef NULL
void NULL(cgc_callback_args_t *p_args);
#endif
/* IOPORT Instance */
extern const ioport_instance_t g_ioport;

/* IOPORT control structure. */
extern ioport_instance_ctrl_t g_ioport_ctrl;
extern TX_EVENT_FLAGS_GROUP g_cdcacm_event_flags0;
void g_common_init(void);
FSP_FOOTER
#endif /* COMMON_DATA_H_ */
