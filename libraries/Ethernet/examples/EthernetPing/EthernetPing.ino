/*
  Web ICMP Ping

  This sketch pings a device based on the IP address or the hostname
  using the Ethernet module.

  created 14 February 2024
  by paulvha

  updated 27 February 2025
  by Fabik111

 */

#include "EthernetC33.h"

int status = WL_IDLE_STATUS;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(10, 130, 22, 84);

/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin() == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to configure using IP address instead of DHCP:
    // IN THAT CASE YOU SHOULD CONFIGURE manually THE DNS or USE the IPAddress Server variable above
    // that is what is automatically done here...
    Ethernet.begin(ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(2000);
}

/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */

  // Ping IP
  const IPAddress remote_ip(140,82,121,4);
  Serial.print("Trying to ping github.com on IP: ");
  Serial.println(remote_ip);

  // using default ping count of 1
  int res = Ethernet.ping(remote_ip);

  if (res > 0) {
    Serial.print("Ping response time: ");
    Serial.print(res);
    Serial.println(" ms");
  }
  else {
    Serial.println("Timeout on IP!");
  }

  // Ping Host
  const char* remote_host = "www.google.com";
  Serial.print("Trying to ping host: ");
  Serial.println(remote_host);

  // setting ttl to 128 and ping count to 10
  int res1 = Ethernet.ping(remote_host);

  if (res1 > 0) {
    Serial.print("Ping average response time: ");
    Serial.print(res1);
    Serial.println(" ms");
  }
  else {
    Serial.println("Timeout on host!");
  }

  Serial.println();
  delay(1000);
}
