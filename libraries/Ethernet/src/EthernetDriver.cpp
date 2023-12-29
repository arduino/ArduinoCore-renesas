#include "EthernetDriver.h"
#include <IRQManager.h>
#include <malloc.h>
#include "utils.h"
#include <Arduino_DebugUtils.h>


#define ETHERNET_PIN_CFG ((uint32_t) ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_ETHER_RMII))
#define ETHERNET_CHANNEL                        (0)
#define ADE_BIT_MASK                            (1 << 23)
#define ETHER_FRAME_RECEIVED_MASK               (1UL << 18)
#define ETHER_FRAME_TRANSFER_COMPLETED          (1UL << 21)
#define ETHER_MAGIC_PACKET_DETECTED_MASK        (1UL << 1)
#define ETHER_RD0_RACT                          (0x80000000UL)

// utility/proxy local functions
void _irq_ether_callback(ether_callback_args_t* p_args);

extern void dump_buffer(uint8_t* b, uint32_t len, uint8_t blocks=4, uint8_t cols=16);

EthernetC33Driver::EthernetC33Driver(
    uint8_t rx_descriptors_len,
    uint8_t tx_descriptors_len,
    void* (*buffer_allocator)(unsigned int),
    uint16_t buffer_size,
    uint8_t* mac_address, uint8_t len)
: NetworkDriver(),
rx_descriptors_len(rx_descriptors_len), tx_descriptors_len(tx_descriptors_len),
buffer_allocator(buffer_allocator), buffer_size(buffer_size) {
    if(mac_address != nullptr && (len == 6 || len == 8)) {
        memcpy(this->macaddress, mac_address, len);
        this->macaddress_len = len;
    } else {
        const bsp_unique_id_t* t = R_BSP_UniqueIdGet();
        this->macaddress[0] = 0xA8;
        this->macaddress[1] = 0x61;
        this->macaddress[2] = 0x0A;
        this->macaddress[3] = t->unique_id_words[0] ^ t->unique_id_words[1];
        this->macaddress[4] = t->unique_id_words[2];
        this->macaddress[5] = t->unique_id_words[3];
    }

    this->rx_descriptors = (ether_instance_descriptor_t*)
        memalign(16, sizeof(ether_instance_descriptor_t)*rx_descriptors_len);
    this->tx_descriptors = (ether_instance_descriptor_t*)
        memalign(16, sizeof(ether_instance_descriptor_t)*tx_descriptors_len);
        // memalign(16, sizeof(ether_instance_descriptor_t)*1);

    rx_buffers      = (uint8_t**) malloc(sizeof(void*)*rx_descriptors_len);

    tx_buffers_info = (_tx_buffer_info*) malloc(sizeof(_tx_buffer_info)*tx_descriptors_len);
    memset(tx_buffers_info, 0, sizeof(_tx_buffer_info)*tx_descriptors_len);

    this->init();
}

EthernetC33Driver::~EthernetC33Driver() {
    free(this->rx_descriptors);
    this->rx_descriptors = nullptr;
    free(this->tx_descriptors);
    this->tx_descriptors = nullptr;

    // TODO free memory of buffers, callback with size 0?
    // this->rx_buffers
    // this->tx_buffers
}

void EthernetC33Driver::init() {
    // FIXME understand the configuration performed here
    // FIXME understand how to pass this configuration as a parameter
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

    // phy setup
    this->phy_cfg.channel                     = ETHERNET_CHANNEL;
    this->phy_cfg.phy_lsi_address             = 0;
    this->phy_cfg.phy_reset_wait_time         = 0x00020000;
    this->phy_cfg.mii_bit_access_wait_time    = 8;
    this->phy_cfg.phy_lsi_type                = ETHER_PHY_LSI_TYPE_DEFAULT;
    this->phy_cfg.flow_control                = ETHER_PHY_FLOW_CONTROL_DISABLE;
    this->phy_cfg.mii_type                    = ETHER_PHY_MII_TYPE_RMII;
    this->phy_cfg.p_context                   = nullptr;
    this->phy_cfg.p_extend                    = nullptr;

    this->phy_instance.p_cfg                  = &this->phy_cfg;
    this->phy_instance.p_ctrl                 = &this->phy_ctrl;
    this->phy_instance.p_api                  = &g_ether_phy_on_ether_phy;

    // setup the driver
    this->extended_cfg.p_rx_descriptors       = this->rx_descriptors; // FIXME
    this->extended_cfg.p_tx_descriptors       = this->tx_descriptors; // FIXME


    this->cfg.channel                         = ETHERNET_CHANNEL;
    this->cfg.zerocopy                        = ETHER_ZEROCOPY_ENABLE;
    this->cfg.multicast                       = ETHER_MULTICAST_ENABLE;
    this->cfg.promiscuous                     = ETHER_PROMISCUOUS_DISABLE;
    this->cfg.flow_control                    = ETHER_FLOW_CONTROL_DISABLE;
    this->cfg.padding                         = ETHER_PADDING_DISABLE; // TODO
    this->cfg.padding_offset                  = 0;
    this->cfg.broadcast_filter                = 0;
    this->cfg.p_mac_address                   = this->macaddress;
    this->cfg.num_tx_descriptors              = this->tx_descriptors_len;
    this->cfg.num_rx_descriptors              = this->rx_descriptors_len;
    this->cfg.pp_ether_buffers                = this->rx_buffers;
    this->cfg.ether_buffer_size               = this->buffer_size;
    this->cfg.irq                             = FSP_INVALID_VECTOR;
    this->cfg.interrupt_priority              = (this->irq_priority);
    this->cfg.p_callback                      = _irq_ether_callback;
    this->cfg.p_ether_phy_instance            = &this->phy_instance;
    this->cfg.p_context                       = this;
    this->cfg.p_extend                        = &this->extended_cfg;
}

