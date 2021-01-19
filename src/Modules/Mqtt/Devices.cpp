#include "Devices.h"
//SENSOR
OHMqttSensor::OHMqttSensor(char *topic)
{
    OHMqttSensor::OHMqttDevice::mqttTopic = topic;
}
void OHMqttSensor::Send(char *message)
{
    OHMqtt.Publish(OHMqttSensor::OHMqttDevice::mqttTopic, message);
    Serial.print(OHMqttSensor::OHMqttDevice::mqttTopic);
    Serial.print("<=");
    Serial.println(message);
}
//Relay
OHMqttRelay::OHMqttRelay(char *topic, void(Func)(char *message))
{
    OHMqttRelay::func = Func;
    OHMqttRelay::OHMqttDevice::mqttTopic = topic;
}
void OHMqttRelay::Recive(char *message, char *topic)
{
    bool sofpad = 1;
    for (int i; topic[i]; i++)
    {
        if (topic[i] != OHMqttRelay::OHMqttDevice::mqttTopic[i])
        {
            sofpad = 0;
            break;
        }
    }
    if (sofpad == 1)
    {
        Serial.print(topic);
        Serial.print("=>");
        Serial.println(message);
        OHMqttRelay::func(message);
    }
}
void OHMqttRelay::Subscribe()
{
    OHMqtt.Subscribe(OHMqttRelay::OHMqttDevice::mqttTopic);
}