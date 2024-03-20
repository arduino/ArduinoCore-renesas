#pragma once

#include <functional>
#include <stdint.h>

enum network_driver_send_flags_t: uint8_t {
    NETWORK_DRIVER_SEND_FLAGS_NONE      = 0,
    // this option instructs the send function that it doesn't need to perform a memcpy to the passed argument
    // and is in charge of deleting the buffer
    NETWORK_DRIVER_SEND_FLAGS_ZERO_COPY = 1
};

enum network_driver_send_err_t: uint8_t {
    NETWORK_DRIVER_SEND_ERR_OK      = 0,
    NETWORK_DRIVER_SEND_ERR_MEM     = 1, // memory issues when trying to send a packet
    NETWORK_DRIVER_SEND_ERR_BUFFER  = 2, // there is no available buffer for sending the packet
    NETWORK_DRIVER_SEND_ERR_DRIVER  = 3  // generic error happening at fsp level
};


class NetworkDriver {
public:
    NetworkDriver() {};
    virtual ~NetworkDriver() {};

    /*
     * This function is used by the Interface handling the driver,
     * if used in polling mode, leave empty definition if the driver works though interrupts.
     * When working with interrupts it is expected that the constructor definces them
     */
    virtual void poll() {}; // TODO is it better to have a function pointer, that when set to null is not called?

    /*
     * This function is used to inistialize the driver at runtime and start using it
     */
    virtual void begin() = 0;

    /*
     * this function is used to send data to the network
     * + flags are used to specify additional options when sending
     * + when NETWORK_DRIVER_SEND_FLAGS_ZERO_COPY is provided, a free function must be passed, [default libc free()]
     */
    virtual network_driver_send_err_t send(uint8_t* data, uint16_t len,
        network_driver_send_flags_t flags=NETWORK_DRIVER_SEND_FLAGS_NONE,
        void(*free_function)(void*)=free) = 0;

    /*
     * Sets the callback funtion that is then used to consume incoming data
     */
    virtual void setConsumeCallback(std::function<void(uint8_t*, uint32_t)> consume_cbk) {this->consume_cbk = consume_cbk;}
    virtual void setLinkUpCallback(std::function<void()> link_up_cbk) {this->link_up_cbk = link_up_cbk;}
    virtual void setLinkDownCallback(std::function<void()> link_down_cbk) {this->link_down_cbk = link_down_cbk;}

    /*
     * FIXME define interfaces for RX zero copy
     */


    /*
     * The following functions should set the low level interface to up or down state
     */
    virtual void up() = 0;
    virtual void down() = 0;

    // TODO maybe we can manage mac address in the interface
    virtual uint8_t* getMacAddress() = 0;
    // TODO define callback functions for generic functionalities a network driver has to cope with, like link_up event
protected:
    std::function<void(uint8_t*, uint32_t)> consume_cbk; // TODO move in callbacks

    std::function<void()> tx_frame_cbk;
    std::function<void()> link_up_cbk;
    std::function<void()> link_down_cbk;
};