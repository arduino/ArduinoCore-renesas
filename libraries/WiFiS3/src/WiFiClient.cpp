#include "WiFiClient.h"

using namespace std;

/* -------------------------------------------------------------------------- */
WiFiClient::WiFiClient() : _sock(-1), rx_buffer(nullptr) {
   rx_buffer = std::shared_ptr<FifoBuffer<uint8_t,RX_BUFFER_DIM>>(new FifoBuffer<uint8_t,RX_BUFFER_DIM>());
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
WiFiClient::WiFiClient(int s) : _sock(s), rx_buffer(nullptr) {
   rx_buffer = std::shared_ptr<FifoBuffer<uint8_t,RX_BUFFER_DIM>>(new FifoBuffer<uint8_t,RX_BUFFER_DIM>());
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
WiFiClient::~WiFiClient() { }
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
WiFiClient::WiFiClient(const WiFiClient& c) {
/* -------------------------------------------------------------------------- */
   _sock = c._sock;
   rx_buffer = c.rx_buffer;
}

/* -------------------------------------------------------------------------- */
void WiFiClient::getSocket() {
/* -------------------------------------------------------------------------- */
   if(_sock >= 0 && !connected()) {
      // if sock >= 0 -> it means we were connected, but something happened and we need
      // to reset this socket in order to be able to connect again
      stop();
   }

   if(_sock == -1) {
      string res = "";
      modem.begin();
      if(modem.write(string(PROMPT(_BEGINCLIENT)),res, "%s" , CMD(_BEGINCLIENT))) {
         _sock = atoi(res.c_str());
      }
   }
}

/* -------------------------------------------------------------------------- */
int WiFiClient::connect(IPAddress ip, uint16_t port){
/* -------------------------------------------------------------------------- */
   return connect(ip.toString().c_str(), port);
}

/* -------------------------------------------------------------------------- */
int WiFiClient::connect(const char *host, uint16_t port){
/* -------------------------------------------------------------------------- */
   getSocket();
   if(_sock >= 0) {
      string res = "";
      modem.begin();
      if (_connectionTimeout) {
         if(modem.write(string(PROMPT(_CLIENTCONNECT)),res, "%s%d,%s,%d,%d\r\n" , CMD_WRITE(_CLIENTCONNECT), _sock, host,port, _connectionTimeout)) {
            return 1;
         }
      } else {
         if(modem.write(string(PROMPT(_CLIENTCONNECTNAME)),res, "%s%d,%s,%d\r\n" , CMD_WRITE(_CLIENTCONNECTNAME), _sock, host,port)) {
            return 1;
         }
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
int WiFiClient::available() {
/* -------------------------------------------------------------------------- */
   int rv = 0;
   if(_sock >= 0 && rx_buffer != nullptr) {
      if(rx_buffer->available() > 0) {
         return rx_buffer->available();
      }
      else {
         string res = "";
         modem.begin();
         if(modem.write(string(PROMPT(_AVAILABLE)),res, "%s%d\r\n" , CMD_WRITE(_AVAILABLE), _sock)) {
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
int WiFiClient::_read() {
/* -------------------------------------------------------------------------- */
   int rv = -1;
   if(_sock >= 0 && rx_buffer != nullptr) {
      string res = "";
      uint32_t size = rx_buffer->freePositions() - 1;
      modem.begin();

      /* important - it works one shot */
      modem.avoid_trim_results();
      modem.read_using_size();

      if(modem.write(string(PROMPT(_CLIENTRECEIVE)),res, "%s%d,%d\r\n" , CMD_WRITE(_CLIENTRECEIVE), _sock, size)) {
         if(res.size() > 0) {
            for(uint32_t i = 0, rv = 0; i < size && i < res.size(); i++) {
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
void WiFiClient::read_if_needed(size_t s) {
/* -------------------------------------------------------------------------- */
   if(rx_buffer != nullptr) {
      if((size_t)rx_buffer->available() < s) {
         _read();
      }
   }
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
   read_if_needed(size);
   int rv = 0;
   bool go_on = true;
   if(_sock >= 0 && rx_buffer != nullptr) {
      for(size_t i = 0; i < size && go_on; i++) {
         bool is_read = false;
         *(buf+i) = rx_buffer->read(&is_read);
         if(is_read) {
            rv++;
         }
         else {
            go_on = false;
         }
      }
   }
   return rv;
}

/* -------------------------------------------------------------------------- */
int WiFiClient::peek() {
/* -------------------------------------------------------------------------- */
   read_if_needed(1);
   if(_sock >= 0 && rx_buffer != nullptr) {
      return rx_buffer->peek();
   }
   return -1;
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

   rx_buffer->clear();
}

/* -------------------------------------------------------------------------- */
uint8_t WiFiClient::connected() {
/* -------------------------------------------------------------------------- */
   uint8_t rv = 0;
   if(this->available() > 0) {
      return 1;
   }
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
bool WiFiClient::operator==(const WiFiClient& whs) {
/* -------------------------------------------------------------------------- */
   return _sock == whs._sock;
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
