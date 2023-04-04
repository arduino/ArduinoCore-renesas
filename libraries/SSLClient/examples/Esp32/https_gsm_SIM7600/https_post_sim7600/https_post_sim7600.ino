/**************************************************************
 *
 * TinyGSM Getting Started guide:
 *   https://tiny.cc/tinygsm-readme
 *
 * NOTE:
 * Some of the functions may be unavailable for your modem.
 * Just comment them out.
 * https://simcom.ee/documents/SIM7600C/SIM7500_SIM7600%20Series_AT%20Command%20Manual_V1.01.pdf
 **************************************************************/

#define TINY_GSM_MODEM_SIM7600

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

/*
 * Tests enabled
 */
#define TINY_GSM_TEST_GPRS          true
#define TINY_GSM_TEST_TCP           true
// powerdown modem after tests
#define TINY_GSM_POWERDOWN          true

// set GSM PIN, if any
#define GSM_PIN             ""

// Your GPRS credentials, if any
const char apn[] = ""; // Your operator APN
const char gprsUser[] = "";
const char gprsPass[] = "";

// Server details to test TCP/SSL
const char server[] = "hookb.in";
const char resource[] = "/eKKEKgbNZPCeYYRdZXDo";
const int port = 443;

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include "SSLClient.h"
#include "utilities.h"
#include "certs.h"

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

// HTTPS Transport
TinyGsmClient base_client(modem, 0);
SSLClient secure_layer(&base_client);
HttpClient client = HttpClient(secure_layer, server, port);

void setup()
{
    // Set console baud rate
    SerialMon.begin(115200);
    delay(10);

    // Set GSM module baud rate
    SerialAT.begin(UART_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);

    /*
    The indicator light of the board can be controlled
    */
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    /*
    MODEM_PWRKEY IO:4 The power-on signal of the modulator must be given to it,
    otherwise the modulator will not reply when the command is sent
    */
    pinMode(MODEM_PWRKEY, OUTPUT);
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(300); //Need delay
    digitalWrite(MODEM_PWRKEY, LOW);

    /*
    MODEM_FLIGHT IO:25 Modulator flight mode control,
    need to enable modulator, this pin must be set to high
    */
    pinMode(MODEM_FLIGHT, OUTPUT);
    digitalWrite(MODEM_FLIGHT, HIGH);    
   
    //Add CA Certificate
    secure_layer.setCACert(root_ca);
}

void light_sleep(uint32_t sec )
{
    esp_sleep_enable_timer_wakeup(sec * 1000000ULL);
    esp_light_sleep_start();
}

