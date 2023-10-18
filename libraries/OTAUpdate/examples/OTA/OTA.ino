/*
  OTA

  This sketch demonstrates how to make an OTA Update on the UNO R4 WiFi.
  Upload the sketch and wait for the invasion!

*/


#include "WiFiS3.h"
#include "OTAUpdate.h"
#include "root_ca.h"
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;

OTAUpdate ota;
static char const OTA_FILE_LOCATION[] = "https://downloads.arduino.cc/ota/UNOR4WIFI_Animation.ota";

/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the Wi-Fi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with Wi-Fi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wi-Fi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 1 seconds for connection:
    delay(1000);
  }

  printWiFiStatus();

  OTAUpdate::Error ret = OTAUpdate::Error::None;
  ret = ota.begin("/update.bin");
  if(ret != OTAUpdate::Error::None) {
    Serial.println("ota.begin() error: ");
    Serial.println((int)ret);
    return;
  }
  ret = ota.setCACert(root_ca);
  if(ret != OTAUpdate::Error::None) {
    Serial.println("ota.setCACert() error: ");
    Serial.println((int)ret);
    return;
  }
  int ota_size = ota.download(OTA_FILE_LOCATION, "/update.bin");
  if(ota_size <= 0) {
    Serial.println("ota.download() error: ");
    Serial.println(ota_size);
    return;
  }
  ret = ota.verify();
  if(ret != OTAUpdate::Error::None) {
    Serial.println("ota.verify() error: ");
    Serial.println((int)ret);
    return;
  }

  ret = ota.update("/update.bin");
  if(ret != OTAUpdate::Error::None) {
    Serial.println("ota.update() error: ");
    Serial.println((int)ret);
    return;
  }
}

/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */
  delay(1000);
}

/* -------------------------------------------------------------------------- */
void printWiFiStatus() {
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
