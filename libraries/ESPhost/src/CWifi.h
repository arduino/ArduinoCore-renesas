#ifndef C_ARDUINO_WIFI_H
#define C_ARDUINO_WIFI_H

class CWifi {
private:

public:
   CWifi() {}

   int begin(unsigned long timeout = 60000, unsigned long responseTimeout = 4000);


};




#endif