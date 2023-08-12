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
#define ARDUINO_USED_OPAMP_CHANNEL 0

/**
 * Pin Mapping for OPAMP
 * Uno R4 (Minima, WiFi):
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
    /* startup the OPAMP on channel 0 */
    bool begin(OpampSpeedMode speed = OPAMP_SPEED_HIGHSPEED);
    /* stop the OPAMP on channel 0 */
    void end();
    /* i-th Bit: 0 if OPAMP channel is stopped, 1 if OPAMP channel is operating */
    uint8_t getStatus();
private:
    /* initializes OPAMP pins */
    bool initPins();
    /* activates OPAMP for given speed and channel(s) */
    void initOpamp(OpampSpeedMode speed, uint8_t channel_mask);
};

extern OpampClass OPAMP;

#endif /* _OPAMP_H_ */
