#include <ROLEBOT.h> // 📌 rolebot Kütüphanesi / rolebot Library

ROLEBOT rolebot; // 📌 rolebot Nesnesi / rolebot Object

// 📌 **Erişim Noktası (AP) Modu İçin Wi-Fi Bilgileri**
#define AP_SSID "CODROB Server" // 📌 AP Modu için SSID / AP Mode SSID
#define AP_PASS "12345678"      // 📌 AP Modu için Şifre / AP Mode Password (En az 8 karakter)

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

// 📌 **Kurulum Fonksiyonu (Setup)**
void setup()
{
  // 📌 **Seri Haberleşmeyi Başlat / Start Serial Communication**
  rolebot.serialStart(115200);

  // 📌 **ESP32'yi Erişim Noktası (AP) Olarak Başlat**
  rolebot.serverStart("AP", AP_SSID, AP_PASS);

  // 📌 **ESP32 Üzerinde Web Sayfasını Yayınla**
  rolebot.serverCreateLocalPage("demopage", WEBPageScript, WEBPageCSS, WEBPageHTML); // Cihaza Bağlanın ve linke gidin: 192.168.4.1/demo / Connect to device and goto link: 192.168.4.1/demo
}

// 📌 **Ana Döngü (Loop)**
void loop()
{
  rolebot.serverContinue(); // 📌 **AP modunda DNS yönlendirmeyi sürdür**
}
