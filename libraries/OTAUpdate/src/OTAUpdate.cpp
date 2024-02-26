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

int OTAUpdate::setCACert(const char* root_ca) {
  string res = "";
  if ( root_ca != nullptr && strlen(root_ca) > 0) {
    modem.write_nowait(string(PROMPT(_OTA_SETCAROOT)), res, "%s%d\r\n", CMD_WRITE(_OTA_SETCAROOT), strlen(root_ca));
    if(modem.passthrough((uint8_t *)root_ca, strlen(root_ca))) {
      return static_cast<int>(Error::None);
    }
    return static_cast<int>(Error::Modem);
  }
  return static_cast<int>(Error::Library);
}

int OTAUpdate::begin() {
  string res = "";
  if (modem.write(string(PROMPT(_OTA_BEGIN)), res, "%s", CMD(_OTA_BEGIN))) {
    return atoi(res.c_str());
  }
  return static_cast<int>(Error::Modem);
}

int OTAUpdate::begin(const char* file_path) {
  string res = "";
  if ( file_path != nullptr && strlen(file_path) > 0) {
    if (modem.write(string(PROMPT(_OTA_BEGIN)), res, "%s%s\r\n", CMD_WRITE(_OTA_BEGIN), file_path)) {
      return atoi(res.c_str());
    }
    return static_cast<int>(Error::Modem);
  }
  return static_cast<int>(Error::Library);
}

int OTAUpdate::download(const char* url) {
  string res = "";
  int ret = -1;
  if ( url != nullptr && strlen(url) > 0) {
    modem.timeout(EXTENDED_MODEM_TIMEOUT);
    if(modem.write(string(PROMPT(_OTA_DOWNLOAD)), res, "%s%s\r\n", CMD_WRITE(_OTA_DOWNLOAD), url)) {
      ret = atoi(res.c_str());
    } else {
      ret = static_cast<int>(Error::Modem);
    }
  } else {
    ret = static_cast<int>(Error::Library);
  }
  modem.timeout(MODEM_TIMEOUT);
  return ret;
}

int OTAUpdate::download(const char* url, const char* file_path) {
  string res = "";
  int ret = -1;
  if ( url != nullptr && strlen(url) > 0 && file_path != nullptr && strlen(file_path) >0) {
    modem.timeout(EXTENDED_MODEM_TIMEOUT);
    if(modem.write(string(PROMPT(_OTA_DOWNLOAD)), res, "%s%s,%s\r\n", CMD_WRITE(_OTA_DOWNLOAD), url, file_path)) {
      ret = atoi(res.c_str());
    } else {
      ret = static_cast<int>(Error::Modem);
    }
  } else {
    ret = static_cast<int>(Error::Library);
  }
  modem.timeout(MODEM_TIMEOUT);
  return ret;
}

int OTAUpdate::verify() {
  string res = "";
  if (modem.write(string(PROMPT(_OTA_VERIFY)), res, "%s", CMD(_OTA_VERIFY))) {
    return atoi(res.c_str());
  }
  return static_cast<int>(Error::Modem);
}

int OTAUpdate::update() {
  string res = "";
  if (modem.write(string(PROMPT(_OTA_UPDATE)), res, "%s", CMD(_OTA_UPDATE))) {
    return atoi(res.c_str());
  }
  return static_cast<int>(Error::Modem);
}

int OTAUpdate::update(const char* file_path) {
  string res = "";
  if ( file_path != nullptr && strlen(file_path) > 0) {
    if (modem.write(string(PROMPT(_OTA_UPDATE)), res, "%s%s\r\n", CMD_WRITE(_OTA_UPDATE), file_path)) {
      return atoi(res.c_str());
    }
    return static_cast<int>(Error::Modem);
  }
  return static_cast<int>(Error::Library);
}

int OTAUpdate::reset() {
  string res = "";
  if (modem.write(string(PROMPT(_OTA_RESET)), res, "%s", CMD(_OTA_RESET))) {
    return static_cast<int>(Error::None);
  }
  return static_cast<int>(Error::Modem);
}
