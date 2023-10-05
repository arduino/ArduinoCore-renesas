/*
  SFU.h
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

#ifndef _SFU_H_
#define _SFU_H_

#include "Arduino.h"
#include "Client.h"

#define PORTENTA_C33_OTA_ERROR_BASE (-300)

enum class OTAError : int
{
  None                              = 0,
  DownloadFailed                    = 1,
  PORTENTA_C33_UrlParseError        = PORTENTA_C33_OTA_ERROR_BASE - 0,
  PORTENTA_C33_ServerConnectError   = PORTENTA_C33_OTA_ERROR_BASE - 1,
  PORTENTA_C33_HttpHeaderError      = PORTENTA_C33_OTA_ERROR_BASE - 2,
  PORTENTA_C33_HttpDataError        = PORTENTA_C33_OTA_ERROR_BASE - 3,
  PORTENTA_C33_ErrorOpenUpdateFile  = PORTENTA_C33_OTA_ERROR_BASE - 4,
  PORTENTA_C33_ErrorWriteUpdateFile = PORTENTA_C33_OTA_ERROR_BASE - 5,
  PORTENTA_C33_ErrorParseHttpHeader = PORTENTA_C33_OTA_ERROR_BASE - 6,
  PORTENTA_C33_ErrorFlashInit       = PORTENTA_C33_OTA_ERROR_BASE - 7,
  PORTENTA_C33_ErrorReformat        = PORTENTA_C33_OTA_ERROR_BASE - 8,
};

class SFU {
public:
	static int begin() {};
	static int download(Client& client, const char* ota_path, const char* ota_url);
	static int apply() { NVIC_SystemReset(); };
};

#endif // _SFU_H_
