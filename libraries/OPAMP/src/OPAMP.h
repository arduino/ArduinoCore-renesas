#ifndef _OPAMP_H_
#define _OPAMP_H_

#include <stdint.h>

/* Available speed modes */
/* Note: No middle speed mode on the Uno R4 boards */
enum OpampSpeedMode {
    OPAMP_SPEED_LOWSPEED = 0,
    OPAMP_SPEED_HIGHSPEED = 1,
};

/* The supported boards have 4 OPAMP channels, however, only channel 0 is accessible. */
/* All other channels are connected to the LED matrix or not exposed. */
#define ARDUINO_UNO_R4_DEFAULT_OPAMP_CHANNEL 0

/**
 * Pin Mapping for OPAMP
 * Arduino UNO R4 (Minima, WiFi):
 * ~Channel 0~
 *  Plus: Analog A1 (Renesas P0.00)
 *  Minus: Analog A2 (Renesas P0.01) 
 *  Output: Analog A3 (Renesas P0.02)
 * ~Channel 1~ (Inaccessible)
 *  +: P0.13, -: Renesas P0.12, Out: Renesas P0.03
 * ~Channel 2~ (Inaccessible)
 *  +: P0.11, -: Renesas P0.10, Out: Renesas P0.04
 * ~Channel 3~ (Inaccessible)
 *  +: P0.05, -: Renesas P0.06, Out: Renesas P0.07
*/
class OpampClass {
public:
    /* startup the OPAMP on channel 0 in high-speed mode */
    bool begin();
    /* startup the OPAMP on channel 0 with specific mode */
    bool begin(OpampSpeedMode const speed);

    /* startup the OPAMP with arbitrary channel mask */
    bool begin(uint8_t const channel_mask, OpampSpeedMode const speed);

    /* stop all OPAMP channels */
    void end();
    /* stop specific OPAMP channel(s) */
    void end(uint8_t const channel_mask);
    /* returns true if the specified OPAMP channel number is running */
    bool isRunning(uint8_t const channel);
private:
    /* initializes OPAMP pins for given channel(s) */
    bool initPins(uint8_t const channel_mask);
    /* activates OPAMP for given speed and channel(s) */
    void initOpamp(OpampSpeedMode const speed, uint8_t const channel_mask);

};

extern OpampClass OPAMP;

#endif /* _OPAMP_H_ */
