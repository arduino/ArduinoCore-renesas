#include "CEspControl.h"
#include <string>
#include <vector>

using namespace std;

/* GPIO_LOCAL C33 
   - 0. P010 
   - 1. P207
   - 2. P803 - D100 - DOWNLOAD - IO9 - DATA_READY - TEST POINT tra 2 e 3 
   - 3. P804 - D101 - RESET
   - 4. P806 - D102 - ACK      - IO5 - HANDSHAKE - ESP MDTI
   - 5. P504 
   - 6. P906 

   CHIP SELECT SPI
   P104 - D104
   
   */

//#define ENABLE_ESP_LOG

#define SerialESP      Serial5

void getESPDebugLog();
int displayMenu();
void makeRequest(int req);
string prompt(string p);


const char mac_address[] = "aa:bb:cc:dd:ee:ff";

bool wifiHwInitialized = false;


int initEvent(CCtrlMsgWrapper *resp) {
  if(resp->isInitEventReceived() == ESP_CONTROL_OK) {
    Serial.println("[EVENT !!!!]: Init event received");
    wifiHwInitialized = true;
  }
  return ESP_CONTROL_OK;
}

int stationDisconnectionEvent(CCtrlMsgWrapper *resp) {
  int reason;
  if(resp->isStationDisconnectionEventReceived(reason) == ESP_CONTROL_OK) {
    Serial.print("[EVENT !!!!]: C33 was disconnected from AP for reason ");
    Serial.println(reason);
  }
  return ESP_CONTROL_OK;
}

int disconnectionFromSofApEvent(CCtrlMsgWrapper *resp) {
  char MAC[18];
  if(resp->isDisconnectionFromSoftApReceived(MAC,18) == ESP_CONTROL_OK) {
    Serial.print("[EVENT !!!!]: the station with MAC address ");
    Serial.print(MAC);
    Serial.println(" disconnected from ESP32 Soft AP");
  }
  return ESP_CONTROL_OK;
}


int heartBeatEvent(CCtrlMsgWrapper *resp) {
  int hb;
  if(resp->isHeartBeatEventReceived(hb) == ESP_CONTROL_OK) {
    Serial.print("[EVENT !!!!]: heart beat n ");
    Serial.println(hb);
  }
  return ESP_CONTROL_OK;
}



/* -------------------------------------------------------------------------- */
/*                                   SETUP                                    */
/* -------------------------------------------------------------------------- */
void setup() {
  Serial.begin(115200);
  SerialESP.begin(115200);
  while(!Serial) {
    
  }
  CEspControl::getInstance().listenForInitEvent(initEvent);
  CEspControl::getInstance().listenForStationDisconnectEvent(stationDisconnectionEvent);
  CEspControl::getInstance().listenForDisconnectionFromSoftApEvent(disconnectionFromSofApEvent);
  

  int err = CEspControl::getInstance().initSpiDriver();
  if (err != 0) {
    Serial.print("Error initSpiDriver err ");
    Serial.println(err);
  } else {
    Serial.println("initSpiDriver OK");
  }
  while (!wifiHwInitialized) {
    CEspControl::getInstance().communicateWithEsp();
    delay(100);
  }

  Serial.println("STARTING PROGRAM");
}


/* -------------------------------------------------------------------------- */
/*                                   SETUP                                    */
/* -------------------------------------------------------------------------- */
void loop() {
  #ifdef ENABLE_ESP_LOG
  getESPDebugLog();
  #endif
  int request = displayMenu();
  makeRequest(request);  
}

/* ########################################################################## 
   #                      GLOBAL APPLICATION VARIABLES                      #
   ########################################################################## */

/* -------------------------------------------------------------------------- */
bool connected_to_access_point = false;
vector<AccessPoint_t> access_point_list;
WifiMode_t status_wifi_mode = WIFI_MODE_NONE;
/* -------------------------------------------------------------------------- */