void EthernetC33Driver::begin() {
    // Fill the rx_buffers
    uint8_t i=0;
    for(; i < rx_descriptors_len; i++) {
        // buffer_allocator has to take into account memory alignment
        rx_buffers[i] = (uint8_t*)buffer_allocator(buffer_size);

        if(rx_buffers[i] == nullptr) {
            break;
        }
    }

    // If at least a buffer is allocated, wait for the link to be up, otherwise report an error
    // TODO report error
    if(i > 0) {
        this->up();
    }
}

void EthernetC33Driver::poll() {
    // Polling the rx descriptor for available data

    uint32_t rx_frame_dim = 0;
    uint8_t* rx_frame_buf = nullptr;
    fsp_err_t err = FSP_SUCCESS;
    struct pbuf *p = nullptr;

    if(this->consume_cbk == nullptr) {
        // Callback is not set, no meaning to release buffers
        // TODO put assertion
        return;
    }

    // netif_stats &stats = *(this->stats);
    // NETIF_STATS_INCREMENT_RX_INTERRUPT_CALLS(stats);

    // arduino::lock();
    while(ETHER_RD0_RACT != (ctrl.p_rx_descriptor->status & ETHER_RD0_RACT)) {
        // NETIF_STATS_RX_TIME_START(stats); // FIXME add stats
        // Getting the available data in the Eth DMA buffer
        err = R_ETHER_Read(&this->ctrl, &rx_frame_buf, &rx_frame_dim);
        // DEBUG_INFO("[polling] read %08X, %u, %u", rx_frame_buf, rx_frame_dim, err);
        if(err != FSP_SUCCESS) {
            // NETIF_STATS_INCREMENT_RX_INTERRUPT_FAILED_CALLS(stats);
            // NETIF_STATS_INCREMENT_ERROR(stats, err);
            // NETIF_STATS_RX_TIME_AVERAGE(stats);


            // Error, discarding the buffer without consuming it
            R_ETHER_BufferRelease(&ctrl);
            break;
        }

        // giving the ownership of the buffer to the module using the driver,
        // this memory should be now handled by the new owner
        if(!this->consumed) {
            // DEBUG_INFO("buf %8X", rx_frame_buf);
            this->consume_cbk(rx_frame_buf, rx_frame_dim);
        }

        // TODO find a way to put a limit into the number of mem_malloc
        arduino::lock();
        uint8_t* new_buffer = (uint8_t*)buffer_allocator(buffer_size);
        arduino::unlock();

        if(new_buffer == nullptr) {
            this->consumed = true; // this indicates that the buffer had been consumed, but the new buffer isn't allocated
            break;
        } else {
            this->consumed = false; // this indicates that the buffer had been consumed and the new buffer is allocated
        }
        err = R_ETHER_RxBufferUpdate(&ctrl, new_buffer);
        // NETIF_STATS_INCREMENT_ERROR(stats, err);
        // NETIF_STATS_RX_TIME_AVERAGE(stats);
        if(err != FSP_SUCCESS) {
            // DEBUG_INFO("%u", err); // FIXME handle this
        }
    }
    // arduino::unlock();

    // Polling the tx descriptor for Tx transmission completed
    // while(R_ETHER_TxStatusGet(this->ctrl, tx_buffers_info[first].buffer) {
        // // FIXME check that first and the completed packet are valid
        // // FIXME move this into the poll function
        // tx_buffers_info[first].len = 0;

        // if(tx_buffers_info[first].free_function) {
        //     tx_buffers_info[first].free_function(tx_buffers_info[first].buffer);
        //     tx_buffers_info[first].free_function = nullptr;
        // } else {
        //     free(tx_buffers_info[first].buffer);
        // }
        // tx_buffers_info[first].buffer = nullptr;
        // first = (first + 1) % tx_descriptors_len;
    // }
}

