/**************************************************************
 *
 * TLS check Example
 *
 * HTTPS (TLS/SSL) with CA Certificate via WiFi
 *
 **************************************************************/

#include "WiFiC3.h"
#include "WiFiClient.h"
#include "SSLClient.h"
//To make http request esay: https://github.com/arduino-libraries/ArduinoHttpClient
#include <ArduinoHttpClient.h>
#include "ca_cert.h"
#include "arduino_secrets.h" 

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;

const char hostname[] = "www.howsmyssl.com";
int port = 443;

//Layers stack
WiFiClient wifi_client;
SSLClient ssl_client(&wifi_client);
HttpClient http_client = HttpClient(ssl_client, hostname, port);

void setup()
{
  Serial.begin(9600);
  while(!Serial);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
     
    // wait 4 seconds for connection:
    delay(4000);
  }
  
  printWifiStatus();

  //Add CA Certificate
  ssl_client.setCACert(root_ca);
}

void loop()
{
  Serial.println("Making GET request...");

  http_client.get("/a/check");

  int status_code = http_client.responseStatusCode();
  String response = http_client.responseBody();

  Serial.print("Status code: ");
  Serial.println(status_code);
  Serial.print("Response: ");
  Serial.println(response);

  http_client.stop();

  delay(5000);
}

void printWifiStatus()
{  
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
