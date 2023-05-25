#include "WiFiSSLClient.h"

/* -------------------------------------------------------------------------- */
WiFiSSLClient::WiFiSSLClient() {
/* -------------------------------------------------------------------------- */

}

/* -------------------------------------------------------------------------- */
int WiFiSSLClient::connect(IPAddress ip, uint16_t port) {
/* -------------------------------------------------------------------------- */
   getSocket();
   string res = "";
   if(modem.write(string(PROMPT(_SSLCLIENTCONNECT)),res, "%s%d,%s,%d\r\n" , CMD_WRITE(_SSLCLIENTCONNECT), _sock, ip.toString(), port)) {
      return 1;
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
int WiFiSSLClient::connect(const char* host, uint16_t port) {
/* -------------------------------------------------------------------------- */
   getSocket();
   string res = "";
   if(modem.write(DO_NOT_CHECK_CMD,res, "%s%d,%s,%d\r\n" , CMD_WRITE(_SSLCLIENTCONNECT), _sock, host, port)) {
      return 1;
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
void WiFiSSLClient::setCACert(const char* root_ca) {
/* -------------------------------------------------------------------------- */
   getSocket();
   string res = "";
   modem.write(string(PROMPT(_SETCAROOT)),res, "%s%d,%s\r\n" , CMD_WRITE(_SETCAROOT), _sock,root_ca);
}

/* -------------------------------------------------------------------------- */
void WiFiSSLClient::setInsecure() {
/* -------------------------------------------------------------------------- */
   getSocket();
   string res = "";
   modem.write(DO_NOT_CHECK_CMD,res, "%s%d\r\n" , CMD_WRITE(_SSLSETINSERCURE), _sock);
   // if(modem.write(string(PROMPT(_SSLSETINSERCURE)),res, "%s%d\r\n" , CMD_WRITE(_SSLSETINSERCURE), _sock)) {
   //    return true;
   // }
   
}
