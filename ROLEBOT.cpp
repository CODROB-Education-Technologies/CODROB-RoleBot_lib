#include "ROLEBOT.h"

ROLEBOT::ROLEBOT()
{
#if defined(USE_SERVER)
  serverCODROBWebSocket = new AsyncWebSocket("/serverCODROBWebSocket");
#endif
}

void ROLEBOT::begin()
{
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(B1_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BLUE_LED, OUTPUT);
}

void ROLEBOT::playIntro()
{
  digitalWrite(BLUE_LED, HIGH);
  delay(100);
  digitalWrite(BLUE_LED, LOW);
  delay(100);
  digitalWrite(BLUE_LED, HIGH);
  delay(100);
  digitalWrite(BLUE_LED, LOW);
  delay(100);
  digitalWrite(BLUE_LED, HIGH);
  delay(100);
  digitalWrite(BLUE_LED, LOW);
  delay(100);
}

/*********************************** Serial Port ***********************************
 */

void ROLEBOT::serialStart(int baudrate)
{
  Serial.begin(baudrate);
}

// Overloaded function for const char* / `const char*` için fonksiyon
void ROLEBOT::serialWrite(const char *message)
{
  Serial.println(message);
}

// Overloaded function for String / `String` için özel fonksiyon
void ROLEBOT::serialWrite(String message)
{
  Serial.println(message.c_str()); // Convert String to const char*
}

// Overloaded function for long / `long` için özel fonksiyon
void ROLEBOT::serialWrite(long value)
{
  Serial.println(String(value).c_str());
}

// Overloaded function for int / `int` için fonksiyon
void ROLEBOT::serialWrite(int value)
{
  Serial.println(String(value).c_str());
}

// Overloaded function for float / `float` için fonksiyon
void ROLEBOT::serialWrite(float value)
{
  Serial.println(String(value).c_str());
}

// Overloaded function for bool / `bool` için fonksiyon
void ROLEBOT::serialWrite(bool value)
{
  Serial.println(value ? "true" : "false");
}

/*********************************** BUTTONS ***********************************
 */
bool ROLEBOT::button1Read()
{
  return digitalRead(B1_BUTTON_PIN);
}

/*********************************** LED ***********************************
 */
void ROLEBOT::ledWrite(bool status)
{
  return digitalWrite(BLUE_LED, status);
}

/*********************************** Relays  ***********************************
 */
void ROLEBOT::Relay1Write(bool status)
{
  digitalWrite(RELAY_1, status);
}

void ROLEBOT::Relay2Write(bool status)
{
  digitalWrite(RELAY_2, status);
}

/*********************************** EEPROM  ***********************************
 */
void ROLEBOT::eepromWriteInt(int address, int value) // EEPROM'a güvenli bir şekilde int türünde veri yazmak için fonksiyon
{
  byte highByte = highByte(value); // int'in yüksek baytını al
  byte lowByte = lowByte(value);   // int'in düşük baytını al

  EEPROM.write(address, highByte);    // İlk baytı EEPROM'a yaz
  EEPROM.write(address + 1, lowByte); // İkinci baytı EEPROM'a yaz
  EEPROM.commit();                    // Değişiklikleri kaydetmek için commit işlemi yapılmalıdır
}

int ROLEBOT::eepromReadInt(int address) // EEPROM'dan int türünde veri okumak için fonksiyon
{
  byte highByte = EEPROM.read(address);    // İlk baytı oku
  byte lowByte = EEPROM.read(address + 1); // İkinci baytı oku
  return word(highByte, lowByte);          // Yüksek ve düşük baytları birleştirerek int değeri oluştur
}

/*********************************** WiFi ***********************************/
#if defined(USE_WIFI)

