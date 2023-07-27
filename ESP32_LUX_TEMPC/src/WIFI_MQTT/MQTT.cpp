#define DATA
#include <init.h>
#include <WiFi.h>
#include <PubSubClient.h>
WiFiClient ESP32client;
PubSubClient client(ESP32client);

#define SERVER_MQTT "test.mosquitto.org"
#define PORT 1883

// ------------------ MY TOPIC --------------
#define HUMIDITY "HUMIDITY"
#define GAS "GAS"
#define TEMPC "TEMPC"
#define LUX "LUX"
#define AUTO "AUTO"
#define LIGHT "LIGHT"

#define MINTEMPC "MINTEMPC"
#define MAXTEMPC "MAXTEMPC"

#define MINLUX "MINLUX"
#define MAXLUX "MAXLUX"

#define MINGAS "MINGAS"
#define MAXGAS "MAXGAS"

#define MINHUMI "MINHUMI"
#define MAXHUMI "MAXHUMI"
// ------------------- END TOPIC ------------
uint64_t lastReconnectAttempt = 0;
uint64_t prevPubData = 0;
Value_t data;
bool modeAuto;
bool LCDclear;
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String dataRecive;
    for (int i = 0; i < length; i++)
    {
        dataRecive += (char)payload[i];
        Serial.print((char)payload[i]);
    }
    Serial.println();
    LCDclear = true;
    if (String(topic) == AUTO)
    {
        data.Auto = dataRecive.toInt();
        if (data.Auto == 1)
        {
            modeAuto = 1;
        }
        else if (data.Auto == 0)
        {
            digitalWrite(LED_GAS, 0);
            digitalWrite(LED_LUX, 0);
            digitalWrite(LED_TEMP, 0);
            modeAuto = 0;
        }
    }
    else if (String(topic) == LIGHT)
    {
        data.Light = dataRecive.toInt();
    }
    else if (String(topic) == MINTEMPC)
    {
        data.minTempC = dataRecive.toInt();
    }
    else if (String(topic) == MAXTEMPC)
    {
        data.maxTempC = dataRecive.toInt();
    }
    else if (String(topic) == MINLUX)
    {
        data.minLux = dataRecive.toInt();
    }
    else if (String(topic) == MAXLUX)
    {
        data.maxLux = dataRecive.toInt();
    }
    else if (String(topic) == MINGAS)
    {
        data.minGas = dataRecive.toInt();
    }
    else if (String(topic) == MAXGAS)
    {
        data.maxGas = dataRecive.toInt();
    }
    else if (String(topic) == MINHUMI)
    {
        data.minHumidity = dataRecive.toInt();
    }
    else if (String(topic) == MAXHUMI)
    {
        data.maxHumidity = dataRecive.toInt();
    }
}

void initMQTT()
{
    client.setServer(SERVER_MQTT, PORT);
    client.setCallback(callback);
    lastReconnectAttempt = 0;
}

boolean reconnect()
{
    String clientId = "ESP32";
    clientId += String(random(0xffff), HEX);
    Serial.print("\nAttempting MQTT connection...");
    if (client.connect(clientId.c_str()))
    {
        client.subscribe(MINGAS);
        client.subscribe(MAXGAS);

        client.subscribe(MINHUMI);
        client.subscribe(MAXHUMI);

        client.subscribe(MINLUX);
        client.subscribe(MAXLUX);

        client.subscribe(MINTEMPC);
        client.subscribe(MAXTEMPC);

        client.subscribe(AUTO);
        // client.subscribe(LIGHT);
        Serial.println("Subscribed !!!");
    }
    return client.connected();
}
void checkConnectMQTT()
{
    if (!client.connected())
    {
        long now = millis();
        if (now - lastReconnectAttempt > 3000)
        {
            lastReconnectAttempt = now;
            if (reconnect())
            {
                lastReconnectAttempt = 0;
            }
        }
    }
    else
    {
        // Client connected
        client.loop();
        if (millis() - prevPubData >= 2000)
        {
            prevPubData = millis();
            Serial.print("GAS :");
            Serial.println(data.Gas);
            Serial.print("TEMPC :");
            Serial.println(data.tempC);
            Serial.print("HUMIDITY :");
            Serial.println(data.Humidity);
            Serial.print("LUX :");
            Serial.println(data.Lux);

            client.publish(GAS, String(data.Gas).c_str());
            client.publish(TEMPC, String(data.tempC).c_str());
            client.publish(HUMIDITY, String(data.Humidity).c_str());
            client.publish(LUX, String(data.Lux).c_str());
        }
    }
}
