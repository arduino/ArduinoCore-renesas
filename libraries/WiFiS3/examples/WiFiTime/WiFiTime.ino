/*
  Get local time from NTP server

  This sketch will obtain the correct local time from the NTP-servers.

  In order to get the correct time, taking in account the daylightsavings, a POSIX
  formatted timezone string MUST be provided. There is NO default setting.

  There are a large number of good articles on the internet
  https://github.com/G6EJD/ESP32-Time-Services-and-SETENV-variable/blob/master/README.md
  Use POSIX timezone format https://developer.ibm.com/articles/au-aix-posix/
  You can choose your time zone also from this list
  https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv


  The output can be formatted according the strftime() function
  see : https://cplusplus.com/reference/ctime/strftime/
  The default format, if nothing provided is "%d-%b-%y, %H:%M:%S"

  There are 2 calls:
  void setTZ(const char * tz);
  const char * getTime(const char * format = "%d-%b-%y, %H:%M:%S");

  Usage
  Set the Timezone information one time in setup(): void WiFi.setTZ("TZ_INFO");
  and then just call WiFi.getTime() or call WiFi.getTime("FORMAT").

  It requires the latest USB Wifi bridge firmware level, with the modified WiFiS3 library.

  This example is written for a network using WPA encryption. For
  WEP or WPA, change the WiFi.begin() call accordingly.

  created 15 February 2024
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
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade to the WiFi USB bridge firmware. freeze !");
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
  const char *tt;

  //=========================================================//

  // this could be in setup() - below is Europe / London
  WiFi.setTZ("GMT0BST,M3.5.0/01,M10.5.0/02");

  // using the default format.
  // Output looks like: 15-Feb-24, 17:02:48
  tt = WiFi.getTime();

  if (strlen(tt) > 0) {
    Serial.print("Current time in London ");
    Serial.println(tt);
  }
  else {
    Serial.println("Error during reading London.");
  }

  //=========================================================//

  // this could be in setup() - below is Europe / Amsterdam
  WiFi.setTZ("CET-1CEST,M3.5.0/2,M10.5.0/3");

  // user defined format definition,
  // Output looks like 06:02PM.
  // see https://cplusplus.com/reference/ctime/strftime/
  tt = WiFi.getTime("%I:%M%p.");

  if (strlen(tt) > 0) {
    Serial.print("Current time in Amsterdam ");
    Serial.println(tt);
  }
  else {
    Serial.println("Error during reading Amsterdam.");
  }

  //=========================================================//

  // this could be in setup() - below is Eastern Standard Time
  WiFi.setTZ("EST5EDT,M3.2.0/2,M11.1.0");

  // user defined format definition,
  // Output looks like 12:02:37.
  // see https://cplusplus.com/reference/ctime/strftime/
  tt = WiFi.getTime("%H:%M:%S");

  if (strlen(tt) > 0) {
    Serial.print("Current time US (EST) Time ");
    Serial.println(tt);
  }
  else {
    Serial.println("Error during reading US (EST) Time.");
  }


  Serial.println();
  delay(5000);
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
