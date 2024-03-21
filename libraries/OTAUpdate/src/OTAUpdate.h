/*
  OTA.h
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

#ifndef OTA_UPDATE_H
#define OTA_UPDATE_H

#include "Arduino.h"
#include "WiFiCommands.h"
#include "Modem.h"

class OTAUpdate {

public:

  enum class Error: int {
    None      =  0,
    Library   = -25,
    Modem     = -26,
  };

  constexpr static const int OTA_ERROR_NONE = static_cast<int>(Error::None);

  OTAUpdate();
  int setCACert(const char* root_ca);
  int begin();
  int begin(const char* file_path);
  int download(const char* url);
  int download(const char* url, const char* file_path);
  int verify();
  int update();
  int update(const char* file_path);
  int reset();

};

#endif /* OTA_UPDATE_H */
