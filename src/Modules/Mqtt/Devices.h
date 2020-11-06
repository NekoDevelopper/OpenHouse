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
    private:
        void (*func)(char *message);
    public:
        OpenHouseMqttRelay(OpenHouseMqtt *mqtt, char *topic,void (func)(char *message));
        void Recive(char *message, char *topic);
        void Subscribe();
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
//Relay
OpenHouseMqttRelay::OpenHouseMqttRelay(OpenHouseMqtt *mqtt, char *topic, void (Func)(char *message)){
    OpenHouseMqttRelay::func=Func;
    OpenHouseMqttRelay::OpenHouseMqttDevice::mqttTopic=topic;
    OpenHouseMqttRelay::OpenHouseMqttDevice::Mqtt=mqtt;
}
void OpenHouseMqttRelay::Recive(char *message, char *topic){
    if(*topic==*OpenHouseMqttRelay::OpenHouseMqttDevice::mqttTopic)
        Serial.print(topic);
        Serial.print("=>");
        Serial.println(message);
        OpenHouseMqttRelay::func(message);
    
}
void OpenHouseMqttRelay::Subscribe(){
    Mqtt->Subscribe(OpenHouseMqttRelay::OpenHouseMqttDevice::mqttTopic);
}