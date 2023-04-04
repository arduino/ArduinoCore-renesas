/**************************************************************
 *
 * ESP32 LilyGo-T-ETH-POE Example
 *
 * HTTPS (TLS/SLL) with CA Certificate via "ETH.h"
 * This example use the WiFiClient class to integrate ETH functionality  
 *
 * About board:   https://github.com/Xinyuan-LilyGO/LilyGO-T-ETH-POE
 * Base example:  https://github.com/Xinyuan-LilyGO/LilyGO-T-ETH-POE/blob/master/example/eth/eth.ino
 * 
 **************************************************************/
#include <ETH.h>
#include "WiFi.h"
#include "SSLClient.h"
//To make http request esay: https://github.com/arduino-libraries/ArduinoHttpClient
#include <ArduinoHttpClient.h>

//Please enter your CA certificate in ca_cert.h
#include "ca_cert.h"

//ESP32 LilyGo-T-ETH-POE Board Ethernet pins definition
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN -1
#define ETH_TYPE ETH_PHY_LAN8720
#define ETH_ADDR 0
#define ETH_MDC_PIN 23
#define ETH_MDIO_PIN 18

const char hostname[] = "www.howsmyssl.com";
int port = 443;

//Layers stack
WiFiClient eth_transpor_layer;
SSLClient secure_presentation_layer(&eth_transpor_layer);
HttpClient http_client = HttpClient(secure_presentation_layer, hostname, port);

static bool eth_connected = false;

//Ethernet events
void ETHEvent(WiFiEvent_t event)
{
  switch (event)
  {
  case SYSTEM_EVENT_ETH_START:
    Serial.println("ETH Started");
    ETH.setHostname("esp32-ethernet");
    break;
  case SYSTEM_EVENT_ETH_CONNECTED:
    Serial.println("ETH Connected");
    break;
  case SYSTEM_EVENT_ETH_GOT_IP:
    Serial.print("ETH MAC: ");
    Serial.print(ETH.macAddress());
    Serial.print(", IPv4: ");
    Serial.print(ETH.localIP());
    if (ETH.fullDuplex())
    {
      Serial.print(", FULL_DUPLEX");
    }
    Serial.print(", ");
    Serial.print(ETH.linkSpeed());
    Serial.println("Mbps");
    eth_connected = true;
    break;
  case SYSTEM_EVENT_ETH_DISCONNECTED:
    Serial.println("ETH Disconnected");
    eth_connected = false;
    break;
  case SYSTEM_EVENT_ETH_STOP:
    Serial.println("ETH Stopped");
    eth_connected = false;
    break;
  default:
    break;
  }
}

void setup()
{
  Serial.begin(9600);

  Serial.println("Starting ETH");
  WiFi.onEvent(ETHEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
  while (!eth_connected)
  {
    Serial.println("connecting to ETH..");
    delay(1000);
  }
  Serial.println("Connected");

  //Add CA Certificate
  secure_presentation_layer.setCACert(root_ca);
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
