#include "ethernetDriver.h"
#include "IRQManager.h"

#define ETHERNET_PIN_CFG ((uint32_t) ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_ETHER_RMII))

/* -------------------------------------------------------------------------- */
EthernetDriver::EthernetDriver() {
/* -------------------------------------------------------------------------- */    
    /* ethernet PHY _________________________________________________________ */
    phy_cfg.channel                     = 0;
    phy_cfg.phy_lsi_address             = 0;
    phy_cfg.phy_reset_wait_time         = 0x00020000;
    phy_cfg.mii_bit_access_wait_time    = 8;
    phy_cfg.flow_control                = ETHER_PHY_FLOW_CONTROL_DISABLE;
    phy_cfg.mii_type                    = ETHER_PHY_MII_TYPE_RMII;
    phy_cfg.p_context                   = nullptr;
    phy_cfg.p_extend                    = nullptr;
    /* ethernet PHY instance (used by ethernet, below) ______________________ */
    phy_instance.p_cfg                  = &phy_cfg;
    phy_instance.p_ctrl                 = &phy_ctrl;
    phy_instance.p_api                  = &g_ether_phy_on_ether_phy;
    /* ethernet _____________________________________________________________ */
    buffs[0]                            = buffer0;
    buffs[1]                            = buffer1;
    cfg.channel                         = 0; 
    cfg.zerocopy                        = ETHER_ZEROCOPY_DISABLE;
    cfg.multicast                       = ETHER_MULTICAST_ENABLE;
    cfg.promiscuous                     = ETHER_PROMISCUOUS_DISABLE;
    cfg.flow_control                    = ETHER_FLOW_CONTROL_DISABLE;
    cfg.padding                         = ETHER_PADDING_DISABLE; 
    cfg.padding_offset                  = 0; 
    cfg.broadcast_filter                = 0;
    cfg.p_mac_address                   = mac_address;
    cfg.p_rx_descriptors                = rx_descriptors;
    cfg.p_tx_descriptors                = tx_descriptors;
    cfg.num_tx_descriptors              = 1;
    cfg.num_rx_descriptors              = 1;
    cfg.pp_ether_buffers                = buffs;
    cfg.ether_buffer_size               = ETH_BUFF_DIM;
    cfg.irq                             = FSP_INVALID_VECTOR;
    cfg.interrupt_priority              = (12);
    cfg.p_callback                      = irq_callback;
    cfg.p_ether_phy_instance            = &phy_instance; 
    cfg.p_context                       = nullptr; 
    cfg.p_extend                        = nullptr;
    /* PIN configuration ____________________________________________________ */
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_11, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_14, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_00, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_05, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_06, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_00, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_01, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_02, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_03, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_04, ETHERNET_PIN_CFG);
    R_IOPORT_PinCfg(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_04, ETHERNET_PIN_CFG);
}

/* -------------------------------------------------------------------------- */
bool EthernetDriver::enableIrq(uint32_t priority /*== 12*/) {
/* -------------------------------------------------------------------------- */
    bool rv = IRQManager::getInstance().addPeripheral(IRQ_ETHERNET,this);
    if(rv) {
        cfg.interrupt_priority = priority;
        R_BSP_IrqCfgEnable(cfg.irq, cfg.interrupt_priority, &ctrl); /* ??? */
    }
    return rv;
}

/* -------------------------------------------------------------------------- */
void EthernetDriver::irq_callback(ether_callback_args_t * p_args) {
/* -------------------------------------------------------------------------- */
    /*
    uint32_t      channel;             ///< Device channel number
    ether_event_t event;               ///< Event code
    uint32_t      status_ecsr;         ///< ETHERC status register for interrupt handler
    uint32_t      status_eesr;         ///< ETHERC/EDMAC status register for interrupt handler

    void const * p_context;    

     ETHER_EVENT_WAKEON_LAN,            ///< Magic packet detection event
    ETHER_EVENT_LINK_ON,               ///< Link up detection event
    ETHER_EVENT_LINK_OFF,              ///< Link down detection event
    ETHER_EVENT_INTERRUPT */

    p_args->status_ecsr;
    p_args->status_eesr;

    if(p_args->event == ETHER_EVENT_WAKEON_LAN) {

    }
    else if(p_args->event == ETHER_EVENT_LINK_ON) {

    }
    else if(p_args->event == ETHER_EVENT_LINK_OFF) {
        
    }
    else if(p_args->event == ETHER_EVENT_INTERRUPT) {
        
    }

}