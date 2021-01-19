#pragma once
#include <Arduino.h>
#include "../Ethernet/OpenHouseNet.h"
#include <PubSubClient.h>
#ifndef OHMqttHandlersCount
#define OHMqttHandlersCount 10
#endif

class OHMqttSender
{
    private:
        char* topic;
    public:
        OHMqttSender(char *Topic);
        void send(char *message);

};
class OHMqttHandler
{
    public:
        OHMqttHandler(char *Topic, void (*Func)(char *message));
        void (*func)(char *mesage);
        char *topic;
};

class OHMqttClass{
    private:
        static void CallbackFunc(char* topic, byte* payload, unsigned int length);
        char *mqttClientId;
        int *mqttPort;
        char *mqttServer;
        unsigned int mqttReconnectionTime;
        EthernetClient MqttClient;
        PubSubClient Mqtt;
        void Reconnect();

        
    public:
        void Begin(char *server, int *port, char *id, unsigned int retryTime=5);
        void Loop();
        void Publish(char *topic, char *message);
        void Subscribe(char *topic);
        void AddReciver(char *topic, void (*func)(char *message));
        OHMqttSender AddSender(char *topic);
};
extern OHMqttClass OHMqtt;