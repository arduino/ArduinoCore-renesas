#include "Arduino.h"
#include "pinmux.inc"

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
    { BSP_IO_PORT_02_PIN_11,  P211 }, /*   D23    |   SDHI CMD  */
    { BSP_IO_PORT_02_PIN_08,  P208 }, /*   D24    |   SDHI D0   */
    { BSP_IO_PORT_04_PIN_10,  P410 }, /*   D25    |   SDHI D1   */
    { BSP_IO_PORT_02_PIN_06,  P206 }, /*   D26    |   SDHI D2   */
    { BSP_IO_PORT_02_PIN_05,  P205 }, /*   D27    |   SDHI D3   */
    { BSP_IO_PORT_02_PIN_10,  P210 }, /*   D28    |   SDHI CD   */
    { BSP_IO_PORT_04_PIN_14,  P414 }, /*   D29    |   SDHI WP   */

    // Other GPIO
    { BSP_IO_PORT_03_PIN_04,  P304 }, /*   D30    |   FREE/IRQ  */
    { BSP_IO_PORT_04_PIN_15,  P415 }, /*   D31    | !USRBTN/IRQ */

    /// ###### FROM HERE, INTERNAL STUFF ONLY

    // DA14531 BLE UART
    { BSP_IO_PORT_06_PIN_02,  P602 }, /*   D32    |     TX1     */
    { BSP_IO_PORT_06_PIN_01,  P601 }, /*   D33    |     RX1     */
    { BSP_IO_PORT_03_PIN_01,  P301 }, /*   D34    |     RTS1    */
    { BSP_IO_PORT_03_PIN_03,  P303 }, /*   D35    |     CTS1    */

    // Dedicated GPIO pins
    { BSP_IO_PORT_07_PIN_08,  P708 }, /*   D36    |    !SDEN    */
    { BSP_IO_PORT_01_PIN_15,  P115 }, /*   D37    |  BLE !RESET */
    { BSP_IO_PORT_05_PIN_00,  P500 }, /*   D38    |   VREF_EN   */
    { BSP_IO_PORT_00_PIN_01,  P001 }, /*   D39    |  PWR_STATUS */
    
    // Onboard Interrupts
    { BSP_IO_PORT_00_PIN_00,  P000 }, /*   D40    | !FUEL_GAUGE/IRQ */

    // QSPI
    { BSP_IO_PORT_01_PIN_00,  P100 }, /*   D41    |   QSPI CLK  */
    { BSP_IO_PORT_05_PIN_01,  P501 }, /*   D42    |   QSPI SS   */
    { BSP_IO_PORT_01_PIN_02,  P102 }, /*   D43    |   QSPI IO0  */
    { BSP_IO_PORT_01_PIN_01,  P101 }, /*   D44    |   QSPI IO1  */
    { BSP_IO_PORT_01_PIN_04,  P104 }, /*   D45    |   QSPI IO2  */
    { BSP_IO_PORT_01_PIN_03,  P103 }, /*   D46    |   QSPI IO3  */
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

  // Setup user button
  pinMode(USR_BTN, INPUT_PULLUP);

  // Set internal gpio pins.
  // Enable SD Card Power
  pinMode(SD_NEN, OUTPUT_OPENDRAIN);
  digitalWrite(SD_NEN, LOW);

  // Turn on VREF.
  pinMode(VREF_EN, OUTPUT_OPENDRAIN);
  digitalWrite(VREF_EN, HIGH);

  // Configure Battery Power Mode Status Pin
  pinMode(PWR_STAT, INPUT_PULLUP);

  // Take BLE out of reset.
  pinMode(BLE_NRST, OUTPUT_OPENDRAIN);
  digitalWrite(BLE_NRST, HIGH);

  // We're using an extenal reference voltage, so set that here. NOTE: AREF = 3.3V
  analogReference(AR_EXTERNAL);
}
