#include "WiFiFileSystem.h"

using namespace std;

/* -------------------------------------------------------------------------- */
WiFiFileSystem::WiFiFileSystem() {
/* -------------------------------------------------------------------------- */
}
void WiFiFileSystem::mount(bool format_on_fault) {
   modem.begin();
   string res = "";
   modem.write(string(PROMPT(_MOUNTFS)), res, "%s%d\r\n" , CMD_WRITE(_MOUNTFS), format_on_fault);
   if (format_on_fault) {
      modem.write(string(PROMPT(_MOUNTFS)), res, "%s%d\r\n" , CMD_WRITE(_MOUNTFS), format_on_fault);
   }
}
size_t WiFiFileSystem::writefile(const char* name, const char* data, size_t size, int operation) {
   string res = "";
   modem.write_nowait(string(PROMPT(_FILESYSTEM)), res, "%s%d,%d,%s,%d\r\n" , CMD_WRITE(_FILESYSTEM), 0, operation, name, size);
   if(modem.passthrough((uint8_t *)data, size)) {
      return size;
   }
   return 0;
}

/* -------------------------------------------------------------------------- */
void WiFiFileSystem::readfile(const char* name) {
/* -------------------------------------------------------------------------- */
   string res = "";
   int i = 0;
   while(1) {
      modem.avoid_trim_results();
      modem.read_using_size();
      if(modem.write(DO_NOT_CHECK_CMD, res, "%s%d,%d,%s,%d,%d\r\n" , CMD_WRITE(_FILESYSTEM), 0, WIFI_FILE_READ, name, i, 1023)) {
         Serial.print(res.c_str()); //WIP i'll fix this in next commit for now just print the filecontent
         i += res.size();
      } else {
         break;
      }
   }
}