/* ########################################################################## 
   #                           PRINTING FUNCTION                            #
   ########################################################################## */

/* -------------------------------------------------------------------------- */
void printResult(int res, const char* desc, const char *val) {
/* -------------------------------------------------------------------------- */  
  if(res == ESP_CONTROL_WRONG_REQUEST_INVALID_MSG) {
    Serial.println("*** [WARNING]: request NOT sent, invalid request");
  }
  else if(res == ESP_CONTROL_OK) {
    Serial.print("--- [OK]: ");
    if(desc != nullptr) {
      Serial.print(desc);
      Serial.print(" ");
    }
    if(val != nullptr)
      Serial.println(val);
  }
  else {
    Serial.print("!!! [ERROR]: ");
    if(desc != nullptr) {
      Serial.print(desc);
      Serial.print(" ");
    }
  }
}

/* -------------------------------------------------------------------------- */
void printIndex(unsigned int i, const char* desc) {
/* -------------------------------------------------------------------------- */  
  Serial.print(i);
  Serial.print(". ");
  Serial.print(desc);
  Serial.print(": ");
}

/* -------------------------------------------------------------------------- */
void printBufferAsString(uint8_t *b, bool ret) {
/* -------------------------------------------------------------------------- */  
  Serial.print((char *)b);
  if(ret) {
    Serial.println();
  }
}

/* -------------------------------------------------------------------------- */
void printBuffer(uint8_t *b, int len, bool ret) {
/* -------------------------------------------------------------------------- */  
  for(int i = 0; i < len; i++) {
    Serial.print(*(b + i));
  }
  if(ret) {
    Serial.println();
  }
}

/* -------------------------------------------------------------------------- */
void printAccessPointList(vector<AccessPoint_t>& list) {
/* -------------------------------------------------------------------------- */  
  for(unsigned int i = 0; i < list.size(); i++) {
      printIndex(i,"SSID");      
      printBufferAsString(list[i].ssid, true);
      printIndex(i,"BSSID");      
      printBufferAsString(list[i].bssid, true);
      printIndex(i,"RSSI");
      Serial.println(list[i].rssi);
      printIndex(i,"Channel");
      Serial.println(list[i].channel);
      printIndex(i,"Encryption mode:");
      Serial.println(list[i].encryption_mode);
      Serial.println();      
  }
}

/* -------------------------------------------------------------------------- */
void printAccessPointCfg(WifiApCfg_t &cfg) {
/* -------------------------------------------------------------------------- */
  Serial.print("SSID: ");  
  printBufferAsString(cfg.ssid, true);
  Serial.print("BSSID: ");  
  printBufferAsString(cfg.bssid, true);
  Serial.print("PSW: ");  
  printBufferAsString(cfg.pwd, true);
  Serial.print("status: ");  
  printBufferAsString((uint8_t *)cfg.status, true);
  Serial.print("MAC Address: ");  
  printBufferAsString((uint8_t *)cfg.out_mac, true);
  Serial.print("WPA3 supported: ");
  if(cfg.is_wpa3_supported) {
    Serial.println("Yes");
  }
  else {
    Serial.println("No");
  }
  Serial.print("RSSI: ");  
  Serial.println(cfg.rssi);
  Serial.print("Channel: ");  
  Serial.println(cfg.channel);
  Serial.print("Encryption Mode: ");  
  Serial.println(cfg.encryption_mode);
  Serial.print("Listen interval: ");  
  Serial.println(cfg.listen_interval);
  
}


