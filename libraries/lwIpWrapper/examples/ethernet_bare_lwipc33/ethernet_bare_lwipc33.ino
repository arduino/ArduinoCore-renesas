#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include <IRQManager.h>
#include <regex>
#include <utils.h>

// #define CNETIF_STATS_ENABLED
// #include "CNetifStats.h"

#ifdef CNETIF_STATS_ENABLED
#define STATS_BUFFER_SIZE 1000
char cnetif_stats_buffer[STATS_BUFFER_SIZE];
// netif_stats _stats;
#endif // CNETIF_STATS_ENABLED

#include <EthernetC33.h>
#include <lwipClient.h>

#define CHECK_PAYLOAD

/* --------------------------------------- */
void timer_cb(timer_callback_args_t *arg);
void application();
void dump_buffer(uint8_t* b, uint32_t len, uint8_t blocks=4, uint8_t cols=16);
void dump_buffer_char(uint8_t* b, uint32_t len);
void application_report(bool force=false);
bool verify_buffer_sequential_faster_4B(uint8_t *buffer, size_t len, uint32_t& offset, uint8_t *excess, uint8_t &excess_len, bool print= false);
bool verify_buffer_sequential_4B(uint8_t *buffer, size_t len, uint32_t& offset, uint8_t *excess, uint8_t &excess_len, bool print=false);
void application_final_report();
uint64_t debug_start;
/* --------------------------------------- */

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Renesas file download example");

  IPAddress ip(192, 168, 10, 130);
  IPAddress gw(192, 168, 10, 1);
  IPAddress nm(255, 255, 255, 0);

  DEBUG_INFO("Setting up netif");
  Ethernet.begin(ip, nm, gw);
  // Ethernet.begin();

  DEBUG_INFO("Begin of reception\n\n");
  debug_start = millis();
}

uint32_t counter=0;
void loop() {
  // __disable_irq();
  uint32_t start = micros();
#ifndef LWIP_USE_TIMER
  CLwipIf::getInstance().task();
#endif
  // Handle application FSM
  application();

  if(millis() - debug_start > 3000) { // print the debug _stats every x second
    // DEBUG_INFO("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    DEBUG_INFO("time: %12ums", millis());
    // DEBUG_INFO("memory: %12u bytes \tmin: %12u bytes \tmax: %12u bytes",
    //   memory_used, memory_used_min, memory_used_max);
    DEBUG_INFO("loop counter %u\n", counter);
    application_report();

#ifdef CNETIF_STATS_ENABLED
    netif_stats_sprintf(cnetif_stats_buffer, Ethernet.stats, STATS_BUFFER_SIZE, (8*1e6)/(1<<20), "Mbit/s");
    // __disable_irq();
    arduino::lock();
    NETIF_STATS_RESET_AVERAGES(Ethernet.stats);
    // __enable_irq();
    arduino::unlock();

    DEBUG_INFO(cnetif_stats_buffer);
#endif // CNETIF_STATS_ENABLED
    // DEBUG_INFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");

    counter = 0;
    // reset some counters
    debug_start = millis();
  }
  counter++;
}

// Application level Stuff
enum app_state_t: uint8_t {
  APP_STATE_NONE = 0,
  APP_STATE_LINK_UP,
  APP_STATE_LINK_DOWN,
  APP_STATE_IFACE_UP,
  APP_STATE_CONNECTING,
  APP_STATE_CONNECTED,
  APP_STATE_PARSE_HEADER,
  APP_STATE_DOWNLOAD,
  APP_STATE_DOWNLOAD_FAILED,
  APP_STATE_DOWNLOAD_FINISHED,
  APP_STATE_ERROR,
  APP_STATE_RESET
};

static const char* state_strings[] = {
  "APP_STATE_NONE",
  "APP_STATE_LINK_UP",
  "APP_STATE_LINK_DOWN",
  "APP_STATE_IFACE_UP",
  "APP_STATE_CONNECTING",
  "APP_STATE_CONNECTED",
  "APP_STATE_PARSE_HEADER",
  "APP_STATE_DOWNLOAD",
  "APP_STATE_DOWNLOAD_FAILED",
  "APP_STATE_DOWNLOAD_FINISHED",
  "APP_STATE_ERROR",
  "APP_STATE_RESET"
};

#define APP_BUFFER_SIZE 1*1024


struct App {
  app_state_t current_state=APP_STATE_NONE;
  app_state_t prev_state=APP_STATE_NONE;

  lwipClient *tcp_client;
  uint16_t port = 8000;
  IPAddress server_ip = IPAddress(192, 168, 10, 250);

  uint8_t buffer[APP_BUFFER_SIZE];