network_driver_send_err_t EthernetC33Driver::send(
    uint8_t* data, uint16_t len, network_driver_send_flags_t flags, void(*free_function)(void*)) {
    // dump_buffer(tx_buffers_info[last].buffer, len);
    // dump_buffer(data, len);
    // DEBUG_INFO("[send] %08X, %u", data, len);
    network_driver_send_err_t res = NETWORK_DRIVER_SEND_ERR_OK;
    fsp_err_t err;

    arduino::lock();
    // the current buffer we are referencing is not yet consumed
    if(this->tx_buffers_info[this->last].len != 0) {
        res = NETWORK_DRIVER_SEND_ERR_BUFFER;
        goto exit;
    }

    if(flags == NETWORK_DRIVER_SEND_FLAGS_NONE) {
        // it could be nice to use buffer_allocator, but we need a way to deallocate it
        this->tx_buffers_info[this->last].buffer = (uint8_t*)memalign(32, len); // TODO does this need to be memaligned? I think not

        if(this->tx_buffers_info[this->last].buffer == nullptr) {
            res = NETWORK_DRIVER_SEND_ERR_BUFFER;
            goto exit;
        }

        // perform a memcpy to the local tx_buffer
        memcpy(this->tx_buffers_info[this->last].buffer, data, len);
    } else if(flags == NETWORK_DRIVER_SEND_FLAGS_ZERO_COPY) {
        this->tx_buffers_info[this->last].buffer = data; // FIXME verify this mode
    }

    this->tx_buffers_info[this->last].len = len;
    this->tx_buffers_info[this->last].free_function = free_function;

    // dump_buffer(this->tx_buffers_info[this->last].buffer, len);
    // dump_buffer(data, len);

    // put this buffer in the next circular buffer position and then increment the index
    // TODO handle the case where a packet is already being transmitted, should WRITE be called after the queued packet is correctly sent?
    err = R_ETHER_Write(
        &this->ctrl, this->tx_buffers_info[this->last].buffer, this->tx_buffers_info[this->last].len);
    this->last = (this->last + 1) % this->tx_descriptors_len;
    if(err != FSP_SUCCESS) {
        res = NETWORK_DRIVER_SEND_ERR_DRIVER;
    }

exit:
    arduino::unlock();
    return res;
}

fsp_err_t EthernetC33Driver::open() {
    bool rv = IRQManager::getInstance().addPeripheral(IRQ_ETHERNET, &cfg);

    if(rv) {
        // cfg.interrupt_priority = irq_priority;
        R_BSP_IrqCfgEnable(cfg.irq, cfg.interrupt_priority, &this->ctrl); /* ??? */
    } else {
        // DEBUG_ERROR("Error setting up irq for ethernet");

        // return -1000; // FIXME error codes should be defined at ArduinoAPI level
        return FSP_ERR_ABORTED;
    }

    return R_ETHER_Open(&this->ctrl, &this->cfg);
}

void EthernetC33Driver::up() {
    fsp_err_t err = FSP_SUCCESS;

    err = this->open();

    if(err != FSP_SUCCESS) {
        // return err;
    }

    do {
        err = this->linkProcess();
        // TODO check if error assumes values that are not correct
        // TODO put a timeout in here
    } while(err != FSP_SUCCESS);

    // return err; // FIXME find a proper way of returning an error
}

void EthernetC33Driver::down() {
    // return
    // FIXME implement this
}

fsp_err_t EthernetC33Driver::linkProcess() {
    return R_ETHER_LinkProcess(&this->ctrl);
}


void EthernetC33Driver::eth_reset_due_to_ADE_bit() {
    uint32_t *EDMAC_EESR_REG = (uint32_t *)0x40114028;
    uint32_t *EDMAC_CONTROL_REG = (uint32_t *)0x40114000;
    if( (*EDMAC_EESR_REG & ADE_BIT_MASK) == ADE_BIT_MASK) {
        R_ETHER_Close(&this->ctrl);
        *EDMAC_CONTROL_REG |= 0x1;
        R_ETHER_Open(&this->ctrl, &this->cfg);
    }
}

