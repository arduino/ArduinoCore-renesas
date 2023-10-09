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

#include "SFU.h"
#include <Arduino_DebugUtils.h>
#include <WiFiC3.h>
#include <WiFiSSLClient.h>
#include <Client.h>

#define AIOT_CONFIG_PORTENTA_C33_OTA_HTTP_HEADER_RECEIVE_TIMEOUT_ms (5*1000UL);
#define AIOT_CONFIG_PORTENTA_C33_OTA_HTTP_DATA_RECEIVE_TIMEOUT_ms (5*60*1000UL);

const unsigned char SFU[0x20000] __attribute__ ((section(".second_stage_ota"), used)) = {
	#include "c33.h"
};

/* Original code: http://stackoverflow.com/questions/2616011/easy-way-to-parse-a-url-in-c-cross-platform */
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>

struct URI {
  public:
    URI(const std::string& url_s) {
      this->parse(url_s);
    }
    std::string protocol_, host_, path_, query_;
  private:
    void parse(const std::string& url_s);
};

using namespace std;

// ctors, copy, equality, ...
// TODO: change me into something embedded friendly (this function adds ~100KB to flash)
void URI::parse(const string& url_s)
{
  const string prot_end("://");
  string::const_iterator prot_i = search(url_s.begin(), url_s.end(),
      prot_end.begin(), prot_end.end());
  protocol_.reserve(distance(url_s.begin(), prot_i));
  transform(url_s.begin(), prot_i,
      back_inserter(protocol_),
      ptr_fun<int,int>(tolower)); // protocol is icase
  if( prot_i == url_s.end() )
    return;
  advance(prot_i, prot_end.length());
  string::const_iterator path_i = find(prot_i, url_s.end(), '/');
  host_.reserve(distance(prot_i, path_i));
  transform(prot_i, path_i,
      back_inserter(host_),
      ptr_fun<int,int>(tolower)); // host is icase
  string::const_iterator query_i = find(path_i, url_s.end(), '?');
  path_.assign(path_i, query_i);
  if( query_i != url_s.end() )
    ++query_i;
  query_.assign(query_i, url_s.end());
}

int SFU::download(Client& client, const char* ota_path, const char* ota_url) {
  int err = -1;

  FILE * file = fopen(ota_path, "wb");
  if (!file)
  {
    DEBUG_ERROR("%s: fopen() failed", __FUNCTION__);
    fclose(file);
    return static_cast<int>(OTAError::PORTENTA_C33_ErrorOpenUpdateFile);
  }

  URI url(ota_url);
  int port = 0;

  if (url.protocol_ == "http") {
    port = 80;
  } else if (url.protocol_ == "https") {
    port = 443;
  } else {
    DEBUG_ERROR("%s: Failed to parse OTA URL %s", __FUNCTION__, url.host_.c_str());
    fclose(file);
    return static_cast<int>(OTAError::PORTENTA_C33_UrlParseError);
  }

  if (!client.connect(url.host_.c_str(), port))
  {
    DEBUG_ERROR("%s: Connection failure with OTA storage server %s", __FUNCTION__, url.host_.c_str());
    fclose(file);
    return static_cast<int>(OTAError::PORTENTA_C33_ServerConnectError);
  }

  client.println(String("GET ") + url.path_.c_str() + " HTTP/1.1");
  client.println(String("Host: ") + url.host_.c_str());
  client.println("Connection: close");
  client.println();

  /* Receive HTTP header. */
  String http_header;
  bool is_header_complete     = false,
       is_http_header_timeout = false;
  for (unsigned long const start = millis(); !is_header_complete;)
  {
    is_http_header_timeout = (millis() - start) > AIOT_CONFIG_PORTENTA_C33_OTA_HTTP_HEADER_RECEIVE_TIMEOUT_ms;
    if (is_http_header_timeout) break;

    if (client.available())
    {
      char const c = client.read();

      http_header += c;
      if (http_header.endsWith("\r\n\r\n"))
        is_header_complete = true;
    }
  }

  if (!is_header_complete)
  {
    DEBUG_ERROR("%s: Error receiving HTTP header %s", __FUNCTION__, is_http_header_timeout ? "(timeout)":"");
    fclose(file);
    return static_cast<int>(OTAError::PORTENTA_C33_HttpHeaderError);
  }

  /* Extract concent length from HTTP header. A typical entry looks like
   *   "Content-Length: 123456"
   */
  char const * content_length_ptr = strstr(http_header.c_str(), "Content-Length");
  if (!content_length_ptr)
  {
    DEBUG_ERROR("%s: Failure to extract content length from http header", __FUNCTION__);
    fclose(file);
    return static_cast<int>(OTAError::PORTENTA_C33_ErrorParseHttpHeader);
  }
  /* Find start of numerical value. */
  char * ptr = const_cast<char *>(content_length_ptr);
  for (; (*ptr != '\0') && !isDigit(*ptr); ptr++) { }
  /* Extract numerical value. */
  String content_length_str;
  for (; isDigit(*ptr); ptr++) content_length_str += *ptr;
  int const content_length_val = atoi(content_length_str.c_str());
  DEBUG_VERBOSE("%s: Length of OTA binary according to HTTP header = %d bytes", __FUNCTION__, content_length_val);

  /* Receive as many bytes as are indicated by the HTTP header - or die trying. */
  int  bytes_received = 0;
  bool is_http_data_timeout = false;
  for(unsigned long const start = millis(); bytes_received < content_length_val;)
  {
    is_http_data_timeout = (millis() - start) > AIOT_CONFIG_PORTENTA_C33_OTA_HTTP_DATA_RECEIVE_TIMEOUT_ms;
    if (is_http_data_timeout) break;

    if (client.available())
    {
      char const c = client.read();

      if (fwrite(&c, 1, sizeof(c), file) != sizeof(c))
      {
        DEBUG_ERROR("%s: Writing of firmware image to flash failed", __FUNCTION__);
        fclose(file);
        return static_cast<int>(OTAError::PORTENTA_C33_ErrorWriteUpdateFile);
      }

      bytes_received++;
    }
  }

  if (bytes_received != content_length_val) {
    DEBUG_ERROR("%s: Error receiving HTTP data %s (%d bytes received, %d expected)", __FUNCTION__, is_http_data_timeout ? "(timeout)":"", bytes_received, content_length_val);
    fclose(file);
    return static_cast<int>(OTAError::PORTENTA_C33_HttpDataError);
  }

  DEBUG_INFO("%s: %d bytes received", __FUNCTION__, ftell(file));
  fclose(file);

  return static_cast<int>(OTAError::None);
}
