#define DATA
#include <init.h>
#include "DHT.h"
#include <SimpleKalmanFilter.h>
#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define PinLux 32
#define PinMQ 33

uint32_t prevReadSensor = 0;
extern Value_t data;

SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

Button_t btGas, btTemp, btLux;

extern bool modeAuto;
void initSensor()
{
    dht.begin();
    pinMode(LED_GAS, OUTPUT);
    pinMode(LED_LUX, OUTPUT);
    pinMode(LED_TEMP, OUTPUT);

    pinMode(BT_GAS, INPUT);
    pinMode(BT_LUX, INPUT);
    pinMode(BT_TEMP, INPUT);

    digitalWrite(LED_GAS, 0);
    digitalWrite(LED_LUX, 0);
    digitalWrite(LED_TEMP, 0);
}
float humi_s;
void readSensor()
{
    if (millis() - prevReadSensor >= 500)
    {
        prevReadSensor = millis();
        data.Humidity = dht.readHumidity();
        data.tempC = dht.readTemperature();
        if (!isnan(data.Humidity))
        {
            humi_s = data.Humidity;
        }
        if (isnan(data.Humidity))
        {
            data.Humidity = humi_s;
            Serial.println("Failed to read from DHT Humidity!");
        }
        if (isnan(data.tempC))
        {
            data.tempC = 0;
            Serial.println("Failed to read from DHT Humidity!");
        }
        data.Lux = map(analogRead(PinLux), 0, 4095, 100, 0);
        data.Gas = map(analogRead(PinMQ), 0, 4095, 0, 100);
        data.Lux = simpleKalmanFilter.updateEstimate(data.Lux);
    }
    if (modeAuto == 1)
    {
        checkData(CHECK_GAS, data.Gas, data.minGas, data.maxGas);
        checkData(CHECK_LUX, data.Lux, data.minLux, data.maxLux);
        checkData(CHECK_TEMPC, data.tempC, data.minTempC, data.maxTempC);
    }
    if (modeAuto == 0)
    {
        checkButton(&btGas, BT_GAS);
        checkButton(&btLux, BT_LUX);
        checkButton(&btTemp, BT_TEMP);
    }
}

void checkData(CHECK_t nameData, float data, float min, float max)
{
    uint32_t prevData;
    bool dataError;
    if (data < min || data > max)
    {
        dataError = true;
    }
    else
    {
        dataError = false;
    }
    if (millis() - prevData >= 200)
    {
        if (dataError == true)
        {
            if (nameData == CHECK_GAS)
            {
                digitalWrite(LED_GAS, !digitalRead(LED_GAS));
            }
            if (nameData == CHECK_LUX)
            {
                digitalWrite(LED_LUX, !digitalRead(LED_LUX));
            }
            if (nameData == CHECK_TEMPC)
            {
                digitalWrite(LED_TEMP, !digitalRead(LED_TEMP));
            }
        }
        else
        {
            if (nameData == CHECK_GAS)
            {
                digitalWrite(LED_GAS, 0);
            }
            if (nameData == CHECK_LUX)
            {
                digitalWrite(LED_LUX, 0);
            }
            if (nameData == CHECK_TEMPC)
            {
                digitalWrite(LED_TEMP, 0);
            }
        }
    }
}

void checkButton(Button_t *bt, uint16_t buttonMode)
{
    bt->currState = digitalRead(buttonMode);

    if (bt->prevState == HIGH && bt->currState == LOW) // press
    {
        bt->pressTime = millis();
    }
    else if (bt->prevState == LOW && bt->currState == HIGH) // release
    {
        bt->releaseTime = millis();
        if (bt->releaseTime - bt->pressTime > 50)
        {
            if (buttonMode == BT_GAS)
            {
                digitalWrite(LED_GAS, !digitalRead(LED_GAS));
            }
            else if (buttonMode == BT_LUX)
            {
                digitalWrite(LED_LUX, !digitalRead(LED_LUX));
            }
            else if (buttonMode == BT_TEMP)
            {
                digitalWrite(LED_TEMP, !digitalRead(LED_TEMP));
            }
        }
    }
    bt->prevState = bt->currState;
}