#include "WiFiClient.h"

/* -------------------------------------------------------------------------- */
WiFiClient::WiFiClient() : _sock(-1) { }
/* -------------------------------------------------------------------------- */
   
/* -------------------------------------------------------------------------- */
void WiFiClient::getSocket() {
/* -------------------------------------------------------------------------- */
   if(_sock == -1) {
      string res = "";
      modem.begin();

      if(modem.write(string(PROMPT(_BEGINCLIENT)),res, "%s" , CMD(_BEGINCLIENT))) {
         _sock = atoi(res.c_str());
         Serial.print("GET SOCKET: ");
         Serial.println(_sock);
      }
   }   
}



/* -------------------------------------------------------------------------- */
int WiFiClient::connect(IPAddress ip, uint16_t port){
/* -------------------------------------------------------------------------- */   
   getSocket();
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_CLIENTCONNECTIP)),res, "%s%d,%s,%d\r\n" , CMD_WRITE(_CLIENTCONNECTIP), _sock, ip.toString(),port)) {
         return 1;
      }  
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
int WiFiClient::connect(const char *host, uint16_t port){
/* -------------------------------------------------------------------------- */   
   getSocket();
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_CLIENTCONNECTNAME)),res, "%s%d,%s,%d\r\n" , CMD_WRITE(_CLIENTCONNECTNAME), _sock, host,port)) {
         return 1;
      }  
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
size_t WiFiClient::write(uint8_t b){
/* -------------------------------------------------------------------------- */   
   return write(&b, 1);
}

/* -------------------------------------------------------------------------- */
size_t WiFiClient::write(const uint8_t *buf, size_t size){
/* -------------------------------------------------------------------------- */
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      modem.write_nowait(string(PROMPT(_CLIENTSEND)),res, "%s%d,%d\r\n" , CMD_WRITE(_CLIENTSEND), _sock, size);
      if(modem.passthrough(buf,size)) {
         return size;
      }
      
   }
   return 0;

}



/* -------------------------------------------------------------------------- */
int WiFiClient::available(){
/* -------------------------------------------------------------------------- */   
   int rv = 0;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_AVAILABLE)),res, "%s%d\r\n" , CMD_WRITE(_AVAILABLE), _sock)) {
         rv = atoi(res.c_str());
      }  
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
int WiFiClient::read() {
/* -------------------------------------------------------------------------- */   
   uint8_t b;
   if(read(&b, 1) == 1) {
      return b;
   }
  return -1;
}

/* -------------------------------------------------------------------------- */
int WiFiClient::read(uint8_t *buf, size_t size) {
/* -------------------------------------------------------------------------- */
   int rv = -1;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      vector<string> tokens;
      if(modem.write(string(PROMPT(_CLIENTRECEIVE)),res, "%s%d,%d\r\n" , CMD_WRITE(_CLIENTRECEIVE), _sock, size)) {
         split(tokens, res, string("|"));
         if(tokens.size() >= 2) {
            rv = atoi(tokens[0].c_str());
            memcpy(buf,tokens[1].data(), (rv < size) ? rv : size);
         }
      }  
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
int WiFiClient::peek() {
/* -------------------------------------------------------------------------- */   
   int rv = -1;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_PEEK)),res, "%s%d\r\n" , CMD_WRITE(_PEEK), _sock)) {
         rv = atoi(res.c_str());
      }  
   }
   return rv;
}


/* -------------------------------------------------------------------------- */
void WiFiClient::flush() {
/* -------------------------------------------------------------------------- */   
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      modem.write(string(PROMPT(_CLIENTFLUSH)),res, "%s%d\r\n" , CMD_WRITE(_CLIENTFLUSH), _sock);
   }
}

/* -------------------------------------------------------------------------- */
void WiFiClient::stop() {
/* -------------------------------------------------------------------------- */   
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      modem.write(string(PROMPT(_CLIENTCLOSE)),res, "%s%d\r\n" , CMD_WRITE(_CLIENTCLOSE), _sock);
      _sock = -1;
   }
}

/* -------------------------------------------------------------------------- */
uint8_t WiFiClient::connected() {
/* -------------------------------------------------------------------------- */   
   uint8_t rv = 0;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_CLIENTCONNECTED)),res, "%s%d\r\n" , CMD_WRITE(_CLIENTCONNECTED), _sock)) {
         rv = atoi(res.c_str());
      }  
   }
   return rv;
}



/* -------------------------------------------------------------------------- */
IPAddress WiFiClient::remoteIP() {
/* -------------------------------------------------------------------------- */
   IPAddress ip;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_REMOTEIP)),res, "%s%d\r\n" , CMD_WRITE(_REMOTEIP), _sock)) {
         ip.fromString(res.c_str());
         return ip;
      }  
   }
   return IPAddress(0,0,0,0);
}

/* -------------------------------------------------------------------------- */
uint16_t WiFiClient::remotePort(){
/* -------------------------------------------------------------------------- */  
   uint16_t rv = 0;
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_REMOTEPORT)),res, "%s%d\r\n" , CMD_WRITE(_REMOTEPORT), _sock)) {
         rv = atoi(res.c_str());
         return rv;
      }  
   }
   return rv; 
}