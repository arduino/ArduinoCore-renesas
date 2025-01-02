#include "WiFiSSLClient.h"

using namespace std;

/* -------------------------------------------------------------------------- */
WiFiSSLClient::WiFiSSLClient() : _sock(-1) {
/* -------------------------------------------------------------------------- */
   rx_buffer = std::shared_ptr<FifoBuffer<uint8_t,RX_BUFFER_DIM>>(new FifoBuffer<uint8_t,RX_BUFFER_DIM>());
}

/* -------------------------------------------------------------------------- */
WiFiSSLClient::~WiFiSSLClient() {
/* -------------------------------------------------------------------------- */

}

/* -------------------------------------------------------------------------- */
void WiFiSSLClient::getSocket() {
/* -------------------------------------------------------------------------- */
   if(_sock == -1) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_SSLBEGINCLIENT)),res, "%s" , CMD(_SSLBEGINCLIENT))) {
         _sock = atoi(res.c_str());
      }
   }   
}

/* -------------------------------------------------------------------------- */
int WiFiSSLClient::connect(IPAddress ip, uint16_t port) {
/* -------------------------------------------------------------------------- */
   getSocket();

   string res = "";
   if(modem.write(string(PROMPT(_SSLCLIENTCONNECTIP)),res, "%s%d,%s,%d\r\n" , CMD_WRITE(_SSLCLIENTCONNECTIP), _sock, ip.toString(), port)) {
      return 1;
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
int WiFiSSLClient::connect(const char* host, uint16_t port) {
/* -------------------------------------------------------------------------- */
   getSocket();
   if (!_custom_root) {
      setCACert();
   }
   string res = "";
   if(modem.write(string(PROMPT(_SSLCLIENTCONNECTNAME)),res, "%s%d,%s,%d\r\n" , CMD_WRITE(_SSLCLIENTCONNECTNAME), _sock, host, port)) {
      return 1;
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
void WiFiSSLClient::setCACert(const char* root_ca, size_t size) {
/* -------------------------------------------------------------------------- */
   getSocket();
   string res = "";
   if(size > 0) {
      modem.write_nowait(string(PROMPT(_SETCAROOT)),res, "%s%d,%d\r\n" , CMD_WRITE(_SETCAROOT), _sock, size);
      if(modem.passthrough((uint8_t *)root_ca, size)) {
         _custom_root = true;
      }
   } else {
      modem.write(string(PROMPT(_SETCAROOT)),res, "%s%d\r\n" , CMD_WRITE(_SETCAROOT), _sock);
   }
}

/* -------------------------------------------------------------------------- */
size_t WiFiSSLClient::write(uint8_t b){
/* -------------------------------------------------------------------------- */   
   return write(&b, 1);
}

/* -------------------------------------------------------------------------- */
size_t WiFiSSLClient::write(const uint8_t *buf, size_t size){
/* -------------------------------------------------------------------------- */
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      modem.write_nowait(string(PROMPT(_SSLCLIENTSEND)),res, "%s%d,%d\r\n" , CMD_WRITE(_SSLCLIENTSEND), _sock, size);
      if(modem.passthrough(buf,size)) {
         return size;
      }
      
   }
   return 0;

}

/* -------------------------------------------------------------------------- */
int WiFiSSLClient::available(){
/* -------------------------------------------------------------------------- */   
   int rv = 0;
   if(_sock >= 0 && rx_buffer != nullptr) {
      if(rx_buffer->available() > 0) {
         return rx_buffer->available();
      }
      else {
         string res = "";
         modem.begin();
         if(modem.write(string(PROMPT(_SSLAVAILABLE)),res, "%s%d\r\n" , CMD_WRITE(_SSLAVAILABLE), _sock)) {
            rv = atoi(res.c_str());
            if (rv < 0) {
               return 0;
            }
         }  
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
int WiFiSSLClient::_read() {
/* -------------------------------------------------------------------------- */   
   int rv = -1;
   if(_sock >= 0) {
      string res = "";
      uint32_t size = rx_buffer->freePositions() - 1;
      modem.begin();
      
      /* important - it works one shot */
      modem.avoid_trim_results();
      modem.read_using_size();
      if(modem.write(string(PROMPT(_SSLCLIENTRECEIVE)),res, "%s%d,%d\r\n" , CMD_WRITE(_SSLCLIENTRECEIVE), _sock, size)) {
         if(res.size() > 0) {
            for(int i = 0, rv = 0; i < size && i < res.size(); i++) {
               rx_buffer->store((uint8_t)res[i]);
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
void WiFiSSLClient::read_if_needed(size_t s) {
/* -------------------------------------------------------------------------- */   
   if((size_t)rx_buffer->available() < s) {
      _read();
   }
}

/* -------------------------------------------------------------------------- */
int WiFiSSLClient::read() {
/* -------------------------------------------------------------------------- */   
   uint8_t b;
   if(read(&b, 1) == 1) {
      return b;
   }
  return -1;
}

/* -------------------------------------------------------------------------- */
int WiFiSSLClient::read(uint8_t *buf, size_t size) {
/* -------------------------------------------------------------------------- */
   read_if_needed(size);
   int rv = 0;
   bool go_on = true;
   for(int i = 0; i < size && go_on; i++) {
      bool is_read = false;
      *(buf+i) = rx_buffer->read(&is_read);
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
int WiFiSSLClient::peek() {
/* -------------------------------------------------------------------------- */   
   int rv = -1;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_SSLPEEK)),res, "%s%d\r\n" , CMD_WRITE(_SSLPEEK), _sock)) {
         rv = atoi(res.c_str());
      }  
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
void WiFiSSLClient::flush() {
/* -------------------------------------------------------------------------- */   
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      modem.write(string(PROMPT(_SSLCLIENTFLUSH)),res, "%s%d\r\n" , CMD_WRITE(_SSLCLIENTFLUSH), _sock);
   }
}

/* -------------------------------------------------------------------------- */
void WiFiSSLClient::stop() {
/* -------------------------------------------------------------------------- */   
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      modem.write(string(PROMPT(_SSLCLIENTCLOSE)),res, "%s%d\r\n" , CMD_WRITE(_SSLCLIENTCLOSE), _sock);
      _sock = -1;
   }
}

/* -------------------------------------------------------------------------- */
uint8_t WiFiSSLClient::connected() {
/* -------------------------------------------------------------------------- */   
   uint8_t rv = 0;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_SSLCLIENTCONNECTED)),res, "%s%d\r\n" , CMD_WRITE(_SSLCLIENTCONNECTED), _sock)) {
         rv = atoi(res.c_str());
      }  
   }
   return rv;
}
bool WiFiSSLClient::operator==(const WiFiSSLClient& whs)
{
       return _sock == whs._sock;
}

/* -------------------------------------------------------------------------- */
IPAddress WiFiSSLClient::remoteIP() {
/* -------------------------------------------------------------------------- */
   IPAddress ip;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_SSLREMOTEIP)),res, "%s%d\r\n" , CMD_WRITE(_SSLREMOTEIP), _sock)) {
         ip.fromString(res.c_str());
         return ip;
      }  
   }
   return IPAddress(0,0,0,0);
}

/* -------------------------------------------------------------------------- */
uint16_t WiFiSSLClient::remotePort(){
/* -------------------------------------------------------------------------- */  
   uint16_t rv = 0;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_SSLREMOTEPORT)),res, "%s%d\r\n" , CMD_WRITE(_SSLREMOTEPORT), _sock)) {
         rv = atoi(res.c_str());
         return rv;
      }  
   }
   return rv; 
}