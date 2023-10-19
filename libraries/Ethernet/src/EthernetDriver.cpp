#include "EthernetDriver.h"
#include "IRQManager.h"

/* IMPORTANT NOTE: this driver is configured to use ZERO COPY 
   This means when a frame is received the Read function do not perform
   a copy of the data bu just returns the pointer to them
   */

#define USE_ZERO_COPY


typedef enum {
    ETH_LINK_UP,
    ETH_LINK_DOWN
} EthLinkStatus_t;





#define ETHERNET_DEBUG_ENABLED 0

class EthernetDriver {
public:
    EthernetDriver();
    bool enableIrq(uint32_t priority = 12); 

    uint8_t mac_address[MAC_ADDRESS_DIM];
    /* ETHERNET PHY */
    ether_phy_cfg_t             phy_cfg;
    ether_phy_instance_ctrl_t   phy_ctrl;
    ether_phy_instance_t        phy_instance;
    
    /* ETHERNET */
    ether_cfg_t                 cfg;
    ether_instance_ctrl_t       ctrl;
    ether_extended_cfg_t        extended_cfg; 

    ether_cfg_t *get_cfg() {return &cfg;}
    ether_instance_ctrl_t *get_ctrl() {return &ctrl;}

private:
    static EthLinkStatus_t link_status;
    static void irq_callback(ether_callback_args_t * p_args);
    #ifndef USE_ZERO_COPY
    uint8_t *buffs[2];
    #else
    uint8_t *buffs[1];
    #endif

    __attribute__((__aligned__(32)))uint8_t buffer0[ETH_BUFF_DIM] ;
    #ifndef USE_ZERO_COPY
    __attribute__((__aligned__(32)))uint8_t buffer1[ETH_BUFF_DIM] ;
    #endif

    __attribute__((__aligned__(16))) ether_instance_descriptor_t tx_descriptors[1] ;
    __attribute__((__aligned__(16))) ether_instance_descriptor_t rx_descriptors[1] ;

};

#define ETHERNET_CHANNEL                        (0)

#define ETHER_FRAME_RECEIVED_MASK               (1UL << 18)
#define ETHER_FRAME_TRANSFER_COMPLETED          (1UL << 21)
#define ETHER_MAGIC_PACKET_DETECTED_MASK        (1UL << 1)

static volatile bool frame_being_transmitted = false;
static EthernetDriver eth_driver;

static uint8_t eth_tx_buffer[ETH_BUFF_DIM];

uint8_t *eth_get_tx_buffer(uint16_t *size) {
    *size = ETH_BUFF_DIM;
    return eth_tx_buffer;
}


#define ETHERNET_PIN_CFG ((uint32_t) ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_ETHER_RMII))

