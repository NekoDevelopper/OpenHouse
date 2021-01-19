#pragma once
#include <Arduino.h>
#include "../Ethernet/OpenHouseNet.h"
#include <PubSubClient.h>
class OHMqttClass{
    private:        
        void (*ReconnectFunc)();
        char *mqttClientId;
        int *mqttPort;
        char *mqttServer;
        unsigned int  mqttReconnectionTime;
        EthernetClient MqttClient;
        PubSubClient Mqtt;
        void Reconnect();
    public:
        void Begin(void (*reconnectFunc)(void), void (*callbackFunc)(char* topic, byte* payload, unsigned int length) , char *server, int *port, char *id, unsigned int retryTime=5);
        void Loop();
        void Publish(char *topic, char *message);
        void Subscribe(char *topic);

};
extern OHMqttClass OHMqtt;

