#include "Arduino.h"
#include "pinmux.inc"

// pins not yet handled by the script
const uint16_t P210[] = {
  PIN_PWM_AGT|CHANNEL_5|PWM_CHANNEL_B|LAST_ITEM_GUARD,
};
const uint16_t P211[] = { LAST_ITEM_GUARD };
const uint16_t P214[] = { LAST_ITEM_GUARD };
const uint16_t P313[] = { LAST_ITEM_GUARD };
const uint16_t P314[] = { LAST_ITEM_GUARD };
const uint16_t P209[] = { LAST_ITEM_GUARD };
const uint16_t P208[] = { LAST_ITEM_GUARD };
const uint16_t P311_b[] = {
  SCI_CHANNEL|PIN_SCK|CHANNEL_3|SCI_ODD_CFG,
  PIN_PWM_AGT|CHANNEL_1|PWM_CHANNEL_B|LAST_ITEM_GUARD
};
#define P311 P311_b

const uint16_t P204_b[] = {
PIN_PWM|CHANNEL_4|PWM_CHANNEL_B|GPT_ODD_CFG,
SCI_CHANNEL|PIN_SCK|CHANNEL_4|SCI_EVEN_CFG,
PIN_SCK|CHANNEL_0|LAST_ITEM_GUARD
};
#define P204 P204_b

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

  // MKR
  { BSP_IO_PORT_01_PIN_05,  P105 }, /*   D0     |             */
  { BSP_IO_PORT_01_PIN_06,  P106 }, /*   D1     |             */
  { BSP_IO_PORT_01_PIN_11,  P111 }, /*   D2     |             */
  { BSP_IO_PORT_03_PIN_03,  P303 }, /*   D3     |             */
  { BSP_IO_PORT_04_PIN_01,  P401 }, /*   D4     |             */
  { BSP_IO_PORT_02_PIN_10,  P210 }, /*   D5     |             */
  { BSP_IO_PORT_06_PIN_01,  P601 }, /*   D6     |             */
  { BSP_IO_PORT_04_PIN_02,  P402 }, /*   D7     |  CS0        */
  { BSP_IO_PORT_09_PIN_00,  P900 }, /*   D8     |  MOSI0      */
  { BSP_IO_PORT_02_PIN_04,  P204 }, /*   D9     |  SCK0       */
  { BSP_IO_PORT_03_PIN_15,  P315 }, /*   D10    |  MISO0      */
  { BSP_IO_PORT_04_PIN_07,  P407 }, /*   D11    |  SDA0       */
  { BSP_IO_PORT_04_PIN_08,  P408 }, /*   D12    |  SCL0       */
  { BSP_IO_PORT_01_PIN_10,  P110 }, /*   D13    |  RX0        */
  { BSP_IO_PORT_06_PIN_02,  P602 }, /*   D14    |  TX0        */

  // Analog
  { BSP_IO_PORT_00_PIN_06,  P006 }, /*   D15    |     A0      */
  { BSP_IO_PORT_00_PIN_05,  P005 }, /*   D16    |     A1      */
  { BSP_IO_PORT_00_PIN_04,  P004 }, /*   D17    |     A2      */
  { BSP_IO_PORT_00_PIN_02,  P002 }, /*   D18    |     A3      */
  { BSP_IO_PORT_00_PIN_01,  P001 }, /*   D19    |     A4      */
  { BSP_IO_PORT_00_PIN_15,  P015 }, /*   D20    |   A5/DAC1   */
  { BSP_IO_PORT_00_PIN_14,  P014 }, /*   D21    |   A6/DAC0   */
  { BSP_IO_PORT_00_PIN_00,  P000 }, /*   D22    |     A7      */

  // Other PWMs
  { BSP_IO_PORT_06_PIN_05,  P605 }, /*   D23    |             */
  { BSP_IO_PORT_06_PIN_08,  P608 }, /*   D24    |             */
  { BSP_IO_PORT_03_PIN_11,  P311 }, /*   D25    |             */
  { BSP_IO_PORT_06_PIN_00,  P600 }, /*   D26    |  ETH_CLOCK  */

  // GPIO (IRQ capable)
  { BSP_IO_PORT_00_PIN_09,  P009 }, /*   D27    |             */
  { BSP_IO_PORT_04_PIN_09,  P409 }, /*   D28    |             */
  { BSP_IO_PORT_05_PIN_05,  P505 }, /*   D29    |             */
  { BSP_IO_PORT_07_PIN_06,  P706 }, /*   D30    |             */
  { BSP_IO_PORT_07_PIN_07,  P707 }, /*   D31    |             */
  { BSP_IO_PORT_07_PIN_08,  P708 }, /*   D32    |             */
  { BSP_IO_PORT_08_PIN_02,  P802 }, /*   D33    |             */

  // RGB LED
  { BSP_IO_PORT_01_PIN_07,  P107 }, /*   D34    |     LEDR    */
  { BSP_IO_PORT_04_PIN_00,  P400 }, /*   D35    |     LEDG    */
  { BSP_IO_PORT_08_PIN_00,  P800 }, /*   D36    |     LEDB    */

  // I2C pins
  { BSP_IO_PORT_05_PIN_11,  P511 }, /*   D37    |     SDA1     */
  { BSP_IO_PORT_05_PIN_12,  P512 }, /*   D38    |     SCL1     */
  { BSP_IO_PORT_03_PIN_02,  P302 }, /*   D39    |     SDA2    */
  { BSP_IO_PORT_03_PIN_01,  P301 }, /*   D40    |     SCL2    */

  // CAN pins
  { BSP_IO_PORT_02_PIN_02,  P202 }, /*   D41    |   CAN RX    */
  { BSP_IO_PORT_02_PIN_03,  P203 }, /*   D42    |   CAN TX    */
  { BSP_IO_PORT_06_PIN_10,  P610 }, /*   D43    |   CAN1 RX   */
  { BSP_IO_PORT_06_PIN_09,  P609 }, /*   D44    |   CAN1 TX   */

  // SPI pins
  { BSP_IO_PORT_01_PIN_00,  P100 }, /*   D45    |   MISO1  */
  { BSP_IO_PORT_01_PIN_01,  P101 }, /*   D46    |   MOSI1  */
  { BSP_IO_PORT_01_PIN_02,  P102 }, /*   D47    |   SCLK1  */
  { BSP_IO_PORT_01_PIN_03,  P103 }, /*   D48    |   CS1    */

  // UART pins
  { BSP_IO_PORT_06_PIN_13,  P613 }, /*   D49    |   TX2    */
  { BSP_IO_PORT_06_PIN_14,  P614 }, /*   D50    |   RX2    */
  { BSP_IO_PORT_06_PIN_11,  P611 }, /*   D51    |   RTS2   */
  { BSP_IO_PORT_04_PIN_04,  P404 }, /*   D52    |   CTS2   */
  { BSP_IO_PORT_05_PIN_06,  P506 }, /*   D53    |   TX3    */
  { BSP_IO_PORT_03_PIN_04,  P304 }, /*   D54    |   RX3    */
  { BSP_IO_PORT_05_PIN_03,  P503 }, /*   D55    |   RTS3   */
  { BSP_IO_PORT_05_PIN_02,  P502 }, /*   D56    |   CTS3   */
  { BSP_IO_PORT_08_PIN_05,  P805 }, /*   D57    |   TX4    */
  { BSP_IO_PORT_05_PIN_13,  P513 }, /*   D58    |   RX4    */
  { BSP_IO_PORT_05_PIN_08,  P508 }, /*   D59    |   RTS4   */
  { BSP_IO_PORT_05_PIN_05,  P500 }, /*   D60    |   CTS4   */
  { BSP_IO_PORT_06_PIN_03,  P603 }, /*   D61    |   RTS0   */
  { BSP_IO_PORT_06_PIN_04,  P604 }, /*   D62    |   CTS0   */

  // SSI (Audio)
  { BSP_IO_PORT_01_PIN_12,  P112 }, /*   D63    |   SSI CK    */
  { BSP_IO_PORT_01_PIN_13,  P113 }, /*   D64    |   SSI WS    */
  { BSP_IO_PORT_01_PIN_14,  P114 }, /*   D65    |   SSI SDI   */
  { BSP_IO_PORT_01_PIN_15,  P115 }, /*   D66    |   SSI SDO   */

  // Generic GPIO pins
  { BSP_IO_PORT_09_PIN_08,  P908 }, /*   D67    |          */
  { BSP_IO_PORT_04_PIN_03,  P403 }, /*   D68    |          */
  { BSP_IO_PORT_09_PIN_01,  P901 }, /*   D69    |          */
  { BSP_IO_PORT_06_PIN_12,  P612 }, /*   D70    |          */
  { BSP_IO_PORT_03_PIN_12,  P312 }, /*   D71    |          */
  { BSP_IO_PORT_03_PIN_13,  P313 }, /*   D72    |          */
  { BSP_IO_PORT_03_PIN_14,  P314 }, /*   D73    |          */
  { BSP_IO_PORT_10_PIN_01,  PA01 }, /*   D74    |          */
  { BSP_IO_PORT_10_PIN_08,  PA08 }, /*   D75    |          */
  { BSP_IO_PORT_10_PIN_09,  PA09 }, /*   D76    |          */
  { BSP_IO_PORT_10_PIN_10,  PA10 }, /*   D77    |          */
  { BSP_IO_PORT_05_PIN_07,  P507 }, /*   D78    |          */
  { BSP_IO_PORT_11_PIN_00,  PB00 }, /*   D79    |          */
  { BSP_IO_PORT_06_PIN_15,  P615 }, /*   D80    |          */
  { BSP_IO_PORT_00_PIN_03,  P003 }, /*   D81    |          */
  { BSP_IO_PORT_00_PIN_07,  P007 }, /*   D82    |          */
  { BSP_IO_PORT_00_PIN_08,  P008 }, /*   D83    |          */

  // SDCARD
  { BSP_IO_PORT_04_PIN_13,  P413 }, /*   D84    |   SDHI CLK  */
  { BSP_IO_PORT_04_PIN_12,  P412 }, /*   D85    |   SDHI CMD  */
  { BSP_IO_PORT_04_PIN_11,  P411 }, /*   D86    |   SDHI D0   */
  { BSP_IO_PORT_04_PIN_10,  P410 }, /*   D87    |   SDHI D1   */
  { BSP_IO_PORT_02_PIN_06,  P206 }, /*   D88    |   SDHI D2   */
  { BSP_IO_PORT_02_PIN_05,  P205 }, /*   D89    |   SDHI D3   */
  { BSP_IO_PORT_04_PIN_15,  P415 }, /*   D90    |   SDHI CD   */
  { BSP_IO_PORT_04_PIN_14,  P414 }, /*   D91    |   SDHI WP   */

  /// ###### FROM HERE, INTERNAL STUFF ONLY

  // ESP32 UART
  { BSP_IO_PORT_10_PIN_00,  PA00 }, /*   D92    |   TX1    */
  { BSP_IO_PORT_06_PIN_07,  P607 }, /*   D93    |   RX1    */
  { BSP_IO_PORT_06_PIN_06,  P606 }, /*   D94    |   RTS1   */
  { BSP_IO_PORT_08_PIN_01,  P801 }, /*   D95    |   CTS1   */

  // INTERNAL I2C
  { BSP_IO_PORT_03_PIN_10,  P310 }, /*   D96    | INTERNAL SDA  */
  { BSP_IO_PORT_03_PIN_09,  P309 }, /*   D97    | INTERNAL SCL  */

  // Dedicated GPIO pins
  { BSP_IO_PORT_00_PIN_10,  P010 }, /*   D98    |   CRYPTO_EN   */
  { BSP_IO_PORT_02_PIN_07,  P207 }, /*   D99    |   BT_SEL      */
  { BSP_IO_PORT_08_PIN_03,  P803 }, /*   D100   |   ESP_DNLOAD  */
  { BSP_IO_PORT_08_PIN_04,  P804 }, /*   D101   |   ESP_EN      */
  { BSP_IO_PORT_08_PIN_06,  P806 }, /*   D102   |   ESP_ACK     */
  { BSP_IO_PORT_01_PIN_04,  P104 }, /*   D103   |   ESP_CS      */
  { BSP_IO_PORT_05_PIN_04,  P504 }, /*   D104   |   PMIC_STDBY  */
  { BSP_IO_PORT_09_PIN_06,  P906 }, /*   D105   |   PMIC_INT    */

  // RMII
  { BSP_IO_PORT_02_PIN_11,  P211 }, /*   D106   |   MDIO        */
  { BSP_IO_PORT_04_PIN_05,  P405 }, /*   D107   |   TXD_EN      */
  { BSP_IO_PORT_04_PIN_06,  P406 }, /*   D108   |   TXD1        */
  { BSP_IO_PORT_07_PIN_00,  P700 }, /*   D109   |   TXD0        */
  { BSP_IO_PORT_07_PIN_01,  P701 }, /*   D110   |   REFCLK50    */
  { BSP_IO_PORT_07_PIN_02,  P702 }, /*   D111   |   RXD0        */
  { BSP_IO_PORT_07_PIN_03,  P703 }, /*   D112   |   RXD1        */
  { BSP_IO_PORT_07_PIN_04,  P704 }, /*   D113   |   RX_ER       */
  { BSP_IO_PORT_07_PIN_05,  P705 }, /*   D114   |   CRS_DV      */
  { BSP_IO_PORT_02_PIN_14,  P214 }, /*   D115   |   MDC         */

  // QSPI
  { BSP_IO_PORT_03_PIN_05,  P305 }, /*   D116   |   QSPI CLK    */
  { BSP_IO_PORT_03_PIN_06,  P306 }, /*   D117   |   QSPI SS     */
  { BSP_IO_PORT_03_PIN_07,  P307 }, /*   D118   |   QSPI IO0    */
  { BSP_IO_PORT_03_PIN_08,  P308 }, /*   D119   |   QSPI IO1    */
  { BSP_IO_PORT_02_PIN_09,  P209 }, /*   D120   |   QSPI IO2    */
  { BSP_IO_PORT_02_PIN_08,  P208 }, /*   D121   |   QSPI IO3    */
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

