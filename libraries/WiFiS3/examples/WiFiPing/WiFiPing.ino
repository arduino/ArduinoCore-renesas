/*
  Web ICMP Ping

  This sketch pings a device based on the IP address or the hostname
  using the WiFi module. By default the attempt is performed 5 times, but can
  be changed to max. 255

  It requires at least version 0.5.0 of USB Wifi bridge firmware and WiFiS3 library.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the WiFi.begin() call accordingly.

  created 14 February 2024
  by paulvha

 */

#include "WiFiS3.h"
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;             // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;

/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed.");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  printWifiStatus();
}

/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */

  // Ping IP
  const IPAddress remote_ip(140,82,121,4);
  Serial.print("Trying to ping github.com on IP: ");
  Serial.println(remote_ip);

  // using default ping count of 1
  int res = WiFi.ping(remote_ip);

  if (res > 0) {
    Serial.print("Ping response time: ");
    Serial.print(res);
    Serial.println(" ms");
  }
  else {
    Serial.println("Timeout on IP!");
    Serial.println("Make sure your WiFi firmware version is at least 0.5.0");
  }

  // Ping Host
  const char* remote_host = "www.google.com";
  Serial.print("Trying to ping host: ");
  Serial.println(remote_host);

  // setting ttl to 128 and ping count to 10
  int res1 = WiFi.ping(remote_host, 128, 10);

  if (res1 > 0) {
    Serial.print("Ping average response time: ");
    Serial.print(res1);
    Serial.println(" ms");
  }
  else {
    Serial.println("Timeout on host!");
    Serial.println("Make sure your WiFi firmware version is at least 0.5.0");
  }

  Serial.println();
  delay(1000);
}

/* -------------------------------------------------------------------------- */
void printWifiStatus() {
/* -------------------------------------------------------------------------- */
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
