#include <Arduino.h>
#include "Modules/Ethernet/OpenHouseNet.h"
#include "Modules/Mqtt/OpenHouseMqtt.h"
#include <DHT.h>

byte ethernetMac[6] = {0x5A,0x12,0xF0,0xAF,0xAD,0x71}; // Mac Adress 
char MqttServerAdress[] = "192.168.1.14"; // Mqtt ip adress
char MqttClientId[] = "Test"; // MQTT Client ID
int MqttServerPort = 1883; // MQTT Server Port

DHT dht(3, 22); // Create DHT Sersor object
OHMqttSender HumiditySender((char*)"/Test/Humidity"); // Create MQTT Sender Object
OHMqttSender TemperatureSender((char*)"/Test/Temperature"); // Create MQTT Sender Object

void RelayFunc(char *message){digitalWrite(2,atoi(message));} // Create Relay Switch Function
OHMqttHandler Relay((char*)"/Test/Relay",&RelayFunc); // Create MQTT Handler

void setup()
{
    Serial.begin(9600); //Begining Arduino Serial Interface
    dht.begin(); //Begining DHT sensor
    pinMode(2, OUTPUT); //Configure Hardware pin 2 to OUTPUT
    OHMqtt.Begin(MqttServerAdress, &MqttServerPort, MqttClientId); //Begining MQTT
    OHNet.Begin(ethernetMac); //Begining Ethernet
}

unsigned long int millissss = 0; //Create Timer property
float Temperature; //Create Temperature property
float Humidity;//Create Humidity property

void loop()
{
    OHNet.Loop(); // Call Ethernet loop function
    OHMqtt.Loop(); // Call MQTT loop function

    if (millis() - millissss >= 5000)
    {
        Temperature = dht.readTemperature(); //Read temperature from DHT sensor
        Humidity = dht.readHumidity(); //Read Humidity from DHT Sensor
        char buf[20]; 
        dtostrf(Temperature, 3, 3, buf); // Float-> Char
        TemperatureSender.send(buf); // Send Temperature 
        dtostrf(Humidity, 3, 3, buf); // Float-> Char
        HumiditySender.send(buf); //Send Humidity
        millissss = millis();
    }
    else if (millis() - millissss < 0)
    {
        millissss = millis();
    }
}
