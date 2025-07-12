#include "Modem.h"

#define OK_TOKEN        "OK"
#define ERROR_TOKEN     "ERROR"
#define TERM_TOKEN      "\r\n"
#define RESULT_OK       OK_TOKEN TERM_TOKEN
#define RESULT_ERROR    OK_TOKEN TERM_TOKEN
#define RESULT_DATA     "DATA" TERM_TOKEN

using namespace std;

/* -------------------------------------------------------------------------- */
ModemClass::ModemClass(int tx, int rx) : beginned(false), delete_serial(false), _timeout(MODEM_TIMEOUT), trim_results(true) {
/* -------------------------------------------------------------------------- */
   _serial = new UART(tx,rx);
}

/* -------------------------------------------------------------------------- */
ModemClass::ModemClass(UART * serial) : beginned(false) , delete_serial(true) , _serial(serial), _timeout(MODEM_TIMEOUT), trim_results(true) {
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
void ModemClass::begin(int badurate, int retry){
/* -------------------------------------------------------------------------- */
   if(_serial != nullptr && !beginned) {
      _serial->begin(badurate);
      string res = "";
      _serial->flush();
      modem.timeout(500);
      while(!beginned && retry > 0) {
         beginned = modem.write(string(PROMPT(_SOFTRESETWIFI)),res, "%s" , CMD(_SOFTRESETWIFI));
         retry -= 1;
      }
      modem.timeout(MODEM_TIMEOUT);
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

   std::string prompt = DO_NOT_CHECK_CMD, data_res;
   auto res = buf_read(prompt, data_res);

   if(_serial_debug && _debug_level >= 2) {
      _serial_debug->print("   ANSWER (passthrough): ");
      _serial_debug->println(data_res.c_str());
      if(res == Ok) {
         _serial_debug->println("   Result: OK");
      } else if(res == Error) {
         _serial_debug->println("   Result: ERROR");
      } else if(res == Timeout) {
         _serial_debug->println("   Result: TIMEOUT");
      } else {
         _serial_debug->println("   Result: ParseError");
      }
   }

   return res == Ok;
}

/* -------------------------------------------------------------------------- */
void ModemClass::write_nowait(const string &cmd, string &str, const char * fmt, ...) {
/* -------------------------------------------------------------------------- */
   (void)cmd;
   (void)str;

   va_list va;
   va_start (va, fmt);
   vsnprintf((char *)tx_buff, MAX_BUFF_SIZE, fmt, va);
   va_end (va);

   if(_serial_debug && _debug_level >= 2) {
      _serial_debug->print("REQUEST (passthrough): ");
      _serial_debug->write(tx_buff,strlen((char *)tx_buff));
      _serial_debug->println();
   }

   _serial->write(tx_buff,strlen((char *)tx_buff));
   return;
}


/* -------------------------------------------------------------------------- */
bool ModemClass::write(const string &prompt, string &data_res, const char * fmt, ...){
/* -------------------------------------------------------------------------- */
   data_res.clear();
   va_list va;
   va_start (va, fmt);
   vsnprintf((char *)tx_buff, MAX_BUFF_SIZE, fmt, va);
   va_end (va);

   if(_serial_debug) {
      _serial_debug->println();
      _serial_debug->print("REQUEST: ");
      _serial_debug->write(tx_buff,strlen((char *)tx_buff));
      _serial_debug->println();
   }

   _serial->write(tx_buff,strlen((char *)tx_buff));
   auto res = buf_read(prompt, data_res);

   if(_serial_debug) {
      _serial_debug->print("   ANSWER: ");
      _serial_debug->println(data_res.c_str());
      if(res == Ok) {
         _serial_debug->println("   Result: OK");
      } else if(res == Error) {
         _serial_debug->println("   Result: ERROR");
      } else if(res == Timeout) {
         _serial_debug->println("   Result: TIMEOUT");
      } else {
         _serial_debug->println("   Result: ParseError");
      }
   }

   return res == Ok;
}


/* -------------------------------------------------------------------------- */
ModemClass::ParseResult ModemClass::buf_read(const string &prompt, string &data_res) {
/* -------------------------------------------------------------------------- */
   /*
    * This function implements as FSM that parses basic AT command responses
    * The expected syntax should match the following regex
    * - (?:\+(\w+)[:=][ ]?(\w*))?(?:\r\n)?(ERROR\r\n|OK\r\n)
    *   + "ERROR<CR><LF>" "OK<CR><LF>"
    *   + "+COMMAND: <CR><LF>OK<CR><LF>"
    *   + "+COMMAND: <CR><LF>ERROR<CR><LF>"
    *   + "+COMMAND: 1231231<CR><LF>OK<CR><LF>" (NOTE only one parameter supported)
    *   + "+COMMAND: 1231231<CR><LF>ERROR<CR><LF>" (NOTE only one parameter supported)
    * - custom sized response:
    *   + "+COMMAND: 4| 123OK<CR><LF>"
    */
   enum class at_parse_state_t {
      Begin       = 0,
      Cmd         = 1,
      Data        = 2,
      Sized       = 3,
      ResWaitLF   = 4,
      Res         = 5,
      Error       = 6,
      ParseError  = 7,
      Ok          = 8,
      Completed   = 9,
   };

   at_parse_state_t state = at_parse_state_t::Begin;
   std::string commandName;

   ModemClass::ParseResult res = Error;
   unsigned int sized_read_size = 0;
   unsigned int sized_read_count = 0;
   unsigned int result_parse = 0;
   bool restart = false,
        consume_char = true; // This flag is used to indicate to consume another character from the stream

   char c;

   // I expect the answer to be in this form: "ERROR<CR><LF>" "OK<CR><LF>"
   // if prompt == DO_NOT_CHECK_CMD
   const bool check_prompt = (prompt != DO_NOT_CHECK_CMD);

   if(_serial_debug  && _debug_level >= 1) {
      _serial_debug->print("RAW: ");
   }

   unsigned long start_time = millis();
   while(state != at_parse_state_t::Completed) {

      if(millis() - start_time > _timeout) {
         res = Timeout;
         break;
      }

      if(consume_char && !_serial->available()) {
         // if there is nothing available, go to the beginning of the cycle
         continue;
      } else if(consume_char) { // available is true
         c = _serial->read();
      } else if(!consume_char) {
         // reset consume_char to true
         consume_char = true;
      }

      if(_serial_debug  && _debug_level >= 1 && consume_char) {
         if(c == '\n') {
            _serial_debug->print("<LF>");
         } else if (c == '\r') {
            _serial_debug->print("<CR>");
         } else if (c == ' ') {
            _serial_debug->print("<SP>");
         } else if(c < ' ') {
            _serial_debug->print("<");
            _serial_debug->print((unsigned int)c);
            _serial_debug->print(">");
         } else {
            _serial_debug->print(c);
         }
      }
      if(_serial_debug  && _debug_level >= 3) {
         _serial_debug->print(" State ");
         _serial_debug->println((int)state);
      }

      switch(state) {
      case at_parse_state_t::Begin:
         /*
          * In this state we wait for a '+' character, which will mark the beginning of a response
          * or the status response code "ERROR<CR><LF>" or "OK<CR><LF>"
          * we need to consume the available buffer if it doesn't match the expected response,
          * in order to avoiding dealing with previous responses which were not parsed successfully
          */

         if(c == '+') {
            // This line allow to strenghten the checks on the response prompt,
            // it is required that passthrough() function is able to know the command prompt,
            // provided in the previous call of write_nowait()
            // the answer doesn't match the expected form, thus the response received
            // is not related to the sent command we need to restart
            // restart = !check_prompt;

            commandName += c; // prompt includes also '+'
            state = at_parse_state_t::Cmd;
         } else if(c == RESULT_OK[result_parse]) {
            // the answer doesn't match the expected form, we need to restart
            restart = check_prompt;

            state = at_parse_state_t::Ok;
            result_parse++;
         } else if(c == RESULT_ERROR[result_parse]) {
            // the answer doesn't match the expected form, we need to restart
            restart = check_prompt;

            state = at_parse_state_t::Error;
            result_parse++;
         }
         // if we uncomment this we can force strict response matching
         // else {
         //    state = at_parse_state_t::ParseError;
         // }

         break;
      case at_parse_state_t::Cmd:
         /*
          * In this state we parse the command prompt and wait for either ':' or '=' characters
          * in order to go the next state
          */

         if(c == ':' || c == '=') {
            commandName += c; // prompt includes also ':'

            if (check_prompt && commandName != prompt) {
               // the response we got is not the one we were expecting, parse the wrong response till the end
               // and start the parse of the next response
               restart = true;
               commandName = "";
            }
            state = at_parse_state_t::Data;

            data_res = "";
            // state = at_parse_state_t::Data;
         } else { // no space should be present in the prompt response
            commandName += c;
         }

         break;
      case at_parse_state_t::Data:
         /*
          * In this state we parse response parameters and push them into data_res
          * in case multiple parameters separated by ',' are sent, they will be present in data_res
          * - if we encounter <CR> we need to wait for <LF>
          * - if we encounter <LF> we need to parse the response status
          * - if we encounter '|', the next token will contain binary sized data, the current value
          *   in data_res contains the length of the next token
          */

         if(c == '|') { // sized read, the previous parameter is the length
            sized_read_size = atoi(data_res.c_str());
            data_res.clear();
            if (sized_read_size != 0) {
               state = at_parse_state_t::Sized;
            } else {
               state = at_parse_state_t::Res;
            }
         } else if(c == '\r') {
            state = at_parse_state_t::ResWaitLF;
         } else if(c == '\n') {
            state = at_parse_state_t::Res;
         } else if(trim_results && c != ' ') {
            data_res += c; // in case trim_result is true, avoid adding spaces
         } else if(!trim_results) {
            data_res += c;
         }

         break;
      case at_parse_state_t::Sized:
         /*
          * In this state we collect exactly sized_read_size characters into data_res
          * when we consume all of them we go into Result parse state, where we supposedly
          * wait for 'OK'
          */
         data_res += c;

         if(++sized_read_count == sized_read_size) {
            state = at_parse_state_t::Res;
         }
         break;
      case at_parse_state_t::ResWaitLF:
         if(c == '\n') {
            state = at_parse_state_t::Res;
         }
         /* fall through */

         /*
          * break is volountary not present, to cover for cases where the response status is in the
          * following form: '...<CR>OK<CR><LF>' '<CR>ERROR<CR><LF>'
          */
      case at_parse_state_t::Res:
         /*
          * In this state we wait for either an 'O' or an 'E', in order to get an 'OK<CR><LF>'
          * or 'ERROR<CR><LF>'
          * The first two cases is when there is no parameter in the response, but just the OK and ERROR tokens
          */

         if(data_res == OK_TOKEN) {
            res = Ok;
            state = at_parse_state_t::Completed;
         } else if(data_res == ERROR_TOKEN) {
            res = Error;
            state = at_parse_state_t::Completed;
         } if(c == RESULT_OK[0]) { // OK response
            state = at_parse_state_t::Ok;
            result_parse = 1;
         } else if(c == RESULT_ERROR[0]) { // Error response
            state = at_parse_state_t::Error;
            result_parse = 1;
         }
         // if we uncomment this we can force strict response matching
         // else {
         //    state = at_parse_state_t::ParseError;
         // }
         break;
      case at_parse_state_t::Ok:
         /*
          * In this state we want to match the exact 'K<CR><LF>' response
          */
         if(c != RESULT_OK[result_parse++]) {
            state = at_parse_state_t::ParseError;
         }

         if(result_parse == strlen(RESULT_OK)) {
            res = Ok;
            state = at_parse_state_t::Completed;
         }
         break;
      case at_parse_state_t::Error:
         /*
          * In this state we want to match the exact 'RROR<CR><LF>' response
          */

         if(c != RESULT_ERROR[result_parse++]) {
            state = at_parse_state_t::ParseError;
         }

         if(result_parse == strlen(RESULT_ERROR)) {
            res = Error;
            state = at_parse_state_t::Completed;
         }
         break;
      case at_parse_state_t::ParseError:
         res = ParseError;
         // if we get a parseError, we go back from the beginning and try again to parse, unitl the timeout expires
         state = at_parse_state_t::Begin;
         restart = false;
         consume_char = false;
         break;
      case at_parse_state_t::Completed:
         break;
      }

      if(restart && state == at_parse_state_t::Completed) {
         state = at_parse_state_t::Begin;
         restart = false;
      }
   }

   if(_serial_debug  && _debug_level >= 3) {
      _serial_debug->print("Final State ");
      _serial_debug->print((int)state);
      _serial_debug->print(" res ");
      _serial_debug->println((int)res);
   }

   trim_results = true;

   if(_serial_debug && _debug_level >= 1) {
      _serial_debug->print("<-RAW END");
      _serial_debug->println();
   }

   return res;
}

#ifdef ARDUINO_UNOWIFIR4
   ModemClass modem = ModemClass(&Serial2);
#else
   ModemClass modem = ModemClass(D24,D25);
#endif
