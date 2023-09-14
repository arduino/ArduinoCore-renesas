#include "OPAMP.h"
#include <Arduino.h>

/* Make sure this library fails to compile for unsupported boards. */
#if !defined(ARDUINO_UNOWIFIR4) && !defined(ARDUINO_MINIMA)
#error "Unsupported board for OPAMP library."
#endif

/* pin mode needed to activate OPAMP functionality */
#define OPAMP_IN_PINCFG (IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_CFG_ANALOG_ENABLE)
#define OPAMP_OUT_PINCFG (IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_CFG_ANALOG_ENABLE)
#define FSP_CHECK(err) do { if( (err) != FSP_SUCCESS) return false; } while(0)

// Compact structure for OPAMP channel pins
struct opamp_channel_pins_t {
    bsp_io_port_pin_t plus;
    bsp_io_port_pin_t minus;
    bsp_io_port_pin_t output;
};

// See Renesas RA4M1 Group Datasheet
// Note: Channel 0 is the only accessible one one the Arduino Minima boards. 
static const opamp_channel_pins_t opamp_channels[] = {
    {BSP_IO_PORT_00_PIN_00, BSP_IO_PORT_00_PIN_01, BSP_IO_PORT_00_PIN_02}, /* CH0 */
    {BSP_IO_PORT_00_PIN_13, BSP_IO_PORT_00_PIN_12, BSP_IO_PORT_00_PIN_03}, /* CH1 */
    {BSP_IO_PORT_00_PIN_11, BSP_IO_PORT_00_PIN_10, BSP_IO_PORT_00_PIN_04}, /* CH2 */
    {BSP_IO_PORT_00_PIN_05, BSP_IO_PORT_00_PIN_06, BSP_IO_PORT_00_PIN_07}, /* CH3 */
};

bool OpampClass::initPins(uint8_t const channel_mask) {
    fsp_err_t err;
    ioport_instance_ctrl_t ioport_ctrl {};
    // Make sure to return false if nothing was given to initialize
    // or a too high channel bit is in there
    if (channel_mask == 0 || channel_mask > 0b1111) {
        return false;
    }
    // Check the 4 possible channels
    for (uint8_t i = 0; i < 4; i++) {
        // was this channel selected?
        if (!(channel_mask & (1u << i))) {
            continue;
        }
        opamp_channel_pins_t pins = opamp_channels[i];
        err = R_IOPORT_PinCfg(&ioport_ctrl, pins.plus, OPAMP_IN_PINCFG);
        FSP_CHECK(err);
        err = R_IOPORT_PinCfg(&ioport_ctrl, pins.minus, OPAMP_IN_PINCFG);
        FSP_CHECK(err);
        err = R_IOPORT_PinCfg(&ioport_ctrl, pins.output, OPAMP_OUT_PINCFG);
        FSP_CHECK(err);
    }
    // if we got here, none of the checks triggered an early return.
    return true;
}

void OpampClass::initOpamp(OpampSpeedMode speed, uint8_t const channel_mask) {
    uint8_t ampmc_val  = 0U;
    /* setup amplifier speed mode within amplifier mode control */
    /* for all boards, this is at bit position 7 with either 0 (lowspeed) or 1 (highspeed) */
    ampmc_val = (uint8_t) ((uint8_t) speed << R_OPAMP_AMPMC_AMPSP_Pos) & R_OPAMP_AMPMC_AMPSP_Msk;
    /* reset opamp */
    R_OPAMP->AMPC = 0U;
    /* write prepared mode control value */
    R_OPAMP->AMPMC  = ampmc_val;
    /* setup activation trigger select register */
    /* we only support "Software start & stop" for now, value 0. */
    R_OPAMP->AMPTRS = 0;
    R_OPAMP->AMPTRM = 0;
    /* set the bits for the activated channels */
    R_OPAMP->AMPC |= channel_mask;
    /* note: we don't have to activate the charge pump (AMPCPC) because here AVCC0 > 2.7V */
    /* delay for the wanted init time in microseconds */
    if (speed == OPAMP_SPEED_LOWSPEED) {
        delayMicroseconds(BSP_FEATURE_OPAMP_MIN_WAIT_TIME_LP_US);
    } else if (speed == OPAMP_SPEED_HIGHSPEED) {
        delayMicroseconds(BSP_FEATURE_OPAMP_MIN_WAIT_TIME_HS_US);
    }
}

bool OpampClass::begin() {
    return this->begin(OPAMP_SPEED_HIGHSPEED);
}

bool OpampClass::begin(OpampSpeedMode const speed) {

    return this->begin(1u << ARDUINO_UNO_R4_DEFAULT_OPAMP_CHANNEL, speed);
}

bool OpampClass::begin(uint8_t const channel_mask, OpampSpeedMode const speed) {

    if (!initPins(channel_mask)) {
        return false;
    }
    initOpamp(speed, channel_mask);
    return true;
}

bool OpampClass::isRunning(uint8_t const channel) {
    return (R_OPAMP->AMPMON & (1u << channel)) != 0;
}

void OpampClass::end() {
    // deactivate all channels.
    R_OPAMP->AMPC = 0;
}

void OpampClass::end(uint8_t const channel_mask) {
    // deactivate given channels
    R_OPAMP->AMPC &= ~channel_mask;
}

/* global instance */
OpampClass OPAMP;
