#include "CMsg.h"

//#define DEBUG_ESP_HOST_PROTOCOL

#define htole16(x)                        ((uint16_t)(x))
#define le16toh(x)                        ((uint16_t)(x))


#define PROTO_PSER_TLV_T_EPNAME           0x01
#define PROTO_PSER_TLV_T_DATA             0x02

/* Protobuf related info */
/* Endpoints registered must have same string length */
#define CTRL_EP_NAME_RESP                         "ctrlResp"
#define CTRL_EP_NAME_EVENT                        "ctrlEvnt"

const uint16_t esp_ep_name_len = strlen(CTRL_EP_NAME_RESP);
const uint16_t esp_payload_header_size = sizeof(struct esp_payload_header);
const uint16_t esp_tlv_header_size = 1 + 2 + esp_ep_name_len + 1 + 2;

const uint16_t esp_tlv_header_ep_name_type_pos = esp_payload_header_size;
const uint16_t esp_tlv_header_ep_name_len_low_pos = esp_tlv_header_ep_name_type_pos + 1;
const uint16_t esp_tlv_header_ep_name_len_high_pos = esp_tlv_header_ep_name_len_low_pos + 1;
const uint16_t esp_tlv_header_ep_name_pos = esp_tlv_header_ep_name_len_high_pos + 1;
const uint16_t esp_tlv_header_ep_data_type_pos = esp_tlv_header_ep_name_pos + strlen(CTRL_EP_NAME_RESP);
const uint16_t esp_tlv_header_ep_data_len_low_pos = esp_tlv_header_ep_data_type_pos + 1;
const uint16_t esp_tlv_header_ep_data_len_high_pos = esp_tlv_header_ep_data_len_low_pos + 1;
const uint16_t esp_tlv_header_payload_pos = esp_tlv_header_ep_data_len_high_pos + 1;

/* ##########################################################################
 *                            PRIVATE FUNCTIONS 
 * ########################################################################## */