/* -------------------------------------------------------------------------- */
EthernetDriver::EthernetDriver() {
/* -------------------------------------------------------------------------- */    

    /* default MAC ADDRESS */
    const bsp_unique_id_t* t = R_BSP_UniqueIdGet();
    mac_address[0] = 0xA8;
    mac_address[1] = 0x61;
    mac_address[2] = 0x0A;
    mac_address[3] = t->unique_id_words[0] ^ t->unique_id_words[1];
    mac_address[4] = t->unique_id_words[2];
    mac_address[5] = t->unique_id_words[3];

    /* ethernet PHY _________________________________________________________ */
    phy_cfg.channel                     = ETHERNET_CHANNEL;
    phy_cfg.phy_lsi_address             = 0;
    phy_cfg.phy_reset_wait_time         = 0x00020000;
    phy_cfg.mii_bit_access_wait_time    = 8;
    phy_cfg.phy_lsi_type                = ETHER_PHY_LSI_TYPE_DEFAULT;
    phy_cfg.flow_control                = ETHER_PHY_FLOW_CONTROL_DISABLE;
    phy_cfg.mii_type                    = ETHER_PHY_MII_TYPE_RMII;
    phy_cfg.p_context                   = nullptr;
    phy_cfg.p_extend                    = nullptr;
    /* ethernet PHY instance (used by ethernet, below) ______________________ */
    phy_instance.p_cfg                  = &phy_cfg;
    phy_instance.p_ctrl                 = &phy_ctrl;
    phy_instance.p_api                  = &g_ether_phy_on_ether_phy;
    /* ethernet _____________________________________________________________ */
    extended_cfg.p_rx_descriptors       = rx_descriptors;
    extended_cfg.p_tx_descriptors       = tx_descriptors;


    buffs[0]                            = buffer0;
    #ifndef USE_ZERO_COPY
    buffs[1]                            = buffer1;
    #endif
    cfg.channel                         = ETHERNET_CHANNEL; 
    #ifndef USE_ZERO_COPY
    cfg.zerocopy                        = ETHER_ZEROCOPY_DISABLE;
    #else
    cfg.zerocopy                        = ETHER_ZEROCOPY_ENABLE;
    #endif
    cfg.multicast                       = ETHER_MULTICAST_ENABLE;
    cfg.promiscuous                     = ETHER_PROMISCUOUS_DISABLE;
    cfg.flow_control                    = ETHER_FLOW_CONTROL_DISABLE;
    cfg.padding                         = ETHER_PADDING_DISABLE; 
    cfg.padding_offset                  = 0; 
    cfg.broadcast_filter                = 0;
    cfg.p_mac_address                   = mac_address;
    //cfg.p_rx_descriptors                = rx_descriptors;
    //cfg.p_tx_descriptors                = tx_descriptors;
    cfg.num_tx_descriptors              = 1;
    cfg.num_rx_descriptors              = 1;
    cfg.pp_ether_buffers                = buffs;
    cfg.ether_buffer_size               = ETH_BUFF_DIM;
    cfg.irq                             = FSP_INVALID_VECTOR;
    cfg.interrupt_priority              = (12);
    cfg.p_callback                      = irq_callback;
    cfg.p_ether_phy_instance            = &phy_instance; 
    cfg.p_context                       = nullptr; 
    cfg.p_extend                        = &extended_cfg;
    /* PIN configuration ____________________________________________________ */
   
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_14, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_11, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_05, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_06, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_00, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_01, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_02, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_03, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_04, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_05, ETHERNET_PIN_CFG);
    //R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_04, ETHERNET_PIN_CFG);
}

/* -------------------------------------------------------------------------- */
bool EthernetDriver::enableIrq(uint32_t priority /*== 12*/) {
/* -------------------------------------------------------------------------- */
    bool rv = IRQManager::getInstance().addPeripheral(IRQ_ETHERNET,&cfg);
    if(rv) {
        cfg.interrupt_priority = priority;
        R_BSP_IrqCfgEnable(cfg.irq, cfg.interrupt_priority, &ctrl); /* ??? */
    }
    return rv;
}

EthLinkStatus_t EthernetDriver::link_status = ETH_LINK_DOWN;
EtherCallback_f frame_received = nullptr;
EtherCallback_f frame_transmitted = nullptr;
EtherCallback_f magic_packet_received = nullptr;
EtherCallback_f link_on = nullptr;
EtherCallback_f link_off = nullptr;
EtherCallback_f lan_wake_up = nullptr;

#define ADE_BIT_MASK (1 << 23)

/* This function performs a sort of reset of the EDMAC and  Ethernet controller
   when the ADE bit in EESR EDMAC register is 1 
   (however it does not solve the problem since the cause of this in unkknown)*/

/* -------------------------------------------------------------------------- */
void eth_reset_due_to_ADE_bit() {
/* -------------------------------------------------------------------------- */    
    uint32_t *EDMAC_EESR_REG = (uint32_t *)0x40114028;
    uint32_t *EDMAC_CONTROL_REG = (uint32_t *)0x40114000;
    if( (*EDMAC_EESR_REG & ADE_BIT_MASK) == ADE_BIT_MASK) {
        R_ETHER_Close(eth_driver.get_ctrl());
        *EDMAC_CONTROL_REG |= 0x1;
        R_ETHER_Open(eth_driver.get_ctrl(), eth_driver.get_cfg());
    }               
}