void loop()
{
    bool res ;

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    DBG("Initializing modem...");
    if (!modem.init()) {
      if (!modem.restart()) {
        DBG("Failed to restart modem, delaying 10s and retrying");
        // restart autobaud in case GSM just rebooted
        return;
      }
    }

#if TINY_GSM_TEST_GPRS
    /*  Preferred mode selection : AT+CNMP
          2 – Automatic
          13 – GSM Only
          14 – WCDMA Only
          38 – LTE Only
          59 – TDS-CDMA Only
          9 – CDMA Only
          10 – EVDO Only
          19 – GSM+WCDMA Only
          22 – CDMA+EVDO Only
          48 – Any but LTE
          60 – GSM+TDSCDMA Only
          63 – GSM+WCDMA+TDSCDMA Only
          67 – CDMA+EVDO+GSM+WCDMA+TDSCDMA Only
          39 – GSM+WCDMA+LTE Only
          51 – GSM+LTE Only
          54 – WCDMA+LTE Only
      */
    String ret;
    do {
        ret = modem.setNetworkMode(54);
        delay(500);
    } while (!ret);

    String name = modem.getModemName();
    DBG("Modem Name:", name);

    String modemInfo = modem.getModemInfo();
    DBG("Modem Info:", modemInfo);

    // Unlock your SIM card with a PIN if needed
    if (GSM_PIN && modem.getSimStatus() != 3) {
        modem.simUnlock(GSM_PIN);
    }

    DBG("Waiting for network...");
    if (!modem.waitForNetwork(600000L)) {
        light_sleep(10);
        return;
    }

    if (modem.isNetworkConnected()) {
        DBG("Network connected");
    }
#endif


#if TINY_GSM_TEST_GPRS
    DBG("Connecting to", apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        light_sleep(10);
        return;
    }

    res = modem.isGprsConnected();
    DBG("GPRS status:", res ? "connected" : "not connected");

    String ccid = modem.getSimCCID();
    DBG("CCID:", ccid);

    String imei = modem.getIMEI();
    DBG("IMEI:", imei);

    String imsi = modem.getIMSI();
    DBG("IMSI:", imsi);

    String cop = modem.getOperator();
    DBG("Operator:", cop);

      /*  Network mode : AT+CNSMOD
        0 – no service
        1 - GSM
        2 - GPRS
        3 - EGPRS (EDGE)
        4 - WCDMA
        5 - HSDPA only(WCDMA)
        6 - HSUPA only(WCDMA)
        7 - HSPA (HSDPA and HSUPA, WCDMA)
        8 - LTE
        9 - TDS-CDMA
        10 - TDS-HSDPA only
        11 - TDS- HSUPA only
        12 - TDS- HSPA (HSDPA and HSUPA)
        13 - CDMA
        14 - EVDO
        15 - HYBRID (CDMA and EVDO)
        16 - 1XLTE(CDMA and LTE)
        23 - eHRPD
        24 - HYBRID(CDMA and eHRPD)
    */
    modem.sendAT(GF("+CNSMOD?"));
    if (modem.waitResponse(GF(GSM_NL "+CNSMOD:")) != 1) { }
    int nmodec = modem.stream.readStringUntil(',').toInt() != 0;
    int nmode = modem.stream.readStringUntil('\n').toInt();
    modem.waitResponse();
    DBG("Network Mode:", nmode);

    IPAddress local = modem.localIP();
    DBG("Local IP:", local);

    int csq = modem.getSignalQuality();
    DBG("Signal quality:", csq);
#endif

#if TINY_GSM_TEST_TCP && defined TINY_GSM_MODEM_HAS_TCP
    
    // Retrieve Time
    String time;
      do {
        time = modem.getGSMDateTime(DATE_FULL).substring(0, 17);
        delay(100);
    } while (!time);
    DBG("Current Network Time:", time);

    // Retrieve Temperature
    float temp;
    do {
        temp = modem.getTemperature();
        delay(100);
    } while (!temp);
    DBG("Modem Temp:", temp);

    if (!modem.isGprsConnected()) {
        DBG("... not connected");
    } else {
        DBG("Connecting to ", server);
        // Make a HTTPS POST request:
        Serial.println("Making POST request securely");
        String contentType = "Content-Type: application/json";
        String postData = "{\"time\": \""+ time + "\", \"temp\": \"" + temp + "\"}";        
        client.post(resource, contentType, postData);
        int status_code = client.responseStatusCode();
        String response = client.responseBody();
        
        Serial.print("Status code: ");
        Serial.println(status_code);
        Serial.print("Response: ");
        Serial.println(response);

        client.stop();        
    }
#endif
    
#if TINY_GSM_TEST_GPRS
    modem.gprsDisconnect();
    light_sleep(5);
    if (!modem.isGprsConnected()) {
        DBG("GPRS disconnected");
    } else {
        DBG("GPRS disconnect: Failed.");
    }
#endif

#if TINY_GSM_POWERDOWN
    // Try to power-off (modem may decide to restart automatically)
    // To turn off modem completely, please use Reset/Enable pins
    modem.poweroff();
    DBG("Poweroff.");
#endif

    SerialMon.printf("End of tests. Enable deep sleep , Will wake up in %d seconds", TIME_TO_SLEEP);

    //Wait moden power off
    light_sleep(5);

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    delay(200);
    esp_deep_sleep_start();

    while (1);
}
