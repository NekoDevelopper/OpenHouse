#include <Arduino.h>
#include "OpenHouseMqtt.h"

class OpenHouseMqttDevice{
    protected:
        OpenHouseMqtt *Mqtt;
        char *mqttTopic;
};
class OpenHouseMqttSensor:protected OpenHouseMqttDevice{
    private:
        
    public:
        OpenHouseMqttSensor(OpenHouseMqtt *mqtt, char *topic);
        void Send(char *message);

};
class OpenHouseMqttRelay:protected OpenHouseMqttDevice{
    public:
        OpenHouseMqttRelay(OpenHouseMqtt *mqtt, char *topic);
        void Recive(void (func)(char *message));
};


//SENSOR
OpenHouseMqttSensor::OpenHouseMqttSensor(OpenHouseMqtt *mqtt, char *topic){
    OpenHouseMqttSensor::OpenHouseMqttDevice::mqttTopic=topic;
    OpenHouseMqttSensor::OpenHouseMqttDevice::Mqtt=mqtt;
}
void OpenHouseMqttSensor::Send(char *message){
    Mqtt->Publish(OpenHouseMqttSensor::OpenHouseMqttDevice::mqttTopic,message);
    Serial.print(OpenHouseMqttSensor::OpenHouseMqttDevice::mqttTopic);
    Serial.print("<=");
    Serial.println(message);
}