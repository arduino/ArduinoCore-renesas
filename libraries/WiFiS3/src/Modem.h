#ifndef _MODEM_WIFIS3_H_
#define _MODEM_WIFIS3_H_

#include "Arduino.h"


#define MAX_BUFF_SIZE 64

class ModemClass {

public:
  ModemClass(int tx, int rx);
  ModemClass(UART * _serial);
  ~ModemClass();

  void begin(int badurate = 115200);
  void end();
  bool write(String &str, char * fmt, ...);

private:
  bool buf_read(String &data_res);
  bool delete_serial = false;
  UART * _serial;
  unsigned long _timeout = 10000;
  uint8_t tx_buff[MAX_BUFF_SIZE];
};

extern ModemClass modem;

#endif