void EthernetC33Driver::irq_ether_callback(ether_callback_args_t* p_args) {
    // ether_callback_args_t* p_args = (ether_callback_args_t*) args;
    p_args->status_ecsr;
    uint32_t reg_eesr = p_args->status_eesr;

    if(p_args->channel == ETHERNET_CHANNEL) {
        if(p_args->event == ETHER_EVENT_WAKEON_LAN) {
            /* WAKE ON */
            if(this->wake_lan_cbk != nullptr) {
                this->wake_lan_cbk();
            }
        } else if(p_args->event == ETHER_EVENT_LINK_ON) {
            // /* LINK ON */
            if(this->link_up_cbk != nullptr) {
                this->link_up_cbk();
            }
        } else if(p_args->event == ETHER_EVENT_LINK_OFF) {
            /* LINK OFF */
            if(this->link_down_cbk != nullptr) {
                this->link_down_cbk();
            }
        } else if(p_args->event == ETHER_EVENT_INTERRUPT) {
            if (ETHER_MAGIC_PACKET_DETECTED_MASK == (p_args->status_ecsr & ETHER_MAGIC_PACKET_DETECTED_MASK)) {
                // /* MAGIC PACKET DETECTED */
                if(this->magic_packet_cbk != nullptr) {
                    this->magic_packet_cbk();
                }
            }
            if (ETHER_FRAME_TRANSFER_COMPLETED  == (reg_eesr & ETHER_FRAME_TRANSFER_COMPLETED)) {


                // FIXME check that first and the completed packet are valid
                // FIXME move this into the poll function
                // FIXME define a function out of this
                if(tx_buffers_info[first].len == 0 || tx_buffers_info[first].buffer == nullptr) {
                    return;
                }
                arduino::lock();

                if(tx_buffers_info[first].free_function) {
                    tx_buffers_info[first].free_function(tx_buffers_info[first].buffer);
                    tx_buffers_info[first].free_function = nullptr;
                } else {
                    free(tx_buffers_info[first].buffer);
                }
                tx_buffers_info[first].len = 0;
                tx_buffers_info[first].buffer = nullptr;
                first = (first + 1) % tx_descriptors_len;
                arduino::unlock();


                if(this->tx_frame_cbk != nullptr) {
                    this->tx_frame_cbk();
                }
            }
            if (ETHER_FRAME_RECEIVED_MASK       == (reg_eesr & ETHER_FRAME_RECEIVED_MASK)) {
                /* FRAME RECEIVED */

                // We are using polling mode, we don't need this
            }
            if (ADE_BIT_MASK                    == (reg_eesr & ADE_BIT_MASK)) {
                /* weird error with ADE bit set as soon as reception is enabled */
                this->eth_reset_due_to_ADE_bit();
            }
        }
    }
}

void _irq_ether_callback(ether_callback_args_t* p_args) {
    // _IRQEtherHandler* context = (_IRQEtherHandler*)p_args->p_context;
        // dynamic_cast<_IRQEtherHandler*>(p_args->p_context);
    EthernetC33Driver* context =
        // dynamic_cast<EthernetC33Driver*>(p_args->p_context);
        (EthernetC33Driver*)p_args->p_context;

    context->irq_ether_callback(p_args);
}

// #ifdef IGMP_HARDWARE_LEVEL
// #if LWIP_IGMP
// #ifndef HASH_BITS
// #define HASH_BITS 6 /* #bits in hash */
// #endif
// FIXME integrate these functions correctly into the library
// uint32_t ethcrc(const uint8_t *data, size_t length)
// {
//   uint32_t crc = 0xffffffff;
//   size_t i;
//   int j;

//   for (i = 0; i < length; i++) {
//     for (j = 0; j < 8; j++) {
//       if (((crc >> 31) ^ (data[i] >> j)) & 0x01) {
//         /* x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1 */
//         crc = (crc << 1) ^ 0x04C11DB7;
//       } else {
//         crc = crc << 1;
//       }
//     }
//   }
//   return ~crc;
// }

// void register_multicast_address(const uint8_t *mac)
// {
//   uint32_t crc;
//   uint8_t hash;

//   /* Calculate crc32 value of mac address */
//   crc = ethcrc(mac, HASH_BITS);

//   /*
//    * Only upper HASH_BITS are used
//    * which point to specific bit in the hash registers
//    */
//   hash = (crc >> 26) & 0x3F;

//   if (hash > 31) {
//     ETH_HashTableHigh |= 1 << (hash - 32);
//     EthHandle.Instance->MACHTHR = ETH_HashTableHigh;
//   } else {
//     ETH_HashTableLow |= 1 << hash;
//     EthHandle.Instance->MACHTLR = ETH_HashTableLow;
//   }
// }

// err_t igmp_mac_filter(struct netif *netif, const ip4_addr_t *ip4_addr, netif_mac_filter_action action)
// {
//   uint8_t mac[6];
//   const uint8_t *p = (const uint8_t *)ip4_addr;

//   mac[0] = 0x01;
//   mac[1] = 0x00;
//   mac[2] = 0x5E;
//   mac[3] = *(p + 1) & 0x7F;
//   mac[4] = *(p + 2);
//   mac[5] = *(p + 3);

//   register_multicast_address(mac);

//   return 0;
// }
// #endif /* LWIP_IGMP */
// #endif
