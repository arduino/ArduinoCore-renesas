#ifndef _MODEM_WIFIS3_H_
#define _MODEM_WIFIS3_H_

#include "Arduino.h"
#include "WiFiCommands.h"
#include "StringHelpers.h"


//#define MODEM_DEBUG
//#define MODEM_DEBUG_PASSTHROUGH
#define MODEM_TIMEOUT  10000
//#define SELECTABLE_MODEM_DEBUG
#define MAX_BUFF_SIZE  64

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

  #ifdef SELECTABLE_MODEM_DEBUG
  bool enable_dbg = false;
  void debug(bool e) {enable_dbg = e;}
  #endif

private:
  bool buf_read(const std::string &cmd, std::string &data_res);
  bool delete_serial;
  UART * _serial;
  unsigned long _timeout;
  uint8_t tx_buff[MAX_BUFF_SIZE];
  bool trim_results;
  bool read_by_size;
  bool read_by_size_finished(std::string &rx);
};

extern ModemClass modem;

#endif