/* -------------------------------------------------------------------------- */
void EthernetDriver::irq_callback(ether_callback_args_t * p_args) {
/* -------------------------------------------------------------------------- */  
    p_args->status_ecsr;
    uint32_t reg_eesr = p_args->status_eesr;
    if(p_args->channel == ETHERNET_CHANNEL) {
        if(p_args->event == ETHER_EVENT_WAKEON_LAN) {
            
            /* WAKE ON */
            if(lan_wake_up != nullptr) {
                lan_wake_up();
            }
        }
        else if(p_args->event == ETHER_EVENT_LINK_ON) {
            
            /* LINK ON */
            if(link_on != nullptr) {
                link_on();
            }
        }
        else if(p_args->event == ETHER_EVENT_LINK_OFF) {
            
            /* LINK OFF */
            if(link_off != nullptr) {
                link_off();
            }
        }
        else if(p_args->event == ETHER_EVENT_INTERRUPT) {
            if (ETHER_MAGIC_PACKET_DETECTED_MASK == (p_args->status_ecsr & ETHER_MAGIC_PACKET_DETECTED_MASK)) {
                
                /* MAGIC PACKET DETECTED */
                if(magic_packet_received != nullptr) {
                    magic_packet_received();
                }
            }
            if (ETHER_FRAME_TRANSFER_COMPLETED == (reg_eesr & ETHER_FRAME_TRANSFER_COMPLETED)) {
                
                
                frame_being_transmitted = false;
                /* FRAME TRANSMISSION COMPLETED */
                if(frame_transmitted != nullptr) {
                    frame_transmitted();
                }
            }
            if (ETHER_FRAME_RECEIVED_MASK == (reg_eesr & ETHER_FRAME_RECEIVED_MASK)) {
                
                /* FRAME RECEIVED */
                if(frame_received != nullptr) {
                    frame_received();
                }
            } 
            if( (reg_eesr & ADE_BIT_MASK) == ADE_BIT_MASK) {
                /* weird error with ADE bit set as soon as reception is enabled */
                eth_reset_due_to_ADE_bit();
            }
        }
        else {
            
        }
    }
}

/* -------------------------------------------------------------------------- */
void eth_set_mac_address(const uint8_t *mad) {
/* -------------------------------------------------------------------------- */    
    for(int i = 0; i < MAC_ADDRESS_DIM; i++) {
        eth_driver.mac_address[i] = *(mad + i);
    }
}

/* -------------------------------------------------------------------------- */
int eth_get_mac_address(uint8_t *mad) {
/* -------------------------------------------------------------------------- */    
    for(int i = 0; i < MAC_ADDRESS_DIM; i++) {
        *(mad + i) = eth_driver.mac_address[i];
    }
    return MAC_ADDRESS_DIM;
}


/* -------------------------------------------------------------------------- */
bool eth_init() {
/* -------------------------------------------------------------------------- */    
    bool rv = false;
    
    if(!eth_driver.enableIrq(ETHERNET_IRQ_PRIORITY)) {
        return rv;
    }


    /* ----
     * open
     * ---- */
    fsp_err_t err = R_ETHER_Open(eth_driver.get_ctrl(), eth_driver.get_cfg());
    
    /*
    uint32_t *reg =  (uint32_t *)0x40114118;
    *reg = 0x37;    
    reg =  (uint32_t *)0x40114030;
    *reg = 0x47FF099F;
    */
    if(err == FSP_SUCCESS) {
        rv = true;
    }
    #ifdef ETHERNET_DEBUG_ENABLED
    else if(err == FSP_ERR_ALREADY_OPEN) {
        rv = false;
    }
    else if(err == FSP_ERR_ETHER_ERROR_PHY_COMMUNICATION) {
        rv = false;
    }
    else if(err == FSP_ERR_ETHER_PHY_ERROR_LINK) {
        rv = false;
    }
    #endif
    
    
    
    #ifdef IGMP_HARDWARE_LEVEL
    /* CODE TO BE VERIFIED  */
    #if LWIP_IGMP
    netif_set_igmp_mac_filter(netif, igmp_mac_filter);
    #endif
    HAL_ETH_ReadPHYRegister(&EthHandle, PHY_IMR, &regvalue);

    regvalue |= PHY_ISFR_INT4;

    /* Enable Interrupt on change of link status */
    HAL_ETH_WritePHYRegister(&EthHandle, PHY_IMR, regvalue);
    #if LWIP_IGMP
    ETH_HashTableHigh = EthHandle.Instance->MACHTHR;
    ETH_HashTableLow = EthHandle.Instance->MACHTLR;
    #endif
    #endif

    return rv;
}

