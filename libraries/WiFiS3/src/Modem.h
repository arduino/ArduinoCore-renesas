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

/**
 * @brief A class that provides methods to interact with a modem.
 * 
 * This class is responsible for providing an interface to communicate with
 * a modem through serial communication. It includes methods for initialization, 
 * sending and receiving data, and handling modem configurations.
 */
class ModemClass {

public:
  /** 
   * @brief Constructor for the ModemClass, which initializes the modem with the specified transmit (TX) and receive (RX) pins.
   * 
   * @param Initializes an instance of the ModemClass class with
   * specific transmit `tx` and receive `rx` pins for communication.
   */
  ModemClass(int tx, int rx);

  /**
   * @brief Constructor for the ModemClass, which initializes the modem with the specified UART interface.
   *
   * @param `_serial` is a pointer to the UART object that will be used for communication with the modem.
   */
  ModemClass(UART * _serial);

  /**
   * @brief Destructor for ModemClass.
   */
  ~ModemClass();


  /**
   * @brief Initializes the modem communication with a specified baud rate.
   * 
   * @param[in] `badurate` sets the baud rate for the serial connection.
   */
  void begin(int badurate = 115200, int retry = 3);

  /** 
   * @brief Ends the modem communication.
   */
  void end();


  /**
   * @brief Sends a formatted command string to a device and stores the response.
   *
   * This function formats a command string using the provided format and arguments, 
   * sends it to a device, and waits for a response, which is stored in the `str` string.
   *
   * @param `cmd` A string representing the command to be sent to the device.
   * @param `str` A reference to a string that will hold the device's response.
   * @param `fmt` A format string for constructing the command.
   * 
   * @return `true` if the command was successfully sent and a response was received, 
   * `false` otherwise.
   */
  bool write(const std::string &cmd, std::string &str, const char * fmt, ...);

  /**
   * @brief Used to send a command to the modem without waiting for a response.
   * 
   * @param It takes a command string `cmd`, a string `str` where the response will be stored,
   * and a format string `fmt` along with additional arguments.
   */
  void write_nowait(const std::string &cmd, std::string &str, const char * fmt, ...);

  /**
   * @brief Sends binary data directly to the modem without any processing or interpretation.
   * 
   * @param It takes a pointer to the binary `data` and the `size` of the data as arguments.
   * Used for sending raw binary commands or data to the modem for operations that 
   * require direct communication without any additional formatting or parsing.
   */
  bool passthrough(const uint8_t *data, size_t size);

  /**
   * @brief Disables automatic trimming of results for one operation.
   * 
   * This function disables the automatic trimming of results for one operation. 
   * After it is called, the results will not be trimmed automatically until 
   * the function is called again.
   */
  void avoid_trim_results() {
    /* one shot - it works only 1 time the it is necessary to call again this
       funtion */
    trim_results = false;
  }

  /**
   * @brief Enables a specific mode of reading where the size of the data
   * to be read is considered for processing.
   */
  void read_using_size() {
    // read_by_size = true; // deprecated
  }
  
  bool beginned;

  /* Calling this function with no argument will enable debug message to be printed
     on Serial
     use first parameter UART *u to redirect debug output to a different serial

     level from 0 defaul to 2 (maximum) */

  void debug(Stream  &u, uint8_t level = 0) {
    _serial_debug = &u;

    if(level > 3) {
      level = 3;
    }
    _debug_level = level;
  }

  /**
   * @brief Used to disable debugging output for the modem communication.
   */
  void noDebug() {
    _serial_debug = nullptr;
  }

  #ifdef SELECTABLE_MODEM_DEBUG
  bool enable_dbg = false;
  void debug(bool e) {enable_dbg = e;}
  #endif

  /**
   * @brief Sets the timeout value for communication operations.
   * 
   * @param Can be called with a specified timeout value in milliseconds.
   */
  void timeout(size_t timeout_ms) {_timeout = timeout_ms;}

private:
  enum ParseResult {
    Ok,
    Error,
    ParseError,
    Timeout
  };

  ParseResult buf_read(const std::string &cmd, std::string &data_res);
  bool delete_serial;
  UART * _serial;
  unsigned long _timeout;
  uint8_t tx_buff[MAX_BUFF_SIZE];
  bool trim_results;
  Stream * _serial_debug;
  uint8_t _debug_level = 0;
};

extern ModemClass modem;

#endif
