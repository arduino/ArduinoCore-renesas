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
    // application_report();

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
  APP_STATE_SEND,
  APP_STATE_RECEIVE,
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
  "APP_STATE_SEND",
  "APP_STATE_RECEIVE",
  "APP_STATE_ERROR",
  "APP_STATE_RESET"
};

#define APP_BUFFER_SIZE 1*1024

typedef uint32_t counter_t;

struct App {
  app_state_t current_state=APP_STATE_NONE;
  app_state_t prev_state=APP_STATE_NONE;

  lwipClient *tcp_client;
  uint16_t port = 2000;
  IPAddress server_ip = IPAddress(192, 168, 10, 250);

  counter_t counter;
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
  app.counter = 0;
}

void reset_app(struct App& app) {
  init_app(app);

  if(app.tcp_client != nullptr) {
    app.tcp_client->stop();
    // delete app.tcp_client;
  }
}

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
      app.current_state = APP_STATE_SEND;
      DEBUG_INFO("State changed: to %s, from %s",
        state_strings[app.current_state],
        state_strings[app.prev_state]);
    }

    break;

  case APP_STATE_CONNECTED:
    app.start = millis();
    app.speed_start = app.start;

    app.prev_state = app.current_state;
    app.current_state = APP_STATE_SEND;
    DEBUG_INFO("State changed: to %s, from %s",
      state_strings[app.current_state],
      state_strings[app.prev_state]);
    break;
  case APP_STATE_SEND: {
    int res = app.tcp_client->write((uint8_t*)&app.counter, sizeof(counter_t));
    DEBUG_INFO("counter sent, value 0x%08X, res: %d", app.counter, res);

    if(res == sizeof(counter_t)) {
      app.counter++;
      app.prev_state = app.current_state;
      app.current_state = APP_STATE_RECEIVE;
      // DEBUG_INFO("State changed: to %s, from %s",
      //   state_strings[app.current_state],
      //   state_strings[app.prev_state]);
    }
    break;
  }
  case APP_STATE_RECEIVE: {
    counter_t read_counter;
    bytes_read = app.tcp_client->read((uint8_t*)&read_counter, sizeof(counter_t));

    if(bytes_read == sizeof(counter_t)) {
      DEBUG_INFO("counter echoed, value 0x%08X", read_counter);

      app.prev_state = app.current_state;
      app.current_state = APP_STATE_SEND;
      // DEBUG_INFO("State changed: to %s, from %s",
      //   state_strings[app.current_state],
      //   state_strings[app.prev_state]);
    }
    break;
  }
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