/*
  Web ICMP Ping

  This sketch pings a device based on the IP address or the hostname
  using the Ethernet module.

  It's required an Ethernet network with DHCP  

  created 14 February 2024
  by paulvha
  modified 8 Jenuary 2025
  by fabik111

 */

#include <EthernetC33.h>

/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  while(Ethernet.begin() == 0){
    Serial.println("Failed to configure Ethernet using DHCP");
    delay(5000);
  }

  Serial.println("Ethernet configuration OK!");
  Serial.print("Ip address: ");
  Serial.println(Ethernet.localIP());
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
  delay(5000);
}
