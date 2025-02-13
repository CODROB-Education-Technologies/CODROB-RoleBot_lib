#ifndef ROLEBOT_H
#define ROLEBOT_H

#if defined(ESP8266)

#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>

// PinS
#define RELAY_1 12
#define RELAY_2 13

#define B1_BUTTON_PIN 0
#define BLUE_LED 16

class ROLEBOT
{
public:
  ROLEBOT();
  void begin();
  void playIntro();

  /*********************************** Serial Port ***********************************
   */
  void serialStart(int baundrate);
  void serialWrite(const char *message);
  void serialWrite(String message);
  void serialWrite(long value);
  void serialWrite(int value);
  void serialWrite(float value);
  void serialWrite(bool value);

  /*********************************** BUTTONS ***********************************/
  bool button1Read();

  /*********************************** LED ***********************************/
  void ledWrite(bool status);

  /*********************************** Relays ***********************************
   */
  void Relay1Write(bool status);
  void Relay2Write(bool status);

  /*********************************** EEPROM  ***********************************
   */
  void eepromWriteInt(int address, int value);
  int eepromReadInt(int address);

  /*********************************** WiFi  ***********************************
   */
  void wifiStartAndConnect(const char *ssid, const char *pass);
  bool wifiConnectionControl();
  String wifiGetMACAddress();
  String wifiGetIPAddress();

  /*********************************** Server  ***********************************
   */
  void serverStart(const char *mode, const char *ssid, const char *password);
  void serverCreateLocalPage(const char *url, const char *WEBPageScript, const char *WEBPageCSS, const char *WEBPageHTML, size_t bufferSize = 1024);
  void serverHandleDNS();
  void serverContinue();

  /*********************************** Firebase Server  ***********************************
   */
  // 📡 Firebase Server Functions
  void fbServerSetandStartWithUser(const char *projectURL, const char *secretKey, const char *userMail, const char *mailPass); // projectURL: YOUR_FIREBASE_PROJECT_ID.firebaseio.com / secretKey: YOUR_FIREBASE_DATABASE_SECRET

  // 🔄 Firebase Database Write Functions
  void fbServerSetInt(const char *dataPath, int data);
  void fbServerSetFloat(const char *dataPath, float data);
  void fbServerSetString(const char *dataPath, String data);
  void fbServerSetDouble(const char *dataPath, double data);
  void fbServerSetBool(const char *dataPath, bool data);
  void fbServerSetJSON(const char *dataPath, String data);

  // 📥 Firebase Database Read Functions
  int fbServerGetInt(const char *dataPath);
  float fbServerGetFloat(const char *dataPath);
  String fbServerGetString(const char *dataPath);
  double fbServerGetDouble(const char *dataPath);
  bool fbServerGetBool(const char *dataPath);
  String fbServerGetJSON(const char *dataPath);

private:
  const IPAddress apIP = IPAddress(192, 168, 4, 1);                                // Sabit IP adresi tanımlanıyor / Define static IP address
  DNSServer dnsServer;                                                             // DNS sunucusu tanımlanıyor / Define DNS Server
  AsyncWebServer serverCODROB = AsyncWebServer(80);                                // Async Web Server nesnesi oluşturuluyor / Create an Async Web Server object
  AsyncWebSocket serverCODROBWebSocket = AsyncWebSocket("/serverCODROBWebSocket"); // WebSocket nesnesi tanımlanıyor / Define WebSocket object

  FirebaseData firebaseData;     // Data object to handle Firebase communication
  FirebaseAuth firebaseAuth;     // Authentication credentials for user verification
  FirebaseConfig firebaseConfig; // Configuration settings for Firebase
  char uid[128] = "";            // User ID storage
};

#else
#error "ROLEBOT sadece ESP8266 için desteklenmektedir."
#endif

#endif
