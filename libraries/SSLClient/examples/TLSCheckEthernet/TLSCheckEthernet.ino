/**************************************************************
 *
 * TLS check Example
 *
 * HTTPS (TLS/SSL) with CA Certificate via Ethernet
 *
 **************************************************************/

#include <EthernetC33.h>
#include "SSLClient.h"
//To make http request esay: https://github.com/arduino-libraries/ArduinoHttpClient
#include <ArduinoHttpClient.h>
#include "ca_cert.h"

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 8, 177);

const char hostname[] = "www.howsmyssl.com";
int port = 443;

//Layers stack
EthernetClient eth_client;
SSLClient ssl_client(&eth_client);
HttpClient http_client = HttpClient(ssl_client, hostname, port);

void setup()
{
  Serial.begin(9600);
  while(!Serial);

  Serial.println("Starting Ethernet");
  if (Ethernet.begin() == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to configure using IP address instead of DHCP:
    Ethernet.begin(ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("Connecting...");

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
