#ifndef _MODEM_WIFIS3_H_
#define _MODEM_WIFIS3_H_

#include "Arduino.h"
#include "WiFiCommands.h"
#include "StringHelpers.h"


/* uncomment this will allow debug for passthrough "binary" commands */
#define MODEM_DEBUG_PASSTHROUGH

#define MODEM_TIMEOUT          10000
#define EXTENDED_MODEM_TIMEOUT 60000
#define MAX_BUFF_SIZE            128

#define DO_NOT_CHECK_CMD "NO_CMD_CHECK"

class ModemClass {

public:
  ModemClass(int tx, int rx);
  ModemClass(UART * _serial);
  ~ModemClass();

  void begin(int badurate = 115200);
  void end();
  bool write(const std::string &cmd, std::string &str, char * fmt, ...);
  void write_nowait(const std::string &cmd, std::string &str, char * fmt, ...);

  bool passthrough(const uint8_t *data, size_t size);
  void avoid_trim_results() {
    /* one shot - it works only 1 time the it is necessary to call again this
       funtion */
    trim_results = false;
  }

  void read_using_size() {
    read_by_size = true;
  } 
  bool beginned;

  /* calling this function with no argument will enable debug message to be printed
     on Serial
     use first parameter UART *u to redirect debug output to a different serial 

     level from 0 defaul to 2 (maximum) */

  void debug(Stream  &u, uint8_t level = 0) {
    _serial_debug = &u;
    
    if(level > 2) {
      level = 2;
    }
    _debug_level = level;
  }

  void noDebug() {
    _serial_debug = nullptr;
  }

  #ifdef SELECTABLE_MODEM_DEBUG
  bool enable_dbg = false;
  void debug(bool e) {enable_dbg = e;}
  #endif

  void timeout(size_t timeout_ms) {_timeout = timeout_ms;}

private:
  bool buf_read(const std::string &cmd, std::string &data_res);
  bool delete_serial;
  UART * _serial;
  unsigned long _timeout;
  uint8_t tx_buff[MAX_BUFF_SIZE];
  bool trim_results;
  bool read_by_size;
  bool read_by_size_finished(std::string &rx);
  Stream * _serial_debug;
  uint8_t _debug_level = 0;
};

extern ModemClass modem;

#endif