  size_t file_length=0;
  size_t downloaded_bytes=0;
  std::string http_header;

  // stats related variables
  uint32_t start = 0;
  uint32_t speed_start = 0;
  uint32_t speed_bytes = 0;

  // payload verification parameters
  uint32_t payload_verify_offset=0;
  uint8_t payload_verify_excess[4]={}; // this should be 3, but there are bugs
  uint8_t payload_verify_excess_len=0;
  uint32_t last_value=0;
} app;

void init_app(struct App& app) {
  app.file_length = 0;
  app.http_header = "";
  app.downloaded_bytes = 0;
  app.start = 0;
  app.payload_verify_excess_len = 0;
  app.payload_verify_offset = 0;
  app.last_value=0;
  app.speed_bytes = 0;
}

void reset_app(struct App& app) {
  init_app(app);

  if(app.tcp_client != nullptr) {
    app.tcp_client->stop();
    // delete app.tcp_client;
  }
}

const char* http_request = "GET /test-4M HTTP/1.1\nHost: 192.168.10.250\nConnection: close\n\n";

void application() {
  bool found = false;
  uint16_t bytes_read=0;

  switch(app.current_state) {
  case APP_STATE_NONE:
    init_app(app);

    // TODO we are not handling link connection and disconnection
    app.prev_state = app.current_state;
    app.current_state = APP_STATE_LINK_UP;
    DEBUG_INFO("State changed: to %s, from %s",
      state_strings[app.current_state],
      state_strings[app.prev_state]);
    break;

  case APP_STATE_LINK_UP:
    if(Ethernet.isDhcpAcquired()) {
      app.prev_state = app.current_state;
      app.current_state = APP_STATE_IFACE_UP;
      DEBUG_INFO("State changed: to %s, from %s",
        state_strings[app.current_state],
        state_strings[app.prev_state]);
    }
    break;
  case APP_STATE_IFACE_UP:
    // The link is up we connect to the server
    app.tcp_client = new lwipClient;

    // Connection details:
    app.tcp_client->connect(app.server_ip, app.port);

    app.prev_state = app.current_state;
    app.current_state = APP_STATE_CONNECTING;
    DEBUG_INFO("State changed: to %s, from %s",
      state_strings[app.current_state],
      state_strings[app.prev_state]);
    break;

  case APP_STATE_CONNECTING:
    // do nothing, until the TCP connection is established
    // TODO handle timeout for connection and go to error state
    if(app.tcp_client->connected()) {
      app.prev_state = app.current_state;
      app.current_state = APP_STATE_CONNECTED;
      DEBUG_INFO("State changed: to %s, from %s",
        state_strings[app.current_state],
        state_strings[app.prev_state]);
    }

    break;

  case APP_STATE_CONNECTED:
    app.tcp_client->write((uint8_t*)http_request, strlen(http_request));
    app.start = millis();
    app.speed_start = app.start;

    app.prev_state = app.current_state;
    app.current_state = APP_STATE_PARSE_HEADER;
    DEBUG_INFO("State changed: to %s, from %s",
      state_strings[app.current_state],
      state_strings[app.prev_state]);
    break;

  case APP_STATE_PARSE_HEADER:
    // FIXME
    bytes_read = app.tcp_client->read_until_token(app.buffer, APP_BUFFER_SIZE, "\r\n\r\n", found);
    // DEBUG_INFO("%s", app.http_header.c_str());

    if(bytes_read>0) {
      // put the buffer into an http header string
      std::string chunk((char*)app.buffer, bytes_read);
      app.http_header += chunk;
      app.speed_bytes += bytes_read;
      DEBUG_INFO("%s", app.http_header.c_str());
    }

    if(found) { // FIXME reduce indentation level
      // we found the http terminating token, go to the next app phase if we extracted the file len
      // otherwise go in error phase

      // Parse the http header and gather information needed for the download
      // dump_buffer_char(app.buffer, APP_BUFFER_SIZE);

      std::regex content_length_regex("Content-Length: ([0-9]+)", std::regex::icase);
      std::smatch matches;

      // DEBUG_INFO(app.http_header.c_str());

      if(std::regex_search(app.http_header, matches, content_length_regex)) {
        app.file_length = stoi(matches[1].str());

        DEBUG_INFO("Download started, file length: %u", app.file_length);

        app.prev_state = app.current_state;
        app.current_state = APP_STATE_DOWNLOAD;
        DEBUG_INFO("State changed: to %s, from %s",
          state_strings[app.current_state],
          state_strings[app.prev_state]);
      } else {
        // Failed to extract the content length from the header, going into an error state
        // TODO report the reason of the error

        app.prev_state = app.current_state;
        app.current_state = APP_STATE_ERROR;
        DEBUG_INFO("State changed: to %s, from %s",
          state_strings[app.current_state],
          state_strings[app.prev_state]);
      }
    }
    break;
  case APP_STATE_DOWNLOAD:
    if(app.tcp_client->available() <= 0) { // no data available
      break;
    }

    bytes_read = app.tcp_client->read(app.buffer, APP_BUFFER_SIZE);
    // DEBUG_INFO("read %6u, available %6u", bytes_read, app.tcp_client->available());

    if(bytes_read > 0) {
      app.downloaded_bytes += bytes_read;
      app.speed_bytes += bytes_read;

      // dump_buffer(app.buffer, APP_BUFFER_SIZE, 4, 128);
#ifdef CHECK_PAYLOAD
      // if(!verify_buffer_sequential_4B(
      if(!verify_buffer_sequential_faster_4B(
        app.buffer,
        bytes_read,
        app.payload_verify_offset,
        app.payload_verify_excess,
        app.payload_verify_excess_len,
        false)) {

        DEBUG_INFO("Payload verification failed");
        app.prev_state = app.current_state;
        app.current_state = APP_STATE_DOWNLOAD_FAILED;
        DEBUG_INFO("State changed: to %s, from %s",
          state_strings[app.current_state],
          state_strings[app.prev_state]);
      }
#endif // CHECK_PAYLOAD
    }

    // test for faking download failed
    // if(app.downloaded_bytes > app.file_length/6) {
    //     app.prev_state = app.current_state;
    //     app.current_state = APP_STATE_DOWNLOAD_FAILED;
    // }

    if(app.downloaded_bytes == app.file_length) {
      app.last_value =
        *(app.buffer + bytes_read - 4) << 24 |
        *(app.buffer + bytes_read - 3) << 16 |
        *(app.buffer + bytes_read - 2) << 8  |
        *(app.buffer + bytes_read - 1);

      // if the download of the counter file is correct the last value should be
      // the size of the file/4 -1
      if(app.last_value == (app.downloaded_bytes/4 - 1)) {
        app.prev_state = app.current_state;
        app.current_state = APP_STATE_DOWNLOAD_FINISHED;
        DEBUG_INFO("State changed: to %s, from %s",
          state_strings[app.current_state],
          state_strings[app.prev_state]);
      } else {
        app.prev_state = app.current_state;
        app.current_state = APP_STATE_DOWNLOAD_FAILED;
        DEBUG_INFO("State changed: to %s, from %s",
          state_strings[app.current_state],
          state_strings[app.prev_state]);
      }
    }
    break;

  case APP_STATE_DOWNLOAD_FAILED:
    // TODO report error in file download and close the connection
    app.prev_state = app.current_state;
    app.current_state = APP_STATE_ERROR;
    DEBUG_INFO("State changed: to %s, from %s",
      state_strings[app.current_state],
      state_strings[app.prev_state]);
    break;

  case APP_STATE_DOWNLOAD_FINISHED:
    DEBUG_INFO("Download finished: %uMB", app.downloaded_bytes>>20);
    DEBUG_INFO("Last value in the buffer: 0x%08X", app.last_value);
    application_final_report();

    app.prev_state = app.current_state;
    app.current_state = APP_STATE_RESET;
    DEBUG_INFO("State changed: to %s, from %s",
      state_strings[app.current_state],
      state_strings[app.prev_state]);
    break;

  case APP_STATE_ERROR:
    // The app reached an expected error state
    // TODO report this state and go in the default, status not defined handler to reset the state
  case APP_STATE_RESET:
    // in this state we reset the application and we start back from the beginning

    reset_app(app);

    app.prev_state = app.current_state;
    app.current_state = APP_STATE_IFACE_UP;
    DEBUG_INFO("State changed: to %s, from %s",
      state_strings[app.current_state],
      state_strings[app.prev_state]);
    break;
  }
}

