#include <Arduino.h>
#include "Modules/Ethernet/OpenHouseNet.h"
#include "Modules/Mqtt/Devices.h"
#include <DHT.h>

OpenHouseNet Network;
byte ethernetMac[6] = {0x5A,0x12,0xF0,0xAF,0xAD,0x71};

EthernetClient MqttEthernetClient;
char MqttServerAdress[] = "192.168.1.14";
char MqttClientId[] = "Test";
int MqttServerPort = 1883;
OpenHouseMqtt Mqtt;

DHT dht(4, 22);
char SensorTempTopic[] = "/Test/Temperature";
OpenHouseMqttSensor SensorTemp(&Mqtt, SensorTempTopic);
char SensorHumTopic[] = "/Test/Humidity";
OpenHouseMqttSensor SensorHum(&Mqtt, SensorHumTopic);

char Relay1Topic[] = "/Test/Termostat/Relay";
void (Relay1On(char *message));
OpenHouseMqttRelay Relay1(&Mqtt, Relay1Topic, &Relay1On);
void Relay1On(char *message)
{
    if (message[0] == '1')
    {
        digitalWrite(2, HIGH);
    }
    else if (message[0] == '0')
    {
        digitalWrite(2, LOW);
    }
}


void subscribe()
{
    Relay1.Subscribe();
}

void HandleMeeesgae(char *topic, byte *payload, unsigned int length)
{
    char bufer[200] = "";
    for (unsigned int i = 0; i < length; i++)
    {
        bufer[i] = (char)payload[i];
    }
    Relay1.Recive(bufer, topic);
}

void setup()
{
    Serial.begin(9600);
    dht.begin();
    pinMode(2, OUTPUT);
    Mqtt.Begin(&MqttEthernetClient, &subscribe, &HandleMeeesgae, MqttServerAdress, &MqttServerPort, MqttClientId);
    Network.Begin(ethernetMac);
}

unsigned long int millissss = 0;
float Temperature;
float Humidity;

void loop()
{
    Network.Loop();
    Mqtt.Loop();

    if (millis() - millissss >= 5000)
    {
        Temperature = dht.readTemperature();
        Humidity = dht.readHumidity();
        char buf[20];
        dtostrf(Temperature, 3, 3, buf);
        SensorTemp.Send(buf);
        dtostrf(Humidity, 3, 3, buf);
        SensorHum.Send(buf);
        millissss = millis();
    }
    else if (millis() - millissss < 0)
    {
        millissss = millis();
    }
}