/* -------------------------------------------------------------------------- */
void printSoftAccessPointCfg(SoftApCfg_t &cfg) {
/* -------------------------------------------------------------------------- */
  Serial.print("SSID: ");  
  printBufferAsString(cfg.ssid, true);
  Serial.print("PSW: ");  
  printBufferAsString(cfg.pwd, true);
  Serial.print("Channel: "); 
  Serial.println(cfg.channel); 
  
  Serial.print("Encryption mode: ");  
  Serial.println(cfg.encryption_mode);
  Serial.print("Max connections: ");  
  Serial.println(cfg.max_connections);
  Serial.print("SSID hidden: ");
  if(cfg.ssid_hidden) {
    Serial.println("YES");
  }
  else {
    Serial.println("No");
  }
  Serial.print("Bandwitdh: ");  
  Serial.println((int)cfg.bandwidth);
  Serial.print("MAC OUT: ");  
  Serial.println(cfg.out_mac);
  
}
/* -------------------------------------------------------------------------- */
void printConnectedStationList(vector<WifiConnectedSta_t>& list) {
/* -------------------------------------------------------------------------- */  
  for(unsigned int i = 0; i < list.size(); i++) {
      printIndex(i,"SSID");      
      printBufferAsString(list[i].bssid, true);
      printIndex(i,"RSSI");
      Serial.println(list[i].rssi);
      Serial.println();      
  }
  
}



/* ########################################################################## 
   #                              WIFI FUNCTIONS                            #
   ########################################################################## */

/* -------------------------------------------------------------------------- */
void connectAccessPoint() {
/* -------------------------------------------------------------------------- */  
  if(access_point_list.size() == 0) {
    Serial.println("***[WARNING]: Access point list is empty, please first read available access points");
  }
  else {
    printAccessPointList(access_point_list);
  
    string ap_str = prompt("Choose Access Point from the previous list : ");
    if(stoi(ap_str) < 0 || stoi(ap_str) >= access_point_list.size() ) {
      Serial.println("[ERROR]: Invalid choice");
      return;
    }

    string pwd = prompt("Enter the password: ");
    int i = stoi(ap_str);
    
    Serial.println(">>> [APP]: Connecting to Access Point");
    WifiApCfg_t ap;
    memcpy(ap.ssid,access_point_list[i].ssid,SSID_LENGTH);
    memcpy(ap.pwd,pwd.c_str(),pwd.size());
    memcpy(ap.bssid,access_point_list[i].bssid,BSSID_LENGTH);
    ap.channel = access_point_list[i].channel;
    ap.encryption_mode = access_point_list[i].encryption_mode;
   
    int res = CEspControl::getInstance().connectAccessPoint(ap);
    if(ESP_CONTROL_OK == res) {
      Serial.println("<<< [APP OK]: Connected to the selected access point, configuration");
      printAccessPointCfg(ap);   
      connected_to_access_point = true;
    }
    else {
      Serial.println("<<< [APP ERROR]: Unable to connect to selected access point");
    } 
  }
}

/* -------------------------------------------------------------------------- */
void getAccessPointCfg() {
/* -------------------------------------------------------------------------- */  
  if(!connected_to_access_point) {
    Serial.println("***[WARNING]: It seems that the device is currently not connected to any access point");
  }
  
    WifiApCfg_t ap_out;
    int res = CEspControl::getInstance().getAccessPointConfig(ap_out);
    if(ESP_CONTROL_OK == res) {
      Serial.println("<<< [APP OK]: Access point configuration is: ");
      printAccessPointCfg(ap_out);   
    }
    else {
      Serial.println("<<< [APP ERROR]: Unable to get access point configuration");
    } 
}



/* -------------------------------------------------------------------------- */
void disconnectAccessPoint() {
/* -------------------------------------------------------------------------- */  
  if(!connected_to_access_point) {
    Serial.println("***[WARNING]: It seems that the device is currently not connected to any access point");
  }
  
    
  int res = CEspControl::getInstance().disconnectAccessPoint();
  if(ESP_CONTROL_OK == res) {
    Serial.println("<<< [APP OK]: Disconnected from access point");
    connected_to_access_point = false;
  }
  else {
    Serial.println("<<< [APP ERROR]: Unable to disconnect from access point");
  } 
  
}