void ROLEBOT::wifiStartAndConnect(const char *ssid, const char *pass)
{
  Serial.printf("[WiFi]: Connection Starting!\r\n[WiFi]: SSID: %s\r\n[WiFi]: Pass: %s\r\n", ssid, pass);

  WiFi.begin(ssid, pass);
  int count = 0;
  while (count < 30)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.printf("\n[WiFi]: Connected!\r\n[WiFi]: Local IP: %s\r\n", WiFi.localIP().toString().c_str());
      Serial.printf("[WiFi]: MAC Address: %s\r\n", WiFi.macAddress().c_str());
      return;
    }
    Serial.print(".");
    delay(500);
    count++;
  }
  Serial.println("[WiFi]: Connection Timeout!");
}

bool ROLEBOT::wifiConnectionControl()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("[WiFi]: Connection OK!");
    return true;
  }
  else
  {
    Serial.println("[WiFi]: Connection ERROR!");
    return false;
  }
}

String ROLEBOT::wifiGetMACAddress()
{
  return WiFi.macAddress();
}

String ROLEBOT::wifiGetIPAddress()
{
  return WiFi.localIP().toString();
}
#endif

/*********************************** Server ***********************************/
#if defined(USE_SERVER)

void ROLEBOT::serverStart(const char *mode, const char *ssid, const char *password)
{
  if (strcmp(mode, "STA") == 0)
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.printf("\n[STA Mode]: Connecting to WiFi: %s\n", ssid);

    int retries = 30;
    while (WiFi.status() != WL_CONNECTED && retries > 0)
    {
      delay(1000);
      Serial.print(".");
      retries--;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\n[STA Mode]: Connected!");
      Serial.printf("[STA Mode]: IP Address: http://%s\n", WiFi.localIP().toString().c_str());
    }
    else
    {
      Serial.println("\n[STA Mode]: Connection Failed! Switching to AP Mode...");
      serverStart("AP", ssid, password);
      return;
    }
  }
  else if (strcmp(mode, "AP") == 0)
  {
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
    dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));

    Serial.printf("\n[AP Mode]: Access Point Started!\n");
    Serial.printf("[AP Mode]: SSID: \"%s\"\n", ssid);
    Serial.printf("[AP Mode]: Password: \"%s\"\n", password);
    Serial.printf("[AP Mode]: AP IP Address: http://%s\n", WiFi.softAPIP().toString().c_str());
  }

  // 📌 Sayfaları tanımla
  serverCODROB.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
      Serial.println("[Local Server]: Root URL Accessed!");
      request->send(200, "text/plain", "CODROB Server is Running!"); });

  // 📌 404 Hatası
  serverCODROB.onNotFound([](AsyncWebServerRequest *request)
                          {
      Serial.println("[Local Server]: Received an Unknown Request!");
      request->send(404, "text/plain", "Not Found"); });

  // 📌 **WebSocket Olaylarını Bağla**
  serverCODROBWebSocket->onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
                                 {
      if (type == WS_EVT_CONNECT) {
          Serial.println("WebSocket Client Connected");
      } else if (type == WS_EVT_DISCONNECT) {
          Serial.println("WebSocket Client Disconnected");
      } });

  // 📌 WebSocket'i Sunucuya Bağla
  serverCODROB.addHandler(serverCODROBWebSocket);

  // 📌 **En son sunucuyu başlat!**
  serverCODROB.begin();
  Serial.println("[Local Server]: Server Started! ✅");
}

void ROLEBOT::serverCreateLocalPage(const char *url, const char *WEBPageScript, const char *WEBPageCSS, const char *WEBPageHTML, size_t bufferSize)
{
  // 📌 Sayfa içeriğini oluştur
  serverCODROB.on(("/" + String(url)).c_str(), HTTP_GET, [WEBPageScript, WEBPageCSS, WEBPageHTML, bufferSize](AsyncWebServerRequest *request)
                  {
                    // Buffer boyutu kullanıcının belirttiği veya varsayılan değerle tanımlanır
                    char *buffer = new char[bufferSize];
                    int len = snprintf(buffer, bufferSize, WEBPageHTML, WEBPageScript, WEBPageCSS);

                    if (len >= bufferSize)
                    {
                      Serial.println("[ERROR]: Buffer size insufficient, content truncated!");
                    }

                    request->send(200, "text/html", buffer);
                    delete[] buffer; // Dinamik olarak ayrılan belleği serbest bırakın
                  });

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.printf("[Local Server]: Page created at: http://%s/%s\n", WiFi.localIP().toString().c_str(), url);
  }
  else
  {
    Serial.printf("[Local Server]: Page created at: http://%s/%s\n", apIP.toString().c_str(), url);
  }
}

