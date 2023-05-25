/*
  Web client

 This sketch connects to a website (http://www.google.com)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen
 modified 23 Jun 2017
 by Wi6Labs
 modified 1 Jun 2018
 by sstaub
 */

#include <EthernetC33.h>

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(10, 130, 22, 84);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  bool use_dns = true; 
  
  // start the Ethernet connection:
  if (Ethernet.begin() == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to configure using IP address instead of DHCP:
    // IN THAT CASE YOU SHOULD CONFIGURE manually THE DNS or USE the IPAddress Server variable above
    // that is what is automatically done here...
    Ethernet.begin(ip);
    use_dns = false;
  }
  // give the Ethernet shield a second to initialize:
  delay(2000);
  Serial.println("connecting...");

  Serial.print("Your DNS server is: ");
  Serial.println(Ethernet.dnsServerIP());

  bool connect_result = false;

  if(use_dns) {
    connect_result = client.connect(server, 80);
  } 
  else {
    connect_result = client.connect(IPAddress(74,125,232,128), 80);
  }

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

/* just wrap the received data up to 80 columns in the serial print*/
void read_request() {
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

void loop() {
 
  read_request();

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}