/* -------------------------------------------------------------------------- */
void getWifiMacAddress() {
/* -------------------------------------------------------------------------- */  
  WifiMac_t MAC;
  MAC.mode = status_wifi_mode;
  Serial.println(">>> [APP]: Sending request GET MAC ADDRESS");
  int res = CEspControl::getInstance().getWifiMacAddress(MAC);
  printResult(res, "get MAC Address", MAC.mac);
}


/* -------------------------------------------------------------------------- */
void setWifiMacAddress() {
/* -------------------------------------------------------------------------- */  
  string mac = prompt("Enter the new MAC address:");
  WifiMac_t MMM;
  MMM.mode = status_wifi_mode;
  memcpy(MMM.mac,mac.c_str(), mac.size());
  
  Serial.println(">>> [APP]: Sending request SET MAC ADDRESS");
  int res = CEspControl::getInstance().setWifiMacAddress(MMM);
  printResult(res, "set MAC Address", mac.c_str());
}

/* -------------------------------------------------------------------------- */
void getWifiMode() {
/* -------------------------------------------------------------------------- */  
  Serial.println(">>> [APP]: Sending request GET WIFI MODE");    
  int res = CEspControl::getInstance().getWifiMode(status_wifi_mode);
  printResult(res, "get Wifi Mode", to_string(status_wifi_mode).c_str());
}

/* -------------------------------------------------------------------------- */
void setWifiMode() {
/* -------------------------------------------------------------------------- */  
  string mode_str = prompt("Enter the new Wifi mode 1. none / 2.sta / 3.ap / 4.apsta: ");
  int mode = stoi(mode_str);
  Serial.print(">>> [APP]: Sending request SET WIFI MODE to ");
  Serial.print(mode);
  int res = CEspControl::getInstance().setWifiMode(WifiMode_t (mode - 1));
  if(res == ESP_CONTROL_OK) {
    status_wifi_mode = (WifiMode_t )mode;
  }
  printResult(res, "set Wifi Mode", to_string(status_wifi_mode).c_str());
  
}



/* -------------------------------------------------------------------------- */
void getAccessPointList() {
/* -------------------------------------------------------------------------- */  
  Serial.println(">>> [APP]: Sending request GET ACCESS POINT LIST");
  int res = CEspControl::getInstance().getAccessPointScanList(access_point_list);
  if(ESP_CONTROL_OK == res) {
    Serial.println("<<< [APP OK]: Available access points");
    printAccessPointList(access_point_list);
  }
  else {
    Serial.println("<<< [APP ERROR]: Unable to get access point list");
  } 
}

SoftApCfg_t cfg;

/* -------------------------------------------------------------------------- */
void startSoftAccessPoint() {
/* -------------------------------------------------------------------------- */  
  uint8_t mySSID[] = "TestAP";
  memset(cfg.ssid, 0x00, SSID_LENGTH);
  memcpy(cfg.ssid, mySSID, sizeof(mySSID));  
  uint8_t myPSW[] = "123456789";
  memset(cfg.pwd, 0x00, PASSWORD_LENGTH);
  memcpy(cfg.pwd, myPSW, sizeof(myPSW));
  cfg.channel = 1;
  cfg.encryption_mode =  WIFI_AUTH_WPA2_PSK;
  cfg.max_connections = 2;
  cfg.ssid_hidden = 0;
  cfg.bandwidth = WIFI_BW_HT20;
  
  Serial.println(">>> [APP]: Sending request START SOFT ACCESS POINT");
  int res = CEspControl::getInstance().startSoftAccessPoint(cfg);
  if(ESP_CONTROL_OK == res) {
    Serial.println("<<< [APP OK]: SOFT ACCESS POINT STARTED");
    Serial.print("     MAC ADDRESS: ");
    Serial.println((char*)cfg.out_mac);
  }
  else {
    Serial.println("<<< [APP ERROR]: START SOFT ACCESS POINT");
  } 
}

SoftApCfg_t cfg_1;