/* -------------------------------------------------------------------------- */
uint32_t CMsg::allocate(uint32_t d) {
/* -------------------------------------------------------------------------- */   
   dim = 0;
   if(d < 1600) {
      buf = new uint8_t[d];
      if(buf != nullptr) {
         /* memset all the buffer to 0 */
         dim = d;
         memset(buf, 0x00, dim);
         payload_header = (struct esp_payload_header *)buf;
         return dim;
      }
      else {
         Serial.println("MSG ALLOCATION FAILED ALLOCATION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      }
   }
   else {
      Serial.println("REQUEST ALLOCATION TOO BIG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
uint16_t CMsg::compute_checksum(uint8_t *buf, uint16_t len) {
/* -------------------------------------------------------------------------- */   
   uint16_t checksum = 0;
   uint16_t i = 0;

   while(i < len) {
      checksum += buf[i];
      i++;
   }
   return checksum;
}

/* -------------------------------------------------------------------------- */
void CMsg::reset_without_delete() {
/* -------------------------------------------------------------------------- */   
   buf            = nullptr;
   dim            = 0;
   payload_header = nullptr;
   proto_dim      = 0;
   //tlv_size       = 0;
}


/* ##########################################################################
 * CONSTRUCTORS AND DESTRUCTORS, COPY CONSTRUCTOR, MOVE CONSTRUCTOR, .... 
 * ########################################################################## */


/* -------------------------------------------------------------------------- */
CMsg::CMsg() : buf{nullptr}, dim{0}, payload_header{nullptr}, proto_dim{0}, tlv_size(esp_tlv_header_size) {
   
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
CMsg::CMsg(uint16_t proto_size, bool use_tlv /*= true*/) : buf{nullptr}, 
                                                  dim{0}, 
                                                  payload_header{nullptr}, 
                                                  proto_dim{proto_size} {
/* -------------------------------------------------------------------------- */      
   uint16_t request_size = 0;
   if(use_tlv) {
      request_size = proto_dim + esp_payload_header_size + esp_tlv_header_size;
      tlv_size = esp_tlv_header_size;
   }
   else {
      request_size = proto_dim + esp_payload_header_size;
      tlv_size = 0;
   }  
   allocate(request_size);
}

/* -------------------------------------------------------------------------- */
CMsg::~CMsg() { 
/* -------------------------------------------------------------------------- */   
   #ifdef DEBUG_ESP_HOST_PROTOCOL
   Serial.println("[CMsg] DESTRUCTOR");
   #endif
   clear(); 
}

/* -------------------------------------------------------------------------- */
CMsg::CMsg(const CMsg& m) {
/* -------------------------------------------------------------------------- */   
   #ifdef DEBUG_ESP_HOST_PROTOCOL
   Serial.println("[CMsg] COPY");
   #endif
   if(allocate(m.dim) == m.dim) {
      memcpy(buf,m.buf,dim);
      payload_header = m.payload_header;
      proto_dim      = m.proto_dim;
      tlv_size       = m.tlv_size;
   }
   else {
      payload_header = nullptr;
      proto_dim      = 0;
      tlv_size       = 0;
   }
}

/* -------------------------------------------------------------------------- */
CMsg::CMsg(CMsg&& m) {
/* -------------------------------------------------------------------------- */      
   #ifdef DEBUG_ESP_HOST_PROTOCOL
   Serial.println("[CMsg] MOVE");
   #endif
   buf            = m.buf;
   dim            = m.dim;
   payload_header = m.payload_header;
   proto_dim      = m.proto_dim;
   tlv_size       = m.tlv_size;
   m.reset_without_delete();
}

/* -------------------------------------------------------------------------- */  
CMsg& CMsg::operator=(const CMsg& m) {
/* -------------------------------------------------------------------------- */      
   #ifdef DEBUG_ESP_HOST_PROTOCOL
   Serial.print("[CMsg] assign COPY operator");
   #endif
   if(this != &m) {
      #ifdef DEBUG_ESP_HOST_PROTOCOL
      Serial.println(" ACTUAL!");
      #endif
      clear();
      if(allocate(m.dim) == m.dim) {
         memcpy(buf,m.buf,dim);
         payload_header = m.payload_header;
         proto_dim      = m.proto_dim;
         tlv_size       = m.tlv_size;
      }
      else {
         payload_header = nullptr;
         proto_dim      = 0;
         tlv_size       = 0;
      }
   }
   else {
      #ifdef DEBUG_ESP_HOST_PROTOCOL
      Serial.println(" bad!");
      #endif
   }
   return *this;
 }
 
 /* -------------------------------------------------------------------------- */    
CMsg& CMsg::operator=(CMsg&& m) {
 /* -------------------------------------------------------------------------- */       
   #ifdef DEBUG_ESP_HOST_PROTOCOL
   Serial.print("[CMsg] assign MOVE operator");
   #endif
   if(this != &m) {
      #ifdef DEBUG_ESP_HOST_PROTOCOL
      Serial.println(" ACTUAL!");
      #endif
      clear();
      buf            = m.buf;
      dim            = m.dim;
      payload_header = m.payload_header;
      proto_dim      = m.proto_dim;
      tlv_size       = m.tlv_size;
      m.reset_without_delete();
   }
   else {
      #ifdef DEBUG_ESP_HOST_PROTOCOL
      Serial.println(" bad!");
      #endif
   }
}


/* ##########################################################################
 *                            PUBLIC FUNCTIONS 
 * ########################################################################## */

/* -------------------------------------------------------------------------- */ 
void CMsg::clear() {
/* -------------------------------------------------------------------------- */    
   #ifdef DEBUG_ESP_HOST_PROTOCOL
   Serial.print("[CMsg] clear ");
   #endif
   if(buf != nullptr) {
      #ifdef DEBUG_ESP_HOST_PROTOCOL
      Serial.println("msg buffer deleted!");
      #endif
      delete []buf;
      buf = nullptr;
      dim = 0;
   }
   else {
      #ifdef DEBUG_ESP_HOST_PROTOCOL
      Serial.println("msg buffer NOT deleted!");
      #endif
   }
   payload_header = nullptr;
   proto_dim = 0;
   //tlv_size = 0;
}

/* -------------------------------------------------------------------------- */ 
bool CMsg::is_valid() {
/* -------------------------------------------------------------------------- */    
      return (buf != nullptr && dim > 0 && payload_header != nullptr) ;
}

/* get_protobuf_ptr() can be used to get the position the protobuffer is.
   That is used when protobuf pack function is used */
/* -------------------------------------------------------------------------- */ 
uint8_t* CMsg::get_protobuf_ptr() { 
/* -------------------------------------------------------------------------- */    
   return buf + esp_payload_header_size + tlv_size; 
}  

/* this function is equivalent to the get_protobuf_ptr when tlv_size is 0 
   use it only in case of net message to be sent */
/* -------------------------------------------------------------------------- */ 
uint8_t *CMsg::data() {
/* -------------------------------------------------------------------------- */    
   return buf + esp_payload_header_size; 
}

/* -------------------------------------------------------------------------- */   
uint16_t CMsg::get_protobuf_dim() { 
/* -------------------------------------------------------------------------- */   
   return proto_dim; 
} 



/* -------------------------------------------------------------------------- */ 
bool CMsg::set_tlv_header(const char *ep_name) {
/* -------------------------------------------------------------------------- */    
   if(tlv_size > 0) {

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
   }
   return true;
}

   
/* -------------------------------------------------------------------------- */ 
bool CMsg::verify_tlv_header() {
/* -------------------------------------------------------------------------- */    
   if(tlv_size == 0) {
      /* if there is not tlv header then it is certainly right ! */
      return true;
   }

   /* verify type name */
   if(buf[esp_tlv_header_ep_name_type_pos] != PROTO_PSER_TLV_T_EPNAME) {      
      return false;
   }
   /* verify type data */
   if(buf[esp_tlv_header_ep_data_type_pos]  != PROTO_PSER_TLV_T_DATA) {
      return false;
   }

   /* verify ep_name length */
   uint16_t l = buf[esp_tlv_header_ep_name_len_low_pos];
   l += ((uint16_t)buf[esp_tlv_header_ep_name_len_high_pos] << 8);

   if(l != esp_ep_name_len) {
      return false;
   }

   /* verify ep_name value*/
   if ((strncmp((char* )(buf + esp_tlv_header_ep_name_pos),CTRL_EP_NAME_RESP,esp_ep_name_len) == 0) ||
       (strncmp((char* )(buf + esp_tlv_header_ep_name_pos),CTRL_EP_NAME_EVENT,esp_ep_name_len) == 0)) {
      /* calculate protobuf dim */
   proto_dim = buf[esp_tlv_header_ep_data_len_low_pos];
   proto_dim += buf[esp_tlv_header_ep_data_len_high_pos] << 8;
   
   return true;
   }
   else {
      
      return false;
   }
}

/* -------------------------------------------------------------------------- */
void CMsg::set_payload_header(uint8_t if_type, uint8_t if_num) {
/* -------------------------------------------------------------------------- */   
   set_if_type(if_type);
   set_if_num(if_num); 
   set_payload_len(proto_dim + tlv_size);
   set_payload_offset(esp_payload_header_size);
   set_checksum();
}

/* -------------------------------------------------------------------------- */
uint32_t CMsg::verify_payload_header(const uint8_t *b) {
/* -------------------------------------------------------------------------- */         
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

/* -------------------------------------------------------------------------- */
void CMsg::set_tlv_size() {
/* -------------------------------------------------------------------------- */   
   if(get_if_type() == ESP_SERIAL_IF) {
      tlv_size = esp_tlv_header_size;
   }
   else {
      tlv_size = 0;
      proto_dim = get_payload_len();
   }
}


/* setter / getter functions */ 
void CMsg::set_if_type(uint8_t if_type)        { 
   if(payload_header != nullptr) {
      payload_header->if_type = if_type;
   }
}
uint8_t CMsg::get_if_type()  { 
   if(payload_header != nullptr) {
      return payload_header->if_type;
   }
   else {
      Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      return 10;
   }
}
void CMsg::set_if_num(uint8_t if_num)          { 
   payload_header->if_num = if_num; 
}
uint8_t CMsg::get_if_num()                     { return payload_header->if_num; }
void CMsg::set_flags(uint8_t flags)            { payload_header->flags = flags; }
uint8_t CMsg::get_flags()                      { return payload_header->flags; }
void CMsg::set_payload_len(uint16_t len)       { payload_header->len = (uint16_t)len;}
uint16_t CMsg::get_payload_len(void)           { return (uint16_t)payload_header->len;}
void CMsg::set_payload_offset(uint16_t offset) { payload_header->offset = (uint16_t)offset;}
uint16_t CMsg::get_payload_offset()            { return (uint16_t)payload_header->offset;}
void CMsg::set_seq_num(uint16_t sq)            { payload_header->seq_num = sq;}
uint16_t CMsg::get_seq_num()                   { return payload_header->seq_num;}

/* -------------------------------------------------------------------------- */
void CMsg::set_checksum() { 
/* -------------------------------------------------------------------------- */   
   payload_header->checksum = htole16(compute_checksum(buf,dim)); 
}

/* -------------------------------------------------------------------------- */
bool CMsg::verify_rx_checksum(struct esp_payload_header *ph, uint16_t len) {
/* -------------------------------------------------------------------------- */   
   uint16_t rx_checksum = le16toh(ph->checksum);
   /* reset checksum because it was 0 when it was calculated */
   ph->checksum = 0;
   uint16_t checksum = compute_checksum((uint8_t *)ph, len);  

   return (checksum != rx_checksum) ? false : true;
}

/* -------------------------------------------------------------------------- */
void CMsg::debug_print(const char* title) {
/* -------------------------------------------------------------------------- */   
   Serial.println(title);
   Serial.print("dim: ");
   Serial.println(dim);
   Serial.print("proto_dim: ");
   Serial.println(proto_dim);
   Serial.print("tlv_size: ");
   Serial.println(tlv_size);

   for(int i = 0; i < dim; i++) {
      Serial.print(*(buf+i),HEX);
      Serial.print(" ");
   }
   Serial.println();
}

/* store_rx_buffer receives all type of messages: 
   - serial control messages have a tlv header
   - other type don't... */

/* -------------------------------------------------------------------------- */
bool CMsg::store_rx_buffer(const uint8_t *buffer, uint32_t d) {
/* -------------------------------------------------------------------------- */   
   /* rx_payload_len is TLV + PROTO (tlv can be present or not) */
   uint16_t rx_payload_len = verify_payload_header(buffer);
   
   if( rx_payload_len > 0 ) {
      /* verify the length and the checksum are correct, if ok it is safe to 
         initialize the message */

      clear(); /* just in case....*/

      uint16_t request_size = esp_payload_header_size + rx_payload_len;
      /* allocate make the payload_header to point at the beginning of the
         newly allocate buffer  */
      if(request_size == allocate(request_size)) {
         /* initialize payload header to point at the beginning of the buffer */
         //payload_header = (esp_payload_header *)buf;
         memcpy(buf,buffer,request_size);

         /* now there are all the information to know if there is a tlv header or not*/
         set_tlv_size();
         return true;
      }
   }     
   return false;
}

/* -------------------------------------------------------------------------- */
bool CMsg::add_msg(CMsg &msg) {
/* -------------------------------------------------------------------------- */   
   if(buf == nullptr && dim == 0) {
      /* message was never used, simply steal from the msg */
      buf = msg.get_buffer();
      dim = msg.get_size();
      payload_header = (esp_payload_header *)buf;
      proto_dim = msg.get_protobuf_dim(); /* should alway be 0 when this function is called */
      /* reset the message added so that is not deleted if clear is called */
      msg.reset_without_delete();

   }
   else {
      uint16_t new_bigger_dim = dim + msg.get_payload_len();
      uint8_t *new_bigger_buf = new uint8_t[new_bigger_dim];
      if(new_bigger_buf != nullptr) {
         /* copy the old data in the new bigger buffer */
         memcpy(new_bigger_buf,buf,dim);
         memcpy(new_bigger_buf+dim,msg.get_buffer() + esp_payload_header_size,msg.get_payload_len());
         proto_dim = 0; //msg.get_protobuf_dim(); /* should alway be 0 when this function is called */
         msg.clear();
         clear();
         buf = new_bigger_buf;
         dim = new_bigger_dim;
         payload_header = (esp_payload_header *)buf; 
      }
      else {
         return false;
      }
   }
} 

/* -------------------------------------------------------------------------- */
uint8_t *CMsg::get_buffer() {return buf;}
/* -------------------------------------------------------------------------- */
uint32_t CMsg::get_size() {return dim;}
/* -------------------------------------------------------------------------- */   

/* -------------------------------------------------------------------------- */
bool CMsg::read(uint8_t *dest, uint32_t d) {
/* -------------------------------------------------------------------------- */      
   if(d >= dim) {
      memcpy(dest, buf, dim);
      clear();
      return true;
   }
   return false;
}   
