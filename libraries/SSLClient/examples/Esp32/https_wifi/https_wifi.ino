/**************************************************************
 *
 * ESP32 Example
 *
 * HTTPS (TLS/SLL) with CA Certificate via "WiFi.h"
 *
 **************************************************************/

#include "WiFi.h"
#include "SSLClient.h"
//To make http request esay: https://github.com/arduino-libraries/ArduinoHttpClient
#include <ArduinoHttpClient.h>

//Please enter your CA certificate in ca_cert.h
#include "secrets.h"

//Please enter your sensitive data in secrets.h
#include "ca_cert.h"
const char *ssid = SECRET_SSID;
const char *pass = SECRET_PASS;

const char hostname[] = "www.howsmyssl.com";
int port = 443;

//Layers stack
WiFiClient wifi_transpor_layer;
SSLClient secure_presentation_layer(&wifi_transpor_layer);
HttpClient http_client = HttpClient(secure_presentation_layer, hostname, port);

void setup()
{
  Serial.begin(9600);

  Serial.println("Starting Wifi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    Serial.print("failed ... ");
    Serial.println("retrying ... ");
    delay(4000);
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
