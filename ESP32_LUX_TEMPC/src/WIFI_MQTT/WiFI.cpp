#define DATA

#include <init.h>
#include <WiFi.h>
#define SSID "NGOC XUAN"
#define PASS "01021967"
uint32_t prevReconnectWiFiTime;
extern bool modeAuto;
void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASS);
    Serial.print("Connecting WIFI !!");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }
}
void checkConnectWiFi()
{
    showLCD();
    if (WiFi.status() == WL_CONNECTED)
    {
        checkConnectMQTT();
    }
    else if ((WiFi.status() != WL_CONNECTED) && (millis() - prevReconnectWiFiTime > 1000))
    {
        modeAuto = 0;
        Serial.println("Reconnect WiFi !!!");
        WiFi.disconnect();
        WiFi.reconnect();
        prevReconnectWiFiTime = millis();
    }
    readSensor();
}