void ROLEBOT::serverHandleDNS()
{
  dnsServer.processNextRequest();
}

void ROLEBOT::serverContinue()
{
  if (WiFi.getMode() == WIFI_AP)
  {
    serverHandleDNS();
  }
}

/*********************************** Firebase Server Functions ***********************************/
#if defined(USE_FIREBASE)

// Initialize Firebase connection with SignUp Authentication
void ROLEBOT::fbServerSetandStartWithUser(const char *projectURL, const char *secretKey, const char *userMail, const char *mailPass)
{
  firebaseData.setResponseSize(1024); // Optimize memory usage

  // Firebase Configuration Settings
  firebaseConfig.api_key = secretKey;
  firebaseConfig.database_url = projectURL;
  firebaseAuth.user.email = userMail;
  firebaseAuth.user.password = mailPass;

  // Zaman aşımı ayarları
  firebaseConfig.timeout.socketConnection = 10 * 1000; // 10 saniye bağlantı zaman aşımı

  // Token durumu izleme ayarı
  // firebaseConfig.token_status_callback = tokenStatusCallback;
  firebaseConfig.max_token_generation_retry = 5; // Daha fazla token yenileme denemesi

  // Wi-Fi bağlantısı kaybolduğunda otomatik yeniden bağlanma
  Firebase.reconnectWiFi(true);

  // Firebase başlat
  Firebase.begin(&firebaseConfig, &firebaseAuth);

  Serial.println("[Firebase]: Verifying user credentials...");
  uint8_t id_count = 0;
  while (firebaseAuth.token.uid == "" && id_count < 50)
  {
    Serial.print('.');
    delay(500);
    id_count++;
  }
  if (firebaseAuth.token.uid == "")
  {
    Serial.println("\n[ERROR]: Authentication timeout.");
  }
  else
  {
    if (Firebase.ready())
    {
      strncpy(uid, firebaseAuth.token.uid.c_str(), 128 - 1); // UID'yi kopyala ve taşma kontrolü yap
      uid[128 - 1] = '\0';                                   // Diziyi null karakter ile sonlandır
      Serial.print("\n[Info]: Doğrulanan Kimlik ID: ");
      Serial.println(uid);
    }
    else
    {
      Serial.print("[ERROR]: Sign-up failed. Reason: ");
      Serial.println(firebaseData.errorReason());
    }
  }
}

/*********************************** Firebase Write Functions ***********************************/

void ROLEBOT::fbServerSetInt(const char *dataPath, int data)
{
  // Corrected function call
  if (Firebase.RTDB.setInt(&firebaseData, dataPath, data))
  {
    Serial.println("[SUCCESS]: Integer data sent successfully!");
  }
  else
  {
    Serial.print("[ERROR]: Failed to send integer data. ");
    Serial.printf("HTTP Code: %d\n", firebaseData.httpCode());
    Serial.println("Reason: " + firebaseData.errorReason());
  }
}

void ROLEBOT::fbServerSetFloat(const char *dataPath, float data)
{
  if (Firebase.RTDB.setFloat(&firebaseData, dataPath, data))
  {
    Serial.println("[SUCCESS]: Float data sent successfully!");
  }
  else
  {
    Serial.print("[ERROR]: Failed to send float data. ");
    Serial.printf("HTTP Code: %d\n", firebaseData.httpCode());
    Serial.println("Reason: " + firebaseData.errorReason());
  }
}

void ROLEBOT::fbServerSetString(const char *dataPath, String data)
{
  if (Firebase.RTDB.setString(&firebaseData, dataPath, data))
  {
    Serial.println("[SUCCESS]: String data sent successfully!");
  }
  else
  {
    Serial.print("[ERROR]: Failed to send string data. ");
    Serial.printf("HTTP Code: %d\n", firebaseData.httpCode());
    Serial.println("Reason: " + firebaseData.errorReason());
  }
}

