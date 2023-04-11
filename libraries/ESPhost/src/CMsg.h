#ifndef _ARDUINO_ESP_MESSAGE_CLASS_H
#define _ARDUINO_ESP_MESSAGE_CLASS_H
#include "Arduino.h"
#include <string.h>

#define MAX_SPI_BUFFER_SIZE   1600

/* Protobuf related info */
/* Endpoints registered must have same string length */
#define CTRL_EP_NAME_RESP                         "ctrlResp"
#define CTRL_EP_NAME_EVENT                        "ctrlEvnt"

typedef enum {
   ESP_STA_IF,
   ESP_AP_IF,
   ESP_SERIAL_IF,
   ESP_HCI_IF,
   ESP_PRIV_IF,
   ESP_TEST_IF,
   ESP_MAX_IF,
} ESP_INTERFACE_TYPE;

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

*** PLEASE NOTE: The previous structure is valid only for Control Message (! what the...)
                 For Network messages the TLV structure is not used
                 This led to some changes to this class
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

   /* that size can be 0 or esp_tlv_header_size depending on the fact that 
      the CMsg uses TLV header or not */
   uint16_t tlv_size;
   
   /* allocate the buffer and set dim */
   uint32_t allocate(uint32_t d);
   /* compute the checksum to be inserted in the esp payload header struct */
   uint16_t compute_checksum(uint8_t *buf, uint16_t len);
   /* reset the message but do not delete the attached buffer*/
   void reset_without_delete();

public:
   /*
    * CONSTRUCTORS (the 1 with argument to send message to SPI, the defaul one
    *               to receive message from SPI)
    */

   /* Default constructor: use it for messages coming from ESP32, then call store
      function to store the arrived message into the CMsg object*/
   CMsg();

   /* Constructor with argument proto_size: use it for messages directed to the 
      ESP32 and pass it the size of the protobuf.
      This constructor will allocate a bigger buffer able to contain also 
      the TLV structure and esp_payload_header in case the use_tlv flag is true*/
   CMsg(uint16_t proto_size, bool use_tlv = true);
   /* delete the message */
   void clear();
   /* destructor (calls clear) */
   ~CMsg();
   /* copy constructor */
   CMsg(const CMsg& m);
   /* move constructor */
   CMsg(CMsg&& m);
   /* assign copy operator */
   CMsg& operator=(const CMsg& m);
   /* assign move operator */
   CMsg& operator=(CMsg&& m);
   
   /* a function to verify buffer is valid, dim is set only if allocation is good */
   bool is_valid();
   /* get_protobuf_ptr() can be used to get the position the protobuffer is.
      That is used when protobuf pack function is used 

      protobuf here is the real protobuf binary array in case of control message
      or the data to be sent and received by network interfaces */
   uint8_t *get_protobuf_ptr();  
   uint16_t get_protobuf_dim(); 
   uint8_t *data();
   

   /* ---------------------------------------
    * function to deal with the TLV structure 
    * --------------------------------------- */   
   /* set tlv structure */
   bool set_tlv_header(const char *ep_name);
   /* verify a TLV structure */
   bool verify_tlv_header();
   void set_tlv_size();

   /* ---------------------------------------
    * function to deal with the esp structure 
    * --------------------------------------- */
   void set_payload_header(uint8_t if_type, uint8_t if_num);

   /* verify the payload header received from SPI it returns the payload's length
      i.e. TLV struct +  proto*/
   uint32_t verify_payload_header(const uint8_t *b);

   /* setter / getter functions */ 
   void set_if_type(uint8_t if_type);
   uint8_t get_if_type();
   void set_if_num(uint8_t if_num);
   uint8_t get_if_num();
   void set_flags(uint8_t flags);
   uint8_t get_flags();
   void set_payload_len(uint16_t len);
   uint16_t get_payload_len(void);
   void set_payload_offset(uint16_t offset);
   uint16_t get_payload_offset();
   void set_seq_num(uint16_t sq);
   uint16_t get_seq_num();

   void set_checksum();
   /* verify checksum use pointer to a user defined payload header because the 
      checksum is verified before to copy the rx_buffer into a new valid message */
   bool verify_rx_checksum(struct esp_payload_header *ph, uint16_t len);

   /* it is supposed that the buffer contains the received message from SPI
      that would always be static buffer rx_buffer with a fixed dimension
      of 1600 bytes */
   bool store_rx_buffer(const uint8_t *buffer, uint32_t d);
   /* message to add payload in case of FRAGMENTS */
   bool add_msg(CMsg &msg);

   uint8_t *get_buffer();
   uint32_t get_size();

   bool read(uint8_t *dest, uint32_t d);

   void debug_print(const char* title);
};




#endif