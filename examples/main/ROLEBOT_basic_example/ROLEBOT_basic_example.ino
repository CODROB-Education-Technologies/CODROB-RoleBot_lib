#include <ROLEBOT.h> // ROLEBOT kütüphanesi / ROLEBOT library

// Create a ROLEBOT object / ROLEBOT nesnesi oluşturuluyor
ROLEBOT rolebot;

void setup()
{
  rolebot.begin();             // Initialize ROLEBOT / ROLEBOT başlatılıyor
  rolebot.playIntro();         // Play startup melody / Giriş müziği çalınıyor
  rolebot.serialStart(115200); // Start serial communication / Seri haberleşmeyi başlat

  rolebot.serialWrite("Welcome to ROLEBOT Test Firmware!"); // Display welcome message / Hoşgeldiniz mesajını göster
}

void loop()
{
  // Read button state / Buton durumunu oku
  if (rolebot.button1Read() == false)
  {
    rolebot.serialWrite("Button Pressed!"); // Print button pressed message / Butona basıldığını yazdır

    rolebot.ledWrite(HIGH); // Turn on LED / LED'i aç
    delay(200);
  }
  else
  {
    rolebot.serialWrite("Button Free"); // Print button released message / Butonun serbest olduğunu yazdır

    rolebot.ledWrite(LOW); // Turn off LED / LED'i kapat
    delay(200);
  }
}
