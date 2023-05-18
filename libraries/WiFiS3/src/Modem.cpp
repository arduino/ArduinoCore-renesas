#include "Modem.h"

#define PROMPT_OK "OK\r\n"
#define PROMPT_ERROR "ERROR\r\n"

ModemClass::ModemClass(int tx, int rx) : beginned(false), delete_serial(false), _timeout(MODEM_TIMEOUT) {
  _serial = new UART(tx,rx);
}

ModemClass::ModemClass(UART * serial) : beginned(false) , delete_serial(true) , _serial(serial), _timeout(MODEM_TIMEOUT){
 
}

ModemClass::~ModemClass() {
  if(_serial != nullptr &&  !delete_serial){
      delete _serial;
      _serial = nullptr;
  }
}

void ModemClass::begin(int badurate){
  if(_serial != nullptr && !beginned) {
    _serial->begin(badurate);
    beginned = true;
  }
}

void ModemClass::end(){
 _serial->end();
}

bool ModemClass::write(const string &prompt, string &data_res, char * fmt, ...){
  memset(tx_buff,0x00,MAX_BUFF_SIZE);
  va_list va;
  va_start (va, fmt);
  vsprintf ((char *)tx_buff, fmt, va);
  va_end (va);
  #ifdef MODEM_DEBUG
    Serial.print("tx_buff: ");
    for(int i =0; i<MAX_BUFF_SIZE; i++) {
      Serial.print(tx_buff[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    Serial.write(tx_buff,strlen((char *)tx_buff));
    Serial.println("ciao");
  #endif
  _serial->write(tx_buff,strlen((char *)tx_buff));
  return buf_read(prompt,data_res);;
}


bool ModemClass::buf_read(const string &prompt, string &data_res) {
   bool res = false;
   unsigned long start_time = millis();
   while(millis() - start_time < _timeout){
      while(_serial->available()){
         char c = _serial->read();
         data_res += c;
         Serial.print(c);
         if(string::npos != data_res.rfind(PROMPT_OK)){
            data_res.substr(0, data_res.length() - sizeof(PROMPT_OK));
            if(prompt != DO_NOT_CHECK_CMD) {
               if(removeAtBegin(data_res, prompt)) {
                  res = true;
               }
            }
            res = true;
            break;
         } 
         else if (string::npos != data_res.rfind(PROMPT_ERROR)) {
            data_res.substr(0, data_res.length() - sizeof(PROMPT_ERROR));
            res = false;
            break;
         }
      }
    }
    #ifdef MODEM_DEBUG
      Serial.print("data_res: ");
      Serial.println(data_res.c_str());
    #endif
   return res;
}

#ifdef ARDUINO_SANTIAGO_COMPOSTA
  ModemClass modem = ModemClass(&Serial2);
#else
  ModemClass modem = ModemClass(D24,D25);
#endif
