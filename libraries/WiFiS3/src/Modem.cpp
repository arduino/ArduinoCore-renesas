#include "Modem.h"

#define RESULT_OK "OK\r\n"
#define RESULT_ERROR "ERROR\r\n"
#define RESULT_DATA "DATA\r\n"

using namespace std;

/* -------------------------------------------------------------------------- */
ModemClass::ModemClass(int tx, int rx) : beginned(false), delete_serial(false), _timeout(MODEM_TIMEOUT), trim_results(true), read_by_size(false) {
/* -------------------------------------------------------------------------- */
  _serial = new UART(tx,rx);
}

/* -------------------------------------------------------------------------- */
ModemClass::ModemClass(UART * serial) : beginned(false) , delete_serial(true) , _serial(serial), _timeout(MODEM_TIMEOUT), trim_results(true), read_by_size(false) {
/* -------------------------------------------------------------------------- */ 
}

/* -------------------------------------------------------------------------- */
ModemClass::~ModemClass() {
/* -------------------------------------------------------------------------- */   
  if(_serial != nullptr &&  !delete_serial){
      delete _serial;
      _serial = nullptr;
  }
}

/* -------------------------------------------------------------------------- */
void ModemClass::begin(int badurate){
/* -------------------------------------------------------------------------- */   
  if(_serial != nullptr && !beginned) {
      _serial->begin(badurate);
      beginned = true;
      string res = "";
      _serial->flush();
      modem.write(string(PROMPT(_SOFTRESETWIFI)),res, "%s" , CMD(_SOFTRESETWIFI));
  }
}

/* -------------------------------------------------------------------------- */
void ModemClass::end(){
/* -------------------------------------------------------------------------- */   
 _serial->end();
}

/* -------------------------------------------------------------------------- */
bool ModemClass::passthrough(const uint8_t *data, size_t size) {
/* -------------------------------------------------------------------------- */   
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
            found = true;
            res = false;
            break;
         }
      }
   }   
   #ifdef MODEM_DEBUG_PASSTHROUGH
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

/* -------------------------------------------------------------------------- */
void ModemClass::write_nowait(const string &cmd, string &str, char * fmt, ...) {
/* -------------------------------------------------------------------------- */   
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


/* -------------------------------------------------------------------------- */
bool ModemClass::write(const string &prompt, string &data_res, char * fmt, ...){
/* -------------------------------------------------------------------------- */  
  data_res.clear();
  memset(tx_buff,0x00,MAX_BUFF_SIZE);
  va_list va;
  va_start (va, fmt);
  vsprintf ((char *)tx_buff, fmt, va);
  va_end (va);
  #ifdef MODEM_DEBUG
    Serial.println();
    Serial.println("###>");
    Serial.print("READ BY SIZE: ");
    Serial.println((int)read_by_size);
    Serial.print("  Write Call, command sent: ");
    Serial.write(tx_buff,strlen((char *)tx_buff));
    Serial.println();

    Serial.println("<###");
  #endif
  _serial->write(tx_buff,strlen((char *)tx_buff));
  return buf_read(prompt,data_res);;
}


typedef enum {
   IDLE,
   WAIT_FOR_SIZE,
   WAIT_FOR_DATA
} ReadBySizeSt_t;


/* -------------------------------------------------------------------------- */
bool ModemClass::read_by_size_finished(string &rx) {
/* -------------------------------------------------------------------------- */   
   bool rv = false;
   static bool first_call = true;
   static ReadBySizeSt_t st = IDLE;
   static int data_to_be_received = 0;
   static int data_received = 0;
   if(first_call) {
      first_call = false;
      st = WAIT_FOR_SIZE;
   }

   switch(st) {
      case IDLE:
         
      break;
      case WAIT_FOR_SIZE: {
         int pos = rx.find("|");
         int pos_space = rx.find(" ");
         if(pos != string::npos && pos_space != string::npos) {
            string n = rx.substr(pos_space,pos);
            /* add 4 because OK\r\n is always added at the end of data */
            data_to_be_received = atoi(n.c_str()) + 4;
            rx.clear();
            data_received = 0;
            st = WAIT_FOR_DATA; 
            
         }
      }
      break;

      case WAIT_FOR_DATA:
         data_received++;
         if(data_received == data_to_be_received) {
            rv = true;
            first_call = true;
            st = IDLE;
         }
      break;

      default:
         st = IDLE;
      break;
   }
   return rv;
}



/* -------------------------------------------------------------------------- */
bool ModemClass::buf_read(const string &prompt, string &data_res) {
/* -------------------------------------------------------------------------- */   
   bool res = false;
   bool found = false;
   
   unsigned long start_time = millis();
   while((millis() - start_time < _timeout) && !found){
      while( _serial->available() ){
         char c = _serial->read();
         data_res += c;
         #ifdef SELECTABLE_MODEM_DEBUG
         if(enable_dbg) {
            Serial.print(c);
         }
         #endif
            
         if(read_by_size) {
            if(read_by_size_finished(data_res)) {
               found = true;
               read_by_size = false;
               res = true;
               data_res = data_res.substr(0, data_res.length() - (sizeof(RESULT_OK) - 1));
            }
         }
         else {
            if(string::npos != data_res.rfind(RESULT_DATA)) {
               found = true;
               data_res = data_res.substr(0, data_res.length() - (sizeof(RESULT_DATA) - 1));
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
            else if(string::npos != data_res.rfind(RESULT_OK)){
               found = true;
               data_res = data_res.substr(0, data_res.length() - (sizeof(RESULT_OK) - 1) );
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
               data_res.substr(0, data_res.length() - (sizeof(RESULT_ERROR) - 1));
               res = false;
               break;
            }
         }
      }
   }
   if(trim_results) {
      trim(data_res);
   }
   trim_results = true;
   read_by_size = false;
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

#ifdef ARDUINO_UNOWIFIR4
  ModemClass modem = ModemClass(&Serial2);
#else
  ModemClass modem = ModemClass(D24,D25);
#endif
