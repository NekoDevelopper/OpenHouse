#pragma once
#include <Arduino.h>
#include "OpenHouseMqtt.h"

class OHMqttDevice
{
protected:
    char *mqttTopic;
};
class OHMqttSensor : protected OHMqttDevice
{
private:
public:
    OHMqttSensor(char *topic);
    void Send(char *message);
};
class OHMqttRelay : protected OHMqttDevice
{
private:
    void (*func)(char *message);

public:
    OHMqttRelay(char *topic, void(func)(char *message));
    void Recive(char *message, char *topic);
    void Subscribe();
};
