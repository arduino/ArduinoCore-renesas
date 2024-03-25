#include "I2S.h"

#define BUFF_SIZE   1024
static volatile i2s_event_t g_i2s_event = I2S_EVENT_TX_EMPTY;  //an actual event updates in callback

extern "C" ssi_instance_ctrl_t g_i2s0_ctrl;
extern "C" i2s_cfg_t g_i2s0_cfg;

static volatile int32_t g_dest_buff[BUFF_SIZE] __attribute__((aligned(4)));
static volatile int16_t g_final[BUFF_SIZE / 2] __attribute__((aligned(4)));

bool I2SClass::begin() {

  IRQManager::getInstance().addPeripheral(IRQ_I2S, &g_i2s0_cfg);
  auto err = R_SSI_Open(&g_i2s0_ctrl, &g_i2s0_cfg);

  if (err != FSP_SUCCESS) {
    return false;
  }

  // Need to find the timer connected to GPT_TIMER 2A for internal clock
  auto pwm = new PwmOut(64);
  pwm->begin(100, 50, true);

  pinPeripheral(BSP_IO_PORT_01_PIN_12, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SSI));
  pinPeripheral(BSP_IO_PORT_01_PIN_13, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SSI));
  pinPeripheral(BSP_IO_PORT_01_PIN_14, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SSI));
  pinPeripheral(BSP_IO_PORT_01_PIN_15, (uint32_t) (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_SSI));

  err = R_SSI_Read(&g_i2s0_ctrl, (void*)g_dest_buff, BUFF_SIZE * sizeof(g_dest_buff[0]));

  if (err != FSP_SUCCESS) {
    return false;
  }
  return true;
}

static volatile bool hasData = false;
extern "C" void i2s_callback(i2s_callback_args_t *p_args)
{
  if ( NULL != p_args)
  {
    /* capture callback event for validating the i2s transfer event*/
    g_i2s_event = p_args->event;
    if (g_i2s_event == I2S_EVENT_RX_FULL) {
      hasData = true;
    }
    if (g_i2s_event == I2S_EVENT_IDLE) {
      R_SSI_Read(&g_i2s0_ctrl, (void*)g_dest_buff, BUFF_SIZE * sizeof(g_dest_buff[0]));
    }
  }
}

bool I2SClass::available() {
    return hasData;
}

size_t I2SClass::read(int16_t* buf) {

  while (hasData == false) {
    delayMicroseconds(1);
  }

  for (int i = 0; i < BUFF_SIZE; i += 2) {
    if (g_dest_buff[i] != 0) {
      g_final[i / 2] = (int16_t)((((int32_t)((g_dest_buff[i] << 14))) / (1 << 14)) + 0x1C00);
    } else {
      g_final[i / 2] = 0;
    }
  }

  memcpy(buf, (void*)g_final, BUFF_SIZE);
  hasData = false;
  return BUFF_SIZE;
}

 I2SClass I2S;