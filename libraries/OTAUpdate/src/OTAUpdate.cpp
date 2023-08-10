/*
  OTA.cpp
  Copyright (c) 2023 Arduino SA.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "OTAUpdate.h"

using namespace std;

OTAUpdate::OTAUpdate() {}

int OTAUpdate::update(const char* url) {
   string res = "";
   if(modem.write(string(PROMPT(_OTA_RUN)), res, "%s%s\r\n" , CMD_WRITE(_OTA_RUN), url)) {
     return 1;
   }
   return 0;
}

int OTAUpdate::setCACert(const char* root_ca) {
   string res = "";
   if(strlen(root_ca) > 0) {
      modem.write_nowait(string(PROMPT(_OTA_SETCAROOT)),res, "%s%d\r\n" , CMD_WRITE(_OTA_SETCAROOT), strlen(root_ca));
      if(modem.passthrough((uint8_t *)root_ca, strlen(root_ca))) {
         return 1;
      }
   }
   return 0;
}

bool OTAUpdate::isRunning() {
   string res = "";
   if(modem.write(string(PROMPT(_OTA_RUN)), res, CMD_READ(_OTA_RUN))) {
      return atoi(res.c_str());
   }
   return 0;
}

int OTAUpdate::getLastError() {
   string res = "";
   if(modem.write(string(PROMPT(_OTA_ERROR)), res, CMD_READ(_OTA_ERROR))) {
      return atoi(res.c_str());
   }
   return 0;
}
