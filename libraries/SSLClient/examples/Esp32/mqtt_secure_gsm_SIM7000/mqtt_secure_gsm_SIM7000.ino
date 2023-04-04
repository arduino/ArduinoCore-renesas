/**************************************************************
 *
 * ESP32 LilyGO-T-SIM7000G Example
 *
 * MQQT (TLS/SLL) with CA Certificate via "TinyGsm.h": https://github.com/vshymanskyy/TinyGSM
 * Tested on Version 20200415
 *
 * About board: https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G
 * About Version 20200415: https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G/blob/master/Historical/SIM7000G_20200415/README.MD
 * Base example:  https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G/blob/master/examples/Arduino_TinyGSM/AllFunctions/AllFunctions.ino
 *                https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G/blob/master/examples/Arduino_NetworkTest/Arduino_NetworkTest.ino
 *                https://github.com/Xinyuan-LilyGO/LilyGO-T-SIM7000G/blob/master/examples/Arduino_Azure_IoTHub/Arduino_Azure_IoTHub.ino
 *                https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_basic/mqtt_basic.ino
 * 
 **************************************************************/
#include "SSLClient.h"
//MQTT Client lib: https://github.com/knolleary/pubsubclient
#include <PubSubClient.h>

//Please enter your CA certificate in ca_cert.h
#include "ca_cert.h"

// ESP32 LilyGO-T-SIM7000G pins definition
#define MODEM_UART_BAUD 9600
#define MODEM_DTR 25
#define MODEM_TX 27
#define MODEM_RX 26
#define MODEM_PWRKEY 4
#define LED_PIN 12

// Set serial for debug console (to the Serial Monitor)
#define SerialMon Serial
// Set serial for AT commands (to the SIM7000 module)
#define SerialAT Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM7000  // Modem is SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

// Include after TinyGSM definitions
#include <TinyGsmClient.h>

// Your GPRS credentials (leave empty, if missing)
const char apn[] = "";       // Your APN
const char gprs_user[] = ""; // User
const char gprs_pass[] = ""; // Password
const char simPIN[] = "";    // SIM card PIN code, if any

// MQTT Config
// EMQX Free Secure Broker for test https://www.emqx.io/mqtt/public-mqtt5-broker
// Check the website to find out more about the available options
// And obtain the current certificate
const char client_name[] = "MyEsp32";
const char mqtt_broker[] = "broker.emqx.io";
int secure_port = 8883; // TCP-TLS Port

// Layers stack
TinyGsm sim_modem(SerialAT);
TinyGsmClient gsm_transpor_layer(sim_modem);
SSLClient secure_presentation_layer(&gsm_transpor_layer);
PubSubClient client(secure_presentation_layer);

// For read the MQTT events
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// To connect to the broker
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(client_name))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("...try again in 5 seconds");
      delay(5000);
    }
  }
}

void turnModemOn()
{
  digitalWrite(LED_PIN, LOW);

  pinMode(MODEM_PWRKEY, OUTPUT);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000); //Datasheet Ton mintues = 1S
  digitalWrite(MODEM_PWRKEY, HIGH);
}

void turnModemOff()
{
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1500); //Datasheet Ton mintues = 1.2S
  digitalWrite(MODEM_PWRKEY, HIGH);

  digitalWrite(LED_PIN, LOW);
}

void setupModem()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  turnModemOff();
  delay(1000);
  turnModemOn();
  delay(5000);
}

void setup()
{
  SerialMon.begin(9600);
  delay(100);

  // Set SIM module baud rate and UART pins
  SerialAT.begin(MODEM_UART_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);

  //Add CA Certificate
  secure_presentation_layer.setCACert(root_ca);

  // SIM modem initial setup
  setupModem();

  // MQTT init
  client.setServer(mqtt_broker, secure_port);
  client.setCallback(callback);
}

void loop()
{
  SerialMon.print("Initializing modem...");
  if (!sim_modem.init())
  {
    SerialMon.print(" fail... restarting modem...");
    setupModem();
    // Restart takes quite some time
    // Use modem.init() if you don't need the complete restart
    if (!sim_modem.restart())
    {
      SerialMon.println(" fail... even after restart");
      return;
    }
  }
  SerialMon.println(" OK");

  // General information
  String name = sim_modem.getModemName();
  Serial.println("Modem Name: " + name);
  String modem_info = sim_modem.getModemInfo();
  Serial.println("Modem Info: " + modem_info);

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && sim_modem.getSimStatus() != 3)
  {
    sim_modem.simUnlock(simPIN);
  }

  // Set modes
  /*
    2 Automatic
    13 GSM only
    38 LTE only
    51 GSM and LTE only
  * * * */
  sim_modem.setNetworkMode(2);
  delay(3000);
  /*
    1 CAT-M
    2 NB-Iot
    3 CAT-M and NB-IoT
  * * */
  sim_modem.setPreferredMode(3);
  delay(3000);

  // Wait for network availability
  SerialMon.print("Waiting for network...");
  if (!sim_modem.waitForNetwork())
  {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");

  // Connect to the GPRS network
  SerialMon.print("Connecting to network...");
  if (!sim_modem.isNetworkConnected())
  {
    SerialMon.println(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");

  // Connect to APN
  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!sim_modem.gprsConnect(apn, gprs_user, gprs_pass))
  {
    SerialMon.println(" fail");
    return;
  }
  digitalWrite(LED_PIN, HIGH);
  SerialMon.println(" OK");

  // More info..
  Serial.println("");
  String ccid = sim_modem.getSimCCID();
  Serial.println("CCID: " + ccid);
  String imei = sim_modem.getIMEI();
  Serial.println("IMEI: " + imei);
  String cop = sim_modem.getOperator();
  Serial.println("Operator: " + cop);
  IPAddress local = sim_modem.localIP();
  Serial.println("Local IP: " + String(local));
  int csq = sim_modem.getSignalQuality();
  Serial.println("Signal quality: " + String(csq));

  // MQTT Test loop
  // As long as we have connectivity
  while (sim_modem.isGprsConnected())
  {
    // We maintain connectivity with the broker
    if (!client.connected())
    {
      reconnect();
    }
    // We are listening to the events
    client.loop();
  }

  // Disconnect GPRS and PowerOff
  // Apparently the "gprsDisconnect()" method (TinyGSM) are not working well with the SIM7000...
  // ...you have to use additionally "poweroff()".
  // With that, the modem can be connected again in the next cycle of the loop.
  //sim_modem.gprsDisconnect();
  //sim_modem.poweroff();

  delay(15000);
}
