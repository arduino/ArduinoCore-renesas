#include "WiFiUdp.h"

using namespace std;

/* -------------------------------------------------------------------------- */ 
WiFiUDP::WiFiUDP() : _sock(-1) {
/* -------------------------------------------------------------------------- */    

} 

/* -------------------------------------------------------------------------- */
uint8_t WiFiUDP::begin(uint16_t p) {
/* -------------------------------------------------------------------------- */    
   if(_sock == -1) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_UDPBEGIN)),res, "%s%d\r\n" , CMD_WRITE(_UDPBEGIN),p)) {
         _sock = atoi(res.c_str());
         return 1;
      }
   }   
   return 0;
} 

/* -------------------------------------------------------------------------- */
uint8_t WiFiUDP::begin(IPAddress a, uint16_t p) {
/* -------------------------------------------------------------------------- */   
   if(_sock == -1) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_UDPBEGIN)),res, "%s%d,%s\r\n" , CMD_WRITE(_UDPBEGIN),p,a.toString().c_str())) {
         _sock = atoi(res.c_str());
         return 1;
      }
   }   
   return 0;
}

/* -------------------------------------------------------------------------- */
uint8_t WiFiUDP::beginMulticast(IPAddress a, uint16_t p) {
/* -------------------------------------------------------------------------- */    
   if(_sock == -1) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_UDPBEGINMULTI)),res, "%s%d,%s\r\n" , CMD_WRITE(_UDPBEGINMULTI),p,a.toString().c_str())) {
         _sock = atoi(res.c_str());
         return 1;
      }
   }   
   return 0;
} 

/* -------------------------------------------------------------------------- */
void WiFiUDP::stop() {
/* -------------------------------------------------------------------------- */    
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      modem.write(string(PROMPT(_UDPSTOP)),res, "%s%d\r\n" , CMD_WRITE(_UDPSTOP), _sock);
      _sock = -1;
   }
} 

/* -------------------------------------------------------------------------- */
int WiFiUDP::beginPacket(IPAddress ip, uint16_t p) {
/* -------------------------------------------------------------------------- */    
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_UDPBEGINPACKETIP)),res, "%s%d,%d,%s\r\n" , CMD_WRITE(_UDPBEGINPACKETIP),_sock,p,ip.toString().c_str())) {
         return 1;
      }
   }   
   return 0;
} 

/* -------------------------------------------------------------------------- */
int WiFiUDP::beginPacket(const char *host, uint16_t p) {
/* -------------------------------------------------------------------------- */    
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_UDPBEGINPACKETNAME)),res, "%s%d,%d,%s\r\n" , CMD_WRITE(_UDPBEGINPACKETNAME),_sock,p,host)) {
         return 1;
      }
   }   
   return 0;
} 

/* -------------------------------------------------------------------------- */
int WiFiUDP::beginMulticastPacket() {
/* -------------------------------------------------------------------------- */  
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_UDPBEGINPACKETMULTI)),res, "%s%d\r\n" , CMD_WRITE(_UDPBEGINPACKETMULTI), _sock)) {
         return 1;
      }
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
int WiFiUDP::endPacket() {
/* -------------------------------------------------------------------------- */    
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_UDPENDPACKET)),res, "%s%d\r\n" , CMD_WRITE(_UDPENDPACKET), _sock)) {
         return 1;
      }
   }
   return 0;
} 

/* -------------------------------------------------------------------------- */
size_t WiFiUDP::write(uint8_t b) {
/* -------------------------------------------------------------------------- */    
   return write(&b, 1);
} 

/* -------------------------------------------------------------------------- */
size_t WiFiUDP::write(const uint8_t *buf, size_t size) {
/* -------------------------------------------------------------------------- */    
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      modem.write_nowait(string(PROMPT(_UDPWRITE)),res, "%s%d,%d\r\n" , CMD_WRITE(_UDPWRITE), _sock, size);
      if(modem.passthrough(buf,size)) {
         return size;
      }
      
   }
   return 0;
} 

