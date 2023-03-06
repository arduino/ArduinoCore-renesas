#ifndef ESP_HOST_PROTOCOL_H
#define ESP_HOST_PROTOCOL_H



#define MAX_SPI_BUFFER_SIZE   1600

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "FifoBuffer.h"

#define htole16(x)                        ((uint16_t)(x))
#define le16toh(x)                        ((uint16_t)(x))

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

/* Protobuf related info */
/* Endpoints registered must have same string length */
#define CTRL_EP_NAME_RESP                         "ctrlResp"
#define CTRL_EP_NAME_EVENT                        "ctrlEvnt"

struct esp_payload_header {
   uint8_t          if_type:4;
   uint8_t          if_num:4;
   uint8_t          flags;
   uint16_t         len;
   uint16_t         offset;
   uint16_t         checksum;
   uint16_t         seq_num;
   uint8_t          reserved2;
   /* Position of union field has to always be last,
    * this is required for hci_pkt_type */
   union {
      uint8_t      reserved3;
      uint8_t      hci_pkt_type;    /* Packet type for HCI interface */
      uint8_t      priv_pkt_type;      /* Packet type for priv interface */
   };
   /* Do no add anything here */
} __attribute__((packed));

typedef enum {
   ESP_STA_IF,
   ESP_AP_IF,
   ESP_SERIAL_IF,
   ESP_HCI_IF,
   ESP_PRIV_IF,
   ESP_TEST_IF,
   ESP_MAX_IF,
} ESP_INTERFACE_TYPE;

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



#define PROTO_PSER_TLV_T_EPNAME           0x01
#define PROTO_PSER_TLV_T_DATA             0x02

/* Protobuf related info */
/* Endpoints registered must have same string length */
#define CTRL_EP_NAME_RESP                         "ctrlResp"
#define CTRL_EP_NAME_EVENT                        "ctrlEvnt"

const uint16_t esp_ep_name_len = sizeof(CTRL_EP_NAME_RESP);
const uint16_t esp_payload_header_size = sizeof(struct esp_payload_header);
const uint16_t esp_tlv_header_size = 1 + 2 + esp_ep_name_len + 1 + 2;

const uint16_t esp_tlv_header_ep_name_type_pos = esp_payload_header_size + 1;
const uint16_t esp_tlv_header_ep_name_len_low_pos = esp_tlv_header_ep_name_type_pos + 1;
const uint16_t esp_tlv_header_ep_name_len_high_pos = esp_tlv_header_ep_name_len_low_pos + 1;
const uint16_t esp_tlv_header_ep_name_pos = esp_tlv_header_ep_name_len_high_pos + 1;
const uint16_t esp_tlv_header_ep_data_type_pos = esp_tlv_header_ep_name_pos + sizeof(CTRL_EP_NAME_RESP);
const uint16_t esp_tlv_header_ep_data_len_low_pos = esp_tlv_header_ep_data_type_pos + 1;
const uint16_t esp_tlv_header_ep_data_len_high_pos = esp_tlv_header_ep_data_len_low_pos + 1;
const uint16_t esp_tlv_header_payload_pos = esp_tlv_header_ep_data_len_high_pos + 1;

/* CMsg maps a buffer made in the following way (this class has been defined in 
   order to simplify and centralize the original way to deal with that buffers
   without changing the underlying message structure). Please note that 
   the positions and dimensions of this message structure are defined above.

+----------------------+--------------------------------+----------------------+
|  esp_payload_header  | [1 + 2 + 8 + 1 + 2] TLV struct |   PROTOBUF           |
+----------------------+--------------------------------+----------------------+

1. esp_payload_header contains the struct esp_payload_header just packed at the
   beginning of the message (in the original code this was the last piece
   attached to the message)

2. TVL "struct" 
   TLV (that means Type, Length, Value) is an additional header added before the 
   protobuf buffer. The 14 bytes are set in the following way:

   byte 1     - contains always   PROTO_PSER_TLV_T_EPNAME   i.e. 0x01
   byte 2,3   - contains the length of the enpoint name (i.e. or CTRL_EP_NAME_RESP 
                or CTRL_EP_NAME_EVENT) always 0x08 but low byte first (byte 2) 
                which is always equal to 8 and high byte in byte 3 always 0
   byte 4-11  - contains the EP_NAME_ 
   byte 12    - contains always - PROTO_PSER_TLV_T_DATA    i.e. 0x02
   byte 13,14 - contains the lenght of the protobuf data byte 13 low byte, 14 high byte

3. PROTOBUF with the actual request to be sent / received
*/


class CMsg {
private:
   /* pointer to the allocate buffer that will containg the message as defined 
      above*/
   uint8_t *buf;
   /* complete dimension of the buffer esp_payload_header + tlv struct + protobuf size 
      please note the at most 1600 can be sent */
   uint16_t dim;
   /* dimension of the protobuf part only */
   uint16_t proto_dim;
   /* pointer to the baginning of the buffer, used to manipulate the payload header
      structure */
   struct esp_payload_header *payload_header;
   