// application stats
void application_report(bool force) {
  if(force || app.current_state == APP_STATE_PARSE_HEADER || app.current_state == APP_STATE_DOWNLOAD) {

    // float speed_conversion_factor = 1e3/(1<<10);
    float speed_conversion_factor = 8*1e3/float(1<<20);
    float elapsed = millis()-app.speed_start;

    float speed = (app.speed_bytes / elapsed) * speed_conversion_factor;
    DEBUG_INFO("Application layer: %12u/%12u speed: %.2f Mbit/s", app.downloaded_bytes, app.file_length, speed);

    app.speed_start = millis();
    app.speed_bytes = 0;
  }
}

void application_final_report() {
  // float speed_conversion_factor = 10e3/(1<<10);
  float speed_conversion_factor = 1e3*8/float(1<<20);

  float elapsed = millis()-app.start;
  float speed = (app.downloaded_bytes / elapsed) * speed_conversion_factor;
  DEBUG_INFO(
    "Application layer: Downloaded %u MB in %.2fs average speed: %.2f Mbit/s",
    app.downloaded_bytes>>20, elapsed/1000, speed);
}

// payload checking function
bool verify_buffer_sequential_4B(uint8_t *buffer, size_t len, uint32_t& offset, uint8_t *excess, uint8_t &excess_len, bool print) {
  size_t i=0;
  bool res = true;
  uint32_t value=0, first=0;

  if(excess_len > 0) {
    uint8_t j=0;
    for(; j<excess_len; j++) {
      value |= excess[j] << ((3-j)*8);
    }

    for(; j<4 && i<len; j++,i++) {
      value |= buffer[i] << ((3-j)*8);

      if(excess_len < 3) {
        excess[j] = buffer[i];
        excess_len++;
      }
    }

    if(value != offset) {
      DEBUG_INFO("perror %08X, %08X", value, offset);

      res = false;
    }
    offset++;
    first = value;
  }

  for(; i+4<=len; i+=4,offset++) {
    // convert buffer from big endian bytearray to uint32
    value =
      *(buffer+i)   << 24 |
      *(buffer+i+1) << 16 |
      *(buffer+i+2) << 8  |
      *(buffer+i+3);

    if(first == 0) {
      first = value;
    }
    // if(print) {
    //   DEBUG_INFO("value: %X", value);
    // }

    if(value != offset && res) {
      DEBUG_INFO("error %8X, %8X", value, offset);

      res = false;
    }
  }

  // put the bytes that exceed the modulo4 in the excess array
  excess_len = len - i;
  for(uint8_t j=0; i<len; j++,i++){
    excess[j] = buffer[i];
  }

  if(print) {
    DEBUG_INFO("packet First: %08X LAST %08X", first, value);
  }


  return res;
}

