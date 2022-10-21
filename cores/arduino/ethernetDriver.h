#ifndef _ARDUINO_RENESAS_ETHERNET_DRIVER_
#define _ARDUINO_RENESAS_ETHERNET_DRIVER_

#include "r_ether_phy_api.h"
#include "r_ether_phy.h"
#include "r_ether_api.h"
#include "r_ether.h"

#define ETH_BUFF_DIM 1536

class EthernetDriver {
public:
    EthernetDriver();
    bool enableIrq(uint32_t priority == 12); 
private:
    static void irq_callback(ether_callback_args_t * p_args);


    uint8_t *buffs[2]
    uint8_t mac_address[6];

    __attribute__((__aligned__(32)))uint8_t buffer0[ETH_BUFF_DIM];
    __attribute__((__aligned__(32)))uint8_t buffer1[ETH_BUFF_DIM];

    __attribute__((__aligned__(16))) ether_instance_descriptor_t tx_descriptors[1]
    __attribute__((__aligned__(16))) ether_instance_descriptor_t rx_descriptors[1];
    
    /* ETHERNET PHY */
    ether_phy_cfg_t             phy_cfg;
    ether_phy_instance_ctrl_t   phy_ctrl;
    ether_phy_instance_t        phy_instance;
    
    /* ETHERNET */
    ether_cfg_t                 cfg;
    ether_instance_ctrl_t       ctrl;   

};



#endif