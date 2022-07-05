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
#include "r_ether_phy.h"
#include "r_ether_phy_api.h"
#include "r_ether.h"
#include "r_ether_api.h"
#include "r_icu.h"
#include "r_external_irq_api.h"
#include "nx_api.h"
#include "rm_netxduo_ether.h"
#include "nxd_bsd.h"
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
/** ether_phy on ether_phy Instance. */
extern const ether_phy_instance_t g_ether_phy0;

/** Access the Ethernet PHY instance using these structures when calling API functions directly (::p_api is not used). */
extern ether_phy_instance_ctrl_t g_ether_phy0_ctrl;
extern const ether_phy_cfg_t g_ether_phy0_cfg;
#if (BSP_FEATURE_TZ_HAS_TRUSTZONE == 1) && (BSP_TZ_SECURE_BUILD != 1) && (BSP_TZ_NONSECURE_BUILD != 1)
#define ETHER_BUFFER_PLACE_IN_SECTION BSP_PLACE_IN_SECTION(".ns_buffer.eth")
#else
#define ETHER_BUFFER_PLACE_IN_SECTION
#endif

/** ether on ether Instance. */
extern const ether_instance_t g_ether0;

/** Access the Ethernet instance using these structures when calling API functions directly (::p_api is not used). */
extern ether_instance_ctrl_t g_ether0_ctrl;
extern const ether_cfg_t g_ether0_cfg;

#ifndef rm_netxduo_ether_callback
void rm_netxduo_ether_callback(ether_callback_args_t *p_args);
#endif
#define G_PACKET_POOL0_PACKET_SIZE (1568)
#define G_PACKET_POOL0_PACKET_NUM  (16)
extern NX_PACKET_POOL g_packet_pool0;
extern uint8_t g_packet_pool0_pool_memory[(G_PACKET_POOL0_PACKET_NUM * (G_PACKET_POOL0_PACKET_SIZE + sizeof(NX_PACKET)))];

/* Quick Setup for g_packet_pool0 instance. */
void g_packet_pool0_quick_setup();
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq9;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq9_ctrl;
extern const external_irq_cfg_t g_external_irq9_cfg;

#ifndef isr_irq9
void isr_irq9(external_irq_callback_args_t *p_args);
#endif
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq8;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq8_ctrl;
extern const external_irq_cfg_t g_external_irq8_cfg;

#ifndef isr_irq8
void isr_irq8(external_irq_callback_args_t *p_args);
#endif
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq7;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq7_ctrl;
extern const external_irq_cfg_t g_external_irq7_cfg;

#ifndef isr_irq7
void isr_irq7(external_irq_callback_args_t *p_args);
#endif
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq6;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq6_ctrl;
extern const external_irq_cfg_t g_external_irq6_cfg;

#ifndef isr_irq6
void isr_irq6(external_irq_callback_args_t *p_args);
#endif
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq5;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq5_ctrl;
extern const external_irq_cfg_t g_external_irq5_cfg;

#ifndef isr_irq5
void isr_irq5(external_irq_callback_args_t *p_args);
#endif
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq4;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq4_ctrl;
extern const external_irq_cfg_t g_external_irq4_cfg;

#ifndef isr_irq4
void isr_irq4(external_irq_callback_args_t *p_args);
#endif
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq3;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq3_ctrl;
extern const external_irq_cfg_t g_external_irq3_cfg;

#ifndef isr_irq3
void isr_irq3(external_irq_callback_args_t *p_args);
#endif
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq2;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq2_ctrl;
extern const external_irq_cfg_t g_external_irq2_cfg;

#ifndef isr_irq2
void isr_irq2(external_irq_callback_args_t *p_args);
#endif
/* NetX Duo Ethernet Driver */
void g_netxduo_ether_0(NX_IP_DRIVER *driver_req_ptr);

/* Instance for storing state information for the Ethernet Driver. */
extern rm_netxduo_ether_instance_t g_netxduo_ether_0_instance;
#define G_IP0_ADDRESS      (IP_ADDRESS(192,168,0,2))
#define G_IP0_SUBNET_MASK     (IP_ADDRESS(255,255,255,0))
#define G_IP0_GATEWAY_ADDRESS (IP_ADDRESS(0,0,0,0))
#define G_IP0_TASK_STACK_SIZE (2048)
#define G_IP0_TASK_PRIORITY   (3)
#define G_IP0_ARP_CACHE_SIZE (520 * 1)

#define RA_NOT_DEFINED 0xFFFFFFFF
#if (RA_NOT_DEFINED != g_netxduo_ether_0)
#define G_IP0_NETWORK_DRIVER g_netxduo_ether_0
#else
#define G_IP0_NETWORK_DRIVER rm_netxduo_wifi
#endif
#undef RA_NOT_DEFINED

void g_ip0_quick_setup();
void g_ip0_error_handler(UINT status);

#ifndef NX_DISABLE_IPV6
extern NXD_ADDRESS g_ip0_ipv6_global_address;
extern NXD_ADDRESS g_ip0_ipv6_link_local_address;
#endif
#define BSD_SUPPORT_TASK_STACK_SIZE (2048)
#define BSD_SUPPORT_TASK_PRIORITY   (3)

void bsd_quick_setup();
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq1;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq1_ctrl;
extern const external_irq_cfg_t g_external_irq1_cfg;

#ifndef isr_irq1
void isr_irq1(external_irq_callback_args_t *p_args);
#endif
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq0;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq0_ctrl;
extern const external_irq_cfg_t g_external_irq0_cfg;

#ifndef isr_irq0
void isr_irq0(external_irq_callback_args_t *p_args);
#endif
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
