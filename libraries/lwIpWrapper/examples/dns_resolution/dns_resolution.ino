#include <Arduino_DebugUtils.h>
#include <EthernetC33.h>
#include <WiFiC3.h>
#include "arduino_secrets.h"

static char const SSID[] = SECRET_SSID;  /* your network SSID (name) */
static char const PASS[] = SECRET_PASS;  /* your network password (use for WPA, or use as key for WEP) */

void application();

#define BLOCKING_DNS_RESOLUTION

void setup() {
    Serial.begin(115200);
    while(!Serial);

    DEBUG_INFO("Setting up netif");

    Ethernet.begin();

    int res = 0;
    DEBUG_INFO("Connecting to AP");
    while((res=WiFi.begin(SSID, SECRET_PASS)) != ESP_CONTROL_OK) {
        DEBUG_INFO("Connection failed retry: %d", res);
        delay(1000);
    }
    DEBUG_INFO("Connected to AP");
    DEBUG_INFO("Beginning");
}

void loop() {
#ifndef LWIP_USE_TIMER
    CLwipIf::getInstance().task();
#endif

    application();
}

// application stuff
volatile uint8_t state = 0;
uint32_t counter = 0;

char* domains[] = {
    "google.it"
    , "www.google.com"
    , "arduino.cc"
    , "oniudra.cc"
    , "youtube.it"
    , "youtube.com"
    , "github.com"
    , "drive.google.com"
};

#ifndef BLOCKING_DNS_RESOLUTION
void dns_cbk(const IPAddress& ip) {
    DEBUG_INFO("%u DNS response for %s: %s ",
        counter,
        domains[counter % (sizeof(domains)/sizeof(char*))],
        ip.toString().c_str());
    state = 1;
    counter++;
}
#endif // BLOCKING_DNS_RESOLUTION

void application() {

    switch(state) {
    case 0:
        if(WiFiStation.isDhcpAcquired() && Ethernet.isDhcpAcquired()) {
            DEBUG_INFO("dhcp acquired");

            state = 1;
        }
        break;
    case 1: {
        DEBUG_INFO("changing default Interface: \"%s\"", counter%2==0 ? "Ethernet": "WiFiStation");

        CLwipIf::getInstance().setDefaultIface(counter%2==0? (CNetIf*)&Ethernet: (CNetIf*)&WiFiStation);

        DEBUG_INFO("%u Performing DNS request for %s",
            counter,
            domains[counter % (sizeof(domains)/sizeof(char*))]);
#ifdef BLOCKING_DNS_RESOLUTION
        IPAddress ip;

        auto res = CLwipIf::getInstance().getHostByName(
            domains[counter % (sizeof(domains)/sizeof(char*))],
            ip,
#ifndef LWIP_USE_TIMER
            true);
#else
            false);
#endif

        counter++;
        DEBUG_INFO("%u DNS response for %s: %u %s ",
            counter,
            domains[counter % (sizeof(domains)/sizeof(char*))],
            res,
            ip.toString().c_str());
#else // BLOCKING_DNS_RESOLUTION
        state = 2;
        auto res = CLwipIf::getInstance().getHostByName(
            domains[counter % (sizeof(domains)/sizeof(char*))],
            dns_cbk);

        if(res != 1) {
            counter++;
        }
#endif // BLOCKING_DNS_RESOLUTION
        break;
    }
    case 2:
        // do nothing, request made, wait for request to complete
        break;
    }

}

