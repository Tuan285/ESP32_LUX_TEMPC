#ifndef INIT
#define INIT
#include <WiFi.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED_TEMP 4
#define LED_LUX 0
#define LED_GAS 2

#define BT_TEMP 36
#define BT_LUX 39
#define BT_GAS 34

typedef struct
{
    bool prevState;
    bool currState;
    uint32_t pressTime;
    uint32_t releaseTime;

} Button_t;

typedef struct
{
    bool Auto;
    bool Light;
    uint16_t Gas;
    uint16_t minGas;
    uint16_t maxGas;
    uint16_t Lux;
    uint16_t minLux;
    uint16_t maxLux;
    uint16_t minTempC;
    uint16_t maxTempC;
    uint16_t minHumidity;
    uint16_t maxHumidity;
    float Humidity;
    float tempC;
} Value_t;
typedef enum
{
    CHECK_LUX,
    CHECK_TEMPC,
    CHECK_GAS
} CHECK_t;

//--------------- LCD ------------
void initLCD();
void showLCD();
void LCDAuto();
//--------------- WIFI ------------
void initWiFi();
void checkConnectWiFi();
//--------------- MQTT ------------
void initMQTT();
boolean reconnect();
void checkConnectMQTT();
//--------------- SENSOR ------------
void initSensor();
void readSensor();
void checkData(CHECK_t nameData, float data, float min, float max);
void checkButton(Button_t *bt, uint16_t buttonMode);
#endif