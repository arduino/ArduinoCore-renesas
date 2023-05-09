#include "Modem.h"

#define PROMPT_OK "OK\r\n"
#define PROMPT_ERROR "ERROR\r\n"
ModemClass::ModemClass(int tx, int rx){
  _serial = new UART(tx,rx);
}

ModemClass::ModemClass(UART * serial) {
  _serial = serial;
  delete_serial = true;
}

ModemClass::~ModemClass() {
  if(_serial != nullptr &&  !delete_serial){
      delete _serial;
      _serial = nullptr;
  }
}

void ModemClass::begin(int badurate){
  if(_serial != nullptr){
    _serial->begin(badurate);
  }
}

void ModemClass::end(){
 _serial->end();
}

bool ModemClass::write(String data_res, char * fmt, ...){
  memset(tx_buff,0x00,MAX_BUFF_SIZE);
  va_list va;
  va_start (va, fmt);
  vsprintf ((char *)tx_buff, fmt, va);
  va_end (va);
  _serial->write(tx_buff,strlen((char *)tx_buff));

  return buf_read(data_res);;
}

bool ModemClass::buf_read(String data_res) {
  bool res = 0;
  unsigned long start_time = millis();
  while(millis() - start_time < _timeout){
    if(_serial->available()){
      char c = _serial->read();
      data_res += c;
      if(res = data_res.endsWith(PROMPT_OK)){
        data_res.substring(0, data_res.length() - sizeof(PROMPT_OK));
        break;
      } else if (data_res.endsWith(PROMPT_ERROR)) {
        data_res.substring(0, data_res.length() - sizeof(PROMPT_ERROR));
        break;
      }
    }
  }
 return res;
}
#ifdef ARDUINO_SANTIAGO_COMPOSTA
  ModemClass modem = ModemClass(&Serial2);
#else
  ModemClass modem = ModemClass(D24,D25);
#endif
