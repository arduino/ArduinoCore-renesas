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
  /** 
   * Initializes an instance of the `ModemClass` class with
   * specific transmit (tx) and receive (rx) pins for communication.
   */
  ModemClass(int tx, int rx);
  ModemClass(UART * _serial);
  ~ModemClass();

  /**
   * Initializes the modem communication with a specified baud rate. By default,
   * the baud rate is set to 115200. Call function after creating an instance of the 
   * `ModemClass` to set up the communication parameters before sending or receiving data.
   */
  void begin(int badurate = 115200);

  /** 
   * Shutts down the modem communication and releases any resources that were allocated during the 
   * communication process.
   */
  void end();

  /**
   * Sends a command to the modem and waits for a response.
   * It takes a command string `cmd`, a string `str` where the response will be stored 
   * and a format string `fmt` along with additional arguments.
   */
  bool write(const std::string &cmd, std::string &str, const char * fmt, ...);

  /**
   * Used to send a command to the modem without waiting for a response.
   * It takes a command string `cmd`, a string `str` where the response will be stored,
   * and a format string `fmt` along with additional arguments.
   */
  void write_nowait(const std::string &cmd, std::string &str, const char * fmt, ...);

  /**
   * Sends binary data directly to the modem without any processing or interpretation.
   * It takes a pointer to the binary `data` and the `size` of the data as arguments.
   * Used for sending raw binary commands or data to the modem for operations that 
   * require direct communication without any additional formatting or parsing.
   */
  bool passthrough(const uint8_t *data, size_t size);

  void avoid_trim_results() {
    /* one shot - it works only 1 time the it is necessary to call again this
       funtion */
    trim_results = false;
  }


  /**
   * When this function is called, it enables a specific mode of reading 
   * where the size of the data to be read is considered for processing.
   */
  void read_using_size() {
    read_by_size = true;
  } 
  bool beginned;

  /* Calling this function with no argument will enable debug message to be printed
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

  /**
   * Used to disable debugging output for the modem communication.
   */
  void noDebug() {
    _serial_debug = nullptr;
  }

  /*NOT SURE*/
  #ifdef SELECTABLE_MODEM_DEBUG
  bool enable_dbg = false;
  void debug(bool e) {enable_dbg = e;}
  #endif

  /**
   * Sets the timeout value for communication operations.
   * Can be called with a specified timeout value in milliseconds.
   */
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
