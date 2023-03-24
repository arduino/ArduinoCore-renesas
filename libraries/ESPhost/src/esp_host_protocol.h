#ifndef ESP_HOST_PROTOCOL_H
#define ESP_HOST_PROTOCOL_H

//#define DEBUG_ESP_HOST_PROTOCOL



#include <stdbool.h>
#include <stdint.h>

#include "CMsg.h"




/** Exported Structures **/
/* interface header */
typedef struct {
   union {
      void *priv_buffer_handle;
   };
   uint8_t if_type;
   uint8_t if_num;
   uint8_t *payload;
   uint8_t flag;
   uint16_t payload_len;
   uint16_t seq_num;

   void (*free_buf_handle)(void *buf_handle);
} interface_buffer_handle_t;




#define PRIO_Q_SERIAL                             0
#define PRIO_Q_BT                                 1
#define PRIO_Q_OTHERS                             2
#define MAX_PRIORITY_QUEUES                       3

/* ESP Payload Header Flags */
#define MORE_FRAGMENT                             (1 << 0)

/* Serial interface */
#define SERIAL_IF_FILE                            "/dev/esps0"





typedef enum {
   ESP_OPEN_DATA_PATH,
   ESP_CLOSE_DATA_PATH,
   ESP_RESET,
   ESP_MAX_HOST_INTERRUPT,
} ESP_HOST_INTERRUPT;


typedef enum {
   ESP_WLAN_SDIO_SUPPORT = (1 << 0),
   ESP_BT_UART_SUPPORT = (1 << 1),
   ESP_BT_SDIO_SUPPORT = (1 << 2),
   ESP_BLE_ONLY_SUPPORT = (1 << 3),
   ESP_BR_EDR_ONLY_SUPPORT = (1 << 4),
   ESP_WLAN_SPI_SUPPORT = (1 << 5),
   ESP_BT_SPI_SUPPORT = (1 << 6),
   ESP_CHECKSUM_ENABLED = (1 << 7),
} ESP_CAPABILITIES;

typedef enum {
   ESP_TEST_RAW_TP = (1 << 0),
   ESP_TEST_RAW_TP__ESP_TO_HOST = (1 << 1)
} ESP_RAW_TP_MEASUREMENT;

typedef enum {
   ESP_PACKET_TYPE_EVENT,
} ESP_PRIV_PACKET_TYPE;

typedef enum {
   ESP_PRIV_EVENT_INIT,
} ESP_PRIV_EVENT_TYPE;

typedef enum {
   ESP_PRIV_CAPABILITY,
   ESP_PRIV_SPI_CLK_MHZ,
   ESP_PRIV_FIRMWARE_CHIP_ID,
   ESP_PRIV_TEST_RAW_TP
} ESP_PRIV_TAG_TYPE;

struct esp_priv_event {
   uint8_t     event_type;
   uint8_t     event_len;
   uint8_t     event_data[0];
}__attribute__((packed));







#endif