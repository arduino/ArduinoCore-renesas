#include "bsp_api.h"
#include "hal_data.h"

extern const canfd_afl_entry_t p_canfd0_afl[CANFD_CFG_AFL_CH0_RULE_NUM] = {
  {
    .id.id = 0x00000010,
    .id.frame_type = CAN_FRAME_TYPE_DATA,
    .id.id_mode    = CAN_ID_MODE_STANDARD,
  },
  {
    .mask.mask_id         = 0x1FFFF000,
    .mask.mask_frame_type = 0,
    .mask.mask_id_mode    = 1,
  },
  {
    .destination.minimum_dlc = (canfd_minimum_dlc_t)0,
    .destination.rx_buffer   = CANFD_RX_MB_0,
    .destination.fifo_select_flags = CANFD_RX_FIFO_0,
  },
};

extern const canfd_afl_entry_t p_canfd1_afl[CANFD_CFG_AFL_CH0_RULE_NUM] = {
  {
    .id.id = 0x00000020,
    .id.frame_type = CAN_FRAME_TYPE_DATA,
    .id.id_mode    = CAN_ID_MODE_STANDARD,
  },
  {
    .mask.mask_id         = 0x1FFFF000,
    .mask.mask_frame_type = 0,
    .mask.mask_id_mode    = 1,
  },
  {
    .destination.minimum_dlc = (canfd_minimum_dlc_t)0,
    .destination.rx_buffer   = CANFD_RX_MB_0,
    .destination.fifo_select_flags = CANFD_RX_FIFO_0,
  },
};