/* -------------------------------------------------------------------------- */
void getSoftAccessPointCfg() {
/* -------------------------------------------------------------------------- */  
  Serial.println(">>> [APP]: Sending request get SOFT ACCESS POINT CONFIGURATION");
  int res = CEspControl::getInstance().getSoftAccessPointConfig(cfg_1);
  if(ESP_CONTROL_OK == res) {
    Serial.println("<<< [APP OK]: GET SOFT ACCESS POINT CONFIGURATION");
    printSoftAccessPointCfg(cfg_1);
  }
  else {
    Serial.println("<<< [APP ERROR]: UNABLE TO GET ACCESS POINT CONFIGURATION");
  } 
}

/* -------------------------------------------------------------------------- */
void stopSoftAccessPoint() {
/* -------------------------------------------------------------------------- */  
  Serial.println(">>> [APP]: Sending request STOP SOFT ACCESS POINT");
  int res = CEspControl::getInstance().stopSoftAccessPoint();
  if(ESP_CONTROL_OK == res) {
    Serial.println("<<< [APP OK]: SOFT ACCESS POINT STOPPED");
  }
  else {
    Serial.println("<<< [APP ERROR]: UNABLE TO STOP ACCESS POINT");
  } 
}



vector<WifiConnectedSta_t> connected_stations;

/* -------------------------------------------------------------------------- */
void getConnectedStationList() {
/* -------------------------------------------------------------------------- */  
  Serial.println(">>> [APP]: Sending request GET CONNECTED STATION LIST");
  int res = CEspControl::getInstance().getSoftConnectedStationList(connected_stations);
  if(ESP_CONTROL_OK == res) {
    Serial.println("<<< [APP OK]: Connected Station list");
    printConnectedStationList(connected_stations);
  }
  else {
    Serial.println("<<< [APP ERROR]: UNABLE TO GET CONNECTED STATION LIST");
  } 
}

WifiVendorSoftApIe_t vendor_ie;



/* -------------------------------------------------------------------------- */
void setSoftApVendorIe() {
/* -------------------------------------------------------------------------- */  
  vendor_ie.enable = true;
  vendor_ie.type = WIFI_VND_IE_TYPE_BEACON;
  vendor_ie.idx = WIFI_VND_IE_ID_0;
  vendor_ie.vnd_ie.element_id = 0xDD;
  vendor_ie.vnd_ie.length = 4;
  vendor_ie.vnd_ie.vendor_oui[0] = 0x0A;
  vendor_ie.vnd_ie.vendor_oui[1] = 0x0B;
  vendor_ie.vnd_ie.vendor_oui[2] = 0x0C;
  vendor_ie.vnd_ie.vendor_oui_type = 0;
  vendor_ie.vnd_ie.payload_len = 0;
  vendor_ie.vnd_ie.payload = nullptr;
  
  
  Serial.println(">>> [APP]: Sending request SET SOFTAP VENDOR IE");
  int res = CEspControl::getInstance().setSoftAccessPointVndIe(vendor_ie);
  if(ESP_CONTROL_OK == res) {
    Serial.println("<<< [APP OK]: SOFTAP VENDOR IE SET");
    
  }
  else {
    Serial.println("<<< [APP ERROR]: UNABLE TO SET SOFTAP VENDOR IE");
  } 
}


void setUpHearthBeat() {
  HeartBeat_t hb;
  hb.enable = 1;
  hb.duration = 100;
  hb.hb_num = 0;
  Serial.println(">>> [APP]: Sending request Configure HEARTHBEAT");
  int res = CEspControl::getInstance().configureHeartbeat(hb, heartBeatEvent);
  if(ESP_CONTROL_OK == res) {
    Serial.println("<<< [APP OK]: HEARTHBEAT CONFIGURED");
  }
  else {
    Serial.println("<<< [APP ERROR]: FAILED TO CONFIGURE HEARTHBEAT");
  }
}

/* ########################################################################## 
   #                         APPLICATION FUNCTIONS                          #
   ########################################################################## */