   /* allocate the buffer and set dim */
   uint32_t allocate(uint32_t d) {
      buf = new uint8_t[d];
      if(buf != nullptr) {
         /* memset all the buffer to 0 */
         memset(buf, 0x00, dim);
         dim = d;
         return dim;
      }
      return 0;
   }

   uint16_t compute_checksum(uint8_t *buf, uint16_t len) {
      uint16_t checksum = 0;
      uint16_t i = 0;

      while(i < len) {
         checksum += buf[i];
         i++;
      }
      return checksum;
   }

public:
   /*
    * CONSTRUCTORS (the 1 with argument to send message to SPI, the defaul one
    *               to receive message from SPI)
    */


   /* Default constructor: use it for messages coming from ESP32, then call store
      function to store the arrived message into the CMsg object*/
   CMsg() : buf{nullptr}, dim{0}, payload_header{nullptr}, proto_dim{0} {}

   /* Constructor with argument proto_size: use it for messages directed to the 
      ESP32 and pass it the size of the protobuf.
      This constructor will allocate a bigger buffer able to contain also 
      the TLV structure and esp_payload_header */
   CMsg(uint16_t proto_size) : buf{nullptr}, dim{0}, payload_header{nullptr}, proto_dim{proto_size} {
      uint16_t request_size = proto_dim + esp_payload_header_size + esp_tlv_header_size;
      if(request_size == allocate(request_size)) {
         /* initialize payload header to point at the beginning of the buffer */
         payload_header = (struct esp_payload_header *)buf;
      }
   }
   
   /* a function to verify buffer is valid, dim is set only if allocation is good */
   bool is_valid() {
      return (proto_dim > 0 && dim > 0);
   }
   /* get_protobuf_ptr() can be used to get the position the protobuffer is.
      That is used when protobuf pack function is used */
   uint8_t *get_protobuf_ptr() { return buf + esp_payload_header_size + esp_tlv_header_size; }  
   uint16_t get_protobuf_dim() { proto_dim; } 


   /* ---------------------------------------
    * function to deal with the TLV structure 
    * --------------------------------------- */
   
   /* set tlv structure */
   bool set_tlv_header(const char *ep_name) {
      /* ALL ep_name must have the same length */
      if(strlen(ep_name) != esp_ep_name_len) {
         return false;
      }
      /*  write TLV structure  */

      /* name */
      buf[esp_tlv_header_ep_name_type_pos]         = PROTO_PSER_TLV_T_EPNAME;
      buf[esp_tlv_header_ep_name_len_low_pos]      = (uint8_t)(esp_ep_name_len & 0xFF);  
      buf[esp_tlv_header_ep_name_len_high_pos]     = (uint8_t)((esp_ep_name_len >> 8) & 0xFF);
      memcpy(buf+esp_tlv_header_ep_name_pos, CTRL_EP_NAME_RESP, esp_ep_name_len);
      
      /* data */
      buf[esp_tlv_header_ep_data_type_pos]         = PROTO_PSER_TLV_T_DATA;
      buf[esp_tlv_header_ep_data_len_low_pos]      = (uint8_t)(proto_dim & 0xFF);
      buf[esp_tlv_header_ep_data_len_high_pos]     = (uint8_t)((proto_dim >> 8) & 0xFF);
      
      /* no need to memcpy protobuf, the buffer is already there */

      return true;
   }

   /* verify a TLV structure 
      work on a external buffer in order to verify TLV structure before to 
      */
   bool verify_rx_tlv_header(uint8_t *b) {
      /* verify type name */
      if(b[esp_tlv_header_ep_name_type_pos] != PROTO_PSER_TLV_T_EPNAME) {
         return false;
      }
      /* verify type data */
      if(b[esp_tlv_header_ep_data_type_pos]  != PROTO_PSER_TLV_T_DATA) {
         return false;
      }

      /* verify ep_name length */
      uint16_t l = b[esp_tlv_header_ep_name_len_low_pos];
      l += ((uint16_t)b[esp_tlv_header_ep_name_len_high_pos] << 8);

      if(l != esp_ep_name_len) {
         return false;
      }
      
      /* verify ep_name value*/
      if ((strncmp((char* )(b + esp_tlv_header_payload_pos),CTRL_EP_NAME_RESP,esp_ep_name_len) == 0) ||
          (strncmp((char* )(b + esp_tlv_header_payload_pos),CTRL_EP_NAME_EVENT,esp_ep_name_len) == 0)) {
         /* calculate protobuf dim */
      proto_dim = b[esp_tlv_header_ep_data_len_low_pos];
      proto_dim += b[esp_tlv_header_ep_data_len_high_pos] << 8;
      return true;
      }
      else {
         return false;
      }

   }