/* -------------------------------------------------------------------------- */
int WiFiUDP::parsePacket() {
/* -------------------------------------------------------------------------- */    
   if(_sock >= 0) {
      string res = "";
      rx_buffer.clear();
      modem.begin();
      if(modem.write(string(PROMPT(_UDPPARSE)),res, "%s%d\r\n" , CMD_WRITE(_UDPPARSE), _sock)) {
         return atoi(res.c_str());
      }
   }
   return 0;
} 

/* -------------------------------------------------------------------------- */
int WiFiUDP::available() {
/* -------------------------------------------------------------------------- */    
   int rv = 0;
   if(_sock >= 0) {
      if(rx_buffer.available() > 0) {
         return rx_buffer.available();
      }
      else {
         string res = "";
         modem.begin();
         if(modem.write(string(PROMPT(_UDPAVAILABLE)),res, "%s%d\r\n" , CMD_WRITE(_UDPAVAILABLE), _sock)) {
            rv = atoi(res.c_str());
         }  
      }
   }
   return rv;
} 

/* -------------------------------------------------------------------------- */
int WiFiUDP::_read() {
/* -------------------------------------------------------------------------- */
   int rv = -1;
   if(_sock >= 0) {
      string res = "";
      uint32_t size = rx_buffer.freePositions() - 1;
      modem.begin();
      
      /* important - it works one shot */
      modem.avoid_trim_results();
      modem.read_using_size();
      if(modem.write(string(PROMPT(_UDPREAD)),res, "%s%d,%d\r\n" , CMD_WRITE(_UDPREAD), _sock, size)) {
         if(res.size() > 0) {
            for(uint32_t i = 0, rv = 0; i < size && i < res.size(); i++) {
               rx_buffer.store((uint8_t)res[i]);
               rv++;
            }
         }
         else {
            rv = 0;
         }
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
void WiFiUDP::read_if_needed(size_t s) {
/* -------------------------------------------------------------------------- */
   if((size_t)rx_buffer.available() < s) {
      _read();
   }
}

/* -------------------------------------------------------------------------- */
int WiFiUDP::read() {
/* -------------------------------------------------------------------------- */    
   uint8_t b;
   if(read(&b, 1) == 1) {
      return b;
   }
   return -1;
} 

/* -------------------------------------------------------------------------- */
int WiFiUDP::read(unsigned char* buf, size_t size) {
/* -------------------------------------------------------------------------- */    
   read_if_needed(size);
   int rv = 0;
   bool go_on = true;
   for(size_t i = 0; i < size && go_on; i++) {
      bool is_read = false;
      *(buf+i) = rx_buffer.read(&is_read);
      if(is_read) {
         rv++;
      }
      else {
         go_on = false;
      }
   }
   return rv; 
} 

/* -------------------------------------------------------------------------- */
int WiFiUDP::peek() {
/* -------------------------------------------------------------------------- */    
   int rv = -1;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_UDPPEEK)),res, "%s%d\r\n" , CMD_WRITE(_UDPPEEK), _sock)) {
         rv = atoi(res.c_str());
      }  
   }
   return rv;
} 

/* -------------------------------------------------------------------------- */
void WiFiUDP::flush() {
/* -------------------------------------------------------------------------- */    
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      modem.write(string(PROMPT(_UDPFLUSH)),res, "%s%d\r\n" , CMD_WRITE(_UDPFLUSH), _sock);
   }
} 

bool WiFiUDP::operator==(const WiFiUDP& whs)
{
   return _sock == whs._sock;
}

/* -------------------------------------------------------------------------- */
IPAddress WiFiUDP::remoteIP() {
/* -------------------------------------------------------------------------- */    
   IPAddress ip;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_UDPREMOTEIP)),res, "%s%d\r\n" , CMD_WRITE(_UDPREMOTEIP), _sock)) {
         ip.fromString(res.c_str());
         return ip;
      }  
   }
   return IPAddress(0,0,0,0);
} 

/* -------------------------------------------------------------------------- */
uint16_t WiFiUDP::remotePort() {
/* -------------------------------------------------------------------------- */    
   uint16_t rv = 0;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_UDPREMOTEPORT)),res, "%s%d\r\n" , CMD_WRITE(_UDPREMOTEPORT), _sock)) {
         rv = atoi(res.c_str());
         return rv;
      }  
   }
   return rv; 
} 