#include "FspTimer.h"

#define AGT_TIMER_CHANNEL 3
#define ETHERNET_CLK_PIN  BSP_IO_PORT_06_PIN_00

agt_instance_ctrl_t TIMER_ETHERNET_ctrl;
agt_extended_cfg_t TIMER_ETHERNET_extend;
timer_cfg_t TIMER_ETHERNET_cfg;


fsp_err_t startETHClock() {
  pinPeripheral(ETHERNET_CLK_PIN, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_AGT));
  
  TIMER_ETHERNET_extend.count_source = AGT_CLOCK_PCLKB;
  TIMER_ETHERNET_extend.agto = AGT_PIN_CFG_START_LEVEL_LOW;
  TIMER_ETHERNET_extend.agtoab_settings_b.agtoa = AGT_PIN_CFG_DISABLED;
  TIMER_ETHERNET_extend.agtoab_settings_b.agtob = AGT_PIN_CFG_DISABLED;
  TIMER_ETHERNET_extend.measurement_mode = AGT_MEASURE_DISABLED;
  TIMER_ETHERNET_extend.agtio_filter = AGT_AGTIO_FILTER_NONE;
  TIMER_ETHERNET_extend.enable_pin = AGT_ENABLE_PIN_NOT_USED;
  TIMER_ETHERNET_extend.trigger_edge = AGT_TRIGGER_EDGE_RISING;
  
  TIMER_ETHERNET_cfg.mode = TIMER_MODE_PERIODIC;
  TIMER_ETHERNET_cfg.period_counts = (uint32_t) 0x1;
  TIMER_ETHERNET_cfg.duty_cycle_counts = 0x00;
  TIMER_ETHERNET_cfg.source_div = (timer_source_div_t) 0;
  TIMER_ETHERNET_cfg.channel = AGT_TIMER_CHANNEL; 
  TIMER_ETHERNET_cfg.p_callback = NULL;
  TIMER_ETHERNET_cfg.p_context  = NULL;
  TIMER_ETHERNET_cfg.p_extend = &TIMER_ETHERNET_extend;
  TIMER_ETHERNET_cfg.cycle_end_ipl = (BSP_IRQ_DISABLED);
  TIMER_ETHERNET_cfg.cycle_end_irq = FSP_INVALID_VECTOR;
  
  fsp_err_t err = R_AGT_Open(&TIMER_ETHERNET_ctrl,&TIMER_ETHERNET_cfg);
  if (err != FSP_SUCCESS) {
    return err;
  }
  err = R_AGT_Enable(&TIMER_ETHERNET_ctrl);
  if (err != FSP_SUCCESS) {
    return err;
  }
  err = R_AGT_Start(&TIMER_ETHERNET_ctrl);
  if (err != FSP_SUCCESS) {
    return err;
  }

  FspTimer::set_timer_is_used(AGT_TIMER, AGT_TIMER_CHANNEL);
  return err;
}

void initVariant() {
  startETHClock();
  // bootloader configures LED_BUILTIN as PWM output, deconfigure it to avoid spurious signals
  pinMode(LED_BUILTIN, INPUT);
}