   /* ---------------------------------------
    * function to deal with the esp structure 
    * --------------------------------------- */
   void set_payload_header(uint8_t if_type, uint8_t if_num) {
      set_if_type(if_type);
      set_if_num(if_num); 
      set_payload_len(proto_dim + esp_tlv_header_size);
      set_payload_offset(esp_payload_header_size);
      set_checksum();
   }

   /* verify the payload header received from SPI it returns the payload's length
      i.e. TLV struct +  proto*/
   uint32_t verify_payload_header(uint8_t *b) {
      
      struct esp_payload_header *ph = (esp_payload_header *)b;
      if(ph->offset != esp_payload_header_size) {
         return 0; /* i.e. 0 to signal something is wrong */
      }
      if(ph->len > MAX_SPI_BUFFER_SIZE - esp_payload_header_size) {
         return 0; /* i.e. 0 to signal something is wrong */
      }
      if(!verify_rx_checksum(ph, ph->len + esp_payload_header_size)){
         return 0; /* i.e. 0 to signal something is wrong */
      }
      return ph->len;
   }

   /* setter / getter functions */ 
   void set_if_type(uint8_t if_type)        { payload_header->if_type = if_type; }
   uint8_t get_if_type()                    { return payload_header->if_type;}
   void set_if_num(uint8_t if_num)          { payload_header->if_num = if_num; }
   uint8_t get_if_num()                     { return payload_header->if_num; }
   void set_flags(uint8_t flags)            { payload_header->flags = flags; }
   uint8_t get_flags()                      { return payload_header->flags; }
   void set_payload_len(uint16_t len)       { payload_header->len = (uint16_t)len;}
   uint16_t get_payload_len(uint16_t len)   { return (uint16_t)payload_header->len;}
   void set_payload_offset(uint16_t offset) { payload_header->offset = (uint16_t)offset;}
   uint16_t get_payload_offset()            { return (uint16_t)payload_header->offset;}
   void set_checksum() { 
      payload_header->checksum = htole16(compute_checksum(buf,dim)); 
   }
   /* verify checksum use pointer to a user defined payload header because the 
      checksum is verified before to copy the rx_buffer into a new valid message */
   bool verify_rx_checksum(struct esp_payload_header *ph, uint16_t len) {
      uint16_t rx_checksum = le16toh(ph->checksum);
      /* reset checksum because it was 0 when it was calculated */
      ph->checksum = 0;
      uint16_t checksum = compute_checksum((uint8_t *)ph, len);  

      return (checksum != rx_checksum) ? false : true;
   }


   ~CMsg() { clear(); }
   
   CMsg(const CMsg& m) {
        buf            = m.buf;
        dim            = m.dim;
        payload_header = m.payload_header;
        proto_dim      = m.proto_dim;
   }
   
   void operator=(const CMsg& m) {
        buf            = m.buf;
        dim            = m.dim;
        payload_header = m.payload_header;
        proto_dim      = m.proto_dim;
    }
   
   void clear() {
      if(buf != nullptr) {
         delete []buf;
         buf = nullptr;
         dim = 0;
      }
      payload_header = nullptr;
      proto_dim = 0;
   }
   /* it is supposed that the buffer contains the received message from SPI
      that would always be static buffer rx_buffer with a fixed dimension
      of 1600 bytes */
   bool store_rx_buffer(uint8_t *buffer, uint32_t d) {
      /* rx_payload_len is TLV + PROTO */
      uint16_t rx_payload_len = verify_payload_header(buffer);
      if( rx_payload_len > 0 && verify_rx_tlv_header(buffer)) {
         /* verify the length and the checksum are correct, if ok it is safe to 
            initialize the message */

         clear(); /* just in case....*/

         uint16_t request_size = esp_payload_header_size + rx_payload_len;
         if(request_size == allocate(request_size)) {
            /* initialize payload header to point at the beginning of the buffer */
            payload_header = (esp_payload_header *)buf;
            memcpy(buf,buffer,request_size);
         }
      }      
   }    

   uint8_t *get_buffer() {return buf;}
   uint32_t get_size() {return dim;}

   bool read(uint8_t *dest, uint32_t d) {
      if(d >= dim) {
         memcpy(dest, buf, dim);
         clear();
         return true;
      }
      return false;
   }

};


bool application_send_msg_to_esp32(CMsg &msg, const char *ep_name, uint8_t if_type, uint8_t if_num);
bool esp32_receive_msg_to_be_sent_on_SPI(uint8_t *buffer, uint16_t dim);
bool esp32_send_msg_to_application(uint8_t *buffer, uint16_t dim);
bool application_receive_msg_from_esp32(CMsg &msg);




#endif