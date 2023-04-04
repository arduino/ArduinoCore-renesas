/**************************************************************
 *  Works - 31/05/2021 - Telemetry + Device Twin
 * ESP32 LilyGo-T-Call-SIM800 Example
 *
 * HTTPS (TLS/SLL) with CA Certificate via "TinyGsm.h": https://github.com/vshymanskyy/TinyGSM
 * Tested on SIM800L_IP5306_VERSION_20190610 (v1.3) (R14.18)
 *
 * About board: https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800
 * About SIM800L_IP5306 v1.3: https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800/blob/master/doc/SIM800L_IP5306.MD    
 * Base example: https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800/tree/master/examples/Arduino_TinyGSM  
 * 
 * 
 * Adapted by Reinaldo Abreu
 **************************************************************/
#include <Wire.h>
#include "SSLClient.h"

//MQTT Client lib: https://github.com/knolleary/pubsubclient
#include <PubSubClient.h>

//Please enter your CA certificate in ca_cert.h
#include "ca_cert.h"

// ESP32 LilyGo-T-Call-SIM800 SIM800L_IP5306_VERSION_20190610 (v1.3) pins definition
#define MODEM_RST 5
#define MODEM_PWRKEY 4
#define MODEM_POWER_ON 23
#define MODEM_TX 27
#define MODEM_RX 26
#define I2C_SDA 21
#define I2C_SCL 22
#define LED_GPIO 13
#define LED_PIN 13
#define LED_ON HIGH
#define LED_OFF LOW
#define IP5306_ADDR 0x75
#define IP5306_REG_SYS_CTL0 0x00

// Set serial for debug console (to the Serial Monitor)
#define SerialMon Serial
// Set serial for AT commands (to the SIM800 module)
#define SerialAT Serial1

#define TINY_GSM_DEBUG SerialMon
//#define DUMP_AT_COMMANDS          //Uncomment to see AT commands on Serial


// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800   // Modem is SIM800
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

// Include after TinyGSM definitions
#include <TinyGsmClient.h>

// Your GPRS credentials (leave empty, if missing)
const char apn[] = "";       // Your APN
const char gprs_user[] = ""; // User
const char gprs_pass[] = ""; // Password
const char simPIN[] = "";    // SIM card PIN code, if any

// MQTT Config

const char mqtt_broker[] = "CHANGE_TO_AZURE_IOT_HUB_NAME.azure-devices.net";
#define MQTT_DEVICE     "CHANGE_TO_DEVICE_NAME_AZURE_IOT_HUB"                   //Device name on Azure IoT Hub
#define MQTT_USERNAME   "CHANGE_TO_AZURE_IOT_HUB_NAME.azure-devices.net/CHANGE_TO_DEVICE_NAME_AZURE_IOT_HUB/?api-version=2020-09-30&DeviceClientType=c%2F1.2.0-beta.1"
//Password Empty to x509 SelfSigned Device or SharedAccessSignature... to SaS Device
#define MQTT_PASS       ""
//#define MQTT_PASS "SharedAccessSignature sr=CHANGE_TO_AZURE_IOT_HUB_NAME.azure-devices.net%2Fdevices%2FCHANGE_TO_DEVICE_NAME_AZURE_IOT_HUB..." //Check SaS Documentation

int secure_port = 8883; // TCP-TLS Port



#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm sim_modem(debugger);
#else
TinyGsm sim_modem(SerialAT);
#endif

//Layers stack
//TinyGsm sim_modem(SerialAT);
TinyGsmClient gsm_transpor_layer(sim_modem);
SSLClient secure_presentation_layer(&gsm_transpor_layer);
PubSubClient client(secure_presentation_layer);

// Power configuration for SIM800L_IP5306_VERSION_20190610 (v1.3) board
bool setupPMU()
{
  bool en = true;
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL0);
  if (en)
  {
    Wire.write(0x37);
  }
  else
  {
    Wire.write(0x35);
  }
  return Wire.endTransmission() == 0;
}

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
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_DEVICE, MQTT_USERNAME, MQTT_PASS))
    //if (client.connect(client_name))
    {
      Serial.println("-----------------------------------connected-----------------------");
      // Once connected, publish an announcement...
     // client.publish("outTopic", "hello world");
      client.publish("devices/CHANGE_TO_DEVICE_NAME_AZURE_IOT_HUB/messages/events/","Test");     // Topic to publish telemetry

      // ... and resubscribe
      client.subscribe("$iothub/twin/PATCH/properties/desired/#");                                //Topic to subscribe Device Twin
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


// Modem initial setup (cold start)
void setupModem()
{
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Reset pin high
  digitalWrite(MODEM_RST, HIGH);

  // Turn on the Modem power first
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Pull down PWRKEY for more than 1 second according to manual requirements
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(200);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1200);
  digitalWrite(MODEM_PWRKEY, HIGH);

  // Initialize the indicator as an output
  digitalWrite(LED_PIN, LOW);
}

void setup()
{
  SerialMon.begin(115200);
  delay(10);

  // Start power management
  if (!setupPMU())
  {
    Serial.println("Setting power error");
  }

  // Set SIM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  
  //Add CA Certificate
  secure_presentation_layer.setCACert(root_ca);
  secure_presentation_layer.setCertificate(client_cert_pem_start);    //x509 client Certificate
  secure_presentation_layer.setPrivateKey(client_key_pem_start);      //x509 client key

  // Modem initial setup
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

  // Wait for network availability
  SerialMon.print("Waiting for network...");
  if (!sim_modem.waitForNetwork(240000L))
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
  SerialMon.print(F("Connecting to APN: "));
  SerialMon.print(apn);
  if (!sim_modem.gprsConnect(apn, gprs_user, gprs_pass))
  {
    SerialMon.println(" fail");
    delay(10000);
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
