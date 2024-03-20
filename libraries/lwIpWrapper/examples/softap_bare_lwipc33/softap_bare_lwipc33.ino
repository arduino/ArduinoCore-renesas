#include <Arduino.h>
#include <Arduino_DebugUtils.h>
#include <regex>
#include <utils.h>

// #define CNETIF_STATS_ENABLED
// #include "CNetifStats.h"

#ifdef CNETIF_STATS_ENABLED
#define STATS_BUFFER_SIZE 1000
char cnetif_stats_buffer[STATS_BUFFER_SIZE];
// netif_stats _stats;
#endif // CNETIF_STATS_ENABLED

// #define ETHER_CFG_PARAM_CHECKING_ENABLE
// Renesas libraries

#include "IPAddress.h"

// IPAddress default_ip("192.168.10.130");
// IPAddress default_nm("255.255.255.0");
// IPAddress default_gw("192.168.10.1");
ip_addr_t ip;
ip_addr_t nm;
ip_addr_t gw;

// FspTimer timer;

#define CHECK_PAYLOAD

#define LOOP_MIN_DURATION 100 // us

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

SoftAPLWIPNetworkInterface C33WifiIface;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Renesas file download example");
  // lwip setup
  lwip_init(); // TODO move this inside the network stack init
  LWIPNetworkStack::getInstance(); // TODO make this automatic

  DEBUG_INFO("Setting up netif");
  auto res = C33WifiIface.begin();
  DEBUG_INFO("%d", res);

  DEBUG_INFO("Starting AP");
  while((res=C33WifiIface.startSoftAp("testAP", "123456789", 5)) != ESP_CONTROL_OK) {
    DEBUG_INFO("Connection failed retry: %d", res);
    delay(1000);
  }
  DEBUG_INFO("AP started");

  // setup netif
  // IP_ADDR4(&ip, 192, 168, 10, 130);
  // IP_ADDR4(&nm, 255, 255, 255, 0);
  // IP_ADDR4(&gw, 192, 168, 10, 1);

  DEBUG_INFO("Begin of reception\n\n");
  debug_start = millis();
}

uint32_t counter =0;
void loop() {

  uint32_t start = micros();
#ifndef NETWORKSTACK_USE_TIMER
  LWIPNetworkStack::getInstance().task();
#endif
  if(millis() - debug_start > 3000) { // print the debug _stats every x second
    // DEBUG_INFO("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    DEBUG_INFO("time: %12ums", millis());
    // DEBUG_INFO("memory: %12u bytes \tmin: %12u bytes \tmax: %12u bytes",
    //   memory_used, memory_used_min, memory_used_max);
    DEBUG_INFO("loop counter %u\n", counter);\

#ifdef CNETIF_STATS_ENABLED
    netif_stats_sprintf(cnetif_stats_buffer, C33WifiIface.stats, STATS_BUFFER_SIZE, (8*1e6)/(1<<20), "Mbit/s");
    // __disable_irq();
    arduino::lock();
    NETIF_STATS_RESET_AVERAGES(C33WifiIface.stats);
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