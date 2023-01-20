#include "bsp_api.h"
#include "hal_data.h"

extern const canfd_afl_entry_t p_canfd0_afl[CANFD_CFG_AFL_CH0_RULE_NUM] = {
  {
    .id =
      {
        .id = 0x10,
        .frame_type = CAN_FRAME_TYPE_DATA,
        .id_mode    = CAN_ID_MODE_STANDARD,
      },
    .mask =
      {
        .mask_id         = 0x7F0,
        .mask_frame_type = 1,
        .mask_id_mode    = 1,
      },
    .destination =
      {
        .minimum_dlc = CANFD_MINIMUM_DLC_0,
        .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_MB_0,
        .fifo_select_flags = CANFD_RX_FIFO_0,
      }
  }
};

extern const canfd_afl_entry_t p_canfd1_afl[CANFD_CFG_AFL_CH1_RULE_NUM] = {
  {
    .id =
      {
        .id = 0x20,
        .frame_type = CAN_FRAME_TYPE_DATA,
        .id_mode    = CAN_ID_MODE_STANDARD,
      },
    .mask =
      {
        .mask_id         = 0x7F0,
        .mask_frame_type = 1,
        .mask_id_mode    = 1,
      },
    .destination =
      {
        .minimum_dlc = CANFD_MINIMUM_DLC_0,
        .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_MB_0,
        .fifo_select_flags = CANFD_RX_FIFO_0,
      }
  }
};
