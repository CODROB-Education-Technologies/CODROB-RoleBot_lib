#include "ROLEBOT.h"

ROLEBOT::ROLEBOT()
{
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