void eth_execute_link_process() {
    R_ETHER_LinkProcess(eth_driver.get_ctrl()); 
}

void eth_release_rx_buffer() {
    R_ETHER_BufferRelease(eth_driver.get_ctrl());
}



bool eth_output(uint8_t *buf, uint16_t dim) {
    bool retval = true;

    fsp_err_t err = R_ETHER_Write(eth_driver.get_ctrl(), buf, dim);
    if(err == FSP_SUCCESS) {
        frame_being_transmitted = true;
        retval = true;
    }
    else {
        retval = false;
    }

    return retval;
}

// this function return true if the tx buffer is not being used for the transmission of another frame
bool eth_output_can_transimit() {
    return !frame_being_transmitted;
}

uint8_t *eth_input(volatile uint32_t *dim) {
    /* NOTE: ZERO COPY IMPLEMENTATION 
       just the pointer and not the data are copied with the Read Function */
    
    uint8_t *ptr1 = nullptr;

    fsp_err_t err = R_ETHER_Read ( eth_driver.get_ctrl(), &ptr1, (uint32_t *)dim);
    if(err == FSP_SUCCESS) {
        return ptr1;
    }
    else {
        return nullptr;
    }
}


void eth_set_rx_frame_cbk    (EtherCallback_f fn) { frame_received = fn; }
void eth_set_tx_frame_cbk    (EtherCallback_f fn) { frame_transmitted = fn; }
void eth_set_link_on_cbk     (EtherCallback_f fn) { link_on = fn; }
void eth_set_link_off_cbk    (EtherCallback_f fn) { link_off = fn; }
void eth_set_lan_wake_up_cbk (EtherCallback_f fn) { lan_wake_up = fn;}
void eth_set_magic_packet_cbk(EtherCallback_f fn) { magic_packet_received = fn;}



#ifdef IGMP_HARDWARE_LEVEL
#if LWIP_IGMP
#ifndef HASH_BITS
#define HASH_BITS 6 /* #bits in hash */
#endif

uint32_t ethcrc(const uint8_t *data, size_t length)
{
  uint32_t crc = 0xffffffff;
  size_t i;
  int j;

  for (i = 0; i < length; i++) {
    for (j = 0; j < 8; j++) {
      if (((crc >> 31) ^ (data[i] >> j)) & 0x01) {
        /* x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1 */
        crc = (crc << 1) ^ 0x04C11DB7;
      } else {
        crc = crc << 1;
      }
    }
  }
  return ~crc;
}

void register_multicast_address(const uint8_t *mac)
{
  uint32_t crc;
  uint8_t hash;

  /* Calculate crc32 value of mac address */
  crc = ethcrc(mac, HASH_BITS);

  /*
   * Only upper HASH_BITS are used
   * which point to specific bit in the hash registers
   */
  hash = (crc >> 26) & 0x3F;

  if (hash > 31) {
    ETH_HashTableHigh |= 1 << (hash - 32);
    EthHandle.Instance->MACHTHR = ETH_HashTableHigh;
  } else {
    ETH_HashTableLow |= 1 << hash;
    EthHandle.Instance->MACHTLR = ETH_HashTableLow;
  }
}

err_t igmp_mac_filter(struct netif *netif, const ip4_addr_t *ip4_addr, netif_mac_filter_action action)
{
  uint8_t mac[6];
  const uint8_t *p = (const uint8_t *)ip4_addr;

  mac[0] = 0x01;
  mac[1] = 0x00;
  mac[2] = 0x5E;
  mac[3] = *(p + 1) & 0x7F;
  mac[4] = *(p + 2);
  mac[5] = *(p + 3);

  register_multicast_address(mac);

  return 0;
}
#endif /* LWIP_IGMP */
#endif
