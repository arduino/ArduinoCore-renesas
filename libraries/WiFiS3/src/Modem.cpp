#include "Modem.h"

#define RESULT_OK "OK\r\n"
#define RESULT_ERROR "ERROR\r\n"
#define RESULT_DATA "DATA\r\n"

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


bool ModemClass::passthrough(const uint8_t *data, size_t size) {
   Serial.println("[CALL] ModemClass::passthrough");
   _serial->write(data,size);
   bool res = false;
   bool found = false;
   string data_res = "";
   unsigned long start_time = millis();
   while(millis() - start_time < _timeout && !found){
      while(_serial->available()){
         char c = _serial->read();
         data_res += c;
         if(string::npos != data_res.rfind(RESULT_OK)){
            found = true;
            res = true;
            break;
         } 
         else if (string::npos != data_res.rfind(RESULT_ERROR)) {
            res = false;
            break;
         }
      }
   }   
   #ifdef MODEM_DEBUG
      Serial.print("  passthrough, rx |>>");
      Serial.print(data_res.c_str());
      Serial.println("<<|");
      if(res) {
         Serial.println("  Result: OK");
      }
      else {
         Serial.println("  Result: FAILED");
      }
    #endif
   return res;
}

void ModemClass::write_nowait(const string &cmd, string &str, char * fmt, ...) {
   
   memset(tx_buff,0x00,MAX_BUFF_SIZE);
   va_list va;
   va_start (va, fmt);
   vsprintf ((char *)tx_buff, fmt, va);
   va_end (va);
   #ifdef MODEM_DEBUG
    Serial.print("  Write Call no wait, command sent: ");
    Serial.write(tx_buff,strlen((char *)tx_buff));
    Serial.println();
   #endif
   _serial->write(tx_buff,strlen((char *)tx_buff));
   return;
}


bool ModemClass::write(const string &prompt, string &data_res, char * fmt, ...){
  
  data_res.clear();

  memset(tx_buff,0x00,MAX_BUFF_SIZE);
  va_list va;
  va_start (va, fmt);
  vsprintf ((char *)tx_buff, fmt, va);
  va_end (va);
  #ifdef MODEM_DEBUG
    Serial.print("  Write Call, command sent: ");
    Serial.write(tx_buff,strlen((char *)tx_buff));
    Serial.println();
  #endif
  _serial->write(tx_buff,strlen((char *)tx_buff));
  return buf_read(prompt,data_res);;
}


bool ModemClass::buf_read(const string &prompt, string &data_res) {
   bool res = false;
   bool found = false;
   unsigned long start_time = millis();
   while(millis() - start_time < _timeout && !found){
      while(_serial->available()){
         char c = _serial->read();
         data_res += c;
         if(string::npos != data_res.rfind(RESULT_DATA)) {
            found = true;
            data_res = data_res.substr(0, data_res.length() - sizeof(RESULT_DATA));
            if(prompt != DO_NOT_CHECK_CMD) {
               if(removeAtBegin(data_res, prompt)) {
                  Serial.println("DATA RECIVED");
                  res = true;
               }
            }
            else {
              res = true;
            }
            break;  
         }
         else if(string::npos != data_res.rfind(RESULT_OK)){
            found = true;
            data_res = data_res.substr(0, data_res.length() - sizeof(RESULT_OK));
            if(prompt != DO_NOT_CHECK_CMD) {
               if(removeAtBegin(data_res, prompt)) {
                  res = true;
               }
            }
            else {
              res = true;
            }
            break;
         } 
         else if (string::npos != data_res.rfind(RESULT_ERROR)) {
            data_res.substr(0, data_res.length() - sizeof(RESULT_ERROR));
            res = false;
            break;
         }
      }
    }
    trim(data_res);
    #ifdef MODEM_DEBUG
      Serial.print("  Write Call, response rx |>>");
      Serial.print(data_res.c_str());
      Serial.println("<<|");
      if(res) {
         Serial.println("  Result: OK");
      }
      else {
         Serial.println("  Result: FAILED");
      }
    #endif
      
   return res;
}

#ifdef ARDUINO_SANTIAGO_COMPOSTA
  ModemClass modem = ModemClass(&Serial2);
#else
  ModemClass modem = ModemClass(D24,D25);
#endif
