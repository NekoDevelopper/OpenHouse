#include <Arduino.h>
#include "Modules/Ethernet/OpenHouseNet.h"
#include "Modules/Mqtt/OpenHouseMqtt.h"
#include <DHT.h>

byte ethernetMac[6] = {0x5A,0x12,0xF0,0xAF,0xAD,0x71};
char MqttServerAdress[] = "192.168.1.14";
char MqttClientId[] = "Test";
int MqttServerPort = 1883;

DHT dht(3, 22);
OHMqttSender HumiditySender((char*)"/Test/Humidity");
OHMqttSender TemperatureSender((char*)"/Test/Temperature");

void RelayFunc(char *message){digitalWrite(2,atoi(message));}
OHMqttHandler Relay((char*)"/Test/Relay",&RelayFunc);

void setup()
{
    Serial.begin(9600);
    dht.begin();
    pinMode(2, OUTPUT);
    OHMqtt.Begin(MqttServerAdress, &MqttServerPort, MqttClientId);
    OHNet.Begin(ethernetMac);
}

unsigned long int millissss = 0;
float Temperature;
float Humidity;

void loop()
{
    OHNet.Loop();
    OHMqtt.Loop();

    if (millis() - millissss >= 5000)
    {
        Temperature = dht.readTemperature();
        Humidity = dht.readHumidity();
        char buf[20];
        dtostrf(Temperature, 3, 3, buf);
        TemperatureSender.send(buf);
        dtostrf(Humidity, 3, 3, buf);
        HumiditySender.send(buf);
        millissss = millis();
    }
    else if (millis() - millissss < 0)
    {
        millissss = millis();
    }
}