/* -------------------------------------------------------------------------- */
string prompt(string p) {
/* -------------------------------------------------------------------------- */  
  Serial.println(p.c_str());
  string c;
  bool cmd_acquired = false;
  
  while(!cmd_acquired) {
    char ch;
    if(Serial.available()) {
      ch = Serial.read();
    
      if(ch == 0x0A) {
        cmd_acquired = true;
        return c;
      }
      else {
        c += ch;
      }
    }
    else {
      delay(100);
    }
  }
  return "";
}

/* -------------------------------------------------------------------------- */
void makeRequest(int req) {
/* -------------------------------------------------------------------------- */  
  switch(req) {
    case 1:
    getWifiMacAddress();
    break;
    case 2:
    setWifiMacAddress();
    break;
    case 3:
    getWifiMode();
    break;
    case 4:
    setWifiMode();
    break;
    case 5:
    getAccessPointList();
    break;
    case 6:
    connectAccessPoint();
    break;
    case 7:
    getAccessPointCfg();
    break;
    case 8:
    disconnectAccessPoint();
    break;
    case 9:
    startSoftAccessPoint();
    break;
    case 10:
    stopSoftAccessPoint();
    break;
    case 11:
    getConnectedStationList();
    break;
    case 12:
    getSoftAccessPointCfg();
    break;
    case 13:
    setSoftApVendorIe();
    break;
    case 14:

    break;
    case 15:

    break;
    case 16:

    break;
    case 17:

    break;
    case 18:

    break;
    case 19:

    break;
    case 21:
    setUpHearthBeat();
    break;
   
    default:
      Serial.println("[WARNING] Option not recognized!");
    break;

  }
}

/* -------------------------------------------------------------------------- */
int displayMenu() {
/* -------------------------------------------------------------------------- */
  Serial.println("-----------------------------------------------------------");
  Serial.println("*** ESP control test menu:");
  Serial.println("- MAC ADDRESS:");
  Serial.println("   1. Get Wifi MAC address");
  Serial.println("   2. Set Wifi MAC address");
  Serial.println("- Wifi Mode");
  Serial.println("   3. Get Wifi Mode");
  Serial.println("   4. Set Wifi Mode");
  Serial.println("- Access point:");
  Serial.println("   5. Get Access Points list");
  Serial.println("   6. Connect to Access Point");
  Serial.println("   7. Get Access Point Configuration");
  Serial.println("   8. Disconnect Access Point");
  Serial.println("- SoftAP");
  Serial.println("   9. Start SoftAP");
  Serial.println("  10. Stop SoftAP");
  Serial.println("  11. Get Connected Station List");
  Serial.println("  12. Get SoftAP Configuration");
  Serial.println("  13. Set SoftAP Vendor Configuration");
  Serial.println("- Power Save:");
  Serial.println("  14. Get Power Save mode");
  Serial.println("  15. Set Power Save mode");
  Serial.println("- OTA:");
  Serial.println("  16. Start OTA");
  Serial.println("  17. End OTA");
  Serial.println("  18. Write OTA");
  Serial.println("- Power:");
  Serial.println("  19. Get Wifi current power");
  Serial.println("  20. Set Max Wifi power");
  Serial.println("- Heartbeat:");
  Serial.println("  21. Configure hearbeat");
  Serial.println();
  Serial.println("Choose the option number:>");
  Serial.println("-----------------------------------------------------------");
  
  string c;
  bool cmd_acquired = false;
  int rv = -1;
  while(!cmd_acquired) {
    char ch;
    if(Serial.available()) {
      ch = Serial.read();
      if(ch == 0x0A) {
        cmd_acquired = true;
        rv = stoi(c);
      }
      else if(ch >= '0' && ch <='9') {
        c += ch;
      }
    }
    else {
      CEspControl::getInstance().communicateWithEsp();
    }
  }
  return rv;
}

void getESPDebugLog() {
  while(SerialESP.available()) {
     Serial.write(SerialESP.read());
  }
}
