#include "OpenHouseMqtt.h"
OHMqttClass OHMqtt=OHMqttClass();

void OHMqttClass::Reconnect(){
    static unsigned long int lastTry;
    if(millis()-lastTry>=mqttReconnectionTime*1000){
        if(OHMqttClass::Mqtt.connected()==0){
            Serial.print("Connection to MQTT server:");
            Serial.println(OHMqttClass::Mqtt.connect(mqttClientId));
            ReconnectFunc();
            lastTry=millis();
        }
    }
}

void OHMqttClass::Begin(void (*reconnectFunc)(void), void (*callbackFunc)(char* topic, byte* payload, unsigned int length), char *server, int *port, char *id, unsigned int retryTime){
    OHMqttClass::mqttPort=port;
    OHMqttClass::mqttServer=server;
    OHMqttClass::mqttClientId=id;
    OHMqttClass::mqttReconnectionTime=retryTime;
    OHMqttClass::ReconnectFunc=reconnectFunc;
    OHMqttClass::Mqtt.setClient(MqttClient);
    OHMqttClass::Mqtt.setServer(mqttServer,*mqttPort);
    OHMqttClass::Mqtt.setCallback(callbackFunc);
}

void OHMqttClass::Loop(){
    OHMqttClass::Mqtt.loop();
    OHMqttClass::Reconnect();
}
void OHMqttClass::Publish(char *topic, char *message){
    OHMqttClass::Mqtt.publish(topic,message);
}
void OHMqttClass::Subscribe(char *topic){
    OHMqttClass::Mqtt.subscribe(topic);    
}