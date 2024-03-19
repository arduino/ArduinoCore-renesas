#include "Arduino.h"
#include "pinmux.inc"

const uint16_t P400_b[] = {PIN_PWM | CHANNEL_6 | PWM_CHANNEL_A | GPT_ODD_CFG,
                           PIN_SCL | CHANNEL_0, PIN_INTERRUPT | CHANNEL_0,
                           SCI_CHANNEL | PIN_SCK | CHANNEL_0 | SCI_EVEN_CFG |
                               LAST_ITEM_GUARD};
#define P400 P400_b

const uint16_t P408_b[] = {
    PIN_PWM | CHANNEL_5 | PWM_CHANNEL_B | GPT_ODD_CFG,
    PIN_INTERRUPT | CHANNEL_7,
    SCI_CHANNEL | PIN_CTS_RTS_SS | CHANNEL_1 | SCI_EVEN_CFG,
    SCI_CHANNEL | PIN_RX_MISO_SCL | CHANNEL_9 | SCI_ODD_CFG | LAST_ITEM_GUARD};
#define P408 P408_b

const uint16_t P014_b[] = {PIN_DAC | CHANNEL_0,
                           PIN_ANALOG | CHANNEL_9 | LAST_ITEM_GUARD};
#define P014 P014_b

extern "C" const PinMuxCfg_t g_pin_cfg[] = {
    /* ++++++++++++++++++++ DIGITAL OUTPUTS +++++++++++++++++++++++++++++++++ */
    {BSP_IO_PORT_00_PIN_00, P000}, /* (0) DIO_RESET_1 */
    {BSP_IO_PORT_00_PIN_01, P001}, /* (1) DIO_RESET_2 */
    {BSP_IO_PORT_00_PIN_02, P002}, /* (2) DIO_RTD_SWITCH_1 */
    {BSP_IO_PORT_00_PIN_03, P003}, /* (3) DIO_RTD_SWITCH_2 */
    /* +++++++++++++++++++++++ PWMs +++++++++++++++++++++++++++++++++++++++++ */
    {BSP_IO_PORT_01_PIN_07, P107}, /* (4) PWM_0 */
    {BSP_IO_PORT_01_PIN_05, P105}, /* (5) PWM_1 */
    {BSP_IO_PORT_05_PIN_00, P500}, /* (6) PwM_2 */
    {BSP_IO_PORT_01_PIN_11, P111}, /* (7) PwM_3 */
    {BSP_IO_PORT_01_PIN_10, P110}, /* (8) PWM fault 1 */
    {BSP_IO_PORT_05_PIN_01, P501}, /* (9) PwM_fault 2 */
    /* ++++++++++++++++++++++++++++++++++ LEDS ++++++++++++++++++++++++++++++ */
    {BSP_IO_PORT_00_PIN_11, P011}, /* (10) LED_1 */
    {BSP_IO_PORT_00_PIN_04, P004}, /* (11) LED_2 */
    {BSP_IO_PORT_00_PIN_12, P012}, /* (12) LED_3 */
    {BSP_IO_PORT_00_PIN_13, P013}, /* (13) LED_4 */
    {BSP_IO_PORT_00_PIN_14, P014}, /* (14) LED_5 */
    {BSP_IO_PORT_00_PIN_15, P015}, /* (15) LED_6 */
    {BSP_IO_PORT_01_PIN_06, P106}, /* (16) LED_7 */
    {BSP_IO_PORT_01_PIN_09, P109}, /* (17) LED_8 */

    {BSP_IO_PORT_01_PIN_13, P113}, /* (18) LED_RED */
    {BSP_IO_PORT_04_PIN_10, P410}, /* (19) LED_BLUE */
    {BSP_IO_PORT_04_PIN_11, P411}, /* (20) LED_GREEN */
    /* +++++++++++++++++++++ IRQs FROM ANALOG BOARD +++++++++++++++++++++++++ */
    {BSP_IO_PORT_02_PIN_05, P205}, /* (21) ALERT1_IRQ1 */
    {BSP_IO_PORT_02_PIN_06, P206}, /* (22) ALERT2_IRQ0 */
    {BSP_IO_PORT_05_PIN_05, P502}, /* (23) ADC_READY_1_IRQ12 */
    {BSP_IO_PORT_03_PIN_04, P304}, /* (24) ADC_READY_2_IRQ9 */
    /* +++++++++++++++++++++++++++++++ DETECTS ++++++++++++++++++++++++++++++ */
    {BSP_IO_PORT_04_PIN_08, P408}, /* (25) DETECT_IN */
    {BSP_IO_PORT_04_PIN_09, P409}, /* (26) DETECT_OUT*/
    /* +++++++++++++++++++++++++++++++++ UART +++++++++++++++++++++++++++++++ */
    {BSP_IO_PORT_03_PIN_01, P301}, /* (27) UART_RX */
    {BSP_IO_PORT_03_PIN_02, P302}, /* (28) UART TX */
    /* +++++++++++++++++++++++++++++++++ I2C ++++++++++++++++++++++++++++++++ */
    {BSP_IO_PORT_04_PIN_00, P400}, /* (29) I2C_SCL */
    {BSP_IO_PORT_04_PIN_01, P401}, /* (30) I2C_SDA */
    /* +++++++++++++++++++++++++++++++++ SPI ++++++++++++++++++++++++++++++++ */
    {BSP_IO_PORT_01_PIN_00, P100}, /* (31) SPI_MISO */
    {BSP_IO_PORT_01_PIN_01, P101}, /* (32) SPI_MOSI */
    {BSP_IO_PORT_01_PIN_02, P102}, /* (33) SPI_CK */
    {BSP_IO_PORT_01_PIN_03, P103}, /* (34) SPI_CS_1 */
    {BSP_IO_PORT_01_PIN_04, P104}, /* (35) SPI_CS_2 */
    /* +++++++++++++++++++++++++++++++++ SPI ++++++++++++++++++++++++++++++++ */
    {BSP_IO_PORT_01_PIN_12, P112}, /* (36) LDAC1 */
    {BSP_IO_PORT_03_PIN_03, P303}, /* (37) LDAC2 */
    
};

