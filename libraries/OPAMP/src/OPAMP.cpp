#include "OPAMP.h"
#include <Arduino.h>

/* pin mode needed to activate OPAMP functionality */
#define OPAMP_IN_PINCFG (IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_CFG_ANALOG_ENABLE)
#define OPAMP_OUT_PINCFG (IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PERIPHERAL_PIN | IOPORT_CFG_ANALOG_ENABLE)
#define FSP_CHECK(err) do { if( (err) != FSP_SUCCESS) return false; } while(0)

bool OpampClass::initPins() {
    fsp_err_t err;
    ioport_instance_ctrl_t ioport_ctrl {};
#if defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_MINIMA)
    /* channel 0 pins. Only accessible ones. */
    err = R_IOPORT_PinCfg(&ioport_ctrl, BSP_IO_PORT_00_PIN_00, OPAMP_IN_PINCFG);
    FSP_CHECK(err);
    err = R_IOPORT_PinCfg(&ioport_ctrl, BSP_IO_PORT_00_PIN_01, OPAMP_IN_PINCFG);
    FSP_CHECK(err);
    err = R_IOPORT_PinCfg(&ioport_ctrl, BSP_IO_PORT_00_PIN_02, OPAMP_OUT_PINCFG);
    FSP_CHECK(err);
#else
#error "Unsupported board."
#endif
    // if we got here, none of the checks triggered an early return.
    return true;
}

void OpampClass::initOpamp(OpampSpeedMode speed, uint8_t channel_mask) {
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

bool OpampClass::begin(OpampSpeedMode speed) {
    if(!initPins()) {
        return false;
    }
    initOpamp(speed, (1u << ARDUINO_USED_OPAMP_CHANNEL));
    return true;
}

uint8_t OpampClass::getStatus() {
    return R_OPAMP->AMPMON;
}

void OpampClass::end() {
    // clear the bit for the used channel
    R_OPAMP->AMPC &= (uint8_t) ~(1u << ARDUINO_USED_OPAMP_CHANNEL);
}

/* global instance */
OpampClass OPAMP;