void ROLEBOT::fbServerSetDouble(const char *dataPath, double data)
{
  if (Firebase.RTDB.setDouble(&firebaseData, dataPath, data))
  {
    Serial.println("[SUCCESS]: Double data sent successfully!");
  }
  else
  {
    Serial.print("[ERROR]: Failed to send double data. ");
    Serial.printf("HTTP Code: %d\n", firebaseData.httpCode());
    Serial.println("Reason: " + firebaseData.errorReason());
  }
}

void ROLEBOT::fbServerSetBool(const char *dataPath, bool data)
{
  if (Firebase.RTDB.setBool(&firebaseData, dataPath, data))
  {
    Serial.println("[SUCCESS]: Boolean data sent successfully!");
  }
  else
  {
    Serial.print("[ERROR]: Failed to send boolean data. ");
    Serial.printf("HTTP Code: %d\n", firebaseData.httpCode());
    Serial.println("Reason: " + firebaseData.errorReason());
  }
}

void ROLEBOT::fbServerSetJSON(const char *dataPath, String data)
{
  FirebaseJson json;
  json.set(dataPath, data);

  if (Firebase.RTDB.setJSON(&firebaseData, dataPath, &json))
  {
    Serial.println("[SUCCESS]: JSON data sent successfully!");
  }
  else
  {
    Serial.print("[ERROR]: Failed to send JSON data. ");
    Serial.printf("HTTP Code: %d\n", firebaseData.httpCode());
    Serial.println("Reason: " + firebaseData.errorReason());
  }
}
#endif

/*********************************** Firebase Read Functions ***********************************/

int ROLEBOT::fbServerGetInt(const char *dataPath)
{
  if (Firebase.RTDB.getInt(&firebaseData, dataPath))
  {
    Serial.println("[SUCCESS]: Integer data retrieved successfully!");
    return firebaseData.intData();
  }
  Serial.println("[ERROR]: Failed to retrieve integer data.");
  return -1;
}

float ROLEBOT::fbServerGetFloat(const char *dataPath)
{
  if (Firebase.RTDB.getFloat(&firebaseData, dataPath))
  {
    Serial.println("[SUCCESS]: Float data retrieved successfully!");
    return firebaseData.floatData();
  }
  Serial.println("[ERROR]: Failed to retrieve float data.");
  return -1.0;
}

String ROLEBOT::fbServerGetString(const char *dataPath)
{
  if (Firebase.RTDB.getString(&firebaseData, dataPath))
  {
    Serial.println("[SUCCESS]: String data retrieved successfully!");
    return firebaseData.stringData();
  }
  Serial.println("[ERROR]: Failed to retrieve string data.");
  return "";
}

double ROLEBOT::fbServerGetDouble(const char *dataPath)
{
  if (Firebase.RTDB.getDouble(&firebaseData, dataPath))
  {
    Serial.println("[SUCCESS]: Double data retrieved successfully!");
    return firebaseData.doubleData();
  }
  Serial.println("[ERROR]: Failed to retrieve double data.");
  return -1.0;
}

bool ROLEBOT::fbServerGetBool(const char *dataPath)
{
  if (Firebase.RTDB.getBool(&firebaseData, dataPath))
  {
    Serial.println("[SUCCESS]: Boolean data retrieved successfully!");
    return firebaseData.boolData();
  }
  Serial.println("[ERROR]: Failed to retrieve boolean data.");
  return false;
}

String ROLEBOT::fbServerGetJSON(const char *dataPath)
{
  if (Firebase.RTDB.getJSON(&firebaseData, dataPath))
  {
    Serial.println("[SUCCESS]: JSON data retrieved successfully!");
    return firebaseData.jsonString();
  }
  Serial.println("[ERROR]: Failed to retrieve JSON data.");
  return "{}";
}
#endif
