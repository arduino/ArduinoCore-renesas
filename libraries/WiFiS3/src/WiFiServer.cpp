#include "WiFiServer.h"

using namespace std;

/* -------------------------------------------------------------------------- */
WiFiServer::WiFiServer() : _sock(-1), _port(80) {}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
WiFiServer::WiFiServer(int p) : _sock(-1), _port(p) {}
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
WiFiClient WiFiServer::available() {
/* -------------------------------------------------------------------------- */
   if(_sock != -1) {
      string res = "";
      modem.begin();
      /* call the server available on esp so that the accept is performed */
      if(modem.write(string(PROMPT(_SERVERAVAILABLE)),res, "%s%d\r\n" , CMD_WRITE(_SERVERAVAILABLE), _sock)) {
         int client_sock = atoi(res.c_str());
         if(client._sock == client_sock) {
            return client;
         }
         else {
            client = WiFiClient(client_sock);
            return client;
         }
      }
   }
   return WiFiClient();
}
  
/* -------------------------------------------------------------------------- */
WiFiClient WiFiServer::accept() {
/* -------------------------------------------------------------------------- */
   if(_sock != -1) {
      string res = "";
      modem.begin();
      /* call the server accept on esp so that the accept is performed */
      if(modem.write(string(PROMPT(_SERVERACCEPT)),res, "%s%d\r\n" , CMD_WRITE(_SERVERACCEPT), _sock)) {
         int client_sock = atoi(res.c_str());
         return WiFiClient(client_sock);
      }
   }
   return WiFiClient();
}

/* -------------------------------------------------------------------------- */
void WiFiServer::begin(int port) {
/* -------------------------------------------------------------------------- */   
   if(_sock == -1) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_BEGINSERVER)),res, "%s%d\r\n" , CMD_WRITE(_BEGINSERVER), port)) {
         _sock = atoi(res.c_str());
      }
   }  
}

/* -------------------------------------------------------------------------- */
void WiFiServer::begin() {
/* -------------------------------------------------------------------------- */   
   begin(_port);
}



/* -------------------------------------------------------------------------- */
size_t WiFiServer::write(uint8_t b){
/* -------------------------------------------------------------------------- */   
   return write(&b, 1);
}

/* -------------------------------------------------------------------------- */
size_t WiFiServer::write(const uint8_t *buf, size_t size) {
/* -------------------------------------------------------------------------- */   
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      modem.write_nowait(string(PROMPT(_SERVERWRITE)),res, "%s%d,%d\r\n" , CMD_WRITE(_SERVERWRITE), _sock, size);
      if(modem.passthrough(buf,size)) {
         return size;
      }
      
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
void WiFiServer::end() {
/* -------------------------------------------------------------------------- */   
   if(_sock != -1) {
      string res = "";
      modem.begin();
      modem.write(string(PROMPT(_SERVEREND)),res, "%s%d\r\n" , CMD_WRITE(_SERVEREND), _sock);
      _sock = -1;
   }
}

WiFiServer::operator bool()
{
   return (_sock != -1);
}

bool WiFiServer::operator==(const WiFiServer& whs)
{
       return _sock == whs._sock;
}
