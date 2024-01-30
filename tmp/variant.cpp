#include "Arduino.h"
#include "pinmux.inc"

// pins not yet handled by the script
const uint16_t P208[] = { LAST_ITEM_GUARD };

const uint16_t P209[] = { LAST_ITEM_GUARD };

const uint16_t P210[] = {
    PIN_PWM_AGT|CHANNEL_5|PWM_CHANNEL_B|LAST_ITEM_GUARD,
};

const uint16_t P211[] = { LAST_ITEM_GUARD };

const uint16_t P214[] = { LAST_ITEM_GUARD };


const uint16_t P015_b[] = {
    PIN_DAC|CHANNEL_1,
    PIN_ANALOG|CHANNEL_13|ADC_0,
    PIN_INTERRUPT|CHANNEL_13|LAST_ITEM_GUARD
};
#define P015 P015_b

const uint16_t P014_b[] = {
    PIN_DAC|CHANNEL_0,
    PIN_ANALOG|CHANNEL_12|ADC_0|LAST_ITEM_GUARD
};
#define P014 P014_b

extern "C" const PinMuxCfg_t g_pin_cfg[] = { 

    // Thing Plus Form Factor
    { BSP_IO_PORT_01_PIN_12,  P112 }, /*   D0     |     CS0     */
    { BSP_IO_PORT_04_PIN_06,  P406 }, /*   D1     |   SSI SDI   */
    { BSP_IO_PORT_04_PIN_05,  P405 }, /*   D2     |   SSI SDO   */
    { BSP_IO_PORT_04_PIN_04,  P404 }, /*   D3     | SSI WS/IRQ  */
    { BSP_IO_PORT_04_PIN_03,  P403 }, /*   D4     | SSI CK/IRQ  */
    { BSP_IO_PORT_04_PIN_02,  P402 }, /*   D5     | AUD_CLK/IRQ */
    { BSP_IO_PORT_02_PIN_07,  P207 }, /*   D6     |             */

    // Analog
    { BSP_IO_PORT_00_PIN_14,  P014 }, /*   D7     |   A0/DAC0   */
    { BSP_IO_PORT_00_PIN_15,  P015 }, /*   D8     | A1/DAC1/IRQ */
    { BSP_IO_PORT_05_PIN_05,  P505 }, /*   D9     |    A2/IRQ   */
    { BSP_IO_PORT_05_PIN_04,  P504 }, /*   D10    |      A3     */
    { BSP_IO_PORT_05_PIN_03,  P503 }, /*   D11    |      A4     */
    { BSP_IO_PORT_05_PIN_02,  P502 }, /*   D12    |    A5/IRQ   */

    // LEDS
    { BSP_IO_PORT_01_PIN_05,  P105 }, /*   D13    |     RGB     */
    { BSP_IO_PORT_01_PIN_06,  P106 }, /*   D14    |    STAT     */

    // I2C pins
    { BSP_IO_PORT_04_PIN_01,  P401 }, /*   D15    |  SDA0/IRQ   */
    { BSP_IO_PORT_04_PIN_00,  P400 }, /*   D16    |  SCL0/IRQ   */

    // SPI pins
    { BSP_IO_PORT_01_PIN_10,  P110 }, /*   D17    |    POCI0    */
    { BSP_IO_PORT_01_PIN_09,  P109 }, /*   D18    |  PICO0/IRQ  */
    { BSP_IO_PORT_01_PIN_11,  P111 }, /*   D19    |  SCK0/IRQ   */

    // UART pins
    { BSP_IO_PORT_04_PIN_09,  P409 }, /*   D20    |   TX0/IRQ   */
    { BSP_IO_PORT_04_PIN_08,  P408 }, /*   D21    |   RX0/IRQ   */

    // SDCARD
    { BSP_IO_PORT_02_PIN_14,  P214 }, /*   D22    |   SDHI CLK  */
    { BSP_IO_PORT_02_PIN_12,  P211 }, /*   D23    |   SDHI CMD  */
    { BSP_IO_PORT_02_PIN_08,  P208 }, /*   D24    |   SDHI D0   */
    { BSP_IO_PORT_04_PIN_10,  P410 }, /*   D25    |   SDHI D1   */
    { BSP_IO_PORT_02_PIN_06,  P206 }, /*   D26    |   SDHI D2   */
    { BSP_IO_PORT_02_PIN_05,  P205 }, /*   D27    |   SDHI D3   */
    { BSP_IO_PORT_02_PIN_10,  P210 }, /*   D28    |   SDHI CD   */
    { BSP_IO_PORT_04_PIN_14,  P414 }, /*   D29    |   SDHI WP   */

    // Other GPIO
    { BSP_IO_PORT_03_PIN_04,  P304 }, /*   D30    |   FREE/IRQ  */

    /// ###### FROM HERE, INTERNAL STUFF ONLY

    // DA14531 BLE UART
    { BSP_IO_PORT_06_PIN_02,  P602 }, /*   D31    |     TX1     */
    { BSP_IO_PORT_06_PIN_01,  P601 }, /*   D32    |     RX1     */
    { BSP_IO_PORT_03_PIN_01,  P301 }, /*   D33    |     RTS1    */
    { BSP_IO_PORT_03_PIN_03,  P303 }, /*   D34    |     CTS1    */

    // Dedicated GPIO pins
    { BSP_IO_PORT_07_PIN_08,  P708 }, /*   D35    |    !SDEN    */
    { BSP_IO_PORT_01_PIN_15,  P115 }, /*   D36    |  BLE !RESET */
    { BSP_IO_PORT_05_PIN_00,  P500 }, /*   D37    |   VREF_EN   */
    { BSP_IO_PORT_00_PIN_01,  P001 }, /*   D38    |  PWR_STATUS */
    
    // Onboard Interrupts
    { BSP_IO_PORT_00_PIN_00,  P000 }, /*   D39    | !FUEL_GAUGE/IRQ */

    // QSPI
    { BSP_IO_PORT_01_PIN_00,  P100 }, /*   D40    |   QSPI CLK  */
    { BSP_IO_PORT_05_PIN_01,  P501 }, /*   D41    |   QSPI SS   */
    { BSP_IO_PORT_01_PIN_02,  P102 }, /*   D42    |   QSPI IO0  */
    { BSP_IO_PORT_01_PIN_01,  P101 }, /*   D43    |   QSPI IO1  */
    { BSP_IO_PORT_01_PIN_04,  P104 }, /*   D44    |   QSPI IO2  */
    { BSP_IO_PORT_01_PIN_03,  P103 }, /*   D45    |   QSPI IO3  */
};

extern "C" {
    unsigned int PINCOUNT_fn() {
        return (sizeof(g_pin_cfg) / sizeof(g_pin_cfg[0]));
    }
}

int32_t getPinIndex(bsp_io_port_pin_t p) {
  int max_index = PINS_COUNT;
  int rv = -1;
  for(int i = 0; i < max_index; i++) {
    if(g_pin_cfg[i].pin == p) {
      rv = i;
      break;
    }
  }
  return rv;
}


void initVariant() {
  // bootloader configures LED_BUILTIN as PWM output, deconfigure it to avoid spurious signals
  pinMode(LED_BUILTIN, INPUT);

  // We're using an extenal reference voltage, so set that here. NOTE: AREF = 3.3V
  analogReference(AR_EXTERNAL);
}
