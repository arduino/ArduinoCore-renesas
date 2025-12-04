/*
  TLS WiFi Web client

  Board CA Root certificate bundle is embedded inside WiFi firmware:
  https://github.com/arduino/uno-r4-wifi-usb-bridge/blob/main/certificates/cacrt_all.pem

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-client-ssl
*/

#include "WiFiS3.h"
#include "WiFiSSLClient.h"
#include "IPAddress.h"

#include "arduino_secrets.h"

// maximum number of times the uri will be checked
#define MaximumConnections 2

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)


int connectionCount = 0;
bool clientConnected = false;
int status = WL_IDLE_STATUS;

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiSSLClient client;

/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
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
    Serial.println("Please upgrade the firmware");
  }

  // 3 second wait for connection
  client.setTimeout(3000);
}

void connectToWifi() {
  if (status != WL_IDLE_STATUS)
    return;

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  status = WiFi.begin(ssid, pass);
}

/* just wrap the received data up to 80 columns in the serial print*/
/* -------------------------------------------------------------------------- */
void read_response() {
/* -------------------------------------------------------------------------- */
  uint32_t received_data_num = 0;
  while (client.available()) {
    /* actual data reception */
    char c = client.read();
    /* print data to serial port */
    Serial.print(c);
    /* wrap data to 80 columns*/
    received_data_num++;
    if(received_data_num % 80 == 0) {
      Serial.println();
    }
  }
}

/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */
    Serial.println("loop processing");
    
    // only allowed to connect n times
    if (connectionCount >= MaximumConnections) {
        delay(2000);
        return;
    }

    // Connect to WiFi if not already connected
    connectToWifi();
    status = WiFi.isConnected();

    if (status == WL_CONNECTING) {
        Serial.println("Connecting to wifi");
        delay(200);
        return; 
    }

    // If connected to Wifi then send a request to a server
    if (status == WL_CONNECTED) {
        Serial.println("Connected to WiFi");
        printWifiStatus();
     
        Serial.println("\nStarting connection to server...");
        
        if (client.connect(server, 443)) {
            connectionCount++;

            Serial.println("connected to server");
            
            // Make HTTP request
            client.println("GET /search?q=arduino HTTP/1.1");
            client.println("Host: www.google.com");
            client.println("Connection: close");
            client.println();

            Serial.println("Reading response");
            read_response();
            
            Serial.println("disconnecting from server.");
            client.stop();
            
            // Reset status so we don't immediately reconnect
            status = WL_IDLE_STATUS;
        } else {
            Serial.println("Connection to server failed!");
        }
    }
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
