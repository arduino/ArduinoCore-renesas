#include "CAN.h"

ArduinoCAN::ArduinoCAN(can_ctrl_t *g_can_ctrl,
                       const can_cfg_t *g_can_cfg):
  g_can_ctrl(g_can_ctrl)
, g_can_cfg(g_can_cfg)
{
  tx_complete = false;
  rx_complete = false;
  err_status = false;
  time_out = 500;
  rx_info.error_code = 0;
  rx_info.error_count_receive = 0;
  rx_info.error_count_transmit = 0;
  rx_info.rx_fifo_status = 0;
  rx_info.rx_mb_status = 0;
  rx_info.status = 0;
}

bool ArduinoCAN::begin() {
  pinMode(CAN_STDBY, OUTPUT);
  digitalWrite(CAN_STDBY, LOW);
  if (R_CANFD_Open(g_can_ctrl, g_can_cfg) != FSP_SUCCESS) {
    return false;
  }
  return true;
}

void ArduinoCAN::end() {
  R_CANFD_Close(g_can_ctrl);
}

uint8_t ArduinoCAN::write(CanMessage msg) {
  can_frame_t can_msg = {msg.id,
                         CAN_ID_MODE_STANDARD,
                         CAN_FRAME_TYPE_DATA,
                         msg.data_length,
                         0};

  memcpy((uint8_t*)&can_msg.data[0], (uint8_t*)&msg.data[0], msg.data_length);

  if(R_CANFD_Write(g_can_ctrl, 0, &can_msg) != FSP_SUCCESS) {
    return 0;
  }
  /* Wait here for an event from callback */
  while ((true != tx_complete) && (time_out--));
  if (0 == time_out)
  {
    return 0;
  }

  /* Reset flag bit */
  tx_complete = false;
  time_out = 500;
  return 1;
}

uint8_t ArduinoCAN::read(CanMessage &msg) {
  uint32_t rx_status = 0;
  while(!rx_status && (time_out--)) {
    // Get the status information for CAN transmission
    if (R_CANFD_InfoGet(g_can_ctrl, &rx_info) != FSP_SUCCESS) {
    time_out = 500;
      return 0;
    }
    rx_status = rx_info.rx_mb_status;
  }
  time_out = 500;

  if (rx_status) {
    /* Read the input frame received */
    can_frame_t can_msg;
    if (R_CANFD_Read(g_can_ctrl, 0, &can_msg) != FSP_SUCCESS) {
      return 0;
    }
    msg.id = can_msg.id;
    msg.data_length = can_msg.data_length_code;
    memcpy((uint8_t*)&msg.data[0], (uint8_t*)&can_msg.data[0], can_msg.data_length_code);
    return 1;
  }
  return 0;
}


#if CAN_HOWMANY > 0
ArduinoCAN CAN(&g_canfd0_ctrl, &g_canfd0_cfg);
void __attribute__((weak)) canfd0_callback(can_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        case CAN_EVENT_TX_COMPLETE:
        {
            CAN.tx_complete = true;        //set flag bit
            break;
        }
        case CAN_EVENT_RX_COMPLETE: // Currently driver don't support this. This is unreachable code for now.
        {
            CAN.rx_complete = true;
            break;
        }
        case CAN_EVENT_ERR_WARNING:             //error warning event
        case CAN_EVENT_ERR_PASSIVE:             //error passive event
        case CAN_EVENT_ERR_BUS_OFF:             //error Bus Off event
        case CAN_EVENT_BUS_RECOVERY:            //Bus recovery error event
        case CAN_EVENT_MAILBOX_MESSAGE_LOST:    //overwrite/overrun error event
        case CAN_EVENT_ERR_BUS_LOCK:            // Bus lock detected (32 consecutive dominant bits).
        case CAN_EVENT_ERR_CHANNEL:             // Channel error has occurred.
        case CAN_EVENT_TX_ABORTED:              // Transmit abort event.
        case CAN_EVENT_ERR_GLOBAL:              // Global error has occurred.
        case CAN_EVENT_TX_FIFO_EMPTY:           // Transmit FIFO is empty.
        {
            CAN.err_status = true;          //set flag bit
            break;
        }
    }
}
#endif
#if CAN_HOWMANY > 1
ArduinoCAN CAN1(&g_canfd1_ctrl, &g_canfd1_cfg);
void __attribute__((weak)) canfd1_callback(can_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        case CAN_EVENT_TX_COMPLETE:
        {
            CAN1.tx_complete = true;        //set flag bit
            break;
        }
        case CAN_EVENT_RX_COMPLETE: // Currently driver don't support this. This is unreachable code for now.
        {
            CAN1.rx_complete = true;
            break;
        }
        case CAN_EVENT_ERR_WARNING:             //error warning event
        case CAN_EVENT_ERR_PASSIVE:             //error passive event
        case CAN_EVENT_ERR_BUS_OFF:             //error Bus Off event
        case CAN_EVENT_BUS_RECOVERY:            //Bus recovery error event
        case CAN_EVENT_MAILBOX_MESSAGE_LOST:    //overwrite/overrun error event
        case CAN_EVENT_ERR_BUS_LOCK:            // Bus lock detected (32 consecutive dominant bits).
        case CAN_EVENT_ERR_CHANNEL:             // Channel error has occurred.
        case CAN_EVENT_TX_ABORTED:              // Transmit abort event.
        case CAN_EVENT_ERR_GLOBAL:              // Global error has occurred.
        case CAN_EVENT_TX_FIFO_EMPTY:           // Transmit FIFO is empty.
        {
            CAN1.err_status = true;          //set flag bit
            break;
        }
    }
}
#endif