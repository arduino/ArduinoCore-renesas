#ifdef ARDUINO_PORTENTA_C33

#include "bsp_api.h"
#include "hal_data.h"

canfd_afl_entry_t const CANFD0_AFL[CANFD_CFG_AFL_CH0_RULE_NUM] = {
  {
    .id =
      {
        .id = 0x1FFFFFFF,
        .frame_type = CAN_FRAME_TYPE_DATA,
        .id_mode    = CAN_ID_MODE_EXTENDED,
      },
    .mask =
      {
        .mask_id         = 0,
        .mask_frame_type = 1,
        .mask_id_mode    = 1,
      },
    .destination =
      {
        .minimum_dlc = CANFD_MINIMUM_DLC_0,
        .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_MB_0,
        .fifo_select_flags = CANFD_RX_FIFO_0,
      }
  },
  {
    .id =
      {
        .id = 0x1FFFFFFF,
        .frame_type = CAN_FRAME_TYPE_DATA,
        .id_mode    = CAN_ID_MODE_STANDARD,
      },
    .mask =
      {
        .mask_id         = 0,
        .mask_frame_type = 1,
        .mask_id_mode    = 1,
      },
    .destination =
      {
        .minimum_dlc = CANFD_MINIMUM_DLC_0,
        .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_MB_1,
        .fifo_select_flags = CANFD_RX_FIFO_1,
      }
  }
};

canfd_afl_entry_t const CANFD1_AFL[CANFD_CFG_AFL_CH1_RULE_NUM] = {
  {
    .id =
      {
        .id = 0x1FFFFFFF,
        .frame_type = CAN_FRAME_TYPE_DATA,
        .id_mode    = CAN_ID_MODE_EXTENDED,
      },
    .mask =
      {
        .mask_id         = 0,
        .mask_frame_type = 1,
        .mask_id_mode    = 1,
      },
    .destination =
      {
        .minimum_dlc = CANFD_MINIMUM_DLC_0,
        .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_MB_1,
        .fifo_select_flags = CANFD_RX_FIFO_1,
      }
  },
    {
    .id =
      {
        .id = 0x1FFFFFFF,
        .frame_type = CAN_FRAME_TYPE_DATA,
        .id_mode    = CAN_ID_MODE_STANDARD,
      },
    .mask =
      {
        .mask_id         = 0,
        .mask_frame_type = 1,
        .mask_id_mode    = 1,
      },
    .destination =
      {
        .minimum_dlc = CANFD_MINIMUM_DLC_0,
        .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_MB_1,
        .fifo_select_flags = CANFD_RX_FIFO_1,
      }
  }
};

#endif /* ARDUINO_PORTENTA_C33 */
