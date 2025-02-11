#include <ROLEBOT.h> // 📌 ROLEBOT Kütüphanesi / ROLEBOT Library

ROLEBOT rolebot; // 📌 ROLEBOT Nesnesi / ROLEBOT Object

// 📌 **Web Sayfası İçeriği (HTML, CSS, JavaScript)**

// ✅ **JavaScript (Web Sayfası İçin)**
// Kullanıcı butona tıkladığında bir mesaj gösterecek.
const char WEBPageScript[] PROGMEM = R"rawliteral(
<script>
  function sayHello() {
    alert("Merhaba ROLEBOT!");
  }
</script>
)rawliteral";

// ✅ **CSS (Web Sayfası Stili)**
// Web sayfasının görünümünü ayarlamak için CSS kullanıyoruz.
const char WEBPageCSS[] PROGMEM = R"rawliteral(
<style>
  body { text-align: center; font-family: Arial, sans-serif; }
  button { font-size: 20px; padding: 10px; margin: 20px; }
</style>
)rawliteral";

// ✅ **HTML (Web Sayfası İçeriği)**
// ESP32 tarafından sunulacak HTML kodu
const char WEBPageHTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
  <meta charset="UTF-8">
  <title>ROLEBOT Web Server</title>
  %s <!-- CSS Dosyası Buraya Eklenir -->
  %s <!-- JavaScript Dosyası Buraya Eklenir -->
</head>
<body>
  <h1>ROLEBOT Web Sayfası</h1>
  <button onclick="sayHello()">Tıklayın</button>
</body>
</html>
)rawliteral";

// 📌 **Kullanıcı için Kolaylaştırılmış Wi-Fi Tanımlamaları**
const char WIFI_SSID = "INTERNET";      // 📌 Wi-Fi Ağ Adı (SSID) / WiFi Network Name (SSID)
const char WIFI_PASS = "INTERNET_2022"; // 📌 Wi-Fi Şifresi / WiFi Password

const char AP_SSID = "CODROB Server"; // 📌 AP Modu için SSID / AP Mode SSID
const char AP_PASS = "12345678";      // 📌 AP Modu için Şifre / AP Mode Password (En az 8 karakter)

// 📌 **Kurulum Fonksiyonu (Setup)**
void setup()
{
  rolebot.serialStart(115200); // 📌 **Seri Haberleşmeyi Başlat / Start Serial Communication**

  // 📌 **STA veya AP Modunda Bağlantı Kur**
  rolebot.serverStart("STA", WIFI_SSID, WIFI_PASS);

  // 📌 **Eğer STA bağlantısı başarısız olursa AP moduna geç**
  if (!rolebot.wifiConnectionControl())
  {
    rolebot.serverStart("AP", AP_SSID, AP_PASS);
  }

  // 📌 **ESP32 Üzerinde Web Sayfasını Yayınla**
  rolebot.serverCreateLocalPage("demo", WEBPageScript, WEBPageCSS, WEBPageHTML); // Cihaza Bağlanın ve linke gidin: 192.168.4.1/demo / Connect to device and goto link: 192.168.4.1/demo
}

// 📌 **Ana Döngü (Loop)**
void loop()
{
  rolebot.serverContinue(); // 📌 **AP modunda DNS yönlendirmeyi sürdür**
}