bool verify_buffer_sequential_faster_4B(uint8_t *buffer, size_t len, uint32_t& offset, uint8_t *excess, uint8_t &excess_len, bool print) {
  size_t i=0;
  bool res = true;
  uint32_t first=0;

  if(excess_len > 0) {
    // the first value needs to be taken from the excess bytes of the previous buffer and the first of this
    uint8_t j=0;
    for(; j<excess_len; j++) {
      first |= excess[j] << ((3-j)*8);
    }

    for(; j<4 && i<len; j++,i++) {
      first |= buffer[i] << ((3-j)*8);
    }
  } else {
    // the first value needs to be taken from the current buffer
    for(; i<4; i++) {
      first |= buffer[i] << ((3-i)*8);
    }
  }

  // DEBUG_INFO("verify: found %08X, expected %08X, i %1u len %8u, excess_len %1u", first, offset, i, len, excess_len);
  if(first != offset) {
    DEBUG_INFO("perror: found %08X, expected %08X", first, offset);

    res = false;
  }
  // offset++;

  // After reconstructing the first integer, we can skip the verification of the rest of the payload,
  // assuming that the issues are always caused by a missing section between buffers.
  // This means that we only need to verify the first value, and update the value for offset

  // The len of the returned excess is the following:
  uint8_t new_excess_len = (len+excess_len) % 4;
  i = len - new_excess_len;
  offset = offset + (i+excess_len)/4;

  // collect the excess for the next buffer
  for(uint8_t j=0; i<len; j++,i++){
    excess[j] = buffer[i];
  }

  excess_len = new_excess_len;

  return res;
}

// Utility functions
void dump_buffer(uint8_t* b, uint32_t len, uint8_t blocks, uint8_t cols) {

  // TODO make sure blocks is less that cols
  Serial.println("BUFFER >>>>>>>");
  for(uint8_t *p=b; p<b+len; p++) {
    if(p == nullptr) {
      break;
    }
    if(*p < 0x10) {
      Serial.print(0);
    }
    Serial.print(*p,  HEX);

    if(cols != 0 && ((p-b)+1) % blocks == 0 && ((p-b)+1) % cols != 0){
      Serial.print(" ");
    }
    if(cols != 0 && ((p-b)+1) % cols == 0){
      Serial.println();
    }
  }
  Serial.println("\nBUFFER <<<<<<<");
}

void dump_buffer_char(uint8_t* b, uint32_t len) {
  Serial.println("BUFFER_CHAR >>>>>>>");
  for(uint8_t *p=b; p<b+len; p++) {
    Serial.print((char)*p);
  }
  Serial.println("\nBUFFER_CHAR <<<<<<<");
}