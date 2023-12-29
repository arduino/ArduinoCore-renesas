#pragma once

#include "r_ether_phy_api.h"
#include "r_ether_phy.h"
#include "r_ether_api.h"
#include "r_ether.h"
#include <functional>
#include <driver.h>

class EthernetC33Driver: public NetworkDriver {
public:
    EthernetC33Driver(
        uint8_t rx_descriptors_len=1,
        uint8_t tx_descriptors_len=1,
        void* (*buffer_allocator)(unsigned int)=malloc, // The allocator should return 16 byte aligned
        uint16_t buffer_size=1536,
        uint8_t* mac_address=nullptr, uint8_t len=0); // TODO provide pinmapping as parameter to the constructor
    ~EthernetC33Driver();


    /*
     * TODO define the meaning of begin: open + link up?
     */
    virtual void begin();

    // Provide a function to poll the driver
    virtual void poll();

    virtual fsp_err_t open(); // FIXME errors should be abstracted
    virtual fsp_err_t linkProcess();
    virtual void up();
    virtual void down();

    virtual network_driver_send_err_t send(uint8_t* data, uint16_t len,
        network_driver_send_flags_t flags=NETWORK_DRIVER_SEND_FLAGS_NONE,
        void(*free_function)(void*)=nullptr);


    // TODO add callbacks getters/setters
    virtual uint8_t* getMacAddress() override { return this->macaddress; }
protected:

    // extend the callbacks and add the Driver specific callbacks
    std::function<void()> wake_lan_cbk;
    std::function<void()> magic_packet_cbk;

private:
    ether_instance_descriptor_t *tx_descriptors;
    ether_instance_descriptor_t *rx_descriptors;

    uint8_t **rx_buffers;

    // array containing the info of the buffers queued to be sent
    struct _tx_buffer_info {
        uint16_t len=0;
        uint8_t* buffer=nullptr;
        void(*free_function)(void*)=nullptr;
    };
    _tx_buffer_info *tx_buffers_info;

    // tx circular buffer cursors
    uint8_t last = 0, first=0;

    // uint8_t tx_buffer[1536];
    volatile bool frame_in_transmission = false;

    uint8_t macaddress[8]; // FIXME differentiate between 6 and 8 len
    uint8_t macaddress_len = 0;

    // FSP structures for control and configuration of the driver
    ether_phy_cfg_t           phy_cfg;
    ether_phy_instance_ctrl_t phy_ctrl;
    ether_phy_instance_t      phy_instance;
    ether_cfg_t               cfg;
    ether_instance_ctrl_t     ctrl;
    ether_extended_cfg_t      extended_cfg;
    const uint32_t            irq_priority = 10;

    uint8_t rx_descriptors_len;
    uint8_t tx_descriptors_len;
    void* (*buffer_allocator)(unsigned int);
    uint16_t buffer_size;

    bool consumed = false;

    // This function initializes the driver and its configuration
    // TODO provide a way for the user to override the settings
    void init();

    // Strange function that needs to be present, for whatever reason, keeping it
    void eth_reset_due_to_ADE_bit();

    virtual void irq_ether_callback(ether_callback_args_t* p_args);
    friend void _irq_ether_callback(ether_callback_args_t* p_args);
};

extern EthernetC33Driver C33EthernetDriver;