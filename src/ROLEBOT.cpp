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
  rolebot.serialStart(115200);
  rolebot.serialWrite("Hi from ROLEBOT!");
  rolebot.ledWrite(HIGH);
  delay(100);
  rolebot.ledWrite(LOW);
  delay(100);
  rolebot.ledWrite(HIGH);
  delay(100);
  rolebot.ledWrite(LOW);
  delay(100);
  rolebot.ledWrite(HIGH);
  delay(100);
  rolebot.ledWrite(LOW);
  delay(100);
}

/*********************************** Serial Port ***********************************
 */
void ROLEBOT::serialStart(int baundrate)
{
  Serial.begin(baundrate);
}

void ROLEBOT::serialWrite(const char *message)
{
  Serial.println(message);
}

void ROLEBOT::serialWrite(int value)
{
  Serial.println(String(value).c_str());
}

void ROLEBOT::serialWrite(float value)
{
  Serial.println(String(value).c_str());
}

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