extern "C" const size_t g_pin_cfg_size = sizeof(g_pin_cfg);

int32_t getPinIndex(bsp_io_port_pin_t p) {
  int max_index = g_pin_cfg_size / sizeof(g_pin_cfg[0]);
  int rv = -1;
  for (int i = 0; i < max_index; i++) {
    if (g_pin_cfg[i].pin == p) {
      rv = i;
      break;
    }
  }
  return rv;
}

extern "C" {
unsigned int PINCOUNT_fn() {
  return (sizeof(g_pin_cfg) / sizeof(g_pin_cfg[0]));
}
}

#include "FspTimer.h"

void initVariant() {
  pinMode(SPI_CS_1,OUTPUT);
  pinMode(SPI_CS_2,OUTPUT);
  digitalWrite(SPI_CS_1,HIGH);
  digitalWrite(SPI_CS_2,HIGH);

  pinMode(DIO_RESET_1,OUTPUT);
  pinMode(DIO_RESET_2,OUTPUT);
  digitalWrite(DIO_RESET_1,HIGH);
  digitalWrite(DIO_RESET_2,HIGH);

  pinMode(DIO_RTD_SWITCH_1,OUTPUT);
  pinMode(DIO_RTD_SWITCH_2,OUTPUT);

  digitalWrite(DIO_RTD_SWITCH_1,LOW);
  digitalWrite(DIO_RTD_SWITCH_2,LOW);

  pinMode(LDAC1,OUTPUT);
  pinMode(LDAC2,OUTPUT);
  digitalWrite(LDAC1,HIGH);
  digitalWrite(LDAC2,HIGH);
  



  pinMode(LED_1, OUTPUT);
  digitalWrite(LED_1, LED_OFF);
  pinMode(LED_2, OUTPUT);
  digitalWrite(LED_2, LED_OFF);
  pinMode(LED_2, OUTPUT);
  digitalWrite(LED_2, LED_OFF);
  pinMode(LED_3, OUTPUT);
  digitalWrite(LED_3, LED_OFF);
  pinMode(LED_4, OUTPUT);
  digitalWrite(LED_4, LED_OFF);
  pinMode(LED_5, OUTPUT);
  digitalWrite(LED_5, LED_OFF);
  pinMode(LED_6, OUTPUT);
  digitalWrite(LED_6, LED_OFF);
  pinMode(LED_7, OUTPUT);
  digitalWrite(LED_7, LED_OFF);
  pinMode(LED_8, OUTPUT);
  digitalWrite(LED_8, LED_OFF);
  pinMode(OPTA_LED_RED, OUTPUT);
  digitalWrite(LED_RED, LED_RGB_OFF);
  pinMode(OPTA_LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LED_RGB_OFF);
  pinMode(OPTA_LED_GREEN, OUTPUT);
  digitalWrite(OPTA_LED_GREEN, LED_RGB_OFF);

  /* TO BE VERIFIED !! */
  FspTimer::set_initial_timer_channel_as_pwm(
      GPT_TIMER, GET_CHANNEL(getPinCfgs(PWM_0, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(
      GPT_TIMER, GET_CHANNEL(getPinCfgs(PWM_1, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(
      GPT_TIMER, GET_CHANNEL(getPinCfgs(PWM_2, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(
      GPT_TIMER, GET_CHANNEL(getPinCfgs(PWM_3, PIN_CFG_REQ_PWM)[0]));
  
}
