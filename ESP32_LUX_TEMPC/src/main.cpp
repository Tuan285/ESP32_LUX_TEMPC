#include <Arduino.h>
#include <init.h>

void setup()
{
  Serial.begin(115200);
  initLCD();
  initWiFi();
  initMQTT();
  initSensor();
}

void loop()
{
  checkConnectWiFi();
}
